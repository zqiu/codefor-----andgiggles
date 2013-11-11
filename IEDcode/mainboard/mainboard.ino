
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
    
  printheart();
  printtemp();
  printresp();
  printcal();
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


void printheart(){
  
}

void make_number(byte x, byte y, byte num){
  //num = 0,2,3,5,6,7,8,9
  if(num == 0 || num == 2 || num == 3 || num > 4){
    colorblock(x+2,y,2,6);
  }
  //num = 2,3,4,5,6,8,9
  if(num > 1 && num != 7){
    colorblock(x+2,y+11,2,6);
  }
  //num = 0,2,3,5,6,8
  if(num == 0 || (num > 1 && num < 7 && num != 4) || num == 8){
      colorblock(x+2,y+21,2,6);
  }
  //num = 0,4,5,6,8,9
  if(num == 0 || (num > 3 && num < 7) || num > 7){
    colorblock(x,y+2,8,2);
  }
  //num = 0,2,6,8
  if(num % 2 == 0 && num != 4){
      colorblock(x,y+13,8,2);
  }
  //num = 0,1,2,3,4,7,8,9
  if(num < 5 || num > 6){
    colorblock(x+9,y+2,8,2);
  }
  //num = 0,1,3,4,5,6,7,8,9
  if(num != 2){
    colorblock(x+9,y+13,8,2);
  }
}

void colorblock(byte x, byte y, byte height, byte width){
  byte i,j;
  for(i = 0; i < height; ++i){
    for(j = 0; j < width; ++j){
      color(x+j,y+i);
    }
  }
}

void color(byte x,byte y){
  Serial.print(x);
  Serial.print(",");
  Serial.println(y);
}
