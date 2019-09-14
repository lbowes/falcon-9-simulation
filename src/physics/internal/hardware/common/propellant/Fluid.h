#ifndef F9SIM_PHYSICS_FLUID_H
#define F9SIM_PHYSICS_FLUID_H
#pragma once

namespace Physics {
	
	enum class FluidState { liquid, gas };

	struct Fluid {
		const FluidState mState;
		const double mDensity;

		Fluid(double density, FluidState state) :
			mState(state),
			mDensity(density)
		{ }
	};

	class Propellants {
	public:
		enum Names : unsigned char { liquidOxygen, RP1, nitrogen };
		static Fluid mFluids[3];
	};
	
}

#endif
