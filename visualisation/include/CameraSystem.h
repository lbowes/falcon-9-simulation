#ifndef F9SIM_GRAPHICS_CAMERAS_H_
#define F9SIM_GRAPHICS_CAMERAS_H_
#pragma once


#include "CameraBaseState.h"

#include <bx/math.h>
#include <chrono/core/ChVector.h>


namespace F9Sim {
namespace Graphics {


class CameraSystem {
private:
    const bx::Vec3 m_eye;
    CameraBaseState const* m_activeCamera;
    CameraBaseState m_defaultCamera;
    std::unordered_map<std::string, const CameraBaseState*> m_cameraMap;

public:
    static CameraSystem& getInstance();

    CameraSystem(const CameraSystem&) = delete;
    CameraSystem(CameraSystem&&) = delete;
    void operator=(const CameraSystem&) = delete;
    void operator=(CameraSystem&&) = delete;

    chrono::Vector getActivePos() const;
    bool registerCam(const CameraBaseState& cam, const std::string& name);
    bool bind(const std::string& name);
    void setViewTransform(float aspectRatio);
    std::vector<std::string> getRegisteredCamNames() const;

private:
    CameraSystem();
    ~CameraSystem() = default;
};


} // namespace Graphics
} // namespace F9Sim


#endif // F9SIM_GRAPHICS_CAMERAS_H_

