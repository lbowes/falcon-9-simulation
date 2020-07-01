#ifndef F9SIM_GRAPHICS_SCENE_H_
#define F9SIM_GRAPHICS_SCENE_H_
#pragma once


namespace F9Sim {
namespace Graphics {


class Scene {
public:
    static Scene& getInstance();
    void draw() const;

    Scene(const Scene&) = delete;
    Scene(Scene&&) = delete;
    void operator=(const Scene&) = delete;
    void operator=(Scene&&) = delete;

private:
    Scene() = default;
    ~Scene() = default;
};


} // namespace Graphics
} // namespace F9Sim


#endif // F9SIM_GRAPHICS_SCENE_H_

