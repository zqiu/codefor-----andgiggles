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
#define READDELAY  5 //#of ms to wait between reading sensors
#define NUMBERFOREACK    100  //send ack every 10 sec
#define NUMBERREADFORSEND 20  //so send every 100ms

#define TEMPPORT       0
#define RESPORT        1
#define HEARTRATEPORT  2

#define RESTHRESHOLD  2.86 //.02 resistance difference. need to find the threshold
#define HEARTTHRESHOLD  2.56 //voltage value. need to find the threshold

RFM12B radio;
byte sendSize=0,read = 0;
bool requestACK=false,resting=false,heartbeat=false;
char payload[30],scratch[10];
int temp,res,heart,temptot = 0,rescount = 0,heartcount = 0;

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
  
  temptot += temp;
  if(resting && res > RESTHRESHOLD){
    resting = false;
    rescount += 1;
  }
  if(!resting && res < RESTHRESHOLD){
    resting = true;
  }
  if(heartbeat && heart > HEARTTHRESHOLD){
    heartbeat = false;
    heartcount += 1;
  }
  if(!heartbeat && heart < HEARTTHRESHOLD){
    heartbeat = true;
  } 
      
  if(!read){
    itoa(temptot/NUMBERREADFORSEND,scratch,10);
    copy(payload,scratch,0,10);
    itoa(rescount,scratch,10);
    copy(payload,scratch,10,10);
    itoa(heartcount,scratch,10);
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
    sendSize = (sendSize + 1) % NUMBERFOREACK;  //add one to send size and reset to zero if overflow
  }
  read = (read + 1) % NUMBERREADFORSEND;
  delay(READDELAY);
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

void copy(char * to, char * from, byte beg, byte num){
  for(byte i = 0; i < num; ++i){
    to[i+beg] = from[i];
  }
}
