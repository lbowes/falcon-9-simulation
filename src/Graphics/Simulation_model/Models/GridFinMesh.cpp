#include "GridFinMesh.h"
#include "Physics/Hardware/Falcon_9/Stage_1/Grid_fins/GridFin.h"

namespace Graphics {

	unsigned char GridFinMesh::mNumInstances = 0;

	GridFinMesh::GridFinMesh(const Physics::Hardware::GridFin& dataSource, GF::ResourceSet& resourceBucket, GF::Model3D& parentModel) :
		IStageComponentMesh(resourceBucket, parentModel),
		mDataSource(dataSource)
	{
		loadResources();
		mNumInstances++;
	}

	void GridFinMesh::loadResources() 
		//Responsible for adding whatever is needed to the mResourceBucket
	{
		GF::Graphics::Shader* shader = mResourceBucket.getResource<GF::Graphics::Shader>("bodyShader");

		//Hinge
		mHingeMesh = mResourceBucket.addOBJMesh("gridFinHinge" + std::to_string(mNumInstances), BOX_MODELS ? "res/models/GridFinHinge_Box.obj" : "res/models/GridFinHinge.obj", GL_TRIANGLES, nullptr, shader);
		mParentModel.addMesh(mHingeMesh);

		//Fin
		mFinMesh = mResourceBucket.addOBJMesh("gridFin" + std::to_string(mNumInstances), BOX_MODELS ? "res/models/GridFin_Box.obj" : "res/models/GridFin_JonRoss.obj", GL_TRIANGLES, nullptr, shader);
		mParentModel.addMesh(mFinMesh);
	}

	void GridFinMesh::updateResources(glm::mat4 stageModelTransform_OGL) {
		using namespace glm;
		
		//Hinge transform
		vec3 XZDirection_stage = rotate(dvec3(0.0, 0.0, -1.0), glm::radians(mDataSource.mClockingDegree_degs), glm::dvec3(0.0, 1.0, 0.0));
		
		mat4 hingeTransform = rotate(static_cast<float>(radians(mDataSource.mRollAngle)), XZDirection_stage);
		hingeTransform = rotate(hingeTransform, static_cast<float>(radians(mDataSource.mClockingDegree_degs)), { 0.0f, 1.0f, 0.0f });
		hingeTransform = stageModelTransform_OGL * mat4(mDataSource.mCompToStage.getLocalToParent_translation()) * hingeTransform;
		mHingeMesh->setModelTransform(hingeTransform);

		//Fin transform
		mFinMesh->setModelTransform(stageModelTransform_OGL * mat4(mDataSource.mCompToStage.getLocalToParent_total()));
	}

}