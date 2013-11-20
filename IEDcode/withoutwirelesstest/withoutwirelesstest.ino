/*
  code for the microcontroller on the shirt
*/
#include <avr/sleep.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SharpMem.h>

#define SERIAL_BAUD  115200
#define READDELAY  5 //#of ms to wait between reading sensors
#define READSBEFOREPRINT 10

#define TEMPPORT       0
#define RESPORT        1
#define HEARTRATEPORT  2

#define RESTHRESHOLD  2.86 //.02 resistance difference. need to find the threshold
#define HEARTTHRESHOLD  2.56 //voltage value. need to find the threshold

#define SCK 7
#define MOSI 8
#define SS 9

bool requestACK=false,resting=false,heartbeat=false;
int temp,res,heart,temptot = 0,rescount = 0,heartcount = 0;
Adafruit_SharpMem display(SCK, MOSI, SS);
char screenread = 0;

//set the ports for input and output
void setup(){
  Serial.begin(SERIAL_BAUD);
  //SPI.begin();
  
  pinMode(TEMPPORT, INPUT);
  pinMode(RESPORT, INPUT);
  pinMode(HEARTRATEPORT, INPUT);
  
  display.begin();
  
  display.clearDisplay();
  printrhs();
  
  display.refresh();
  delay(500);  
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
      
  delay(READDELAY);
  screenread++;
  if(screenread > READSBEFOREPRINT){
    screenread = 0;
     display.clearDisplay();
     printrhs();
     display.refresh();
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
