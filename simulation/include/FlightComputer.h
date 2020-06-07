#ifndef SOFTWARE_FLIGHTCOMPUTER_H_
#define SOFTWARE_FLIGHTCOMPUTER_H_


#include "Falcon9/Falcon9_Vehicle.h"


namespace Software {


class FlightComputer {
public:
    FlightComputer() = default;
    ~FlightComputer() = default;

    ControlProfile process(const SensorReadings& readings);
};


} // namespace Software


#endif // SOFTWARE_FLIGHTCOMPUTER_H_

