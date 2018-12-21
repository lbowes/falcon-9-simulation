#include "IThrustGenerator.h"

namespace Physics {
	namespace Hardware {
		
		IThrustGenerator::IThrustGenerator(CoordTransform3D thrusterToStage) :
			StageComponent(thrusterToStage)
		{ }
		
		void IThrustGenerator::update(double dt) {
			updateDeviceSpecific(dt);
			mThrust_stage = mActive ? mCompToStage.toParentSpace_rotation({ 0.0, mThrustMagnitude, 0.0 }) : glm::dvec3(0.0);
		}

	}
}