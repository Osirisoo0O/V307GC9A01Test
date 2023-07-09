#include "debug.h"
#include "GC9A01.h"
#include "GC9A01_Test.h"

#define USE_HORIZONTAL 0
#define LCD_Select() GPIO_WriteBit(GPIOA, GPIO_Pin_4, Bit_RESET);
#define LCD_Unselect() GPIO_WriteBit(GPIOA, GPIO_Pin_4, Bit_SET);

#define LCD_Reset_Disable() GPIO_WriteBit(GPIOA, GPIO_Pin_0, Bit_SET);
#define LCD_Reset_Enable() GPIO_WriteBit(GPIOA, GPIO_Pin_0, Bit_RESET);
#define LCD_Send_Data() GPIO_WriteBit(GPIOA, GPIO_Pin_1, Bit_SET);
#define LCD_Send_Command() GPIO_WriteBit(GPIOA, GPIO_Pin_1, Bit_RESET);
#define LCD_BackLight_ON() GPIO_WriteBit(GPIOA, GPIO_Pin_2, Bit_SET);
#define LCD_BackLight_OFF() GPIO_WriteBit(GPIOA, GPIO_Pin_2, Bit_RESET);

#define WHITE 0xFFFF
#define BLACK 0x0000
#define BLUE 0x001F
#define BRED 0XF81F
#define GRED 0XFFE0
#define GBLUE 0X07FF
#define RED 0xF800
#define GREEN 0x07E0

#define ORIENTATION 2 // Set the display orientation 0,1,2,3

// Command codes:
#define COL_ADDR_SET 0x2A
#define ROW_ADDR_SET 0x2B
#define MEM_WR 0x2C
#define COLOR_MODE 0x3A
#define COLOR_MODE_12_BIT 0x03
#define COLOR_MODE_16_BIT 0x05
#define COLOR_MODE_18_BIT 0x06
#define MEM_WR_CONT 0x3C

void SPI_Write_Bus(uint32_t SPIx, uint16_t data)
{
    while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET)
    {
    }
    SPI_I2S_SendData((SPI_TypeDef *)SPIx, data);
    Delay_Us(6);
}
void SPI_Send_CMD(uint16_t data)
{
    LCD_Send_Command();
    SPI_Write_Bus(SPI1, data);
    LCD_Send_Data();
}
void SPI_Send_Data(uint16_t data)
{
    SPI_Write_Bus(SPI1, data);
}

void SPI_Send_DataH(uint16_t data)
{
    LCD_Send_Data();
    SPI_Write_Bus(SPI1, data >> 8);
    SPI_Write_Bus(SPI1, data);
}
void LCD_Address_Set(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2)
{
    SPI_Send_CMD(0x2A); // 列地址设置
    SPI_Send_Data(0);
    SPI_Send_Data(x1);
    SPI_Send_Data(0);
    SPI_Send_Data(x2);
    SPI_Send_CMD(0x2B); // 行地址设置
    SPI_Send_Data(0);
    SPI_Send_Data(y1);
    SPI_Send_Data(0);
    SPI_Send_Data(y2);
    SPI_Send_CMD(0x2C); // 储存器写
}
void LCD_Fill(uint16_t xsta, uint16_t ysta, uint16_t xend, uint16_t yend, uint16_t color)
{
    uint16_t i, j;
    LCD_Address_Set(xsta, ysta, xend - 1, yend - 1); // 设置显示范围
    for (i = ysta; i < yend; i++)
    {
        for (j = xsta; j < xend; j++)
        {
            SPI_Send_Data(color >> 8);
            SPI_Send_Data(color);
        }
    }
}

void Usart_Init()
{
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
    SystemCoreClockUpdate();
    Delay_Init();
    USART_Printf_Init(115200);
    printf("SystemClk: %d\r\n", (int)SystemCoreClock);
    printf("ChipID: %08x\r\n", (unsigned int)DBGMCU_GetCHIPID());
    printf("This is GC9A01 example\r\n");
}

void SPI_1Line_Tx_Init(void)
{
    // 初始化SPI/GPIO
    SPI_InitTypeDef SPI_InitStructure = {0};
    GPIO_InitTypeDef GPIO_InitStructure = {0};
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_SPI1, ENABLE);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_7;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    SPI_InitStructure.SPI_Direction = SPI_Direction_1Line_Tx; // 方向
    SPI_InitStructure.SPI_Mode = SPI_Mode_Master;             // 主机模式

    SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
    SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;                         // 时钟极性
    SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;                       // 时钟沿
    SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;                          // 硬件片选
    SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_4; // 分频
    SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;                 // 第一位
    SPI_InitStructure.SPI_CRCPolynomial = 7;
    SPI_Init(SPI1, &SPI_InitStructure);
    SPI_CalculateCRC(SPI1, DISABLE);

    // 开启DMA
    SPI_I2S_DMACmd(SPI1, SPI_I2S_DMAReq_Tx, ENABLE);

    SPI_Cmd(SPI1, ENABLE);
}
/*********************************************************************
 * @fn      DMA_Rx_Init
 *
 * @brief   Initializes the SPI1 DMA Channelx configuration.
 *
 * @param   DMA_CHx - x can be 1 to 7.
 *          ppadr - Peripheral base address.
 *          memadr - Memory base address.
 *          bufsize - DMA channel buffer size.
 *
 * @return  none
 */
