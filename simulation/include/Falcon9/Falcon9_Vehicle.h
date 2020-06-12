#ifndef HARDWARE_FALCON9_VEHICLE_H_
#define HARDWARE_FALCON9_VEHICLE_H_
#pragma once


#include "../3rd_party/json.hpp"

#include <chrono/physics/ChSystemNSC.h>


struct SensorReadings {};
struct ControlProfile {};


namespace Hardware {


class Falcon9_Vehicle {
private:
    chrono::ChSystemNSC& mSystemHandle;

    // temp
    std::shared_ptr<chrono::ChBodyAuxRef> mCube;
    std::shared_ptr<chrono::ChBody> mFloorPlane;
    //

public:
    Falcon9_Vehicle(chrono::ChSystemNSC& system);
    ~Falcon9_Vehicle() = default;

    SensorReadings readSensors() const;
    void setControlProfile(const ControlProfile& p);
    void saveSnapshotTo(nlohmann::json& snapshot) const;
};


} // namespace Hardware


#endif // HARDWARE_FALCON9_VEHICLE_H_

