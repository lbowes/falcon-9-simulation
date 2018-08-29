#ifndef PHYSICS_FLUID_H
#define PHYSICS_FLUID_H
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
		enum Names { liquidOxygen, RP1, nitrogen };
		static Fluid mFluids[3];
	};
	
}

#endif
