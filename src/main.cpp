#include <Joystick.h>
#include <Servo.h>
#include <movingAvg.h>

Joystick_ Joystick;

int zAxis_ = 0; 
int RxAxis_ = 0;                    
int RyAxis_ = 0;  
int RzAxis_ = 0;          
int Throttle_ = 0;         

const bool initAutoSendState = true; 
Servo trimServo,flapServo;
int trimServoValue,trimValue,flapServoValue;
movingAvg trim(10),flap(30),throttle(10),mixture(10),prop(10);


void setup() {
  Joystick.begin();
  trimServo.attach(2);
  flapServo.attach(3);
  trim.begin();
  flap.begin();
  prop.begin();
  mixture.begin();
  throttle.begin();
}

#define TRIM_MIN 256
#define TRIM_MAX 768
#define TRIM_SERVO_TRIP 160
#define TRIM_SERVO_FACTOR 0.625 //TRIM_SERVO_TRIP*2.0/(TRIM_MAX - TRIM_MIN) 
   
void loop() { 
  zAxis_ = analogRead(A0);
  trimValue = zAxis_ > TRIM_MAX ? TRIM_MAX : zAxis_;
  trimValue = trimValue < TRIM_MIN ? TRIM_MIN : trimValue;
  trimValue = trimValue - TRIM_MIN;
  trim.reading(trimValue);
  Joystick.setZAxis(trim.getAvg()); //0 to 512

  trimServoValue = 1500 - ((trim.getAvg() - TRIM_MIN) * TRIM_SERVO_FACTOR);
  trimServo.writeMicroseconds(trimServoValue);
 
  RxAxis_ = analogRead(A1);
  RxAxis_ = map(RxAxis_,0,1023,0,255);
  flap.reading(RxAxis_);
  Joystick.setRxAxis(flap.getAvg());
  
  flapServoValue = 1500;
  flapServo.writeMicroseconds(flapServoValue);

  RyAxis_ = analogRead(A2);
  RyAxis_ = map(RyAxis_,0,1023,0,255);
  prop.reading(RyAxis_);
  Joystick.setRyAxis(prop.getAvg());

  RzAxis_ = analogRead(A3);
  RzAxis_ = map(RzAxis_,1023,0,255,0);   
  mixture.reading(RzAxis_);         
  Joystick.setRzAxis(mixture.getAvg());

  Throttle_ = analogRead(A6);
  Throttle_ = map(Throttle_,1023,0,255,0);
  throttle.reading(Throttle_);        
  Joystick.setThrottle(throttle.getAvg());

  delay (50);
}
