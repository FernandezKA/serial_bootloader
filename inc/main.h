#ifndef _main_h_
#define _main_h_
//User definitions
#define BLOCK_SIZE      64
#define BOOT_PORT       GPIOB
#define BOOT_PIN        0x10 //GPIOB4
#define CountSimleNums  0x06
#define InfoBlock 0x00009F80
//User include
#include "stm8s_conf.h"
#include "uart.h"
#include "base_logic.h"
#include "flash.h"
//User variables
//Varilable for scrambling
extern uint8_t SimpleNums[CountSimleNums];

extern uint8_t RXBuff[BLOCK_SIZE];
extern uint8_t u8CountBlock;
extern uint8_t u8CountBlockReieved;
extern uint8_t u8CountEnd;
extern uint8_t u8BlockCRC;
extern uint8_t u8CRC;
extern uint8_t *pu8PageInfo;
extern uint8_t u8BootVersion;
extern uint8_t u8SoftVersion;
extern uint8_t u8HardVersion;
extern bool RecieveSoftware;
extern uint8_t u8CountRequest;
extern uint8_t Request;
extern uint8_t u8SoftSize;
extern uint8_t u8CountBlock;
//uint8_t u8CRC_Calculate(uint8_t cCRC, uint8_t nData);
#endif