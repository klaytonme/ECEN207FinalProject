#ifndef ACTUATION_H
#define ACTUATION_H


const int tilt_max		  = 200;
const int tilt_speed	  = 50000;	// in 1/us
const int tilt_fast_TO	  = 500000; // in us
const int lift_up_speed	  = 2;		// in PWM
const int lift_up_time	  = 5000000 / lift_up_speed;
const int lift_down_speed = 2; // in PWM

typedef struct {
	int nw;
	int ne;
	int se;
} LiftServoState;

typedef struct {
	int x;
	int y;
} TiltServoState;

enum { LIFT_DIR_UP = 1, LIFT_DIR_DOWN = -1 };


int initActuationPins(void);
int liftSet(LiftServoState);
int tiltSet(TiltServoState);


#endif