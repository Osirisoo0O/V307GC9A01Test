#include "debug.h"
#include "GC9A01.h"
#include "GC9A01_Test.h"
#include "board.h"

// void SPI_Write_Bus(uint32_t SPIx, uint16_t data)
// {
//     while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET)
//     {
//     }
//     SPI_I2S_SendData((SPI_TypeDef *)SPIx, data);
//     Delay_Us(6);
// }
// void SPI_Send_CMD(uint16_t data)
// {
//     LCD_Send_Command();
//     SPI_Write_Bus(SPI1, data);
//     LCD_Send_Data();
// }
// void SPI_Send_Data(uint16_t data)
// {
//     SPI_Write_Bus(SPI1, data);
// }

// void SPI_Send_DataH(uint16_t data)
// {
//     LCD_Send_Data();
//     SPI_Write_Bus(SPI1, data >> 8);
//     SPI_Write_Bus(SPI1, data);
// }

u8 TxData[BufferSize];

int main(void)
{
    Board_Init();
    TFT_init();

    Delay_Ms(1000);
    int delays = 100;
    while (1)
    {
#if 1
        Delay_Ms(delays);
        LCD_Full(&TxData,BufferSize,WHITE);
        Delay_Ms(delays);
        LCD_Full(&TxData,BufferSize,RED);
        Delay_Ms(delays);
        LCD_Full(&TxData,BufferSize,BLACK);
        Delay_Ms(delays);
        LCD_Full(&TxData,BufferSize,BLUE);
        Delay_Ms(delays);
        LCD_Full(&TxData,BufferSize,BRED);
        Delay_Ms(delays);
        LCD_Full(&TxData,BufferSize,GRED);
        Delay_Ms(delays);
        LCD_Full(&TxData,BufferSize,GBLUE);
        Delay_Ms(delays);
        LCD_Pic_Display(0, 0, 240, 240, BufferSize, &TxData, &gImage_eye);

#else
        Delay_Ms(delays);
        TFT_full(WHITE);
        Delay_Ms(delays);
        TFT_full(RED);
        Delay_Ms(delays);
        TFT_full(BLACK);
        Delay_Ms(delays);
        TFT_full(BLUE);
        Delay_Ms(delays);
        TFT_full(BRED);
        Delay_Ms(delays);
        TFT_full(GRED);
        Delay_Ms(delays);
        TFT_full(GBLUE);
        Delay_Ms(delays);
        Picture_display(0, 0, 240, 240, gImage_eye);
#endif

        Delay_Ms(2000);
    }
}