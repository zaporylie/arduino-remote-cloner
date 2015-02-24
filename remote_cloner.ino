#include <EEPROM.h>

#define button 2            //The button attached to pin 2
 
int eepromButton1 = 0;
int eepromButton2 = 2;
int eepromButton3 = 4;
int eepromButton4 = 6;
 
int code1 = 0;
int code2 = 0;
int code3 = 0;
int code4 = 0;


void registerCode() {
  detachInterrupt(0);
  delay(20);
  int buttonVal = digitalRead(button);
  Serial.println(buttonVal);
  delay(20);
  
  attachInterrupt(0, registerCode, RISING);
}

void setup(){
  Serial.begin(9600);    //Initialise Serial communication - only required if you plan to print to the Serial monitor
  pinMode(button, INPUT);
   
  EEPROMWriteInt(eepromButton1, 10);
  EEPROMWriteInt(eepromButton2, 20);
  EEPROMWriteInt(eepromButton3, 30);
  EEPROMWriteInt(eepromButton4, 40);
   
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
   
  attachInterrupt(0, registerCode, RISING);
  int0_guard = true ;   // only set the guard variable after attachInterrupt
}
 
void loop(){
  delay(20);
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

