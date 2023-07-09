/*
 * @FilePath: GC9A01.c
 * @Author: ASWaterbenben
 * @Date: 2022-09-09 11:04:18
 * @LastEditors: ASWaterbenben
 * @LastEditTime: 2022-09-09 16:47:31
 * Copyright: 2022 oulidaTech CO.,LTD. All Rights Reserved.
 * @Descripttion: 
 */
#include "GC9A01.h"
#include "debug.h"

const uint8_t DefaultGamma[36] = {0x01, 0x00, 0x05, 0x09, 0x08, 0x08, 0x02, 0x06, 0x2A,
                                  0x43, 0x03, 0x10, 0x03, 0x12, 0x36, 0x37, 0x06, 0x0F,
                                  0x01, 0x00, 0x05, 0x09, 0x08, 0x08, 0x02, 0x06, 0x2A,
                                  0x43, 0x03, 0x10, 0x03, 0x12, 0x36, 0x37, 0x06, 0x0F};

const bool DefaultMemAccess[6] = {0,0,0,0,1,0};

const uint8_t DefaultPowerControl[3] = {0x13, 0x13, 0x22};

const uint8_t DefaultPixelConfig[2] = {0x00, 0x05};

GC9A01_IC TFT_LCD;

/**
 * @description: default GC9A01 some paramter
 * @param {GC9A01_IC} *lcd
 * @return {*}
 * @author: ASWaterbenben
 */
void TFT_config_default(GC9A01_IC *lcd)
{
    lcd->MAC.MY = DefaultMemAccess[0];
    lcd->MAC.MX = DefaultMemAccess[1];
    lcd->MAC.MV = DefaultMemAccess[2];
    lcd->MAC.ML = DefaultMemAccess[3];
    lcd->MAC.RGB = DefaultMemAccess[4];
    lcd->MAC.MH = DefaultMemAccess[5];

    lcd->GAMMA_config.G1_dig2j0_n = DefaultGamma[0];
    lcd->GAMMA_config.G1_dig2j1_n = DefaultGamma[1];
    lcd->GAMMA_config.G1_vr1_n = DefaultGamma[2];
    lcd->GAMMA_config.G1_vr2_n = DefaultGamma[3];
    lcd->GAMMA_config.G1_vr4_n = DefaultGamma[4];
    lcd->GAMMA_config.G1_vr6_n = DefaultGamma[5];
    lcd->GAMMA_config.G1_vr0_n = DefaultGamma[6];
    lcd->GAMMA_config.G1_vr13_n = DefaultGamma[7];
    lcd->GAMMA_config.G1_vr20_n = DefaultGamma[8];

    lcd->GAMMA_config.G2_vr43_n = DefaultGamma[0+9];
    lcd->GAMMA_config.G2_vr27_n = DefaultGamma[1+9];
    lcd->GAMMA_config.G2_vr57_n = DefaultGamma[2+9];
    lcd->GAMMA_config.G2_vr36_n = DefaultGamma[3+9];
    lcd->GAMMA_config.G2_vr59_n = DefaultGamma[4+9];
    lcd->GAMMA_config.G2_vr61_n = DefaultGamma[5+9];
    lcd->GAMMA_config.G2_vr62_n = DefaultGamma[6+9];
    lcd->GAMMA_config.G2_vr50_n = DefaultGamma[7+9];
    lcd->GAMMA_config.G2_vr63_n = DefaultGamma[8+9];

    lcd->GAMMA_config.G3_dig2j0_p = DefaultGamma[0+18];
    lcd->GAMMA_config.G3_dig2j1_p = DefaultGamma[1+18];
    lcd->GAMMA_config.G3_vr1_p = DefaultGamma[2+18];
    lcd->GAMMA_config.G3_vr2_p = DefaultGamma[3+18];
    lcd->GAMMA_config.G3_vr4_p = DefaultGamma[4+18];
    lcd->GAMMA_config.G3_vr6_p = DefaultGamma[5+18];
    lcd->GAMMA_config.G3_vr0_p = DefaultGamma[6+18];
    lcd->GAMMA_config.G3_vr13_p = DefaultGamma[7+18];
    lcd->GAMMA_config.G3_vr20_p = DefaultGamma[8+18];

    lcd->GAMMA_config.G4_vr43_p = DefaultGamma[0+27];
    lcd->GAMMA_config.G4_vr27_p = DefaultGamma[1+27];
    lcd->GAMMA_config.G4_vr57_p = DefaultGamma[2+27];
    lcd->GAMMA_config.G4_vr36_p = DefaultGamma[3+27];
    lcd->GAMMA_config.G4_vr59_p = DefaultGamma[4+27];
    lcd->GAMMA_config.G4_vr61_p = DefaultGamma[5+27];
    lcd->GAMMA_config.G4_vr62_p = DefaultGamma[6+27];
    lcd->GAMMA_config.G4_vr50_p = DefaultGamma[7+27];
    lcd->GAMMA_config.G4_vr63_p = DefaultGamma[8+27];

    lcd->Power_Ctrl_config.vbp = DefaultPowerControl[0];
    lcd->Power_Ctrl_config.vbn = DefaultPowerControl[1];
    lcd->Power_Ctrl_config.vrh = DefaultPowerControl[2];

    lcd->Pixel_config.RGBInterfaceForma = DefaultPixelConfig[0];
    lcd->Pixel_config.MCUInterfaceForma = DefaultPixelConfig[1];

    lcd->Frame_rate_config = dot_inversion_4;
    lcd->DFC = G1_G32__S360_S1;
}

