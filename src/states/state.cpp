#include <Arduino.h>

#include "context.h"
#include "state.h"

void		  State::setContext(Context* ctx) { ctx_ = ctx; }
void		  State::setStateStart(unsigned long state_start) { state_start_ = state_start; }
unsigned long State::t() { return ctx_->t(); }