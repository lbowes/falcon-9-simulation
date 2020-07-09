#include "Falcon9/Falcon9_Vehicle.h"


namespace F9Sim {
namespace Physics {


Falcon9_Vehicle::Falcon9_Vehicle(chrono::ChSystemNSC& system) :
    mSystemHandle(system) {

    // temp
    // Shared material
    auto mat = std::make_shared<chrono::ChMaterialSurfaceNSC>();
    mat->SetFriction(0.4f);
    mat->SetRestitution(0.1f);

    // Cube 1
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
        cubeCollisionModel->AddBox(mat, 1.0f, 1.0f, 1.0f);
        mCube->SetCollide(true);

        const chrono::ChVector<> position = {4.0f, 10.0f, 0.0f};
        const chrono::ChQuaternion<> orientation = chrono::Q_from_AngX(10.0);
        const chrono::ChFrame<> frame = chrono::ChFrame<>(position, orientation);
        mCube->SetFrame_REF_to_abs(frame);
    }

    // Cube 2
    {
        m_cube2 = std::make_shared<chrono::ChBodyAuxRef>();
        mSystemHandle.AddBody(m_cube2);

        m_cube2->SetNameString("unit_cube");
        const double mass = 10.0;
        m_cube2->SetMass(mass);
        m_cube2->SetBodyFixed(false);
        m_cube2->SetInertia(1.0 / 6.0 * mass);
        auto cubeCollisionModel = m_cube2->GetCollisionModel();
        cubeCollisionModel->ClearModel();
        cubeCollisionModel->AddBox(mat, 1.0f, 1.0f, 1.0f);
        m_cube2->SetCollide(true);

        const chrono::ChVector<> position = {4.0f, 10.0f, 0.0f};
        const chrono::ChQuaternion<> orientation = chrono::Q_from_Euler123({10.0, 40.0, 140.0});
        const chrono::ChFrame<> frame = chrono::ChFrame<>(position, orientation);
        m_cube2->SetFrame_REF_to_abs(frame);
    }

    // Floor plane
    {
        mFloorPlane = std::make_unique<chrono::ChBody>();
        mSystemHandle.AddBody(mFloorPlane);
        mFloorPlane->SetBodyFixed(true);
        auto planeCollisionModel = mFloorPlane->GetCollisionModel();
        planeCollisionModel->ClearModel();
        planeCollisionModel->AddBox(mat, 20.0f, 1.0f, 20.0f);
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

    const chrono::ChVector<> cube1Pos = mCube->GetPos();
    const chrono::ChQuaternion<> cube1Orientation = mCube->GetRot();
    nlohmann::json& cube1 = snapshot["cube1"];
    cube1["position_world"] = {cube1Pos.x(), cube1Pos.y(), cube1Pos.z()};
    cube1["orientation_world"] = {cube1Orientation.e0(), cube1Orientation.e1(), cube1Orientation.e2(), cube1Orientation.e3()};

    const chrono::ChVector<> cube2Pos = m_cube2->GetPos();
    const chrono::ChQuaternion<> cube2Orientation = m_cube2->GetRot();
    nlohmann::json& cube2 = snapshot["cube2"];
    cube2["position_world"] = {cube2Pos.x(), cube2Pos.y(), cube2Pos.z()};
    cube2["orientation_world"] = {cube2Orientation.e0(), cube2Orientation.e2(), cube2Orientation.e2(), cube2Orientation.e3()};
}


} // namespace Physics
} // namespace F9Sim
