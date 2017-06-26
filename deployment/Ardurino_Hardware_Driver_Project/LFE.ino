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

// left rs A1 23
const int RS_LEFT_PIN = 23;
//right rs A0 24
const int RS_RIGHT_PIN = 24;
//left servo 5
const int SERVO_LEFT_PIN = 5;
//right servo 6
const int SERVO_RIGHT_PIN = 6;

//IO
//8 is connected to ATMega1284p pin 17 D3
const int SERVO_LEFT_ENABLE_PIN = 8;
//9 is connected to ATMega1284p pin 16 D2
const int SERVO_RIGHT_ENABLE_PIN = 9;

//software serial
//Arduino TX 10 to ATMega1284p RX pin 14
const int SW_SERIAL_TX_PIN = 10;
//Arduino RX 11 to ATMega1284p TX pin 15
const int SW_SERIAL_RX_PIN = 11;

SoftwareSerial mySerial(SW_SERIAL_RX_PIN, SW_SERIAL_TX_PIN); // RX, TX

//
//const int fullSpeedClockwise = 1300;
//1530
//const int fullSpeedCounterclockwise = 1700;
const int deadStill = 1528;

const int fullSpeedClockwise = deadStill-10;
const int fullSpeedCounterclockwise = deadStill+15;



//left servi still 1528

void setup() {
	pinMode(SERVO_LEFT_ENABLE_PIN, INPUT);
	pinMode(SERVO_RIGHT_ENABLE_PIN, INPUT);

	pinMode(RS_LEFT_PIN, INPUT);
	pinMode(RS_RIGHT_PIN, INPUT);

	leftServo.attach(SERVO_LEFT_PIN);
	rightServo.attach(SERVO_RIGHT_PIN);

	Serial.begin(57600);
	while (!Serial) {
		; // wait for serial port to connect. Needed for native USB port only
	}

	Serial.println("Ready!");

	// set the data rate for the SoftwareSerial port
	mySerial.begin(19200);
	//mySerial.println("Hello, world?");
}

uint8_t leftServoState = 4;
uint8_t rightServoState = 4;
int leftSensorValue = 0;
int rightSensorValue = 0;

void loop() {


	uint8_t lServoCmd = digitalRead(SERVO_LEFT_ENABLE_PIN);
	uint8_t rServoCmd = digitalRead(SERVO_RIGHT_ENABLE_PIN);

	//rServoCmd=!lServoCmd;

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

	leftSensorValue = analogRead(RS_LEFT_PIN);
	rightSensorValue = analogRead(RS_RIGHT_PIN);
//	Serial.print("Sensor Left = ");
//	Serial.println(leftSensorValue);
//	Serial.print("Sensor Right = ");
//	Serial.println(leftSensorValue);

	if (mySerial.available()) {
		int d = mySerial.read();
		Serial.write(d);
		if(d=='X')
		{
			Serial.write("\r\n");
		}

	}
//	if (Serial.available()) {
//		mySerial.write(Serial.read());
//	}

	delay(15);                           // waits for the servo to get there
}

