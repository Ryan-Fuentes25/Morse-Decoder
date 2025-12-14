/**
 * @file UART.c
 *
 * @brief REVISED Source code for the UART driver (UART0) set to 115200 Baud.
 *
 * Initializes UART0 for Stellaris ICDI Virtual COM Port communication.
 *
 * @author Ryan Fuentes
 */

#include "TM4C123GH6PM.h"

void UART0_Init(void) {
    // 1. Enable the clock for UART0 and GPIOA
    SYSCTL->RCGCUART |= 0x01; // UART0
    SYSCTL->RCGCGPIO |= 0x01; // Port A
    
    // ... Wait for the clocks to stabilize ...
    while((SYSCTL->PRUART & 0x01) == 0);
    while((SYSCTL->PRGPIO & 0x01) == 0);
    
    // 2. Disable the UART during configuration
    UART0->CTL &= ~0x00000001; 
    
    // 3. Set the Baud Rate: 115200 for 50 MHz System Clock
    // IBRD = 27
    // FBRD = 8
    UART0->IBRD = 27;
    UART0->FBRD = 8;
    
    // 4. Configure the line control: 8-bit, No Parity, 1 Stop bit, Enable FIFO
    // Set WLEN=0x3 (8 bits), FEN=0x1 (FIFO enable)
    UART0->LCRH = (0x3 << 5) | (0x1 << 4); 
    
    // 5. Enable UART, Transmit (TXE), and Receive (RXE)
    UART0->CTL |= 0x301; 
    
    // 6. Configure GPIO Port A pins PA1 (U0Tx) and PA0 (U0Rx)
    GPIOA->AFSEL |= 0x03; 
    GPIOA->PCTL = (GPIOA->PCTL & 0xFFFFFF00) | 0x00000011; 
    GPIOA->DEN |= 0x03; 
}

void UART0_OutChar(char data) {
    // Wait until the transmit FIFO is not full (TXFF flag is 0, Bit 5)
    while((UART0->FR & 0x20) != 0);
    
    // Write data to the Data Register
    UART0->DR = data;
}