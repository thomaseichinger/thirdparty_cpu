/*
 * cpu.c
 *
 *  Created on: 07.09.2012
 *      Author: pfeiffer
 */
#include <stdio.h>
#include <stdint.h>
#include "stm32f10x_tim.h"
#include "stm32f10x.h"

#define ENABLE_DEBUG (0)
#include "debug.h"

int inISR(void)
{
    return (__get_IPSR() & 0xFF);
}

unsigned int disableIRQ(void)
{
    // FIXME PRIMASK is the old CPSR (FAULTMASK ??? BASEPRI ???)
    //PRIMASK lesen
    unsigned int uiPriMask = __get_PRIMASK();
    __disable_irq();
    return uiPriMask;
}

void restoreIRQ(unsigned oldPRIMASK)
{
    //PRIMASK lesen setzen
    __set_PRIMASK(oldPRIMASK);
}


__attribute__((naked))
void HardFault_Handler(void)
{
    puts("HARD FAULT");

    while (1);
}

__attribute__((naked))
void BusFault_Handler(void)
{
    puts("BusFault_Handler");

    while (1);
}

__attribute__((naked))
void Usage_Handler(void)
{
    puts("Usage FAULT");

    while (1);
}

__attribute__((naked))
void WWDG_Handler(void)
{
    puts("WWDG FAULT");

    while (1);
}

__attribute__((naked))
void NMI_Handler(void)
{
    DEBUG(__PRETTY_FUNCTION__);

    while (1);
}

__attribute__((naked))
void MemManage_Handler(void)
{
    DEBUG(__PRETTY_FUNCTION__);

    while (1);
}

__attribute__((naked))
void DebugMon_Handler(void)
{
    DEBUG(__PRETTY_FUNCTION__);

    while (1);
}

__attribute__((naked))
void SysTick_Handler(void)
{
    DEBUG(__PRETTY_FUNCTION__);

    while (1);
}

extern void rtc_handler(void);
extern void rtcalarm_handler(void);
extern void spi1_handler(void);
extern void exti4_handler(void);

void RTC_IRQHandler(void)
{
    DEBUG("%s\n",__PRETTY_FUNCTION__);
    rtc_handler();
    // if(RTC_GetFlagStatus(RTC_IT_ALR) != RESET)
//     {
//       RTC_ClearITPendingBit(RTC_IT_ALR);      //Clear RTC Alarm interrupt pending bit
//       RTC_WaitForLastTask();                  //Wait until last write operation on RTC registers has finished
//     }
}

void RTCAlarm_IRQHandler(void)
{
    DEBUG("%s 1\n",__PRETTY_FUNCTION__);
    rtcalarm_handler();
    DEBUG("%s 2\n",__PRETTY_FUNCTION__);
  // // debugpins_isr_set();
//   if(EXTI_GetITStatus(EXTI_Line17) != RESET)
//   {
//     EXTI_ClearITPendingBit(EXTI_Line17);
//         radiotimer_isr();
//   }
//   // debugpins_isr_clr();
}

void SPI1_IRQHandler(void)
{
    DEBUG("%s\n",__PRETTY_FUNCTION__);
    spi1_handler();
  // // debugpins_isr_set();
//   if(SPI_I2S_GetFlagStatus(SPI1,SPI_I2S_FLAG_RXNE) != RESET)
//   {
//     spi_isr();
//   }
//   // debugpins_isr_clr();
}

void EXTI4_IRQHandler(void)
{
    DEBUG("%s\n",__PRETTY_FUNCTION__);
    exti4_handler();
  // if(EXTI_GetITStatus(EXTI_Line4) != RESET){
//     // debugpins_isr_set();
// 
//     //leds_error_toggle();
//     EXTI_ClearITPendingBit(EXTI_Line4);
// 
//     //RCC_Wakeup();
//     radio_isr();
// 
//     // debugpins_isr_clr();
//   }
}
