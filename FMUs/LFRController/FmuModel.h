#ifndef FMU_MODEL_H_
#define FMU_MODEL_H_

#include "RobotSensor.h"
#include "Port.h"
#include "IntPort.h"
#include "BoolPort.h"
#include "RealPort.h"
#include "StringPort.h"
#include "Controller.h"
#include "System.h"
#include "World.h"
#include "HardwareInterface.h"
#include "RobotServo.h"

#define PERIODIC_GENERATED_COUNT 1



void syncInputsToModel();
void syncOutputsToBuffers();
void systemInit();
void systemDeInit();

extern struct PeriodicThreadStatus threads[];

#endif /* FMU_MODEL_H_ */
