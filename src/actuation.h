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
	TiltServoState orig_;
	TiltServoState state_;
	TiltServoState targ_;
	time_t		   duration;

	const int PIN_X;
	const int PIN_Y;

	int tiltUpdate();

  public:
	TiltController(int, int);

	void		   initPins(void);
	TiltServoState getPosition(void);
	void		   setTraj(TiltServoState, int = TILTSPEED_IMMEDIATE);
	int			   updateTraj(time_t);
	int			   set(TiltServoState);
	void		   center(int = TILTSPEED_IMMEDIATE);
};



enum { LIFT_DIR_UP = 1, LIFT_DIR_DOWN = -1 };

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


struct LimitState;

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
	void raise(int = lift_up_speed);
	void lower(LimitState, int = lift_down_speed);
};


#endif