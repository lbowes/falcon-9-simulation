#include "FairingHalf.h"

namespace Physics {
	namespace Hardware { 

		FairingHalf::FairingHalf(double clockingDegree) :
			StageComponent({ mPos_stage3D, glm::rotate(glm::radians(clockingDegree), glm::dvec3(0.0, 1.0, 0.0)) }, recalcMass_local(), recalcInertia_local())
		{ }

		Mass FairingHalf::recalcMass_local() const {
			//TODO: Just an estimate
			return Mass(1000.0, {0.0, mCentreMassPos_fairing2D.y, -mCentreMassPos_fairing2D.x});
		}
		
		InertiaTensor FairingHalf::recalcInertia_local() const {
			//TODO: Currently unknown
			return InertiaTensor();
		}

		void FairingHalf::addForces(const State& state, double t) { }
		
		void FairingHalf::addTorques(const State& state, double t) { }

	}
}