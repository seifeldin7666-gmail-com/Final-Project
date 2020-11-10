#include "library.h"

EthernetUDP udp;
#define dir_1 12
#define pwm_1 11
#define dir_2 10
#define pwm_2 9
#define dir_3 8
#define pwm_3 7
#define dir_4 6
#define pwm_4 5
#define dir_5 4
#define pwm_5 6 



#define  first_row 4
#define first_col 3
#define second_col 1

PIDController pidHor , pidZ;


float matrix1[4][3]={{-0.3571,0.3571,.25},{0.3571,0.3571,-.25},{0.3571,0.3571,.25},{-0.3571,0.3571,-.25}};
float mul[4][1];
float matrix2[3][1];




float accX = 0 , accY = 0 , accZ = 0 ;
unsigned int pressure = 1000 ;
float depth = 0;
int yaw =  0 ;


void setup() {

  Serial.begin(115200);

  uint8_t mac[6] = {0x00,0x01,0x02,0x03,0x04,0x06}; //0::1::2::3::4::6
  Ethernet.begin(mac,IPAddress(192,168,137,50));//192.168.137.6
  udp.begin(5100); //Port 5000

  pidHor.begin();
  pidZ.begin();

  pidHor.tune(KP_HOR , KD_HOR , 0);
  pidZ.tune(KP_VER,KD_VER,0);
  pidHor.limit(-180, 180);
  pidZ.limit(0,255);
  pidHor.setpoint(0);
  pidZ.setpoint(0);

  pinMode(pwm_1,OUTPUT);
  pinMode(dir_1,OUTPUT);
  pinMode(pwm_2,OUTPUT);
  pinMode(dir_2,OUTPUT);
  pinMode(pwm_3,OUTPUT);
  pinMode(dir_3,OUTPUT);
  pinMode(pwm_4,OUTPUT);
  pinMode(dir_4,OUTPUT);
  pinMode(pwm_5,OUTPUT);
  pinMode(dir_5,OUTPUT);

//  nano.begin(9500);
}
void loop() {
  uint8_t *forces = (uint8_t*)calloc(sizeof(uint8_t) , 3); //FX , FY , Button
  forces[0] = 127;
  forces[1] = 127;

  int framesize = udp.parsePacket();
  if(framesize>0){
//    Serial.println("Received");
    do{
        int res= udp.read(forces,3);

    }
    while ((framesize = udp.available())>0);
    udp.flush(); 
  }
/*
 * Manage vertical motor
 */
  uint8_t button = forces[2];
  Serial.print("Button is : ");
  Serial.println(button);
  button = button < 125 ? map(button , 0 , 124 , -255 , 0) : map(button , 125 , 250 , 0 , 255);  
  digitalWrite(dir_5 , button > 0 ? 0 : 1);
  digitalWrite(pwm_5 , button  > 0 ? button : -1*button);
  
//  switch(button){
//      case BUTTON_UP_f:analogWrite(pwm_5,255);//analogWrite(pwm_5,map(BUTTON_UP_f,0,32,0,255));
//                      digitalWrite(dir_5 , 0);
//                      pidZ.setpoint(depth);
//                      break;
//      case BUTTON_UP_h:analogWrite(pwm_5,126);//analogWrite(pwm_5,map(BUTTON_UP_h,0,16,0,126));
//                      digitalWrite(dir_5 , 0);
//                      pidZ.setpoint(depth);
//                      break;                
//      case BUTTON_DOWN_f:analogWrite(pwm_5,255);//analogWrite(pwm_5,map(BUTTON_DOWN_h,0,25,0,255));
//                      digitalWrite(dir_5 , 1);
//                      pidZ.setpoint(depth);
//                      break;
//      case BUTTON_DOWN_h:analogWrite(pwm_5,126);//analogWrite(pwm_5,map(BUTTON_DOWN_h,0,9,0,126));
//                      digitalWrite(dir_5 , 1);
//                      pidZ.setpoint(depth);
//                      break;
//      default:double _Fz=(-1) * pidZ.compute(depth , 0);
//                  //After some mapping Fz = map()
//                      analogWrite(pwm_5,_Fz);
//                      digitalWrite(dir_5 ,_Fz > 0 ? 0 : 1); 
//  }
/**
 * forces[0]:FX
 * forces[1]:FY
*/
 
//  pidX.setpoint(forces[0]);//Setting new setpoint
//  pidY.setpoint(forces[1]);

  /**
   * Try adding setpoint.
   * System will be moment.
   * Read on gyroscope.
   * We have Yaw angle
   * setpoint 90
   * input 45
   * 45*5 = 180
   */
   //90 map(90 , 0 , 90 , 0 , maximummomment)
   //maximum moment = 100 ... 
//  matrix2[0][0] = forces[0];//pidX.compute( ROVMASS * accX , matrix2[0][0]);//Current force on ROV
  matrix2[0][0] = forces[0] >127 ? map(forces[0] , 128 , 255 , 0 , 255) :  map(forces[0] , 0 , 127 , -255 , 0); 
//  matrix2[1][0] = pidY.compute( ROVMASS * accY , matrix2[1][0]);
  matrix2[1][0] = forces[1] >127 ? map(forces[1] , 128 , 255 , 0 , 255) :  map(forces[1] , 0 , 127 , -255 , 0); 
//  matrix2[1][0] = matrix2[1][0] >127 ? map(matrix2[1][0] , 128 , 255 , 0 , 255) :  map(matrix2[1][0] , 0 , 127 , -255 , 0); 
  matrix2[2][0] = -1 * pidHor.compute(yaw , matrix2[2][0]); //Moment is obtained from yaw.
  matrix2[2][0] = matrix2[2][0] > 0 ? map(matrix2[2][0] , 0 , 180 , 0 , 720) : map(matrix2[2][0] , -180 , 0 , -720 , 0);
  /*------------------------------------------------------------------
    //Now we apply forces on each thruster
    -------------------------------------------------------------------------------*/
  free(forces);
  
  getForcesOnThrusters();
//  Serial.print("F1: ");
//  Serial.print(mul[0][0]);
//  Serial.print("\tF2: ");
//  Serial.print(mul[1][0]);
//  Serial.print("\tF3: ");
//  Serial.print(mul[2][0]);  
//  Serial.print("\tF4: ");
//  Serial.println(mul[3][0]);
   // return;
  analogWrite(dir_1,mul[0][0]< 0 ? 0 : 1);
  analogWrite(dir_2,mul[1][0]< 0 ? 0 : 1);
  analogWrite(dir_3,mul[2][0]< 0 ? 0 : 1);
  analogWrite(dir_4,mul[3][0]< 0 ? 0 : 1);
  analogWrite(pwm_1 , mul[0][0]);
  analogWrite(pwm_2,  mul[1][0]);
  analogWrite(pwm_3 , mul[2][0]);
  analogWrite(pwm_4,  mul[3][0]);
  delay(200);
//  udp.stop();

//Read from nano//////
//Serial.print("Entered");
    uint8_t send[8];
    if (Serial.available()){
          Serial.println("UART REceived");
          //{int x , float x , sign x , int y , float y , sign y , pressure / 255 , pressure % 255 , yaw , signOfYaw}
          send[0] = Serial.read();
          send[1] = Serial.read();
          send[2] = Serial.read();
          
          accX  = (send[0] + (float)send[1] / 100) ;
          if (send[2] == 1)
            accX = (-1) * accX;
          
          send[3] = Serial.read();
          send[4] = Serial.read();
          send[5] = Serial.read();  

          accY  = (send[4] + (float)send[5] / 100) ;
          if (send[5] == 1)
            accY = (-1) * accY;
            
          send[6] = Serial.read();
          send[7] = Serial.read();

          yaw = Serial.read() * 255 + Serial.read();
            
          Serial.flush(); 

          pressure = send[6] * 255 + send[7];
          depth = pressure / (GRAVITY * DENSITY);
          
    }
    else{
       accX = 0 , accY = 0 , accZ = 0 ;
       pressure = 10100 , depth = 0;
    }
//    Serial.print("X  : ");
//    Serial.print(accX);
//    Serial.print("\tY  : ");
//    Serial.print(accY);    
//    Serial.print("\tPressure  : ");
//    Serial.print(pressure);
//    Serial.print("\tDepth  : ");
//    Serial.println(depth);
    //IMU must be read at any time as it is displayed in GUI
return;
//Send data to python
if(udp.beginPacket(IPAddress(192,168,137,100) , 1234)){
Serial.println("Sending...");
uint8_t sned[] = {120,10,154,111};
udp.write(sned , 4);
udp.endPacket();
}

//Send data to console
//udp.beginPacket(udp.remoteIP() ,udp.remotePort());
//udp.write(send , 9);
//udp.endPacket();

      
}

    


//-----------------------------------------------------------------------------------------------------
void getForcesOnThrusters()
{
  int i,j,k;
    for(i=0;i<first_row;i++)
    {
        for(j=0;j<second_col;j++)
        {
        mul[i][j]=0;
        for(k=0;k<first_col;k++)
        {
        mul[i][j]=mul[i][j]+matrix1[i][k]*matrix2[k][j];
        }
        mul[i][j] = mul[i][j] > 0 ? map(mul[i][j] , 0 , 185 , 0 , 255) : map(mul[i][j] , -185 , 0 , -255 , 0); 
        }
    }
//    for(i=0;i<first_row;i++)
//    {
//      for(j=0;j<second_col;j++)
//      {
//        Serial.println(mul[i][j]);
//      }
//    }
//    delay(1000);
}
  
//void send(uint8_t *sensors){
//  udp.beginPacket(udp.remoteIP() , udp.remotePort());
//  udp.write(sensors,5);
//  //Send a variable
//  udp.endPacket();
//  udp.stop();
//  ///Loop is iterated.
//}