/**
 * @description: reset the lcd
 * @return {*}
 * @author: ASWaterbenben
 */
void TFT_Rst(void)
{
    SPI_RST_0;
    TFT_delay_ms(100);
    SPI_RST_1;
    SPI_CS_0;
    TFT_delay_ms(100);
}

/**
 * @description: enable LCD inter_command
 * @return {*}
 * @author: ASWaterbenben
 */
void TFT_enable_inter_command(void)
{
    TFT_send_cmd(INTER_REGISTER_ENABLE_2);
    TFT_send_cmd(0xEB);//暂不明确0xEB功能
    TFT_send_data(0x14);
    TFT_send_cmd(INTER_REGISTER_ENABLE_1);
    TFT_send_cmd(INTER_REGISTER_ENABLE_2);
    TFT_send_cmd(0xEB);
    TFT_send_data(0x14);
}

/**
 * @description: Wait for test function
 * @return {*}
 * @author: ASWaterbenben
 */
void unknow_function_1(void)
{
    TFT_send_cmd(0x84); 
	TFT_send_data(0x40); 
	TFT_send_cmd(0x85); 
    TFT_send_data(0xFF); 
	TFT_send_cmd(0x86); 
    TFT_send_data(0xFF); 
	TFT_send_cmd(0x87); 
	TFT_send_data(0xFF); 
	TFT_send_cmd(0x88); 
	TFT_send_data(0x0A); 
	TFT_send_cmd(0x89); 
	TFT_send_data(0x21);

    TFT_send_cmd(0x8A); 
    TFT_send_data(0x00);  
	TFT_send_cmd(0x8B); 
	TFT_send_data(0x80);  
	TFT_send_cmd(0x8C); 
	TFT_send_data(0x01);  
	TFT_send_cmd(0x8D); 
	TFT_send_data(0x01);  
	TFT_send_cmd(0x8E); 
	TFT_send_data(0xFF);  
	TFT_send_cmd(0x8F); 
	TFT_send_data(0xFF);
}

/**
 * @description: set GC9A01 diaplay function control register
 * @param {GC9A01_IC} *lcd
 * @return {*}
 * @author: ASWaterbenben
 */
void config_display_function_control(GC9A01_IC *lcd)
{
    TFT_send_cmd(DISPLAY_FUNCTION_CONTROL);
    TFT_send_data(0x00);
    TFT_send_data(lcd->DFC);
}

/**
 * @description: set GC9A01 memory access control register
 * @param {GC9A01_IC} *lcd
 * @return {*}
 * @author: ASWaterbenben
 */
void config_memory_access_control(GC9A01_IC *lcd)
{
    uint8_t data = (lcd->MAC.MY << 7)
                    + (lcd->MAC.MX << 6)
                    + (lcd->MAC.MV << 5)
                    + (lcd->MAC.ML << 4)
                    + (lcd->MAC.RGB << 3)
                    + (lcd->MAC.MH << 2);
    TFT_send_cmd(MEMORY_ACCESS_CONTROL);
    TFT_send_data(data);
}

/**
 * @description: config pixel format set register
 * @param {GC9A01_IC} *lcd
 * @return {*}
 * @author: ASWaterbenben
 */