void DMA_Tx_Init(DMA_Channel_TypeDef *DMA_CHx, u32 ppadr, u32 memadr, u16 bufsize)
{
    DMA_InitTypeDef dma_def = {0};
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);

    DMA_DeInit(DMA_CHx);

    dma_def.DMA_PeripheralBaseAddr = ppadr;
    dma_def.DMA_MemoryBaseAddr = memadr;
    dma_def.DMA_DIR = DMA_DIR_PeripheralDST;
    dma_def.DMA_BufferSize = bufsize;
    dma_def.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
    dma_def.DMA_MemoryInc = DMA_MemoryInc_Enable;
    dma_def.DMA_PeripheralDataSize = DMA_MemoryDataSize_Byte;
    dma_def.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
    dma_def.DMA_Mode = DMA_Mode_Normal;
    dma_def.DMA_Priority = DMA_Priority_Medium;
    dma_def.DMA_M2M = DMA_M2M_Disable;
    DMA_Init(DMA_CHx, &dma_def);
}

void LCD_Init()
{

    LCD_Reset_Enable();
    Delay_Ms(10); // 100
    LCD_Reset_Disable();
    Delay_Ms(10); // 100

    SPI_Send_CMD(0xEF);

    SPI_Send_CMD(0xEB);
    SPI_Send_Data(0x14);

    SPI_Send_CMD(0xFE);
    SPI_Send_CMD(0xEF);

    SPI_Send_CMD(0xEB);
    SPI_Send_Data(0x14);

    SPI_Send_CMD(0x84);
    SPI_Send_Data(0x40);

    SPI_Send_CMD(0x85);
    SPI_Send_Data(0xFF);

    SPI_Send_CMD(0x86);
    SPI_Send_Data(0xFF);

    SPI_Send_CMD(0x87);
    SPI_Send_Data(0xFF);

    SPI_Send_CMD(0x88);
    SPI_Send_Data(0x0A);

    SPI_Send_CMD(0x89);
    SPI_Send_Data(0x21);

    SPI_Send_CMD(0x8A);
    SPI_Send_Data(0x00);

    SPI_Send_CMD(0x8B);
    SPI_Send_Data(0x80);

    SPI_Send_CMD(0x8C);
    SPI_Send_Data(0x01);

    SPI_Send_CMD(0x8D);
    SPI_Send_Data(0x01);

    SPI_Send_CMD(0x8E);
    SPI_Send_Data(0xFF);

    SPI_Send_CMD(0x8F);
    SPI_Send_Data(0xFF);

    SPI_Send_CMD(0xB6);
    SPI_Send_Data(0x00);
    SPI_Send_Data(0x00);

    SPI_Send_CMD(0x36);

#if ORIENTATION == 0
    SPI_Send_Data(0x18);
#elif ORIENTATION == 1
    SPI_Send_Data(0x28);
#elif ORIENTATION == 2
    SPI_Send_Data(0x48);
#else
    SPI_Send_Data(0x88);
#endif

    SPI_Send_CMD(COLOR_MODE);
    SPI_Send_Data(COLOR_MODE_16_BIT);

    SPI_Send_CMD(0x90);
    SPI_Send_Data(0x08);
    SPI_Send_Data(0x08);
    SPI_Send_Data(0x08);
    SPI_Send_Data(0x08);

    SPI_Send_CMD(0xBD);
    SPI_Send_Data(0x06);

    SPI_Send_CMD(0xBC);
    SPI_Send_Data(0x00);

    SPI_Send_CMD(0xFF);
    SPI_Send_Data(0x60);
    SPI_Send_Data(0x01);
    SPI_Send_Data(0x04);

    SPI_Send_CMD(0xC3);
    SPI_Send_Data(0x13);
    SPI_Send_CMD(0xC4);
    SPI_Send_Data(0x13);

    SPI_Send_CMD(0xC9);
    SPI_Send_Data(0x22);

    SPI_Send_CMD(0xBE);
    SPI_Send_Data(0x11);

    SPI_Send_CMD(0xE1);
    SPI_Send_Data(0x10);
    SPI_Send_Data(0x0E);

    SPI_Send_CMD(0xDF);
    SPI_Send_Data(0x21);
    SPI_Send_Data(0x0c);
    SPI_Send_Data(0x02);

    SPI_Send_CMD(0xF0);
    SPI_Send_Data(0x45);
    SPI_Send_Data(0x09);
    SPI_Send_Data(0x08);
    SPI_Send_Data(0x08);
    SPI_Send_Data(0x26);
    SPI_Send_Data(0x2A);

    SPI_Send_CMD(0xF1);
    SPI_Send_Data(0x43);
    SPI_Send_Data(0x70);
    SPI_Send_Data(0x72);
    SPI_Send_Data(0x36);
    SPI_Send_Data(0x37);
    SPI_Send_Data(0x6F);

    SPI_Send_CMD(0xF2);
    SPI_Send_Data(0x45);
    SPI_Send_Data(0x09);
    SPI_Send_Data(0x08);
    SPI_Send_Data(0x08);
    SPI_Send_Data(0x26);
    SPI_Send_Data(0x2A);

    SPI_Send_CMD(0xF3);
    SPI_Send_Data(0x43);
    SPI_Send_Data(0x70);
    SPI_Send_Data(0x72);
    SPI_Send_Data(0x36);
    SPI_Send_Data(0x37);
    SPI_Send_Data(0x6F);

    SPI_Send_CMD(0xED);
    SPI_Send_Data(0x1B);
    SPI_Send_Data(0x0B);

    SPI_Send_CMD(0xAE);
    SPI_Send_Data(0x77);

    SPI_Send_CMD(0xCD);
    SPI_Send_Data(0x63);

    SPI_Send_CMD(0x70);
    SPI_Send_Data(0x07);
    SPI_Send_Data(0x07);
    SPI_Send_Data(0x04);
    SPI_Send_Data(0x0E);
    SPI_Send_Data(0x0F);
    SPI_Send_Data(0x09);
    SPI_Send_Data(0x07);
    SPI_Send_Data(0x08);
    SPI_Send_Data(0x03);

    SPI_Send_CMD(0xE8);
    SPI_Send_Data(0x34);

    SPI_Send_CMD(0x62);
    SPI_Send_Data(0x18);
    SPI_Send_Data(0x0D);
    SPI_Send_Data(0x71);
    SPI_Send_Data(0xED);
    SPI_Send_Data(0x70);
    SPI_Send_Data(0x70);
    SPI_Send_Data(0x18);
    SPI_Send_Data(0x0F);
    SPI_Send_Data(0x71);
    SPI_Send_Data(0xEF);
    SPI_Send_Data(0x70);
    SPI_Send_Data(0x70);

    SPI_Send_CMD(0x63);
    SPI_Send_Data(0x18);
    SPI_Send_Data(0x11);
    SPI_Send_Data(0x71);
    SPI_Send_Data(0xF1);
    SPI_Send_Data(0x70);
    SPI_Send_Data(0x70);
    SPI_Send_Data(0x18);
    SPI_Send_Data(0x13);
    SPI_Send_Data(0x71);
    SPI_Send_Data(0xF3);
    SPI_Send_Data(0x70);
    SPI_Send_Data(0x70);

    SPI_Send_CMD(0x64);
    SPI_Send_Data(0x28);
    SPI_Send_Data(0x29);
    SPI_Send_Data(0xF1);
    SPI_Send_Data(0x01);
    SPI_Send_Data(0xF1);
    SPI_Send_Data(0x00);
    SPI_Send_Data(0x07);

    SPI_Send_CMD(0x66);
    SPI_Send_Data(0x3C);
    SPI_Send_Data(0x00);
    SPI_Send_Data(0xCD);
    SPI_Send_Data(0x67);
    SPI_Send_Data(0x45);
    SPI_Send_Data(0x45);
    SPI_Send_Data(0x10);
    SPI_Send_Data(0x00);
    SPI_Send_Data(0x00);
    SPI_Send_Data(0x00);

    SPI_Send_CMD(0x67);
    SPI_Send_Data(0x00);
    SPI_Send_Data(0x3C);
    SPI_Send_Data(0x00);
    SPI_Send_Data(0x00);
    SPI_Send_Data(0x00);
    SPI_Send_Data(0x01);
    SPI_Send_Data(0x54);
    SPI_Send_Data(0x10);
    SPI_Send_Data(0x32);
    SPI_Send_Data(0x98);

    SPI_Send_CMD(0x74);
    SPI_Send_Data(0x10);
    SPI_Send_Data(0x85);
    SPI_Send_Data(0x80);
    SPI_Send_Data(0x00);
    SPI_Send_Data(0x00);
    SPI_Send_Data(0x4E);
    SPI_Send_Data(0x00);

    SPI_Send_CMD(0x98);
    SPI_Send_Data(0x3e);
    SPI_Send_Data(0x07);

    SPI_Send_CMD(0x35);
    SPI_Send_CMD(0x21);

    SPI_Send_CMD(0x11);
    Delay_Ms(120);
    SPI_Send_CMD(0x29);
    Delay_Ms(20);
}

