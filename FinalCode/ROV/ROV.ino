#include "library.h"

EthernetUDP udp;

#define  first_row 4
#define first_col 3
#define second_col 1
float matrix1[4][3]={{-0.3571,0.3571,.25},{0.3571,0.3571,-.25},{0.3571,0.3571,.25},{-0.3571,0.3571,-.25}};
float mul[4][1];
float matrix2[3][1];

// CytronMD motor(PWM_DIR, 3, 4);  // PWM = Pin 3, DIR = Pin 4.

CytronMD motor1(PWM_DIR, 3, 4);  // PWM = Pin 3, DIR = Pin 4.
CytronMD motor2(PWM_DIR, 5, 6);  // PWM = Pin 5, DIR = Pin 6.
CytronMD motor3(PWM_DIR, 7, 8);  // PWM = Pin 7, DIR = Pin 8.
CytronMD motor4(PWM_DIR, 9, 10);  // PWM = Pin 9, DIR = Pin 10.
CytronMD motor5(PWM_DIR, 11, 12);  // PWM = Pin 11, DIR = Pin 12. Vertical

uint8_t button;


PIDController FPID,SPID;
double ki , kd , kp;
double sp,sp2;
double sensorValue;

float accX = 0 , accY = 0 , accZ = 0 , temp = 27;
unsigned int pressure = 1000 , depth = 0;



void setup() {

  FPID.begin();
  SPID.begin();
  Serial.begin(9600);
  IMU_begin();
  PR_begin();
  FPID.tune(10, 20,20);
  SPID.tune(10,20,20);
  FPID.limit(0, 255);
  SPID.limit(0,255);
  FPID.setpoint(sp);
  SPID.setpoint(sp2);

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
      button = forces[3];
      switch(button){
        case BUTTON_UP:motor5.setSpeed(255);break;
        case BUTTON_DOWN:motor5.setSpeed(-255);break;
        default:double _Fz=SPID.compute(depth);
                //After some mapping Fz = map()
                motor5.setSpeed((int)_Fz);
      }

      if(forces[0] != 0 || forces[1] != 0)//No readings from joystick
      {
        matrix2[0][0] = forces[0]; //FX
        matrix2[1][0] = forces[2]; //FY
        matrix2[2][0] = 0; //Moment
        free(forces);
      }
    
    else{  //PID
      free(forces);
      double _Fx = FPID.compute(accX);
      double _Fy = FPID.compute(accY);

      matrix2[0][0] = _Fx; //FX
      matrix2[1][0] = _Fy; //FY
      matrix2[2][0] = 0; //Moment
  }
    //Now we apply forces on each thruster
  getForcesOnThrusters();
  motor1.setSpeed(map(forces[0] , 0 , 255 , 0 , 1023));
  motor2.setSpeed(map(forces[1] , 0 , 255 , 0 , 1023));
  motor3.setSpeed(map(forces[2] , 0 , 255 , 0 , 1023));
  motor4.setSpeed(map(forces[3] , 0 , 255 , 0 , 1023));

///////////////////////////////

    IMU_readValues();//Read sensor values

    PR_get_Results(&pressure , 0);//We don't need temperature
    depth = PR_get_depth(pressure);

    //IMU must be read at any time as it is displayed in GUI
     accX = IMU_getValue(GET_X);
     accY = IMU_getValue(GET_Y);
     accZ = IMU_getValue(GET_Z);
     temp = IMU_getValue(GET_TEMP); 
     //Now we begin sending
     uint8_t sensors_readings[5] = {accX , accY , temp , pressure , depth};

     send(sensors_readings);
     


      
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
    for(i=0;i<first_row;i++)
    {
      for(j=0;j<second_col;j++)
      {
        Serial.println(mul[i][j]);
      }
    }
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
