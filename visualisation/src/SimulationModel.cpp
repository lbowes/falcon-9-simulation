#include "SimulationModel.h"

#include <chrono_irrlicht/ChIrrTools.h>


SimulationModel::SimulationModel(irr::scene::ISceneManager& sceneMgr, irr::video::IVideoDriver& vidDriver) :
    mSceneMgr(sceneMgr) {

    addAllMeshes();
    addAllLights();
    addSky(vidDriver);
}


void SimulationModel::updateAllNodeTransforms() {
    // todo: This function should update the transforms of all scene nodes in the model. Where these updated transforms
    // will come from is not yet known.
    using namespace chrono;

    // Meshes
    mSceneNodes["Merlin1D"].transform_world = Coordsys(ChVector<>(0.0f, 0.6f, 0.0f));
    mSceneNodes["someExampleLight"].transform_world = Coordsys(ChVector<>(100.0f, 0.0f, 0.0f));
    mSceneNodes["anotherExampleLight"].transform_world = Coordsys(ChVector<>(-90.0f, 0.0f, 120.0f));
}


void SimulationModel::offsetAllNodePositionsBy(chrono::ChVector<> offset) {
    // Applies an offset to all model positions to eliminate the problem of floating point errors at large distances.
    for(auto& sceneNode : mSceneNodes) {
        chrono::Coordsys afterCamOffset = sceneNode.second.transform_world;
        afterCamOffset.pos += offset;

        chrono::irrlicht::ChIrrTools::alignIrrlichtNodeToChronoCsys(sceneNode.second.node, afterCamOffset);
    }
}


void SimulationModel::addSky(irr::video::IVideoDriver& vidDriver) {
    vidDriver.setTextureCreationFlag(irr::video::ETCF_CREATE_MIP_MAPS, false);

    irr::scene::ISceneNode* skybox = mSceneMgr.addSkyBoxSceneNode(
        vidDriver.getTexture("../res/textures/sky_up.jpg"),
        vidDriver.getTexture("../res/textures/sky_down.jpg"),
        vidDriver.getTexture("../res/textures/sky_lf.jpg"),
        vidDriver.getTexture("../res/textures/sky_lf.jpg"),
        vidDriver.getTexture("../res/textures/sky_lf.jpg"),
        vidDriver.getTexture("../res/textures/sky_lf.jpg"));

    vidDriver.setTextureCreationFlag(irr::video::ETCF_CREATE_MIP_MAPS, true);
}


void SimulationModel::addAllMeshes() {
    const std::string& modelDir = "../res/obj/";

    addMesh("Merlin1D", modelDir + "Merlin1D.obj");
    // TODO: Add all simulation meshes here
}


void SimulationModel::addAllLights() {
    using namespace irr;

    mSceneMgr.setAmbientLight(video::SColor(255, 200, 200, 255));

    addLight(
        "someExampleLight",
        mSceneMgr.addLightSceneNode(0, core::vector3df(0, 0, 0), video::SColorf(1.0f, 1.0f, 1.0f)));

    addLight(
        "anotherExampleLight",
        mSceneMgr.addLightSceneNode(0, core::vector3df(0, 0, 0), video::SColorf(1.0f, 1.0f, 1.0f)));

    scene::ILightSceneNode* sun = mSceneMgr.addLightSceneNode(0, core::vector3df(0, 1000, 0), video::SColorf(1.0f, 0.0f, 0.0f));
    sun->setLightType(video::ELT_DIRECTIONAL);
    addLight("sun", sun);
}


void SimulationModel::addMesh(const std::string& name, const std::string& filePath) {
    irr::scene::IMesh* mesh = mSceneMgr.getMesh(filePath.c_str());
    mSceneNodes[name] = {mSceneMgr.addMeshSceneNode(mesh), chrono::Coordsys()};
    // mSceneNodes[name].node->setMaterialFlag(irr::video::EMF_WIREFRAME, true);
    mSceneNodes[name].node->setMaterialFlag(irr::video::EMF_BACK_FACE_CULLING, false);
}


void SimulationModel::addLight(const std::string& name, irr::scene::ISceneNode* light) {
    mSceneNodes[name] = {light, chrono::Coordsys()};
}
