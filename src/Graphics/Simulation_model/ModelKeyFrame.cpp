#include "ModelKeyFrame.h"

namespace Graphics {

    ModelKeyFrame::ModelKeyFrame(const std::string& csvRow) {
        std::vector<double> data;
        std::stringstream ss(csvRow);
        double d;

        while (ss >> d) {
            data.push_back(d);

            if (ss.peek() == ',')
                ss.ignore();
        }

        //todo: this is not the general solution, but it works for testing that the visualisation system should work
        //const chrono::ChVector<> tankPos_world = ;
        //const chrono::ChQuaternion<> tankRot_world = ;
        mS1RP1TankFrame_world = chrono::ChCoordsys<>({ data[0], data[1], data[2] }, { data[3], data[4], data[5], data[6] });
    }

    ModelKeyFrame ModelKeyFrame::lerp(const ModelKeyFrame& a, const ModelKeyFrame& b, float x) {
        ModelKeyFrame result;

        // Stage 1 RP1 tank
		chrono::Vector 
			pos_a = a.mS1RP1TankFrame_world.pos,
			pos_b = b.mS1RP1TankFrame_world.pos,
			lerpPos = pos_a + (pos_b - pos_a) * x;

		chrono::Quaternion
			rot_a = a.mS1RP1TankFrame_world.rot,
			rot_b = b.mS1RP1TankFrame_world.rot,
			lerpRot_world = rot_a + (rot_b - rot_a) * x;

        result.mS1RP1TankFrame_world = chrono::ChCoordsys<>(lerpPos, lerpRot_world);

		return result;
    }

}