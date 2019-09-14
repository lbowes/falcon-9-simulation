#include "Falcon9Keyframe.h"

template <class Real = double>
chrono::ChCoordsys<Real> lerp(const chrono::ChCoordsys<Real>& a, const chrono::ChCoordsys<Real>& b, Real x) {
    chrono::ChCoordsys<Real> output;

    // Position
	chrono::Vector
		pos_a = a.pos,
		pos_b = b.pos;

    output.pos = pos_a + (pos_b - pos_a) * x;

    // Rotation
	chrono::Quaternion
		rot_a = a.rot,
		rot_b = b.rot;

    output.rot = rot_a + (rot_b - rot_a) * x;

    return output;
}

namespace Graphics {

    Falcon9Keyframe::Falcon9Keyframe() {
        addCoordSystems();
    }

    Falcon9Keyframe::Falcon9Keyframe(const std::string& csvRow) {
        addCoordSystems();
        loadFromCSV(csvRow);
    }

    Falcon9Keyframe Falcon9Keyframe::lerp(const Falcon9Keyframe& a, const Falcon9Keyframe& b, float x) {
        Falcon9Keyframe result;

        for(auto& c : result.mCoordSystems)
            c.second = ::lerp<double>(a.mCoordSystems.at(c.first), b.mCoordSystems.at(c.first), x);

		return result;
    }

    const chrono::ChCoordsys<double>* Falcon9Keyframe::getCoordSystem(const std::string& name) const {
        if(mCoordSystems.find(name) == mCoordSystems.end()) {
            printf("Falcon9Keyframe::getCoordSystem(): name '%s' passed in is invalid", name.c_str());
            return nullptr;
        }
        else
            return &mCoordSystems.at(name);
    }

    void Falcon9Keyframe::addCoordSystems() {
        mCoordSystems["S1RP1Tank"] = chrono::ChCoordsys<double>();
        mCoordSystems["LandingLeg_0"] = chrono::ChCoordsys<double>();

        //mCoordSystems[""] = chrono::ChCoordsys<double>();
        //TODO: add more
    }

    void Falcon9Keyframe::loadFromCSV(const std::string& csvRow) {
        std::vector<double> data;
        std::stringstream ss(csvRow);
        double d;

        while (ss >> d) {
            data.push_back(d);

            if (ss.peek() == ',')
                ss.ignore();
        }

        // The index into the array of doubles on a CSV line corresponding to the first data point for this coordinate system (pos.x)
        unsigned short dataStartIdx = 0;
        for(auto& c : mCoordSystems) {
            c.second.pos = { data[dataStartIdx], data[dataStartIdx + 1], data[dataStartIdx + 2] };
            c.second.rot = { data[dataStartIdx + 3], data[dataStartIdx + 4], data[dataStartIdx + 5], data[dataStartIdx + 6] };
            dataStartIdx += 7;
        }
    }

}