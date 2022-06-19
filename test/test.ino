
#include "SoftwareSerial.h"

SoftwareSerial mySerial(2, 3);

String cmd = "";

#include<Servo.h>
#define servoPin 9 
int pos = 0;


Servo servoMain;

#define trigPin 6
#define echoPin 10

#define trigPin1 5
#define echoPin1 4

int buzzPin = 7;

long duration, distance,duration1, dustbinlabel;

void setup(){
  
  mySerial.begin(9600);

  Serial.begin(9600);
  delay(1000);
  servoMain.attach(servoPin);
  servoMain.write(pos);
  pinMode(trigPin,OUTPUT);
  pinMode(echoPin,INPUT);
  pinMode(trigPin1,OUTPUT);
  pinMode(echoPin1,INPUT);
  pinMode(buzzPin,OUTPUT);
  digitalWrite(buzzPin,LOW);
  
  
  Serial.println("Initializing...");
  delay(1000);

  mySerial.println("AT");                 // Sends an ATTENTION command, reply should be OK
  updateSerial();
  mySerial.println("AT+CMGF=1");          // Configuration for sending SMS
  updateSerial();
  mySerial.println("AT+CNMI=1,2,0,0,0");  // Configuration for receiving SMS
  updateSerial();

  
}


int getDistance(){

  digitalWrite(trigPin,LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin,HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin,LOW);
  duration = pulseIn(echoPin,HIGH);
  distance = (duration/2) * 0.034;
//  Serial.print("Distance : ");
//  Serial.println(distance);

  return distance;
  
  
}
int getDustbinLabel(){

  digitalWrite(trigPin1,LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin1,HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin1,LOW);
  duration1 = pulseIn(echoPin1,HIGH);
  dustbinlabel = (duration1/2) * 0.034;
//  Serial.print("Distance : ");
//  Serial.println(distance);

  return dustbinlabel;
  
  
}



void updateSerial()                                                                                                                                                
{
  delay(500);
  while (Serial.available()) 
  {

    cmd+=(char)Serial.read();
 
    if(cmd!=""){
      cmd.trim();  // Remove added LF in transmit
      if (cmd.equals("S")) {
        sendSMS();
      } else {
        mySerial.print(cmd);
        mySerial.println("");
      }
    }
  }
  
  while(mySerial.available()) 
  {
    Serial.write(mySerial.read());//Forward what Software Serial received to Serial Port
  }
}


void sendSMS(){
  mySerial.println("AT+CMGF=1");
  delay(500);
  mySerial.println("AT+CMGS=\"+8801769117283\"\r");
  delay(500);
  mySerial.print("The Dustbin is Full");
  delay(500);
  mySerial.write(26);
}

void loop(){

   int object = getDistance();
   int dustBin = getDustbinLabel();
   updateSerial();

     
   Serial.print("Distance : ");
   Serial.println(object);
   Serial.print("Dustbin Label :  ");
   Serial.println(dustBin);

   if(dustBin <= 15){
    sendSMS();
    digitalWrite(buzzPin,HIGH);
    delay(1500);
    
   }
   else{
    digitalWrite(buzzPin,LOW);
   }

   if(object < 50){
    servoMain.write(pos+120);
//    sendSMS();
    delay(1500);
    
   }

   else{
    servoMain.write(pos);
    delay(300);
   }
}
