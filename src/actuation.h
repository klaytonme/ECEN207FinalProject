#ifndef ACTUATION_H
#define ACTUATION_H


const int tilt_max	 = 200;
const int tilt_speed = 50000; // in 1/us
const int lift_diff	 = 200;
const int lift_speed = 50000; // in 1/us

typedef struct {
	int nw;
	int ne;
	int se;
} LiftServoState;

typedef struct {
	int x;
	int y;
} TiltServoState;

LiftServoState lift_up;


int liftSet(LiftServoState);
int tiltSet(TiltServoState);


#endif