void config_pixel_format_set(GC9A01_IC *lcd)
{
    TFT_send_cmd(PIXEL_FORMAT_SET);
    uint8_t data = (lcd->Pixel_config.RGBInterfaceForma << 4) + (lcd->Pixel_config.MCUInterfaceForma);
    TFT_send_data(data);
}

/**
 * @description: Wait for test function
 * @return {*}
 * @author: ASWaterbenben
 */
void unknow_function_2(void)
{
    TFT_send_cmd(0x90); 	//xxx
	TFT_send_data(0x08); 
	TFT_send_data(0x08); 
	TFT_send_data(0x08); 
	TFT_send_data(0x08);
	  
	TFT_send_cmd(0xBD); 	//XXX
	TFT_send_data(0x06);
	  
	TFT_send_cmd(0xBC); 	//XXX
	TFT_send_data(0x00); 
	  
	TFT_send_cmd(0xFF); 	//XXX	
	TFT_send_data(0x60); 
	TFT_send_data(0x01); 
	TFT_send_data(0x04);
}

/**
 * @description: config power control register
 * @param {GC9A01_IC} *lcd
 * @return {*}
 * @author: ASWaterbenben
 */
void config_power_control(GC9A01_IC *lcd)
{
    TFT_send_cmd(POWER_CONTROL_2);
	TFT_send_data(lcd->Power_Ctrl_config.vbp);
	  
	TFT_send_cmd(POWER_CONTROL_3);
	TFT_send_data(lcd->Power_Ctrl_config.vbn);
	  
	TFT_send_cmd(POWER_CONTROL_4);
	TFT_send_data(lcd->Power_Ctrl_config.vrh);
}

/**
 * @description: Wait for test function
 * @return {*}
 * @author: ASWaterbenben
 */
void unknow_function_3(void)
{
    TFT_send_cmd(0xBE); 	//XXX
	TFT_send_data(0x11); 
	  
	TFT_send_cmd(0xE1); 	//XXX
	TFT_send_data(0x10);
	TFT_send_data(0x0E); 
	  
	TFT_send_cmd(0xDF); 	//XXX
	TFT_send_data(0x21); 
	TFT_send_data(0x0c); 
	TFT_send_data(0x02); 
}


/**
 * @description: config gamma 1~4 register
 * @param {GC9A01_IC} *lcd
 * @return {*}
 * @author: ASWaterbenben
 */
#define GAMA lcd->GAMMA_config
void config_gamma(GC9A01_IC *lcd)
{
    uint8_t data[6] = {0};

    memset(data, 0, 6);
    data[0] = (GAMA.G1_dig2j0_n<<6) + GAMA.G1_vr0_n;
    data[1] = (GAMA.G1_dig2j1_n<<6) + GAMA.G1_vr2_n;
    data[2] = GAMA.G1_vr4_n;
    data[3] = GAMA.G1_vr6_n;
    data[4] = (GAMA.G1_vr0_n<<4) + GAMA.G1_vr13_n;
    data[5] = GAMA.G1_vr20_n;
    TFT_send_cmd(SET_GAMMA_1);
    TFT_send_data(data[0]);
    TFT_send_data(data[1]);
    TFT_send_data(data[2]);
    TFT_send_data(data[3]);
    TFT_send_data(data[4]);
    TFT_send_data(data[5]);

    memset(data, 0, 6);
    data[0] = GAMA.G2_vr43_n;
    data[1] = (GAMA.G2_vr27_n << 5) + GAMA.G2_vr57_n;
    data[2] = (GAMA.G2_vr36_n <<5)+GAMA.G2_vr59_n;
    data[3] = GAMA.G2_vr61_n;
    data[4] = GAMA.G2_vr62_n;
    data[5] = (GAMA.G2_vr50_n<<4)+GAMA.G2_vr63_n;
    TFT_send_cmd(SET_GAMMA_2);
    TFT_send_data(data[0]);
    TFT_send_data(data[1]);
    TFT_send_data(data[2]);
    TFT_send_data(data[3]);
    TFT_send_data(data[4]);
    TFT_send_data(data[5]);

    memset(data, 0, 6);
    data[0] = (GAMA.G3_dig2j0_p<<6) + GAMA.G3_vr0_p;
    data[1] = (GAMA.G3_dig2j1_p<<6) + GAMA.G3_vr2_p;
    data[2] = GAMA.G3_vr4_p;
    data[3] = GAMA.G3_vr6_p;
    data[4] = (GAMA.G3_vr0_p<<4) + GAMA.G3_vr13_p;
    data[5] = GAMA.G3_vr20_p;
    TFT_send_cmd(SET_GAMMA_3);
    TFT_send_data(data[0]);
    TFT_send_data(data[1]);
    TFT_send_data(data[2]);
    TFT_send_data(data[3]);
    TFT_send_data(data[4]);
    TFT_send_data(data[5]);

    memset(data, 0, 6);
    data[0] = GAMA.G4_vr43_p;
    data[1] = (GAMA.G4_vr27_p << 5) + GAMA.G4_vr57_p;
    data[2] = (GAMA.G4_vr36_p <<5)+GAMA.G4_vr59_p;
    data[3] = GAMA.G4_vr61_p;
    data[4] = GAMA.G4_vr62_p;
    data[5] = (GAMA.G4_vr50_p<<4)+GAMA.G4_vr63_p;
    TFT_send_cmd(SET_GAMMA_4);
    TFT_send_data(data[0]);
    TFT_send_data(data[1]);
    TFT_send_data(data[2]);
    TFT_send_data(data[3]);
    TFT_send_data(data[4]);
    TFT_send_data(data[5]);

}


