/*
 * Copyright (C) 2013 Freie Universität Berlin
 *
 * This file is subject to the terms and conditions of the GNU Lesser General
 * Public License. See the file LICENSE in the top level directory for more
 * details.
 */

/**
 * @ingroup     cpu_stm32f303vg
 * @{
 *
 * @file        hwtimer_arch.c
 * @brief       Implementation of the kernels hwtimer interface
 *
 * The hardware timer implementation uses the Coretex build-in system timer as backend.
 *
 * @author      Hauke Petersen <hauke.petersen@fu-berlin.de>
 *
 * @}
 */

#include <stdint.h>
#include <stddef.h>

#include "hwtimer_arch.h"
#include "board.h"
#include "timer.h"
#include "thread.h"

#define ENABLE_DEBUG (1)
#include "debug.h"

void irq_handler(int channel);
void (*timeout_handler)(int);


void hwtimer_arch_init(void (*handler)(int), uint32_t fcpu)
{
    timeout_handler = handler;
    timer_init(HW_TIMER, 1, &irq_handler);
}

void hwtimer_arch_enable_interrupt(void)
{
    timer_irq_enable(HW_TIMER);
}

void hwtimer_arch_disable_interrupt(void)
{
    timer_irq_disable(HW_TIMER);
}

void hwtimer_arch_set(unsigned long offset, short timer)
{
    timer_set(HW_TIMER, timer + 1, offset);
}

void hwtimer_arch_set_absolute(unsigned long value, short timer)
{
    // will not be implemented
}

void hwtimer_arch_unset(short timer)
{
    timer_clear(HW_TIMER, timer + 1);
}

unsigned long hwtimer_arch_now(void)
{
    return timer_read(HW_TIMER);
}

void irq_handler(int channel)
{     
    timeout_handler((short)(channel - 1));
    thread_yield();
}


