#include <EEPROM.h>
#define INSIZE 20 //longueur max de l'input utilisateur
byte input[INSIZE+1];
int index=0;
boolean complete;
typedef struct __attribute__ ((packed)) cmd_pkt {
//typedef struct cmd_pkt {
  char c;//command
  uint16_t a;//address EEPROM ou port
  uint8_t v;//valeur ou high/low
} cmd_pkt;

void setup() {
  // initialize serial:
  Serial.begin(9600);
  pinMode(13, OUTPUT);
  cmd_pkt test;
  test.c='l';//test.a=atoi("13"); test.v=atoi("1");
  test.a=13; test.v=1;
  Serial.write((uint8_t*) &test, sizeof(cmd_pkt));
  byte* str = (byte*) &test;
  Serial.println();
  for (int i=0; i<sizeof(cmd_pkt); i++){
    Serial.println(str[i], HEX);
  }
//  Serial.println((char*) &test);
  Serial.print("size:");
  Serial.println(sizeof(cmd_pkt));
}

void loop() {
  cmd_pkt commande;
  if (complete) {
    Serial.println("received: ");
    for(int i=0;i<sizeof(cmd_pkt);i++){
      Serial.println(input[i], HEX);
    }
    Serial.println("decoded: ");
    //memcpy(&commande, inputString.c_str(), sizeof(cmd_pkt));
    memcpy(&commande, input, sizeof(cmd_pkt));
    complete = false;
    Serial.print(commande.c, HEX);
    Serial.print(commande.a, HEX);
    Serial.println(commande.v, HEX);
  }
  if(commande.c=='l'){Serial.println("yikes");
    if(commande.a==13){
      if(commande.v==1){
        digitalWrite(13,HIGH);
      }
      if(commande.v==0){digitalWrite(13,LOW);}
    }
  }
}
boolean saveActive=false;
void serialEvent() {
  while (Serial.available()) {
    // get the new byte:
    byte inB = Serial.read();
    if(inB==3) {
      saveActive=false;
      Serial.println("stop rcv");
      if(index!=0){complete=true;}
      index=0;
    } //stop
    if(saveActive){ //get byte into buffer
      input[index]=inB;
      index++;
    }
    if(inB==2){
      Serial.println("start rcv");
      saveActive=true;
    } //start
    // if the incoming character is a newline, set a flag
    // so the main loop can do something about it: 
  }
}
