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

int count=100;
int siz=1;

void setup() {
  Serial.begin(9600);
  serial1.begin(9600);
  xbee.setSerial(serial1);
  while(!Serial.available()){;}
}

void loop() {
 recvWithMarker();
 gotNewData();
 uint8_t payload[siz];
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
        Serial.println(data);
        xbee.send( tx16 );
        delay(count);
      } 
  }else{
    sent=false;
    }
}

void recvWithMarker() {
 static byte ndx = 0;
 static byte ndx2 = 0;
 char endMarker = '\n';
 char midMarker = ',';
 char rc;
 
 // if (Serial.available() > 0) {
           while (Serial.available() > 0 && newData == false) {
 rc = Serial.read();
 if (rc != midMarker) {
 receivedChars[ndx] = rc;
 ndx++;
 if (ndx >= numChars) {
 ndx = numChars - 1;
 }
 }
 else {
 receivedChars[ndx] = '\0'; // terminate the string
 ndx = 0;
 midData=true;
 }
 if(midData=true){
  if(rc!=endMarker){
     receivedChars2[ndx2] = rc;
     ndx2++;
     if (ndx2 >= numChars) {
       ndx2 = numChars - 1;
       }
  }
  else{
     receivedChars2[ndx2] = '\0'; // terminate the string
     ndx2 = 0;
     newData = true;
  }
 }
 }
}

void gotNewData() {
 if (newData == true) {
 //Serial.print("This just in ... ");
 //Serial.println(receivedChars);
 newData = false;
 count = atoi(receivedChars2);
 siz = atoi(receivedChars);
 }
}
