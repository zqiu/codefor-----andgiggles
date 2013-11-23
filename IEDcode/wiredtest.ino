#include <SPI.h>

unsigned int resp;
unsigned int therm;
unsigned int ad;
byte hr;
byte lr;
byte ht;
byte lt;
byte hh;
byte lh;
void setup()
{
  pinMode(A0, INPUT);
  pinMode(A1, INPUT);
  pinMode(A2, INPUT);
  digitalWrite(A0, LOW);
  digitalWrite(A1, LOW);
  digitalWrite(A2, LOW);
  SPI.setClockDivider(64);
  SPI.begin();
}
void loop()
{
  //PORTB |= 0x10;
  //PORTB &= ~0x10;
  SPI.transfer(0x0F);
  therm = analogRead(A0);
  resp = analogRead(A1);
  ad = analogRead(A2);
  hr = (resp >> 8) & 0x03;
  lr = resp & 0x00FF;
  ht = (therm >> 8) & 0x03;
  lt = therm & 0x00FF;
  hh = (ad >> 8) & 0x03;
  lh = ad & 0x00FF;
  //PORTB |= 0x10;
  //PORTB &= ~0x10;
  SPI.transfer(hr);
  //PORTB |= 0x10;
  //PORTB &= ~0x10;
  SPI.transfer(lr);
  //PORTB |= 0x10;
  //PORTB &= ~0x10;
  SPI.transfer(ht);
  //PORTB |= 0x10;
  //PORTB &= ~0x10;
  SPI.transfer(lt);
  SPI.transfer(hh);
  SPI.transfer(lh);
  delay(25);
}
