#include "Falcon9/Falcon9_Vehicle.h"


namespace Hardware {


Falcon9_Vehicle::Falcon9_Vehicle(chrono::ChSystemNSC& system) :
    mSystemHandle(system) {

    // temp
    // Shared material
    auto mat = std::make_shared<chrono::ChMaterialSurfaceNSC>();
    mat->SetFriction(0.4f);
    mat->SetRestitution(0.1f);

    // Cube
    {
        mCube = std::make_shared<chrono::ChBodyAuxRef>();
        mSystemHandle.AddBody(mCube);

        mCube->SetNameString("unit_cube");
        const double mass = 10.0;
        mCube->SetMass(mass);
        mCube->SetBodyFixed(false);
        mCube->SetInertia(1.0 / 6.0 * mass);
        auto cubeCollisionModel = mCube->GetCollisionModel();
        cubeCollisionModel->ClearModel();
        cubeCollisionModel->AddBox(1.0f, 1.0f, 1.0f);
        mCube->SetCollide(true);

        const chrono::ChVector<> position = {0.0f, 10.0f, 0.0f};
        const chrono::ChQuaternion<> orientation = chrono::Q_from_AngX(10.0);
        const chrono::ChFrame<> frame = chrono::ChFrame<>(position, orientation);
        mCube->SetFrame_REF_to_abs(frame);
    }

    // Floor plane
    {
        mFloorPlane = std::make_unique<chrono::ChBody>();
        mSystemHandle.AddBody(mFloorPlane);
        mFloorPlane->SetBodyFixed(true);
        auto planeCollisionModel = mFloorPlane->GetCollisionModel();
        planeCollisionModel->ClearModel();
        planeCollisionModel->AddBox(20.0f, 1.0f, 20.0f);
        mFloorPlane->SetCollide(true);
    }
    //
}


SensorReadings Falcon9_Vehicle::readSensors() const {
    SensorReadings readings;

    // todo

    return readings;
}


void Falcon9_Vehicle::setControlProfile(const ControlProfile& p) {
    // todo
    // Responsible for setting the _target_ state of all subsystems (they then each respond to try to reach this target
    // state).
}


void Falcon9_Vehicle::saveSnapshotTo(nlohmann::json& snapshot) const {
    // todo: In future this will call the saveSnapshotTo methods of subsystems

    const chrono::ChVector<> position = mCube->GetPos();
    const chrono::ChQuaternion<> orientation = mCube->GetRot();
    snapshot["position_world"] = {position.x(), position.y(), position.z()};
    snapshot["orientation_world"] = {orientation.e0(), orientation.e1(), orientation.e2(), orientation.e3()};
}


} // namespace Hardware
