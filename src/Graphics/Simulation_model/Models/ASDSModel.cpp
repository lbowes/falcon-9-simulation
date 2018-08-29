#include "ASDSModel.h"

namespace Graphics {

	ASDSModel::ASDSModel(GF::Graphics::Renderer& renderer, GF::ResourceSet& resourceBucket, GF::Graphics::Shader& shader, float height) :
		ISimulationModel(renderer, resourceBucket),
		mShader(shader),
		mHeight(height)
	{
		loadResources();
	}

	void ASDSModel::loadResources() {
		mSimpleCameraState.mPosition_highP = glm::dvec3(0.0, mHeight, 0.0);
		
		mMeshHandle = mResourceBucket.addOBJMesh("ASDS", "res/models/ASDS_Box.obj", GL_TRIANGLES, nullptr, &mShader, translate(mSimpleCameraState.mPosition_highP));
	}

	void ASDSModel::updateAllTransforms_OGL(SimpleCameraState currentCameraState) {
		glm::dvec3 posToCamPos = currentCameraState.mPosition_highP - mSimpleCameraState.mPosition_highP;
		glm::mat4 updatedTransform = inverse(translate(posToCamPos));
		mMeshHandle->setModelTransform(updatedTransform);
	}
	
	void ASDSModel::makeRenderCalls() {
		mMeshHandle->sendRenderCommand(mRenderer);
	}

}