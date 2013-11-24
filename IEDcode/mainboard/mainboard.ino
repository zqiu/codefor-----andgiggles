/*code for the main board*/
#include <RFM12B.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SharpMem.h>
#include <math.h>

#define NODEID           1  //network ID used for this unit
#define NETWORKID       99  //the network ID we are on
#define SERIAL_BAUD 115200
#define DELAYONREAD     10  //milliseconds to wait before reading

#define SCK 7
#define MOSI 8
#define SS 9

#define POWER 4
#define GROUND 6

#define BETA 		40
#define RINFINITY 	100

Adafruit_SharpMem display(SCK, MOSI, SS);

// Need an instance of the Radio Module
RFM12B radio;
unsigned int temp,res[15],heart[15];
char scratch[10];
bool male = false;
byte temp0disp,temp1disp,respdisp,heartdisp;
byte age,weight,restingrate,temporary;
byte packetnum = 0;

void setup(){
  byte i;
  pinMode(POWER,OUTPUT);
  pinMode(GROUND,OUTPUT);
  digitalWrite(POWER,HIGH);
  digitalWrite(GROUND,LOW);
  
  for(i = 0; i < 15; ++i){
	res[i] = 3;
	heart[i] = 8;
  }
  temp0disp = 37;
  respdisp = 45;
  heart = 100;
  
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

  display.begin();
  printrhs();
  printnumbers(temp0disp,temp1disp,respdisp,heartdisp);
  
  display.clearDisplay();
  display.refresh();
  delay(500);
}

void loop(){
  byte i;
  int convertedtemp = 0;
  if (radio.ReceiveComplete()){
    if (radio.CRCPass()){
      Serial.print('[');Serial.print(radio.GetSender());Serial.print("] ");
      copy(scratch,(char*)radio.Data,0,10);
      temp = atoi(scratch);
      copy(scratch,(char*)radio.Data,10,10);
      res[packetnum] = atoi(scratch);
      copy(scratch,(char*)radio.Data,20,10);
      heart[packetnum] = atoi(scratch);
	  respdisp = 0;
	  heartdisp = 0;
	  for(i = 0; j < 15; ++j){
		respdisp += res[i];
		heartdisp += heart[i];
	  }
	  convertedtemp = 10*BETA/log((sqrt(50000.0*5000.0+1023.0/temp) - 50000.0)/RINFINITY)
	  temp0disp = convertedtemp/10;
	  temp1disp = convertedtemp%10;
      if(respdisp < 10 || respdisp > 55){
        Serial.println("respitory rate out of range");
      }
      if(heartdisp > temporary || heartdisp < restingrate){
        Serial.println("heartrate out of range");
      }
      if(temp0disp > 40 || temp0disp < 35){
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

  printrhs();
  printnumbers(temp0disp,temp1disp,respdisp,heartdisp);
  display.clearDisplay();
  display.refresh();
  i = (i + 1)%15;
  dalay (500);
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

void printbm(int x,int y){
  colorblock(x+1,y,1,3);
  colorblock(x+1,y+1,5,1);
  colorblock(x+1,y+6,1,3);
  colorblock(x+2,y+3,1,1);
  colorblock(x,y+9,1,5);
  colorblock(x,y+13,6,1);
  colorblock(x+2,y+13,6,1);
  colorblock(x+4,y+13,6,1);
  colorblock(x,y+12,1,5);
}

void printnumbers(byte temp1, byte temp2, byte resp, byte heart){
  make_number(50,1,heart/100);
  make_number(62,1,(heart/10)%10);
  make_number(74,1,heart%100);
  
  make_number(50,25,temp1/10);
  make_number(62,25,temp1%10);
  make_number(74,25,temp2%10);
  
  
  make_number(62,49,resp/10);
  make_number(74,49,resp%10);
  
  make_number(62,73,9);
  make_number(50,73,9);
  make_number(74,73,9);
  
  colorblock(72,46,2,2);
  colorblock(87,31,3,2);
  colorblock(90,33,1,3);
  colorblock(90,34,5,1);
  colorblock(90,39,1,3);
  
  printbm(87,3);
  printbm(87,51);
}

void printrhs(){
  printheart();
  printtemp();
  printresp();
  printcal();
}

void printheart(){
  colorblock(17,1,2,4);
  colorblock(27,1,2,4);
  colorblock(21,3,2,2);
  colorblock(25,3,2,2);
  colorblock(23,5,2,2);
  colorblock(15,3,10,2);
  colorblock(31,3,10,2);
  colorblock(17,13,2,2);
  colorblock(19,15,2,2);
  colorblock(21,17,2,2);
  colorblock(29,13,2,2);
  colorblock(27,15,2,2);
  colorblock(25,17,2,2);
  colorblock(22,19,2,4);
}

void printtemp(){
  colorblock(16,26,3,16);
  colorblock(23,29,17,2);
}

void printresp(){
  byte i;
  for(i = 0; i < 2; ++i){
    colorblock(i*12 + 13,50,2,8);
    colorblock(i*12 + 13,52,18,2);
    colorblock(i*12 + 15,59,2,6);
    colorblock(i*12 + 20,52,7,2);
    colorblock(i*12 + 20,61,10,2);
  }
}

void printcal(){
  colorblock(8,74,2,9);
  colorblock(8,76,17,2);
  colorblock(8,93,2,9);
  colorblock(23,74,2,2);
  colorblock(21,76,2,2);
  colorblock(25,76,2,2);
  colorblock(20,78,2,2);
  colorblock(26,78,2,2);
  colorblock(19,80,15,2);  
  colorblock(27,80,15,2);
  colorblock(21,83,2,6);
  colorblock(31,74,19,2);
  colorblock(31,93,2,9);
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
  display.drawPixel(x,y, 0);
}
