#include "LandingLegMesh.h"
#include "Physics/Hardware/Falcon_9/Stage_1/Landing_legs/LandingLeg.h"

namespace Graphics {

	LandingLegMesh::LandingLegMesh(const Physics::Hardware::LandingLeg& dataSource) :
		mDataSource(dataSource)
	{
		mPistonCylinderTransformMap.resize(dataSource.mPiston->getCylinders().size());
		std::fill(mPistonCylinderTransformMap.begin(), mPistonCylinderTransformMap.end(), glm::mat4(1.0f));
	}

	void LandingLegMesh::updateTransform_OGL(glm::mat4 stageModelTransform_OGL) {
		using namespace glm;
		
		//A-frame transform
		mTransform_OGL = stageModelTransform_OGL * mat4(mDataSource.mCompToStage.getLocalToParent_total());

		//Telescoping piston cylinder transforms
		updateCylinderTransforms_OGL(length(mDataSource.mAlongPiston_stage3D), 0.0f/* mDataSource.mPiston->getAngleFromVertical_stage() */, stageModelTransform_OGL);
	}

	glm::mat4 LandingLegMesh::getCylinderTransform(unsigned char cylinderNumber) {
		if (cylinderNumber < mPistonCylinderTransformMap.size())
			return mPistonCylinderTransformMap[cylinderNumber];
		else
			return glm::mat4();
	}

	void LandingLegMesh::updateCylinderTransforms_OGL(float pistonLength, float angleDownFromVertical, glm::mat4 stageModelTransform_OGL) {
		using namespace glm;

		//Using the information about mParent, this function needs to update the OpenGL transform of the
		//cylinders along the telescoping piston.
		float
			alongPiston = 0.0,
			cylinderLength = 0.0;

		mat4 scaledModel;

		Physics::Hardware::TelescopingPiston* piston = mDataSource.mPiston.get();
		Physics::Hardware::PistonCylinder* cylinder = nullptr;

		for (unsigned char i = 0; i < mPistonCylinderTransformMap.size(); i++) {
			cylinder = piston->getCylinder(i);
			cylinderLength = cylinder->getLength();
			scaledModel = scale(vec3(cylinder->getWidthScale(), cylinderLength, cylinder->getWidthScale()));

			if ((i + 1) * cylinderLength < pistonLength)
				alongPiston = i * cylinderLength;
			else
				alongPiston = (i * cylinderLength) - (((i + 1) * cylinderLength) - pistonLength);

			//Rotate the cylinder around the stage
			mPistonCylinderTransformMap[i] = rotate(static_cast<float>(radians(piston->getClockingDegree_degs())), vec3(0.0f, 1.0f, 0.0f));

			//Rotate the cylinder downwards to line up with the piston centre line
			mPistonCylinderTransformMap[i] = rotate(mPistonCylinderTransformMap[i], static_cast<float>(radians(angleDownFromVertical)), vec3(-1.0f, 0.0f, 0.0f));

			//Slide the cylinder into the correct position along the piston centre line
			mPistonCylinderTransformMap[i] = translate(mPistonCylinderTransformMap[i], vec3(0.0f, alongPiston, 0.0f));

			//Translate the cylinder upwards to the mounting point on the stage
			mat4 translation = translate(vec3(piston->getMountPoint_stage()));
			mPistonCylinderTransformMap[i] = stageModelTransform_OGL * translation * mPistonCylinderTransformMap[i] * scaledModel;
		}
	}

}