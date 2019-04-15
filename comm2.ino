#include <EEPROM.h>
int index=0;
boolean complete;
typedef struct __attribute__ ((packed)) cmd_pkt {
//typedef struct cmd_pkt {
  char c;//command
  uint16_t a;//address EEPROM ou port (0-511 pour EEPROM)
  uint8_t v;//valeur ou high/low
} cmd_pkt;
byte input[sizeof(cmd_pkt)+1];

void setup() {
  // initialize serial:
  Serial.begin(9600);
  pinMode(13, OUTPUT);
  //test();
}

void loop() {
  cmd_pkt commande;
  if (complete) {
    //display_input();
    memcpy(&commande, input, sizeof(cmd_pkt));
    complete = false;
    /*Serial.print(commande.c, HEX);
    Serial.print(commande.a, HEX);
    Serial.println(commande.v, HEX);*/
    executeCommande(commande);
    commande.c='0';//reset
  }
}
boolean saveActive=false;
void serialEvent() {
  while (Serial.available()) {
    // get the new byte:
    byte inB = Serial.read();
    if(inB==3) { //stop buffering
      saveActive=false;
  //    Serial.println("stop rcv");
      if(index!=0){complete=true;}
      index=0;
    } //stop
    if(saveActive){ //get byte into buffer
      input[index]=inB;
      index++;
    }
    if(inB==2){ //start buffering
//      Serial.println("start rcv");
      saveActive=true;
    } //start
  }
}


void executeCommande(struct cmd_pkt commande) {
 if(commande.c=='l'){
    digitalWrite(commande.a, commande.v);
  }
 if(commande.c=='r'){
  Serial.print("\nread 0x");
  Serial.print(EEPROM.read(commande.a), HEX);
  Serial.print(" (");
  Serial.write(EEPROM.read(commande.a));
  Serial.print(") at ");Serial.println(commande.a);
 }
  if (commande.c=='w'){
   EEPROM.write(commande.a, commande.v);
   Serial.print("wrote 0x");
   Serial.print(commande.v, HEX);
   Serial.print(" (");
   Serial.write(commande.v);
   Serial.print(") at ");
   Serial.println(commande.a);
  } 
}
void display_input(void) {
    Serial.println("received: ");
    for(int i=0;i<sizeof(cmd_pkt);i++){
      Serial.println(input[i], HEX);
    }
    Serial.println();
}
void test(void) {
  cmd_pkt test;
  test.c='N';//test.a=atoi("13"); test.v=atoi("1");
  test.a=254; test.v=1022;
  Serial.write((uint8_t*) &test, sizeof(cmd_pkt));
  byte* str = (byte*) &test;
  Serial.println();
  for (int i=0; i<sizeof(cmd_pkt); i++){
    Serial.println(str[i], HEX);
  }
  Serial.print("size:");
  Serial.println(sizeof(cmd_pkt));
}