uint8_t i = 6;
uint16_t COLOR = 0xFFFF;
u8 TxData[480];

static void LCD_Clear(uint16_t color)
{
    unsigned char Line, column;
    LCD_Address_Set(0, 0, 239, 239);
    for (Line = 0; Line < 240; Line++) // Line loop
    {
        for (column = 0; column < 240; column++) // column loop
        {
            SPI_Send_Data(color >> 8);
            SPI_Send_Data(color);
        }
    }
}
static void LCD_Full(uint16_t color)
{
    LCD_Address_Set(0, 0, 239, 239);
    for (int i = 0; i < 480; i += 2)
    {
        TxData[i] = color >> 8;
        TxData[i + 1] = color;
    }

    for (i = 0; i < 240; i++)
    {

        SPI_I2S_DMACmd(SPI1, SPI_I2S_DMAReq_Tx, ENABLE); // SPI1DMA

        DMA_Cmd(DMA1_Channel3, DISABLE);
        DMA_SetCurrDataCounter(DMA1_Channel3, 480);
        DMA_Cmd(DMA1_Channel3, ENABLE);

        while (1)
        {
            if (DMA_GetFlagStatus(DMA1_FLAG_TC3) != RESET)
            {
                DMA_ClearFlag(DMA1_FLAG_TC3);
                break;
            }
        }
    }
}

