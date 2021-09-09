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
uint32_t u32ProgAddr = 0x8040;
//Info variables
uint8_t u8SoftVersion = 0x01;
uint8_t u8BootVersion = 0x01;
uint8_t u8CountRequest = 0x00;
bool RecieveSoftware = FALSE;
uint8_t u8SoftSize = 0x00;
/*********************************/
int SystemInit(void)
{
  CLK->CKDIVR = 2;
  vUART_Config();
  //TODO Edit gpio initialization (without SPL)
  GPIO_Init(GPIOB, GPIO_PIN_5, GPIO_MODE_OUT_PP_LOW_SLOW);
  GPIO_Init(BOOT_PORT, GPIO_PIN_7, GPIO_MODE_IN_PU_NO_IT);
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
      asm("JP $8040");
    }
    else
    { //with bootloader
      //uint8_t SoftSize = 0;
      //uint8_t u8CountRecieve = 0;
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
          case 0x01:
            vUART_Transmit(u8ACK);
            Request = 0x00;
            break;
          case 0x02:
            vUART_Transmit(u8BootVersion);
            Request = 0x00;
            break;
          case 0x03:
            vUART_Transmit(u8BootVersion);
            Request = 0x00;
            break;
          case 0x04:
            vUART_Transmit(u8ACK);
            uint8_t u8SoftSize = u8UART_RecieveNoIRQ();
            uint8_t u8SoftRecieve = 0x00;
            //uint8_t u8dCRC = 0xFF;
            uint8_t u8CRC = 0xFF;
            while (u8SoftRecieve < u8SoftSize)
            {
              while (eBuffState != complete)
              {
                if ((UART1->SR & UART1_SR_RXNE) == UART1_SR_RXNE)
                {
                  uint8_t u8RecieveData = UART1->DR;
                  vUART_Recieve(u8RecieveData);
                  u8rCRC ^= u8RecieveData;
                }
              }
              vUART_Transmit(u8NACK);
              u8CRC = u8UART_RecieveNoIRQ();
              if (u8rCRC == u8CRC)
              {
                //++u8SoftRecieve;?
                FLASH_Unlock(FLASH_MEMTYPE_PROG);
                FLASH_ProgramBlock(++u8SoftRecieve, FLASH_MEMTYPE_PROG, FLASH_PROGRAMMODE_STANDARD, RXBuff);
                FLASH_Lock(FLASH_MEMTYPE_PROG);
                u8rCRC = 0xFF;
                u8CRC = 0xFF;
                eBuffState = few;
                vUART_Transmit(u8ACK);
              }
              else
              {
                vUART_Transmit(u8NACK);
              }
            }
            //u8SoftRecieve = 0x00;
            //u8SoftSize = 0x00;
            //Request = 0x00;
            asm("jp 0x8040");
            break;
          default:
            Request = 0x00;
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