#ifndef ACTUATION_H
#define ACTUATION_H

#include "sensing.h"

const int tilt_max = 200;

const int kLiftUpTime	= 2000000;
const int kLiftMaxSpeed = 15;

const int			tilt_speed_immediate_TO = 500000;
const unsigned long kTiltInitTO				= 10000000;

const int kTiltMin			= 570;
const int kTiltMax			= 2320;
const int kServoRefreshRate = 50000;


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
	};
};

enum { TRAJ_DURATION, TRAJ_SPEED };
enum { TILTSPEED_SLOW = 10000, TILTSPEED_FAST = 5000, TILTSPEED_IMMEDIATE = 0 }; // in us/deg

class TiltController {
  private:
	TiltServoState orig_;
	TiltServoState state_;
	TiltServoState targ_;
	time_t		   duration;

	TiltServoState center_;
	TiltServoState min_;
	TiltServoState max_;

	const int kPinX_;
	const int kPinY_;

	int tiltUpdate();

  public:
	TiltController(int, int);

	void		   initPins(void);
	TiltServoState getPosition(void);

	void		   updateCenter(void);
	TiltServoState getCenter(void) { return center_; }
	void		   toCenter(int = TRAJ_SPEED, int = TILTSPEED_IMMEDIATE);

	void		   updateBounds(void);
	TiltServoState getMin(void) { return min_; }
	TiltServoState getMax(void) { return max_; }

	void setTraj(TiltServoState, int = TRAJ_SPEED, int = TILTSPEED_IMMEDIATE);
	int	 updateTraj(time_t);
	int	 set(TiltServoState);

	const int kInitListLength = 6;
};



enum { LIFT_DIR_DOWN = -1, LIFT_DIR_STOP = 0, LIFT_DIR_UP = 1 };

typedef struct LiftServoState {
	int nw;
	int ne;
	int se;
	LiftServoState() {
		nw = 0;
		ne = 0;
		se = 0;
	};
	LiftServoState(int nw_in, int ne_in, int se_in) {
		nw = nw_in;
		ne = ne_in;
		se = se_in;
	}
};

class LiftController {
  private:
	const int	   kPinNW_;
	const int	   kPinNE_;
	const int	   kPinSE_;
	LiftServoState state_;

	void update();

  public:
	LiftController(int, int, int);
	void initPins(void);
	void set(LiftServoState);
	void write(int = LIFT_DIR_UP, LimitState = LimitState(false, false, false));
	void stop();

	const int kLiftMax_ = kLiftMaxSpeed;
};


#endif