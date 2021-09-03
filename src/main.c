#include "main.h"
/*********************************/
//This block contain main constant
uint8_t RXBuff[BLOCK_SIZE];
uint8_t u8CountBlock = 0U;
uint8_t u8CountRecieve = 0U;
uint8_t u8BlockCRC = 0xFFU;
uint8_t u8CRC = 0xFF;
uint8_t u8rCRC = 0xFF;
uint8_t u8ACK = 0x55U;
uint8_t u8NACK = 0x65U;
uint32_t u32ProgAddr = 0x9000;
uint8_t u8SoftVersion = 0x01;
uint8_t u8BootVersion = 0x01;
/*********************************/
int SystemInit(void)
{
  CLK->CKDIVR = 2;
  vUART_Config();
  GPIO_Init(GPIOB, GPIO_PIN_5, GPIO_MODE_OUT_PP_LOW_SLOW);
  GPIO_Init(BOOT_PORT, GPIO_PIN_7, GPIO_MODE_IN_PU_NO_IT);
  asm("RIM");
  return 0;
}

void main(void)
{
  SystemInit();
  while (1)
  {
    //GPIOB->ODR^=(1<<5);
    if ((BOOT_PORT->IDR & BOOT_PIN) == BOOT_PIN)
    { //without bootloader
      UART1->DR = 0x00;
    }
    else
    { //with bootloader
      while(1){//Check for new data
        bool RecieveSoftware = FALSE;
        if(!RecieveSoftware){
          uint8_t Request = 0x00;
          Request = u8UART_RecieveNoIRQ();
          switch(Request){
          case 0x01://Detect device
            
            break;
          case 0x02://Check boot version
            
            break;
          case 0x03://Check soft version
            
            break;
          case 0x04://Begin soft recieve
            
            break;
          }
        }
        else{
          
        }
      }
    }
  }
}
#ifdef USE_FULL_ASSERT
void assert_failed(u8 *file, u32 line)
{
  return;
}
#endif