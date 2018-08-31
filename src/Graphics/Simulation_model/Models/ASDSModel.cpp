#include "ASDSModel.h"

namespace Graphics {

	ASDSModel::ASDSModel(GF::Graphics::Renderer& renderer, GF::ResourceSet& resourceBucket, GF::Graphics::Shader& shader) :
		ISimulationModel(renderer, resourceBucket),
		mShader(shader)
	{
		loadResources();
	}

	void ASDSModel::loadResources() {
		mMeshHandle = mResourceBucket.addOBJMesh("ASDS", "res/models/ASDS_Box.obj", GL_TRIANGLES, nullptr, &mShader, translate(mPosition));
	}

	void ASDSModel::updateAllTransforms_OGL(glm::dvec3 currentCameraPosition) {
		glm::dvec3 posToCamPos = currentCameraPosition - mPosition;
		glm::mat4 updatedTransform = inverse(translate(posToCamPos));
		mMeshHandle->setModelTransform(updatedTransform);
	}
	
	void ASDSModel::makeRenderCalls() {
		mMeshHandle->sendRenderCommand(mRenderer);
	}

}