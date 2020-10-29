#include "library.h"

void setup(){
    Serial.begin(9600);
    IMU_begin();
    PR_begin();
}

void loop(){
    IMU_readValues();
    float accX = IMU_getValue(GET_X);
    float accY = IMU_getValue(GET_Y);
    float accZ = IMU_getValue(GET_Z);

    unsigned int pressure;
    PR_get_Results(&pressure , 0);//We don't need temperature
    unsigned int depth = PR_get_depth(pressure);

    //Inputs of PID:
    /**
     * X: accX
     * Y: accY
     * depth: Depth of ROV
     * */

     Serial.print("X is ");
     Serial.print(accX);
     Serial.print("\tY is ");
     Serial.print(accY);
     Serial.print("\tZ is ");
     Serial.print(accZ);
     Serial.print("\t Pressure is ");
     Serial.print(pressure);
     Serial.print("\tDepth is ");
     Serial.println(depth);      

     delay(2000);
    

}
