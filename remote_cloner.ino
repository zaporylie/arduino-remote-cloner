#include <EEPROM.h>

#define RFRECEIVEPIN A0
#define RFTRANSMITPIN 4
#define BUTTONRECORDER 2
#define BUTTONTRANSMITER 3
#define MAXSIGNALLENGTH 255
#define DATASIZE 400
#define THERESHOLD 150

int timeDelay = 105;    //Used to slow down the signal transmission (can be from 75 - 135)
byte button1[DATASIZE];  //Create an array to store the data
//byte button2[DATASIZE];  //Create an array to store the data
//byte button3[DATASIZE];  //Create an array to store the data
//byte button4[DATASIZE];  //Create an array to store the data
 
int eepromButton1 = 0;
int eepromButton2 = 2;
int eepromButton3 = 4;
int eepromButton4 = 6;
 
int code1 = 0;
int code2 = 0;
int code3 = 0;
int code4 = 0;


void registerCode() {
  Serial.println("registerCode");
//  detachInterrupt(0);
//  detachInterrupt(1);
  delay(20);
  
  EEPROMWriteInt(eepromButton1, random(300));
  code1 = EEPROMReadInt(eepromButton1);
  Serial.print("Code1: ");
  Serial.println(code1);

  Serial.println("Waiting for signal");
  while (analogRead(RFRECEIVEPIN) < THERESHOLD) {
    delay(20);
  }
  
  initVariables();
  listenForSignal();
  
  Serial.println("Signal");
  for(int i = 0; i < DATASIZE; i=i+2){
    for (int j=0; j < button1[i]; j++) {
      Serial.print("-");
    }
    for (int j=0; j < button1[i+1]; j++) {
      Serial.print("_");
    }
  }
  Serial.println("end of signal");
  
  delay(20);
  
//  attachInterrupt(0, registerCode, RISING);
//  attachInterrupt(1, sendCode, RISING);
}

void sendCode() {
  Serial.println("sendCode");
//  detachInterrupt(0);
//  detachInterrupt(1);
  for (int i=0; i < DATASIZE; i=i+2) {
    //Send HIGH signal
    digitalWrite(RFTRANSMITPIN, HIGH);     
    delayMicroseconds(button1[i] * timeDelay);
    //Send LOW signal
    digitalWrite(RFTRANSMITPIN, LOW);     
    delayMicroseconds(button1[i+1] * timeDelay);
  }
   
  delay(1000);
   
  Serial.println("Signal");
  for(int i = 0; i < DATASIZE; i=i+2){
    for (int j=0; j < button1[i]; j++) {
      Serial.print("-");
    }
    for (int j=0; j < button1[i+1]; j++) {
      Serial.print("_");
    }
  }
  Serial.println("end of signal");
//  attachInterrupt(0, registerCode, RISING);
//  attachInterrupt(1, sendCode, RISING);
}

void setup(){
  Serial.begin(19200);    //Initialise Serial communication - only required if you plan to print to the Serial monitor
  pinMode(BUTTONRECORDER, INPUT);
  pinMode(BUTTONTRANSMITER, INPUT);
   
  code1 = EEPROMReadInt(eepromButton1);
  code2 = EEPROMReadInt(eepromButton2);
  code3 = EEPROMReadInt(eepromButton3);
  code4 = EEPROMReadInt(eepromButton4);
   
  Serial.print("Code1: ");
  Serial.println(code1);
  Serial.print("Code2: ");
  Serial.println(code2);
  Serial.print("Code3: ");
  Serial.println(code3);
  Serial.print("Code4: ");
  Serial.println(code4);
  
  initVariables();
   
//  attachInterrupt(0, registerCode, RISING);
//  attachInterrupt(1, sendCode, RISING);
  Serial.println("Ready");
}
 
void loop(){
  delay(20);
  if (digitalRead(BUTTONRECORDER) == HIGH) {
    registerCode();
  }
}
 
void EEPROMWriteInt(int p_address, int p_value) {
  byte lowByte = ((p_value >> 0) & 0xFF);
  byte highByte = ((p_value >> 8) & 0xFF);
  
  EEPROM.write(p_address, lowByte);
  EEPROM.write(p_address + 1, highByte);
}

//This function will read a 2 byte integer from the eeprom at the specified address and address + 1
unsigned int EEPROMReadInt(int p_address) {
  byte lowByte = EEPROM.read(p_address);
  byte highByte = EEPROM.read(p_address + 1);
 
  return ((lowByte << 0) & 0xFF) + ((highByte << 8) & 0xFF00);
}

void initVariables() {
  for(int i = 0; i < DATASIZE; i++) {
    button1[i]=0;
//    button2[i]=0;
//    button3[i]=0;
//    button4[i]=0;
  }
}

void listenForSignal() {
  Serial.println("listen");
  
  int dataCounter = 0;
  
  for(int i = 0; i < DATASIZE; i=i+2) {
      dataCounter = 0; //reset the counter
      while(analogRead(RFRECEIVEPIN) > THERESHOLD && dataCounter < MAXSIGNALLENGTH){
        dataCounter++;
      }
      button1[i] = dataCounter;    //Store the length of the HIGH signal
    
      
      //Identify the length of the LOW signal---------------LOW
      dataCounter=0; //reset the counter
      while(analogRead(RFRECEIVEPIN) < THERESHOLD && dataCounter < MAXSIGNALLENGTH){
        dataCounter++;
      }
      button1[i+1]=dataCounter;  //Store the length of the LOW signal
   }
   
   button1[0]++;  //Account for the first AnalogRead>threshold = lost while listening for signal
}
