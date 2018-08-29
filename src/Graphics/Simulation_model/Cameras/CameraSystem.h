/* The camera system should be responsible for the ownership of the different Cameras in the simulation.
 * It should also manage the transfer of control between them, deciding which one is active etc.
*/

#ifndef GRAPHICS_CAMERASYSTEM_H
#define GRAPHICS_CAMERASYSTEM_H
#pragma once

#include "AllCameras.h"

#include <vector>

#define FPV_CAM static_cast<FPVCamera*>(mCameras[0].get())
#define INTERSTAGE_CAM static_cast<InterstageCamera*>(mCameras[1].get())
#define CHASER_CAM static_cast<ChaserCamera*>(mCameras[2].get())

namespace Physics {
	namespace Hardware {
		class Falcon9;
	}
}

namespace Graphics {
	
	class CameraSystem {
	public:
		enum CameraNames : unsigned char { FPV, interstage, chaser/*, droneShip*/ };
	
	private:
		std::vector<std::unique_ptr<SimulationCamera>> mCameras;

		unsigned mCurrentCamera = FPV;

		bool mHasFocus = false;

	public:
		CameraSystem(const State& stage1State, float windowAspect);
		~CameraSystem() = default;

		void update(float windowAspect, float dt, glm::dvec3 stage1CoMPosition_world);
		void checkInput(float dt);
		
		bool hasFocus() const { return mHasFocus; }
		SimulationCamera& getCurrentSimCamera() const { return *mCameras[mCurrentCamera].get(); }

		void shouldHaveFocus(bool shouldHaveFocus) { mHasFocus = shouldHaveFocus; }

	private:
		void addAllCameras(const State& stage1State, float windowAspect);

	};

}

#endif
