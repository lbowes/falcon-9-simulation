#ifndef ANIMATION_H_
#define ANIMATION_H_

#include "AnimationFrame.h"

#include <vector>


template <class Real = double>
chrono::ChCoordsys<Real> lerpCoordSys(const chrono::ChCoordsys<Real>& a, const chrono::ChCoordsys<Real>& b, Real x);

class Animation {
private:
    const double mFrameInterval_s;
    double mDuration_s;
    unsigned int mFrameCount;
    AnimationFrame mVisibleFrame;
    std::vector<AnimationFrame> mFrames;

public:
    Animation(double frameInterval_s, double duration_s);
    ~Animation() = default;

    void updateTime(double time_s);

private:
    void load();
};


#endif

