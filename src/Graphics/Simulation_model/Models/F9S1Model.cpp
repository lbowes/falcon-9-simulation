#include "F9S1Model.h"
#include "Physics/Hardware/Falcon_9/Stage_1/Falcon9Stage1.h"

namespace Graphics {

	F9S1Model::F9S1Model(const Physics::Hardware::Falcon9Stage1& stage1Data, GF::Graphics::Renderer& renderer, GF::ResourceSet& resourceBucket) :
		ISimulationModel(renderer, resourceBucket),
		mStage1Data(stage1Data)
	{
		loadResources();
	}

	void F9S1Model::loadResources() {
		mMainShader = mResourceBucket.addShader("bodyShader", "res/shaders/flat.vert", "res/shaders/flat.frag");
		mMainShader->addUniform("modelMatrix");
		mMainShader->addUniform("viewMatrix");
		mMainShader->addUniform("projectionMatrix");
																 
		mModel.addMesh(mResourceBucket.addOBJMesh("F9S1Fuselage", BOX_MODELS ? "res/models/Falcon9Stage1_Box.obj" : "res/models/F9S1Fuselage.obj", GL_TRIANGLES, nullptr, mMainShader));
		//mModel.getMesh(0)->setVisibility(false);

		addComponentModels();
	}

	void F9S1Model::addComponentModels() {
		using namespace Physics::Hardware;

		//Landing leg meshes
		for (const auto& landingLeg : mStage1Data.getLandingLegs().getAllComponents())
			mComponentMeshes.push_back(std::make_unique<LandingLegMesh>(*static_cast<LandingLeg*>(landingLeg.get()), mResourceBucket, mModel));

		//Grid fin meshes
		for (const auto& gridFin : mStage1Data.getGridFins().getAllComponents())
			mComponentMeshes.push_back(std::make_unique<GridFinMesh>(*static_cast<GridFin*>(gridFin.get()), mResourceBucket, mModel));

		//Gas thruster meshes
		for (const auto& thruster : mStage1Data.getThrusters().getAllComponents())
			mComponentMeshes.push_back(std::make_unique<GasThrusterMesh>(*static_cast<GasThruster*>(thruster.get()), mResourceBucket, mModel));

		//Engine meshes
		unsigned char count = 0;
		for (const auto& engine : mStage1Data.getEngines().getAllComponents()) {
			const Physics::Hardware::Engine& engineDataSource = *static_cast<Engine*>(engine.get());
			
			mComponentMeshes.push_back(std::make_unique<EngineMesh>("Merlin1D" + std::to_string(count), BOX_MODELS ? "res/models/Merlin1D_Box.obj" : "res/models/Merlin1D.obj", engineDataSource, mResourceBucket, mModel));
			mExhausts.push_back(std::make_unique<ExhaustJet>(engineDataSource, mStage1Data.getEngines(), mResourceBucket, mModel));

			count++;
		}
	}

	void F9S1Model::update(glm::mat4 totalStageTransform_OGL) {
		using namespace Physics::External;
		
		//Updates the mesh representing the fuselage
		mModel.getMesh(0)->setModelTransform(totalStageTransform_OGL);

		//Update all component meshes
		for (const auto& componentMesh : mComponentMeshes) 
			componentMesh->updateResources(totalStageTransform_OGL);

		//Update all exhaust meshes
		float percentAirPressure = static_cast<float>(Environment::getAirPressure_Pa(floor(mStage1Data.getState().getCMPosition_world().y))) / Environment::seaLevelStdPressure;
		const State& s1State = mStage1Data.getState();
		glm::vec3 ambientFlow_stage = -s1State.getObjectSpace().toLocalSpace_rotation(s1State.velocityAtLocalPoint_world(glm::vec3(0.0f)));

		for (const auto& exhaustJet : mExhausts)
			exhaustJet->update(totalStageTransform_OGL, percentAirPressure, ambientFlow_stage);
	}

	void F9S1Model::updateAllTransforms_OGL(SimpleCameraState currentCameraState) {
		using namespace glm;

		const State& state = mStage1Data.getState();

		mSimpleCameraState.mPosition_highP = state.getObjectSpace().toParentSpace();
		mSimpleCameraState.mOrientation_highP = state.getOrientation_world();

		mat4
			posTransform_OGL = translate(currentCameraState.mPosition_highP - mSimpleCameraState.mPosition_highP),
			rotTransform_OGL = toMat4(inverse(mSimpleCameraState.mOrientation_highP)),
			totalTransform_OGL = inverse(rotTransform_OGL * posTransform_OGL);

		update(totalTransform_OGL);
	}

	void F9S1Model::makeRenderCalls() {
		mModel.sendRenderCommands(mRenderer);
	}

	//------------------------------------------LANDING LEG MODEL------------------------------------------

