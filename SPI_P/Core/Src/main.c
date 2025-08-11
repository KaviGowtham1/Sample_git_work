#include "stm32f401re_custom.h"

void delay(void) {
    for (volatile int i = 0; i < 50000; i++);
}

// ----------- SPI1 GPIO Initialization -----------
void SPI1_GPIO_Init(void) {
    RCC_AHB1ENR |= (1 << 0);  // Enable GPIOA

    GPIOA->MODER &= ~(0x3F << (5 * 2));    // Clear MODER5,6,7
    GPIOA->MODER |=  (0x2A << (5 * 2));    // Set to AF mode

    GPIOA->AFRL &= ~((0xF << (4 * 5)) | (0xF << (4 * 6)) | (0xF << (4 * 7)));
    GPIOA->AFRL |=  ((5 << (4 * 5)) | (5 << (4 * 6)) | (5 << (4 * 7)));
}

// ----------- SPI1 NSS GPIO Initialization -----------
void SPI1_NSS_GPIO_Init(void) {
    RCC_AHB1ENR |= (1 << 1);  // Enable GPIOB
    RCC_AHB1ENR |= (1 << 2);  // Enable GPIOC

    // PA4 - NSS1
    GPIOA->MODER &= ~(3 << (4 * 2));
    GPIOA->MODER |=  (1 << (4 * 2));   // Output
    GPIOA->ODR |= (1 << 4);            // High

    // PB0 - NSS2
    GPIOB->MODER &= ~(3 << (0 * 2));
    GPIOB->MODER |=  (1 << (0 * 2));
    GPIOB->ODR |= (1 << 0);

    // PC1 - NSS3
    GPIOC->MODER &= ~(3 << (1 * 2));
    GPIOC->MODER |=  (1 << (1 * 2));
    GPIOC->ODR |= (1 << 1);
}

// ----------- SPI1 Peripheral Init -----------
void SPI1_Init(void) {
    RCC_APB2ENR |= (1 << 12);  // Enable SPI1

    SPI1->CR1 = 0;
    SPI1->CR1 |= (1 << 2);     // Master
    SPI1->CR1 |= (7 << 3);     // fPCLK/256 (slow)
    SPI1->CR1 &= ~(1 << 11);   // 8-bit
    SPI1->CR1 &= ~(1 << 10);   // Full duplex
    SPI1->CR1 &= ~(1 << 1);    // CPOL = 0
    SPI1->CR1 &= ~(1 << 0);    // CPHA = 0
    SPI1->CR1 |= (1 << 6);     // SPI enable
}

// ----------- SPI1 Send & Receive Dummy Byte -----------
uint8_t SPI1_TransmitReceive(uint8_t data) {
    while (!(SPI1->SR & (1 << 1)));  // Wait TXE
    SPI1->DR = data;
    while (!(SPI1->SR & (1 << 0)));  // Wait RXNE
    return SPI1->DR;
}

// ----------- USART2 GPIO Init (PA2) -----------
void USART2_GPIO_Init(void) {
    RCC_AHB1ENR |= (1 << 0);  // GPIOA

    GPIOA->MODER &= ~(3 << (2 * 2));
    GPIOA->MODER |=  (2 << (2 * 2));   // AF mode

    GPIOA->AFRL &= ~(0xF << (4 * 2));
    GPIOA->AFRL |=  (7   << (4 * 2));  // AF7 for USART2
}

// ----------- USART2 Peripheral Init -----------
void USART2_Init(void) {
    RCC_APB1ENR |= (1 << 17);  // Enable USART2

    USART2->BRR = 16000000 / 9600;  // Baud rate (assuming 16 MHz)
    USART2->CR1 |= (1 << 3);        // TE: Transmit enable
    USART2->CR1 |= (1 << 13);       // UE: USART enable
}

// ----------- USART2 Transmit One Byte -----------
void USART2_Transmit(uint8_t data) {
    while (!(USART2->SR & (1 << 7)));  // Wait TXE
    USART2->DR = data;
}

// ----------- USART2 Print String -----------
void USART2_Print(char *str) {
    while (*str) {
        USART2_Transmit(*str++);
    }
}

// ----------- Print Hex Format (1 byte) -----------
void print_hex(uint8_t value) {
    char hex[3];
    hex[0] = "0123456789ABCDEF"[value >> 4];
    hex[1] = "0123456789ABCDEF"[value & 0x0F];
    hex[2] = '\0';
    USART2_Print(hex);
}

// ----------- NSS Control -----------
void NSS_Select(GPIO_TypeDef* port, uint8_t pin) {
    port->ODR &= ~(1 << pin);
}

void NSS_Deselect(GPIO_TypeDef* port, uint8_t pin) {
    port->ODR |= (1 << pin);
}

// ----------- Simulated SPI Slave Read Function -----------
void read_from_slave(GPIO_TypeDef* port, uint8_t pin, char* label, uint8_t simulated_response) {
    NSS_Select(port, pin);
    delay();

  /*  SPI1_TransmitReceive(0xFF);  // Just generate SPI clock
    delay(); */

    NSS_Deselect(port, pin);

    // Simulated response via UART
    USART2_Print(label);
    USART2_Print(": 0x");
    print_hex(simulated_response);
    USART2_Print("\r\n");
    delay();
}

// ----------- Main Function -----------
int main(void) {
    SPI1_GPIO_Init();
    SPI1_NSS_GPIO_Init();
    SPI1_Init();

    USART2_GPIO_Init();
    USART2_Init();

    USART2_Print("System Start\r\n");

    while (1) {
        read_from_slave(GPIOA, 4, "Slave 1", 0x3A);
        read_from_slave(GPIOB, 0, "Slave 2", 0x7F);
        read_from_slave(GPIOC, 1, "Slave 3", 0xC5);
        delay();
    }
}
