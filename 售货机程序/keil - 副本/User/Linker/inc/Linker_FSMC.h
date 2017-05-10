#ifndef __LINKER_FSMC_H
#define __LINKER_FSMC_H

#include "stm32f4xx.h"
/**
  * @brief  Configures the FSMC and GPIOs to interface with the SRAM memory.
  *         This function must be called before any write/read operation
  *         on the SRAM.
  * @param  None
  * @retval None
  */
void FSMC_Init(void);

void FSMC_WriteData(uint16_t tData);

void FSMC_WriteCommand(uint16_t tCMD);

uint16_t  FSMC_ReadData(void);

uint16_t FSMC_ReadStatus(void);

#endif
