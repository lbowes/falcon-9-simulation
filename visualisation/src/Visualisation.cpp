#include "Visualisation.h"
#include "../3rd_party/imgui/imgui.h"
#include "../3rd_party/imgui/imgui_impl_bgfx.h"
#include "../3rd_party/imgui/imgui_impl_glfw.h"
#include "CameraSystem.h"
#include "Input.h"
#include "Scene.h"

#include <bgfx/bgfx.h>
#include <bgfx/platform.h>
#include <bx/bx.h>
#include <bx/math.h>
#include <stdio.h>

#define GLFW_EXPOSE_NATIVE_X11
#include <GLFW/glfw3native.h>


namespace F9Sim {
namespace Graphics {


GLFWwindow* Visualisation::s_window = nullptr;
int Visualisation::s_width = 0;
int Visualisation::s_height = 0;
std::unique_ptr<Mesh> Visualisation::m_mesh;
std::unique_ptr<FPVCamera> Visualisation::s_fpvCam;


static void glfw_errorCallback(int error, const char* description) {
    fprintf(stderr, "GLFW error %d: %s\n", error, description);
}


Visualisation::Visualisation() {
    glfwSetErrorCallback(glfw_errorCallback);

    if(!glfwInit())
        printf("glfwInit() failed");

    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

    s_window = glfwCreateWindow(960, 540, "Simulation Visualiser", nullptr, nullptr);

    if(!s_window)
        printf("glfwCreateWindow() failed");

    bgfx::renderFrame();

    bgfx::Init init;
    init.platformData.ndt = glfwGetX11Display();
    init.platformData.nwh = (void*)(uintptr_t)glfwGetX11Window(s_window);
    glfwGetWindowSize(s_window, &s_width, &s_height);
    init.resolution.width = (uint32_t)s_width;
    init.resolution.height = (uint32_t)s_height;

    if(!bgfx::init(init))
        printf("bgfx::init() failed");

    bgfx::setViewClear(0, BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH);
    bgfx::setViewRect(0, 0, 0, bgfx::BackbufferRatio::Equal);
    bgfx::setDebug(BGFX_DEBUG_TEXT);

    // Setup imgui
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui_ImplGlfw_InitForOpenGL(s_window, true);
    ImGui_Implbgfx_Init(0);

    Input_init(*s_window);
    Input_hideMouseCursor();

    // temp
    m_mesh = std::make_unique<Mesh>("resources/obj/Merlin1D.obj");
    s_fpvCam = std::make_unique<FPVCamera>();
    CameraSystem::getInstance().bind("first_person_view");
    Scene& s = Scene::getInstance();
    //

    bgfx::touch(0);
}


Visualisation::~Visualisation() {
    ImGui_Implbgfx_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    bgfx::shutdown();
    glfwTerminate();
}


Visualisation& Visualisation::getInstance() {
    static Visualisation instance;
    return instance;
}


void Visualisation::run() {
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

        // todo: get the required input from the Input module into the FPVCamera's input struct here
        FPVCamera::Input input;
        input.mouseDelta = Input_getMouseDelta();
        input.move.forward = Input_isKeyDown(GLFW_KEY_E);
        input.move.backwards = Input_isKeyDown(GLFW_KEY_D);
        input.move.up = Input_isKeyDown(GLFW_KEY_SPACE);
        input.move.down = Input_isKeyDown(GLFW_KEY_LEFT_SHIFT);
        input.move.left = Input_isKeyDown(GLFW_KEY_S);
        input.move.right = Input_isKeyDown(GLFW_KEY_F);

        s_fpvCam->process(input, dt);
        s_fpvCam->update(dt);

        bgfx::touch(0);
        {
            const float aspectRatio = (float)s_width / (float)s_height;
            CameraSystem::getInstance().setViewTransform(aspectRatio);

            m_mesh->draw();
        }
        bgfx::frame();

        // Render ImGui on top of everything else
        ImGui::Render();

        dt = frameTime - lastFrameTime;
    }
}


} // namespace Graphics
} // namespace F9Sim

