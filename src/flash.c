#include "flash.h"

void vFlashUnlock(void)
{
  FLASH->PUKR = 0x56;
  FLASH->PUKR = 0xAE;
}
void vFlashWritePage(uint32_t addr, uint8_t *pData)
{
  //FLASH -> CR2|= FLASH_CR2_PRG;
  for (uint32_t i = addr; i < addr + 64; ++i)
  {
    FLASH_ProgramByte(addr, *(pData + i));
  }
}