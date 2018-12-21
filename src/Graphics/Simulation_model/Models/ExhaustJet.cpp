#include "ExhaustJet.h"
#include "Physics/Hardware/Common/Propulsion/Engine.h"
#include "Physics/External/Environment.h"

#include <GLFW/glfw3.h>

#include <GraphicsFramework/Vendor/ImGui/imgui.h>
#include <glm/gtx/rotate_vector.hpp>

#define EXHAUST_WIREFRAME 0

namespace Graphics {
	
	unsigned char ExhaustJet::mNumInstances = 0;

	ExhaustJet::ExhaustJet(const Physics::Hardware::Engine& sourceEngine, const Physics::Hardware::ThrustGeneratorGroup& nearbyEngines, GF::ResourceSet& resourceBucket) :
		mSourceEngine(sourceEngine),
		mNearbyEngines(nearbyEngines),
		mResourceBucket(resourceBucket) 
	{
		loadResources();
		mNumInstances++;
	}

	void ExhaustJet::render(GF::Camera& currentCamera) {
		mRenderer.setCamera(currentCamera);
		
		mMesh->sendRenderCommand(mRenderer);
		
#if EXHAUST_WIREFRAME
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);		
#endif
		glDepthMask(GL_FALSE);
		mRenderer.flush();
		glDepthMask(GL_TRUE);
#if EXHAUST_WIREFRAME
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);		
#endif
	}

	void ExhaustJet::updateTransform(glm::mat4 totalStageTransform_OGL) {
		glm::mat4 engineTransform_OGL = totalStageTransform_OGL * glm::mat4(mSourceEngine.getCompToStageTransform().getLocalToParent_total());
		mMesh->setModelTransform(engineTransform_OGL * glm::translate(glm::vec3(0.0f, -mSourceEngine.mTotalLength, 0.0f)));
	}

	void ExhaustJet::update(float airPressure_percent, glm::vec3 ambientFlowDir_stage) {
		if(!mSourceEngine.isActive()) {
			mMesh->setVisibility(false);
			return;
		}
		else {
			mMesh->setVisibility(true);
			
			float throttleInRange_percent = (mSourceEngine.getThrottle() - mSourceEngine.getThrottleMin()) / (mSourceEngine.getThrottleMax() - mSourceEngine.getThrottleMin());

			mShader->bind();
			mShader->setUniform("u_throttleInRange_percent", throttleInRange_percent);
			mShader->setUniform("u_airPressure_percent", airPressure_percent);
			mShader->setUniform("u_time", static_cast<float>(glfwGetTime()));
			mShader->setUniform("u_ambientFlowDir", -ambientFlowDir_stage);
			mShader->setUniform("u_ambientPressureDir", recalcAmbPressureDir_stage());
		}
	}

	void ExhaustJet::loadResources() {
		std::string id = std::to_string(mNumInstances);

		initBufferData();
		
		GF::Graphics::VertexFormat posFormat(0, 3, GL_FLOAT);
		GF::Graphics::VertexBuffer* positionBuffer = mResourceBucket.addVertexBuffer("exhaustPositionBuffer" + id, GL_STATIC_DRAW, posFormat, mPositionData);
		GF::Graphics::IndexBuffer* indexBuffer = mResourceBucket.addIndexBuffer("exhaustIndexBuffer" + id, GL_STATIC_DRAW, mIndexData);

		mShader = mResourceBucket.addShader("exhaustShader", "res/shaders/exhaust.vert", "res/shaders/exhaust.frag");
		mShader->addUniform("modelMatrix");
		mShader->addUniform("viewMatrix");
		mShader->addUniform("projectionMatrix");
		mShader->addUniformWithDefault("u_nozzleDiameter_m", mSourceEngine.getNozzleExitDiameter());
		mShader->addUniform("u_throttleInRange_percent");
		mShader->addUniform("u_airPressure_percent");
		mShader->addUniform("u_time");
		mShader->addUniform("u_ambientFlowDir");
		mShader->addUniform("u_ambientPressureDir");

		mMesh = mResourceBucket.addMesh("exhaustMesh" + id, GL_TRIANGLES, nullptr, mShader);
		mMesh->addBuffer(positionBuffer);
		mMesh->addIndexBuffer(indexBuffer);
	}

	void ExhaustJet::initBufferData() {
		//Position data
		for(unsigned char ring = 0; ring < mNumRings; ring++) {
			for(unsigned char segment = 0; segment < mNumSegments; segment++) {
				glm::vec2 vertexPointer = glm::rotate(glm::vec2(0.0f, -mSourceEngine.mNozzleExitDiameter * 0.5f), static_cast<float>(segment) / mNumSegments * glm::two_pi<float>());
				mPositionData.push_back(vertexPointer.x);
				mPositionData.push_back(-static_cast<float>(ring) / mNumRings);
				mPositionData.push_back(vertexPointer.y);
			}
		}

		//Index data
		for (unsigned char ring = 0; ring < mNumRings - 1; ring++) {
			unsigned short ringMod = ring * mNumSegments;
			for (unsigned char side = 0; side < mNumSegments; side++) {
				mIndexData.push_back(ringMod + side);
				mIndexData.push_back(ringMod + ((side + 1) % mNumSegments));
				mIndexData.push_back(ringMod + ((side + 1) % mNumSegments) + mNumSegments);
				mIndexData.push_back(ringMod + ((side + 1) % mNumSegments) + mNumSegments);
				mIndexData.push_back(ringMod + side + mNumSegments);
				mIndexData.push_back(ringMod + side);
			}
		}
	}

	glm::vec3 ExhaustJet::recalcAmbPressureDir_stage() {	
		using namespace glm;
		
		vec3 averageActiveEnginePos_stage = vec3(0.0f);

		unsigned activeEngineCount = 0;

		for(const auto& c : mNearbyEngines.getAllComponents()) {
			const Physics::Hardware::Engine* engine = static_cast<Physics::Hardware::Engine*>(c.get());

			if(engine->isActive()) {
				dvec3 enginePos_stage = engine->getCompToStageTransform().toParentSpace();
				averageActiveEnginePos_stage += enginePos_stage;
				activeEngineCount++;
			}
		}

		if(activeEngineCount > 0)
			averageActiveEnginePos_stage /= activeEngineCount;

		vec3 sourceEnginePosition = mSourceEngine.getCompToStageTransform().toParentSpace();

		vec3 output;
		if(averageActiveEnginePos_stage == sourceEnginePosition)
			output = vec3(0.0, 1.0, 0.0);
		else
			output = normalize(averageActiveEnginePos_stage - sourceEnginePosition);

		return output;
	}

}