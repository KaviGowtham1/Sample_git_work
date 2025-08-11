/*
 * stm32custom_spi.h
 *
 *  Created on: Jul 30, 2025
 *      Author: Kavigowtham
 */

#ifndef STM32F401RE_CUSTOM_H
#define STM32F401RE_CUSTOM_H

#include <stdint.h>

/* ========== Base Addresses ========== */
#define PERIPH_BASE           0x40000000UL
#define AHB1PERIPH_BASE       (PERIPH_BASE + 0x00020000UL)
#define APB1PERIPH_BASE       (PERIPH_BASE + 0x00000000UL)
#define APB2PERIPH_BASE       (PERIPH_BASE + 0x00010000UL)

/* ========== RCC ========== */
#define RCC_BASE              (AHB1PERIPH_BASE + 0x3800UL)
#define RCC_AHB1ENR           (*(volatile uint32_t *)(RCC_BASE + 0x30))
#define RCC_APB1ENR           (*(volatile uint32_t *)(RCC_BASE + 0x40))
#define RCC_APB2ENR           (*(volatile uint32_t *)(RCC_BASE + 0x44))

/* ========== GPIO ========== */
typedef struct {
    volatile uint32_t MODER;
    volatile uint32_t OTYPER;
    volatile uint32_t OSPEEDR;
    volatile uint32_t PUPDR;
    volatile uint32_t IDR;
    volatile uint32_t ODR;
    volatile uint32_t BSRR;
    volatile uint32_t LCKR;
    volatile uint32_t AFRL;
    volatile uint32_t AFRH;
} GPIO_TypeDef;

#define GPIOA                 ((GPIO_TypeDef *)(AHB1PERIPH_BASE + 0x0000))
#define GPIOB                 ((GPIO_TypeDef *)(AHB1PERIPH_BASE + 0x0400))
#define GPIOC                 ((GPIO_TypeDef *)(AHB1PERIPH_BASE + 0x0800))

/* ========== USART2 ========== */
typedef struct {
    volatile uint32_t SR;
    volatile uint32_t DR;
    volatile uint32_t BRR;
    volatile uint32_t CR1;
    volatile uint32_t CR2;
    volatile uint32_t CR3;
    volatile uint32_t GTPR;
} USART_TypeDef;

#define USART2                ((USART_TypeDef *)(APB1PERIPH_BASE + 0x4400))

/* ========== SPI1 ========== */
typedef struct {
    volatile uint32_t CR1;
    volatile uint32_t CR2;
    volatile uint32_t SR;
    volatile uint32_t DR;
    volatile uint32_t CRCPR;
    volatile uint32_t RXCRCR;
    volatile uint32_t TXCRCR;
    volatile uint32_t I2SCFGR;
    volatile uint32_t I2SPR;
} SPI_TypeDef;

#define SPI1                  ((SPI_TypeDef *)(APB2PERIPH_BASE + 0x3000))

/* ========== Bit Positions ========== */
// SPI_SR bits
#define SPI_SR_RXNE           (1 << 0)
#define SPI_SR_TXE            (1 << 1)

// USART_SR bits
#define USART_SR_TXE          (1 << 7)
#define USART_SR_RXNE         (1 << 5)


#endif // STM32F401RE_CUSTOM_H
