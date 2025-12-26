/*
 * RTC.h
 *
 *  Created on: Sep 21, 2025
 *      Author: BOLO
 */

#ifndef RTC_H_
#define RTC_H_


#include "types_h.h"
#include "inttypes.h"
//=====================================================================================================================
//=====================================================================================================================
//=====================================================================================================================


//=====================================================================================================================
//=====================================================================================================================
//=====================================================================================================================
 

char* getDayofweek(uint8_t number);
int calc_dow(int y, int m,int d);
void set_RTC_DATE(void);

void colon_print(void);
void read_RTC(void);
void print_RTC(void);
void set_color_RTC(void);
void test_print_RTC(void);
void test_setup(void);
bool clear_ss(void);
bool set_hours(void);
void put_number(uint8_t data);
void get_pressed_point(void);
uint8_t test_action(void);
bool test_pressed_point(uint16_t xmin, uint16_t xmax, uint16_t ymin, uint16_t ymax);
bool set_minuts(void);
void check_RTC_DATE(void);
bool set_year(void);
void put_DATE_number(void);
bool set_month(void);
bool set_day(void);
bool setting_globals(void);
void wait_for_releasseTS(void);


void set_LIGHT(void);
void set_PWM(void);
void COLOR_FONTS(void);
void night_mode(void);
void check_night_mode(void);
void CHECK_CALIBRATE_TIME_VALUES(void);
void TIM_CALIBRATE_SETUP(void);
uint8_t calibr_action(void);
void next_page(void);
void test_page(void);
void CAL_TS(void) ;
void SLIDE_SHOW(void);
void DEMOS_RUN(void);

void check_work_orientation(void);
void ROTATION_TEST(void);
void slideshow_disable(void);
void check_slideshow(void);
void night_light(void);
void set_night_PWM(void);
void set_EPOCH(void);
void compare_EPOCH();
void read_timestamp(void);
void SET_CENTURY(void);
void READ_CENTURY(void);


bool go_to_back(void);
bool XPT2046_TouchPressed(void);
void store_timestamp(void);
//=====================================================================================================================
//=====================================================================================================================
//=====================================================================================================================



//=====================================================================================================================
//=====================================================================================================================
//=====================================================================================================================


#endif /* RTC_H_ */
