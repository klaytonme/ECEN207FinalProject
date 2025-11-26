#ifndef STATEINIT_H
#define STATEINIT_H

#include "state.h"

enum class InitFlags : unsigned int {
	NONE	= 0,
	UP		= 1 << 0,
	DOWN	= 1 << 1,
	N		= 1 << 2,
	S		= 1 << 3,
	CENTER1 = 1 << 4,
	W		= 1 << 5,
	E		= 1 << 6,
	CENTER2 = 1 << 7,
	NFAST	= 1 << 8,
	SFAST	= 1 << 9,
	CENTER3 = 1 << 10,
	WFAST	= 1 << 11,
	EFAST	= 1 << 12,
	CENTER4 = 1 << 13,
	DISP	= 1 << 14,
	BUTTON	= 1 << 15,
	ALL		= 0xFFFFFFFF
};

#define ENABLE_BITMASK_OPERATORS(x)                                                                                    \
	inline constexpr x operator|(x a, x b) {                                                                           \
		using T = std::underlying_type_t<x>;                                                                           \
		return static_cast<x>(static_cast<T>(a) | static_cast<T>(b));                                                  \
	}                                                                                                                  \
	inline constexpr x operator&(x a, x b) {                                                                           \
		using T = std::underlying_type_t<x>;                                                                           \
		return static_cast<x>(static_cast<T>(a) & static_cast<T>(b));                                                  \
	}                                                                                                                  \
	inline constexpr x operator^(x a, x b) {                                                                           \
		using T = std::underlying_type_t<x>;                                                                           \
		return static_cast<x>(static_cast<T>(a) ^ static_cast<T>(b));                                                  \
	}                                                                                                                  \
	inline constexpr x operator~(x a) {                                                                                \
		using T = std::underlying_type_t<x>;                                                                           \
		return static_cast<x>(~static_cast<T>(a));                                                                     \
	}                                                                                                                  \
	inline x& operator|=(x& a, x b) {                                                                                  \
		return a = a | b;                                                                                              \
	}                                                                                                                  \
	inline x& operator&=(x& a, x b) {                                                                                  \
		return a = a & b;                                                                                              \
	}                                                                                                                  \
	inline x& operator^=(x& a, x b) {                                                                                  \
		return a = a ^ b;                                                                                              \
	}                                                                                                                  \
	inline constexpr bool any(x a) {                                                                                   \
		using T = std::underlying_type_t<x>;                                                                           \
		return static_cast<T>(a) != 0;                                                                                 \
	}                                                                                                                  \
	inline constexpr bool all(x value, x flags) {                                                                      \
		using T = std::underlying_type_t<x>;                                                                           \
		return (static_cast<T>(value) & static_cast<T>(flags)) == static_cast<T>(flags);                               \
	}

ENABLE_BITMASK_OPERATORS(InitFlags)



class InitState : public State {
  public:
	void enter() override;
	void update() override;
	void exit() override;

  private:
	InitFlags init_;
};


#endif