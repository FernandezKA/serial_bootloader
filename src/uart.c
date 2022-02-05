#include "uart.h"
uint8_t u8CountRecieveBlock = 0;
/*
*@brief: This function configured UART interface
*@inval: none
*@retval: none
*/
void vUART_Config(void)
{
  CLK->PCKENR1 |= CLK_PCKENR1_UART1; //ENABLE CLOCKING
  UART1->BRR1 = 0x68;
  UART1->BRR2 = 0x02;
  UART1->CR2 |= UART1_CR2_REN | UART1_CR2_TEN;
  //UART1->CR5|=UART1_CR5_HDSEL;
  //UART1_Init(9600, UART1_WORDLENGTH_8D, UART1_STOPBITS_1, UART1_PARITY_NO, UART1_SYNCMODE_CLOCK_DISABLE, UART1_MODE_TXRX_ENABLE);
  //UART1_ITConfig(UART1_IT_RXNE, ENABLE);
  //UART1_Cmd(ENABLE);
}
/*
*@brief:  This function recieve data into UART
*@inval:  none
*@retval: enumerate of state recieve
*/
void vUART_Recieve(uint8_t data)
{
  //if((UART1->SR & UART1_SR_RXNE) == UART1_SR_RXNE){
  uint8_t u8CurrData = data;
  RXBuff[u8CountRecieve++] = u8CurrData;
  if (u8CountRecieve == 64)
  {
    ++u8CountBlock;
    eBuffState = complete;
    u8CountRecieve = 0;
  }
  else
  {
    eBuffState = few;
  }
}
/*
*@brief:  this function transmit data at uart interface
*@inval:  uint8_t transmitted data
*@retval: none
*/
void vUART_Transmit(uint8_t data)
{
  UART1->CR2&=~UART1_CR2_REN;
  while ((UART1->SR & UART1_SR_TXE) != UART1_SR_TXE)
  { 
    asm("nop");//Wait empty buff state
  }
  UART1->DR = data;
  for(uint16_t i = 0; i < 0xFFF; ++i){asm("nop");}
  UART1->CR2|=UART1_CR2_REN;
}
/*
*@brief:  this function receieve size of loaded soft
*@retval: uint8_t count block
*@inval:  none
*/
uint8_t u8UART_RecieveNoIRQ(void)
{
  while ((UART1->SR & UART1_SR_RXNE) != UART1_SR_RXNE)
  {
    asm("nop");
  }
  return UART1->DR;
}