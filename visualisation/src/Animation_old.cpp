#include "Animation.h"

#include <algorithm>


Animation::Animation(double frameInterval_s, double duration_s) :
    mFrameInterval_s(frameInterval_s),
    mDuration_s(duration_s),
    mFrameCount(0) {

    load();
}


void Animation::updateTime(double time_s) {
    // This function is responsible for updating `mVisibleFrame` to show the state of the simulation at time `time_s`
    // given the data loaded in `mFrames`.

    // Work out which `AnimationFrame`s sit at either side of the time we want to update to
    const unsigned int prevFrameIdx = static_cast<unsigned int>(floor(time_s / mFrameInterval_s));
    const AnimationFrame& prevFrame = mFrames[prevFrameIdx];

    const unsigned int nextFrameIdx = std::clamp(prevFrameIdx + 1, 1U, mFrameCount);
    const AnimationFrame& nextFrame = mFrames[nextFrameIdx];

    const double betweenFrames_0_1 = fmod(time_s, mFrameInterval_s) / mFrameInterval_s;
    AnimationFrame::lerp(prevFrame, nextFrame, betweenFrames_0_1, mVisibleFrame);
}


void Animation::load() {
    // This function is responsible for fully populating `mFrames` with the correct information that has come from the
    // simulation output file. Note, there does not need to be a 1:1 correspondence between lines in the simulation
    // output file and key frames stored in this class. The simulation just outputs 'some' data at 'some' (possibly
    // irregular) timestamps. This function needs to convert this output into an animation format with regularly spaced
    // keyframes.

    // TODO
}
