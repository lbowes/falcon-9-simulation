#ifndef F9SIM_GRAPHICS_CAMERAS_H_
#define F9SIM_GRAPHICS_CAMERAS_H_
#pragma once


#include "CameraBaseState.h"

#include <chrono/core/ChVector.h>


namespace bx {
struct Vec3;
}


namespace F9Sim {
namespace Graphics {


class CameraSystem {
private:
    static const bx::Vec3 s_eye;
    static CameraBaseState const* s_activeCamera;
    static CameraBaseState s_defaultCamera;
    static std::unordered_map<std::string, const CameraBaseState*> s_cameraMap;

public:
    static CameraSystem& getInstance();

    CameraSystem(const CameraSystem&) = delete;
    CameraSystem(CameraSystem&&) = delete;
    void operator=(const CameraSystem&) = delete;
    void operator=(CameraSystem&&) = delete;

    chrono::Vector getActivePos();
    void registerCam(const CameraBaseState& cam, const std::string& name);
    void bind(const std::string& name);
    void setViewTransform(float aspectRatio);

private:
    CameraSystem();
    ~CameraSystem() = default;
};


} // namespace Graphics
} // namespace F9Sim


#endif // F9SIM_GRAPHICS_CAMERAS_H_

