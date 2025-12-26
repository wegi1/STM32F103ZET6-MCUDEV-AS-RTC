/*
 * GUI.h
 *
 *  Created on: Aug 17, 2025
 *      Author: BOLO
 */

#ifndef INC_GUI_H_
#define INC_GUI_H_

#include <fonts.h>
#include "inttypes.h"
#include  "main.h"
#include "ili9341.h"


//*******************************************************************************************************



/**
  * @brief  Draw Properties structures definition
  */
typedef struct
{
	uint32_t 	TextColor;
	uint32_t 	BackColor;
	sFONT*    	pFont;
	uint8_t		TextWrap;
}lcdFontPropTypeDef;

typedef struct
{
	unsigned short	x;
	unsigned short	y;
}lcdCursorPosTypeDef;


//***************************************************************************************************
void LCD_DisASquare(uint16_t x0, uint16_t y0, uint16_t wide, uint16_t Color);
void LCD_DisARectangular(u16 x0, u16 y0, u16 x1, u16 y1, u16 Color);

void LCD_DisALoop(u16 x0, u16 y0, u8 r, u16 Color);

void LCD_DrawLine(uint16_t color, uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2);


void fillCircle(int16_t x0, int16_t y0, int16_t r, uint16_t color);
void fillCircleHelper(int16_t x0, int16_t y0, int16_t r, uint8_t corners, int16_t delta, uint16_t color);
void writeFastVLine(int16_t x, int16_t y, int16_t h, uint16_t color);
void writeFastHLine(int16_t x, int16_t y, int16_t w, uint16_t color);

//************************************************************************************

void lcdHome(void);
void lcdDrawChar(int16_t x, int16_t y, unsigned char c, uint16_t color, uint16_t bg);
void lcdPrintf(const char *fmt, ...);
void lcdSetTextFont(sFONT* font);
void lcdSetTextColor(uint16_t c, uint16_t b);
void lcdSetTextWrap(uint8_t w);
void lcdSetCursor(unsigned short x, unsigned short y);

void BSP_LCD_SetTextColor(u16 color) ;
u16  BSP_LCD_GetTextColor(void);
void BSP_LCD_FillRect(u16 x, u16 y, u16 width, u16 height);
void BSP_LCD_DrawHLine(uint16_t Xpos, uint16_t Ypos, uint16_t Length);
void BSP_LCD_DrawVLine(uint16_t Xpos, uint16_t Ypos, uint16_t Length);
void BSP_LCD_FillCircle(int16_t x0, int16_t y0, int16_t r);
void LCD_No_Fill_Draw(u16 color, u16 x0, u16 y0, u16 x1, u16 y1);
//************************************************************************************
#endif /* INC_GUI_H_ */
