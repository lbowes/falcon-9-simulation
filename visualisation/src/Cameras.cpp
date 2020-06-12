#include "Cameras.h"
#include "CameraBaseState.h"

//temp
#include "FPVCamera.h"
#include "Input.h"
//

#include "../3rd_party/imgui/imgui.h"

#include <bgfx/bgfx.h>
#include <bx/math.h>
#include <memory>


namespace F9Sim {
namespace Graphics {


// Internally, the position of the camera is locked at the origin to eliminate floating point errors
static const bx::Vec3 s_eye = {0.0f, 0.0f, 0.0f};
static CameraBaseState s_activeCamera;
static std::unique_ptr<FPVCamera> s_fpvCam;


void Cameras_init() {
    s_fpvCam = std::make_unique<FPVCamera>();

    s_activeCamera.near = 0.1f;
    s_activeCamera.far = 100.0f;
    s_activeCamera.verticalFOV = 60.0f;
    s_activeCamera.position = {0.0, 0.0, -3.0};
    s_activeCamera.up = {0.0, 1.0, 0.0};
    s_activeCamera.lookAt = {0.0, 0.0, 1.0};
}


chrono::Vector Cameras_getActivePos() {
    return s_activeCamera.position;
}


void Cameras_setViewTransform(float aspectRatio, float dt) {
    // temp
    Input_hideMouseCursor();
    s_fpvCam->handleInput(dt);
    s_fpvCam->update(dt);
    s_activeCamera = s_fpvCam->m_camera;
    //

    const bx::Vec3 at = {
        s_activeCamera.lookAt.x(),
        s_activeCamera.lookAt.y(),
        s_activeCamera.lookAt.z()};

    const bx::Vec3 up = {
        s_activeCamera.up.x(),
        s_activeCamera.up.y(),
        s_activeCamera.up.z()};

    float view[16];
    bx::mtxLookAt(view, s_eye, at, up, bx::Handness::Right);

    float proj[16];
    bx::mtxProj(
        proj,
        s_activeCamera.verticalFOV,
        aspectRatio,
        s_activeCamera.near,
        s_activeCamera.far,
        bgfx::getCaps()->homogeneousDepth,
        bx::Handness::Right);

    bgfx::setViewTransform(0, view, proj);
}


} // namespace Graphics
} // namespace F9Sim
