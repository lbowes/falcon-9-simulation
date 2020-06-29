#include "Mesh.h"
#include "../3rd_party/imgui/imgui.h"
#include "Cameras.h"
#include "ShaderUtils.h"

#include <bgfx/bgfx.h>
#include <bgfx/platform.h>
#include <bx/math.h>


namespace F9Sim {
namespace Graphics {


bgfx::VertexLayout Vertex::ms_decl;


uint64_t Mesh::s_renderState =
    0 |
    BGFX_STATE_WRITE_RGB |
    BGFX_STATE_WRITE_A |
    BGFX_STATE_WRITE_Z |
    BGFX_STATE_DEPTH_TEST_LESS |
    BGFX_STATE_MSAA;


Mesh::Mesh(const char* filepath) {
    Vertex::init();

    // Get the data from the obj file into the correct bgfx objects ready for rendering
    // (this is just temporary data for testing camera movement)
    {
        static Vertex s_cubeVertices[] = {
            {-1.0f, 1.0f, 1.0f, 0xff000000},
            {1.0f, 1.0f, 1.0f, 0xff0000ff},
            {-1.0f, -1.0f, 1.0f, 0xff00ff00},
            {1.0f, -1.0f, 1.0f, 0xff00ffff},
            {-1.0f, 1.0f, -1.0f, 0xffff0000},
            {1.0f, 1.0f, -1.0f, 0xffff00ff},
            {-1.0f, -1.0f, -1.0f, 0xffffff00},
            {1.0f, -1.0f, -1.0f, 0xffffffff},
        };

        m_vbh = bgfx::createVertexBuffer(bgfx::makeRef(s_cubeVertices, sizeof(s_cubeVertices)), Vertex::ms_decl);

        static const uint16_t s_cubeTriList[] = {
            0,
            1,
            2, // 0
            1,
            3,
            2,
            4,
            6,
            5, // 2
            5,
            6,
            7,
            0,
            2,
            4, // 4
            4,
            2,
            6,
            1,
            5,
            3, // 6
            5,
            7,
            3,
            0,
            4,
            1, // 8
            4,
            5,
            1,
            2,
            3,
            6, // 10
            6,
            3,
            7,
        };

        m_ibh = bgfx::createIndexBuffer(bgfx::makeRef(s_cubeTriList, sizeof(s_cubeTriList)));
    }

    // Set up shader
    bgfx::ShaderHandle vsh = loadShader("resources/shaders/v_square.bin");
    bgfx::ShaderHandle fsh = loadShader("resources/shaders/f_square.bin");
    m_shader = bgfx::createProgram(vsh, fsh, true);
}


Mesh::~Mesh() {
    bgfx::destroy(m_vbh);
    bgfx::destroy(m_ibh);
    bgfx::destroy(m_shader);
}


void Mesh::setTransform(const chrono::ChCoordsys<>& transform) {
    m_transform = transform;
}


void Mesh::draw() const {
    updateTransform();

    bgfx::setVertexBuffer(0, m_vbh);
    bgfx::setIndexBuffer(m_ibh);
    bgfx::setState(s_renderState);
    bgfx::submit(0, m_shader);
}


void Mesh::updateTransform() const {
    // Rotation
    const chrono::Quaternion r = m_transform.rot;
    const bx::Quaternion orientation = {(float)r.e3(), (float)r.e0(), (float)r.e1(), (float)r.e2()};
    float rotation[16];
    bx::mtxQuat(rotation, orientation);

    // Translation
    const chrono::Vector camPos = CameraSystem::getInstance().getActivePos();
    const chrono::Vector d = m_transform.pos - camPos;
    float translation[16];
    bx::mtxTranslate(translation, (float)d.x(), (float)d.y(), (float)d.z());

    // Combine rotation and translation
    float transform[16];
    bx::mtxMul(transform, rotation, translation);
    bgfx::setTransform(transform);
}


} // namespace Graphics
} // namespace F9Sim
