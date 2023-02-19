#include "Falcon9/Falcon9_Vehicle.h"

#include <chrono/physics/ChBodyEasy.h>
#include <chrono/utils/ChUtilsGeometry.h>


namespace F9Sim {
namespace Physics {


Falcon9_Vehicle::Falcon9_Vehicle(chrono::ChSystemNSC& system) :
    m_systemHandle(system) {

    // temp
    // Shared material
    auto mat = std::make_shared<chrono::ChMaterialSurfaceNSC>();
    mat->SetFriction(0.4);
    mat->SetSpinningFriction(1.0);
    mat->SetCompliance(0.0);
    mat->SetComplianceT(0.0);
    mat->SetDampingF(0.2);
    mat->SetRestitution(0.3);

    // Cube
    {
        m_cube1 = std::make_shared<chrono::ChBodyAuxRef>();

        m_cube1->SetNameString("unit_cube_1");
        m_cube1->SetBodyFixed(false);

        // Mass/inertia
        const double mass = 10.0;
        m_cube1->SetMass(mass);
        m_cube1->SetInertiaXX(chrono::Vector(1 / 6.0f * mass, 1 / 6.0f * mass, 1 / 6.0f * mass));

        // Centre of mass
        m_cube1->SetFrame_COG_to_REF(chrono::ChFrame<>(chrono::ChVector(0.0, 0.5, 0.0)));

        // Collision features
        auto cubeCollisionModel = m_cube1->GetCollisionModel();
        cubeCollisionModel->ClearModel();
        cubeCollisionModel->AddBox(mat, 0.5, 0.5, 0.5, chrono::Vector(0.0, 0.5, 0.0));
        cubeCollisionModel->BuildModel();
        m_cube1->SetCollide(true);

        chrono::ChFrame<> frame;
        frame.SetPos({0.0, 1.0f, 0.0f});
        m_cube1->SetFrame_REF_to_abs(frame);

        m_cube1->SetWvel_par(chrono::Vector(0, chrono::CH_C_DEG_TO_RAD * 180.0, 0));

        m_systemHandle.AddBody(m_cube1);
    }

    // Cube 2
    {
        m_cube2 = std::make_shared<chrono::ChBodyAuxRef>();

        m_cube2->SetNameString("unit_cube_2");
        m_cube2->SetBodyFixed(true);

        const double mass = 10.0;
        m_cube2->SetMass(mass);
        m_cube2->SetInertiaXX(chrono::Vector(1 / 6.0f * mass, 1 / 6.0f * mass, 1 / 6.0f * mass));

        // Centre of mass
        m_cube2->SetFrame_COG_to_REF(chrono::ChFrame(chrono::Vector(0.0, 0.5, 0.0)));

        auto cubeCollisionModel = m_cube2->GetCollisionModel();
        cubeCollisionModel->ClearModel();
        cubeCollisionModel->AddBox(mat, 0.5, 0.5, 0.5, {0.0, 0.5, 0.0});
        cubeCollisionModel->BuildModel();
        m_cube2->SetCollide(true);

        chrono::ChFrame<> frame;
        frame.SetPos({0.0, 10.0f, -1.0f});
        m_cube2->SetFrame_REF_to_abs(frame);

        m_systemHandle.AddBody(m_cube2);
    }

    // Floor plane
    {
        m_floorPlane = std::make_unique<chrono::ChBodyAuxRef>();

        m_floorPlane->SetBodyFixed(true);

        // Centre of mass
        const chrono::ChFrame<> cog = chrono::ChFrame<>(chrono::ChVector(0.0, 0.5, 0.0));
        m_floorPlane->SetFrame_COG_to_REF(cog);

        auto planeCollisionModel = m_floorPlane->GetCollisionModel();
        planeCollisionModel->ClearModel();
        planeCollisionModel->AddBox(mat, 20.0, 0.5, 20.0, {0.0, 0.5, 0.0});
        planeCollisionModel->BuildModel();
        m_floorPlane->SetCollide(true);

        chrono::ChFrame<> frame;
        frame.SetPos({0.0, -1.0, 0.0});
        m_floorPlane->SetFrame_REF_to_abs(frame);

        m_systemHandle.AddBody(m_floorPlane);
    }
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

    const chrono::ChFrame<> cube1Frame = m_cube1->GetFrame_REF_to_abs();
    const chrono::Vector cube1Pos = cube1Frame.GetPos();
    const chrono::ChQuaternion<> cube1Orientation = cube1Frame.GetRot();
    nlohmann::json& cube1 = snapshot["cube1"];
    cube1["position_world"] = {cube1Pos.x(), cube1Pos.y(), cube1Pos.z()};
    cube1["orientation_world"] = {cube1Orientation.e0(), cube1Orientation.e1(), cube1Orientation.e2(), cube1Orientation.e3()};

    const chrono::ChFrame<> cube2Frame = m_cube2->GetFrame_REF_to_abs();
    const chrono::Vector cube2Pos = cube2Frame.GetPos();
    const chrono::ChQuaternion<> cube2Orientation = cube2Frame.GetRot();
    nlohmann::json& cube2 = snapshot["cube2"];
    cube2["position_world"] = {cube2Pos.x(), cube2Pos.y(), cube2Pos.z()};
    cube2["orientation_world"] = {cube2Orientation.e0(), cube2Orientation.e1(), cube2Orientation.e2(), cube2Orientation.e3()};
}


} // namespace Physics
} // namespace F9Sim
