#ifndef F9SIM_PHYSICS_FALCON9_VEHICLE_H_
#define F9SIM_PHYSICS_FALCON9_VEHICLE_H_
#pragma once


#include "../3rd_party/json.hpp"

#include <chrono/physics/ChSystemNSC.h>


struct SensorReadings {};
struct ControlProfile {};


namespace F9Sim {
namespace Physics {


class Falcon9_Vehicle {
private:
    chrono::ChSystemNSC& mSystemHandle;

    // temp
    std::shared_ptr<chrono::ChBodyAuxRef> m_cube1;
    std::shared_ptr<chrono::ChBodyAuxRef> m_cube2;
    std::shared_ptr<chrono::ChBody> mFloorPlane;
    //

public:
    Falcon9_Vehicle(chrono::ChSystemNSC& system);
    ~Falcon9_Vehicle() = default;

    SensorReadings readSensors() const;
    void setControlProfile(const ControlProfile& p);
    void saveSnapshotTo(nlohmann::json& snapshot) const;
};


} // namespace Physics
} // namespace F9Sim


#endif // F9SIM_PHYSICS_FALCON9_VEHICLE_H_

