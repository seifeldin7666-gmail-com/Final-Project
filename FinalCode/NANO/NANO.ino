#include "library.h"

void setup(){
    Serial.begin(9600);
    IMU_begin();
    PR_begin();
//delay(10);
}

void loop(){
    float x = -1 , y = -1 ;
    int pressure = 0 , depth = 0;
    IMU_readValues();
    x = IMU_getValue(GET_X);
    y = IMU_getValue(GET_Y);
    PR_get_Results(&pressure ,0);
    depth = PR_get_depth(pressure);
        
    uint8_t toBeSend[9];    
    toBeSend[2] = x > 0 ? 0 : 1; //Negative    
    toBeSend[2] = x> 0 ? x : (-1) * x;
    uint8_t integerX = (int8_t) x;
    x -= integerX; 

    x *= 100;
    x = getfloatingNums(x);

    toBeSend[5] = y > 0 ? 0 : 1; //Negative    
    toBeSend[5] = y> 0 ? y : (-1) * y;
    uint8_t integerY = (int8_t) y;
    y -= integerY;
    y *= 100;
    y = getfloatingNums(y);
    
    toBeSend[0] = integerX ;
    toBeSend[1] = (int8_t)x;
    toBeSend[3] = integerY;
    toBeSend[4] = (int8_t)y;
    toBeSend[6] = pressure / 255;
    toBeSend[7] = pressure % 255;
    toBeSend[8] = (int8_t)depth;

    //{int x , float x , sign x , int y , float y , sign y , pressure / 255 , pressure % 255 , depth}

    Serial.write(toBeSend , 9);

//Serial.write(100);
    





}

int8_t getfloatingNums(int x){
        if (x % 10 ==0){
        x /= 10;
        if( x % 10 == 0){
            x/=10;
        }
    }
    return x;
}