/**
 * @description: Wait for test function
 * @return {*}
 * @author: ASWaterbenben
 */
void unknow_function_4(void)
{
    TFT_send_cmd(0xED); 	//XXX
	TFT_send_data(0x1B); 
	TFT_send_data(0x0B);
	  
	TFT_send_cmd(0xAE); 	//XXX
	TFT_send_data(0x77);
	  
	TFT_send_cmd(0xCD); 	//XXX
	TFT_send_data(0x63);
	  
	TFT_send_cmd(0x70); 
	TFT_send_data(0x07); 
	TFT_send_data(0x07); 
	TFT_send_data(0x04); 
	TFT_send_data(0x0E); 
	TFT_send_data(0x0F); 
	TFT_send_data(0x09); 
	TFT_send_data(0x07); 
	TFT_send_data(0x08); 
	TFT_send_data(0x03); 
}

/**
 * @description: Set display inversion mode
 * @param {GC9A01_IC} *lcd
 * @return {*}
 * @author: ASWaterbenben
 */
void config_frame_rate(GC9A01_IC *lcd)
{
    TFT_send_cmd(FRAME_RATE);
	TFT_send_data(lcd->Frame_rate_config);
}

/**
 * @description: Wait for test function
 * @return {*}
 * @author: ASWaterbenben
 */
void unknow_function_5(void)
{
    TFT_send_cmd(0x62); // 似乎无影响
    TFT_send_data(0x18);
    TFT_send_data(0x0D);
    TFT_send_data(0x71);
    TFT_send_data(0xED);
    TFT_send_data(0x70);
    TFT_send_data(0x70);
    TFT_send_data(0x18);
    TFT_send_data(0x0F);
    TFT_send_data(0x71);
    TFT_send_data(0xEF);
    TFT_send_data(0x70);
    TFT_send_data(0x70);

    TFT_send_cmd(0x63); // 似乎无影响
    TFT_send_data(0x18);
    TFT_send_data(0x11);
    TFT_send_data(0x71);
    TFT_send_data(0xF1);
    TFT_send_data(0x70);
    TFT_send_data(0x70);
    TFT_send_data(0x18);
    TFT_send_data(0x13);
    TFT_send_data(0x71);
    TFT_send_data(0xF3);
    TFT_send_data(0x70);
    TFT_send_data(0x70);

    TFT_send_cmd(0x64); // 似乎无影响
    TFT_send_data(0x28);
    TFT_send_data(0x29);
    TFT_send_data(0xF1);
    TFT_send_data(0x01);
    TFT_send_data(0xF1);
    TFT_send_data(0x00);
    TFT_send_data(0x07);

    TFT_send_cmd(0x66); // 影响图像带竖纹——1
    TFT_send_data(0x3C);
    TFT_send_data(0x00);
    TFT_send_data(0xCD);
    TFT_send_data(0x67);
    TFT_send_data(0x45);
    TFT_send_data(0x45);
    TFT_send_data(0x10);
    TFT_send_data(0x00);
    TFT_send_data(0x00);
    TFT_send_data(0x00);

    TFT_send_cmd(0x67); // 影响图像带竖纹——1,1\2均取消会导致屏幕模糊变光圈
    TFT_send_data(0x00);
    TFT_send_data(0x3C);
    TFT_send_data(0x00);
    TFT_send_data(0x00);
    TFT_send_data(0x00);
    TFT_send_data(0x01);
    TFT_send_data(0x54);
    TFT_send_data(0x10);
    TFT_send_data(0x32);
    TFT_send_data(0x98);

    TFT_send_cmd(0x74); // 图像刷新流畅度影响
    TFT_send_data(0x10);
    TFT_send_data(0x85);
    TFT_send_data(0x80);
    TFT_send_data(0x00);
    TFT_send_data(0x00);
    TFT_send_data(0x4E);
    TFT_send_data(0x00);

    TFT_send_cmd(0x98); // 似乎无影响
    TFT_send_data(0x3e);
    TFT_send_data(0x07);
}


