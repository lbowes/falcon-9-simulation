#include "System.h"

#include <chrono/physics/ChBodyAuxRef.h>


System::System() {
    setupInitialState();
}


void System::setupInitialState() {
    using namespace chrono;

    collision::ChCollisionModel::SetDefaultSuggestedEnvelope(1.0);
    collision::ChCollisionModel::SetDefaultSuggestedMargin(0.05);

    mSystem.SetSolverMaxIterations(200);
    mSystem.Set_G_acc(ChVector<>(0.0, -9.80665, 0.0));

    mCube = std::make_shared<ChBodyAuxRef>();
    mSystem.AddBody(mCube);

    mCube->SetNameString("unit_cube");
    const double mass = 10.0;
    mCube->SetMass(mass);
    mCube->SetBodyFixed(false);
    mCube->SetCollide(true);
    mCube->SetInertia(1.0 / 6.0 * mass);
}


void System::advanceTimeBy(double dt) {
    // TODO: Update all models

    mSystem.DoStepDynamics(dt);
    printf("position y: %f\n", mCube->GetFrame_REF_to_abs().GetPos().y());
}


void System::serialiseState() {
}
