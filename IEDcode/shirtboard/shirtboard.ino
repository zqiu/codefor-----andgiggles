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
#define INTERPACKETDELAY  100 //#of ms to wait between sending packets
#define NUMBEFOREACK    10

#define TEMPPORT       0
#define RESPORT        1
#define HEARTRATEPORT  2

RFM12B radio;
byte sendSize=0;
bool requestACK=false;
char payload[30],scratch[10];
int temp,res,heart;
String tempvar;

//set the ports for input and output
void setup(){
  Serial.begin(SERIAL_BAUD);
  
  pinMode(TEMPPORT, INPUT);
  pinMode(RESPORT, INPUT);
  pinMode(HEARTRATEPORT, INPUT);
  
  radio.Initialize(NODEID, RF12_433MHZ, NETWORKID);
  if (waitForAck()){
     Serial.print("ok!\n");
  }else{
      Serial.print("nothing...\n");
  }
  radio.Sleep();
}

//will readin values for the sensor and communicate with the main board 
void loop(){
  
  temp = analogRead(TEMPPORT);
  res = analogRead(RESPORT);
  heart = analogRead(HEARTRATEPORT);
  Serial.print("read in ");
  printinfo(temp,res,heart);
  Serial.println();
  
  tempvar = String(temp);
  tempvar.toCharArray(scratch,10);
  copy(payload,scratch,0,10);
  tempvar = String(res);
  tempvar.toCharArray(scratch,10);
  copy(payload,scratch,10,10);
  tempvar = String(heart);
  tempvar.toCharArray(scratch,10);  
  copy(payload,scratch,20,10);
  
  requestACK =! sendSize; //request ACK everysingle time this is the first message in chunk
  radio.Wakeup();
  radio.Send(GATEWAYID, payload, sendSize+1, requestACK);
  if (requestACK){
    Serial.print(" - waiting for ACK...");
    if (waitForAck()){
       Serial.print("ok!");
    }else {
       Serial.print("nothing...");
    }
  }
  radio.Sleep();
  sendSize = (sendSize + 1) % NUMBEFOREACK;  //add one to send size and reset to zero if overflow
  delay(INTERPACKETDELAY);
}

// wait a few milliseconds for proper ACK, return true if received
static bool waitForAck() {
  long now = millis();
  while (millis() - now <= ACK_TIME)
    if (radio.ACKReceived(GATEWAYID))
      return true;
  return false;
}

void printinfo(int temperature, int resprate, int heartrate){
  Serial.print("temp:");
  Serial.print(temperature);
  Serial.print(",res:");
  Serial.print(resprate);
  Serial.print(",heart:");
  Serial.print(heartrate);
}

void copy(char * to, char * from, int beg, int num){
  int i;
  for(i = 0; i < num; ++i){
    to[i+beg] = from[i];
  }
}

