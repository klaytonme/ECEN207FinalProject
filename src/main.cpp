#include <Arduino.h>

#include "actuation.h"
#include "sensing.h"


enum class State { INIT, IDLE, IDLE_DANCING, IDLE_AUTO, PLAYING, WIN, LOSE, RESET };
enum class ResetStage { RAISING, TILTING, LOWERING, RIGHTING };

typedef struct {
	State	   state;
	ResetStage reset_stage;
} CTX;

CTX current_state;
CTX next_state;

CTX reset_state() {
	CTX state;
	state.state		  = State::RESET;
	state.reset_stage = ResetStage::RAISING;
	return state;
}


SwitchState	   limit_state;
JoyState	   joy_state;
LiftServoState lift_state;
TiltServoState tilt_state;



const int resetTiltTimeout = 3000000;


time_t current_time, state_start, prev_loop;


int updateState();

int	 liftInc(time_t, bool);
bool liftUp();
bool liftDown();
bool tiltInc(time_t, int, int);



// ============ MAIN ============

void setup() {
	next_state	  = reset_state();
	current_state = next_state;


	current_time = millis();
	state_start	 = current_time;
}

void loop() {
	prev_loop	 = current_time;
	current_time = millis();

	updateState();
}



// ============ STATE FUNCTIONS ============

int updateState() {
	current_state = next_state;

	switch (current_state.state) {
		case State::INIT:
			next_state.state = State::IDLE;
			break;

		case State::IDLE:
			break;

		case State::IDLE_DANCING:
			break;

		case State::IDLE_AUTO:
			break;

		case State::PLAYING:
			break;

		case State::WIN:
			break;

		case State::LOSE:
			break;

		case State::RESET:
			switch (current_state.reset_stage) {
				case ResetStage::RAISING:
					liftInc(current_time - prev_loop, 0);

					if (!liftUp())
						break;

					next_state.reset_stage = ResetStage::TILTING;
					state_start			   = current_time;

					break;

				case ResetStage::TILTING:
					if (!tiltInc(current_time - prev_loop, -tilt_max, -tilt_max))
						break;

					if (current_time < state_start + resetTiltTimeout)
						break;

					next_state.reset_stage = ResetStage::LOWERING;
					state_start			   = current_time;

					break;

				case ResetStage::LOWERING:
					liftInc(current_time - prev_loop, 1);

					if (!liftDown())
						break;

					next_state.reset_stage = ResetStage::TILTING;
					state_start			   = current_time;

					break;

				case ResetStage::RIGHTING:
					if (!tiltInc(current_time - prev_loop, 0, 0))
						break;

					next_state.state = State::IDLE;
					state_start		 = current_time;

					break;
			}
			break;

		default:
			current_state = reset_state();
			state_start	  = current_time;
			prev_loop	  = current_time;
			break;
	}
}

int updateState() {
	current_state = next_state;
}



int liftInc(time_t time_elapsed, bool dir) {
	int inc = (time_elapsed / lift_speed) * (dir * -1);


	lift_state.ne += inc;
	lift_state.nw += inc;
	lift_state.se += inc;

	liftSet(lift_state);

	return 0;
}

bool liftUp() {
	return lift_state.nw >= lift_up.nw || lift_state.ne >= lift_up.ne || lift_state.se >= lift_up.se;
}

bool liftDown() {
	limit_state = readLimits();

	return limit_state.nw && limit_state.ne && limit_state.se;
}


bool tiltInc(time_t time_elapsed, int x_targ, int y_targ) {
	bool completed = true;

	int inc = (time_elapsed / tilt_speed);

	int e_x = x_targ - tilt_state.x;
	int e_y = y_targ - tilt_state.y;

	if (abs(e_x) < inc)
		tilt_state.x = x_targ;
	else {
		tilt_state.x += inc * (e_x > 0);
		completed = false;
	}

	if (abs(e_y) < inc)
		tilt_state.y = y_targ;
	else {
		tilt_state.y += inc * (e_y > 0);
		completed = false;
	}

	return completed;
}