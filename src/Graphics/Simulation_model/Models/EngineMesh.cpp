#include "EngineMesh.h"
#include "Physics/Hardware/Common/Propulsion/Engine.h"
#include "Physics/External/Environment.h"

namespace Graphics {

	unsigned char EngineMesh::mNumInstances = 0;

	EngineMesh::EngineMesh(const std::string& name, const std::string& objFilePath, const Physics::Hardware::Engine& engine, GF::ResourceSet& resourceBucket, GF::Model3D& parentModel) :
		IStageComponentMesh(resourceBucket, parentModel),
		mName(name),
		mOBJFilePath(objFilePath),
		mEngineDataSource(engine)
	{
		loadResources();
		mNumInstances++;
	}

	void EngineMesh::loadResources() {
		GF::Graphics::Shader* shader = mResourceBucket.getResource<GF::Graphics::Shader>("bodyShader");

		mMesh = mResourceBucket.addOBJMesh(mName + std::to_string(mNumInstances), mOBJFilePath, GL_TRIANGLES, nullptr, shader);
		mParentModel.addMesh(mMesh);
	}

	void EngineMesh::updateResources(glm::mat4 totalStageTransform_OGL) {
		mMesh->setModelTransform(totalStageTransform_OGL * glm::mat4(mEngineDataSource.getCompToStageTransform().getLocalToParent_total()));
	}
	
}