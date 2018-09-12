#ifndef VISUALISATION_H
#define VISUALISATION_H
#pragma once

#include "Graphics/Simulation_model/SimulationModelLayer.h"
#include "Graphics/Overlay/Overlay2D.h"
#include "Physics/SimState.h"

#include <GraphicsFramework/Application.h>
#include <fstream>
#include <sstream>
#include <map>

class Visualisation : public GF::Application {
private:
	const std::string mDataFilePath = "";
	std::unique_ptr<Graphics::Overlay2D> m2DOverlay;
	std::unique_ptr<Graphics::SimulationModelLayer> mSimulationModelLayer;
	const std::map<const double, const Physics::SimState>& mStateHistoryHandle;

	float 
		mSimTime_s = 0.0,
		mPlaybackSpeed = 1.0;

public:
	Visualisation(const std::map<const double, const Physics::SimState>& stateHistoryHandle);
	~Visualisation() = default;

private:
	virtual void onLoad();
	virtual void onInputCheck();
	virtual void onUpdate();
	virtual void onRender();

};

#endif
