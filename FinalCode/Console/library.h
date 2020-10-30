
#include <Arduino.h>
#include<UIPEthernet.h>

#include <usbhid.h>
#include <hiduniversal.h>
#include <usbhub.h>

#include "logitech.h"


//Joystick variables////
#define JOYSTICK_MIN_RANGE  505
#define JOYSTICK_MAX_RANGE  515

#define ROV_PORT 5010

#define  BUTTON_UP 12
#define  BUTTON_DOWN 10
#define  NOTHING_PRESSED 0