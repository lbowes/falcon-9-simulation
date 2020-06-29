#ifndef F9SIM_GRAPHICS_VISUALISATION_H_
#define F9SIM_GRAPHICS_VISUALISATION_H_
#pragma once


#include "FPVCamera.h"
#include "Mesh.h"

#include <GLFW/glfw3.h>
#include <memory>


namespace F9Sim {
namespace Graphics {


class Visualisation {
private:
    static GLFWwindow* s_window;
    static int s_width;
    static int s_height;

    static std::unique_ptr<Mesh> m_mesh;
    static std::unique_ptr<FPVCamera> s_fpvCam;

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
};


} // namespace Graphics
} // namespace F9Sim


#endif // F9SIM_GRAPHICS_VISUALISATION_H_

