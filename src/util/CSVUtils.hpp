#ifndef F9SIM_UTIL_CSVROW_HPP_
#define F9SIM_UTIL_CSVROW_HPP_
#pragma once

#include <string>
#include <sstream>
#include <iomanip>

#include <chrono/core/ChCoordsys.h>
#include <chrono/core/ChVector.h>
#include <chrono/core/ChFrameMoving.h>

namespace Util {

    void outputToCSV(double& scalar, std::string& dest) {
        std::stringstream ss;
        ss << std::setprecision(std::numeric_limits<long double>::digits10 + 1);
        ss << dest << ",";
        dest += ss.str();
    }

    void outputToCSV(const chrono::ChVector<double>& vec, std::string& dest) {
        std::stringstream ss;
        ss << std::setprecision(std::numeric_limits<long double>::digits10 + 1);
        ss << vec.x() << "," << vec.y() << "," << vec.z() << ",";
        dest += ss.str();
    }

    void outputToCSV(const chrono::ChQuaternion<double>& quat, std::string& dest) {
        std::stringstream ss;
        ss << std::setprecision(std::numeric_limits<long double>::digits10 + 1);
        ss << quat.e0() << "," << quat.e1() << "," << quat.e2() << "," << quat.e3() << ",";
        dest += ss.str();
    }

    void outputTosCSV(const chrono::ChFrame<double>& frame, std::string& dest) {
        std::stringstream ss;
        ss << std::setprecision(std::numeric_limits<long double>::digits10 + 1);
        outputToCSV(frame.GetPos(), dest);
        outputToCSV(frame.GetRot(), dest);
        dest += ss.str();
    }

    void outputToCSV(const chrono::ChCoordsys<double>& coordSys, std::string& dest) {
        std::stringstream ss;
        ss << std::setprecision(std::numeric_limits<long double>::digits10 + 1);
        outputToCSV(coordSys.pos, dest);
        outputToCSV(coordSys.rot, dest);
        dest += ss.str();
    }

}

#endif // F9SIM_UTIL_CSVROW_HPP_
