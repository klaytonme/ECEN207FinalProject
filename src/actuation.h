#ifndef ACTUATION_H
#define ACTUATION_H


const int tilt_max				  = 200;
const int lift_up_speed			  = 2; // in PWM
const int lift_up_time			  = 5000000 / lift_up_speed;
const int lift_down_speed		  = 2; // in PWM
const int tilt_speed_immediate_TO = 500000;

enum { TILTSPEED_SLOW = 10000, TILTSPEED_FAST = 5000, TILTSPEED_IMMEDIATE = 0 }; // in us/deg

typedef struct TiltServoState {
	int x;
	int y;
	TiltServoState() {
		x = 0;
		y = 0;
	};
	TiltServoState(int x_in, int y_in) {
		x = x_in;
		y = y_in;
	}
};

class TiltController {
  private:
	TiltServoState orig;
	TiltServoState state;
	TiltServoState targ;
	time_t		   duration;

	int tiltUpdate();

  public:
	TiltController();

	TiltServoState getPosition(void);
	void		   setTraj(TiltServoState, int = TILTSPEED_IMMEDIATE);
	int			   updateTraj(time_t);
	int			   tiltSet(TiltServoState);
	void		   center(int = TILTSPEED_IMMEDIATE);
};

typedef struct {
	int nw;
	int ne;
	int se;
} LiftServoState;

enum { LIFT_DIR_UP = 1, LIFT_DIR_DOWN = -1 };


int initActuationPins(void);
int liftSet(LiftServoState);
int tiltSet(TiltServoState);


#endif