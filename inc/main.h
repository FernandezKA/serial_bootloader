#ifndef _main_h_
#define _main_h_
#define BLOCK_SIZE      64
#define BOOT_PORT       GPIOC
#define BOOT_PIN        0x80
#define MAIN_ADDR       0x8400
#include "stm8s_conf.h"
#include "uart.h"
#include "base_logic.h"
#include "flash.h"
extern uint8_t RXBuff[BLOCK_SIZE];
extern uint8_t u8CountBlock;
extern uint8_t u8CountBlockReieved;
extern uint8_t u8CountEnd;
extern uint8_t u8BlockCRC;
extern uint8_t u8CRC;
extern uint8_t u8BootVersion;
extern uint8_t u8SoftVersion;
uint8_t u8CRC_Calculate(uint8_t cCRC, uint8_t nData);
#endif