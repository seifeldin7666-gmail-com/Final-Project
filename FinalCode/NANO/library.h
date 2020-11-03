# include <Arduino.h>
#include <SPI.h>
#include <Wire.h>

#include <stdint.h>

#define GRAVITY 9.81

#define GET_X 'x'
#define GET_Y  'y'
#define GET_Z  'z'
#define GET_TEMP 't'

///IMU Functions/////////////
void IMU_begin();
void IMU_readValues();
float IMU_getValue(char type );
///////////////////////////////

///////////Pressure functions//////////////
void PR_begin();
void resetsensor();
void PR_get_Results(unsigned int *pressure , float *temp);
unsigned int PR_get_depth(unsigned int pressure);
//////////////////////////////////////////