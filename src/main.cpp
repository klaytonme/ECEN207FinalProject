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
TiltController TiltCont;


time_t current_time, state_start, prev_loop;


int updateState();

int	 liftMove(int, int = 0);
bool liftIsDown();



// ============ MAIN ============

void setup() {
	Serial.begin(9600);
	Serial.println("Ran!");

	initSensingPins();
	initActuationPins();

	next_state	  = init_state();
	current_state = next_state;


	current_time = micros();
	state_start	 = current_time;
}

void loop() {
	prev_loop	 = current_time;
	current_time = micros();

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
				Serial.println("Down");
				next_state.init |= InitFlags::UP;
			}
			if ((current_state.init & InitFlags::DOWN) == InitFlags::NONE) {
				liftMove(LIFT_DIR_DOWN);

				if (!liftIsDown())
					break;

				Serial.println("North");
				next_state.init |= InitFlags::DOWN;

				state_start = current_time;
				TiltCont.setTraj(TiltServoState(0, tilt_max), TILTSPEED_SLOW);
			}
			if ((current_state.init & InitFlags::N) == InitFlags::NONE) {
				if (!TiltCont.updateTraj(current_time - state_start))
					break;

				Serial.println("South");
				next_state.init |= InitFlags::N;

				state_start = current_time;
				TiltCont.setTraj(TiltServoState(0, -tilt_max), TILTSPEED_SLOW);
			}
			if ((current_state.init & InitFlags::S) == InitFlags::NONE) {
				if (!TiltCont.updateTraj(current_time - state_start))
					break;

				Serial.println("Center");
				next_state.init |= InitFlags::S;

				state_start = current_time;
				TiltCont.center(TILTSPEED_SLOW);
			}
			if ((current_state.init & InitFlags::CENTER1) == InitFlags::NONE) {
				if (!TiltCont.updateTraj(current_time - state_start))
					break;

				Serial.println("West");
				next_state.init |= InitFlags::CENTER1;

				state_start = current_time;
				TiltCont.setTraj(TiltServoState(-tilt_max, 0), TILTSPEED_SLOW);
			}
			if ((current_state.init & InitFlags::W) == InitFlags::NONE) {
				if (!TiltCont.updateTraj(current_time - state_start))
					break;

				Serial.println("East");
				next_state.init |= InitFlags::W;

				state_start = current_time;
				TiltCont.setTraj(TiltServoState(tilt_max, 0), TILTSPEED_SLOW);
			}
			if ((current_state.init & InitFlags::E) == InitFlags::NONE) {
				if (!TiltCont.updateTraj(current_time - state_start))
					break;

				Serial.println("Center");
				next_state.init |= InitFlags::E;

				state_start = current_time;
				TiltCont.center(TILTSPEED_SLOW);
			}
			if ((current_state.init & InitFlags::CENTER2) == InitFlags::NONE) {
				if (!TiltCont.updateTraj(current_time - state_start))
					break;

				Serial.println("North Fast");
				next_state.init |= InitFlags::CENTER2;

				state_start = current_time;
				TiltCont.setTraj(TiltServoState(0, tilt_max), TILTSPEED_IMMEDIATE);
			}
			if ((current_state.init & InitFlags::NFAST) == InitFlags::NONE) {
				if (current_time < state_start + tilt_speed_immediate_TO)
					break;

				Serial.println("South Fast");
				next_state.init |= InitFlags::NFAST;

				state_start = current_time;
				TiltCont.setTraj(TiltServoState(0, -tilt_max), TILTSPEED_IMMEDIATE);
			}
			if ((current_state.init & InitFlags::SFAST) == InitFlags::NONE) {
				if (current_time < state_start + tilt_speed_immediate_TO)
					break;

				Serial.println("Center");
				next_state.init |= InitFlags::SFAST;

				state_start = current_time;
				TiltCont.center(TILTSPEED_IMMEDIATE);
			}
			if ((current_state.init & InitFlags::CENTER3) == InitFlags::NONE) {
				if (current_time < state_start + tilt_speed_immediate_TO)
					break;

				Serial.println("West Fast");
				next_state.init |= InitFlags::CENTER3;

				state_start = current_time;
				TiltCont.setTraj(TiltServoState(-tilt_max, 0), TILTSPEED_IMMEDIATE);
			}
			if ((current_state.init & InitFlags::WFAST) == InitFlags::NONE) {
				if (current_time < state_start + tilt_speed_immediate_TO)
					break;

				Serial.println("East Fast");
				next_state.init |= InitFlags::WFAST;

				state_start = current_time;
				TiltCont.setTraj(TiltServoState(tilt_max, 0), TILTSPEED_IMMEDIATE);
			}
			if ((current_state.init & InitFlags::EFAST) == InitFlags::NONE) {
				if (current_time < state_start + tilt_speed_immediate_TO)
					break;

				Serial.println("Center");
				next_state.init |= InitFlags::EFAST;

				state_start = current_time;
				TiltCont.center(TILTSPEED_IMMEDIATE);
			}
			if ((current_state.init & InitFlags::CENTER4) == InitFlags::NONE) {
				if (current_time < state_start + tilt_speed_immediate_TO)
					break;

				Serial.println("Display");
				next_state.init |= InitFlags::CENTER4;
				state_start = current_time;
			}
			if ((current_state.init & InitFlags::DISP) == InitFlags::NONE) {
				Serial.println("Button");
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
					TiltCont.setTraj(TiltServoState(-tilt_max, -tilt_max), TILTSPEED_FAST);

					break;

				case ResetStage::TILT:
					if (!TiltCont.updateTraj(current_time - state_start))
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
					TiltCont.center(TILTSPEED_FAST);

					break;

				case ResetStage::CENTER:
					if (!TiltCont.updateTraj(current_time - state_start))
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
