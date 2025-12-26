/*********************************************************************************************************
*
* File                : touch_panel.c
* Hardware Environment: 
* Build Environment   : RealView MDK-ARM  Version: 4.20
* Version             : V1.0
* By                  : 
*
*                                  (c) Copyright 2005-2011, WaveShare
*                                       http://www.waveshare.net
*                                          All Rights Reserved
*
*********************************************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include "touch_panel.h"
#include "GUI.h"
#include "stm32f1xx.h"
#include "main.h"
/* Private variables ---------------------------------------------------------*/
//Matrix matrix ;
Matrix  matrix = {

#define NOHARDWARE_SPI // if Hardware SPI exists - change definition to "HARDWARE_SPI"

#define NOT_OLD_LCD // definition of use old or new LCD

#ifdef NOT_OLD_LCD
//--------------------------------------
//- for old ILI9341 LCD TS BLACK BOARD -
//--------------------------------------
		0x0002c362 ,
		0xFFFFFe7a ,
		0xFF870d1c ,
		0xffffff4c ,
		0x000223bc ,
		0xffa26760 ,
		0x0008476C
#else
//--------------------------------------
//- for new ILI9341 LCD TS BLACK BOARD -
//--------------------------------------
		0x0002C876 ,
		0x0000030C ,
		0xFF73B7B2 ,
		0xFFFFFF4C ,
		0x0001FBA8 ,
		0xFF9D0CC0 ,
		0x0007BA76
#endif
};
Coordinate  display ;


Coordinate ScreenSample[3] = {
								{129 , 162},
								{915 , 150},
								{525 , 860}
};

Coordinate DisplaySample[3] = {
                                {30  , 30},
                                {290 , 30},
                                {160 , 210}
                              };

/* Private define ------------------------------------------------------------*/
#define THRESHOLD 2


/*******************************************************************************
* Function Name  : Delay500ns
* Description    :
* Input          : - cnt:
* Output         : None
* Return         : None
* Attention      : None
*******************************************************************************/
void DelayUS(uint16_t value)
{

	TIM6->SR = 0;
	TIM6->CNT = 0;
	TIM6->ARR = value;
	TIM6->SR = 0;
	TIM6->CR1 = 1;
	while((TIM6->SR &1) == 0 ) {;} // wait for event flag
	TIM6->SR = 0; // clear interrupt pending flag
	TIM6->CR1 = 0;
}


#ifdef HARDWARE_SPI
extern SPI_HandleTypeDef hspi2;

#define TP_hspi hspi2

#else
//=========================================================================================================================
// definitions for software SPI
//=========================================================================================================================


#define SPI_CLK_L()		HAL_GPIO_WritePin(LCDTP_CLK_GPIO_Port, LCDTP_CLK_Pin, GPIO_PIN_RESET)
#define SPI_CLK_H()		HAL_GPIO_WritePin(LCDTP_CLK_GPIO_Port, LCDTP_CLK_Pin, GPIO_PIN_SET)
#define SPI_MOSI_L()	HAL_GPIO_WritePin(LCDTP_DIN_GPIO_Port, LCDTP_DIN_Pin, GPIO_PIN_RESET)
#define SPI_MOSI_H()	HAL_GPIO_WritePin(LCDTP_DIN_GPIO_Port, LCDTP_DIN_Pin, GPIO_PIN_SET)
#define SPI_MISO()		HAL_GPIO_ReadPin(LCDTP_DOUT_GPIO_Port, LCDTP_DOUT_Pin)

