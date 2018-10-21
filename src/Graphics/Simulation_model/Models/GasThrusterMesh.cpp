#include "GasThrusterMesh.h"
#include "Physics/Hardware/Common/Propulsion/GasThruster.h"

namespace Graphics {

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

}