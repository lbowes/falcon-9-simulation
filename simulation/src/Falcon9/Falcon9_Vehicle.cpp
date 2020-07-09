#include "Falcon9/Falcon9_Vehicle.h"


namespace Hardware {


Falcon9_Vehicle::Falcon9_Vehicle(chrono::ChSystemNSC& system) :
    m_systemHandle(system) {

    // temp
    // Shared material
    auto mat = std::make_shared<chrono::ChMaterialSurfaceNSC>();
    mat->SetFriction(0.4f);
    mat->SetRestitution(0.1f);

    // Cube
    {
        m_cube = std::make_shared<chrono::ChBodyAuxRef>();
        m_systemHandle.AddBody(m_cube);

        m_cube->SetNameString("unit_cube");
        const double mass = 10.0;
        m_cube->SetMass(mass);
        m_cube->SetBodyFixed(false);
        m_cube->SetInertia(1.0 / 6.0 * mass);
        const chrono::ChFrame<> cogToRef(chrono::Vector(0.0, 0.5, 0.0));
        m_cube->SetFrame_COG_to_REF(cogToRef);
        auto cubeCollisionModel = m_cube->GetCollisionModel();
        cubeCollisionModel->ClearModel();
        cubeCollisionModel->AddBox(mat, 0.5, 0.5, 0.5);
        m_cube->SetCollide(true);

        const chrono::Vector position = {0.0, 10.0, 0.0};
        const chrono::Quaternion orientation = chrono::Q_from_Euler123({10.0, 70.0, 130.0});
        //const chrono::Quaternion orientation = chrono::Q_from_Euler123({0.0, 0.0, 0.0});
        chrono::ChFrame<> refToAbs = chrono::ChFrame<>(position, orientation);
        m_cube->SetFrame_REF_to_abs(refToAbs);
        m_cube->SetPos_dt(chrono::Vector(4.0, 0.0, 0.0));
    }

    // Floor plane
    {
        m_floorPlane = std::make_unique<chrono::ChBody>();
        m_systemHandle.AddBody(m_floorPlane);

        m_floorPlane->SetNameString("floor_plane");
        m_floorPlane->SetBodyFixed(true);
        auto planeCollisionModel = m_floorPlane->GetCollisionModel();
        planeCollisionModel->ClearModel();

        const double floorPlaneHeight = 1.0;
        const chrono::Vector floorPos = {0.0, -floorPlaneHeight * 0.5, 0.0};
        planeCollisionModel->AddBox(mat, 20.0, floorPlaneHeight * 0.5, 20.0, floorPos);
        m_floorPlane->SetCollide(true);
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

    const chrono::ChFrame<>& abs = m_cube->GetFrame_REF_to_abs();
    const chrono::ChVector<> position = abs.GetPos();
    const chrono::ChQuaternion<> orientation = abs.GetRot();
    snapshot["position_world"] = {position.x(), position.y(), position.z()};
    snapshot["orientation_world"] = {orientation.e0(), orientation.e1(), orientation.e2(), orientation.e3()};
}


} // namespace Hardware