//================================================================================================================================
static void spi_write_byte(uint8_t data)
{
	for(size_t i = 0; i < 8; i++)
	{
		if (data & 0x80)
		{
			SPI_MOSI_H();
		}
		else
		{
			SPI_MOSI_L();
		}
		data = data << 1;
		SPI_CLK_L();
		__NOP();
		__NOP();
		__NOP();
		__NOP();
		__NOP();
		__NOP();
		__NOP();
		__NOP();
		__NOP();
		__NOP();
		__NOP();
		__NOP();
		__NOP();
		SPI_CLK_H();
		__NOP();
		__NOP();
		__NOP();
		__NOP();
		__NOP();
		__NOP();
		__NOP();
		__NOP();
		__NOP();
	}
}
//============================================================
static uint16_t spi_read_13bits(void)
{
	uint8_t i,  value;
	uint16_t result = 0;
	i = 13;

	for(i=13; i>0; i--)
	{
		result = result << 1;
		SPI_CLK_L();
		DelayUS(1);
		value = SPI_MISO();
		if (value != 0) { result |= 1;  }
		SPI_CLK_H();

	}
	return result;
}
#endif
//=========================================================================================

/*******************************************************************************
* Function Name  : WR_CMD
* Description    : 
* Input          : - cmd: 
* Output         : None
* Return         : None
* Attention      : None
*******************************************************************************/
static void WR_CMD (uint8_t cmd)  
{ 
#ifdef HARDWARE_SPI
  HAL_SPI_Transmit(&TP_hspi,&cmd,1,1000);
#else
	spi_write_byte(cmd);
#endif
} 



/*******************************************************************************
* Function Name  : RD_AD
* Description    : 
* Input          : None
* Output         : None
* Return         : 
* Attention      : None
*******************************************************************************/
static int RD_AD(void)  
{ 
#ifdef HARDWARE_SPI
	  uint8_t buf[2];
	  int value;
	  HAL_SPI_Receive(&TP_hspi,buf,2,1000);
	  value = (uint16_t)((buf[0] << 8) + buf[1]) >> 3;
	  return value;
#else
  return (int) spi_read_13bits();  // read spi analog data
#endif
} 


/*******************************************************************************
* Function Name  : Read_X
* Description    : Read ADS7843 ADC X 
* Input          : None
* Output         : None
* Return         : 
* Attention      : None
*******************************************************************************/
int Read_X(void)  
{  
  int i; 
  TP_CS(0); 
  DelayUS(1); 
  WR_CMD(CHX); 
  DelayUS(1); 
  i=RD_AD(); 
  TP_CS(1); 
  return i;    
} 

/*******************************************************************************
* Function Name  : Read_Y
* Description    : Read ADS7843 ADC Y
* Input          : None
* Output         : None
* Return         : 
* Attention      : None
*******************************************************************************/
int Read_Y(void)  
{  
  int i; 
  TP_CS(0); 
  DelayUS(1); 
  WR_CMD(CHY); 
  DelayUS(1); 
  i=RD_AD(); 
  TP_CS(1); 
  return i;     
} 


/*******************************************************************************
* Function Name  : TP_GetAdXY
* Description    : Read ADS7843
* Input          : None
* Output         : None
* Return         : 
* Attention      : None
*******************************************************************************/
void TP_GetAdXY(int *x,int *y)  
{ 
  int adx,ady; 
  adx=Read_X(); 
  DelayUS(1); 
  ady=Read_Y(); 
  *x=adx; 
  *y=ady; 
} 



/*******************************************************************************
* Function Name  : DrawCross
* Description    : 
* Input          : - Xpos: Row Coordinate
*                  - Ypos: Line Coordinate 
* Output         : None
* Return         : None
* Attention      : None
*******************************************************************************/
void DrawCross(uint16_t Xpos,uint16_t Ypos)
{

	writeFastHLine(Xpos-13, Ypos, 10, 0xffff);
	writeFastHLine(Xpos+4, Ypos, 10, 0xffff);
	writeFastVLine(Xpos, Ypos-13, 10, 0xffff);
	writeFastVLine(Xpos, Ypos+4, 10, 0xffff);

}  
  