void tearing_effect_line_off(void)
{
    TFT_send_cmd(TEARING_EFFECT_LINE_OFF);
}

void display_inversion_on(void)
{
    TFT_send_cmd(DISPLAY_INVERSION_ON);
}

void wake_up_lcd(void)
{
    TFT_send_cmd(SLEEP_OUT_MODE);
    TFT_delay_ms(120);
}

void display_on(void)
{
    TFT_send_cmd(DISPLAY_ON);
    TFT_delay_ms(20);
}

/**
 * @description: Init lcd
 * @return {*}
 * @author: ASWaterbenben
 */
void TFT_init(void)
{
//    SPI_BLK_1;
//    TFT_config_default(&TFT_LCD);
//    TFT_Rst();
//    TFT_enable_inter_command(); //OK
//    unknow_function_1();
//    config_display_function_control(&TFT_LCD);
//    config_memory_access_control(&TFT_LCD);
//    config_pixel_format_set(&TFT_LCD);
//    unknow_function_2();
//    config_power_control(&TFT_LCD);
//    unknow_function_3();
//    config_gamma(&TFT_LCD);
//    unknow_function_4();
//    config_frame_rate(&TFT_LCD);
//    unknow_function_5();
//    tearing_effect_line_off();
//    display_inversion_on();
//    wake_up_lcd();
//    display_on();

    SPI_RST_0;//复位
    Delay_Ms(100);
    SPI_RST_1;
    //SPI_CS_0;
    Delay_Ms(100);

    TFT_send_cmd(0xEF); 		//使用指令2
    TFT_send_cmd(0xEB); 		//
    TFT_send_data(0x14);

    TFT_send_cmd(0xFE); 		//使用指令1
    TFT_send_cmd(0xEF); 		//使用指令2  设置inter指令可用完成，复位之后才可关闭

    TFT_send_cmd(0xEB);
    TFT_send_data(0x14);

    TFT_send_cmd(0x84);
    TFT_send_data(0x40);
    TFT_send_cmd(0x85);
    TFT_send_data(0xFF);
    TFT_send_cmd(0x86);
    TFT_send_data(0xFF);
    TFT_send_cmd(0x87);
    TFT_send_data(0xFF);
    TFT_send_cmd(0x88);
    TFT_send_data(0x0A);
    TFT_send_cmd(0x89);
    TFT_send_data(0x21);

    TFT_send_cmd(0x8A);
    TFT_send_data(0x00);

    TFT_send_cmd(0x8B);
    TFT_send_data(0x80);

    TFT_send_cmd(0x8C);
    TFT_send_data(0x01);

    TFT_send_cmd(0x8D);
    TFT_send_data(0x01);

    TFT_send_cmd(0x8E);
    TFT_send_data(0xFF);

    TFT_send_cmd(0x8F);
    TFT_send_data(0xFF);

    TFT_send_cmd(0xB6); 	//外部指令  显示功能控制
    TFT_send_data(0x00); 		//扫描方向控制S1-S360 G1-32
    TFT_send_data(0x20);

    TFT_send_cmd(0x36); 	//调整指令 存储控制 Memory Access Control
    TFT_send_data(0x08); 		//MY MX MV ML BGR MH X X

    TFT_send_cmd(0x3A); 	//调整指令 	Pixel Format Set
    TFT_send_data(0x05); 			//16bit MCU数据模式

    TFT_send_cmd(0x90); 	//xxx
    TFT_send_data(0x08);
    TFT_send_data(0x08);
    TFT_send_data(0x08);
    TFT_send_data(0x08);

    TFT_send_cmd(0xBD); 	//XXX
    TFT_send_data(0x06);

    TFT_send_cmd(0xBC); 	//XXX
    TFT_send_data(0x00);

    TFT_send_cmd(0xFF); 	//XXX
    TFT_send_data(0x60);
    TFT_send_data(0x01);
    TFT_send_data(0x04);

    TFT_send_cmd(0xC3); 	//	Vreg1a voltage Control
    TFT_send_data(0x13);

    TFT_send_cmd(0xC4); 	// Vreg1b voltage Control
    TFT_send_data(0x13);

    TFT_send_cmd(0xC9); 	//Vreg2a voltage Control
    TFT_send_data(0x22);

    TFT_send_cmd(0xBE); 	//XXX
    TFT_send_data(0x11);

    TFT_send_cmd(0xE1); 	//XXX
    TFT_send_data(0x10);
    TFT_send_data(0x0E);

    TFT_send_cmd(0xDF); 	//XXX
    TFT_send_data(0x21);
    TFT_send_data(0x0c);
    TFT_send_data(0x02);

    TFT_send_cmd(0xF0); 	//GAMMA1
    TFT_send_data(0x45);
    TFT_send_data(0x09);
    TFT_send_data(0x08);
    TFT_send_data(0x08);
    TFT_send_data(0x26);
    TFT_send_data(0x2A);

    TFT_send_cmd(0xF1); 	//GAMMA2
    TFT_send_data(0x43);
    TFT_send_data(0x70);
    TFT_send_data(0x72);
    TFT_send_data(0x36);
    TFT_send_data(0x37);
    TFT_send_data(0x6F);

    TFT_send_cmd(0xF2); 	//GAMMA3
    TFT_send_data(0x45);
    TFT_send_data(0x09);
    TFT_send_data(0x08);
    TFT_send_data(0x08);
    TFT_send_data(0x26);
    TFT_send_data(0x2A);

    TFT_send_cmd(0xF3); 	////GAMMA4
    TFT_send_data(0x43);
    TFT_send_data(0x70);
    TFT_send_data(0x72);
    TFT_send_data(0x36);
    TFT_send_data(0x37);
    TFT_send_data(0x6F);

    TFT_send_cmd(0xED); 	//XXX
    TFT_send_data(0x1B);
    TFT_send_data(0x0B);

    TFT_send_cmd(0xAE); 	//XXX
    TFT_send_data(0x77);

    TFT_send_cmd(0xCD); 	//XXX
    TFT_send_data(0x63);

    TFT_send_cmd(0x70);
    TFT_send_data(0x07);
    TFT_send_data(0x07);
    TFT_send_data(0x04);
    TFT_send_data(0x0E);
    TFT_send_data(0x0F);
    TFT_send_data(0x09);
    TFT_send_data(0x07);
    TFT_send_data(0x08);
    TFT_send_data(0x03);

    TFT_send_cmd(0xE8); 	//Frame Rate
    TFT_send_data(0x34);

    TFT_send_cmd(0x62); 	//XXX
    TFT_send_data(0x18);
    TFT_send_data(0x0D);
    TFT_send_data(0x71);
    TFT_send_data(0xED);
    TFT_send_data(0x70);
    TFT_send_data(0x70);
    TFT_send_data(0x18);
    TFT_send_data(0x0F);
    TFT_send_data(0x71);
    TFT_send_data(0xEF);
    TFT_send_data(0x70);
    TFT_send_data(0x70);

    TFT_send_cmd(0x63); 	//XXX
    TFT_send_data(0x18);
    TFT_send_data(0x11);
    TFT_send_data(0x71);
    TFT_send_data(0xF1);
    TFT_send_data(0x70);
    TFT_send_data(0x70);
    TFT_send_data(0x18);
    TFT_send_data(0x13);
    TFT_send_data(0x71);
    TFT_send_data(0xF3);
    TFT_send_data(0x70);
    TFT_send_data(0x70);

    TFT_send_cmd(0x64); 	//XXX
    TFT_send_data(0x28);
    TFT_send_data(0x29);
    TFT_send_data(0xF1);
    TFT_send_data(0x01);
    TFT_send_data(0xF1);
    TFT_send_data(0x00);
    TFT_send_data(0x07);

    TFT_send_cmd(0x66); 	//XXX
    TFT_send_data(0x3C);
    TFT_send_data(0x00);
    TFT_send_data(0xCD);
    TFT_send_data(0x67);
    TFT_send_data(0x45);
    TFT_send_data(0x45);
    TFT_send_data(0x10);
    TFT_send_data(0x00);
    TFT_send_data(0x00);
    TFT_send_data(0x00);

    TFT_send_cmd(0x67); 	//XXX
    TFT_send_data(0x00);
    TFT_send_data(0x3C);
    TFT_send_data(0x00);
    TFT_send_data(0x00);
    TFT_send_data(0x00);
    TFT_send_data(0x01);
    TFT_send_data(0x54);
    TFT_send_data(0x10);
    TFT_send_data(0x32);
    TFT_send_data(0x98);

    TFT_send_cmd(0x74); 	//XXX
    TFT_send_data(0x10);
    TFT_send_data(0x85);
    TFT_send_data(0x80);
    TFT_send_data(0x00);
    TFT_send_data(0x00);
    TFT_send_data(0x4E);
    TFT_send_data(0x00);

    TFT_send_cmd(0x98); 		//XXX
    TFT_send_data(0x3e);
    TFT_send_data(0x07);

    TFT_send_cmd(0x34); 	//Tearing Effect Line Off
    TFT_send_cmd(0x21); 	//Display Inversion ON
    TFT_send_cmd(0x11); 	//唤醒
    //TFT_send_cmd(0x38);	//Idle Mode OFF
    Delay_Ms(120);
    TFT_send_cmd(0x29); 	//显示开
    Delay_Ms(20);
}