	unsigned char LandingLegMesh::mNumInstances = 0;

	LandingLegMesh::LandingLegMesh(const Physics::Hardware::LandingLeg& dataSource, GF::ResourceSet& resourceBucket, GF::Model3D& parentModel) :
		IStageComponentMesh(resourceBucket, parentModel),
		mDataSource(dataSource)
	{
		loadResources();
		mNumInstances++;
	}

	void LandingLegMesh::loadResources() 
		//Responsible for adding whatever is needed to the mResourceBucket
	{
		GF::Graphics::Shader* shader = mResourceBucket.getResource<GF::Graphics::Shader>("bodyShader");

		std::string id = std::to_string(mNumInstances);

		//A-frame																		 
		mAFrameMesh = mResourceBucket.addOBJMesh("landingLeg" + id, BOX_MODELS ? "res/models/LandingLeg_Box.obj" : "res/models/LandingLeg.obj", GL_TRIANGLES, nullptr, shader);
		mParentModel.addMesh(mAFrameMesh);

		//Telescoping piston cylinders
		Physics::Hardware::TelescopingPiston* piston = mDataSource.getPiston();
		for (unsigned char i = 0; i < piston->getCylinders().size(); i++) {
			mPistonCylinderMeshes.push_back(mResourceBucket.addOBJMesh("piston" + std::to_string(i) + "leg" + id, BOX_MODELS ? "res/models/PistonCylinder_Box.obj" : "res/models/PistonCylinder.obj", GL_TRIANGLES, nullptr, shader));
			mParentModel.addMesh(mPistonCylinderMeshes.back());
		}
	}

	void LandingLegMesh::updateResources(glm::mat4 stageModelTransform_OGL) 
		//Responsible for recalculating the mesh transforms of the A frame and piston cylinders based on the mDataSource and 
		//stageModelTransform_OGL, and updating the meshes with this transform
	{
		using namespace glm;

		//A-Frame mesh updates
		{
			mat4 AFrameTransform = stageModelTransform_OGL * mat4(mDataSource.getCompToStageTransform().getLocalToParent_total());
			mAFrameMesh->setModelTransform(AFrameTransform);
		}

		//Piston cylinder mesh updates
		{
			//Using the information about mParent, this function needs to update the OpenGL transform of the cylinders along the telescoping piston.
			Physics::Hardware::TelescopingPiston* piston = mDataSource.getPiston();
		
			for (unsigned char i = 0; i < piston->getCylinders().size(); i++) {
				Physics::Hardware::PistonCylinder* cylinder = piston->getCylinder(i);
				
				float 
					cylinderLength = static_cast<float>(cylinder->getLength()),
					pistonLength = piston->getLength(),
					alongPiston = 0.0;
				
				mat4 pistonScale = scale(vec3(cylinder->getWidthScale(), cylinderLength, cylinder->getWidthScale()));

				if ((i + 1) * cylinderLength < pistonLength)
					alongPiston = i * cylinderLength;
				else
					alongPiston = (i * cylinderLength) - (((i + 1) * cylinderLength) - pistonLength);
		
				//Rotate the cylinder around the stage
				mat4 pistonFinalTransform = rotate(static_cast<float>(radians(piston->getClockingDegree_degs())), vec3(0.0f, 1.0f, 0.0f));
		
				//Rotate the cylinder downwards to line up with the piston centre line
				float angleFromVertical = glm::angle(normalize(mDataSource.mAlongPiston_stage3D), { 0.0, 1.0, 0.0 });
				pistonFinalTransform = rotate(pistonFinalTransform, static_cast<float>(angleFromVertical), vec3(-1.0f, 0.0f, 0.0f));
		
				//Slide the cylinder into the correct position along the piston centre line
				pistonFinalTransform = translate(pistonFinalTransform, vec3(0.0f, alongPiston, 0.0f));
		
				//Translate the cylinder upwards to the mounting point on the stage
				mat4 pistonTranslation = translate(vec3(piston->getMountPoint_stage()));
				pistonFinalTransform = stageModelTransform_OGL * pistonTranslation * pistonFinalTransform * pistonScale;
		
				//Apply this newly-calculated transform to the piston mesh
				mPistonCylinderMeshes[i]->setModelTransform(pistonFinalTransform);
			}
		}
	}

	//------------------------------------------GRID FIN MODEL---------------------------------------------

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
		hingeTransform = stageModelTransform_OGL * mat4(mDataSource.mCompToStage.getLocalToParent_position()) * hingeTransform;
		mHingeMesh->setModelTransform(hingeTransform);

		//Fin transform
		mFinMesh->setModelTransform(stageModelTransform_OGL * mat4(mDataSource.mCompToStage.getLocalToParent_total()));
	}

}