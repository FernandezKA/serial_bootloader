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
        uint8_t SoftSize = 0;
        uint8_t u8CountRecieve = 0;
        bool RecieveSoftware = FALSE;
        if(!RecieveSoftware){//Detect device action
          uint8_t Request = 0x00;
          Request = u8UART_RecieveNoIRQ();
          switch(Request){
          case 0x01://Detect device
            vUART_Transmit(u8ACK);
            break;
          case 0x02://Check boot version
            vUART_Transmit(u8BootVersion);
            break;
          case 0x03://Check soft version
            vUART_Transmit(u8SoftVersion);
            break;
          case 0x04://Begin soft recieve
            vUART_Transmit(u8ACK);
            SoftSize = u8UART_RecieveNoIRQ();
            vUART_Transmit(u8ACK);
            RecieveSoftware = TRUE;
            break;
          }
        }
        else{//Recieve software and write flash action
          asm("RIM");
          while(RecieveSoftware){
            if(eBuffState == complete){
              uint8_t u8dCRC = 0xFF;
              uint8_t u8CRC = 0xFF;
              for(uint8_t i = 0; i < 64; ++i){
                u8dCRC^=RXBuff[i]; 
              }
              u8CRC = u8UART_RecieveNoIRQ();
              if(u8CRC == u8dCRC){
                ++u8CountRecieve;
                vUART_Transmit(u8ACK);
              }
              else{
                vUART_Transmit(u8NACK);
                for(uint8_t i = 0; i < 64; ++i){//Clear buffer
                  RXBuff[i] = 0x00;
                }
              }
              if(u8CountRecieve == SoftSize){
                vUART_Transmit(u8ACK);
                RecieveSoftware = FALSE;
                asm("SIM");
                asm("jp 0x9000");
              }
            }
          }
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