#ifndef F9SIM_GRAPHICS_VISUALISATION_H_
#define F9SIM_GRAPHICS_VISUALISATION_H_
#pragma once


#include "CameraSystem.h"
#include "FPVCamera.h"
#include "Scene.h"

#include <memory>


typedef struct GLFWwindow GLFWwindow;


namespace F9Sim {
namespace Graphics {


class Visualisation {
private:
    GLFWwindow* m_window;
    int m_width;
    int m_height;

    std::unique_ptr<FPVCamera> m_fpvCam;
    bool m_fpvCamFocused;
    CameraSystem m_camSystem;
    CameraBaseState m_staticCamera;
    std::unique_ptr<Scene> m_scene;

public:
    Visualisation();
    ~Visualisation();

    void run();

private:
    void showCameraSelectPanel();
};


} // namespace Graphics
} // namespace F9Sim


#endif // F9SIM_GRAPHICS_VISUALISATION_H_

