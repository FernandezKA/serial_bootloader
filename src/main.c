#include "main.h"

int SystemInit(void)
{
    CLK->CKDIVR = 0;
    //GPIO_Init(GPIOD, GPIO_PIN_5, GPIO_MODE_OUT_PP_LOW_SLOW);//stop
    vUART_Config();
    return 0;
}

void main(void)
{
	SystemInit();
	while (1){

        };
}

#ifdef USE_FULL_ASSERT
void assert_failed(u8 *file, u32 line)
{
  asm("nop");
  return;
}
#endif