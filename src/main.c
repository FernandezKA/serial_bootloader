#include "main.h"
uint8_t RXBuff[BLOCK_SIZE];
uint8_t u8CountBlock = 0U;
uint8_t u8CountRecieve = 0U;
uint8_t u8BlockCRC = 0xFFU;
uint8_t u8ACKRequest = 0xABU;
uint8_t u8CountEnd = 0; 
int SystemInit(void)
{
    //CLK->CKDIVR = 2;
    //vUART_Config();
    GPIO_Init(GPIOB, GPIO_PIN_5, GPIO_MODE_OUT_PP_LOW_SLOW);
    GPIO_Init(BOOT_PORT, GPIO_PIN_7, GPIO_MODE_IN_PU_NO_IT);
    //asm("RIM");
    return 0;
}

void main(void)
{
	SystemInit();
	while (1){
          GPIOB->ODR^=(1<<5);
          if((BOOT_PORT->IDR & BOOT_PIN) == BOOT_PIN){//without bootloader
            asm("nop");
          }else{//with bootloader
            CLK->CKDIVR = 2;
            vUART_Config();
            vUART_Transmit('S');
            vUART_Transmit('\n');
            asm("RIM");
            asm("nop");
            //asm("jp 0x9000");
          }
          /*FLASH_Unlock(FLASH_MEMTYPE_PROG);
          FLASH_SetProgrammingTime(FLASH_PROGRAMTIME_TPROG);
          FLASH_ProgramBlock(50, FLASH_MEMTYPE_PROG, FLASH_PROGRAMMODE_STANDARD, buff);
          FLASH_ProgramBlock(51, FLASH_MEMTYPE_PROG, FLASH_PROGRAMMODE_STANDARD, buff);
          FLASH_Lock(FLASH_MEMTYPE_DATA);*/
        }
}

#ifdef USE_FULL_ASSERT
void assert_failed(u8 *file, u32 line)
{
  return;
}
#endif