static void LCD_Pic_display(uint16_t x, uint16_t y, uint16_t length, uint16_t width, uint16_t perLength, const unsigned char *ptr_pic)
{
    uint16_t k;
    Add_set(x, y, x + length - 1, y + width - 1);
    k = length * width;

    for (uint16_t z = 0; z < k / perLength*2; z++)
    {
        for (uint16_t s = 0; s < perLength; s++)
        {
            TxData[s] = *ptr_pic++;
        }

        SPI_I2S_DMACmd(SPI1, SPI_I2S_DMAReq_Tx, ENABLE); // SPI1DMA

        DMA_Cmd(DMA1_Channel3, DISABLE);
        DMA_SetCurrDataCounter(DMA1_Channel3, 480);
        DMA_Cmd(DMA1_Channel3, ENABLE);

        while (1)
        {
            if (DMA_GetFlagStatus(DMA1_FLAG_TC3) != RESET)
            {
                DMA_ClearFlag(DMA1_FLAG_TC3);
                break;
            }
        }
    }
}

int main(void)
{
    Usart_Init();
    SPI_1Line_Tx_Init();
    printf("SPI Init\r\n");

    DMA_Tx_Init(DMA1_Channel3, (u32)&SPI1->DATAR, (u32)TxData, 480);
    DMA_Cmd(DMA1_Channel3, ENABLE);

    // LCD_Init();
    // LCD_Select();
    // LCD_BackLight_ON();
    // printf("LCD init\r\n");
    // LCD_Full(0xFFFF);
    // printf("LCD Clear\r\n");

    TFT_init();

    Delay_Ms(1000);

    int delays = 100;
    while (1)
    {
#if 1

        Delay_Ms(delays);
        LCD_Full(WHITE);
        Delay_Ms(delays);
        LCD_Full(RED);
        Delay_Ms(delays);
        LCD_Full(BLACK);
        Delay_Ms(delays);
        LCD_Full(BLUE);
        Delay_Ms(delays);
        LCD_Full(BRED);
        Delay_Ms(delays);
        LCD_Full(GRED);
        Delay_Ms(delays);
        LCD_Full(GBLUE);
        Delay_Ms(delays);
        LCD_Pic_display(0, 0, 240, 240, 480, gImage_eye);

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

    //     while (1)
    //     {
    //         i = i < 6 ? i + 1 : 0;
    // 	    printf("LCD test: %d \r\n",i);
    //         switch (i) {
    //           case 0:
    //               COLOR = WHITE;
    //               LCD_Full(COLOR);
    //               break;
    //           case 1:
    //               COLOR = BLACK;
    //               LCD_Full(COLOR);
    //               break;
    //           case 2:
    //               COLOR = BLUE;
    //               LCD_Full(COLOR);
    //               break;
    //           case 3:
    //               COLOR = BRED;
    //               LCD_Full(COLOR);
    //               break;
    //           case 4:
    //               COLOR = GRED;
    //               LCD_Full(COLOR);
    //               break;
    //           case 5:
    //               COLOR = GBLUE;
    //               LCD_Full(COLOR);
    //               break;
    //           case 6:
    //               COLOR = RED;
    //               LCD_Full(COLOR);
    //               break;

    //       }
    //     Delay_Ms(100);
    //   }
}