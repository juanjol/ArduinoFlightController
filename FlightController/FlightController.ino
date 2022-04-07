#include <Joystick.h>

#include <Keyboard.h>
#include <Servo.h>


Joystick FlightProJoystick;
Servo TrimLevelServo;

const int numReadings = 10;

//Average trim readings
int trimReadings[numReadings];      // the readings from the analog input
int trimReadIndex = 0;              // the index of the current reading
float trimTotal = 0;                  // the running total
float trimAverage = 0;

//Average left pedal readings
int leftPedalReadings[numReadings];      // the readings from the analog input
int leftPedalReadIndex = 0;              // the index of the current reading
float leftPedalTotal = 0;                  // the running total
float leftPedalAverage = 0;

//Average right pedal readings
int rightPedalReadings[numReadings];      // the readings from the analog input
int rightPedalReadIndex = 0;              // the index of the current reading
float rightPedalTotal = 0;                  // the running total
float rightPedalAverage = 0;

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
  trimTotal = trimTotal + value;
  trimReadIndex++;
  if (trimReadIndex >= numReadings) {
    trimReadIndex = 0;
  }
  return trimTotal / numReadings;
}

float leftPedalAverageReadings(int value) {
  leftPedalTotal = leftPedalTotal - leftPedalReadings[leftPedalReadIndex];
  leftPedalReadings[leftPedalReadIndex] = value;
  leftPedalTotal = leftPedalTotal + value;
  leftPedalReadIndex++;
  if (leftPedalReadIndex >= numReadings) {
    leftPedalReadIndex = 0;
  }
  return leftPedalTotal / numReadings;
}

float rightPedalAverageReadings(int value) {
  rightPedalTotal = rightPedalTotal - rightPedalReadings[rightPedalReadIndex];
  rightPedalReadings[rightPedalReadIndex] = value;
  rightPedalTotal = rightPedalTotal + value;
  rightPedalReadIndex++;
  if (rightPedalReadIndex >= numReadings) {
    rightPedalReadIndex = 0;
  }
  return rightPedalTotal / numReadings;
}

void loop() {
  // An average should be made to
  trimAverage = trimAverageReadings(analogRead(trimWheelPin));
  leftPedalAverage = leftPedalAverageReadings(analogRead(leftPedalPin))/2;
  rightPedalAverage = (rightPedalAverageReadings(analogRead(rightPedalPin))/2) + 512;

  // Serial.println(trimAverage);
  FlightProJoystick.setThrottle(trimAverage);
  FlightProJoystick.setXAxis(leftPedalAverage + rightPedalAverage);

  FlightProJoystick.sendState();
  // TrimLevelServo.write(map(trimAverage, 0, 1023, servoMax, servoMin));
  delay(50);
}
