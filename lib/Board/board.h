#ifndef BOARD_H
#define BOARD_H

#include "ch32v30x.h"
#include "config.h"

#define Screen_SPI SPI1
#define Screen_SPI_DMA DMA1
#define Screen_DMA_Channel DMA1_Channel3

extern u8 TxData[];

void Board_Init(void);

void SPI_DMA_Transmit(SPI_TypeDef* SPIx,DMA_Channel_TypeDef * Channelx,uint16_t DataLength);

#endif