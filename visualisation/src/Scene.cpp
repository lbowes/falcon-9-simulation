#include "Scene.h"


namespace F9Sim {
namespace Graphics {


Scene& Scene::getInstance() {
    static Scene instance;
    return instance;
}


void Scene::draw() const {
    // todo
}


} // namespace Graphics
} // namespace F9Sim
