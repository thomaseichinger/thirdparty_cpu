
#ifndef HWTIMER_CPU_H_
#define HWTIMER_CPU_H_

#define ARCH_MAXTIMERS      4               ///< the cpu implementation supports 4 HW timers
#define HWTIMER_SPEED       1000000         ///< the HW timer runs with 1MHz
#define HWTIMER_MAXTICKS    (0xFFFFFFFF)    ///< 32-bit timer

#endif /* HWTIMER_CPU_H_ */