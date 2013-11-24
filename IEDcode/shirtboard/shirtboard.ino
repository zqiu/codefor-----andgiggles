/*
  code for the microcontroller on the shirt
*/
#include <RFM12B.h>
#include <avr/sleep.h>
#include <SPI.h>

#define NODEID        2  //network ID used for this unit
#define NETWORKID    99  //the network ID we are on
#define GATEWAYID     1  //the node ID we're sending to
#define ACK_TIME     50  // # of ms to wait for an ack
#define SERIAL_BAUD  115200
#define READDELAY      40 	//#of ms to wait between reading sensors
#define NUMBERFOREACK    5  //send ack every 20 sec
#define NUMBERREADFORSEND 100  //so send every 4s

#define TEMPPORT       A0
#define RESPORT        A1
#define HEARTRATEPORT  A2

RFM12B radio;
byte sendSize=0,read = 0;
bool requestACK=false,resting=false,heartbeat=false;
char payload[30],scratch[10];
unsigned int temp,res,heart,temptot = 0,rescount = 0,heartcount = 0;
unsigned int maxresp,maxhrt,minresp,minhrt;

//set the ports for input and output
void setup(){
  Serial.begin(SERIAL_BAUD);
  
  pinMode(TEMPPORT, INPUT);
  pinMode(RESPORT, INPUT);
  pinMode(HEARTRATEPORT, INPUT);
  digitalWrite(TEMPPORT, LOW);
  digitalWrite(RESPORT, LOW);
  digitalWrite(HEARTRATEPORT, LOW);
  
  radio.Initialize(NODEID, RF12_433MHZ, NETWORKID);
  if (waitForAck()){
     Serial.print("ok!\n");
  }else{
      Serial.print("nothing...\n");
  }
  radio.Sleep();
  
  SPI.setClockDivider(64);
  SPI.begin();
  
  initvalues();
  SPI.transfer(maxresp >> 8);
  SPI.transfer(maxresp & 0x00FF);
  SPI.transfer(minresp >> 8);
  SPI.transfer(minresp & 0x00FF);  
  SPI.transfer(maxhrt >> 8);
  SPI.transfer(maxhrt & 0x00FF);
  SPI.transfer(minhrt >> 8);
  SPI.transfer(minhrt & 0x00FF);
  
  minresp += (maxresp - minresp)/3;
  maxresp = (maxresp + minresp) >> 1;
  minhrt += (maxhrt - minhrt)/3;
  maxhrt = (maxhrt + minhrt) >> 1;
}

//will readin values for the sensor and communicate with the main board 
void loop(){ 
  temp = analogRead(TEMPPORT) & 0x03FF;
  res = analogRead(RESPORT) & 0x03FF;
  heart = analogRead(HEARTRATEPORT) & 0x03FF;
  Serial.print("read in ");
  printinfo(temp,res,heart);
  Serial.println();
  
  temptot += temp;
  if(resting && res > maxresp){
    resting = false;
    rescount += 1;
  }
  if(!resting && res < minresp){
    resting = true;
  }
  if(heartbeat && heart > maxhrt){
    heartbeat = false;
    heartcount += 1;
  }
  if(!heartbeat && heart < minhrt){
    heartbeat = true;
  } 
      
  if(!read){
    itoa(temptot/NUMBERREADFORSEND,scratch,10);
    copy(payload,scratch,0,10);
    itoa(rescount,scratch,10);
    copy(payload,scratch,10,10);
    itoa(heartcount,scratch,10);
    copy(payload,scratch,20,10);

    printSPI(temptot/NUMBERREADFORSEND,rescount,heartcount);
	
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
    temptot = 0;
    rescount = 0;
    heartcount = 0;
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

void printSPI(int temperature, int resprate, int heartrate){
  SPI.transfer(temperature >> 8);
  SPI.transfer(temperature & 0x00FF);
  SPI.transfer(resprate >> 8);
  SPI.transfer(resprate & 0x00FF);
  SPI.transfer(heartrate >> 8);
  SPI.transfer(heartrate & 0x00FF);
}

void copy(char * to, char * from, byte beg, byte num){
  for(byte i = 0; i < num; ++i){
    to[i+beg] = from[i];
  }
}

void initvalues(){
  unsigned int i;
  for(i = 0; i < 400; ++i){
    res = analogRead(A1) & 0x03FF;
    heart = analogRead(A2) & 0x03FF;
    maxresp = (res> maxresp) ? res: maxresp;
    maxhrt = (heart> maxhrt) ? heart: maxhrt;
    minresp = (res < minresp) ? res: minresp;
    minhrt = (heart < minhrt) ? heart: minhrt;		
    delay(10);
  }
}
