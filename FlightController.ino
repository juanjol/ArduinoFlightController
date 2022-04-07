#include <Keyboard.h>
#include <Joystick.h>
#include <Servo.h>


Joystick_ FlightProJoystick;
Servo TrimLevelServo;

const int numReadings = 10;

//Average trim readings
int trimReadings[numReadings];      // the readings from the analog input
int trimReadIndex = 0;              // the index of the current reading
float trimTotal = 0;                  // the running total
float trimAverage = 0;    
 
int servoMin = 60;
int servoMax = 110;

int trimWheelPin = A0;
int rightPedalPin = A1;
int leftPedalPin = A2;

void setup() {
  Serial.begin(9600);
  FlightProJoystick.setThrottleRange(0, 1024);
  FlightProJoystick.begin(); 
  TrimLevelServo.attach(3);
  pinMode(trimWheelPin, INPUT_PULLUP);
  pinMode(rightPedalPin, INPUT_PULLUP);
  pinMode(leftPedalPin, INPUT_PULLUP);
  
  for (int thisReading = 0; thisReading < numReadings; thisReading++) {
    trimReadings[thisReading] = trimAverage;
  }
}

float trimAverageReadings(int value) {
  trimTotal = trimTotal - trimReadings[trimReadIndex];
  trimReadings[trimReadIndex] = value;
  trimTotal = trimTotal + trimReadings[trimReadIndex];
  trimReadIndex = trimReadIndex + 1;
  if (trimReadIndex >= numReadings) {
    trimReadIndex = 0;
  }
  return trimTotal / numReadings;
  
}

void loop() {
  trimAverage = trimAverageReadings(analogRead(trimWheelPin)); 
  Serial.println(trimAverage);
  FlightProJoystick.setThrottle(trimAverage);
  FlightProJoystick.setXAxis(analogRead(rightPedalPin));
  FlightProJoystick.setYAxis(analogRead(leftPedalPin));
  FlightProJoystick.sendState();
  TrimLevelServo.write(map(trimAverage, 0, 1023, servoMax, servoMin));
  delay(50);
}
