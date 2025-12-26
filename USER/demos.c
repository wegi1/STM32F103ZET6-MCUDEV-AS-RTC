/*
 * demos.c
 *
 *  Created on: 03.10.2019
 *      Author: bolo
 */

//-------------------------
#include "demos.h"
#include "main.h"
#include "stdlib.h"
#include "main.h"
//-------------------------

//-------------------------
#define sq_size 100
#define rect_size 100
#define t_wait 2


void half_sec_delay(void);
void wait_t(void);


void demo_01(void);
void demo_02(void);
void demo_03(void);
void demo_04(void);
void demo_05(void);
void demo_06(void);
void demo_07(void);
void demo_08(void);
void demo_09(void);
void demo_10(void);
void demo_11(void);
void demo_12(void);
void demo_13(void);
uint8_t test_stop(void) ;
uint32_t randomize(void);
static void LCD_CLS_BLACK(void);
//--------------------------------------------------------
__STATIC_INLINE void DelayMicro(__IO uint32_t micros);
//-------------------------
__STATIC_INLINE void DelayMicro(__IO uint32_t micros)
{
    micros *=(SystemCoreClock / 1000000) / 5;
    while (micros--);
}
static void LCD_CLS_BLACK(void){ LCD_ClrScr(COLOR_565_BLACK) ;}
//-------------------------
void start_demos(void)
{
    demo_01();
    demo_02();
    demo_03();
    demo_04();
    demo_05();
    demo_06();
    demo_07();
    demo_08();
    demo_09();
    demo_10();
    demo_11();

    demo_12();
    demo_13();


    LCD_CLS_BLACK();

    half_sec_delay();
}
//------------------------------
void demo_13(void)
{
    LCD_ClrScr(COLOR_565_BLUE);
    HAL_Delay(1000);
    LCD_CLS_BLACK();
}
void demo_12(void)
{
    u16 color = COLOR_565_BLUE;
    u16 x0,y0;
    u8 movetype = 0; //x++ y++
    u32 counter = 0;

    x0 = rect_size/2;
    y0 = rect_size/2;


    LCD_CLS_BLACK();
    fillCircle(x0, y0, rect_size/2, color);




    while(counter < 1000)
    {
    	if(test_stop() != 0) { half_sec_delay(); return; } // check the stop action
        //-----------
        if(movetype == 0)
        {
            if(x0 < LCD_ROW_NUM-rect_size/2)
            {
                if(y0 < LCD_COL_NUM-rect_size/2 )
                {
                    LCD_DisALoop(x0, y0, rect_size/2+2, 0);
                    LCD_DisALoop(x0, y0, rect_size/2+1, 0);
                    LCD_DisALoop(x0, y0, rect_size/2+3, 0);
                    x0++;
                    y0++;
                    fillCircle(x0, y0, rect_size/2, color);

                    wait_t();
                }
                else
                {
                    movetype = 1; //x++ y--
                }
            }
            else
            {
                movetype = 3; //
            }
        }
        //--------------
        if(movetype == 1)
        {
            if(x0 < LCD_ROW_NUM-rect_size/2)
            {
                if(y0 > rect_size/2 )
                {
                    LCD_DisALoop(x0, y0, rect_size/2+2, 0);
                    LCD_DisALoop(x0, y0, rect_size/2+1, 0);
                    LCD_DisALoop(x0, y0, rect_size/2+3, 0);
                    x0++;
                    y0--;
                    fillCircle(x0, y0, rect_size/2, color);

                    wait_t();
                }
                else
                {
                    movetype = 0; //
                }
            }
            else
            {
                movetype = 2; // x-- y--
            }
        }
        //--------------
        if(movetype == 2)
        {
            if(x0 > rect_size/2)
            {
                if(y0 > rect_size/2 )
                {
                    LCD_DisALoop(x0, y0, rect_size/2+2, 0);
                    LCD_DisALoop(x0, y0, rect_size/2+1, 0);
                    LCD_DisALoop(x0, y0, rect_size/2+3, 0);

                    x0--;
                    y0--;
                    fillCircle(x0, y0, rect_size/2, color);
                    wait_t();
                }
                else
                {
                    movetype = 3; //x-- y++
                }
            }
            else
            {
                movetype = 1; //
            }
        }
        //--------------
        if(movetype == 3)
        {
            if(x0 > rect_size/2)
            {
                if(y0 < LCD_COL_NUM-rect_size/2 )
                {
                    LCD_DisALoop(x0, y0, rect_size/2+2, 0);
                    LCD_DisALoop(x0, y0, rect_size/2+1, 0);
                    LCD_DisALoop(x0, y0, rect_size/2+3, 0);
                    x0--;
                    y0++;
                    fillCircle(x0, y0, rect_size/2, color);
                    wait_t();
                }
                else
                {
                    movetype = 2;
                }
            }
            else
            {
                movetype = 0; // x-- y--
            }
        }
        //--------------



        counter ++;

    }// while(counter < 2000)
    HAL_Delay(1000);
}
//------------------------------
void demo_11(void)
{
    u16 color = COLOR_565_RED;
    u16 x0,y0;
    u8 movetype = 0; //x++ y++

    x0 = rect_size;
    y0 = LCD_COL_NUM/2;


    LCD_CLS_BLACK();
    fillCircle(x0, y0, rect_size, color);


    //-----------
    while(movetype == 0)
    {
    	if(test_stop() != 0) { half_sec_delay(); return; } // check the stop action
        if(x0 < LCD_ROW_NUM-rect_size)
        {
            LCD_DisALoop(x0, y0, rect_size+2, 0);
            LCD_DisALoop(x0, y0, rect_size+1, 0);
            x0++;
            LCD_DisALoop(x0, y0, rect_size, color);
            LCD_DisALoop(x0, y0, rect_size+1, color);
            LCD_DisALoop(x0, y0, rect_size+2, color);
        }
        else
        {
            movetype = 1; //
        }
    }
    //--------------
    while(movetype == 1)
    {
    	if(test_stop() != 0) { half_sec_delay(); return; } // check the stop action
        if(x0 > rect_size+2)
        {
            LCD_DisALoop(x0, y0, rect_size+2, 0);
            LCD_DisALoop(x0, y0, rect_size+1, 0);
            x0--;
            LCD_DisALoop(x0, y0, rect_size, color);
            LCD_DisALoop(x0, y0, rect_size+1, color);
            LCD_DisALoop(x0, y0, rect_size+2, color);
        }
        else
        {
            movetype = 0; // x-- y--
        }
    }


    half_sec_delay();
}
//------------------------------
void demo_10(void)
{
    u16 i;
    LCD_CLS_BLACK();
    //fillCircle(160,120,110,0xf800);
    for(i=0;i<2500;i++)
    {
        fillCircle(randomize()%220 +50,
        		randomize()%140 +50,
				randomize()%50,
				randomize()%0xffff);
        if(test_stop() != 0) { half_sec_delay(); return; } // check the stop action
    }

    HAL_Delay(1000);
}
//------------------------------
void demo_09(void)
{
    u16 x0,y0,x1,y1,color,i;

    LCD_CLS_BLACK();

    for(i=0;i<100;i++)
    {
        color = randomize()%0xffff;
        x0 = randomize()%LCD_ROW_NUM;
        x1 = randomize()%LCD_ROW_NUM;
        y0 = randomize()%LCD_COL_NUM;
        y1 = randomize()%LCD_COL_NUM;

        LCD_No_Fill_Draw(color, x0, y0, x1, y1);
        HAL_Delay(10);
        if(test_stop() != 0) {half_sec_delay(); return; } // check the stop action

    }
    HAL_Delay(1000);

}
//------------------------------
void demo_08(void)
{

	LCD_CLS_BLACK();
    int16_t x0,y0,x1,y1;
    x0 = 0;
    y0 = 0;
    x1 = 4;
    y1 = 239;

    while(y0 < 239)
    {
        LCD_DrawLine(COLOR_565_BLUE,x0,y0,x1,y1);
        y0 = y0+6;
        x1 = x1+8;
        HAL_Delay(10);
        if(test_stop() != 0) { half_sec_delay();  return; } // check the stop action
    }

    x0 = 319;
    y0 = 239;
    x1 = 315;
    y1 = 0;

    while(y0 > 0)
    {
        LCD_DrawLine(COLOR_565_BLUE,x0,y0,x1,y1);
        y0 = y0-6;
        x1 = x1-8;
        HAL_Delay(10);
        if(test_stop() != 0) { half_sec_delay(); return; } // check the stop action
    }

    x0 = 0;
    y0 = 239;
    x1 = 4;
    y1 = 0;

    while(y0 > 0)
    {
        LCD_DrawLine(COLOR_565_RED,x0,y0,x1,y1);
        y0 = y0-6;
        x1 = x1+8;
        HAL_Delay(10);
        if(test_stop() != 0) { half_sec_delay();  return; } // check the stop action
    }

    x0 = 319;
    y0 = 0;
    x1 = 315;
    y1 = 239;

    while(y0 < 239)
    {
        LCD_DrawLine(COLOR_565_RED,x0,y0,x1,y1);
        y0 = y0+6;
        x1 = x1-8;
        HAL_Delay(10);
        if(test_stop() != 0) { half_sec_delay();  return; } // check the stop action
    }
    if(test_stop() != 0) {
    	half_sec_delay();
    	return;
    } // check the stop action
    HAL_Delay(1000);
}
//------------------------------
void demo_07(void)
{
    u16 color = COLOR_565_RED;
    u16 x0,y0,x1,y1;
    u8 movetype = 0; //x++ y++
    u16 counter = 0;
    x0 = 0;
    x1 = x0+sq_size;
    y0 = 0;
    y1 = y0+sq_size;

    LCD_CLS_BLACK();
    LCD_DisARectangular(x0, y0, x1, y1, color);

    while(counter < 1000)
    {
        if(test_stop() != 0) {
        	half_sec_delay();
        	return;
        } // check the stop action
        //-----------
        if(movetype == 0)
        {
            if(x0 < LCD_ROW_NUM-sq_size)
            {
                if(y0 < LCD_COL_NUM-sq_size )
                {
                    LCD_DrawLine(0,x0,y0,x1,y0); //gorna
                    LCD_DrawLine(0,x0,y0,x0,y1); //prawa
                    x0++;
                    x1++;
                    y0++;
                    y1++;
                    LCD_DrawLine(color,x0,y1,x1,y1); //dolna
                    LCD_DrawLine(color,x1,y0,x1,y1); //lewa
                    wait_t();
                }
                else
                {
                    movetype = 1; //x++ y--
                }
            }
            else
            {
                movetype = 3; //
            }
        }
        //--------------
        if(movetype == 1)
        {
            if(x0 < LCD_ROW_NUM-sq_size)
            {
                if(y0 > 0 )
                {
                    LCD_DrawLine(0,x0,y0,x0,y1); //prawa
                    LCD_DrawLine(0,x0,y1,x1,y1); //dolna
                    x0++;
                    x1++;
                    y0--;
                    y1--;
                    LCD_DrawLine(color,x1,y0,x1,y1); //lewa
                    LCD_DrawLine(color,x0,y0,x1,y0); //gorna

                    wait_t();
                }
                else
                {
                    movetype = 0; //
                }
            }
            else
            {
                movetype = 2; // x-- y--
            }
        }
        //--------------
        if(movetype == 2)
        {
            if(x0 > 0)
            {
                if(y0 > 0 )
                {
                    LCD_DrawLine(0,x0,y1,x1,y1); //dolna
                    LCD_DrawLine(0,x1,y0,x1,y1); //lewa
                    x0--;
                    x1--;
                    y0--;
                    y1--;
                    LCD_DrawLine(color,x0,y0,x1,y0); //gorna
                    LCD_DrawLine(color,x0,y0,x0,y1); //prawa
                    wait_t();
                }
                else
                {
                    movetype = 3; //x-- y++
                }
            }
            else
            {
                movetype = 1; //
            }
        }
        //--------------
        if(movetype == 3)
        {
            if(x0 > 0)
            {
                if(y0 < LCD_COL_NUM-sq_size )
                {
                    LCD_DrawLine(0,x0,y0,x1,y0); //gorna
                    LCD_DrawLine(0,x1,y0,x1,y1); //lewa
                    x0--;
                    x1--;
                    y0++;
                    y1++;
                    LCD_DrawLine(color,x0,y1,x1,y1); //dolna
                    LCD_DrawLine(color,x0,y0,x0,y1); //prawa
                    wait_t();
                }
                else
                {
                    movetype = 2;
                }
            }
            else
            {
                movetype = 0; // x-- y--
            }
        }
        //--------------



        counter ++;

    }// while(counter < 2000)

}
//------------------------------
void demo_06(void)
{
    u16 x0,y0,x1,y1,color,i;
    LCD_CLS_BLACK();
    for(i=0;i<2000;i++)
    {
        color = randomize()%0xffff;
        x0 = randomize()%LCD_ROW_NUM;
        x1 = randomize()%LCD_ROW_NUM;
        y0 = randomize()%LCD_COL_NUM;
        y1 = randomize()%LCD_COL_NUM;

        LCD_DisARectangular(x0, y0, x1, y1, color);
        if(test_stop() != 0) {
        	half_sec_delay();
        	return;
        } // check the stop action
    }
    half_sec_delay();

}
//------------------------------
void demo_05(void)
{
    u16 i, color;
    u16 x0,y0,x1,y1;

    LCD_ClrScr(COLOR_565_BLACK);


    for(i=0;i<LCD_COL_NUM;i++)
    {
        LCD_DrawLine(randomize()%0xffff,0,i,LCD_ROW_NUM-1,i);
        if(test_stop() != 0) {
        	half_sec_delay();
        	return;
        } // check the stop action
    }

    half_sec_delay();
    for(i=0;i<LCD_ROW_NUM;i++)
    {
        LCD_DrawLine(randomize()%0xffff,i,0,i,LCD_COL_NUM-1);
        if(test_stop() != 0) {
        	half_sec_delay();
        	return;
        } // check the stop action
    }

    half_sec_delay();

    x0 = 0;
    x1 = LCD_ROW_NUM-1;
    y0 = 0;
    y1 = LCD_COL_NUM-1;

    for(i=0;i<LCD_COL_NUM/2;i++)
    {
        color = randomize()%0xffff;
        LCD_No_Fill_Draw(color,x0,y0,x1,y1);
        x0++;
        x1--;
        y0++;
        y1--;
        if(test_stop() != 0) {
        	half_sec_delay();
        	return;
        } // check the stop action
    }
    half_sec_delay();
}
//------------------------------
void demo_04(void)
{
    u32 i;
    LCD_CLS_BLACK();

    for(i=0;i<1500;i++)
    {
        LCD_DisALoop(randomize()%280+20,
        		randomize()%200+20,
				randomize()%20,
				randomize()%0xffff);
        HAL_Delay(1);
        if(test_stop() != 0) {
        	half_sec_delay();
        	return;
        } // check the stop action
    }
    half_sec_delay();
}
//------------------------------
void demo_03(void)
{
    u32 i;
    LCD_CLS_BLACK();

    for(i=0;i<1000;i++)
    {
        LCD_DrawLine(randomize()%0xffff,randomize()%LCD_ROW_NUM,
        		randomize()%LCD_COL_NUM,randomize()%LCD_ROW_NUM,
				randomize()%LCD_COL_NUM);
        HAL_Delay(1);
        if(test_stop() != 0) {
        	half_sec_delay();
        	return;
        } // check the stop action
    }
    half_sec_delay();
}
//------------------------------
void demo_02(void)
{
    u32 i;
    LCD_CLS_BLACK();
    for(i=0;i<15000;i++)
    {
        LCD_Put_Pixel(randomize()%LCD_ROW_NUM,
        		randomize()%LCD_COL_NUM,
				randomize()%0xffff);
        DelayMicro(95);
        if(test_stop() != 0) {
        	half_sec_delay();
        	LCD_ClrScr(COLOR_565_BLACK);
        	return;
        } // check the stop action
    }
    half_sec_delay();
}
//------------------------------
void demo_01(void)
{
	lcdSetOrientation(LCD_ORIENTATION_LANDSCAPE);
}
//-----------------------------------
void half_sec_delay(void)
{
    HAL_Delay(500);
}
//----------
void wait_t(void)
{
    HAL_Delay(t_wait);
}
//-----------------------------------
u16  getscanline(void)
{
    uint8_t dat1,dat2;
    LCD_REG = 0x45;
    DelayMicro(3);
    dat1 = LCD_RAM;
    dat1 = LCD_RAM;
    dat2 = LCD_RAM;

    return(dat1*256 + dat2);
}
//-----------------------------------
uint32_t randomize(void){


	uint32_t data = (uint32_t) rand();


	return  data ;
}

//=============================================
uint8_t test_stop(void) {
uint8_t result = 0;

	if((BTN_GPIO_Port->IDR & BTN_Pin) == 0){ result = 1 ;} // key1 pressed?
	if((LCDTP_IRQ_GPIO_Port->IDR & LCDTP_IRQ_Pin) == 0){ result = 1 ;} // screen pressed ?
	if(result != 0){ HAL_Delay(20) ;}
	return result;
}
//=========================================================

