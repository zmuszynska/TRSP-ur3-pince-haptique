#include <Servo.h>

#define PROGRAM_VERSION 1.0
#define ANGLE_PIN A0
#define SERVO_PIN 3

Servo servo;

//Charactere de fin : #
//Angles : 0 - 255

unsigned long timer = 0;
float sum = 0.0;
char *myBuffer;
int angleToReach = 0;
const String endChar = "#";

void setup() {
  Serial.begin(115200);
  Serial.print("Gripper program v");
  Serial.println(PROGRAM_VERSION);
  
  pinMode(ANGLE_PIN, INPUT);
  pinMode(SERVO_PIN, OUTPUT);
  myBuffer = (char*)malloc(64*sizeof(char));
  servo.attach(SERVO_PIN);
}



void loop() {

  for(int i = 0;i<64;i++)     
    myBuffer[i] = 0;
      
  while(Serial.available())
  {
    Serial.readBytesUntil('#', myBuffer, 6);
    angleToReach = atoi(myBuffer);
    if(angleToReach > 140)
      angleToReach = 140;
    else if(angleToReach < 13)
      angleToReach = 13;
    servo.write(angleToReach);
    Serial.flush();
  }
 
  if(millis()-timer >= 10)
  {
    sum = 0.0;
    for(int i=0;i<100;i++)
      sum += analogRead(ANGLE_PIN);
    sum = sum/100.0;
    Serial.print(sum);
    Serial.println(endChar);
    Serial.flush();
  }
}