/*******************************************************************************
* Function Name  : Read_Ads7846
* Description    : Get TouchPanel X Y
* Input          : None
* Output         : None
* Return         : Coordinate *
* Attention      : None
*******************************************************************************/
Coordinate *Read_Ads7846(void)
{
  static Coordinate  screen;
//  static uint8_t t6_init = 0;
  int m0,m1,m2,TP_X[1],TP_Y[1],temp[3];
  uint8_t count=0;
  int buffer[2][9]={{0},{0}};
  
//  if(t6_init == 0) {
//	  t6_init = 1;
//	  RCC->APB1ENR |= RCC_APB1ENR_TIM6EN;
//	  __DSB();
//	  TIM6->CR1 = 0;
//	  TIM6->PSC = 71;
//	  TIM6->ARR = 0xFFFF;
//  }

  do
  {       
    TP_GetAdXY(TP_X,TP_Y);  
    buffer[0][count]=TP_X[0];  
    buffer[1][count]=TP_Y[0];
    count++;  
  }
  while(!TP_INT_IN&& count<9);  /* TP_INT_IN  */
  if(count==9)   /* Average X Y  */ 
  {
    /* Average X  */
    temp[0]=(buffer[0][0]+buffer[0][1]+buffer[0][2])/3;
    temp[1]=(buffer[0][3]+buffer[0][4]+buffer[0][5])/3;
    temp[2]=(buffer[0][6]+buffer[0][7]+buffer[0][8])/3;
   
    m0=temp[0]-temp[1];
    m1=temp[1]-temp[2];
    m2=temp[2]-temp[0];
   
    m0=m0>0?m0:(-m0);
    m1=m1>0?m1:(-m1);
    m2=m2>0?m2:(-m2);
   
    if( m0>THRESHOLD  &&  m1>THRESHOLD  &&  m2>THRESHOLD ) return 0;
   
    if(m0<m1)
    {
      if(m2<m0) 
        screen.x=(temp[0]+temp[2])/2;
      else 
        screen.x=(temp[0]+temp[1])/2;  
    }
    else if(m2<m1) 
      screen.x=(temp[0]+temp[2])/2;
    else 
      screen.x=(temp[1]+temp[2])/2;
   
    /* Average Y  */
    temp[0]=(buffer[1][0]+buffer[1][1]+buffer[1][2])/3;
    temp[1]=(buffer[1][3]+buffer[1][4]+buffer[1][5])/3;
    temp[2]=(buffer[1][6]+buffer[1][7]+buffer[1][8])/3;
    m0=temp[0]-temp[1];
    m1=temp[1]-temp[2];
    m2=temp[2]-temp[0];
    m0=m0>0?m0:(-m0);
    m1=m1>0?m1:(-m1);
    m2=m2>0?m2:(-m2);
    if(m0>THRESHOLD&&m1>THRESHOLD&&m2>THRESHOLD) return 0;
   
    if(m0<m1)
    {
      if(m2<m0) 
        screen.y=(temp[0]+temp[2])/2;
      else 
        screen.y=(temp[0]+temp[1])/2;  
      }
    else if(m2<m1) 
       screen.y=(temp[0]+temp[2])/2;
    else
       screen.y=(temp[1]+temp[2])/2;
   
    screen.x = screen.x >> 2;
    screen.y = screen.y >> 2;
    return &screen;
  }
  return 0; 
}
   

