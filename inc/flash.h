#ifndef _flash_h_
#define _flash_h_
#include "main.h"
#include "stm8s_flash.h"
void vFlashUnlock(void);
void vFlashWritePage(uint32_t addr, uint8_t* pData);
#endif