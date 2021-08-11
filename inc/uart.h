#ifndef _uart_h_
#define _uart_h_
#include "main.h"
#include "stm8s_uart1.h"
//Variables
extern uint8_t RXBuff[BLOCK_SIZE];
extern uint8_t u8CountRecieve;
enum uart_state{
  empty = 0, 
  recieve = 1
};
//Function block
void            vUART_Config(void);
uint8_t         u8UART_Recieve(void);
void            vUART_Transmit(uint8_t data);

#endif