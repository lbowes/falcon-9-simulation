#include "Scene.h"

#include <chrono_irrlicht/ChIrrTools.h>


Scene::Scene(irr::scene::ISceneManager& sceneMgr) :
    mSceneMgr(sceneMgr) {

    addAllMeshes();
    addAllLights();
}


void Scene::updateAllNodeTransforms() {
    // todo: This function should update the transforms of all scene nodes in the model. Where these updated transforms
    // will come from is not yet known.

    // Meshes
    mSceneNodes["Merlin1D_0"].transform_world = chrono::ChCoordsys<>();

    // Lights
    // todo
}


void Scene::applyCameraPosOffset(chrono::ChVector<> camPos) {
    // Applies an offset to all model positions to eliminate the problem of floating point errors at large distances.
    for(auto& sceneNode : mSceneNodes) {
        chrono::ChCoordsys<> afterCamOffset = sceneNode.second.transform_world;
        afterCamOffset.pos -= camPos;

        chrono::irrlicht::ChIrrTools::alignIrrlichtNodeToChronoCsys(sceneNode.second.internalNode, afterCamOffset);
    }
}


void Scene::addAllMeshes() {
    const std::string& modelDir = "../res/obj/";
    addMesh("Merlin1D_0", modelDir + "Merlin1D.obj");
    // TODO: Add all simulation meshes here
}


void Scene::addAllLights() {
    // todo
}


void Scene::addMesh(const std::string& name, const std::string& filePath) {
    irr::scene::IMesh* mesh = mSceneMgr.getMesh(filePath.c_str());
    mSceneNodes[name] = {mSceneMgr.addMeshSceneNode(mesh), chrono::ChCoordsys<>()};
}
