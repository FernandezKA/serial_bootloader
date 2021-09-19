#include "main.h"
/*********************************/
//This block contain main constant
uint8_t RXBuff[BLOCK_SIZE];
uint8_t u8CountBlock = 0U;
uint8_t u8CountRecieve = 0U;
//uint8_t u8BlockCRC = 0xFFU;
uint8_t u8CRC = 0xFF;
uint8_t u8rCRC = 0xFF;
//Communication variables
uint8_t u8ACK = 0x55U;
uint8_t u8NACK = 0x65U;
uint8_t Request = 0x00;
//Info variables
uint8_t u8SoftVersion = 0x01;
uint8_t u8BootVersion = 0x02;
uint8_t u8CountRequest = 0x00;
bool RecieveSoftware = FALSE;
uint8_t u8SoftSize = 0x00;
uint16_t u16FreeSize = 0x1A00; 
uint8_t u8UID[12] = {0};
/*********************************/
int SystemInit(void)
{
  CLK->CKDIVR = 0;
  vUART_Config();
  //OUTPUT PUSH PULL
  GPIOB->DDR|=(1<<5);
  GPIOB->CR1|=(1<<5);
  //INPUT PULL UP
  GPIOB->CR1|=(1<<4);
  for(uint8_t i = 0; i < 12; ++i){
    u8UID[i] = FLASH_ReadByte(0x4865 + i);
  }
  return 0;
}

void main(void)
{
  SystemInit();
  while (1)
  {
    //GPIOB->ODR^=(1<<5);
    if ((BOOT_PORT->IDR & BOOT_PIN) == BOOT_PIN) //Warning! For simulation edit at !=, correctly ==
    {                                            //without bootloader
      asm("LDW X,  SP ");
      asm("LD  A,  $FF");
      asm("LD  XL, A  ");
      asm("LDW SP, X  ");
      asm("JP $8000");
    }
    else
    { //with bootloader
      vUART_Transmit(0x55);
      vUART_Transmit(0x55);
      vUART_Transmit(0xAA);
      uint8_t Request = 0x00;
      for (;;)
      {
        while (Request == 0x00)
        {
          //UART1->DR = 0x64;
          if ((UART1->SR & UART1_SR_RXNE) == UART1_SR_RXNE)
          {
            Request = UART1->DR;
          }
          switch (Request)
          {
          case 0x01://Soft and boot version
            vUART_Transmit(u8BootVersion);
            vUART_Transmit(u8SoftVersion);
            vUART_Transmit((u16FreeSize >> 8)&0xFF);//MSB
            vUART_Transmit(u16FreeSize & 0xFF);//LSB
            for(int i = 0; i < 12; ++i){
              vUART_Transmit(u8UID[i]);
            }
            Request = 0x00;
            break;
/******************************************************************************/
          case 0x02://Recieve soft request
            Request = 0x00;
            uint8_t u8CountRecieve = 0x00;
            while(u8CountRecieve < 64){
              uint8_t u8RecieveByte = 0x00;
              uint8_t u8rCRC = 0xFF;
              uint8_t u8dCRC = 0xFF;
              while(u8RecieveByte < 64){
                if((UART1->SR & UART1_SR_RXNE) == UART1_SR_RXNE){
                  RXBuff[u8RecieveByte++] = UART1->DR;
                }
              }
              for(uint8_t i = 0; i < 64; ++i){
                u8dCRC^=RXBuff[i];
              }
              u8rCRC = u8UART_RecieveNoIRQ();
              if(u8rCRC == u8dCRC){
                FLASH_Unlock(FLASH_MEMTYPE_PROG);
                FLASH_ProgramBlock(u8CountRecieve, FLASH_MEMTYPE_PROG, FLASH_PROGRAMMODE_STANDARD, RXBuff);
                FLASH_Lock(FLASH_MEMTYPE_PROG);
                ++u8CountRecieve;
                vUART_Transmit(u8ACK);
              }
              else{
                vUART_Transmit(u8NACK);
              }

            }
            break;
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