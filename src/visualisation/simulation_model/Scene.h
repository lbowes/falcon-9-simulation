#ifndef SCENE_H_
#define SCENE_H_

#include "HighPrecisionSceneNode.h"

#include <IMeshSceneNode.h>
#include <ISceneManager.h>
#include <chrono/core/ChCoordsys.h>
#include <chrono/core/ChVector.h>
#include <string>
#include <unordered_map>


class Scene {
private:
    irr::scene::ISceneManager& mSceneMgr;
    std::unordered_map<std::string, HighPrecisionSceneNode> mSceneNodes;

public:
    Scene(irr::scene::ISceneManager& sceneMgr);
    ~Scene() = default;

    void updateAllNodeTransforms();
    void applyCameraPosOffset(chrono::ChVector<> camPos);

private:
    void addAllMeshes();
    void addAllLights();
    void addMesh(const std::string& name, const std::string& filePath);
};

#endif // SCENE_H_

