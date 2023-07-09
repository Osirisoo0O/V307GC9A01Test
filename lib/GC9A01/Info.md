# GC9A01使用简述

## 引脚说明

| 引脚名称 | 用途           | 备注                       |
| -------- | -------------- | -------------------------- |
| VCC      | +3.3V供电      |                            |
| GND      | 供电地         |                            |
| SCL      | SPI的CLK信号   |                            |
| SDA      | SPI的MOSI信号  |                            |
| RES      | 复位引脚       | 将屏幕复位，低电平有效     |
| DC       | 数据命令控制脚 | 高电平为数据、低电平为命令 |
| CS       | SPI片选信号    | 低电平有效                 |
| BLK      | 背光LED        | 可PWM调光                  |



## 屏幕基础参数

| 参数     | 数据                   | 备注                                  |
| -------- | ---------------------- | ------------------------------------- |
| 分辨率   | 240*240                |                                       |
| 颜色类型 | RGB565彩色             | 每个像素点需要一个16bit的数据进行显示 |
| 驱动IC   | GC9A01                 |                                       |
| 通信接口 | 4 Line SPI             |                                       |
| 尺寸     | 1.28                   | 单位：英寸                            |
| 显示直径 | 32.4                   | 单位：mm                              |
| 像素间距 | 0.135                  | 单位：mm                              |
| 背光类型 | 2个平行白色LED         |                                       |
| 视角方向 | ALL View               |                                       |
| 面板尺寸 | 36.5(H)×38.1(V)×1.5(D) | 单位：mm                              |
| 工作电压 | 3.3                    | 单位：V                               |
| 工作电流 | 20                     | 单位：mA                              |
| 工作温度 | -20~70                 | 单位：℃                               |



## 屏幕控制相关寄存器

| 寄存器名称                   | 寄存器ID                                 | 默认参数                             | 说明     |
| ---------------------------- | ---------------------------------------- | ------------------------------------ | -------- |
| **Inter Register Enable1**   | 0xFE                                     | 1+2=enable Inter_command             |          |
| **Inter Register Enable2**   | 0xEF                                     | 1+2=enable Inter_command             |          |
| **Display Function Control** | 0xB6                                     | 0x00 0x20                            | 扫描方向 |
| **Memory Access Control**    | 0x36                                     | 设置显示起始点                       |          |
| **Pixel Format Set**         | 0x3A                                     | 设置RGB彩色接收格式（12、16、18bit） |          |
| **Power Control**            | 0xC3<br />0xC4<br />0xC9                 | 设置LCD的电压输出电平                |          |
| **Set GAMMA**                | 0xF0<br />0xF1<br />0xF2<br />0xF3<br /> | 设置LCD显示伽马值                    |          |
| **Frame Rate**               | 0xE8                                     | 设置显示的翻转模式                   |          |
| **Tearing Effect Line OFF**  | 0x34                                     | 关闭撕裂效果                         |          |
| **Display Inversion ON**     | 0x21                                     | 进入翻转显示模式                     |          |
| **Sleep Out Mode**           | 0x11                                     | 退出休眠模式                         |          |
| **Display ON**               | 0x29                                     | 使能显示功能                         |          |



## 基础控制接口函数

### SPI发送一个字节

```c
/*!
 * @function	:SPI发送一个字节
 * @param byte	:需要发送的数据
 */
void SPI_SendByte(unsigned char byte)
```



### LCD发送命令

```c
/*!
 * @function	:LCD发送命令
 * @param o_command ：需要发送的命令编号
 */
void TFT_send_cmd(unsigned char o_command)
```



### LCD发送数据

```c
/*!
 * @function	:LCD发送数据
 * @param o_data ：需要发送的数据内容
 */
void TFT_send_data(unsigned char o_data)
```



### LCD初始化

```c
/*!
 * @function	:LCD初始化
 */
void TFT_init(void)
```



### LCD设定显示范围

```c
/*!
 * @function	:LCD设定操作区域
 * @param x_start	：X轴起始位置
 * @param y_start	：Y周起始位置
 * @param x_end		：X轴结束位置
 * @param y_end		：Y轴结束位置
 */
void Add_set(unsigned char x_start,unsigned char y_start,unsigned char x_end,unsigned char y_end)
```



### LCD清屏

```c
/*!
 * @function	:LCD清屏
 */
void TFT_clear(void)
```



### LCD完全填充

```C
/*!
 * @function	:LCD完全填充
 * @param color	:填充颜色
 */
void TFT_full(unsigned int color)
```



### LCD画点

```C
/*!
 * @function	:LCD画点
 * @param x		:点所在的X坐标
 * @param y 	:点所在的Y坐标
 * @param color	:点的颜色
 */
void TFT_dispoint(unsigned char x,unsigned char y,unsigned int color)
```



### LCD显示图片

```c
/*!
 * @function	:LCD显示图片
 * @param x 	:起始点X坐标
 * @param y 	:起始点Y坐标
 * @param length :图片长度
 * @param width  :图片宽度
 * @param ptr_pic:图片数据指针
 */
void Picture_display(u16 x,u16 y,u16 length,u16 width,const unsigned char *ptr_pic)
```



## 使用Demo
```c
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_SPI1_Init();
  MX_USART1_UART_Init();
  /* USER CODE BEGIN 2 */
    TFT_init();
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1) {
      /* USER CODE END WHILE */

      /* USER CODE BEGIN 3 */
      i = i < 7 ? i + 1 : 0;
      switch (i) {
          case 0:
              COLOR = WHITE;
              TFT_full(COLOR);
              break;
          case 1:
              COLOR = BLACK;
              TFT_full(COLOR);
              break;
          case 2:
              COLOR = BLUE;
              TFT_full(COLOR);
              break;
          case 3:
              COLOR = BRED;
              TFT_full(COLOR);
              break;
          case 4:
              COLOR = GRED;
              TFT_full(COLOR);
              break;
          case 5:
              COLOR = GBLUE;
              TFT_full(COLOR);
              break;
          case 6:
              COLOR = RED;
              TFT_full(COLOR);
              break;
          case 7:
              Picture_display(0, 0, 240, 240, gImage_eye);
              Delay_Ms(3000);
              break;
      }
      Delay_Ms(1000);
  }
  /* USER CODE END 3 */
}
```
