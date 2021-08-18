#include "main.h"
enum ebuffState eBuffState;
uint8_t u8CRC_Calculate(uint8_t cCRC, uint8_t nData){
  return cCRC ^ nData;
}