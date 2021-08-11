#include "uart.h"

uint8_t RXBuff[64];
uint8_t u8CountRecieve;
/*
*@brief: This function configured UART interface
*@inval: none
*@retval: none
*/
void vUART_Config(void){
  CLK->PCKENR1|=CLK_PCKENR1_UART1;//ENABLE CLOCKING
  UART1_Init(9600, UART1_WORDLENGTH_8D, UART1_STOPBITS_1, UART1_PARITY_NO, UART1_SYNCMODE_CLOCK_DISABLE, UART1_MODE_TXRX_ENABLE);
  UART1_ITConfig(UART1_IT_RXNE, ENABLE);
  UART1_Cmd(ENABLE);
}
/*
*@brief: This function recieve data into UART
*@inval: none
*@retval: enumerate of state recieve
*/
enum uart_state eUART_Recieve(void){
  enum uart_state eState = empty;
  if((UART1->SR & UART1_SR_RXNE) == UART1_SR_RXNE){
    uint8_t tData = UART1->DR;
    UART1->DR = tData;
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
