#include <Adafruit_MotorShield.h>

Adafruit_MotorShield ms = Adafruit_MotorShield(); 

Adafruit_DCMotor *leftMotor1 = ms.getMotor(1);
Adafruit_DCMotor *leftMotor2 = ms.getMotor(2);
Adafruit_DCMotor *rightMotor1 = ms.getMotor(3);
Adafruit_DCMotor *rightMotor2 = ms.getMotor(4);


String inputString = "";         // a String to hold incoming data
boolean stringComplete = false;  // whether the string is complete

void setup() {
  // initialize serial:
  Serial.begin(576000);
  Serial.setTimeout(100);
  ms.begin();
  // reserve 200 bytes for the inputString:
  inputString.reserve(200);
  
}

void loop() {
  // print the string when a newline arrives:
  if (stringComplete) {
    clearBuffer();
    if (inputString.equals("release")){
      releaseMotors();
    } else {
      //Takes inputString (which is the drivespeed and turnvalue separated by a colon) and splits it into it's values, also the values should be from -255 to 255
      int driveSpeed = inputString.substring(0, inputString.indexOf(':')).toInt();
      int turnValue = inputString.substring(inputString.indexOf(':') + 1).toInt();
//      Serial.print("Drive speed is ");
//      Serial.print(driveSpeed);
//      Serial.print("    turnValue is ");
//      Serial.println(turnValue);
      //takes driveSpeed and turnValue and make them motor speeds (-255 to 255, where 0 is not moving, 255 is full forward and 255 is full back)f
      int leftMotorSpeed = constrain(driveSpeed + turnValue, -255, 255);
      int rightMotorSpeed = constrain(driveSpeed - turnValue, -255, 255);
  
      //set the motor speed
      driveLeft(leftMotorSpeed);
      driveRight(rightMotorSpeed);
    }
    // clear the string:
    inputString = "";
    stringComplete = false;
    clearBuffer();
  }
}
void clearBuffer(){
  while(Serial.available()){Serial.read();}
}
/*
  SerialEvent occurs whenever a new data comes in the hardware serial RX.
*/
void serialEvent() {
  while (Serial.available()) {
    // get the new byte:
    char inChar = (char)Serial.read();
    // add it to the inputString:
    inputString += inChar;
    // if the incoming character is a newline, set a flag so the main loop can
    // do something about it:
    if (inChar == '\n') {
      stringComplete = true;
    }
    delay(1);
  }
}

void driveLeft(int speed){
  //spin left motor at speed, -255 is full reverse, 0 is not moving, and 255 is full forward

  leftMotor1->setSpeed(abs(speed));
  leftMotor2->setSpeed(abs(speed));
  if (speed > 0){
    leftMotor1->run(FORWARD);
    leftMotor2->run(FORWARD);
  } else {
    leftMotor1->run(BACKWARD);
    leftMotor2->run(BACKWARD);
  }
}
void releaseMotors(){
  leftMotor1->run(RELEASE);
  rightMotor1->run(RELEASE);
  leftMotor2->run(RELEASE);
  rightMotor2->run(RELEASE);
}

void driveRight(int speed){
  //spin left motor at speed, -255 is full reverse, 0 is not moving, and 255 is full forward
  rightMotor1->setSpeed(abs(speed));
  rightMotor2->setSpeed(abs(speed));
  if (speed > 0){
    rightMotor1->run(FORWARD);
    rightMotor2->run(FORWARD);
  } else {
    rightMotor1->run(BACKWARD);
    rightMotor2->run(BACKWARD);
  }
}

