
#include <stdio.h>
#include <sys/time.h>
#include <sys/types.h>
#include "lfr_ctrl.h"

#define FORWARDSPEED	4.0f
#define FORWARDROTATE 	5.0f
#define BACKWARDROTATE	1.0f

#define SENSORVAL	150.0f

#define _ms( t ) ( (t) * 1000 )
float preServoRight = 0;
float preServoLeft = 0;

VSTimer_t t;

void reset( VSTimer_t* timer)
{
  struct timeval now;
  ti_gettimeofday( &now, NULL );
  *timer = now.tv_sec * 1000000 + now.tv_usec;
}

BOOLEAN elapsed( VSTimer_t* timer, long usec )
{
  struct timeval now;
  long long usec_now;

  ti_gettimeofday( &now, NULL );
  usec_now = now.tv_sec * 1000000 + now.tv_usec;
  return ( ( usec_now - (*timer) ) >= usec );
}

int firstCall = 1;

/** Initialize SUT */
void sut_init()
{
	preServoLeft	= 0;
	preServoRight	= 0;
	reset(&t);
}

/** Run SUT (one step) */
void sut_run(float sensorLeftVal, float sensorRightVal, float* servoLeft, float* servoRight)
{
	if(firstCall) 
    {
        if(!elapsed(&t, _ms(1002))) 
        {
			*servoRight = preServoRight;
			*servoLeft = preServoLeft;
            return;
        }
    }
    else 
    {
        if(!elapsed(&t, _ms(1000))) 
        {
			*servoRight = preServoRight;
			*servoLeft = preServoLeft;
            return;
        }
    }

    firstCall = 0;
    reset(&t);

	if(sensorLeftVal < SENSORVAL) 
	{
		if(sensorRightVal < SENSORVAL) 
		{
			preServoRight = *servoRight = -FORWARDSPEED;
			preServoLeft = *servoLeft = FORWARDSPEED;
		}
		else //if(sensorRightVal >= SENSORVAL)
		{
			preServoRight = *servoRight = -BACKWARDROTATE;
			preServoLeft = *servoLeft = FORWARDROTATE;
		}
	}
	else // if(sensorLeftVal >= SENSORVAL)
	{
		if(sensorRightVal < SENSORVAL) 
		{
			preServoRight = *servoRight = -FORWARDROTATE;
			preServoLeft = *servoLeft = BACKWARDROTATE;
		}
		else // if(sensorRightVal >= SENSORVAL)
		{
			*servoRight = preServoRight;
			*servoLeft = preServoLeft;
		}
	}

	return;
}
