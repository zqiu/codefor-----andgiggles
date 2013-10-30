/*
  code for the microcontroller on the shirt
*/
#include <RFM12B.h>
#include <avr/sleep.h>

#define NODEID        2  //network ID used for this unit
#define NETWORKID    99  //the network ID we are on
#define GATEWAYID     1  //the node ID we're sending to
#define ACK_TIME     50  // # of ms to wait for an ack
#define SERIAL_BAUD  115200
#define INTERPACKETDELAY  1000 //#of ms to wait between sending packets

#define TEMPPORT       0
#define RESPORT        1
#define HEARTRATEPORT  2

RFM12B radio;
byte sendSize=0;
bool requestACK=false;

//set the ports for input and output
void setup(){
  pinMode(TEMPPORT, INPUT);
  pinMode(RESPORT, INPUT);
  pinMode(HEARTRATEPORT, INPUT);
  
  Serial.begin(SERIAL_BAUD);
  radio.Initialize(NODEID, RF12_433MHZ, NETWORKID);
  Serial.println("Transmitting...\n\n");
}

//will readin values for the sensor and communicate with the main board 
void loop(){
  
}


