#ifndef VISUALISATION_H
#define VISUALISATION_H
#pragma once

#include "Graphics/Simulation_model/SimulationModelLayer.h"
#include "Physics/Hardware/Falcon_9/Falcon9.h"
#include "Graphics/Overlay/Overlay2D.h"
#include "Physics/DynamicSimState.h"

#include <GraphicsFramework/Application.h>
#include <fstream>
#include <sstream>
#include <map>
#include <algorithm>

class Visualisation : public GF::Application {
private:
	const std::string mDataFilePath = "";
	
	std::unique_ptr<Graphics::Overlay2D> m2DOverlay;
	
	std::unique_ptr<Graphics::SimulationModelLayer> mSimulationModelLayer;
	
	const std::map<const unsigned, const Physics::DSS>& mStateHistory;

	//This Falcon9 instance is updated using the dynamic simulation state history, rather than integration.
	Physics::Hardware::Falcon9 mDataSource;

	float 
		mSimTime_s = 0.0,
		mPlaybackSpeed = 1.0;

	const double mSnapshotInterval_s = 0;

public:
	Visualisation(const std::map<const unsigned, const Physics::DSS>& stateHistoryHandle, double snapshotInterval_s);
	~Visualisation() = default;

private:
	virtual void onLoad();
	virtual void onInputCheck();
	virtual void onUpdate();
	virtual void onRender();

};

#endif
