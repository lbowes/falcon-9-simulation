#include "F9S2Model.h"
#include "Physics/Hardware/Falcon_9/Stage_2/Falcon9Stage2Composite.h"

namespace Graphics {

	F9S2Model::F9S2Model(const Physics::Hardware::Falcon9Stage2Composite& dataSource, GF::Graphics::Renderer& renderer, GF::ResourceSet& resourceBucket) :
		ISimulationModel(renderer, resourceBucket),
		mDataSource(dataSource)
	{
		loadResources();
	}

	void F9S2Model::loadResources() {
		//Relies on this shader having been already added by the F9S1Model
		GF::Graphics::Shader* shader = mResourceBucket.addShader("bodyShader", "res/shaders/body.vert", "res/shaders/body.frag");
		mModel.addMesh(mResourceBucket.addOBJMesh("F9S2Fuselage", "res/models/F9S2Fuselage.obj", GL_TRIANGLES, nullptr, shader));

		addComponentModels();
	}

	void F9S2Model::addComponentModels() {
		using namespace Physics::Hardware;

		//Engine
		mEngineMesh = std::make_unique<EngineMesh>("Merlin1DVac", "res/models/Merlin1DVac.obj", *mDataSource.getCore().getEngines().get<Engine>(0), mResourceBucket, mModel);

		//Fairing halves
		mFairingHalfMeshes.first = std::make_unique<FairingHalfMesh>(mDataSource.getFairings().first, mResourceBucket, mModel);
		mFairingHalfMeshes.second = std::make_unique<FairingHalfMesh>(mDataSource.getFairings().second, mResourceBucket, mModel);
	}

	void F9S2Model::updateAllTransforms_OGL(glm::dvec3 currentCameraPos_world) {
		using namespace glm;

		const State& state = mDataSource.getCore().getState();

		mat4
			posTransform_OGL = translate(currentCameraPos_world - state.getObjectSpace().toParentSpace()),
			rotTransform_OGL = toMat4(inverse(state.getOrientation_world())),
			totalTransform_OGL = inverse(rotTransform_OGL * posTransform_OGL);

		//Updates the mesh representing the fuselage
		mModel.getMesh(0)->setModelTransform(totalTransform_OGL);

		//Updates the engine mesh (that inherit from IStageComponentMesh)
		mEngineMesh->updateResources(totalTransform_OGL);

		//Updates the two fairing halves (that DO NOT inherit from IStageComponentMesh)
		mFairingHalfMeshes.first->updateTransform_OGL(currentCameraPos_world);
		mFairingHalfMeshes.second->updateTransform_OGL(currentCameraPos_world);
	}

	void F9S2Model::makeRenderCalls() {
		mModel.sendRenderCommands(mRenderer);
	}

}
