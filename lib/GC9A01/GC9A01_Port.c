/*
 * @FilePath: GC9A01_port.c
 * @Author: ASWaterbenben
 * @Date: 2022-09-09 11:03:58
 * @LastEditors: ASWaterbenben
 * @LastEditTime: 2022-09-09 14:36:31
 * Copyright: 2022 oulidaTech CO.,LTD. All Rights Reserved.
 * @Descripttion: 
 */
#include "GC9A01_port.h"
#include "debug.h"


static void SPI_Write_Bus(SPI_TypeDef * SPIx,char data){
     while (SPI_I2S_GetFlagStatus(SPI1,SPI_I2S_FLAG_TXE)==RESET){}
    SPI_I2S_SendData((SPI_TypeDef*)SPIx,data);
    Delay_Us(6);
}


/**
 * @description: SPI send one byte
 * @param {unsigned char} byte
 * @return {*}
 * @author: ASWaterbenben
 */
void SPI_SendByte(unsigned char byte)
{
    SPI_CS_0;
    // Add SPI transmit ont byte function begin
    //HAL_SPI_Transmit(&hspi1, &byte, 1, 0xFFFF);
    SPI_Write_Bus(SPI1,byte);

    // Add SPI transmit ont byte function end
    SPI_CS_1;
}

/**
 * @description: Send command to GC9A01
 * @param {unsigned char} o_command
 * @return {*}
 * @author: ASWaterbenben
 */
void TFT_send_cmd(unsigned char o_command)
{
    SPI_DC_0;
    SPI_SendByte(o_command);
    SPI_DC_1;
}

/**
 * @description: Send data to GC9A01
 * @param {unsigned char} o_data
 * @return {*}
 * @author: ASWaterbenben
 */
void TFT_send_data(unsigned char o_data)
{
    SPI_SendByte(o_data);
}

void TFT_delay_ms(unsigned int _ms_time)
{
    Delay_Ms(_ms_time);
}

void TFT_delay_us(unsigned int _us_time)
{
    Delay_Us(_us_time);
}