/*******************************************************************************
* Function Name  : setCalibrationMatrix
* Description    : Calculate K A B C D E F
* Input          : None
* Output         : None
* Return         : 
* Attention      : None
*******************************************************************************/
FunctionalState setCalibrationMatrix( Coordinate * displayPtr,
                          Coordinate * screenPtr,
                          Matrix * matrixPtr)
{

  FunctionalState retTHRESHOLD = ENABLE ;
  /* K=(X0-X2) (Y1-Y2)-(X1-X2) (Y0-Y2) */
  matrixPtr->Divider = ((screenPtr[0].x - screenPtr[2].x) * (screenPtr[1].y - screenPtr[2].y)) - 
                       ((screenPtr[1].x - screenPtr[2].x) * (screenPtr[0].y - screenPtr[2].y)) ;
  if( matrixPtr->Divider == 0 )
  {
    retTHRESHOLD = DISABLE;
  }
  else
  {
    /* A=((XD0-XD2) (Y1-Y2)-(XD1-XD2) (Y0-Y2))/K  */
    matrixPtr->An = ((displayPtr[0].x - displayPtr[2].x) * (screenPtr[1].y - screenPtr[2].y)) - 
                    ((displayPtr[1].x - displayPtr[2].x) * (screenPtr[0].y - screenPtr[2].y)) ;
  /* B=((X0-X2) (XD1-XD2)-(XD0-XD2) (X1-X2))/K  */
    matrixPtr->Bn = ((screenPtr[0].x - screenPtr[2].x) * (displayPtr[1].x - displayPtr[2].x)) - 
                    ((displayPtr[0].x - displayPtr[2].x) * (screenPtr[1].x - screenPtr[2].x)) ;
    /* C=(Y0(X2XD1-X1XD2)+Y1(X0XD2-X2XD0)+Y2(X1XD0-X0XD1))/K */
    matrixPtr->Cn = (screenPtr[2].x * displayPtr[1].x - screenPtr[1].x * displayPtr[2].x) * screenPtr[0].y +
                    (screenPtr[0].x * displayPtr[2].x - screenPtr[2].x * displayPtr[0].x) * screenPtr[1].y +
                    (screenPtr[1].x * displayPtr[0].x - screenPtr[0].x * displayPtr[1].x) * screenPtr[2].y ;
    /* D=((YD0-YD2) (Y1-Y2)-(YD1-YD2) (Y0-Y2))/K  */
    matrixPtr->Dn = ((displayPtr[0].y - displayPtr[2].y) * (screenPtr[1].y - screenPtr[2].y)) - 
                    ((displayPtr[1].y - displayPtr[2].y) * (screenPtr[0].y - screenPtr[2].y)) ;
    /* E=((X0-X2) (YD1-YD2)-(YD0-YD2) (X1-X2))/K  */
    matrixPtr->En = ((screenPtr[0].x - screenPtr[2].x) * (displayPtr[1].y - displayPtr[2].y)) - 
                    ((displayPtr[0].y - displayPtr[2].y) * (screenPtr[1].x - screenPtr[2].x)) ;
    /* F=(Y0(X2YD1-X1YD2)+Y1(X0YD2-X2YD0)+Y2(X1YD0-X0YD1))/K */
    matrixPtr->Fn = (screenPtr[2].x * displayPtr[1].y - screenPtr[1].x * displayPtr[2].y) * screenPtr[0].y +
                    (screenPtr[0].x * displayPtr[2].y - screenPtr[2].x * displayPtr[0].y) * screenPtr[1].y +
                    (screenPtr[1].x * displayPtr[0].y - screenPtr[0].x * displayPtr[1].y) * screenPtr[2].y ;
  }
  return( retTHRESHOLD ) ;
}

/*******************************************************************************
* Function Name  : getDisplayPoint
* Description    : Touch panel X Y to display X Y
* Input          : None
* Output         : None
* Return         : 
* Attention      : None
*******************************************************************************/
FunctionalState getDisplayPoint(Coordinate * displayPtr,
                     Coordinate * screenPtr,
                     Matrix * matrixPtr )
{
  FunctionalState retTHRESHOLD =ENABLE ;
  /*
  An=168
  */
  if( matrixPtr->Divider != 0 )
  {
    /* XD = AX+BY+C */        
    displayPtr->x = ( (matrixPtr->An * screenPtr->x) + 
                      (matrixPtr->Bn * screenPtr->y) + 
                       matrixPtr->Cn 
                    ) / matrixPtr->Divider ;
    /* YD = DX+EY+F */        
    displayPtr->y = ( (matrixPtr->Dn * screenPtr->x) + 
                      (matrixPtr->En * screenPtr->y) + 
                       matrixPtr->Fn 
                    ) / matrixPtr->Divider ;
  }
  else
  {
    retTHRESHOLD = DISABLE;
  }
  return(retTHRESHOLD);
} 

