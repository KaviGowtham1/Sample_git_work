#include <stdint.h>
#include "stm32f401re_custom.h"

void delay(void) {
    for (volatile int i = 0; i < 50000; i++);
}

// ----- I2C Initialization -----
void I2C1_Init(void) {
    // 1. Enable clocks
    RCC_AHB1ENR |= (1 << 1);     // Enable GPIOB clock
    RCC_APB1ENR |= (1 << 21);    // Enable I2C1 clock

    // 2. Configure PB6 & PB7 as AF4 (I2C1)
    GPIOB_MODER &= ~((3 << 12) | (3 << 14));      // Clear mode bits
    GPIOB_MODER |= (2 << 12) | (2 << 14);         // Alternate function
    GPIOB_OTYPER |= (1 << 6) | (1 << 7);          // Open-drain
    GPIOB_OSPEEDR |= (3 << 12) | (3 << 14);       // High speed
    GPIOB_AFRL &= ~((0xF << 24) | (0xF << 28));   // Clear AFR
    GPIOB_AFRL |= (4 << 24) | (4 << 28);          // AF4 for I2C1

    // 3. Configure I2C timing
    I2C1_CR1 &= ~(1 << 0);     // Disable PE
    I2C1_CR2 = 16;             // APB1 = 16MHz
    I2C1_CCR = 80;             // Standard mode, 100kHz
    I2C1_TRISE = 17;           // (1000ns / (1/16MHz)) + 1 = 17
    I2C1_CR1 |= (1 << 0);      // Enable PE
}

// ----- I2C Write -----
void I2C1_Write(uint8_t slave_addr, uint8_t reg_addr, uint8_t data) {
    I2C1_CR1 |= (1 << 8); // START
    while (!(I2C1_SR1 & (1 << 0))); // SB

    I2C1_DR = (slave_addr << 1); // Write
    while (!(I2C1_SR1 & (1 << 1))); // ADDR
    (void)I2C1_SR1; (void)I2C1_SR2;

    I2C1_DR = reg_addr;
    while (!(I2C1_SR1 & (1 << 7))); // TXE

    I2C1_DR = data;
    while (!(I2C1_SR1 & (1 << 7))); // TXE
    while (!(I2C1_SR1 & (1 << 2))); // BTF

    I2C1_CR1 |= (1 << 9); // STOP
}

// ----- I2C Read -----
uint8_t I2C1_Read(uint8_t slave_addr, uint8_t reg_addr) {
    uint8_t data;

    I2C1_CR1 |= (1 << 8); // START
    while (!(I2C1_SR1 & (1 << 0))); // SB

    I2C1_DR = (slave_addr << 1); // Write
    while (!(I2C1_SR1 & (1 << 1))); // ADDR
    (void)I2C1_SR1; (void)I2C1_SR2;

    I2C1_DR = reg_addr;
    while (!(I2C1_SR1 & (1 << 7))); // TXE

    I2C1_CR1 |= (1 << 8); // Repeated START
    while (!(I2C1_SR1 & (1 << 0))); // SB

    I2C1_DR = (slave_addr << 1) | 1; // Read
    while (!(I2C1_SR1 & (1 << 1))); // ADDR
    (void)I2C1_SR1; (void)I2C1_SR2;

    I2C1_CR1 &= ~(1 << 10); // Clear ACK
    I2C1_CR1 |= (1 << 9);   // STOP

    while (!(I2C1_SR1 & (1 << 6))); // RXNE
    data = I2C1_DR;

    return data;
}

int main(void) {
    uint8_t received = 0;

    I2C1_Init();

    delay();
    I2C1_Write(0x50, 0x10, 0xAB);  // Slave=0x50, Register=0x10, Data=0xAB
    delay();
    received = I2C1_Read(0x50, 0x10); // Read back from 0x10

    while (1);
}
