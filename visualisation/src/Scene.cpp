#include "Scene.h"
#include "CameraSystem.h"
#include "ShaderUtils.h"

#include <bgfx/bgfx.h>
#include <bx/math.h>
#include <vector>


namespace F9Sim {
namespace Graphics {


bgfx::VertexLayout GridVertex::ms_layout;

uint64_t Scene::s_gridRenderState =
    0 |
    BGFX_STATE_WRITE_RGB |
    BGFX_STATE_WRITE_A |
    BGFX_STATE_WRITE_Z |
    BGFX_STATE_DEPTH_TEST_LESS |
    BGFX_STATE_MSAA |
    BGFX_STATE_PT_LINES;


Scene::Scene(CameraSystem& camSys) :
    m_mountedCam(camSys) {

    loadGrid();

    m_mesh = std::make_unique<Mesh>("resources/obj/Merlin1D.obj");
}


Scene::~Scene() {
    bgfx::destroy(m_gridVBH);
    bgfx::destroy(m_gridIBH);
    bgfx::destroy(m_gridShader);
}


void Scene::loadGrid() {
    // load all resources required by the grid
    GridVertex::init();

    // Generate grid buffer data
    const int halfExtent = 10;
    uint16_t vertexCounter = 0;

    static const uint16_t numVertices = 8 * halfExtent + 4;
    static GridVertex gridVertices[numVertices];
    static uint16_t gridIndices[numVertices];
    const uint32_t col_abgr = 0xff666666;

    // Horizontal grid lines
    for(int x = -halfExtent; x <= halfExtent; x++) {
        GridVertex lineStart = {(float)x, 0.0f, -(float)halfExtent, col_abgr};
        gridVertices[vertexCounter] = lineStart;
        gridIndices[vertexCounter] = vertexCounter;
        vertexCounter++;

        GridVertex lineEnd = {lineStart.m_x, lineStart.m_y, (float)halfExtent, col_abgr};
        gridVertices[vertexCounter] = lineEnd;
        gridIndices[vertexCounter] = vertexCounter;
        vertexCounter++;
    }

    // Vertical grid lines
    for(int z = -halfExtent; z <= halfExtent; z++) {
        GridVertex lineStart = {-(float)halfExtent, 0.0f, (float)z, col_abgr};
        gridVertices[vertexCounter] = lineStart;
        gridIndices[vertexCounter] = vertexCounter;
        vertexCounter++;

        // End vertex
        GridVertex lineEnd = {(float)halfExtent, lineStart.m_y, lineStart.m_z, col_abgr};
        gridVertices[vertexCounter] = lineEnd;
        gridIndices[vertexCounter] = vertexCounter;
        vertexCounter++;
    }

    m_gridVBH = bgfx::createVertexBuffer(bgfx::makeRef(gridVertices, sizeof(gridVertices)), GridVertex::ms_layout);
    m_gridIBH = bgfx::createIndexBuffer(bgfx::makeRef(gridIndices, sizeof(gridIndices)));

    // Set up shader
    bgfx::ShaderHandle vsh = loadShader("resources/shaders/v_square.bin");
    bgfx::ShaderHandle fsh = loadShader("resources/shaders/f_square.bin");
    m_gridShader = bgfx::createProgram(vsh, fsh, true);
}


void Scene::setState(StateSnapshot state) {
    const Transform cubeTransform = {state.cube1.position, state.cube1.orientation};

    m_mesh->setTransform(cubeTransform);
    m_mountedCam.setParentTransform(cubeTransform);
}


void Scene::drawFrom(glm::dvec3 camPos) const {
    drawGrid(camPos);
    drawState(camPos);
}


void Scene::drawGrid(glm::dvec3 camPos) const {
    updateTransformRelativeTo(camPos);

    bgfx::setVertexBuffer(0, m_gridVBH);
    bgfx::setIndexBuffer(m_gridIBH);
    bgfx::setState(s_gridRenderState);
    bgfx::submit(0, m_gridShader);
}


void Scene::drawState(glm::dvec3 camPos) const {
    m_mesh->draw(camPos);
}


void Scene::updateTransformRelativeTo(glm::dvec3 camPos) const {
    float translation[16];
    const glm::dvec3 d = -camPos;
    bx::mtxTranslate(translation, (float)d.x, (float)d.y, (float)d.z);

    bgfx::setTransform(translation);
}


} // namespace Graphics
} // namespace F9Sim
