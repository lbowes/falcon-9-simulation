#ifndef F9SIMULATION_H
#define F9SIMULATION_H
#pragma once

#include "Physics/Hardware/Falcon_9/Falcon9.h"
#include "Graphics/GraphicalShell.h"

#include <GraphicsFramework/Application.h>

class F9Simulation : public GF::Application {
private:
	Physics::Hardware::Falcon9 mFalcon9;
	std::unique_ptr<Graphics::GraphicalShell> mGraphics;
	
	float mSimulationSpeed = 1.0;

public:
	F9Simulation();
	~F9Simulation() = default;

private:
	virtual void onLoad();
	virtual void onInputCheck();
	virtual void onUpdate();
	virtual void onRender();

};

#endif
