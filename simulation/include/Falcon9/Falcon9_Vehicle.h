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
    chrono::ChSystemNSC& m_systemHandle;

    // temp
    std::shared_ptr<chrono::ChBodyAuxRef> m_cube;
    std::shared_ptr<chrono::ChBody> m_floorPlane;
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

