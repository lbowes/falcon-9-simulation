#include "AllComponentMeshes.h"

#include "Physics/Hardware/Common/Propulsion/Engine.h"
#include "Physics/Hardware/Common/Propulsion/GasThruster.h"
#include "Physics/Hardware/Falcon_9/Stage_1/Grid_fins/GridFin.h"
#include "Physics/Hardware/Falcon_9/Stage_1/Landing_legs/LandingLeg.h"
#include "Physics/Hardware/Falcon_9/Stage_2/FairingHalf.h"

namespace Graphics {

	//-------------------------------------------------------------------ENGINE-------------------------------------------------------------------

	EngineMesh::EngineMesh(const std::string& name, const std::string& objFilePath, const Physics::Hardware::Engine& engine, GF::ResourceSet& resourceBucket, GF::Model3D& parentModel) :
		IStageComponentMesh(resourceBucket, parentModel),
		mName(name),
		mOBJFilePath(objFilePath),
		mEngineDataSource(engine)
	{
		loadResources();
	}

	void EngineMesh::loadResources() {
		GF::Graphics::Shader* shader = mResourceBucket.getResource<GF::Graphics::Shader>("bodyShader");

		mMesh = mResourceBucket.addOBJMesh(mName, mOBJFilePath, GL_TRIANGLES, nullptr, shader);
		mParentModel.addMesh(mMesh);
	}

	void EngineMesh::updateResources(glm::mat4 totalStageTransform_OGL) {
		mMesh->setModelTransform(totalStageTransform_OGL * glm::mat4(mEngineDataSource.getCompToStageTransform().getLocalToParent_total()));
	}

	//-------------------------------------------------------------------GAS THRUSTER-------------------------------------------------------------------

	unsigned char GasThrusterMesh::mNumInstances = 0;

	GasThrusterMesh::GasThrusterMesh(Physics::Hardware::GasThruster& dataSource, GF::ResourceSet& resourceBucket, GF::Model3D& parentModel) :
		IStageComponentMesh(resourceBucket, parentModel),
		mDataSource(dataSource)
	{
		loadResources();
		mNumInstances++;
	}

	void GasThrusterMesh::loadResources() {
		using namespace std;
		using namespace GF::Graphics;

		float
			sr = mStartPlumeRadius,
			er = mEndPlumeRadius,
			l = mPlumeLength;

		vector<float> positions = {
			// front
			-er, -l,    er, //bottom left
			er, -l,    er, //bottom right
			sr,  0.0,  sr, //top right
			-sr,  0.0,  sr, //top left

			// back
			-er, -l,   -er, //bottom left
			er, -l,   -er, //bottom right
			sr,  0.0, -sr, //top right
			-sr,  0.0, -sr, //top left
		};
		vector<unsigned int> indices = {
			// front
			0, 1, 2,
			2, 3, 0,
			// top
			1, 5, 6,
			6, 2, 1,
			// back
			7, 6, 5,
			5, 4, 7,
			// bottom
			4, 0, 3,
			3, 7, 4,
			// left
			4, 5, 1,
			1, 0, 4,
			// right
			3, 2, 6,
			6, 7, 3
		};

		VertexBuffer* plumePositionBuffer = mResourceBucket.addVertexBuffer("nitrogenPlumePositions" + to_string(mNumInstances), GL_STATIC_DRAW, { 0, 3, GL_FLOAT, false }, positions);
		IndexBuffer* plumeIndexBuffer = mResourceBucket.addIndexBuffer("nitrogenPlumeIndices" + to_string(mNumInstances), GL_STATIC_DRAW, indices);
		
		Shader* shader = mResourceBucket.addShader("whiteShader", "res/shaders/white.vert", "res/shaders/white.frag");
		shader->addUniform("modelMatrix");
		shader->addUniform("viewMatrix");
		shader->addUniform("projectionMatrix");

		mGasMesh = mResourceBucket.addMesh("nitrogenPlume" + to_string(mNumInstances), GL_TRIANGLES, nullptr, shader);
		mGasMesh->addBuffer(plumePositionBuffer);
		mGasMesh->addIndexBuffer(plumeIndexBuffer);

		mParentModel.addMesh(mGasMesh);
	}	

