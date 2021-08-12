#include "main.h"
uint8_t RXBuff[BLOCK_SIZE];
uint8_t u8CountBlock = 0U;
uint8_t u8CountRecieve = 0U;
uint8_t u8BlockCRC = 0xFFU;
uint8_t u8ACKRespond = 0xAAU;
uint8_t u8NACKRespond = 0x55U;
uint8_t u8RecieveCRC = 0xFFU;
uint8_t u8ValidCRC = 0x00U;
uint8_t u8ACKRequest = 0xABU;
int SystemInit(void)
{
    CLK->CKDIVR = 2;
    vUART_Config();
    GPIO_Init(GPIOB, GPIO_PIN_5, GPIO_MODE_OUT_PP_LOW_SLOW);
    GPIO_Init(GPIOD, GPIO_PIN_1, GPIO_MODE_IN_PU_NO_IT);
    asm("RIM");
    return 0;
}

void main(void)
{
	SystemInit();
	while (1){
          GPIOB->ODR^=(1<<5);
          if(BOOT_PORT->IDR&BOOT_PIN == BOOT_PIN){
            if(eBuffState == complete){
              //TODO this we begin start write block
                //send request from crc
                UART1->DR = u8ACKRequest;
                //recieve CRC
                uint8_t bIsRecieve = 0x00U;
                uint16_t u16WaitCounter = 0x0000U;
                while(!bIsRecieve && u16WaitCounter < 0xFFFF - 1){
                  ++u16WaitCounter;
                  if(UART1->SR & UART1_SR_RXNE == UART1_SR_RXNE){
                    bIsRecieve = 0xFFU;
                    u8ValidCRC = UART1->DR;
                  }
                }
                if(bIsRecieve && u8ValidCRC == u8RecieveCRC){
                  UART1->DR = u8ACKRespond;
                }
                else{
                  UART1->DR = u8NACKRespond;
                }
                bIsRecieve = 0x00U;
                u16WaitCounter = 0x0000U;
            } 
          }
          else{
            asm("jp 0x8400");
          }
        };
}

#ifdef USE_FULL_ASSERT
void assert_failed(u8 *file, u32 line)
{
  return;
}
#endif