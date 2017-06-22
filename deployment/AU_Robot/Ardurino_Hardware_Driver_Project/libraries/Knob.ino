/*
 Controlling a servo position using a potentiometer (variable resistor)
 by Michal Rinott <http://people.interaction-ivrea.it/m.rinott>

 modified on 8 Nov 2013
 by Scott Fitzgerald
 http://www.arduino.cc/en/Tutorial/Knob
 */
#include <Arduino.h>
#include <Servo.h>
#include <SoftwareSerial.h>

//http://learn.parallax.com/tutorials/robot/shield-bot/robotics-board-education-shield-arduino/chapter-2-shield-lights-servo-11
Servo leftServo;  // create servo object to control a servo
Servo rightServo;

const int leftServoPpmPin = 9;
const int rightServoPpmPin = 10;

const int leftServoInPinA = 2;
const int leftServoInPinB = 3;

const int rightServoInPinA = 4;
const int rightServoInPinB = 5;

const int rightIrSensorPin = 6;
const int leftIrSensorPin = 7;

const int fullSpeedClockwise = 1300;
const int fullSpeedCounterclockwise = 1700;

void setup() {
	leftServo.attach(leftServoPpmPin); // attaches the servo on pin 9 to the servo object
	rightServo.attach(rightServoPpmPin);

//	leftServo.writeMicroseconds(1200);
//	leftServo.writeMicroseconds(1700);
//	leftServo.writeMicroseconds(1500);

	pinMode(leftServoInPinA, INPUT);
//	pinMode(leftServoInPinB, INPUT);

	pinMode(rightServoInPinA, INPUT);
//	pinMode(rightServoInPinB, INPUT);

	pinMode(rightIrSensorPin, INPUT);
	pinMode(leftIrSensorPin, INPUT);

	Serial.begin(57600);
	while (!Serial) {
		; // wait for serial port to connect. Needed for native USB port only
	}

	Serial.println("Ready!");
}

uint8_t leftServoState = 4;
uint8_t rightServoState = 4;
int leftSensorValue = 0;
int rightSensorValue = 0;

void loop() {

	uint8_t lServoCmd = digitalRead(leftServoInPinA);
	uint8_t rServoCmd = digitalRead(rightServoInPinA);
	//& (digitalRead(leftServoInPinB) << 1);

	if (leftServoState != lServoCmd) {

		if (lServoCmd == 0) {
			Serial.println("Servo Left = CCW");
			leftServo.writeMicroseconds(fullSpeedCounterclockwise);
		} else if (lServoCmd > 0) {
			Serial.println("Servo Left = CW");
			leftServo.writeMicroseconds(fullSpeedClockwise);
		}
		leftServoState = lServoCmd;
	}

	//& (digitalRead(leftServoInPinB) << 1);
	if (rightServoState != rServoCmd) {
		if (rServoCmd == 0) {
			Serial.println("Servo Right = CCW");
			rightServo.writeMicroseconds(fullSpeedCounterclockwise);
		} else if (rServoCmd > 0) {
			Serial.println("Servo Right = CW");
			rightServo.writeMicroseconds(fullSpeedClockwise);
		}
		rightServoState = rServoCmd;
	}

	leftSensorValue = analogRead(leftIrSensorPin);
	rightSensorValue = analogRead(rightIrSensorPin);
	Serial.print("Sensor Left = ");
	Serial.println(leftSensorValue);
	Serial.print("Sensor Right = ");
	Serial.println(leftSensorValue);

	delay(15);                           // waits for the servo to get there
}

