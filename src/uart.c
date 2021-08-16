#include "uart.h"

//uint8_t RXBuff[64];
//uint8_t u8CountRecieve;
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
*@brief:  This function recieve data into UART
*@inval:  none
*@retval: enumerate of state recieve
*/
void vUART_Recieve(void){
  //if((UART1->SR & UART1_SR_RXNE) == UART1_SR_RXNE){
    uint8_t u8CurrData = UART1->DR;
    if(u8CurrData == 0xFF){
      ++u8CountEnd;
      if(u8CountEnd == 4){
        eBuffState = complete;
        u8CountEnd = 0;
      }
    }
  RXBuff[u8CountRecieve++] = u8CurrData;
  if(u8CountRecieve == 64){
    eBuffState = complete;
    u8CountRecieve = 0;
  }
  else{
    eBuffState = few;
  }
}
/*
*@brief:  this function transmit data at uart interface
*@inval:  uint8_t transmitted data
*@retval: none
*/
void vUART_Transmit(uint8_t data){
  while((UART1->SR&UART1_SR_TXE) != UART1_SR_TXE) asm("nop");
  UART1->DR = data;
}
/*
*@brief:  this function is IRQ at RXNE UART1
*@retval: none
*@inval:  none
*/
INTERRUPT_HANDLER(UART1_RX_IRQHandler, 18)
{
        UART1->SR&=~UART1_SR_RXNE;
	vUART_Recieve();
}
 