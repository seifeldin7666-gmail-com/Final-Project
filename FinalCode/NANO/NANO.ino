#include "library.h"

void setup(){
    Serial.begin(115200);
    IMU_begin();
    PR_begin();
}

void loop(){
    float x = -1 , y = -1 ;
    int pressure = 0 ;
    IMU_readValues();
    x = IMU_getValue(GET_X);
    y = IMU_getValue(GET_Y);
    PR_get_Results(&pressure ,0);
            
    uint8_t toBeSend[10];    
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

    int yaw = IMU_getValue(GET_YAW);
    toBeSend[9] = yaw > 0 ? 0 : 1 ;
    toBeSend[8] = yaw  > 0 ? yaw : -1 * yaw ;

    //{int x , float x , sign x , int y , float y , sign y , pressure / 255 , pressure % 255 , yaw , signOfYaw}
//    Serial.println(IMU_getValue(GET_X));
//    Serial.println(IMU_getValue(GET_Y));
//    Serial.println(IMU_getValue(GET_YAW));
   
    Serial.write(toBeSend , 10);


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
