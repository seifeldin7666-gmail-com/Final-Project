# include <Arduino.h>
#include <SPI.h>
#include <Wire.h>
#include <UIPEthernet.h>

#include <stdint.h>

#include "CytronMotorDriver.h"
#include "PIDController.h"


#define GRAVITY 9.81

#define GET_X 'x'
#define GET_Y  'y'
#define GET_Z  'z'
#define GET_TEMP 't'

#define  BUTTON_UP 12
#define  BUTTON_DOWN 10
#define  NOTHING_PRESSED 0
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
