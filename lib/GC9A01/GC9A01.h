/*
 * @FilePath: GC9A01.h
 * @Author: ASWaterbenben
 * @Date: 2022-09-09 11:04:34
 * @LastEditors: ASWaterbenben
 * @LastEditTime: 2022-09-09 16:45:05
 * Copyright: 2022 oulidaTech CO.,LTD. All Rights Reserved.
 * @Descripttion:
 */
#ifndef GC9A01
#define GC9A01

#include <stdint.h>
#include <stdbool.h>
#include <string.h>

#include "GC9A01_port.h"

extern const uint8_t DefaultGamma[36];
extern const bool DefaultMemAccess[6];
extern const uint8_t DefaultPowerControl[3];

#define TFT_COLUMN_NUMBER 240
#define TFT_LINE_NUMBER 240

/*****************颜色数据**************************/
#define WHITE            0xFFFF
#define BLACK            0x0000   
#define BLUE             0x001F  
#define BRED             0XF81F
#define GRED             0XFFE0
#define GBLUE            0X07FF
#define RED              0xF800
#define GREEN            0x07E0

// CG9A01 command code define
#define ENTER_SLEEP_MODE 0x10
#define SLEEP_OUT_MODE 0x11
#define PARTIAL_MODE_ON 0x12
#define NORMAL_DISPLAY_MODE_ON 0x13
#define DISPLAY_INVERSION_OFF 0x20
#define DISPLAY_INVERSION_ON 0x21
#define DISPLAY_OFF 0x28
#define DISPLAY_ON 0x29
#define COLUMN_ADDRESS_SET 0x2A
#define ROW_ADDRESS_SET 0x2B
#define MEMORY_WRITE 0X2C
#define PARTIAL_AREA 0X30
#define VERTICAL_SCROLLING_DEFINITION 0X33
#define TEARING_EFFECT_LINE_OFF 0X34
#define TEARING_EFFECT_LINE_ON 0X35
#define MEMORY_ACCESS_CONTROL 0x36
#define VERTICAL_SCROLLING_START_ADDRESS 0X37
#define IDLE_MODE_OFF 0X38
#define IDLE_MODE_ON 0X39
#define PIXEL_FORMAT_SET 0x3A
#define WRITE_MEMORY_CONTIUE 0X3C
#define SET_TEAR_SCANLINE 0X45
#define WRITE_DIAPLAY_BRIGHTNESS 0X51
#define WRITE_CTRL_DISPLAY 0X53
#define RGB_INTERFACE_SIGNAL_CONTROL 0XB0
#define BLANKING_PORCH_CONTROL 0XB5
#define DISPLAY_FUNCTION_CONTROL 0XB6
#define TEARING_EFFECT_CONTROL 0XBA
#define INTERFACE_CONTROL 0XF6
#define FRAME_RATE 0xE8
#define SPI_2DATA_CONTROL 0XE9
#define POWER_CONTROL_1 0xC1
#define POWER_CONTROL_2 0xC3
#define POWER_CONTROL_3 0xC4
#define POWER_CONTROL_4 0xC9
#define POWER_CONTROL_7 0xA7
#define INTER_REGISTER_ENABLE_1 0xFE
#define INTER_REGISTER_ENABLE_2 0xEF
#define SET_GAMMA_1 0xF0
#define SET_GAMMA_2 0xF1
#define SET_GAMMA_3 0xF2
#define SET_GAMMA_4 0xF3

typedef enum
{
    G1_G32__S1_S360 = 0x00,
    G1_G32__S360_S1 = 0x20,
    G32_G1__S1_S360 = 0x40,
    G32_G1__S360_S1 = 0x60
} DisFuncCtrl;

typedef struct
{
    bool MY;  // Row Address Order
    bool MX;  // Column Address Order
    bool MV;  // Row / Column Exchange
    bool ML;  // Vertical Refresh Order
    bool RGB; // RGB-BGR Order
    bool MH;  // Horizontal Refresh ORDER
} MemAccessCtrl;

typedef enum
{
    P_pixel_16_bit = 0x5,
    P_pixel_18_bit = 0x6
} DPI;

typedef enum
{
    B_pixel_12_bit = 0x3,
    B_pixel_16_bit = 0x5,
    B_pixel_18_bit = 0x6
} DBI;

typedef struct
{
    DPI RGBInterfaceForma;
    DBI MCUInterfaceForma;
} PixFmtSet;

