/*
 * watertankFmu.cpp
 *
 *  Created on: Mar 3, 2016
 *      Author: kel
 */
#include "Fmu.h"
#include "FmuModel.h"

#define PERIODIC_GENERATED



#include <stdarg.h>
#include "Fmu.h"
#include "Vdm.h"

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

TVP sys = NULL;
fmi2Boolean syncOutAllowed = fmi2True;
fmi2Real maxStepSize = 0.0;


void syncInputsToModel(){
	{
		TVP newValue = newReal(fmiBuffer.realBuffer[0]);
		SET_FIELD(RealPort,RealPort,g_HardwareInterface_forwardRotate,value,newValue);
		vdmFree(newValue);
	}
	{
		TVP newValue = newReal(fmiBuffer.realBuffer[1]);
		TVP p = GET_FIELD(HardwareInterface,HardwareInterface,g_System_hwi,leftVal);
		SET_FIELD(RealPort,RealPort,p,value,newValue);
		vdmFree(newValue);vdmFree(p);
	}
	{
		TVP newValue = newReal(fmiBuffer.realBuffer[2]);
		TVP p = GET_FIELD(HardwareInterface,HardwareInterface,g_System_hwi,rightVal);
		SET_FIELD(RealPort,RealPort,p,value,newValue);
		vdmFree(newValue);vdmFree(p);
	}
	{
		TVP newValue = newReal(fmiBuffer.realBuffer[4]);
		TVP p = GET_FIELD(HardwareInterface,HardwareInterface,g_System_hwi,total_energy_used);
		SET_FIELD(RealPort,RealPort,p,value,newValue);
		vdmFree(newValue);vdmFree(p);
	}
	{
		TVP newValue = newReal(fmiBuffer.realBuffer[6]);
		SET_FIELD(RealPort,RealPort,g_HardwareInterface_backwardRotate,value,newValue);
		vdmFree(newValue);
	}
	{
		TVP newValue = newReal(fmiBuffer.realBuffer[7]);
		SET_FIELD(RealPort,RealPort,g_HardwareInterface_forwardSpeed,value,newValue);
		vdmFree(newValue);
	}
}
void syncOutputsToBuffers(){
	if(syncOutAllowed == fmi2False) return;

	{
		TVP p = GET_FIELD(HardwareInterface,HardwareInterface,g_System_hwi,servo_left_out);
		TVP v = GET_FIELD(RealPort,RealPort,p,value);
		fmiBuffer.realBuffer[3]=v->value.doubleVal;
		vdmFree(v);vdmFree(p);
	}
	{
		TVP p = GET_FIELD(HardwareInterface,HardwareInterface,g_System_hwi,servo_right_out);
		TVP v = GET_FIELD(RealPort,RealPort,p,value);
		fmiBuffer.realBuffer[5]=v->value.doubleVal;
		vdmFree(v);vdmFree(p);
	}
}
void periodic_taskg_System_controller__Z12control_loopEV()
{
	CALL_FUNC(Controller, Controller, g_System_controller, CLASS_Controller__Z12control_loopEV);
	g_fmiCallbackFunctions->logger((void*) 1, g_fmiInstanceName, fmi2OK, "logAll", "called &periodic_taskg_System_controller__Z12control_loopEV\n");
}


struct PeriodicThreadStatus threads[] ={
{ 1.0E7, &periodic_taskg_System_controller__Z12control_loopEV, 0 }
};


void systemInit()
{
	vdm_gc_init();

	int i;

	for(i = 0; i < PERIODIC_GENERATED_COUNT; i++) threads[i].period = threads[i].period / 1.0E9;

	RobotSensor_const_init();
	Port_const_init();
	IntPort_const_init();
	BoolPort_const_init();
	RealPort_const_init();
	StringPort_const_init();
	Controller_const_init();
	System_const_init();
	World_const_init();
	HardwareInterface_const_init();
	RobotServo_const_init();

	RobotSensor_static_init();
	Port_static_init();
	IntPort_static_init();
	BoolPort_static_init();
	RealPort_static_init();
	StringPort_static_init();
	Controller_static_init();
	System_static_init();
	World_static_init();
	HardwareInterface_static_init();
	RobotServo_static_init();

	sys = _Z6SystemEV(NULL);

}


