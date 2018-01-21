#include <Wire.h>
#include <L3G.h>
#include <LSM303.h>
#include <Servo.h>

#define M_PI 3.14159265359
#define RAD_TO_DEG 57.29578

#define GYR_PERCENTAGE 0.98
#define GAIN  8.75/1000.0

Servo servo;

int EnablePontDiv = 4;
int PinServo = 3;
int PinPontDiv = A0;

float ZOffset = 0;
L3G gyro;
LSM303 compass;

void setup() {
  Serial.begin(115200);
  Wire.begin();

  pinMode(EnablePontDiv, OUTPUT);
  pinMode(PinServo, OUTPUT);
  pinMode(PinPontDiv, INPUT);

  digitalWrite(EnablePontDiv, HIGH);
  
  servo.attach(PinServo);

  if(!compass.init())
  {
    Serial.println("Couldn't autodetect the compass");
    while(1);
  }
  if(!gyro.init())
  {
    Serial.println("Couldn't autodetect the gyroscope");
    while(1);
  }
  
  gyro.enableDefault();
  compass.enableDefault(); 
  
  int counter = 0;
  int t = 0;
  int t_old = 0;
  float dt_sum = 0;
  float dt = 0;
  
  while(counter < 100)
  {
    counter ++;
    
    if((millis()-t)>=20)
    {
        t_old  = t;
        t = millis();
  
        if(t > t_old)
        {
          dt = ( (float)t -  (float)t_old)/1000.0;
          if(dt >0.2)
            dt = 0;
        }
        else
          dt = 0;
        
    }
    gyro.read();
    ZOffset += gyro.g.z*dt*GAIN;
    dt_sum += dt;
  }
  
  ZOffset /= dt_sum; 
}


long disp_timer = 0;

long timer = 0;
long timer_old = 0;
float dt = 0;


float CFangleX, CFangleY, CFangleZ;


float tau=0.075;
float a=0.1;

float x_angleC = 0.0;

void loop() 
{
  float gyrX, gyrY, gyrZ;      
  float AccXangle, AccYangle, AccZangle;
  int gyrData[] = {gyro.g.x, gyro.g.y, gyro.g.z};
  int accData[] = {compass.a.x, compass.a.y, compass.a.z};
  if((millis()-timer)>=20)
  {
    timer_old  = timer;
    timer = millis();

    if(timer > timer_old)
    {
      dt = ( (float)timer -  (float)timer_old)/1000.0;
      if(dt >0.2)
        dt = 0;
    }
    else
      dt = 0;

      
    gyro.read();
    compass.read();


    gyrX = gyro.g.x*dt;//Complementary(, compass.a.x, dt);
    gyrY = gyro.g.y*dt;
    gyrZ = gyro.g.z*GAIN*dt;

    AccXangle = (float) (atan2( compass.a.y, compass.a.z)+M_PI)*RAD_TO_DEG;
    AccYangle = (float) (atan2( compass.a.z,compass.a.x)+M_PI)*RAD_TO_DEG;
    AccZangle = (float) (atan2( compass.a.x, compass.a.y)+M_PI)*RAD_TO_DEG;
    
    if (AccXangle > 180)
        AccXangle -= (float)360.0;
    if (AccYangle > 180)
        AccYangle -= (float)360.0;

    CFangleX=GYR_PERCENTAGE*(CFangleX+gyrX*dt) +(1 - GYR_PERCENTAGE) * AccXangle;
    CFangleY=GYR_PERCENTAGE*(CFangleY+gyrY*dt) +(1 - GYR_PERCENTAGE) * AccYangle;
    CFangleZ += gyrZ - ZOffset*dt;

    float angle = 0;
    for(int k = 0;k<100;k++)
      angle = analogRead(PinPontDiv);
    angle = (float)angle/100.0;
    
    Serial.println( 
    String(CFangleX, DEC)
    + "#" + String(CFangleY, DEC)
    + "#" + String(CFangleZ, DEC)
    + "#" + String(angle, DEC)
    );
    
    }
    char myBuffer[64];
    while(Serial.available())
    {
      Serial.readBytesUntil('#', myBuffer, 6);
      int angleToReach = atoi(myBuffer);
      Serial.println(angleToReach);
      servo.write(angleToReach);
    }
   
}
