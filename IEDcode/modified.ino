#include <SPI.h>

unsigned int resp,temp,hrt,maxresp,maxtemp,maxhrt,minresp,mintemp,minhrt;
unsigned int hr = 0,rr = 0;
byte read = 0;
bool resting,heartbeat;

void setup(){
  pinMode(A0, INPUT);
  pinMode(A1, INPUT);
  pinMode(A2, INPUT);
  digitalWrite(A0, LOW);
  digitalWrite(A1, LOW);
  digitalWrite(A2, LOW);
  SPI.setClockDivider(64);
  SPI.begin();
  SPI.transfer(0xFF);
  initvalues();
  resp = (maxresp + minresp) >> 1;
  hrt = (maxhrt+ minhrt) >> 1;
  resting = ((analogRead(A1) & 0x03FF) > resp)? false:true;
  heartbeat = (hrt > analogRead(A2) & 0x03FF) ? false:true;
  SPI.transfer(maxresp >> 8);
  SPI.transfer(maxresp & 0x00FF);
  SPI.transfer(minresp >> 8);
  SPI.transfer(minresp & 0x00FF);
}

void loop(){
  temp += analogRead(A0) & 0x03FF;
  resp = analogRead(A1) & 0x03FF;
  hrt = analogRead(A2) & 0x03FF;
  if(resting && (resp > ((maxresp + minresp) >> 1))){
    resting = false;
    rr += 1;
  }else if(!resting && (resp < ((maxresp + minresp) >> 1))){
    resting = true;
    rr += 1;
  }
  if(heartbeat && hrt > ((maxhrt+ minhrt) >> 1)){
    heartbeat = false;
    hr += 1;
  }
  if(!heartbeat && hrt < ((maxhrt+ minhrt) >> 1)){
    heartbeat = true;
  }
  if(!read){
	/*SPI.transfer(maxresp >> 8);
	SPI.transfer(maxresp & 0x00FF);
	SPI.transfer(rr);
	SPI.transfer(maxtemp >> 8);
	SPI.transfer(maxtemp & 0x00FF);
	temp = temp >> 8;
	SPI.transfer(temp >> 8);
	SPI.transfer(temp & 0x00FF);
	SPI.transfer(maxhrt >> 8);
	SPI.transfer(maxhrt & 0x00FF);
    SPI.transfer(hr);
	*/
	SPI.transfer(rr >> 8);
	SPI.transfer(rr & 0x00FF);
	temp = 0;
  }
  delay(16);
  read = (read + 1) % 64;
}

void initvalues(){
	unsigned int i;
	for(i = 0; i < 300; ++i){
		temp = analogRead(A0) & 0x03FF;
		resp = analogRead(A1) & 0x03FF;
		hrt = analogRead(A2) & 0x03FF;
		maxtemp = (temp > maxtemp) ? temp : maxtemp;
		maxresp = (resp> maxresp) ? resp: maxresp;
		maxhrt = (hrt> maxhrt) ? hrt: maxhrt;
		mintemp = (temp < mintemp) ? temp : mintemp;
		minresp = (resp < minresp) ? resp: minresp;
		minhrt = (hrt < minhrt) ? hrt: minhrt;		
		delay(10);
	}
}
