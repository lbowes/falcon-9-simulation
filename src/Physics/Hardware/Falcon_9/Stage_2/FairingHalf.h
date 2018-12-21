#ifndef PHYSICS_HARDWARE_FAIRINGHALF_H
#define PHYSICS_HARDWARE_FAIRINGHALF_H
#pragma once

#include "Physics/Hardware/Common/Core/StageComponent.hpp"

#include <PhysicsFramework/RigidBody.h>

namespace Graphics {
	class FairingHalfMesh;
}

namespace Physics {
	namespace Hardware {

		class FairingHalf : public RigidBody, public StageComponent {
			friend class Graphics::FairingHalfMesh;
		private:
			static constexpr glm::dvec3 mPos_stage3D = { 0.0, 12.6, 0.0 };            //m

			static constexpr glm::dvec2 mCentreMassPos_fairing2D = { 1.5235, 6.09934 };

		public:
			FairingHalf(double clockingDegree);
			~FairingHalf() = default;

		private:
			Mass recalcMass_local() const;
			InertiaTensor recalcInertia_local() const;
			virtual void addForces(const State& state, double t);
			virtual void addTorques(const State& state, double t);

		};

	}
}

#endif
