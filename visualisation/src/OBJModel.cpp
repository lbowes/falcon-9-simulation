#include "OBJModel.h"
//#include "Cameras.h"

#include "../3rd_party/imgui/imgui.h"

#include <bgfx/bgfx.h>
#include <bgfx/platform.h>
#include <bx/math.h>
#include <fstream>


namespace F9Sim {
namespace Graphics {


bgfx::ShaderHandle loadShader(const char* _name) {
    char* data = new char[2048];
    std::ifstream file;
    size_t fileSize;
    file.open(_name);
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
    bgfx::setName(handle, _name);
    return handle;
}

bgfx::VertexLayout PosColorVertex::ms_decl;

static PosColorVertex s_cubeVertices[] = {
    {0.5f, 0.5f, 0.0f, 0xff0000ff},
    {0.5f, -0.5f, 0.0f, 0xff0000ff},
    {-0.5f, -0.5f, 0.0f, 0xff00ff00},
    {-0.5f, 0.5f, 0.0f, 0xff00ff00}};

static const uint16_t s_cubeTriList[] = {
    0, 1, 3,
    1, 2, 3};


OBJModel::OBJModel(const char* filepath) {
    // Get the data from the obj file into the correct bgfx objects ready for rendering
    PosColorVertex::init();
    m_vbh = bgfx::createVertexBuffer(bgfx::makeRef(s_cubeVertices, sizeof(s_cubeVertices)), PosColorVertex::ms_decl);
    m_ibh = bgfx::createIndexBuffer(bgfx::makeRef(s_cubeTriList, sizeof(s_cubeTriList)));

    bgfx::ShaderHandle vsh = loadShader("resources/shaders/v_square.bin");
    bgfx::ShaderHandle fsh = loadShader("resources/shaders/f_square.bin");

    m_shader = bgfx::createProgram(vsh, fsh, true);
}


OBJModel::~OBJModel() {
    bgfx::destroy(m_vbh);
    bgfx::destroy(m_ibh);
    bgfx::destroy(m_shader);
}


void OBJModel::setTransform(const chrono::ChCoordsys<>& transform_world) {
    mTransform_world = transform_world;
}


void OBJModel::draw(float aspectRatio) const {
    // Get the active camera coordsys, and move floating point precision errors to objects far away
    //const chrono::ChCoordsys<> camCoordsys = Cameras_getActiveTransform();

    const bx::Vec3 at = {0.0f, 0.0f, 0.0f};
    const bx::Vec3 eye = {0.0f, 0.0f, 1.0f};

    // Set camera transform
    float view[16];
    bx::mtxLookAt(view, eye, at);

    float proj[16];
    bx::mtxProj(proj,
                60.0f,
                aspectRatio,
                0.1f, 100.0f,
                bgfx::getCaps()->homogeneousDepth);

    bgfx::setViewTransform(0, view, proj);
    //

    // Model transform
    static float rotation = 0.0f;
    ImGui::Begin("temp");
    ImGui::SliderAngle("rotation", &rotation, 0.0f, 360.0f);
    ImGui::End();

    float mtx[16];
    bx::mtxRotateY(mtx, rotation);

    // position x,y,z
    mtx[12] = 0.0f;
    mtx[13] = 0.0f;
    mtx[14] = 0.0f;
    bgfx::setTransform(mtx);
    //

    // Set uniforms for lighting
    // todo

    bgfx::setVertexBuffer(0, m_vbh);
    bgfx::setIndexBuffer(m_ibh);

    uint64_t _state =
        0 |
        BGFX_STATE_WRITE_RGB |
        BGFX_STATE_WRITE_A |
        BGFX_STATE_WRITE_Z |
        BGFX_STATE_DEPTH_TEST_LESS |
        BGFX_STATE_MSAA;

    bgfx::setState(_state);
    bgfx::submit(0, m_shader);
}


} // namespace Graphics
} // namespace F9Sim
