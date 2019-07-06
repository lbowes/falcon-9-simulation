#ifndef GRAPHICS_GROUNDMODEL_H
#define GRAPHICS_GROUNDMODEL_H
#pragma once

#include <chrono/core/ChVector.h>

namespace Physics {
    namespace External {
        class GroundPlane;
    }
}

namespace irr {
	namespace video {
		class IVideoDriver;
    }
}

namespace Graphics {

    class GroundModel {
    private:
        irr::video::IVideoDriver* mVidDriver;

    public:
        GroundModel(irr::video::IVideoDriver& vidDriver);
        ~GroundModel() = default;

        void render(const chrono::Vector& currentCamPos_world) const;

    };

}

#endif // GRAPHICS_GROUNDMODEL_H
