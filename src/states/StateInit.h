#ifndef STATEINIT_H
#define STATEINIT_H

#include "state.h"

// enum class InitFlags : unsigned int {
// 	NONE   = 0,
// 	UP	   = 1 << 0,
// 	DOWN   = 1 << 1,
// 	N	   = 1 << 2,
// 	S	   = 1 << 3,
// 	W	   = 1 << 5,
// 	E	   = 1 << 6,
// 	CENTER = 1 << 4,
// 	JOY	   = 1 << 7,
// 	CALIB  = 1 << 8,
// 	BUTTON = 1 << 9,
// 	DISP   = 1 << 10,
// 	ALL	   = 0xFFFFFFFF
// };

// #define ENABLE_BITMASK_OPERATORS(x) \
// 	inline constexpr x operator|(x a, x b) {                                                                           \
// 		using T = std::underlying_type_t<x>;                                                                           \
// 		return static_cast<x>(static_cast<T>(a) | static_cast<T>(b));                                                  \
// 	}                                                                                                                  \
// 	inline constexpr x operator&(x a, x b) {                                                                           \
// 		using T = std::underlying_type_t<x>;                                                                           \
// 		return static_cast<x>(static_cast<T>(a) & static_cast<T>(b));                                                  \
// 	}                                                                                                                  \
// 	inline constexpr x operator^(x a, x b) {                                                                           \
// 		using T = std::underlying_type_t<x>;                                                                           \
// 		return static_cast<x>(static_cast<T>(a) ^ static_cast<T>(b));                                                  \
// 	}                                                                                                                  \
// 	inline constexpr x operator~(x a) {                                                                                \
// 		using T = std::underlying_type_t<x>;                                                                           \
// 		return static_cast<x>(~static_cast<T>(a));                                                                     \
// 	}                                                                                                                  \
// 	inline x& operator|=(x& a, x b) {                                                                                  \
// 		return a = a | b;                                                                                              \
// 	}                                                                                                                  \
// 	inline x& operator&=(x& a, x b) {                                                                                  \
// 		return a = a & b;                                                                                              \
// 	}                                                                                                                  \
// 	inline x& operator^=(x& a, x b) {                                                                                  \
// 		return a = a ^ b;                                                                                              \
// 	}                                                                                                                  \
// 	inline constexpr bool any(x a) {                                                                                   \
// 		using T = std::underlying_type_t<x>;                                                                           \
// 		return static_cast<T>(a) != 0;                                                                                 \
// 	}                                                                                                                  \
// 	inline constexpr bool all(x value, x flags) {                                                                      \
// 		using T = std::underlying_type_t<x>;                                                                           \
// 		return (static_cast<T>(value) & static_cast<T>(flags)) == static_cast<T>(flags);                               \
// 	}

// ENABLE_BITMASK_OPERATORS(InitFlags)

// class InitSubState : public State {
//   public:
// 	void enter() override;
// 	void update() override;
// 	void exit() override;

// 	void setParentState(State*);

//   public:
// 	State* parentState;
// };

class InitSubState;

class InitState : public State {
  public:
	void enter() override;
	void update() override;
	void exit() override;

	void transitionTo(InitSubState*);

  private:
	InitSubState* substate_;
	// InitFlags init_;
};


class InitSubState : public State {
  public:
	virtual ~InitSubState() {}

	virtual void enter() override {};
	virtual void update() override {};
	virtual void exit() override {};

	void setContext(Context* ctx, InitState* parent) {
		ctx_	= ctx;
		parent_ = parent;
	};

  public:
	InitState* parent_;
	Context*   ctx_;
};


class InitSubstateButton : public InitSubState {
	void enter();
};
class InitSubstateTestLift : public InitSubState {
	void update();
};
class InitSubstateInitLift : public InitSubState {
	void update();
};

#endif