#include "AnimationFrame.h"


template <class Real = double>
void lerpCoordSys(const chrono::ChCoordsys<Real>& a, const chrono::ChCoordsys<Real>& b, Real x, chrono::ChCoordsys<Real>& dest) {
    dest.pos = a.pos + (b.pos - a.pos) * x;
    dest.rot = a.rot + (b.rot - a.rot) * x;
}


void AnimationFrame::lerp(const AnimationFrame& a, const AnimationFrame& b, double x, AnimationFrame& dest) {
    dest.mTimeStamp_s = a.mTimeStamp_s + (b.mTimeStamp_s - a.mTimeStamp_s) * x;

    for(auto& namedCoordSys : dest.mCoordSystems) {
        const std::string& name = namedCoordSys.first;
        const chrono::Coordsys& coordSysA = a.mCoordSystems.at(name);
        const chrono::Coordsys& coordSysB = b.mCoordSystems.at(name);

        lerpCoordSys(coordSysA, coordSysB, x, namedCoordSys.second);
    }
}
