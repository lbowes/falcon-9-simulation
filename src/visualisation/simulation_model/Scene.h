#ifndef SCENE_H_
#define SCENE_H_

#include <IMeshSceneNode.h>
#include <ISceneManager.h>
#include <chrono/core/ChCoordsys.h>
#include <chrono/core/ChVector.h>
#include <string>
#include <unordered_map>


class Scene {
public:
    struct HighPrecisionNode {
        irr::scene::ISceneNode* node;
        chrono::ChCoordsys<> transform_world;
    };

private:
    irr::scene::ISceneManager& mSceneMgr;
    std::unordered_map<std::string, HighPrecisionNode> mSceneNodes;

public:
    Scene(irr::scene::ISceneManager& sceneMgr);
    ~Scene() = default;

    void updateAllNodeTransforms();
    void offsetAllNodePositionsBy(chrono::ChVector<> offset);

private:
    void addAllMeshes();
    void addAllLights();
    void addMesh(const std::string& name, const std::string& filePath);
    void addLight(const std::string& name, irr::scene::ILightSceneNode* light);
};

#endif // SCENE_H_

