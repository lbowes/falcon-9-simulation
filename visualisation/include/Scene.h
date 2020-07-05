#ifndef F9SIM_GRAPHICS_SCENE_H_
#define F9SIM_GRAPHICS_SCENE_H_
#pragma once


#include <bgfx/bgfx.h>
#include <glm/vec3.hpp>


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


class Scene {
private:
    static uint64_t s_gridRenderState;

    bgfx::VertexBufferHandle m_gridVBH;
    bgfx::IndexBufferHandle m_gridIBH;
    bgfx::ProgramHandle m_gridShader;

public:
    Scene();
    ~Scene() = default;

    void draw(glm::dvec3 camPos) const;

private:
    void drawGrid(glm::dvec3 camPos) const;
    void updateTransformRelativeTo(glm::dvec3 camPos) const;
};


} // namespace Graphics
} // namespace F9Sim


#endif // F9SIM_GRAPHICS_SCENE_H_

