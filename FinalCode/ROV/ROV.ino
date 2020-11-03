#include "library.h"

EthernetUDP udp;

#define dir_1 10
#define pwm_1 2
#define dir_2 11
#define pwm_2 3
#define dir_3 7
#define pwm_3 4
#define dir_4 8
#define pwm_4 5
#define dir_5_1 9
#define dir_5_2 12 
#define pwm_5 6 



#define  first_row 4
#define first_col 3
#define second_col 1
float matrix1[4][3]={{-0.3571,0.3571,.25},{0.3571,0.3571,-.25},{0.3571,0.3571,.25},{-0.3571,0.3571,-.25}};
float mul[4][1];
float matrix2[3][1];


uint8_t button;


PIDController pidX,pidY , pidZ;
// double ki , kd , kp;
// double sp,sp2;
double sensorValue;

float accX = 0 , accY = 0 , accZ = 0 , temp = 27;
unsigned int pressure = 1000 , depth = 0;



void setup() {

  pidX.begin();
  pidY.begin();
  pidZ.begin();
  Serial.begin(9600);

  pidX.tune(KP_HOR, KD_HOR,0);
  pidY.tune(KP_HOR, KD_HOR,0);
  pidZ.tune(KP_VER,KD_VER,0);
  pidX.limit(0, 255);
  pidY.limit(0, 255);
  pidZ.limit(0,255);
  pidX.setpoint(0);
  pidY.setpoint(0);
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
  pinMode(dir_5_1,OUTPUT);
  pinMode(dir_5_2,OUTPUT);


  uint8_t mac[6] = {0x00,0x01,0x02,0x03,0x04,0x05}; //0::1::2::3::4::5
  Ethernet.begin(mac,IPAddress(192,168,0,6));//192.168.0.6
  udp.begin(5000); //Port 5000
}
void loop() {


    // Usb.Task();                                                    //Use to read joystick input to controller
    // JoyEvents.PrintValues();                                       //Returns joystick values to user
    // JoyEvents.GetValues( &fx, &fy, &Hat, &Twist, &slider, &Button);   //Copies joystick values to user

  uint8_t *forces = (int8_t*)calloc(sizeof(int8_t) , 3); //FX , FY , Button

  int framesize = udp.parsePacket();
  if(framesize>0){
    do{
        int res= udp.read(forces,framesize+1);
    }
    while ((framesize = udp.available())>0);
    udp.flush(); 
    udp.stop();
  }
  button = forces[2];
    // analogWrite(dir_4,mul[3][0]< 0 ? LOW : HIGH);
  // analogWrite(pwm_1,map(mul[0][0] , 0 , 255 , 0 , 1023));
  switch(button){
    case BUTTON_UP:analogWrite(pwm_5,1023);
                    digitalWrite(dir_5_1 , 1);
                    digitalWrite(dir_5_2,0);
                    break;
    case BUTTON_DOWN:analogWrite(pwm_5,1023);
                    digitalWrite(dir_5_1 , 0);
                    digitalWrite(dir_5_2,1);
                    break;
    default:double _Fz=pidZ.compute(depth);
                //After some mapping Fz = map()
                    analogWrite(pwm_5,1023);
                    digitalWrite(dir_5_1 ,_Fz ? 0 : 1); 
                    digitalWrite(dir_5_2 ,_Fz ? 1 : 0);   
  }
/**
 * forces[0]:FX
 * forces[1]:FY
*/
  pidX.setpoint(forces[0]);//Setting new setpoint
  pidY.setpoint(forces[1]);
  free(forces);

  matrix2[0][0] = pidX.compute( ROVMASS * accX );//Current force on ROV
  matrix2[1][0] = pidY.compute( ROVMASS * accY );
  matrix2[2][0] = 0;
    //Now we apply forces on each thruster
  
  getForcesOnThrusters();

  analogWrite(dir_1,mul[0][0]< 0 ? 0 : 1);
  analogWrite(dir_2,mul[1][0]< 0 ? 0 : 1);
  analogWrite(dir_3,mul[2][0]< 0 ? 0 : 1);
  analogWrite(dir_4,mul[3][0]< 0 ? 0 : 1);
  analogWrite(pwm_1,map(mul[0][0] , 0 , 255 , 0 , 1023));
  analogWrite(pwm_2,map(mul[1][0] , 0 , 255 , 0 , 1023));
  analogWrite(pwm_3,map(mul[2][0] , 0 , 255 , 0 , 1023));
  analogWrite(pwm_4,map(mul[3][0] , 0 , 255 , 0 , 1023));

///////////////////////////////
//Read from nano//////
    if (Serial.available()){
          //{int x , float x , sign x , int y , float y , sign y , pressure / 255 , pressure % 255 , depth}
          accX  = Serial.read() + Serial.read();
          if (Serial.read() == 1)
            accX = (-1) * accX;

          accY  = Serial.read() + Serial.read();
          if (Serial.read() == 1)
            accY = (-1) * accY;

          pressure = Serial.read() * 255 + Serial.read();
          depth = Serial.read();  
    }


    //IMU must be read at any time as it is displayed in GUI
     //Now we begin sending
     uint8_t sensors_readings[5] = {accX , accY , temp , pressure , depth};

     send(sensors_readings);

     if (button == BUTTON_UP ||button == BUTTON_DOWN)
      pidZ.setpoint(depth);
      
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
        }
    }
//    for(i=0;i<first_row;i++)
//    {
//      for(j=0;j<second_col;j++)
//      {
//        Serial.println(mul[i][j]);
//      }
//    }
    delay(1000);
}
  
void send(uint8_t *sensors){
  udp.beginPacket(udp.remoteIP() , udp.remotePort());
  udp.write(sensors,5);
  //Send a variable
  udp.endPacket();
  udp.stop();
  ///Loop is iterated.
}
