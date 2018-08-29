#include "Fluid.h"

namespace Physics {
	Fluid Propellants::mFluids[3] = {
		Fluid(1230.0, FluidState::liquid),
		Fluid(860.0, FluidState::liquid),
		Fluid(280.0, FluidState::gas)
	};
}
