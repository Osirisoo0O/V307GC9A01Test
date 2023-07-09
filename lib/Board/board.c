#include "board.h"
#include "debug.h"


static void Usart_Init()
{
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
    SystemCoreClockUpdate();
    Delay_Init();
    USART_Printf_Init(115200);
    printf("SystemClk: %d\r\n", (int)SystemCoreClock);
    printf("ChipID: %08x\r\n", (unsigned int)DBGMCU_GetCHIPID());
    printf("This is GC9A01 example\r\n");
}

static void Screen_SPI_Init(SPI_TypeDef* SPIx)
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
    SPI_Init(SPIx, &SPI_InitStructure);
    SPI_CalculateCRC(SPIx, DISABLE);

    // 开启DMA
    SPI_I2S_DMACmd(SPIx, SPI_I2S_DMAReq_Tx, ENABLE);

    SPI_Cmd(SPIx, ENABLE);
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
static void DMA_Tx_Init(DMA_Channel_TypeDef *DMA_CHx, u32 ppadr, u32 memadr, u16 bufsize)
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


void Board_Init(void)
{
    Usart_Init();
    Screen_SPI_Init((SPI_TypeDef*)Screen_SPI);
    printf("SPI Init\r\n");

    DMA_Tx_Init(Screen_DMA_Channel, (u32)&SPI1->DATAR, (u32)&TxData, BufferSize);
    DMA_Cmd(Screen_DMA_Channel, ENABLE);
}

void SPI_DMA_Transmit(SPI_TypeDef* SPIx,DMA_Channel_TypeDef * Channelx,uint16_t DataLength)
{
    SPI_I2S_DMACmd(SPIx, SPI_I2S_DMAReq_Tx, ENABLE); // SPI1DMA

    DMA_Cmd(Channelx, DISABLE);
    DMA_SetCurrDataCounter(Channelx, DataLength);
    DMA_Cmd(Channelx, ENABLE);

    while (1)
    {
        if (DMA_GetFlagStatus(DMA1_FLAG_TC3) != RESET)
        {
            DMA_ClearFlag(DMA1_FLAG_TC3);
            break;
        }
    }
}