#include "GridFinMesh.h"
#include "Physics/Hardware/Falcon_9/Stage_1/Grid_fins/GridFin.h"

namespace Graphics {

	GridFinMesh::GridFinMesh(Physics::Hardware::GridFin& dataSource) : 
		mDataSource(dataSource)
	{ }

	void GridFinMesh::updateTransforms_OGL(glm::mat4 stageModelTransform_OGL) {
		using namespace glm;
	
		//Simply moves/rotates BOTH the hinge and fin with the stage
		mat4 mainTransform = stageModelTransform_OGL * translate(vec3(mDataSource.mCompToStage.toParentSpace()));
	
		//Hinge transform
		mHingeTransform_OGL = rotate(static_cast<float>(radians(mDataSource.mRollAngle)), vec3(rotate(glm::vec3(0.0f, 0.0f, 1.0f), static_cast<float>(mDataSource.mClockingDegree_degs), glm::vec3(0.0f, 1.0f, 0.0f))));
		mHingeTransform_OGL = rotate(mHingeTransform_OGL, static_cast<float>(radians(mDataSource.mClockingDegree_degs)), vec3(0.0f, 1.0f, 0.0f));
		mHingeTransform_OGL = mainTransform * mHingeTransform_OGL;
	
		//Fin transform
		mFinTransform_OGL = stageModelTransform_OGL * mat4(mDataSource.mCompToStage.getLocalToParent_total());
	}

}