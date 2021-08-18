#include "main.h"
uint8_t RXBuff[BLOCK_SIZE];
uint8_t u8CountBlock = 0U;
uint8_t u8CountRecieve = 0U;
uint8_t u8BlockCRC = 0xFFU;
uint8_t u8CRC = 0xFF;
uint8_t u8rCRC = 0xFF;
uint8_t u8ACK = 0xABU;
uint8_t u8NACK = 0xAA;
uint32_t u32ProgAddr = 0x9000;
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
	while (1){
          //GPIOB->ODR^=(1<<5);
          if((BOOT_PORT->IDR & BOOT_PIN) == BOOT_PIN){//without bootloader
            UART1->DR = 0x00;
          }else{//with bootloader
            UART1->DR = 0xFF;//send respond for size
            asm("SIM");
            u8SoftSize = u8UART_RecieveNoIRQ();//recieve size at block
            asm("RIM");
             while(eBuffState != complete){//recieve next data at IRQ
              asm("nop");//if status is complete, disable IRQ and check CRC
            }
            asm("SIM");
            u8rCRC = u8UART_RecieveNoIRQ();//Recieve CRC        
            if(u8rCRC == u8CRC){
             vUART_Transmit(u8ACK);
             u8CountRecieveBlock++;
             FLASH_Unlock(FLASH_MEMTYPE_PROG);
             FLASH_ProgramBlock(((u32ProgAddr - 0x8000)/64) + u8CountRecieveBlock, FLASH_MEMTYPE_PROG, FLASH_PROGRAMMODE_STANDARD, RXBuff);
             FLASH_Lock(FLASH_MEMTYPE_DATA);
            }
            else{
              vUART_Transmit(u8NACK);
              for(uint8_t i = 0; i < 64; i++){//clear Buff
                RXBuff[i] = 0;
              }
            }
    //if no all program block is recieved, write next block
            if(u8CountRecieveBlock == u8SoftSize){//Increment block, write flash
              vUART_Transmit(u8ACK);
            }
            else{//If all of software block is recieved, write last data and send respond
              
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