// #include "cpu.h"
// #include "sched.h"
// #include "hwtimer.h"
// 
// #include "board.h"
// 
// #define ENABLE_DEBUG    (1)
// #include "debug.h"
// 
// static void (*int_handler)(int);
// 
// unsigned long hwtimer_arch_now(void)
// {
//     return TIM_GetCounter(TIM2);
// }
// 
// /*---------------------------------------------------------------------------*/
// 
// __attribute__((naked))
// void TIM2_IRQHandler(void)
// {
//     asm("push   {LR}");
//     //save_context();
//     puts("fired");
//     if(TIM_GetITStatus(TIM2, TIM_IT_CC1) != RESET)
//     {
//         DEBUG("Firing 0 at %lu\n", hwtimer_now());
//         int_handler(0);
//         TIM_ClearITPendingBit(TIM2, TIM_IT_CC1);
//         TIM_ITConfig(TIM2, TIM_IT_CC1, DISABLE);
//     }
//     if(TIM_GetITStatus(TIM2, TIM_IT_CC2) != RESET)
//     {
//         DEBUG("Firing 1 at %lu\n", hwtimer_now());
//         int_handler(1);
//         TIM_ClearITPendingBit(TIM2, TIM_IT_CC2);
//         TIM_ITConfig(TIM2, TIM_IT_CC2, DISABLE);
//     }
//     if(TIM_GetITStatus(TIM2, TIM_IT_CC3) != RESET)
//     {
//         DEBUG("Firing 2 at %lu\n", hwtimer_now());
//         int_handler(2);
//         TIM_ClearITPendingBit(TIM2, TIM_IT_CC3);
//         TIM_ITConfig(TIM2, TIM_IT_CC3, DISABLE);
//     }
//     if(TIM_GetITStatus(TIM2, TIM_IT_CC4) != RESET)
//     {
//         DEBUG("Firing 3 at %lu\n", hwtimer_now());
//         int_handler(3);
//         TIM_ClearITPendingBit(TIM2, TIM_IT_CC4);
//         TIM_ITConfig(TIM2, TIM_IT_CC4, DISABLE);
//     }
// 
//     if (sched_context_switch_request) {
//         sched_run();
//     }
//     //restore_context();
//     asm("pop    {r0}");
//     asm("bx     r0");
// }
// 
// __attribute__((naked))
// void TIM3_IRQHandler(void)
// {
//     asm("push   {LR}");
//     // save_context();
// 
//     if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET) {
//         TIM_ClearITPendingBit(TIM3, TIM_IT_Update);
// 
//         if (!(TIM3->CR1 & TIM_CR1_CEN)) {
//             RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, DISABLE);
//             int_handler(1);
//         }
// 
//         if (sched_context_switch_request) {
//             // scheduler
//             sched_run();
//         }
//     }
// 
//     // restore_context();
//     asm("pop    {r0}");
//     asm("bx     r0");
// }
// 
// /*---------------------------------------------------------------------------*/
// 
// void hwtimer_arch_init(void (*handler)(int), uint32_t fcpu)
// {
//     DEBUG("hwtimer : arch_init\n");
//     TIM_TimeBaseInitTypeDef tim_init;
//     NVIC_InitTypeDef nvic_init;
//     TIM_OCInitTypeDef tim_ocinit;
// 
//     // init generic timer options
//     tim_init.TIM_ClockDivision = TIM_CKD_DIV1;
//     tim_init.TIM_CounterMode = TIM_CounterMode_Up;
// 
//     // setup the interrupt controller
//     nvic_init.NVIC_IRQChannelCmd = ENABLE;
//     nvic_init.NVIC_IRQChannelSubPriority = 0;
// 
//     // set callback routine
//     int_handler = handler;
//     
//     // enable clocks
//     RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
//     // timer init
//     tim_init.TIM_Period = 0xffff;
//     tim_init.TIM_Prescaler = 2303;
//     tim_init.TIM_ClockDivision = 0;
//     tim_init.TIM_CounterMode = TIM_CounterMode_Up;
//     TIM_TimeBaseInit(TIM2, &tim_init);
//     
//     tim_ocinit.TIM_OCMode = TIM_OCMode_Toggle;
//     tim_ocinit.TIM_Pulse = 0;
//     tim_ocinit.TIM_OutputState = TIM_OutputState_Enable;
//     tim_ocinit.TIM_OCPolarity = TIM_OCPolarity_High;
//     TIM_OC1Init(TIM2, &tim_ocinit);
//     
//     // irq setup
//     nvic_init.NVIC_IRQChannel = TIM2_IRQn;
//     nvic_init.NVIC_IRQChannelPreemptionPriority = 1;
//     nvic_init.NVIC_IRQChannelCmd = ENABLE;
//     NVIC_Init(&nvic_init);
//     // enable timer
//     TIM_Cmd(TIM2, ENABLE);
// }
// 
// /*---------------------------------------------------------------------------*/
// 
// void hwtimer_arch_enable_interrupt(void)
// {
//     NVIC_EnableIRQ(TIM2_IRQn);
// }
// 
// /*---------------------------------------------------------------------------*/
// 
// void hwtimer_arch_disable_interrupt(void)
// {
//     NVIC_DisableIRQ(TIM2_IRQn);
// }
// 
// /*---------------------------------------------------------------------------*/
// 
// void hwtimer_arch_set_absolute(unsigned long value, short timer)
// {
//     DEBUG("set absolute timer %hu to %lu\n", timer, value);
//     switch (timer) {
//         case 0:
//             TIM_SetCompare1(TIM2, value);
//             TIM_ClearFlag(TIM2, TIM_FLAG_CC1);
//             TIM_ITConfig(TIM2, TIM_IT_CC1, ENABLE);
//             break;
//         case 1:
//             TIM_SetCompare2(TIM2, value);
//             TIM_ClearFlag(TIM2, TIM_FLAG_CC2);
//             TIM_ITConfig(TIM2, TIM_IT_CC2, ENABLE);
//             break;
//         case 2:
//             TIM_SetCompare3(TIM2, value);
//             TIM_ClearFlag(TIM2, TIM_FLAG_CC3);
//             TIM_ITConfig(TIM2, TIM_IT_CC3, ENABLE);
//             break;
//         case 3:
//             TIM_SetCompare4(TIM2, value);
//             TIM_ClearFlag(TIM2, TIM_FLAG_CC4);
//             TIM_ITConfig(TIM2, TIM_IT_CC4, ENABLE);
//             break;
//     }
// }
// 
// /*---------------------------------------------------------------------------*/
// 
// void hwtimer_arch_set(unsigned long offset, short timer)
// {
//     unsigned long now = hwtimer_now();
//     DEBUG("set timer %hu to %lu + %lu\n", timer, now, offset);
//     switch (timer) {
//         case 0:
//             TIM_SetCompare1(TIM2, now + offset- 1);
//             TIM_ClearFlag(TIM2, TIM_FLAG_CC1);
//             TIM_ITConfig(TIM2, TIM_IT_CC1, ENABLE);
//             break;
//         case 1:
//             TIM_SetCompare2(TIM2, now + offset- 1);
//             TIM_ClearFlag(TIM2, TIM_FLAG_CC2);
//             TIM_ITConfig(TIM2, TIM_IT_CC2, ENABLE);
//             break;
//         case 2:
//             TIM_SetCompare3(TIM2, now + offset- 1);
//             TIM_ClearFlag(TIM2, TIM_FLAG_CC3);
//             TIM_ITConfig(TIM2, TIM_IT_CC3, ENABLE);
//             break;
//         case 3:
//             TIM_SetCompare4(TIM2, now + offset- 1);
//             TIM_ClearFlag(TIM2, TIM_FLAG_CC4);
//             TIM_ITConfig(TIM2, TIM_IT_CC4, ENABLE);
//             break;
//     }
// }
// 
// /*---------------------------------------------------------------------------*/
// 
// void hwtimer_arch_unset(short timer)
// {
//     switch (timer) {
//         case 0:
//             TIM_SetCompare1(TIM2,0);
//             TIM_ITConfig(TIM2, TIM_IT_CC1, DISABLE);
//             break;
//         case 1:
//             TIM_SetCompare2(TIM2,0);
//             TIM_ITConfig(TIM2, TIM_IT_CC2, DISABLE);
//             break;
//         case 2:
//             TIM_SetCompare3(TIM2,0);
//             TIM_ITConfig(TIM2, TIM_IT_CC3, DISABLE);
//             break;
//         case 3:
//             TIM_SetCompare4(TIM2,0);
//             TIM_ITConfig(TIM2, TIM_IT_CC4, DISABLE);
//             break;
//     }
// }
// /*---------------------------------------------------------------------------*/
