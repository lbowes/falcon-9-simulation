#ifndef PHYSICS_HARDWARE_GASTHRUSTER_H
#define PHYSICS_HARDWARE_GASTHRUSTER_H
#pragma once

#include "IThrustGenerator.h"
#include "Physics/Hardware/Common/Propellant/SupplyLine.h"

namespace Graphics {
	class GasThrusterMesh;
}

namespace Physics {
	namespace Hardware {

		class GasThruster : public IThrustGenerator {
			friend class Graphics::GasThrusterMesh;
			friend class Physics::DSS;
		private:
			GasSupplyLine* mSupplyLine = nullptr;
			
			double
				mCurrentMassFlowRate = 0.0,
				mIsp = 0.0; //TODO - Implement a use for specific impulse

		public:
			GasThruster(CoordTransform3D thrusterToStage);
			~GasThruster() = default;

			void attachPropSupplyLine(GasSupplyLine* supplyLine) { mSupplyLine = supplyLine; }

		private:
			void setTypeSpecificParams();
			void updateDeviceSpecific(double dt);

		};
		
	}
}

#endif