#ifndef F9SIM_PHYSICS_EXTERNAL_GROUNDPLANE_H
#define F9SIM_PHYSICS_EXTERNAL_GROUNDPLANE_H
#pragma once

#include <core/ChVector.h>
#include <physics/ChBodyAuxRef.h>

namespace chrono {
    class ChSystemNSC;
}

namespace Physics {
    namespace External {

        class GroundPlane {
        private:
            static const chrono::ChVector<> mPos_world;
            static const float mThickness;
            chrono::ChSystemNSC& mSystemHandle;
            std::shared_ptr<chrono::ChBodyAuxRef> mBody;

        public:
            GroundPlane(chrono::ChSystemNSC& systemHandle);
            ~GroundPlane() = default;

            static const chrono::Vector& getPos_world();
        
        };

    }
}

#endif // F9SIM_PHYSICS_EXTERNAL_GROUNDPLANE_H
