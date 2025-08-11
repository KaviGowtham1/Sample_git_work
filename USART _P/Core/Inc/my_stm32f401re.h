/*
 * my_stm32f401re.h
 *
 *  Created on: Jul 28, 2025
 *      Author: Kavigowtham
 */

#ifndef MY_STM32F401RE_H
#define MY_STM32F401RE_H

#include <stdint.h>

#define RCC_BASE       0x40023800
#define GPIOA_BASE     0x40020000
#define USART2_BASE    0x40004400

#define RCC_AHB1ENR    (*(volatile uint32_t*)(RCC_BASE + 0x30))
#define RCC_APB1ENR    (*(volatile uint32_t*)(RCC_BASE + 0x40))

#define GPIOA_MODER    (*(volatile uint32_t*)(GPIOA_BASE + 0x00))
#define GPIOA_AFRL     (*(volatile uint32_t*)(GPIOA_BASE + 0x20))

#define USART2_SR      (*(volatile uint32_t*)(USART2_BASE + 0x00))
#define USART2_DR      (*(volatile uint32_t*)(USART2_BASE + 0x04))
#define USART2_BRR     (*(volatile uint32_t*)(USART2_BASE + 0x08))
#define USART2_CR1     (*(volatile uint32_t*)(USART2_BASE + 0x0C))

#define NVIC_ISER1     (*(volatile uint32_t*)(0xE000E104))  // Interrupt Set Enable Register 1
#define USART2_IRQn    38


#endif
