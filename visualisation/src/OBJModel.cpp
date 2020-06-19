#include "OBJModel.h"
#include "Cameras.h"

#include "../3rd_party/imgui/imgui.h"

#include <bgfx/bgfx.h>
#include <bgfx/platform.h>
#include <bx/math.h>
#include <fstream>


namespace F9Sim {
namespace Graphics {


bgfx::ShaderHandle loadShader(const char* filename) {
    char* data = new char[2048];
    std::ifstream file;
    size_t fileSize;

    file.open(filename);

    if(file.is_open()) {
        file.seekg(0, std::ios::end);
        fileSize = file.tellg();
        file.seekg(0, std::ios::beg);
        file.read(data, fileSize);
        file.close();
    }

    const bgfx::Memory* mem = bgfx::copy(data, fileSize + 1);
    mem->data[mem->size - 1] = '\0';
    bgfx::ShaderHandle handle = bgfx::createShader(mem);
    bgfx::setName(handle, filename);

    return handle;
}


bgfx::VertexLayout PosColorVertex::ms_decl;


uint64_t OBJModel::s_renderState =
    0 |
    BGFX_STATE_WRITE_RGB |
    BGFX_STATE_WRITE_A |
    BGFX_STATE_WRITE_Z |
    BGFX_STATE_DEPTH_TEST_LESS |
    BGFX_STATE_MSAA;


OBJModel::OBJModel(const char* filepath) {
    PosColorVertex::init();

    // Get the data from the obj file into the correct bgfx objects ready for rendering
    // (this is just temporary data for testing camera movement)
    {
        static PosColorVertex s_squareVertices[] = {
            {0.5f, 0.5f, 0.0f, 0xff0000ff},
            {0.5f, -0.5f, 0.0f, 0xff0000ff},
            {-0.5f, -0.5f, 0.0f, 0xff00ff00},
            {-0.5f, 0.5f, 0.0f, 0xff00ff00}};

        m_vbh = bgfx::createVertexBuffer(bgfx::makeRef(s_squareVertices, sizeof(s_squareVertices)), PosColorVertex::ms_decl);

        static const uint16_t s_cubeTriList[] = {
            0, 1, 3,
            1, 2, 3};

        m_ibh = bgfx::createIndexBuffer(bgfx::makeRef(s_cubeTriList, sizeof(s_cubeTriList)));
    }

    // Set up shader
    bgfx::ShaderHandle vsh = loadShader("resources/shaders/v_square.bin");
    bgfx::ShaderHandle fsh = loadShader("resources/shaders/f_square.bin");
    m_shader = bgfx::createProgram(vsh, fsh, true);
}


OBJModel::~OBJModel() {
    bgfx::destroy(m_vbh);
    bgfx::destroy(m_ibh);
    bgfx::destroy(m_shader);
}


void OBJModel::setTransform(const chrono::ChCoordsys<>& transform) {
    m_transform = transform;
}


void OBJModel::draw() const {
    updateTransform();

    bgfx::setVertexBuffer(0, m_vbh);
    bgfx::setIndexBuffer(m_ibh);
    bgfx::setState(s_renderState);
    bgfx::submit(0, m_shader);
}


void OBJModel::updateTransform() const {
    // Rotation
    const chrono::Quaternion r = m_transform.rot;
    const bx::Quaternion orientation = {(float)r.e3(), (float)r.e0(), (float)r.e1(), (float)r.e2()};
    float rotation[16];
    bx::mtxQuat(rotation, orientation);

    // Translation
    const chrono::Vector camPos = Cameras_getActivePos();
    const chrono::Vector d = m_transform.pos - camPos;
    float translation[16];
    bx::mtxTranslate(translation, (float)d.x(), (float)d.y(), (float)d.z());

    // Combine rotation and translation
    float transform[16];
    bx::mtxMul(transform, rotation, translation);
    bx::mtxIdentity(transform);
    bgfx::setTransform(transform);
}


} // namespace Graphics
} // namespace F9Sim
