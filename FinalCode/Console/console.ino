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

IPAddress ROV(192,168,137,6);
#define ROVPORT 5100


uint16_t fx , fy;   // 0 - 1023
uint8_t Button; // 0 - 12 (0 = No button)

USB                                             Usb;
USBHub                                          Hub(&Usb);
HIDUniversal                                    Hid(&Usb);
JoystickEvents                                  JoyEvents;
JoystickReportParser                            Joy(&JoyEvents);



void setup(){
  
  uint8_t mac[6] = {0x00,0x01,0x02,0x03,0x04,0x05}; //0::1::2::3::4::5
  Ethernet.begin(mac , IPAddress(192,168,137,7));//192.168.0.7
  udp.begin(5000); // console port for receiving
  
  Serial.begin(115200);//To send data to python

  
  if (Usb.Init() == -1)
      Serial.println("OSC did not start.");

  if (!Hid.SetReportParser(0, &Joy))
      ErrorMessage<uint8_t>(PSTR("SetReportParser"),1);

}

void loop(){
 
    Usb.Task();                                                    //Use to read joystick input to controller
    // JoyEvents.PrintValues();                                       //Returns joystick values to user
    JoyEvents.GetValues( &fx, &fy, 0, 0 , 0, &Button);
    JoyEvents.PrintValues();


    //We need to do some operations on forces in order to send them
    
    if ( JOYSTICK_MIN_RANGE<=fx && fx <= JOYSTICK_MAX_RANGE){
        fx = 0;
    }
    if ( JOYSTICK_MIN_RANGE<=fy && fy <= JOYSTICK_MAX_RANGE){
        fy = 0;
    }

   /** 
    *  Adding values to buffer to send to BOX.
    */
    uint8_t *speed =(uint8_t*) calloc(sizeof(uint8_t) , 3); //FX , FY , button
    speed[0] = map(fx , 0 , 1023 , 0 , 255);
    speed[1] = map(fy , 0 , 1023 , 0 , 255);
    speed[2] = Button;
    
    //Now we send

    if(udp.beginPacket(ROV , ROVPORT))
    {
      Serial.println("Sending data .....");
      udp.write(speed , 3);
      udp.endPacket();
      free(speed);
    }

  /**
   * Start receiving from Box to be shown on LCD for pilot.
   */
   
    ///Now we receive 

    uint8_t sizeOfBuffer = udp.parsePacket();
    if(sizeOfBuffer > 0){
      uint8_t buffer[9];
      udp.read(buffer , 9);

      //Some code for LCD
    }
  
}
