#include <Adafruit_MotorShield.h>

Adafruit_MotorShield ms = Adafruit_MotorShield(); 

Adafruit_DCMotor *leftMotor = ms.getMotor(1);
Adafruit_DCMotor *rightMotor = ms.getMotor(2);

String inputString = "";         // a String to hold incoming data
boolean stringComplete = false;  // whether the string is complete

void setup() {
  // initialize serial:
  Serial.begin(9600);
  ms.begin();
  // reserve 200 bytes for the inputString:
  inputString.reserve(200);
  
}

void loop() {
  // print the string when a newline arrives:
  if (stringComplete) {
    Serial.println(inputString);
    
    if (inputString.equals("release")){
      releaseMotors();
    } else {
      //Takes inputString (which is the drivespeed and turnvalue separated by a colon) and splits it into it's values, also the values should be from -255 to 255
      int driveSpeed = inputString.substring(0, inputString.indexOf(':')).toInt();
      int turnValue = inputString.substring(inputString.indexOf(':') + 1).toInt();
  
      //takes driveSpeed and turnValue and make them motor speeds (-255 to 255, where 0 is not moving, 255 is full forward and 255 is full back)f
      int leftMotorSpeed = constrain(turnValue + driveSpeed, -255, 255);
      int rightMotorSpeed = constrain(turnValue - driveSpeed, -255, 255);
  
      //set the motor speed
      driveLeft(leftMotorSpeed);
      driveRight(rightMotorSpeed);
    }
    // clear the string:
    inputString = "";
    stringComplete = false;
  }
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
  }
}

void driveLeft(int speed){
  //spin left motor at speed, -255 is full reverse, 0 is not moving, and 255 is full forward
  leftMotor->setSpeed(speed);
  if (speed > 0){
    leftMotor->run(FORWARD);
  } else {
    leftMotor->run(BACKWARD);
  }
}
void releaseMotors(){
  leftMotor->run(RELEASE);
  rightMotor->run(RELEASE);
}

void driveRight(int speed){
  //spin left motor at speed, -255 is full reverse, 0 is not moving, and 255 is full forward
  rightMotor->setSpeed(speed);
  if (speed > 0){
    rightMotor->run(FORWARD);
  } else {
    rightMotor->run(BACKWARD);
  }
}

