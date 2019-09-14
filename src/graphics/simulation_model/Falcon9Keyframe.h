#ifndef F9SIM_GRAPHICS_FALCON9KEYFRAME_H_
#define F9SIM_GRAPHICS_FALCON9KEYFRAME_H_
#pragma once

#include <chrono/core/ChFrame.h>
#include <chrono/core/ChMatrixDynamic.h>
#include <map>

template <class Real = double>
chrono::ChCoordsys<Real> lerp(const chrono::ChCoordsys<Real>& a, const chrono::ChCoordsys<Real>& b, Real x);

namespace Graphics {

    // Should contain one graphical Keyframe of the entire simulation model at some point in time
    class Falcon9Keyframe {
    private:
        std::map<std::string, chrono::ChCoordsys<double>> mCoordSystems;

    public:
        Falcon9Keyframe();
        Falcon9Keyframe(const std::string& csvRow);
        ~Falcon9Keyframe() = default;

        static Falcon9Keyframe lerp(const Falcon9Keyframe& a, const Falcon9Keyframe& b, float x);

        const chrono::ChCoordsys<double>* getCoordSystem(const std::string& name) const;

    private:
        void addCoordSystems();
        void loadFromCSV(const std::string& csvRow);

    };

}

#endif // F9SIM_GRAPHICS_FALCON9KEYFRAME_H_
