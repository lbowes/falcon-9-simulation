/* AnimationFrame
 *
 * This class encapsulates a single animation keyframe of the simulation.
 * It stores a time stamp associated with the frame, and a set of named coordinate systems representing the positions
 * and orientations of objects included in the frame. 
*/

#ifndef ANIMATIONFRAME_H_
#define ANIMATIONFRAME_H_


#include <chrono/core/ChCoordsys.h>

#include <unordered_map>


template <class Real = double>
void lerpCoordSys(const chrono::ChCoordsys<Real>& a, const chrono::ChCoordsys<Real>& b, Real x, chrono::ChCoordsys<Real>& dest);


class AnimationFrame {
private:
    double mTimeStamp_s;
    std::unordered_map<std::string, chrono::ChCoordsys<double>> mCoordSystems;

public:
    AnimationFrame() = default;
    ~AnimationFrame() = default;

    static void lerp(const AnimationFrame& a, const AnimationFrame& b, double x, AnimationFrame& dest);
};


#endif // ANIMATIONFRAME_H_

