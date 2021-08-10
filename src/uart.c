#include "uart.h"

uint8_t RXBuff[64];
uint8_t u8CountRecieve;
/*
*@brief: This function configured UART interface
*@inval: none
*@retval: none
*/
void vUART_Config(void){
  UART1->BRR1 = 0x34U;
  UART1->BRR2 = 0x08U;
  UART1->CR2|= UART1_CR2_TEN|UART1_CR2_REN;
}
/*
*@brief: This function recieve data into UART
*@inval: none
*@retval: enumerate of state recieve
*/
enum uart_state eUART_Recieve(void){
  enum uart_state eState = empty;
  if((UART1->SR & UART1_SR_RXNE) == UART1_SR_RXNE){
    RXBuff[u8CountRecieve++] = UART1->DR;
    eState = recieve;
  }
  return eState;
}
/*
*@brief: this function transmit data at uart interface
*@inval: uint8_t transmitted data
*@retval: none
*/
void vUART_Transmit(uint8_t data){
  while((UART1->SR&UART1_SR_TXE) != UART1_SR_TXE) asm("nop");
  UART1->DR = data;
}