/*!
 * @function	:LCD设定操作区域
 * @param x_start	：X轴起始位置
 * @param y_start	：Y周起始位置
 * @param x_end		：X轴结束位置
 * @param y_end		：Y轴结束位置
 */
void Add_set(unsigned char x_start,unsigned char y_start,unsigned char x_end,unsigned char y_end)
{
    TFT_send_cmd(COLUMN_ADDRESS_SET); //设置列地址
    TFT_send_data(0);   //列地址起始
    TFT_send_data(x_start);
    TFT_send_data(0); //列地址结束
    TFT_send_data(x_end);
    TFT_send_cmd(ROW_ADDRESS_SET); //设置行地址
    TFT_send_data(0);   //行地址起始
    TFT_send_data(y_start);
    TFT_send_data(0); //行地址结束
    TFT_send_data(y_end);
    TFT_send_cmd(MEMORY_WRITE); //准备写入显示数据
}


/*!
 * @function	:LCD清屏
 */
void TFT_clear(void)
{
    unsigned char Line, column;
    Add_set(0, 0, TFT_COLUMN_NUMBER - 1, TFT_LINE_NUMBER - 1);
    for (Line = 0; Line < TFT_LINE_NUMBER; Line++) // Line loop
    {
        for (column = 0; column < TFT_COLUMN_NUMBER; column++) // column loop
        {
            TFT_send_data(0x00);
            TFT_send_data(0x00);
        }
    }
}


