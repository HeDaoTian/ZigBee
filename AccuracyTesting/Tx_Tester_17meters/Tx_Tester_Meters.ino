#include <Printers.h>
#include <XBee.h>
#include <SoftwareSerial.h>

SoftwareSerial serial1(6, 7); // RX, TX

XBee xbee=XBee();
XBeeResponse response = XBeeResponse();
Rx16Response rx16 = Rx16Response();
uint8_t rssi = 0;
float data = 0;
boolean sent=false;
const byte numChars = 32;
char receivedChars[numChars]; // an array to store the received data
char receivedChars2[numChars];
boolean newData = false;
boolean midData = false;
Tx16Request tx16;
float m = 100;
int count = 1;

void setup() {
  Serial.begin(9600);
  serial1.begin(9600);
  xbee.setSerial(serial1);
  while(!Serial.available()){;}
}

void loop() {
 uint8_t payload[15];
 payload[0]=sizeof(payload);
 tx16 = Tx16Request(0xFFFF, payload, sizeof(payload));
 while(!sent){
  
    xbee.send( tx16 );
    sent=true;
  }
  
  xbee.readPacket(100);
  if (xbee.getResponse().isAvailable())
  {
    //Serial.println(xbee.getResponse().getApiId());
      if (xbee.getResponse().getApiId() == RX_16_RESPONSE) 
      {
        xbee.getResponse().getRx16Response(rx16); 
        rssi = rx16.getRssi();
        data = rx16.getData(0);
        data = (rssi + data)/2;
        //Serial.println(data);
        if (data<m){
          m=data;
        }
        if(count==60){
          Serial.println(pow(10, (-39.918+m)/19.614));
          m=100;
          count=1;
          while(!Serial.available()){;}
        }
        count++;
        xbee.send( tx16 );
        delay(200);
        
      } 
  }else{
    sent=false;
    }
}

