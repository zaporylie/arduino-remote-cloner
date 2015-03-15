
#define RFRECEIVEPIN A0
#define RFTRANSMITPIN 7
#define BUTTONRECORDER 2
#define BUTTONTRANSMITER 3
#define MAXSIGNALLENGTH 255
#define DATASIZE 20
#define THERESHOLD 200

int timeDelay = 105;    //Used to slow down the signal transmission (can be from 75 - 135)
byte button1[DATASIZE];  //Create an array to store the data
 
void sendCode(int reg) {
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
}

void setup(){
  Serial.begin(19200);    //Initialise Serial communication - only required if you plan to print to the Serial monitor
  pinMode(BUTTONTRANSMITER, INPUT);
  initVariables();
  Serial.println("Ready");
}

void initVariables() {
  for(int i = 0; i < DATASIZE; i++) {
    button1[i] = random(1, 20);
  }
}
 
void loop(){
  delay(10000);
  sendCode(1);
}
