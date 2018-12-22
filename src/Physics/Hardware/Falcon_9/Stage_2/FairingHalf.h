#ifndef PHYSICS_HARDWARE_FAIRINGHALF_H
#define PHYSICS_HARDWARE_FAIRINGHALF_H
#pragma once

#include "Physics/Hardware/Common/Core/StageComponent.hpp"
#include "Physics/External/Environment.h"

#include <PhysicsFramework/RigidBody.h>

namespace Graphics {
	class FairingHalfMesh;
}

namespace Physics {
	namespace Hardware {

		class FairingHalf : public RigidBody {
			friend class Graphics::FairingHalfMesh;
		private:
			static constexpr glm::dvec2 mCentreMassPos_fairing2D = { 1.5235, 6.09934 };

		public:
			FairingHalf();
			~FairingHalf() = default;

			void update(double t, double dt);

		private:
			virtual void addForces(const State& state, double t);
			virtual void addTorques(const State& state, double t);
			void basicCollision_temp();
			Mass recalcMass_local() const;
			InertiaTensor recalcInertia_local() const;

		};

	}
}

#endif
