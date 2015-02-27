#include <EEPROM.h>

#define RFRECEIVEPIN A0
#define RFTRANSMITPIN 4
#define BUTTONRECORDER 2
#define BUTTONTRANSMITER 3
#define MAXSIGNALLENGTH 30
#define DATASIZE 100
#define THERESHOLD 100

int timeDelay = 105;    //Used to slow down the signal transmission (can be from 75 - 135)
byte code[4][DATASIZE];

void setup(){
  Serial.begin(19200);    //Initialise Serial communication - only required if you plan to print to the Serial monitor
  pinMode(BUTTONRECORDER, INPUT);
  pinMode(BUTTONTRANSMITER, INPUT);
  
//  initVariables();
  
  Serial.println("Ready!");
  printMenu();
}

void initVariables() {
//  EEPROMRead(1);
//  EEPROMRead(2);
//  EEPROMRead(3);
//  EEPROMRead(4);  
  for (int i = 0; i < DATASIZE; i++) {
    code[1][i] = 0;
    code[2][i] = 0;
    code[3][i] = 0;
    code[4][i] = 0;
    Serial.print(i);
  }
}

void initVariable(int reg) {
  for (int i = 0; i < DATASIZE; i++) {
    code[reg][i] = 0;
  }
//  EEPROMRead(1);
//  EEPROMRead(2);
//  EEPROMRead(3);
//  EEPROMRead(4);  
}

void loop(){
  
  delay(20);
  if (Serial.available()) {
    int c = Serial.read();
    Serial.println(c);
    int reg = printSubMenu();
    if (c == 49) {
      receiveCode(reg);
    }
    else if (c == 50) {
      sendCode(reg);
    }
  }
}

void printMenu() {
  Serial.println("Choose mode:");
  Serial.println("[1] - receive");
  Serial.println("[2] - send");
}

int printSubMenu() {
  Serial.println("Which register:");
  Serial.println("[1]");
  Serial.println("[2]");
  Serial.println("[3]");
  Serial.println("[4]");
  while(!Serial.available()) {
    delay(10);
  }
  return Serial.read();
}

void receiveCode(int reg) {
  Serial.println("registerCode");
  initVariable(reg);
  Serial.println("Waiting for signal...");
  do {
    delay(20);
  } while (analogRead(RFRECEIVEPIN) < THERESHOLD);

  listenForSignal(reg);
  printSignal(reg);
  EEPROMWrite(reg);
}

void printSignal(int reg) {
//  EEPROMRead(reg);
  Serial.println("Signal:");
  for(int i = 0; i < DATASIZE; i=i+2){
    for (int j = 0; j < code[reg][i]; j++) {
      Serial.print("^");
    }
    for (int j=0; j < code[reg][i+1]; j++) {
      Serial.print("_");
    }
  }
  Serial.println("end of signal");
}

void sendCode(int reg) {
  Serial.println("sendCode");
  for (int i=0; i < DATASIZE; i=i+2) {
    //Send HIGH signal
    digitalWrite(RFTRANSMITPIN, HIGH);     
    delayMicroseconds(code[reg][i] * timeDelay);
    //Send LOW signal
    digitalWrite(RFTRANSMITPIN, LOW);     
    delayMicroseconds(code[reg][i+1] * timeDelay);
  }
  printSignal(reg);
}

void EEPROMWrite(int reg) {
  for (int i = 0; i < DATASIZE; i++) {
    EEPROM.write(((reg - 1) * DATASIZE) + i, code[reg][i]);
  }
}

//This function will read a 2 byte integer from the eeprom at the specified address and address + 1
void EEPROMRead(int reg) {
  for (int i = 0; i < DATASIZE; i++) {
    code[reg][i] = EEPROM.read(((reg - 1) * DATASIZE) + i);
  }
}

void listenForSignal(int reg) {
  Serial.println("Receiving...");
  
  int i = 0;
  boolean state;
  code[reg][0]++;
  for (int i = 0; i < DATASIZE; i=i+2) {
     while (state = analogRead(RFRECEIVEPIN) > THERESHOLD) {
       code[reg][i]++;
       if (code[reg][i] >= sizeof(code[reg][i])) {
         break;
       }
     }
     if (state) {
       continue;
     }
     while (!(state = analogRead(RFRECEIVEPIN) > THERESHOLD)) {
       code[reg][i+1]++;
       if (code[reg][i+1] >= sizeof(code[reg][i+1])) {
         break;
       }
     }
  }

//  int dataCounter = 0;
  
//  for(int i = 0; i < DATASIZE; i=i+2) {
//      dataCounter = 0; //reset the counter
//      while(analogRead(RFRECEIVEPIN) > THERESHOLD && dataCounter < sizeof(code[reg][i])){
//        dataCounter++;
//      }
//      code[reg][i] = dataCounter;    //Store the length of the HIGH signal
//    
//      
//      //Identify the length of the LOW signal---------------LOW
//      dataCounter=0; //reset the counter
//      while(analogRead(RFRECEIVEPIN) < THERESHOLD && dataCounter < sizeof(code[reg][i+1])){
//        dataCounter++;
//      }
//      code[reg][i+1]=dataCounter;  //Store the length of the LOW signal
//   }
}
