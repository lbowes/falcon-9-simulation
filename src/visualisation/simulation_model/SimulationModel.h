#ifndef SIMULATIONMODEL_H_
#define SIMULATIONMODEL_H_

#include <IMeshSceneNode.h>
#include <ISceneManager.h>
#include <chrono/core/ChCoordsys.h>
#include <chrono/core/ChVector.h>

#include <string>
#include <unordered_map>


class SimulationModel {
public:
    struct HighPrecisionNode {
        irr::scene::ISceneNode* node;
        chrono::Coordsys transform_world;
    };

private:
    irr::scene::ISceneManager& mSceneMgr;
    std::unordered_map<std::string, HighPrecisionNode> mSceneNodes;

public:
    SimulationModel(irr::scene::ISceneManager& sceneMgr, irr::video::IVideoDriver& vidDriver);
    ~SimulationModel() = default;

    void updateAllNodeTransforms();
    void offsetAllNodePositionsBy(chrono::ChVector<> offset);

private:
    void addSky(irr::video::IVideoDriver& vidDriver);
    void addAllMeshes();
    void addAllLights();
    void addMesh(const std::string& name, const std::string& filePath);
    void addLight(const std::string& name, irr::scene::ISceneNode* light);
};


#endif // SIMULATIONMODEL_H_

