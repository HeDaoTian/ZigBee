#include <Printers.h>
#include <XBee.h>
#include <SoftwareSerial.h>

SoftwareSerial serial1(8, 9); // RX, TX

XBee xbee2=XBee();
XBeeResponse response = XBeeResponse();
Rx16Response rx16 = Rx16Response();
uint8_t rssi = 0;
Tx16Request tx16;
int data;

void setup() 
{
  Serial.begin(9600);
  serial1.begin(9600);
  xbee2.setSerial(serial1);
}

void loop() 
{
  xbee2.readPacket(100);
  if (xbee2.getResponse().isAvailable())
  {
      if (xbee2.getResponse().getApiId() == RX_16_RESPONSE) 
      {
        xbee2.getResponse().getRx16Response(rx16); 
        rssi = rx16.getRssi();
        data = rx16.getData(0);
        uint8_t payload[data];
        payload[0] = rssi;
        tx16 = Tx16Request(0xFFFF, payload, sizeof(payload));
        xbee2.send( tx16 );
      } 
  }
  delay(100);
}
