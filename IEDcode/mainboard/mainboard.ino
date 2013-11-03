
/*code for the main board*/
#include <RFM12B.h>

#define NODEID           1  //network ID used for this unit
#define NETWORKID       99  //the network ID we are on
#define SERIAL_BAUD 115200
#define DELAYONREAD     10  //milliseconds to wait before reading

// Need an instance of the Radio Module
RFM12B radio;
int temp,res,heart;
char scratch[10];
bool male = false;
byte age,weight,restingrate,temporary;

void setup(){
  Serial.begin(SERIAL_BAUD);
  while(!male){
    Serial.println("are you a male?y/n");
    waitforinput();
    temporary = Serial.read();
    if (temporary == 'y'){
      male = true;
    }else if(temporary == 'n'){
      break;
    }
  }
  Serial.println("enter age:");
  waitforinput();
  age = constrain(Serial.parseInt(), 0, 255);
  Serial.println("enter weight in kilograms:");
  waitforinput();
  weight = constrain(Serial.parseInt(), 0, 255);
  Serial.println("enter resting heart rate:");
  waitforinput();
  restingrate = constrain(Serial.parseInt(), 0, 255);
  temporary = (byte)(206.9 - (0.67 * age));
  radio.Initialize(NODEID, RF12_433MHZ, NETWORKID);
  Serial.println("Listening...");
}

void loop(){
  if (radio.ReceiveComplete()){
    if (radio.CRCPass()){
      Serial.print('[');Serial.print(radio.GetSender());Serial.print("] ");
      copy(scratch,(char*)radio.Data,0,10);
      temp = atoi(scratch);
      copy(scratch,(char*)radio.Data,10,10);
      res = atoi(scratch);
      copy(scratch,(char*)radio.Data,20,10);
      heart = atoi(scratch);
      printinfo(temp,res,heart);
      //TODO:convert readings of voltage value to measured values
      if(res < 10 || res > 55){
        Serial.println("respitory rate out of range");
      }
      if(heart > temporary || heart < restingrate){
        Serial.println("heartrate out of range");
      }
      if(temp > 40 || temp < 35){
        Serial.println("temp out of range");      
      }
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

void waitforinput(){
  while(Serial.available() == 0){
    delay(DELAYONREAD);
  }
}
