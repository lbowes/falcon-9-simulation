#include "F9S1Model.h"
#include "Physics/Hardware/Falcon_9/Stage_1/Falcon9Stage1.h"

#include <GraphicsFramework/Vendor/ImGui/imgui.h>

namespace Graphics {

	F9S1Model::F9S1Model(const Physics::Hardware::Falcon9Stage1& dataSource, GF::Graphics::Renderer& renderer, GF::ResourceSet& resourceBucket) :
		ISimulationModel(renderer, resourceBucket),
		mDataSource(dataSource)
	{
		loadResources();
	}

	void F9S1Model::loadResources() {
		mMainShader = mResourceBucket.addShader("bodyShader", "res/shaders/flat.vert", "res/shaders/flat.frag");
		mMainShader->addUniform("modelMatrix");
		mMainShader->addUniform("viewMatrix");
		mMainShader->addUniform("projectionMatrix");
																 
		mModel.addMesh(mResourceBucket.addOBJMesh("F9S1Fuselage", BOX_MODELS ? "res/models/Falcon9Stage1_Box.obj" : "res/models/F9S1Fuselage.obj", GL_TRIANGLES, nullptr, mMainShader));

		addComponentModels();
	}

	void F9S1Model::addComponentModels() {
		using namespace Physics::Hardware;

		//Landing leg meshes
		for (const auto& landingLeg : mDataSource.getLandingLegs().getAllComponents())
			mComponentMeshes.push_back(std::make_unique<LandingLegMesh>(*static_cast<LandingLeg*>(landingLeg.get()), mResourceBucket, mModel));

		//Grid fin meshes
		for (const auto& gridFin : mDataSource.getGridFins().getAllComponents())
			mComponentMeshes.push_back(std::make_unique<GridFinMesh>(*static_cast<GridFin*>(gridFin.get()), mResourceBucket, mModel));

		//Gas thruster meshes
		for (const auto& thruster : mDataSource.getThrusters().getAllComponents())
			mComponentMeshes.push_back(std::make_unique<GasThrusterMesh>(*static_cast<GasThruster*>(thruster.get()), mResourceBucket, mModel));

		//Engine meshes
		unsigned char count = 0;
		for (const auto& engine : mDataSource.getEngines().getAllComponents()) {
			mComponentMeshes.push_back(std::make_unique<EngineMesh>("Merlin1D" + std::to_string(count), BOX_MODELS ? "res/models/Merlin1D_Box.obj" : "res/models/Merlin1D.obj", *static_cast<Engine*>(engine.get()), mResourceBucket, mModel));
			count++;
		}
	}

	void F9S1Model::updateAllTransforms_OGL(glm::dvec3 currentCameraPosition) {
		using namespace glm;

		mat4
			posTransform_OGL = translate(currentCameraPosition - mDataSource.getState().getObjectSpace().toParentSpace()),
			rotTransform_OGL = toMat4(inverse(mDataSource.getState().getOrientation_world()));
			
		mTotalTransform_OGL = inverse(rotTransform_OGL * posTransform_OGL);

		//Updates the transform of the mesh representing the fuselage
		mModel.getMesh(0)->setModelTransform(mTotalTransform_OGL);

		//Update all component mesh transforms
		for (const auto& componentMesh : mComponentMeshes) 
			componentMesh->updateResources(mTotalTransform_OGL);
	}

	void F9S1Model::makeRenderCalls() {
		mModel.sendRenderCommands(mRenderer);
	}

}