void systemDeInit()
{
	RobotSensor_static_shutdown();
	Port_static_shutdown();
	IntPort_static_shutdown();
	BoolPort_static_shutdown();
	RealPort_static_shutdown();
	StringPort_static_shutdown();
	Controller_static_shutdown();
	System_static_shutdown();
	World_static_shutdown();
	HardwareInterface_static_shutdown();
	RobotServo_static_shutdown();

	RobotSensor_const_shutdown();
	Port_const_shutdown();
	IntPort_const_shutdown();
	BoolPort_const_shutdown();
	RealPort_const_shutdown();
	StringPort_const_shutdown();
	Controller_const_shutdown();
	System_const_shutdown();
	World_const_shutdown();
	HardwareInterface_const_shutdown();
	RobotServo_const_shutdown();

	vdmFree(sys);

	vdm_gc_shutdown();
}



/*
* Both time value are given in seconds
*/
fmi2Status vdmStep(fmi2Real currentCommunicationPoint, fmi2Real communicationStepSize)
{
	int i, j;
	int threadRunCount;

	//Call each thread the appropriate number of times.
	for(i = 0;  i < PERIODIC_GENERATED_COUNT; i++)
	{
		//Times align, sync took place last time.
		if(threads[i].lastExecuted >= currentCommunicationPoint)
		{
			//Can not do anything, still waiting for the last step's turn to come.
			if(threads[i].lastExecuted >= currentCommunicationPoint + communicationStepSize)
			{
				threadRunCount = 0;
				syncOutAllowed = fmi2False;
			}
			//Previous step will finish inside this step.
			//At least one execution can be fit inside this step.
			else if(threads[i].lastExecuted + threads[i].period <= currentCommunicationPoint + communicationStepSize)
			{
				//Find number of executions to fit inside of step, allow sync.
				threadRunCount = (currentCommunicationPoint + communicationStepSize - threads[i].lastExecuted) / threads[i].period;
				syncOutAllowed = fmi2True;
			}
			//Can not execute, but can sync existing values at the end of this step.
			else 
			{
				threadRunCount = 0;
				syncOutAllowed = fmi2True;
			}
		}
		else
		{
			//Find number of executions to fit inside of step, allow sync because need to update regardless.
			threadRunCount = (currentCommunicationPoint + communicationStepSize - threads[i].lastExecuted) / threads[i].period;
			syncOutAllowed = fmi2True;

			//Period too long for this step so postpone until next step.
			if(threadRunCount == 0)
			{
				syncOutAllowed = fmi2False;
			}
		}		

		//Execute each thread the number of times that its period fits in the step size.
		for(j = 0; j < threadRunCount; j++)
		{
			threads[i].call();

			//Update the thread's last execution time.
			threads[i].lastExecuted += threads[i].period;
		}

		vdm_gc();
	}

	/* Calculate maximum step size for next step.  Cyclic controllers with no feedback do not have
	a limit on how large a step they can take.  To be considered in the future for controllers
	with feedback.
	*/
	maxStepSize = INT_MAX * 1.0;

	g_fmiCallbackFunctions->logger((void*) 1, g_fmiInstanceName, fmi2OK, "logDebug", "NOW:  %f, TP: %f, LE:  %f, STEP:  %f, SYNC:  %d, RUNS:  %d\n", currentCommunicationPoint, threads[0].period, threads[0].lastExecuted, communicationStepSize, syncOutAllowed, threadRunCount);

	return fmi2OK;
}

void systemMain()
{
	TVP world = _Z5WorldEV(NULL);
	CALL_FUNC(World, World, world, CLASS_World__Z3runEV);
	vdmFree(world);
}
