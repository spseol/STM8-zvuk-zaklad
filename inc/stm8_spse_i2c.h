#ifndef __I2C_OPT_H
#define __I2C_OPT_H

#include "stm8s.h"

#define tout() 1

#if defined(__CSMC__)
 #define i2c_dead_time() { _asm("nop"); _asm("nop"); }
#elif defined(__SDCC)
 #define i2c_dead_time() { __asm__("nop"); __asm__("nop"); }
#elif defined(__RCST7__)
#else
 #error Unsupported Compiler!          /* Compiler defines not found */
#endif

void I2C_RandomRead(uint8_t slaveaddr, uint8_t u8_NumByteToRead, uint8_t *u8_DataBuffer) ;
void I2C_ReadRegister(uint8_t slaveaddr, uint8_t u8_regAddr, uint8_t u8_NumByteToRead, uint8_t *u8_DataBuffer);
void I2C_WriteRegister(uint8_t slaveaddr, uint8_t u8_regAddr, uint8_t u8_NumByteToWrite,uint8_t *u8_DataBuffer);

#endif /* __I2C_OPT_H */
