#ifndef PHYSICS_DYNAMICSIMSTATE_H
#define PHYSICS_DYNAMICSIMSTATE_H
#pragma once

#include <core/ChFrame.h>

namespace Physics {
	namespace Hardware {
		class Falcon9;
	}
}

namespace Physics {

	/*
	Holds one snapshot of the dynamic state generated at each timestep by the simulation.
	*/
	class DSS {
	private:
		unsigned long mNumber;

		struct F9S1 {
			chrono::ChFrame<> mStage1ToWorldTransform; 
		} f9s1;

	public:
		DSS() = default;
		DSS(const Hardware::Falcon9& f9, unsigned long snapshotNumber);
		~DSS() = default;
		
	};

}

#endif
