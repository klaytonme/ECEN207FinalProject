#include <Arduino.h>

#include "actuation.h"
#include "main.h"
#include "sensing.h"

CTX current_state;
CTX next_state;

SwitchState	   limit_state;
JoyState	   joy_state;
LiftServoState lift_top;
LiftServoState lift_state;
TiltServoState tilt_state;


time_t current_time, state_start, prev_loop;


int updateState();

int	 liftMove(int, int = 0);
bool liftIsDown();
bool tiltInc(time_t, int, int);
int	 tiltTo(int, int);



// ============ MAIN ============

void setup() {
	initSensingPins();
	initActuationPins();

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
		case State::INIT: {

			if ((current_state.init & InitFlags::UP) == InitFlags::NONE) {
				liftMove(current_time - prev_loop, 0);

				if (current_time < state_start + lift_up_time)
					break;

				next_state.init |= InitFlags::UP;
			}
			if ((current_state.init & InitFlags::DOWN) == InitFlags::NONE) {
				liftMove(LIFT_DIR_DOWN);

				if (!liftIsDown())
					break;

				next_state.init |= InitFlags::DOWN;
			}
			if ((current_state.init & InitFlags::N) == InitFlags::NONE) {
				if (!tiltInc(current_time - prev_loop, 0, tilt_max))
					break;

				next_state.init |= InitFlags::N;
			}
			if ((current_state.init & InitFlags::S) == InitFlags::NONE) {
				if (!tiltInc(current_time - prev_loop, 0, -tilt_max))
					break;

				next_state.init |= InitFlags::S;
			}
			if ((current_state.init & InitFlags::CENTER1) == InitFlags::NONE) {
				if (!tiltInc(current_time - prev_loop, 0, 0))
					break;

				next_state.init |= InitFlags::CENTER1;
			}
			if ((current_state.init & InitFlags::W) == InitFlags::NONE) {
				if (!tiltInc(current_time - prev_loop, -tilt_max, 0))
					break;

				next_state.init |= InitFlags::W;
			}
			if ((current_state.init & InitFlags::E) == InitFlags::NONE) {
				if (!tiltInc(current_time - prev_loop, tilt_max, 0))
					break;

				next_state.init |= InitFlags::E;
			}
			if ((current_state.init & InitFlags::CENTER2) == InitFlags::NONE) {
				if (!tiltInc(current_time - prev_loop, 0, 0))
					break;

				next_state.init |= InitFlags::N;
				state_start = current_time;
			}
			if ((current_state.init & InitFlags::NFAST) == InitFlags::NONE) {
				tiltTo(0, tilt_max);

				if (current_time < state_start + tilt_fast_TO)
					break;

				next_state.init |= InitFlags::NFAST;
				state_start = current_time;
			}
			if ((current_state.init & InitFlags::SFAST) == InitFlags::NONE) {
				tiltTo(0, -tilt_max);

				if (current_time < state_start + tilt_fast_TO)
					break;

				next_state.init |= InitFlags::SFAST;
				state_start = current_time;
			}
			if ((current_state.init & InitFlags::CENTER3) == InitFlags::NONE) {
				tiltTo(0, 0);

				if (current_time < state_start + tilt_fast_TO)
					break;

				next_state.init |= InitFlags::CENTER3;
				state_start = current_time;
			}
			if ((current_state.init & InitFlags::WFAST) == InitFlags::NONE) {
				tiltTo(-tilt_max, 0);

				if (current_time < state_start + tilt_fast_TO)
					break;

				next_state.init |= InitFlags::WFAST;
				state_start = current_time;
			}
			if ((current_state.init & InitFlags::EFAST) == InitFlags::NONE) {
				tiltTo(tilt_max, 0);

				if (current_time < state_start + tilt_fast_TO)
					break;

				next_state.init |= InitFlags::EFAST;
				state_start = current_time;
			}
			if ((current_state.init & InitFlags::CENTER4) == InitFlags::NONE) {
				tiltTo(0, 0);

				if (current_time < state_start + tilt_fast_TO)
					break;

				next_state.init |= InitFlags::CENTER4;
				state_start = current_time;
			}
			if ((current_state.init & InitFlags::DISP) == InitFlags::NONE) {
				current_state.init |= InitFlags::DISP;
			}
			if ((current_state.init & InitFlags::BUTTON) == InitFlags::NONE) {
				current_state.init |= InitFlags::BUTTON;
			}

			next_state.state = State::IDLE;
			break;
		}
		case State::IDLE: {
			break;
		}
		case State::IDLE_DANCING: {
			break;
		}
		case State::IDLE_AUTO: {
			break;
		}
		case State::PLAYING: {
			break;
		}
		case State::WIN: {
			break;
		}
		case State::LOSE: {
			break;
		}
		case State::RESET: {
			switch (current_state.reset_stage) {
				case ResetStage::UP:
					liftMove(LIFT_DIR_UP);

					if (current_time < state_start + lift_up_time)
						break;

					next_state.reset_stage = ResetStage::TILT;
					state_start			   = current_time;

					break;

				case ResetStage::TILT:
					if (!tiltInc(current_time - prev_loop, -tilt_max, -tilt_max))
						break;

					if (current_time < state_start + resetTiltTimeout)
						break;

					next_state.reset_stage = ResetStage::DOWN;
					state_start			   = current_time;

					break;

				case ResetStage::DOWN:
					liftMove(LIFT_DIR_DOWN);

					if (!liftIsDown())
						break;

					next_state.reset_stage = ResetStage::TILT;
					state_start			   = current_time;

					break;

				case ResetStage::CENTER:
					if (!tiltInc(current_time - prev_loop, 0, 0))
						break;

					next_state.state = State::IDLE;
					state_start		 = current_time;

					break;
			}
			break;
		}
		default: {
			current_state = reset_state();
			state_start	  = current_time;
			prev_loop	  = current_time;
			break;
		}
	}
}



int liftMove(int dir, int speed) {
	if (!speed)
		speed = dir == LIFT_DIR_UP ? lift_up_speed : lift_down_speed;

	if (dir) {
		lift_state.ne = speed;
		lift_state.nw = speed;
		lift_state.se = speed;

	} else {
		limit_state = readLimits();

		int velocity  = (speed * dir);
		lift_state.nw = 90 + (velocity * !limit_state.nw);
		lift_state.ne = 90 + (velocity * !limit_state.nw);
		lift_state.se = 90 + (velocity * !limit_state.nw);
	}

	liftSet(lift_state);

	return 0;
}

bool liftIsDown() {
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

	tiltSet(tilt_state);

	return completed;
}

int tiltTo(int x_targ, int y_targ) {
	tilt_state.x = x_targ;
	tilt_state.y = y_targ;

	tiltSet(tilt_state);
}
