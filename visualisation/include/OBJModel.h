#ifndef F9SIM_GRAPHICS_OBJMODEL_H_
#define F9SIM_GRAPHICS_OBJMODEL_H_
#pragma once


#include <bgfx/bgfx.h>
#include <chrono/core/ChCoordsys.h>


namespace F9Sim {
namespace Graphics {


struct PosColorVertex {
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

    static bgfx::VertexLayout ms_decl;
};


class OBJModel {
private:
    // todo: static shader, bgfx mesh data
    chrono::ChCoordsys<> mTransform_world;

    bgfx::VertexBufferHandle m_vbh;
    bgfx::IndexBufferHandle m_ibh;
    bgfx::ProgramHandle m_shader;

public:
    OBJModel(const char* filepath);
    ~OBJModel();

    void setTransform(const chrono::ChCoordsys<>& transform_world);
    void draw(float aspectRatio) const;
};


} // namespace Graphics
} // namespace F9Sim


#endif // F9SIM_GRAPHICS_OBJMODEL_H_

