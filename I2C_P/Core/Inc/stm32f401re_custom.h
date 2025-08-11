/*
 * stm32f401re_custom.h
 *
 *  Created on: Jul 31, 2025
 *      Author: Kavigowtham
 */

#ifndef INC_STM32F401RE_CUSTOM_H_
#define INC_STM32F401RE_CUSTOM_H_

#define RCC_AHB1ENR   (*(volatile uint32_t*)0x40023830)
#define RCC_APB1ENR   (*(volatile uint32_t*)0x40023840)

#define GPIOB_MODER   (*(volatile uint32_t*)0x40020400)
#define GPIOB_OTYPER  (*(volatile uint32_t*)0x40020404)
#define GPIOB_OSPEEDR (*(volatile uint32_t*)0x40020408)
#define GPIOB_AFRL    (*(volatile uint32_t*)0x40020420)

#define I2C1_CR1      (*(volatile uint32_t*)0x40005400)
#define I2C1_CR2      (*(volatile uint32_t*)0x40005404)
#define I2C1_OAR1     (*(volatile uint32_t*)0x40005408)
#define I2C1_DR       (*(volatile uint32_t*)0x40005410)
#define I2C1_SR1      (*(volatile uint32_t*)0x40005414)
#define I2C1_SR2      (*(volatile uint32_t*)0x40005418)
#define I2C1_CCR      (*(volatile uint32_t*)0x4000541C)
#define I2C1_TRISE    (*(volatile uint32_t*)0x40005420)


#endif /* INC_STM32F401RE_CUSTOM_H_ */