	void GasThrusterMesh::updateResources(glm::mat4 stageModelTransform_OGL) {
		using namespace glm;

		if (!mDataSource.isActive()) {
			mGasMesh->setVisibility(false);
			return;
		}

		float angle = 0.0;
		vec3 direction = mDataSource.getCompToStageTransform().toParentSpace_rotation({0.0, -1.0, 0.0});

		//x
		angle = asin(direction.x / length(direction));
		mat4 transform_OGL = rotate(angle, vec3(0.0f, 0.0f, 1.0f));
		//z
		angle = asin(direction.z / length(direction));
		transform_OGL = rotate(transform_OGL, angle, vec3(-1.0f, 0.0f, 0.0f));

		transform_OGL = stageModelTransform_OGL * mat4(mDataSource.mCompToStage.getLocalToParent_translation()) * transform_OGL;

		mGasMesh->setVisibility(true);
		mGasMesh->setModelTransform(transform_OGL);
	}

	//-------------------------------------------------------------------GRID FIN-------------------------------------------------------------------
	
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


		const std::string id = std::to_string(mNumInstances);

		//Hinge
		mHingeMesh = mResourceBucket.addOBJMesh("gridFinHinge" + id, BOX_MODELS ? "res/models/GridFinHinge_Box.obj" : "res/models/GridFinHinge.obj", GL_TRIANGLES, nullptr, shader);
		mParentModel.addMesh(mHingeMesh);

		//Fin
		mFinMesh = mResourceBucket.addOBJMesh("gridFin" + id, BOX_MODELS ? "res/models/GridFin_Box.obj" : "res/models/GridFin_JonRoss.obj", GL_TRIANGLES, nullptr, shader);
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

	//-------------------------------------------------------------------LANDING LEG-------------------------------------------------------------------

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

		const std::string id = std::to_string(mNumInstances);

		//A-frame																		 
		mAFrameMesh = mResourceBucket.addOBJMesh("landingLeg" + id, BOX_MODELS ? "res/models/LandingLeg_Box.obj" : "res/models/LandingLeg.obj", GL_TRIANGLES, nullptr, shader);
		mParentModel.addMesh(mAFrameMesh);

		//Telescoping piston cylinders
		Physics::Hardware::TelescopingPiston* piston = mDataSource.getPiston();
		for (unsigned char i = 0; i < piston->getSubCylinderLengths().size(); i++) {
			const std::string objFilepath = BOX_MODELS ? "res/models/PistonCylinder_box.obj" : "res/models/PistonCylinder_" + std::to_string(i) + ".obj";
			mPistonCylinderMeshes.push_back(mResourceBucket.addOBJMesh("piston" + std::to_string(i) + "leg" + id, objFilepath, GL_TRIANGLES, nullptr, shader));
			mParentModel.addMesh(mPistonCylinderMeshes.back());
		}

