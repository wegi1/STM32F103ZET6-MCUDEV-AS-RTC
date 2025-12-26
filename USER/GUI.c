/*
 * GUI.c
 *
 *  Created on: Aug 17, 2025
 *      Author: BOLO
 */

#include "GUI.h"
#include "ili9341.h"
#include "inttypes.h"
#include "stdlib.h"
#include "stdio.h"
#include "types_h.h"
#include <stdarg.h>
//****************************************************************************************************************
extern lcdPropertiesTypeDef  lcdProperties ;

extern void FILL_AREA(u16 color, u32 area);



//**************************************************************************************

static lcdFontPropTypeDef lcdFont = {COLOR_565_YELLOW, COLOR_565_BLACK, &Font24, 1};
//static lcdFontPropTypeDef lcdFont = {COLOR_565_YELLOW, COLOR_565_BLACK, 0, 1};
static lcdCursorPosTypeDef cursorXY = {0, 0};

//**************************************************************************************
/********************************************************************************************************
 *  Function: LCD_DisALoop
 *  Object: Display a loop
 *  Input: site,radius and color
 *  Output: none
 *  brief: none
 ********************************************************************************************************/
void LCD_DisALoop(u16 x0, u16 y0, u8 r, u16 Color)
{
    s16 a,b,next;
    a	=	0;
    b = r;
    next = 3 - (r<<1);
    while(a <= b)
    {
    	LCD_Put_Pixel(x0+a, y0-b, Color);
    	LCD_Put_Pixel(x0+b, y0-a, Color);
    	LCD_Put_Pixel(x0+b, y0+a, Color);
    	LCD_Put_Pixel(x0+a, y0+b, Color);
    	LCD_Put_Pixel(x0-a, y0+b, Color);
    	LCD_Put_Pixel(x0-b, y0+a, Color);
    	LCD_Put_Pixel(x0-a, y0-b, Color);
    	LCD_Put_Pixel(x0-b, y0-a, Color);
        a++;
        //use the bresenham
        if(next<0)
            next += 4*a+6;
        else
        {
            next += 10+4*(a-b);
            b--;
        }
    }
}
/********************************************************************************************************
 *  Function: LCD_DisASquare
 *  Object: Display a square
 *  Input: start point, wide, color
 *  Output: none
 *  brief: none
 ********************************************************************************************************/
void LCD_DisASquare(uint16_t x0, uint16_t y0, uint16_t wide, uint16_t Color)
{
    u32 i;

    LCD_OpenWin(x0, y0, x0+wide-1, y0+wide-1);
    for(i = 0; i < (wide*wide); i++) {
    	LCD_RAM = Color;
    }
}
/********************************************************************************************************
 *  Function: LCD_DisARectangular
 *  Object: Display a rectangular
 *  Input: start point, end point, color
 *  Output: none
 *  brief: none
 ********************************************************************************************************/
void LCD_DisARectangular(u16 x0, u16 y0, u16 x1, u16 y1, u16 Color)
{
    u32 area;
    u16 x00, x01, y00, y01;


    if(x0 < x1) {x00 = x0; x01 = x1;}
    else {x00 = x1; x01 = x0;}

    if(y0 < y1) {y00 = y0; y01 = y1;}
    else {y00 = y1; y01 = y0;}

    area = ((1+x01-x00)*(1+y01-y00));
    LCD_OpenWin(x00, y00, x01, y01);
    for(u32 i = 0; i < area; i++){
    	LCD_RAM = Color;
    }

}
//-------------------------------------

//------------------------------------
void LCD_DrawLine(u16 color, u16 x1, u16 y1, u16 x2, u16 y2)
{
    if(y1 == y2)
    {
        LCD_DisARectangular(x1,y1,x2,y2,color);
        return;
    }

    if(x1 == x2)
    {
        LCD_DisARectangular(x1,y1,x2,y2,color);
        return;
    }

    int steep = abs(y2-y1)>abs(x2-x1);
    if (steep)
    {
        swap(x1,y1);
        swap(x2,y2);
    }
    if(x1>x2)
    {
        swap(x1,x2);
        swap(y1,y2);
    }
    int dx,dy;
    dx=x2-x1;
    dy=abs(y2-y1);
    int err=dx/2;
    int ystep;
    if(y1<y2)   ystep = 1;
    else        ystep = -1;

    for (;x1<=x2;x1++)
    {
        if (steep)        LCD_Put_Pixel(y1,x1,color);
        else LCD_Put_Pixel(x1,y1,color);
        err-=dy;
        if (err<0)
        {
            y1 += ystep;
            err+=dx;
        }
    }
}

