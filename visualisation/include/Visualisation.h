#ifndef F9SIM_GRAPHICS_VISUALISATION_H_
#define F9SIM_GRAPHICS_VISUALISATION_H_
#pragma once


#include "CameraSystem.h"
#include "FPVCamera.h"
#include "Mesh.h"
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

    std::unique_ptr<Mesh> m_mesh;
    std::unique_ptr<FPVCamera> m_fpvCam;
    bool m_fpvCamFocused;
    CameraSystem m_camSystem;
    CameraBaseState m_staticCamera;
    std::unique_ptr<Scene> m_scene;

public:
    static Visualisation& getInstance();

    Visualisation(const Visualisation&) = delete;
    Visualisation(Visualisation&&) = delete;
    void operator=(const Visualisation&) = delete;
    void operator=(Visualisation&&) = delete;

    void run();

private:
    Visualisation();
    ~Visualisation();

    void showCameraSelectPanel();
};


} // namespace Graphics
} // namespace F9Sim


#endif // F9SIM_GRAPHICS_VISUALISATION_H_

