#include "Visualisation.h"
#include "../3rd_party/imgui/imgui.h"
#include "../3rd_party/imgui/imgui_impl_bgfx.h"
#include "../3rd_party/imgui/imgui_impl_glfw.h"
#include "Cameras.h"
#include "FPVCamera.h"
#include "Input.h"
#include "OBJModel.h"

#include <GLFW/glfw3.h>
#include <bgfx/bgfx.h>
#include <bgfx/platform.h>
#include <bx/bx.h>
#include <bx/math.h>
#include <memory>
#include <stdio.h>

#define GLFW_EXPOSE_NATIVE_X11
#include <GLFW/glfw3native.h>


namespace F9Sim {
namespace Graphics {


static GLFWwindow* s_window = nullptr;
static int s_width = 0;
static int s_height = 0;
static std::unique_ptr<OBJModel> m_model;
static std::unique_ptr<FPVCamera> s_fpvCam;

static void shutdown();
static void glfw_errorCallback(int error, const char* description);


bool Visualisation_init() {
    glfwSetErrorCallback(glfw_errorCallback);

    if(!glfwInit())
        return false;

    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

    s_window = glfwCreateWindow(960, 540, "Simulation Visualiser", nullptr, nullptr);

    if(!s_window)
        return false;

    bgfx::renderFrame();

    bgfx::Init init;
    init.platformData.ndt = glfwGetX11Display();
    init.platformData.nwh = (void*)(uintptr_t)glfwGetX11Window(s_window);
    glfwGetWindowSize(s_window, &s_width, &s_height);
    init.resolution.width = (uint32_t)s_width;
    init.resolution.height = (uint32_t)s_height;

    if(!bgfx::init(init))
        return false;

    bgfx::setViewClear(0, BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH);
    bgfx::setViewRect(0, 0, 0, bgfx::BackbufferRatio::Equal);
    bgfx::setDebug(BGFX_DEBUG_TEXT);

    // Setup imgui
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui_ImplGlfw_InitForOpenGL(s_window, true);
    ImGui_Implbgfx_Init(0);

    Cameras_init();
    Input_init(*s_window);
    Input_hideMouseCursor();

    // temp
    m_model = std::make_unique<OBJModel>("resources/obj/Merlin1D.obj");
    s_fpvCam = std::make_unique<FPVCamera>();
    //

    bgfx::touch(0);

    return true;
}


static void temp_updateCameras(float aspectRatio) {
}


void Visualisation_run() {
    double dt = 0.0;
    double frameTime = 0.0;
    double lastFrameTime = 0.0;

    while(!glfwWindowShouldClose(s_window)) {
        lastFrameTime = frameTime;
        frameTime = glfwGetTime();

        glfwPollEvents();
        Input_update();

        // Handle window resize.
        const int oldWidth = s_width;
        const int oldHeight = s_height;
        glfwGetWindowSize(s_window, &s_width, &s_height);
        if(s_width != oldWidth || s_height != oldHeight) {
            bgfx::reset((uint32_t)s_width, (uint32_t)s_height);
            bgfx::setViewRect(0, 0, 0, bgfx::BackbufferRatio::Equal);
        }

        ImGui_Implbgfx_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        s_fpvCam->handleInput(dt);
        s_fpvCam->update(dt);

        bgfx::touch(0);
        {
            const float aspectRatio = (float)s_width / (float)s_height;
            Cameras_setViewTransform(aspectRatio);

            m_model->draw();
        }
        bgfx::frame();

        // Render ImGui on top of everything else
        ImGui::Render();

        dt = frameTime - lastFrameTime;
    }
}


static void shutdown() {
    ImGui_Implbgfx_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    bgfx::shutdown();
    glfwTerminate();
}


static void glfw_errorCallback(int error, const char* description) {
    fprintf(stderr, "GLFW error %d: %s\n", error, description);
}


} // namespace Graphics
} // namespace F9Sim
