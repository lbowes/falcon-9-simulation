#ifndef FALCON_9_SIMULATION_GROUNDMODEL_H
#define FALCON_9_SIMULATION_GROUNDMODEL_H
#pragma once

namespace Physics {
    namespace External {
        class GroundPlane;
    }
}

namespace Graphics {

    class GroundModel {
    private:
        const Physics::External::GroundPlane& mDataSource;

    public:
        GroundModel(const Physics::External::GroundPlane& plane);
        GroundModel() = default;

        void render(const chrono::Vector& currentCamPos_world) const;

    };

}

#endif //FALCON_9_SIMULATION_GROUNDMODEL_H
