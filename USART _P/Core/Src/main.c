#include "my_stm32f401re.h"

#define BUFFER_SIZE 100
char buffer[BUFFER_SIZE];
volatile int i = 0;

void usart2_init(void);
void usart2_write(char ch);
void usart2_print(const char *str);
void USART2_IRQHandler(void);

int main(void)
{
    usart2_init();

    while (1)
    {
        // Main loop can be empty, UART handled in ISR
    }
}

void usart2_init(void)
{
    // 1. Enable GPIOA and USART2 clocks
    RCC_AHB1ENR |= (1 << 0);      // GPIOA clock enable
    RCC_APB1ENR |= (1 << 17);     // USART2 clock enable

    // 2. Configure PA2 and PA3 to AF mode (10)
    GPIOA_MODER &= ~((3 << (2 * 2)) | (3 << (3 * 2)));
    GPIOA_MODER |= ((2 << (2 * 2)) | (2 << (3 * 2)));

    // 3. Set AF7 (USART2) for PA2 and PA3
    GPIOA_AFRL &= ~((0xF << (4 * 2)) | (0xF << (4 * 3)));
    GPIOA_AFRL |= ((7 << (4 * 2)) | (7 << (4 * 3)));

    // 4. Set baud rate for 9600 @ 16 MHz PCLK
    USART2_BRR = (1666<<4)|11;

    // 5. Enable RXNEIE (RX interrupt), UE, TE, RE
    USART2_CR1 = (1 << 13) | (1 << 3) | (1 << 2) | (1 << 5);

    // 6. Enable USART2 interrupt in NVIC
    NVIC_ISER1 |= (1 << (USART2_IRQn - 32)); // USART2_IRQn = 38
}

void usart2_write(char ch)
{
    while (!(USART2_SR & (1 << 7))); // Wait for TXE
    USART2_DR = (ch & 0xFF);
}

void usart2_print(const char *str)
{
    while (*str)
    {
        usart2_write(*str++);
    }
}

// USART2 Interrupt Service Routine
void USART2_IRQHandler(void)
{
    if (USART2_SR & (1 << 5)) // RXNE set
    {
        char c = USART2_DR & 0xFF; // Read data

        usart2_write(c); // Echo back

        if (c == '\r')  // Enter key pressed
        {
            buffer[i] = '\0'; // Null-terminate
            usart2_print("\r\nYou typed: ");
            usart2_print(buffer);
            usart2_print("\r\n");
            i = 0; // Reset index
        }
        else if (i < BUFFER_SIZE - 1)
        {
            buffer[i++] = c; // Store char
        }
    }
}
