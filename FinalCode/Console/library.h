
#include <Arduino.h>
#include<UIPEthernet.h>

#include <usbhid.h>
#include <hiduniversal.h>
#include <usbhub.h>

#include "logitech.h"


//Joystick variables////
#define JOYSTICK_MIN_RANGE  490
#define JOYSTICK_MAX_RANGE  530

#define ROV_PORT 5010

#define  BUTTON_UP_f 6
#define  BUTTON_UP_h 4
#define  BUTTON_DOWN_f 5
#define  BUTTON_DOWN_h 3
