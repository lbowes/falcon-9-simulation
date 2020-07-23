#include "Visualisation.h"
#include "../3rd_party/imgui/imgui.h"
#include "../3rd_party/imgui/imgui_impl_bgfx.h"
#include "../3rd_party/imgui/imgui_impl_glfw.h"
#include "Input.h"

#include <GLFW/glfw3.h>
#include <bgfx/bgfx.h>
#include <bgfx/platform.h>
#include <bx/bx.h>
#include <bx/math.h>
#include <fstream>
#include <glm/geometric.hpp>
#include <sstream>
#include <stdio.h>

#define GLFW_EXPOSE_NATIVE_X11
#include <GLFW/glfw3native.h>


namespace F9Sim {
namespace Graphics {


static void glfw_errorCallback(int error, const char* description) {
    fprintf(stderr, "GLFW error %d: %s\n", error, description);
}


Visualisation::Visualisation() :
    m_window(nullptr),
    m_width(0),
    m_height(0),
    m_fpvCamFocused(false) {

    glfwSetErrorCallback(glfw_errorCallback);

    if(!glfwInit())
        printf("glfwInit() failed");

    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

    m_window = glfwCreateWindow(960, 540, "Simulation Visualiser", nullptr, nullptr);

    if(!m_window)
        printf("glfwCreateWindow() failed");

    bgfx::renderFrame();

    bgfx::Init init;
    init.platformData.ndt = glfwGetX11Display();
    init.platformData.nwh = (void*)(uintptr_t)glfwGetX11Window(m_window);
    glfwGetWindowSize(m_window, &m_width, &m_height);
    init.resolution.width = (uint32_t)m_width;
    init.resolution.height = (uint32_t)m_height;

    if(!bgfx::init(init))
        printf("bgfx::init() failed");

    bgfx::setViewClear(0, BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH);
    bgfx::setViewRect(0, 0, 0, bgfx::BackbufferRatio::Equal);
    bgfx::setDebug(BGFX_DEBUG_TEXT);

    // Setup imgui
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui_ImplGlfw_InitForOpenGL(m_window, true);
    ImGui_Implbgfx_Init(0);

    Input::init(*m_window);

    std::ifstream simDataFile("../simulation/output.json");
    std::stringstream buffer;
    buffer << simDataFile.rdbuf();
    const nlohmann::json simData = nlohmann::json::parse(buffer.str());
    m_animation = std::make_unique<Animation>(simData["history"], 0.2);

    m_scene = std::make_unique<Scene>();

    // Static camera
    m_staticCamera.aspectRatio = 1.0f;
    m_staticCamera.near = 0.1f;
    m_staticCamera.far = 100.0f;
    m_staticCamera.verticalFOV = 45.0f;
    m_staticCamera.position = {10.0f, 10.0f, 10.0f};
    m_staticCamera.up = {0.0f, 1.0f, 0.0f};
    m_staticCamera.lookAt = -glm::normalize(m_staticCamera.position);
    m_camSystem.registerCam(m_staticCamera, "static");

    // First-person-view camera
    m_fpvCam = std::make_unique<FPVCamera>(m_camSystem);
    m_camSystem.bind("first_person_view");

    bgfx::touch(0);
}


Visualisation::~Visualisation() {
    ImGui_Implbgfx_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    bgfx::shutdown();
    glfwTerminate();
}


void Visualisation::run() {
    double dt = 0.0;
    double frameTime = 0.0;
    double lastFrameTime = 0.0;

    while(!glfwWindowShouldClose(m_window)) {
        lastFrameTime = frameTime;
        frameTime = glfwGetTime();

        glfwPollEvents();

        Input::update();

        // Handle window resize.
        const int oldWidth = m_width;
        const int oldHeight = m_height;
        glfwGetWindowSize(m_window, &m_width, &m_height);
        if(m_width != oldWidth || m_height != oldHeight) {
            bgfx::reset((uint32_t)m_width, (uint32_t)m_height);
            bgfx::setViewRect(0, 0, 0, bgfx::BackbufferRatio::Equal);
        }

        ImGui_Implbgfx_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        ImGuiIO& io = ImGui::GetIO();
        const std::string boundCam = m_camSystem.getActiveName();
        if(Input::isMouseButtonReleased(GLFW_MOUSE_BUTTON_LEFT) && !io.WantCaptureMouse && boundCam == "first_person_view")
            m_fpvCamFocused = true;

        if(Input::isMouseButtonReleased(GLFW_MOUSE_BUTTON_RIGHT))
            m_fpvCamFocused = false;

        // todo: get the required input from the Input module into the FPVCamera's input struct here
        FPVCamera::Input input;

        if(m_fpvCamFocused) {
            glm::ivec2 mouseDelta = Input::getMouseDelta();
            input.pitchDelta_degs = -mouseDelta.y * 0.05f;
            input.yawDelta_degs = mouseDelta.x * 0.05f;
            input.move.forward = Input::isKeyDown(GLFW_KEY_E);
            input.move.backwards = Input::isKeyDown(GLFW_KEY_D);
            input.move.up = Input::isKeyDown(GLFW_KEY_SPACE);
            input.move.down = Input::isKeyDown(GLFW_KEY_LEFT_SHIFT);
            input.move.left = Input::isKeyDown(GLFW_KEY_S);
            input.move.right = Input::isKeyDown(GLFW_KEY_F);
        }

        Input::setCursorVisible(!m_fpvCamFocused);

        m_fpvCam->process(input);
        m_fpvCam->update(dt);

        bgfx::touch(0);
        {
            const float aspectRatio = (float)m_width / (float)m_height;
            m_camSystem.setViewTransform(aspectRatio);

            const StateSnapshot& s = m_animation->stateAt(1.0);
            m_scene->setState(s);

            const glm::dvec3 activeCamPos = m_camSystem.getActivePos();
            m_scene->drawFrom(activeCamPos);
        }
        bgfx::frame();

        // Render ImGui on top of everything else
        showCameraSelectPanel();
        ImGui::Render();

        dt = frameTime - lastFrameTime;
    }
}


void Visualisation::showCameraSelectPanel() {
    ImGui::Begin("Cameras");

    const std::vector<std::string> items = m_camSystem.getRegisteredCamNames();
    static int itemIdx = 0;
    const std::string label = items[itemIdx];

    if(ImGui::BeginCombo("active camera", label.c_str())) {
        for(int n = 0; n < items.size(); n++) {
            const bool isSelected = (itemIdx == n);
            if(ImGui::Selectable(items[n].c_str(), isSelected))
                itemIdx = n;

            if(isSelected)
                ImGui::SetItemDefaultFocus();
        }
        ImGui::EndCombo();
    };

    m_camSystem.bind(items[itemIdx]);

    ImGui::End();
}


} // namespace Graphics
} // namespace F9Sim
