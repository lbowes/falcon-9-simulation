#include "CameraSystem.h"

#include <bgfx/bgfx.h>
#include <memory>
#include <string>
#include <unordered_map>


namespace F9Sim {
namespace Graphics {


CameraSystem::CameraSystem() :
    m_eye({0.0f, 0.0f, 0.0f}),
    m_activeCamera(nullptr) {

    registerCam(m_defaultCamera, "default_camera");
    bind("default_camera");
}


CameraSystem& CameraSystem::getInstance() {
    static CameraSystem instance;
    return instance;
}


chrono::Vector CameraSystem::getActivePos() const {
    return m_activeCamera->position;
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

    m_activeCamera = it->second;
    return true;
}


void CameraSystem::setViewTransform(float aspectRatio) {
    const bx::Vec3 at = bx::Vec3(
        m_activeCamera->lookAt.x(),
        m_activeCamera->lookAt.y(),
        m_activeCamera->lookAt.z());

    const bx::Vec3 up = bx::Vec3(
        m_activeCamera->up.x(),
        m_activeCamera->up.y(),
        m_activeCamera->up.z());

    float view[16];
    bx::mtxLookAt(view, m_eye, at, up, bx::Handness::Right);

    float proj[16];
    bx::mtxProj(
        proj,
        m_activeCamera->verticalFOV,
        aspectRatio,
        m_activeCamera->near,
        m_activeCamera->far,
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
