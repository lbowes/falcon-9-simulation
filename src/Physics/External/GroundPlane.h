#ifndef PHYSICS_EXTERNAL_GROUNDPLANE_H
#define PHYSICS_EXTERNAL_GROUNDPLANE_H
#pragma once

#include <core/ChVector.h>
#include <physics/ChBody.h>

namespace chrono {
    class ChSystemNSC;
}

namespace Physics {
    namespace External {

        class GroundPlane {
        private:
            chrono::ChSystemNSC& mSystemHandle;
            std::shared_ptr<chrono::ChBody> mBody;

        public:
            GroundPlane(chrono::ChSystemNSC& systemHandle);
            GroundPlane() = default;

            const chrono::Vector& getPos_world() const;

        };

    }
}

#endif
