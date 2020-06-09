#ifndef APPLICATION_H
#define APPLICATION_H
#pragma once

#include "Input.h"
#include "ResourceSet.h"
#include "Vendor/ImGui/imgui.h"
#include "Vendor/ImGui/imgui_impl_glfw_gl3.h"
#include "Window.h"

namespace GF {
class Application {
protected:
    Window mWindow;
    ResourceSet mResourceSet;

    const double mUpdateDelta = 1.0 / 2000.0;

    double
        mCurrentTime = 0.0,
        mLastRenderTime = 0.0,
        mLastTime = 0.0,
        mFrameTime = 0.0,
        mAccumulator = 0.0;

    bool mRunning = true;

public:
    Application(const std::string& windowTitle = "Application", const std::string& iconPath = "", glm::ivec2 windowDimensions = glm::ivec2(-1, -1));
    virtual ~Application();

    void run();
    void setWindowPosition(glm::ivec2 newPosition) const { glfwSetWindowPos(mWindow.getHandle(), newPosition.x, newPosition.y); }

    virtual void onLoad() = 0;
    virtual void onInputCheck() = 0;
    virtual void onUpdate() = 0;
    virtual void onRender() = 0;

private:
    void init();
    void render();
};

} // namespace GF

#endif
