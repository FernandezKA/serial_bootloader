#ifndef _main_h_
#define _main_h_
#define BLOCK_SIZE      64
#define BOOT_PORT       GPIOD
#define BOOT_PIN        0x01
#define MAIN_ADDR       0x8400
#include "stm8s_conf.h"
#include "uart.h"
#include "base_logic.h"
extern uint8_t RXBuff[BLOCK_SIZE];
extern uint8_t u8CountBlock;
extern uint8_t u8CountBlockReieved;
extern uint8_t u8BlockCRC;
#endif