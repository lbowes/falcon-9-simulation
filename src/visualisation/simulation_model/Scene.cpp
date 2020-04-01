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
    using namespace chrono;

    // Meshes
    mSceneNodes["Merlin1D_0"].transform_world = ChCoordsys<>(ChVector<>(0.0f, 0.6f, 0.0f));
    mSceneNodes["Merlin1D_1"].transform_world = ChCoordsys<>(ChVector<>(1.0f, 0.6f, 0.0f));
    mSceneNodes["someExampleLight"].transform_world = ChCoordsys<>(ChVector<>(50.0f, 0.0f, 0.0f));
    mSceneNodes["anotherExampleLight"].transform_world = ChCoordsys<>(ChVector<>(-50.0f, 0.0f, 50.0f));
}


void Scene::offsetAllNodePositionsBy(chrono::ChVector<> offset) {
    // Applies an offset to all model positions to eliminate the problem of floating point errors at large distances.
    for(auto& sceneNode : mSceneNodes) {
        chrono::ChCoordsys<> afterCamOffset = sceneNode.second.transform_world;
        afterCamOffset.pos += offset;

        chrono::irrlicht::ChIrrTools::alignIrrlichtNodeToChronoCsys(sceneNode.second.node, afterCamOffset);
    }
}


void Scene::addAllMeshes() {
    const std::string& modelDir = "../res/obj/";
    addMesh("Merlin1D_0", modelDir + "Merlin1D.obj");
    addMesh("Merlin1D_1", modelDir + "Merlin1D.obj");
    // TODO: Add all simulation meshes here
}


void Scene::addAllLights() {
    mSceneMgr.setAmbientLight(irr::video::SColor(0, 150, 150, 150));

    addLight(
        "someExampleLight",
        mSceneMgr.addLightSceneNode(0, irr::core::vector3df(0, 0, 0), irr::video::SColorf(1.0f, 0.0f, 0.0f)));

    addLight(
        "anotherExampleLight",
        mSceneMgr.addLightSceneNode(0, irr::core::vector3df(0, 0, 0), irr::video::SColorf(1.0f, 1.0f, 0.8f)));
}


void Scene::addMesh(const std::string& name, const std::string& filePath) {
    irr::scene::IMesh* mesh = mSceneMgr.getMesh(filePath.c_str());
    mSceneNodes[name] = {mSceneMgr.addMeshSceneNode(mesh), chrono::ChCoordsys<>()};
    // mSceneNodes[name].node->setMaterialFlag(irr::video::EMF_WIREFRAME, true);
    mSceneNodes[name].node->setMaterialFlag(irr::video::EMF_BACK_FACE_CULLING, false);
}


void Scene::addLight(const std::string& name, irr::scene::ILightSceneNode* light) {
    mSceneNodes[name] = {light, chrono::ChCoordsys<>()};
}