//------------------------------------

//*******************************************************************************************************
//*******************************************************************************************************

/**************************************************************************/
/*!
   @brief    Draw a circle with filled color
    @param    x0   Center-point x coordinate
    @param    y0   Center-point y coordinate
    @param    r   Radius of circle
    @param    color 16-bit 5-6-5 Color to fill with
 */
/**************************************************************************/
void fillCircle(int16_t x0, int16_t y0, int16_t r, uint16_t color)
{
    writeFastVLine(x0, y0-r, 2*r+1, color);
    fillCircleHelper(x0, y0, r, 3, 0, color);
}
/**************************************************************************/
/*!
    @brief  Quarter-circle drawer with fill, used for circles and roundrects
    @param  x0       Center-point x coordinate
    @param  y0       Center-point y coordinate
    @param  r        Radius of circle
    @param  corners  Mask bits indicating which quarters we're doing
    @param  delta    Offset from center-point, used for round-rects
    @param  color    16-bit 5-6-5 Color to fill with
 */
/**************************************************************************/
void fillCircleHelper(int16_t x0, int16_t y0, int16_t r, uint8_t corners, int16_t delta, uint16_t color)
{

    int16_t f     = 1 - r;
    int16_t ddF_x = 1;
    int16_t ddF_y = -2 * r;
    int16_t x     = 0;
    int16_t y     = r;
    int16_t px    = x;
    int16_t py    = y;

    delta++; // Avoid some +1's in the loop

    while(x < y) {
        if (f >= 0) {
            y--;
            ddF_y += 2;
            f     += ddF_y;
        }
        x++;
        ddF_x += 2;
        f     += ddF_x;
        // These checks avoid double-drawing certain lines, important
        // for the SSD1306 library which has an INVERT drawing mode.
        if(x < (y + 1)) {
            if(corners & 1) writeFastVLine(x0+x, y0-y, 2*y+delta, color);
            if(corners & 2) writeFastVLine(x0-x, y0-y, 2*y+delta, color);
        }
        if(y != py) {
            if(corners & 1) writeFastVLine(x0+py, y0-px, 2*px+delta, color);
            if(corners & 2) writeFastVLine(x0-py, y0-px, 2*px+delta, color);
            py = y;
        }
        px = x;
    }
}
/*****************************************
 * help function to write H | V lines
 *****************************************/
void writeFastVLine(int16_t x, int16_t y, int16_t h, uint16_t color)
{
    LCD_DrawLine(color, x, y, x, y+h-1);
}
void writeFastHLine(int16_t x, int16_t y, int16_t w, uint16_t color)
{
    LCD_DrawLine(color, x, y, x+w-1, y);
}
/*******************************************************************************************************
 * end of screen functions
 *******************************************************************************************************/


/**
 * \brief Draws a character at the specified coordinates
 *
 * \param x			The x-coordinate
 * \param y			The y-coordinate
 * \param c			Character
 * \param color		Character color
 * \param bg		Background color
 * \param size		Character Size
 *
 * \return void
 */
