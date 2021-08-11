#include "main.h"

int SystemInit(void)
{
    CLK->CKDIVR = 2;
    vUART_Config();
    GPIO_Init(GPIOB, GPIO_PIN_5, GPIO_MODE_OUT_PP_LOW_SLOW);//stop
    asm("RIM");
    return 0;
}

void main(void)
{
	SystemInit();
	while (1){
          GPIOB->ODR^=(1<<5);
          UART1->DR = 0x64;
          for(uint16_t i = 0; i < 0xFFFF; ++i) asm("nop");
        };
}

#ifdef USE_FULL_ASSERT
void assert_failed(u8 *file, u32 line)
{
  return;
}
#endif