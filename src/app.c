#include "debug.h"
#include "GC9A01.h"
#include "GC9A01_Test.h"
#include "board.h"


u8 TxData[BufferSize];

int main(void)
{
    Board_Init();
    TFT_init();

    Delay_Ms(1000);
    int delays = 500;
    while (1)
    {
#if 1
        LCD_Full((u8*)&TxData,BufferSize,WHITE);
        Delay_Ms(delays);
        LCD_Full((u8*)&TxData,BufferSize,RED);
        Delay_Ms(delays);
        LCD_Full((u8*)&TxData,BufferSize,BLACK);
        Delay_Ms(delays);
        LCD_Full((u8*)&TxData,BufferSize,BLUE);
        Delay_Ms(delays);
        LCD_Full((u8*)&TxData,BufferSize,GREEN);
        Delay_Ms(delays);
        LCD_Full((u8*)&TxData,BufferSize,GBLUE);
        Delay_Ms(delays);
        LCD_Pic_Display(0, 0, 240, 240, BufferSize, (u8*)&TxData, (const unsigned char *)&gImage_eye);
        Delay_Ms(delays);

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

        Delay_Ms(1000);
    }
}