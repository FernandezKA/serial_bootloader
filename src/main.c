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
uint8_t u8CountRequest = 0x00;
bool RecieveSoftware = FALSE;
uint8_t u8SoftSize = 0x00;
uint8_t Request = 0x00;
/*********************************/
int SystemInit(void)
{
  CLK->CKDIVR = 2;
  vUART_Config();
  GPIO_Init(GPIOB, GPIO_PIN_5, GPIO_MODE_OUT_PP_LOW_SLOW);
  GPIO_Init(BOOT_PORT, GPIO_PIN_7, GPIO_MODE_IN_PU_NO_IT);
  //FLASH_DeInit();
  //FLASH_SetProgrammingTime(FLASH_PROGRAMTIME_TPROG);
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
      asm("LDW X,  SP ");
      asm("LD  A,  $FF");
      asm("LD  XL, A  ");
      asm("LDW SP, X  ");
      asm("JP 0x9000");
    }
    else
    { //with bootloader
        //uint8_t SoftSize = 0;
        uint8_t u8CountRecieve = 0;
      while(1){//Check for new data
        /*******************************************/
        if(!RecieveSoftware&&u8CountRequest > 0){
          --u8CountRequest;
          switch(Request){
          case 0x01:
            vUART_Transmit(u8ACK);
            break;
          case 0x02:
            vUART_Transmit(u8BootVersion);
            break;
          case 0x03:
            vUART_Transmit(u8SoftVersion);
            break;
          case 0x04:
            vUART_Transmit(u8ACK);
            RecieveSoftware = TRUE;
            break;
          }
        }
       /*******************************************/
        else if(RecieveSoftware){
          FLASH_Unlock(FLASH_MEMTYPE_PROG);
          if(u8CountBlock > 0/*eBuffState == complete*/){
              --u8CountBlock;
              uint8_t u8dCRC = 0xFF;
              uint8_t u8CRC = 0xFF;
              for(uint8_t i = 0; i < 64; ++i){//Check CRC8
                u8dCRC^=RXBuff[i]; 
              }
              asm("SIM");
              vUART_Transmit(u8NACK);
              u8CRC = u8UART_RecieveNoIRQ();//Recieve CRC from PC
              if(u8CRC == u8dCRC){//If block is valid
                ++u8CountRecieve;
                FLASH_Unlock(FLASH_MEMTYPE_PROG);
                FLASH_ProgramBlock((((u32ProgAddr - 0x8000)/64) + u8CountRecieve) - 1, FLASH_MEMTYPE_PROG, FLASH_PROGRAMMODE_STANDARD, RXBuff);
                //FLASH_Lock(FLASH_MEMTYPE_PROG);
                vUART_Transmit(u8ACK);
                asm("RIM");
              }
              else{
                vUART_Transmit(u8NACK);
                for(uint8_t i = 0; i < 64; ++i){//Clear buffer
                  RXBuff[i] = 0x00;
                }
                asm("RIM");
              }
              if(u8CountRecieve == u8SoftSize&&u8SoftSize != 0){
                vUART_Transmit(u8ACK);
                RecieveSoftware = FALSE;
                FLASH_Lock(FLASH_MEMTYPE_PROG);
                asm("SIM");
                asm("LDW X,  SP ");
                asm("LD  A,  $FF");
                asm("LD  XL, A  ");
                asm("LDW SP, X  ");
                asm("JPF $9000");
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