#include "Application.h"
#include "Logger.h"
#include <time.h>

namespace GF {

Application::Application(const std::string& windowTitle, const std::string& iconPath, glm::ivec2 windowDimensions) :
    mWindow(windowTitle, windowDimensions) {
    init();

    if(iconPath != "")
        mWindow.setIcon(iconPath);
}

Application::~Application() {
    //IMGUI
    ImGui_ImplGlfwGL3_Shutdown();
    ImGui::DestroyContext();
}

void Application::run() {
    while(mRunning) {
        mRunning = !glfwWindowShouldClose(mWindow.getHandle());

        mCurrentTime = glfwGetTime();
        mFrameTime = mCurrentTime - mLastTime;
        mLastTime = mCurrentTime;
        mAccumulator += mFrameTime;

        //IMGUI
        ImGui_ImplGlfwGL3_NewFrame();

        onInputCheck();
        Input::update();

        while(mAccumulator >= mUpdateDelta) {
            mAccumulator -= mUpdateDelta;
            onUpdate();
        }

        render();
    }
}

void Application::render() {
    mWindow.clear();

    onRender();

    //IMGUI on top of everything else
    GLint mode[2];
    glGetIntegerv(GL_POLYGON_MODE, mode);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    ImGui::Render();
    ImGui_ImplGlfwGL3_RenderDrawData(ImGui::GetDrawData());
    glPolygonMode(GL_FRONT_AND_BACK, *mode);

    const double currentTime = glfwGetTime();
    //if(currentTime - mLastRenderTime > 1.0 / 75.0) {
    mWindow.update();
    mLastRenderTime = currentTime;
    //}
}

void Application::init() {
    //HWND consoleWindow = GetConsoleWindow();
    //SetWindowPos(consoleWindow, 0, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOACTIVATE);

    //Utils::Logger::open(/*"src/ErrorLog.txt"*/);

    srand((unsigned)time(NULL));

    //IMGUI
    ImGui::CreateContext();
    ImGui_ImplGlfwGL3_Init(mWindow.getHandle(), true);

    // This must be called after ImGui_ImplGlfwGL3_Init, because of the order in which callback functions should be set.
    // ImGui sets all callbacks for keys, characters, mouse etc
    // but we want our input handling system to overwrite the key and mouse callback, but not the character callback.
    Input::init(&mWindow);
}

} // namespace GF
