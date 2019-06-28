#ifndef GRAPHICS_MODELKEYFRAME_H
#define GRAPHICS_MODELKEYFRAME_H
#pragma once

#include <chrono/core/ChFrame.h>
#include <chrono/core/ChMatrixDynamic.h>

namespace Graphics {

    // Should contain one graphical keyframe of the entire simulation model at some point in time        
    class ModelKeyFrame {
    // temp: should not be public
    public:
        // todo
        chrono::ChCoordsys<> mS1RP1TankFrame_world;

    public:
        ModelKeyFrame() = default;
        ModelKeyFrame(const std::string& csvRow);
        ~ModelKeyFrame() = default;

        static ModelKeyFrame lerp(const ModelKeyFrame& a, const ModelKeyFrame& b, float x);

    };

}

#endif // GRAPHICS_MODELKEYFRAME_H
