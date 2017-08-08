
#ifndef _LFR_CTRL_H_
#define _LFR_CTRL_H_

typedef long long VSTimer_t;
typedef unsigned char BOOLEAN;

/** Initialize SUT */
extern void sut_init();

/** Run SUT (one step) */
extern void sut_run(float sensorLeftVal, float sensorRightVal, float* servoLeft, float* servoRight);

BOOLEAN elapsed( VSTimer_t* timer, long usec );
// -------------------------------------------------------------------------
extern int ti_gettimeofday(struct timeval *tv, struct timezone *tz);
// -------------------------------------------------------------------------
//
#endif /* _LFR_CTRL_H_ */
