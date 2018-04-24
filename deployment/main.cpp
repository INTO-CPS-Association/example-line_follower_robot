#include "Arduino.h"

#include <stdarg.h>
#include <stdio.h>
#include <stdio.h>

#include "modeldescription.h"
extern "C" {

#include "Fmu.h"
//#include "FmuModel.h"
#include "Controller.h"

}
int enA = 2;
int in1 = 3;
int in2 = 4;
// motor two

int in3 = 5;
int in4 = 6;
int enB = 7;

#define SENSOR_CENTER_LEFT_PIN A7
#define SENSOR_CENTER_CENTER_PIN A6
#define SENSOR_CENTER_RIGHT_PIN A5

#define MOTOR_LEFT 0
#define MOTOR_RIGHT 1




void direction(int motor, bool fw) {

	int k1 = 0;
	int k2 = 0;

	if (motor == MOTOR_LEFT) {
		k1 = in1;
		k2 = in2;
	} else if (motor == MOTOR_RIGHT) {
		k1 = in3;
		k2 = in4;
	} else {
		return;
	}

	if (fw) {
		digitalWrite(k1, HIGH);
		digitalWrite(k2, LOW);
	} else {
		digitalWrite(k1, LOW);
		digitalWrite(k2, HIGH);
	}
}

#define TURN_RIGHT 0
#define TURN_LEFT 1
void turn(bool dir) {
	if (dir == TURN_RIGHT) {
		direction(MOTOR_LEFT, true);
		direction(MOTOR_RIGHT, false);
	} else {
		direction(MOTOR_LEFT, false);
		direction(MOTOR_RIGHT, true);
	}
}




void fmuLoggerCache(void *componentEnvironment, fmi2String instanceName,
		fmi2Status status, fmi2String category, fmi2String format, ...) {
return;
//	Serial.println(format);
	va_list args;
	size_t len;
	char *space;

	va_start(args, format);
	len = vsnprintf(0, 0, format, args);
	va_end(args);
	if ((space = (char*)malloc(len + 1)) != 0) {
		va_start(args, format);
		vsnprintf(space, len + 1, format, args);
		va_end(args);
		Serial.println(space);
		free(space);
	}

}

fmi2Component instReturn = NULL;
double now = 0;
double step = 0.00001;

int alive = 0;

#define FR 5.0
#define FS 4.0
#define BR 1.0

#define ALIVE_PIN 13

int availableMemory()
{
  int size = 8192;
  byte *buf;
  while ((buf = (byte *) malloc(--size)) == NULL);
  free(buf);
  return size;
}

void setup() {

	pinMode(ALIVE_PIN, OUTPUT);
	Serial.begin(115200);
	while (!Serial)
		;
	Serial.println("booting...");

	Serial.println("setting pin mode");
	// set all the motor control pins to outputs
	pinMode(enA, OUTPUT);
	pinMode(enB, OUTPUT);
	pinMode(in1, OUTPUT);
	pinMode(in2, OUTPUT);
	pinMode(in3, OUTPUT);
	pinMode(in4, OUTPUT);

//	pinMode(13,OUTPUT);
	pinMode(SENSOR_CENTER_RIGHT_PIN, INPUT);
	pinMode(SENSOR_CENTER_CENTER_PIN, INPUT);
	pinMode(SENSOR_CENTER_LEFT_PIN, INPUT);

	Serial.println("setting motor direction motors");
	// turn motor A FW
	digitalWrite(in1, HIGH);
	digitalWrite(in2, LOW);

	// turn motor B FW
	digitalWrite(in3, HIGH);
	digitalWrite(in4, LOW);

	// turn on

	Serial.println("starting motors low speed");
	analogWrite(enA, 50);
	analogWrite(enB, 50);

	for (int i = 0; i < 9; i++)
		fmiBuffer.realBuffer[i] = 0.0;

	fmiBuffer.realBuffer[FMI_LEFTVAL] = 0.0;
	fmiBuffer.realBuffer[FMI_RIGHTVAL] = 0.0;
	fmiBuffer.realBuffer[FMI_SERVOLEFTOUT] = -9.99;
	fmiBuffer.realBuffer[FMI_SERVORIGHTOUT] = -9.99;
	fmiBuffer.realBuffer[FMI_THRESHOLD] = 400.0;
	fmiBuffer.realBuffer[FMI_FORWARDROTATE] = FR;
	fmiBuffer.realBuffer[FMI_BACKWARDROTATE] = BR;
	fmiBuffer.realBuffer[FMI_FORWARDSPEED] = FS;
	




	fmi2CallbackFunctions callback = { &fmuLoggerCache, NULL, NULL, NULL, NULL };

	Serial.println("Instantiating: ");
	Serial.println(availableMemory());
	instReturn = fmi2Instantiate("this system", fmi2CoSimulation,	FMI_GUID, "", &callback, fmi2True, fmi2True);
	Serial.println("Instantiate called");
	Serial.println(availableMemory());
	Serial.println(instReturn == NULL);
	Serial.println("setup done");

	//callback.logger(NULL, "some name", fmi2OK, "logAll", "called &periodic_taskg_System_controller__Z12control_loopEV\n");
}

int count = 0;

// The loop function is called in an endless loop
void loop() {
//	Serial.print("loop\n");
//	return;
	if (instReturn == NULL)
		return;

//	Serial.println("looping");
//Add your repeated code here
	//Read switch values that indicate sensor threshold crossings.
	//Serial.println("ADC'ing");
	fmiBuffer.realBuffer[FMI_LEFTVAL] = 500*!digitalRead(SENSOR_CENTER_LEFT_PIN);
	fmiBuffer.realBuffer[FMI_RIGHTVAL] = 500*!digitalRead(SENSOR_CENTER_RIGHT_PIN);
	
/*	Serial.print("L ");
	Serial.print(fmiBuffer.realBuffer[FMI_LEFTVAL]);
	Serial.print(" R ");
	Serial.println(fmiBuffer.realBuffer[FMI_RIGHTVAL]);
*/
	Serial.println("fmi2DoStep");
	fmi2DoStep(NULL, now, step, false);
//	syncInputsToModel();
//CALL_FUNC(Controller, Controller, g_System_controller, CLASS_Controller__Z12control_loopEV);
	//threads[0].call();
	//vdm_gc();
 //syncOutputsToBuffers();
//	Serial.println("fmi2DoStep done");

	now = now + step;

	// sync buffer with hardware
	double servoRight = fmiBuffer.realBuffer[FMI_SERVORIGHTOUT];
	double servoLeft = fmiBuffer.realBuffer[FMI_SERVOLEFTOUT];
	
	/*Serial.print("LM ");
	Serial.print(servoLeft);
	Serial.print(" RM ");
	Serial.println(servoRight);
*/
	if (servoRight == -FS && servoLeft == FS) {
		//Go Strait
		direction(MOTOR_LEFT, true);
		direction(MOTOR_RIGHT, true);
//		Serial.println("||");
	}

	if (servoRight == -FR && servoLeft == BR) {
		//go left
		turn(TURN_LEFT);
//		Serial.println("<");
	}

	if (servoRight == -BR && servoLeft == FR) {
		//go right
		turn(TURN_RIGHT);
//		Serial.println(">");
	}

	// alive indicator
	digitalWrite(ALIVE_PIN, alive);

	alive = !alive;

	//Serial.println(availableMemory());
	//Serial.println(count++);
	//delay(1000);
}


