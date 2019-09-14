#ifndef F9SIM_PHYSICS_SYSTEM_H_
#define F9SIM_PHYSICS_SYSTEM_H_
#pragma once

#include "internal/hardware/falcon_9/Falcon9.h"
#include "external/GroundPlane.h"

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

#endif // F9SIM_PHYSICS_SYSTEM_H_
