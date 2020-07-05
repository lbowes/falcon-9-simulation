#ifndef F9SIM_GRAPHICS_CAMERAS_H_
#define F9SIM_GRAPHICS_CAMERAS_H_
#pragma once


#include "CameraBaseState.h"

#include <bx/math.h>
#include <glm/vec3.hpp>
#include <unordered_map>
#include <vector>


namespace F9Sim {
namespace Graphics {


class CameraSystem {
private:
    const bx::Vec3 m_eye;
    CameraBaseState const* m_activeCamera;
    CameraBaseState m_defaultCamera;
    std::unordered_map<std::string, const CameraBaseState*> m_cameraMap;

public:
    CameraSystem();
    ~CameraSystem() = default;

    glm::dvec3 getActivePos() const;
    bool registerCam(const CameraBaseState& cam, const std::string& name);
    bool bind(const std::string& name);
    void setViewTransform(float aspectRatio);
    std::vector<std::string> getRegisteredCamNames() const;
};


} // namespace Graphics
} // namespace F9Sim


#endif // F9SIM_GRAPHICS_CAMERAS_H_

