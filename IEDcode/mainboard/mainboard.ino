
/*code for the main board*/
#include <RFM12B.h>

#define NODEID           1  //network ID used for this unit
#define NETWORKID       99  //the network ID we are on
#define SERIAL_BAUD 115200

// Need an instance of the Radio Module
RFM12B radio;
byte temp,res,heart;
char scratch[10];

void setup(){
  //TODO:init char. of the person. (sex,age,etc...)
  radio.Initialize(NODEID, RF12_433MHZ, NETWORKID);
  Serial.begin(SERIAL_BAUD);
  Serial.println("Listening...");
}

void loop(){
  if (radio.ReceiveComplete()){
    if (radio.CRCPass()){
      Serial.print('[');Serial.print(radio.GetSender());Serial.print("] ");
      copy((char*)radio.Data,scratch,0,10);
      temp = atoi(scratch);
      copy((char*)radio.Data,scratch,10,10);
      res = atoi(scratch);
      copy((char*)radio.Data,scratch,20,10);
      heart = atoi(scratch);
      printinfo(temp,res,heart);
      //TODO:convert readings of voltage value to measured values
      if (radio.ACKRequested()){
        radio.SendACK();
        Serial.print(" - ACK sent");
      }
    }
    else
      Serial.print("BAD-CRC");
    Serial.println();
  }
}

void printinfo(int temperature, int resprate, int heartrate){
  Serial.print("temp:");
  Serial.print(temperature);
  Serial.print(",res:");
  Serial.print(resprate);
  Serial.print(",heart:");
  Serial.print(heartrate);
}

void copy(char * to, char * from, byte beg, byte num){
  for(byte i = 0; i < num; ++i){
    to[i+beg] = from[i];
  }
}