//***********************************************************************************************
static void releasse_wait(void) {
	// wait for releasse button key and touch screen
	while((HAL_GPIO_ReadPin(LCDTP_IRQ_GPIO_Port, LCDTP_IRQ_Pin)) == 0);
	while((HAL_GPIO_ReadPin(BTN_GPIO_Port, BTN_Pin)) == 0); // R_KEY

}
static void press_wait(void) {
	uint8_t test = 0;

	HAL_Delay(100);
	// wait for press button key or touch screen
	while(test == 0) {
		if((HAL_GPIO_ReadPin(LCDTP_IRQ_GPIO_Port, LCDTP_IRQ_Pin)) == 0) { test = 1 ;}
		if((HAL_GPIO_ReadPin(BTN_GPIO_Port, BTN_Pin)) == 0) { test = 1 ; } // R_KEY

	}
}
//=========================================================
static void check_keys(void) {


	HAL_Delay(100);
	// wait for press button key or touch screen
	press_wait();
	HAL_Delay(50);
	// wait for releasse button key and touch screen
	releasse_wait();
	HAL_Delay(50);
}
//===============================================================================================
static void print_data( int32_t data, char * member) {

 	  LCD_ClrScr(COLOR_565_BLACK);

	  lcdSetCursor(20, 50);
	  lcdPrintf("Matrix.");
	  lcdPrintf(member);
	  my_htoa32(idx , data);

	  lcdSetCursor(20, 100);
	  lcdPrintf((char *) idx);

	  check_keys();

}
static void show_Matrix(void) {
	lcdSetTextColor(0xffff, 0);
	lcdSetTextFont(&Font24);

	LCD_ClrScr(COLOR_565_BLACK);

	lcdSetCursor(20, 50);
	lcdPrintf("MATRIX ADDRESS:");

	lcdSetCursor(20, 100);
	my_htoa32(idx , (u32) &matrix);
	lcdPrintf((char *) idx);


	// wait for releasse touch screen and button key
	releasse_wait();

	// now wait for press touch screen or button key and releasse it
	check_keys();

	print_data(matrix.An, "An")  ;
	print_data(matrix.Bn, "Bn")  ;
	print_data(matrix.Cn, "Cn")  ;
	print_data(matrix.Dn, "Dn")  ;
	print_data(matrix.En, "En")  ;
	print_data(matrix.Fn, "Fn")  ;
	print_data(matrix.Divider, "Divider")  ;
	LCD_ClrScr(COLOR_565_BLACK);
}

