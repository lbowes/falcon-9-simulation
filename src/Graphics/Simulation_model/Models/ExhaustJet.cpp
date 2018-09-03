#include "ExhaustJet.h"
#include "Physics/Hardware/Common/Propulsion/Engine.h"
#include "Physics/External/Environment.h"

#include <GLFW/glfw3.h>

#include <GraphicsFramework/Vendor/ImGui/imgui.h>
#include <glm/gtx/rotate_vector.hpp>

namespace Graphics {
	
	unsigned char ExhaustJet::mNumInstances = 0;

	ExhaustJet::ExhaustJet(const Physics::Hardware::Engine& sourceEngine, const Physics::Hardware::ThrustGeneratorGroup& nearbyEngines, GF::ResourceSet& resourceBucket, GF::Graphics::Renderer& renderer) :
		mSourceEngine(sourceEngine),
		mNearbyEngines(nearbyEngines),
		mResourceBucket(resourceBucket), 
		mRenderer(renderer)
	{
		loadResources();
		mNumInstances++;
	}

	void ExhaustJet::render() {
		mMesh->sendRenderCommand(mRenderer);
		mRenderer.flush();
	}

	void ExhaustJet::updateTransform(glm::mat4 totalStageTransform_OGL) {
		glm::mat4 engineTransform_OGL = totalStageTransform_OGL * glm::mat4(mSourceEngine.getCompToStageTransform().getLocalToParent_position());
		mMesh->setModelTransform(engineTransform_OGL * glm::translate(glm::vec3(0.0f, -mSourceEngine.mTotalLength, 0.0f)));
	}

	void ExhaustJet::update(float percentAirPressure, glm::vec3 ambientFlow_stage) {
		if(!mSourceEngine.isActive()) {
			mMesh->setVisibility(false);
			return;
		}
		else {
			mMesh->setVisibility(true);
			
			mShader->bind();
			mShader->setUniform("throttleInRange_percent", mSourceEngine.getThrottle());
			mShader->setUniform("airPressure_percent", percentAirPressure);
			mShader->setUniform("time", static_cast<float>(glfwGetTime()));
			mShader->setUniform("ambientFlow", -ambientFlow_stage);
			mShader->setUniform("ambientPressureDir", recalcAmbPressureDir_stage());
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
		mShader->addUniform("maxLength");
		mShader->addUniform("percentAirPressure");
		mShader->addUniform("time");
		mShader->addUniform("ambientFlow");
		mShader->addUniform("ambientPressureDir");

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
				averageActiveEnginePos_stage += vec3(enginePos_stage.x, 0.0f, enginePos_stage.z);
				activeEngineCount++;
			}
		}

		averageActiveEnginePos_stage /= activeEngineCount;

		vec3 sourceEnginePosition = mSourceEngine.getCompToStageTransform().toParentSpace();

		if(averageActiveEnginePos_stage == sourceEnginePosition)
			return vec3(0.0f, -1.0f, 0.0f);	
		else {
			vec3 output = normalize(averageActiveEnginePos_stage - sourceEnginePosition);
			return vec3(output.x, 0.0f, output.z);
		}
	}

}