/*!
 * @function	:LCD完全填充
 * @param color	:填充颜色
 */
void TFT_full(unsigned int color)
{
    unsigned char Line, column;
    Add_set(0, 0, TFT_COLUMN_NUMBER - 1, TFT_LINE_NUMBER - 1);
    for (Line = 0; Line < TFT_LINE_NUMBER; Line++) // Line loop
    {

        for (column = 0; column < TFT_COLUMN_NUMBER; column++) // column loop
        {
            TFT_send_data(color >> 8);
            TFT_send_data(color);
        }
    }
}

/*!
 * @function	:LCD画点
 * @param x		:点所在的X坐标
 * @param y 	:点所在的Y坐标
 * @param color	:点的颜色
 */
void TFT_dispoint(unsigned char x,unsigned char y,unsigned int color)
{
    Add_set(x,y,x,y);
	TFT_send_data(color>>8);
	TFT_send_data(color);
}

/*!
 * @function	:LCD显示图片
 * @param x 	:起始点X坐标
 * @param y 	:起始点Y坐标
 * @param length :图片长度
 * @param width  :图片宽度
 * @param ptr_pic:图片数据指针
 */
void Picture_display(uint16_t x,uint16_t y,uint16_t length,uint16_t width,const unsigned char *ptr_pic)
{
    uint16_t i, j;
    Add_set(x, y, x + length - 1, y + width - 1);
    for (i = 0; i < length; i++)
    {
        for (j = 0; j < width; j++)
        {
            TFT_send_data(*ptr_pic++);
            TFT_send_data(*ptr_pic++);
        }
    }
}
