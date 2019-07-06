#ifndef PHYSICS_SYSTEM_H
#define PHYSICS_SYSTEM_H
#pragma once

#include "Internal/Hardware/Falcon_9/Falcon9.h"
#include "External/GroundPlane.h"

#include <chrono/physics/ChSystemNSC.h>

namespace Physics {

    class System {
    private:
        chrono::ChSystemNSC mSystem;
		Hardware::Falcon9 mFalcon9;
		External::GroundPlane mGround;

    public:
        System();
        ~System() = default;

        void update(double dt);
        std::string sample() const;

    };

}

#endif
