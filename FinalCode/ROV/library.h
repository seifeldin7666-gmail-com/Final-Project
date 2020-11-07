# include <Arduino.h>
#include <SPI.h>
#include <Wire.h>
#include <UIPEthernet.h>

#include <stdint.h>

#include "PIDController.h"


#define GRAVITY 9.81
#define ROVMASS 20

#define GET_X 'x'
#define GET_Y  'y'
#define GET_Z  'z'
#define GET_TEMP 't'

#define KP_HOR 3
#define KD_HOR 2

#define KP_VER 10
#define KD_VER 0



#define  BUTTON_UP_f 6
#define  BUTTON_UP_h 4
#define  BUTTON_DOWN_f 5
#define  BUTTON_DOWN_h 3
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
void getForcesOnThrusters();
void send(float ccX , float accY , float temp , float pressure ,float depth);