		//Pusher
		mPusherMesh = mResourceBucket.addOBJMesh("leg" + id + "pusher", "res/models/UnitCylinder.obj", GL_TRIANGLES, nullptr, shader);
		mParentModel.addMesh(mPusherMesh);
	}

	void LandingLegMesh::updateResources(glm::mat4 stageModelTransform_OGL) 
		//Responsible for recalculating the mesh transforms of the A frame and piston cylinders based on the mDataSource and 
		//stageModelTransform_OGL, and updating the meshes with this transform
	{
		using namespace glm;

		const float clockDegree_rads = glm::radians(mDataSource.mClockingDegree);

		//A-Frame mesh updates
		{
			mat4 AFrameTransform = stageModelTransform_OGL * mat4(mDataSource.getCompToStageTransform().getLocalToParent_total());
			mAFrameMesh->setModelTransform(AFrameTransform);
		}

		//Telescoping piston mesh updates
		{
			//Using the information about mParent, this function needs to update the OpenGL transform of the cylinders along the telescoping piston.
			Physics::Hardware::TelescopingPiston* piston = mDataSource.getPiston();
		
			const std::vector<double> cylinderLengths = piston->getSubCylinderLengths();
			float lengthCovered = 0.0f;			
			for (unsigned char i = 0; i < cylinderLengths.size(); i++) {
				float 
					pistonLength = piston->getLength(),
					alongPiston = 0.0;
				
				if(lengthCovered + cylinderLengths[i] < pistonLength)
					alongPiston = lengthCovered;
				else
					alongPiston = pistonLength - cylinderLengths[i];

				lengthCovered += cylinderLengths[i];

				//Rotate the cylinder around the stage
				mat4 pistonFinalTransform = rotate(clockDegree_rads, vec3(0.0f, 1.0f, 0.0f));
		
				//Rotate the cylinder downwards to line up with the piston centre line
				const float angleFromVertical = glm::angle(normalize(mDataSource.mAlongTPiston_stage3D), { 0.0, 1.0, 0.0 });
				pistonFinalTransform = rotate(pistonFinalTransform, angleFromVertical, vec3(-1.0f, 0.0f, 0.0f));
		
				//Slide the cylinder into the correct position along the piston centre line
				pistonFinalTransform = translate(pistonFinalTransform, vec3(0.0f, alongPiston, 0.0f));
		
				//Translate the cylinder upwards to the mounting point on the stage
				const mat4 pistonTranslation = translate(mDataSource.getPistonStartPoint_stage());
				pistonFinalTransform = stageModelTransform_OGL * pistonTranslation * pistonFinalTransform;
		
				//Apply this newly-calculated transform to the piston mesh
				mPistonCylinderMeshes[i]->setModelTransform(pistonFinalTransform);
			}
		}

		//Deployment pusher mesh updates			
		{
			const float diameter = 0.05f;

			//A scale transform to convert the unit cylinder into the a cylinder with different proportions to represent the deployment pusher
			const mat4 pusherScale = scale(vec3(diameter, static_cast<float>(mDataSource.getDeploymentActuator()->getLength()), diameter));

			mat4 pusherTransform = rotate(clockDegree_rads, vec3(0.0f, 1.0f, 0.0f));

			const float angleFromVertical = glm::angle(normalize(mDataSource.mAlongPusher_stage3D), { 0.0, 1.0, 0.0 });
			pusherTransform = rotate(pusherTransform, angleFromVertical, vec3(-1.0f, 0.0f, 0.0f));

			const mat4 pusherTranslation = translate(mDataSource.getPusherStartPoint_stage());
			pusherTransform = stageModelTransform_OGL * pusherTranslation * pusherTransform * pusherScale;
			
			mPusherMesh->setModelTransform(pusherTransform);
		}
	}

	//-------------------------------------------------------------------FAIRING HALF-------------------------------------------------------------------

	unsigned char FairingHalfMesh::mNumInstances = 0;

	FairingHalfMesh::FairingHalfMesh(const Physics::Hardware::FairingHalf& dataSource, GF::ResourceSet& resourceBucket, GF::Model3D& parentModel) :
		mDataSource(dataSource)
	{
		GF::Graphics::Shader* shader = resourceBucket.getResource<GF::Graphics::Shader>("bodyShader");
		mMesh = resourceBucket.addOBJMesh("fairingHalf" + std::to_string(mNumInstances), "res/models/FairingHalf.obj", GL_TRIANGLES, nullptr, shader);
		parentModel.addMesh(mMesh);

		mNumInstances++;
	}

	void FairingHalfMesh::updateTransform_OGL(glm::dvec3 currentCameraPos_world) {
		const State& state = mDataSource.getState();

		glm::mat4
			posTransform_OGL = translate(currentCameraPos_world - state.getObjectSpace().toParentSpace()),
			rotTransform_OGL = toMat4(inverse(state.getOrientation_world())),
			totalTransform_OGL = inverse(rotTransform_OGL * posTransform_OGL);
		
		mMesh->setModelTransform(totalTransform_OGL);
	}

}