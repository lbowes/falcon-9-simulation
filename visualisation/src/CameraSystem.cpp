#include "CameraSystem.h"

#include <bgfx/bgfx.h>
#include <memory>
#include <string>
#include <unordered_map>


namespace F9Sim {
namespace Graphics {


CameraSystem::CameraSystem() :
    m_eye({0.0f, 0.0f, 0.0f}),
    m_activeCam({.handle = nullptr, .name = ""}) {

    registerCam(m_defaultCamera, "default");
    bind("default");
}


glm::dvec3 CameraSystem::getActivePos() const {
    return m_activeCam.handle->position;
}


std::string CameraSystem::getActiveName() const {
    return m_activeCam.name;
}


bool CameraSystem::registerCam(const CameraBaseState& cam, const std::string& name) {
    const auto it = m_cameraMap.find(name);

    if(it != m_cameraMap.end()) {
        printf("A camera with name '%s' has already been registered.\n", name.c_str());
        return false;
    }

    m_cameraMap[name] = &cam;
    return true;
}


bool CameraSystem::bind(const std::string& name) {
    const auto it = m_cameraMap.find(name);

    if(it == m_cameraMap.end()) {
        printf("Camera '%s' has not been registered, so cannot be bound.\n", name.c_str());
        return false;
    }

    m_activeCam.handle = it->second;
    m_activeCam.name = name;

    return true;
}


void CameraSystem::setViewTransform(float aspectRatio) {
    CameraBaseState const* const cam = m_activeCam.handle;

    const bx::Vec3 at = bx::Vec3(cam->lookAt.x, cam->lookAt.y, cam->lookAt.z);
    const bx::Vec3 up = bx::Vec3(cam->up.x, cam->up.y, cam->up.z);

    float view[16];
    bx::mtxLookAt(view, m_eye, at, up, bx::Handness::Right);

    float proj[16];
    bx::mtxProj(
        proj,
        cam->verticalFOV,
        aspectRatio,
        cam->near,
        cam->far,
        bgfx::getCaps()->homogeneousDepth,
        bx::Handness::Right);

    bgfx::setViewTransform(0, view, proj);
}


std::vector<std::string> CameraSystem::getRegisteredCamNames() const {
    std::vector<std::string> output;

    for(auto it = m_cameraMap.begin(); it != m_cameraMap.end(); ++it)
        output.push_back(it->first);

    return output;
}


} // namespace Graphics
} // namespace F9Sim
