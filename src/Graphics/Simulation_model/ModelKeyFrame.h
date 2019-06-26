#ifndef GRAPHICS_MODELKEYFRAME_H
#define GRAPHICS_MODELKEYFRAME_H
#pragma once

namespace Graphics {

    // Should contain one graphical keyframe of the entire simulation model at some point in time        
    class ModelKeyFrame {
    private:
        // todo

    public:
        ModelKeyFrame() = default;
        ~ModelKeyFrame() = default;

        static ModelKeyFrame lerp(const ModelKeyFrame& a, const ModelKeyFrame& b, float x);

    };

}

#endif // GRAPHICS_MODELKEYFRAME_H