//***************************************************************************************************
/*******************************************************************************
* Function Name  : TouchPanel_Calibrate
* Description    : 
* Input          : None
* Output         : None
* Return         : None
* Attention      : None
*******************************************************************************/
void TouchPanel_Calibrate(void)
{
	uint8_t i;
	Coordinate * Ptr;
//	uint8_t logic = 0; // condition to check timelapse
//	uint8_t test;
//	uint32_t tick1, tick2, tickcntr;


	TP_CS(0);
	HAL_Delay(10);
	TP_CS(1);
	HAL_Delay(10);

	HAL_Delay(2000);

	for(i=0;i<3;i++)
	{
		lcdSetTextFont(&Font12);
		LCD_ClrScr(COLOR_565_BLACK);
		lcdSetTextColor(COLOR_565_WHITE, COLOR_565_BLACK );
		lcdSetCursor(5,5);
		lcdPrintf("        Touch crosshair to calibrate");
		lcdSetTextFont(&Font24);
		lcdSetCursor(0,95);
		lcdPrintf("   ILI9341 LCD");
		HAL_Delay(20);
		DrawCross(DisplaySample[i].x,DisplaySample[i].y);
//		test = 0;
//		tickcntr = 0;
//		tick1 = HAL_GetTick();

//		while(test == 0){
//			if(HAL_GPIO_ReadPin(LCDTP_IRQ_GPIO_Port, LCDTP_IRQ_Pin) == 0) { // do calibrate
//				logic =1;
//				test = 1;
//			}
//			if(logic == 0) {
//				while((tick2 = HAL_GetTick()) == tick1){;}
//				tick1 = tick2;
//				tickcntr++;
//				if(tickcntr > 600) { return;} // wait time is over
//			}
//		}
//		logic = 1;

		HAL_Delay(2000); // debounce tactic
		do
		{
			Ptr=Read_Ads7846();
		}
		while( Ptr == (void*)0 );
		ScreenSample[i].x= Ptr->x; ScreenSample[i].y= Ptr->y;

		LCD_ClrScr(COLOR_565_BLACK);

		my_utoa(idx, (uint32_t) Ptr->x);
		lcdSetTextFont(&Font24);
		lcdSetCursor(0,70);
		lcdPrintf("ADC->X = ");
		lcdPrintf((char*) idx);
		lcdPrintf("\nADC->Y = ");
		my_utoa(idx, (uint32_t) Ptr->y);
		lcdPrintf((char*) idx);

		releasse_wait();
		lcdPrintf("\n\n\n  Press TS...");

		press_wait();
		releasse_wait();

	}
	setCalibrationMatrix( &DisplaySample[0],&ScreenSample[0],&matrix );
	LCD_ClrScr(COLOR_565_BLACK);


//===================================================================================================
//===================================================================================================

	i = 0;

	HAL_Delay(1000); // chance for press TS or BTN for Matrix show

	// test for pressed BTN key or TS (Touch Screen)
	if(HAL_GPIO_ReadPin(LCDTP_IRQ_GPIO_Port, LCDTP_IRQ_Pin) == 0) { i = 1;}
	if(HAL_GPIO_ReadPin(BTN_GPIO_Port, BTN_Pin) == 0) { i = 1;} // R_KEY


	// IF TOUCH or BTN KEY WAS PRESSED THAN SHOW MATRIX VALUES
	if(i == 1) { show_Matrix(); }


//===================================================================================================
//===================================================================================================


} 
//============================================================================================================================
static void init_paint(void) {
	LCD_ClrScr(COLOR_565_BLACK);
	lcdSetTextFont(&Font12);
	BSP_LCD_SetTextColor(LCD_COLOR_WHITE);


	BSP_LCD_SetTextColor(LCD_COLOR_RED);
	BSP_LCD_FillRect(5, 200, 30, 30);
	BSP_LCD_SetTextColor(LCD_COLOR_BLUE);
	BSP_LCD_FillRect(40, 200, 30, 30);
	BSP_LCD_SetTextColor(LCD_COLOR_GREEN);
	BSP_LCD_FillRect(75, 200, 30, 30);
	BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
	BSP_LCD_FillRect(110, 200, 30, 30);
	BSP_LCD_SetTextColor(LCD_COLOR_MAGENTA);
	BSP_LCD_FillRect(145, 200, 30, 30);
	BSP_LCD_SetTextColor(LCD_COLOR_ORANGE);
	BSP_LCD_FillRect(180, 200, 30, 30);
	BSP_LCD_SetTextColor(LCD_COLOR_CYAN);
	BSP_LCD_FillRect(215, 200, 30, 30);
	BSP_LCD_SetTextColor(LCD_COLOR_YELLOW);
	BSP_LCD_FillRect(250, 200, 30, 30);

	BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
	BSP_LCD_FillRect(285, 0, 35, 35);
	BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
	BSP_LCD_FillRect(286, 1, 33, 33);
	BSP_LCD_SetTextColor(LCD_COLOR_WHITE);




	lcdSetTextFont(&Font24);
	lcdSetCursor(290, 205);
	lcdPrintf("C");
	lcdSetCursor(294, 7);
	lcdPrintf("X");

	BSP_LCD_DrawHLine(  0, 196, 320);

	BSP_LCD_DrawVLine(  1, 198,  35);
	BSP_LCD_DrawVLine( 37, 198,  35);
	BSP_LCD_DrawVLine( 72, 198,  35);
	BSP_LCD_DrawVLine(107, 198,  35);
	BSP_LCD_DrawVLine(142, 198,  35);

	BSP_LCD_DrawVLine(177, 198,  35);
	BSP_LCD_DrawVLine(212, 198,  35);
	BSP_LCD_DrawVLine(247, 198,  35);
	BSP_LCD_DrawVLine(282, 198,  35);
	BSP_LCD_DrawVLine(317, 198,  35);

	BSP_LCD_DrawHLine(  1, 232, 320);

}
static void TS_banner(void) {
    lcdSetTextFont(&Font12);
	lcdSetCursor(5,5);
	lcdPrintf( "Touch Panel Paint" );
	lcdSetCursor(5,20);
	lcdPrintf( "Example");
}
//================================
static void repaint_Matrix(void) {
	show_Matrix();
	init_paint();
	TS_banner();
}
//======================================
void paint_proc(void) {

    init_paint();
    TS_banner();

	while (1)
	{
		if(HAL_GPIO_ReadPin(BTN_GPIO_Port, BTN_Pin) == 0) {repaint_Matrix();} // R_KEY

		getDisplayPoint(&display, Read_Ads7846(), &matrix );
		if ((display.y < 35) && (display.x > 285))
		{
			u16 bckp = BSP_LCD_GetTextColor();
			LCD_ClrScr(COLOR_565_BLACK);
            BSP_LCD_SetTextColor(bckp);
            while((HAL_GPIO_ReadPin(LCDTP_IRQ_GPIO_Port, LCDTP_IRQ_Pin)) == 0);
            HAL_Delay(50);
            return;
		}

		if(((display.y < 190) && (display.y >= 2)))
		{
			if((display.x >= 318) || (display.x < 2))
			{}
			else
			{
				BSP_LCD_FillCircle(display.x, display.y, 2);
			}
		}
		else if ((display.y <= 230) && (display.y >= 190) && (display.x >= 180) && (display.x <= 210))
		{
			BSP_LCD_SetTextColor(LCD_COLOR_ORANGE);
		}
		else if ((display.y <= 230) && (display.y >= 190) && (display.x >= 215) && (display.x <= 245))
		{
			BSP_LCD_SetTextColor(LCD_COLOR_CYAN);
		}
		else if ((display.y <= 230) && (display.y >= 190) && (display.x >= 250) && (display.x <= 280))
		{
			BSP_LCD_SetTextColor(LCD_COLOR_YELLOW);
		}
		else if ((display.y <= 230) && (display.y >= 190) && (display.x >= 5) && (display.x <= 35))
		{
			BSP_LCD_SetTextColor(LCD_COLOR_RED);
		}
		else if ((display.y <= 230) && (display.y >= 190) && (display.x >= 40) && (display.x <= 70))
		{
			BSP_LCD_SetTextColor(LCD_COLOR_BLUE);
		}
		else if ((display.y <= 230) && (display.y >= 190) && (display.x >= 75) && (display.x <= 105))
		{
			BSP_LCD_SetTextColor(LCD_COLOR_GREEN);
		}
		else if ((display.y <= 230) && (display.y >= 190) && (display.x >= 110) && (display.x <= 140))
		{
			BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
		}
		else if ((display.y <= 230) && (display.y >= 190) && (display.x >= 145) && (display.x <= 175))
		{
			BSP_LCD_SetTextColor(LCD_COLOR_MAGENTA);
		}
		else if ((display.y <= 230) && (display.y >= 190) && (display.x >= 285) && (display.x <= 315))
		{
			u16 bckp = BSP_LCD_GetTextColor();
            init_paint();
            BSP_LCD_SetTextColor(bckp);
            while((HAL_GPIO_ReadPin(LCDTP_IRQ_GPIO_Port, LCDTP_IRQ_Pin)) == 0);
            HAL_Delay(50);
		}


	}
}
/*********************************************************************************************************
      END FILE
*********************************************************************************************************/