typedef struct
{
    short int vbp; // VREG1A=vdp*0.02+4.8V VREG1B=vdp*0.02+0.3V default:0x13
    short int vbn; // VREG2A=vbn*0.02-4.2V VREG2B=vbn*0.02+0.3V default:0x13
    short int vrh; // VREG1A=vrh*0.02+5.2V VREG2A=-vrh*0.02-2.2V default:0x22
} PowCtrl;

typedef struct
{
    uint8_t G1_dig2j0_n; // 0x01
    uint8_t G1_dig2j1_n; // 0x00
    uint8_t G1_vr1_n;    // 0x05
    uint8_t G1_vr2_n;    // 0x09
    uint8_t G1_vr4_n;    // 0x08
    uint8_t G1_vr6_n;    // 0x08
    uint8_t G1_vr0_n;    // 0x02
    uint8_t G1_vr13_n;   // 0x06
    uint8_t G1_vr20_n;   // 0x2A

    uint8_t G2_vr43_n; // 0x43
    uint8_t G2_vr27_n; // 0x03
    uint8_t G2_vr57_n; // 0x10
    uint8_t G2_vr59_n; // 0x12
    uint8_t G2_vr36_n; // 0x03
    uint8_t G2_vr61_n; // 0x36
    uint8_t G2_vr62_n; // 0x37
    uint8_t G2_vr50_n; // 0x06
    uint8_t G2_vr63_n; // 0x0F

    uint8_t G3_dig2j0_p; // 0x01
    uint8_t G3_dig2j1_p; // 0x00
    uint8_t G3_vr1_p;    // 0x05
    uint8_t G3_vr2_p;    // 0x09
    uint8_t G3_vr4_p;    // 0x08
    uint8_t G3_vr6_p;    // 0x08
    uint8_t G3_vr0_p;    // 0x02
    uint8_t G3_vr13_p;   // 0x06
    uint8_t G3_vr20_p;   // 0x2A

    uint8_t G4_vr43_p; // 0x43
    uint8_t G4_vr27_p; // 0x03
    uint8_t G4_vr57_p; // 0x10
    uint8_t G4_vr59_p; // 0x12
    uint8_t G4_vr36_p; // 0x03
    uint8_t G4_vr61_p; // 0x36
    uint8_t G4_vr62_p; // 0x37
    uint8_t G4_vr50_p; // 0x06
    uint8_t G4_vr63_p; // 0x0F
} SetGamma;

typedef enum
{
    column_inversion = 0x04,
    dot_inversion_1 = 0x14,
    dot_inversion_2 = 0x24,
    dot_inversion_4 = 0x34,
    dot_inversion_8 = 0x44
} FRate;

typedef struct
{
    DisFuncCtrl DFC;
    MemAccessCtrl MAC;
    SetGamma GAMMA_config;
    PixFmtSet Pixel_config;
    PowCtrl Power_Ctrl_config;
    FRate Frame_rate_config;
    void (*lcd_init)(void);
    void (*set_area)(unsigned char x_start, unsigned char y_start, unsigned char x_end, unsigned char y_end);
    void (*clean_scree)(void);
    void (*fill_scree)(unsigned int color);
    void (*print_point)(unsigned char x, unsigned char y, unsigned int color);
    void (*display_pic)(unsigned char x, unsigned char y, unsigned char length, unsigned char width, const unsigned char *ptr_pic);
} GC9A01_IC;

void TFT_config_default(GC9A01_IC *lcd);
void TFT_Rst(void);
void TFT_enable_inter_command(void);
void unknow_function_1(void);
void unknow_function_2(void);
void unknow_function_3(void);
void unknow_function_4(void);
void unknow_function_5(void);
void config_display_function_control(GC9A01_IC *lcd);
void config_memory_access_control(GC9A01_IC *lcd);
void config_pixel_format_set(GC9A01_IC *lcd);
void config_power_control(GC9A01_IC *lcd);
void config_gamma(GC9A01_IC *lcd);
void config_frame_rate(GC9A01_IC *lcd);
void tearing_effect_line_off(void);
void display_inversion_on(void);
void wake_up_lcd(void);
void display_on(void);

void TFT_init(void);
void Add_set(unsigned char x_start,unsigned char y_start,unsigned char x_end,unsigned char y_end);
void TFT_clear(void);
void TFT_full(unsigned int color);
void TFT_dispoint(unsigned char x,unsigned char y,unsigned int color);
void Picture_display(uint16_t x,uint16_t y,uint16_t length,uint16_t width,const unsigned char *ptr_pic);



#endif