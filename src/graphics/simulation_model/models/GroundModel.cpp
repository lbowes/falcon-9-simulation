#include "GroundModel.h"
#include "../../../physics/external/GroundPlane.h"

#include <chrono_irrlicht/ChIrrTools.h>
#include <chrono_irrlicht/ChIrrWizard.h>

namespace Graphics {

    GroundModel::GroundModel(irr::video::IVideoDriver& vidDriver) :
        mVidDriver(&vidDriver)
    { }

    void GroundModel::render(const chrono::Vector& currentCamPos_world) const {
        using namespace chrono;

		//See F9S1Model::update() for explanation
		const chrono::ChVector<> displacement_world = Physics::External::GroundPlane::getPos_world() - currentCamPos_world;

		irrlicht::ChIrrTools::drawGrid(
			mVidDriver,
			1,
			1,
			50,
			50,
			ChCoordsys<>(displacement_world, Q_from_AngAxis(CH_C_PI / 2, VECT_X)),
            irr::video::SColor(10, 100, 100, 100),
			true
		);

        const chrono::ChVector<> 
            springStart = chrono::ChVector<>(-10, 0, -10) - currentCamPos_world,
            springEnd = chrono::ChVector<>(-10, 40, -10) - currentCamPos_world;

        irrlicht::ChIrrTools::drawSpring(mVidDriver, 0.5, springStart, springEnd, irr::video::SColor(255, 255, 255, 255), 65, 10.0, true);
    }

}