void lcdDrawChar(int16_t x, int16_t y, unsigned char c, uint16_t color, uint16_t bg)
{
	if ((x >= lcdProperties.width) || 			// Clip right
			(y >= lcdProperties.height) || 		// Clip bottom
			((x + lcdFont.pFont->Width) < 0) || // Clip left
			((y + lcdFont.pFont->Height) < 0))  // Clip top
		return;

	uint8_t fontCoeff = lcdFont.pFont->Height / 8;
	uint8_t xP = 0;

	for(uint8_t i = 0; i < lcdFont.pFont->Height; i++)
	{
		uint8_t line;

		for(uint8_t k = 0; k < fontCoeff; k++)
		{
			line = lcdFont.pFont->table[((c - 0x20) * lcdFont.pFont->Height * fontCoeff) + (i * fontCoeff) + k];

			for(uint8_t j = 0; j < 8; j++)
			{
				if((line & 0x80) == 0x80)
				{
					LCD_Put_Pixel(x + j + xP, y + i, color);
				}
				else if (bg != color)
				{
					LCD_Put_Pixel(x + j + xP, y + i, bg);
				}
				line <<= 1;
			}

			xP += 8;
		}

		xP = 0;
	}
}

/**
 * \brief Print the specified Text
 *
 * \param fmt	Format text
 * \param
 *
 * \return void
 */
void lcdPrintf(const char *fmt, ...)
{
	static char buf[256];
	char *p;
	va_list lst;

	va_start(lst, fmt);
	vsprintf(buf, fmt, lst);
	va_end(lst);

	p = buf;
	while (*p)
	{
		if (*p == '\n')
		{
			cursorXY.y += lcdFont.pFont->Height;
			cursorXY.x = 0;
		}
		else if (*p == '\r')
		{
			// skip em
		}
		else if (*p == '\t')
		{
			cursorXY.x += lcdFont.pFont->Width * 4;
		}
		else
		{
			lcdDrawChar(cursorXY.x, cursorXY.y, *p, lcdFont.TextColor, lcdFont.BackColor);
			cursorXY.x += lcdFont.pFont->Width;
			if (lcdFont.TextWrap && (cursorXY.x > (lcdProperties.width - lcdFont.pFont->Width)))
			{
				cursorXY.y += lcdFont.pFont->Height;
				cursorXY.x = 0;
			}
		}
		p++;

		if (cursorXY.y >= lcdProperties.height)
		{
			cursorXY.y = 0;
		}
	}
}

/**
 * \brief Sets the font
 *
 * \param font pointer font
 *
 * \return void
 */
void lcdSetTextFont(sFONT* font)
{
	lcdFont.pFont = font;
}
void BSP_LCD_SetTextColor(u16 color) {
	lcdFont.TextColor = color;
}
u16  BSP_LCD_GetTextColor(void) {
	return (u16) lcdFont.TextColor ;
}
void BSP_LCD_FillRect(u16 x, u16 y, u16 width, u16 height) {

	LCD_DisARectangular(x, y, (x+width-1), (y+height-1), lcdFont.TextColor);
}
void BSP_LCD_DrawHLine(uint16_t Xpos, uint16_t Ypos, uint16_t Length) {
	writeFastHLine(Xpos, Ypos, Length, lcdFont.TextColor);
}
void BSP_LCD_DrawVLine(uint16_t Xpos, uint16_t Ypos, uint16_t Length) {
	writeFastVLine(Xpos, Ypos, Length, lcdFont.TextColor);
}
void BSP_LCD_FillCircle(int16_t x0, int16_t y0, int16_t r)
{
	fillCircle(x0, y0, r, lcdFont.TextColor);
}
/**
 * \brief Sets the text color
 *
 * \param c		Text color
 * \param b		Background color
 *
 * \return void
 */
void lcdSetTextColor(uint16_t c, uint16_t b)
{
	lcdFont.TextColor = c;
	lcdFont.BackColor = b;
}
/*******************************************************************************************************
 * drawing no fill rectangle
 ******************************************************************************************************/
void LCD_No_Fill_Draw(u16 color, u16 x0, u16 y0, u16 x1, u16 y1)
{
    LCD_DrawLine(color,x0,y0,x1,y0);
    LCD_DrawLine(color,x0,y1,x1,y1);
    LCD_DrawLine(color,x0,y0,x0,y1);
    LCD_DrawLine(color,x1,y0,x1,y1);
}
//**************
void lcdSetCursor(unsigned short x, unsigned short y)
{
	cursorXY.x = x;
	cursorXY.y = y;
	//LCD_OpenWin(x, y, x, y);
}
//*********************
