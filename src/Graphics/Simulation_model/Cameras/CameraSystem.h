#ifndef GRAPHICS_CAMERASYSTEM_H
#define GRAPHICS_CAMERASYSTEM_H
#pragma once

#include "AllCameras.h"
#include "../../../3rd_Party/json.hpp"
#include "../../ISerialisable.hpp"

#include <vector2d.h>
#include <vector>

namespace Input {
	class HWEventReceiver;
}

namespace irr {
	class IrrlichtDevice;

	namespace scene {
		class ISceneManager;
	}
}

namespace Graphics {

	class CameraSystem : public ISerialisable {
	public:
		enum CameraNames : unsigned char { FPV, interstage/*, chaser, droneShip*/ };
	
	private:
		irr::IrrlichtDevice& mDevice;
		irr::scene::ISceneManager& mSceneManager;
		const Input::HWEventReceiver& mHWInput;
		std::vector<std::unique_ptr<SimulationCamera>> mCameras;
		unsigned char mCurrentCamera;
		bool mHasFocus;

	public:
		CameraSystem(irr::IrrlichtDevice& device, irr::scene::ISceneManager& sceneManager, Input::HWEventReceiver& input, float windowAspect);
		~CameraSystem() = default;

		void update(chrono::ChCoordsys<double> stage1Transform_world, float windowAspect, float dt);
		void handleInput(irr::core::vector2di centreScreenPos_scr, float dt);
        void save(nlohmann::json& dest) const override;
        void load(const std::string& source) override;

		bool hasFocus() const { return mHasFocus; }
		SimulationCamera& getCurrentSimCamera() const { return *mCameras[mCurrentCamera].get(); }
		void setFocus(bool shouldHaveFocus) { mHasFocus = shouldHaveFocus; }

	};

}

#endif // GRAPHICS_CAMERASYSTEM_H
