#ifndef _uart_h_
#define _uart_h_
#include "main.h"
#include "stm8s_uart1.h"
//Variables
extern uint8_t RXBuff[BLOCK_SIZE];
extern uint8_t u8CountRecieve;
extern uint8_t u8SoftSize;
extern uint8_t u8CRC;
extern uint8_t u8CountRecieveBlock;
enum uart_state{
  empty = 0, 
  recieve = 1
};
//Function block
void            vUART_Config(void);
void         u8UART_Recieve(uint8_t data);
void            vUART_Transmit(uint8_t data);
uint8_t         u8UART_RecieveNoIRQ(void);
#endif