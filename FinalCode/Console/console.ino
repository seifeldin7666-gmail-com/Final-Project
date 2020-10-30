#include "library.h"


/**
 * We read from joystick
 * Send data
 * Receive sensors reading
 * Send data to python code
 * 
 * 
 * */
EthernetUDP udp;

IPAddress ROV(192,168,0,6);

void send_data_to_python(uint8_t *sensors){
  Serial.write(sensors, 5);
}



USB                                             Usb;
USBHub                                          Hub(&Usb);
HIDUniversal                                    Hid(&Usb);
JoystickEvents                                  JoyEvents;
JoystickReportParser                            Joy(&JoyEvents);

uint16_t fx , fy;   // 0 - 1023
uint8_t M=0;
uint8_t Hat;    // 0 - 15;
uint8_t Twist , slider;  // 0 - 255
uint8_t Button; // 0 - 12 (0 = No button)
int F;

void setup(){

    Serial.begin(9600);//To send data to python


  Serial.println("Start");
  if (Usb.Init() == -1)
      Serial.println("OSC did not start.");

  if (!Hid.SetReportParser(0, &Joy))
      ErrorMessage<uint8_t>(PSTR("SetReportParser"), 1  );

  uint8_t mac[6] = {0x00,0x01,0x02,0x03,0x04,0x05}; //0::1::2::3::4::6
  Ethernet.begin(udp.remoteIP() , udp.remotePort());//192.168.0.7
  udp.begin(5000); // console port for receiving
}

void loop(){
    Usb.Task();                                                    //Use to read joystick input to controller
    // JoyEvents.PrintValues();                                       //Returns joystick values to user
    JoyEvents.GetValues( &fx, &fy, &Hat, &Twist, &slider, &Button);   //Copies joystick values to user

    //We need to do some operations on forces in order to send them
    
    if ( JOYSTICK_MIN_RANGE<=fx && fx <= JOYSTICK_MAX_RANGE){
        fx = 0;
    }
    if ( JOYSTICK_MIN_RANGE<=fy && fy <= JOYSTICK_MAX_RANGE){
        fy = 0;
    }
    //We need to detect for up and downs
    uint8_t *speed =(uint8_t*) calloc(sizeof(uint8_t) , 3); //FX , FY , button
    speed[0] = fx;
    speed[1] = fy;
    speed[3] = Button;

    udp.beginPacket(ROV , ROV_PORT);
    //Now we send
    udp.write(speed , 3);
    udp.endPacket();
    free(speed);

    ///Now we receive 
    uint8_t size = 0;
    uint8_t *sensors =(uint8_t*) calloc(sizeof(uint8_t) , 5); // 5 sensors
    while(size <=0){
        
        size = udp.parsePacket();
        if (size > 0){
          do{
            sensors = udp.read(sensors , 5);
          }
          while ((size = udp.available()) > 0);
          udp.flush();
          udp.stop();
        }}

     send_data_to_python(sensors);

}
