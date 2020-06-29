#ifndef F9SIM_GRAPHICS_MESH_H_
#define F9SIM_GRAPHICS_MESH_H_
#pragma once


#include <bgfx/bgfx.h>
#include <chrono/core/ChCoordsys.h>


namespace F9Sim {
namespace Graphics {


struct Vertex {
    static bgfx::VertexLayout ms_decl;

    float m_x;
    float m_y;
    float m_z;
    uint32_t m_abgr;

    static void init() {
        ms_decl
            .begin()
            .add(bgfx::Attrib::Position, 3, bgfx::AttribType::Float)
            .add(bgfx::Attrib::Color0, 4, bgfx::AttribType::Uint8, true)
            .end();
    };
};


class Mesh {
private:
    static uint64_t s_renderState;

    // todo: static shader, bgfx mesh data
    chrono::ChCoordsys<> m_transform;

    bgfx::VertexBufferHandle m_vbh;
    bgfx::IndexBufferHandle m_ibh;
    bgfx::ProgramHandle m_shader;

public:
    Mesh(const char* filepath);
    ~Mesh();

    void setTransform(const chrono::ChCoordsys<>& transform_world);
    void draw() const;

private:
    void updateTransform() const;
};


} // namespace Graphics
} // namespace F9Sim


#endif // F9SIM_GRAPHICS_MESH_H_

