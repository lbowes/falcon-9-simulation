#ifndef F9SIM_GRAPHICS_SCENE_H_
#define F9SIM_GRAPHICS_SCENE_H_
#pragma once


#include "Mesh.h"
#include "MountedCamera.h"
#include "StateSnapshot.h"

#include <bgfx/bgfx.h>
#include <glm/vec3.hpp>
#include <memory>


namespace F9Sim {
namespace Graphics {


struct GridVertex {
    static bgfx::VertexLayout ms_layout;

    float m_x;
    float m_y;
    float m_z;
    uint32_t m_abgr;

    static void init() {
        ms_layout
            .begin()
            .add(bgfx::Attrib::Position, 3, bgfx::AttribType::Float)
            .add(bgfx::Attrib::Color0, 4, bgfx::AttribType::Uint8, true)
            .end();
    }
};


class CameraSystem;

class Scene {
private:
    static uint64_t s_gridRenderState;

    MountedCamera m_mountedCam;
    bgfx::VertexBufferHandle m_gridVBH;
    bgfx::IndexBufferHandle m_gridIBH;
    bgfx::ProgramHandle m_gridShader;

    std::unique_ptr<Mesh> m_cube1;
    std::unique_ptr<Mesh> m_cube2;

public:
    Scene(CameraSystem& camSys);
    ~Scene();

    void setState(StateSnapshot state);
    void drawFrom(glm::dvec3 camPos) const;

private:
    void loadGrid();
    void drawGrid(glm::dvec3 camPos) const;
    void drawState(glm::dvec3 camPos) const;
    void updateTransformRelativeTo(glm::dvec3 camPos) const;
};


} // namespace Graphics
} // namespace F9Sim


#endif // F9SIM_GRAPHICS_SCENE_H_

