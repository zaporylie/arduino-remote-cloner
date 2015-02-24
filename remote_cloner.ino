 #include <EEPROM.h>

 #define rfReceivePin A0     //RF Receiver data pin = Analog pin 0
 #define rfTransmitPin 4     //RF Transmitter pin = digital pin 4
 #define button 2            //The button attached to digital pin 6
 #define ledPin 13           //Onboard LED = digital pin 13
 
 int eepromButton1 = 0;
 int eepromButton2 = 2;
 int eepromButton3 = 4;
 int eepromButton4 = 6;
 
 int code1 = 0;
 int code2 = 0;
 int code3 = 0;
 int code4 = 0;

 void setup(){
   Serial.begin(9600);    //Initialise Serial communication - only required if you plan to print to the Serial monitor
   pinMode(rfTransmitPin, OUTPUT);    
   pinMode(ledPin, OUTPUT); 
   pinMode(button, INPUT);
   
   EEPROMWriteInt(eepromButton1, 10);
   EEPROMWriteInt(eepromButton2, 20);
   EEPROMWriteInt(eepromButton3, 30);
   EEPROMWriteInt(eepromButton4, 40);
   
   code1 = EEPROMReadInt(eepromButton1);
   code2 = EEPROMReadInt(eepromButton2);
   code3 = EEPROMReadInt(eepromButton3);
   code4 = EEPROMReadInt(eepromButton4);
   
   Serial.println("Code1: " + code1);
   Serial.println("Code2: " + code2);
   Serial.println("Code3: " + code3);
   Serial.println("Code4: " + code4);
 }
 
 void loop(){
//   digitalRead(button);
//   Serial.println(buttonVal);
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

