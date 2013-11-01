
/*code for the main board*/
#include <RFM12B.h>

#define NODEID           1  //network ID used for this unit
#define NETWORKID       99  //the network ID we are on
#define SERIAL_BAUD 115200

// Need an instance of the Radio Module
RFM12B radio;

void setup(){
  radio.Initialize(NODEID, RF12_433MHZ, NETWORKID);
  Serial.begin(SERIAL_BAUD);
  Serial.println("Listening...");
}

void loop(){
  if (radio.ReceiveComplete()){
    if (radio.CRCPass()){
      Serial.print('[');Serial.print(radio.GetSender());Serial.print("] ");
      for (byte i = 0; i < *radio.DataLen; i++) //can also use radio.GetDataLen() if you don't like pointers
        Serial.print((char)radio.Data[i]);
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
