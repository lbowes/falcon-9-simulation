#include "ExhaustJet.h"
#include "Physics/Hardware/Common/Propulsion/Engine.h"
#include "Physics/External/Environment.h"

#include <GLFW/glfw3.h>

#include <GraphicsFramework/Vendor/ImGui/imgui.h>
#include <glm/gtx/rotate_vector.hpp>

namespace Graphics {
	
	unsigned char ExhaustJet::mNumInstances = 0;

	ExhaustJet::ExhaustJet(const Physics::Hardware::Engine& sourceEngine, const Physics::Hardware::ThrustGeneratorGroup& nearbyEngines, GF::ResourceSet& resourceBucket, GF::Model3D& parentModel) :
		mSourceEngine(sourceEngine),
		mNearbyEngines(nearbyEngines),
		mResourceBucket(resourceBucket), 
		mParentModel(parentModel)
	{
		loadResources();
		mNumInstances++;
	}

	void ExhaustJet::update(glm::mat4 totalStageTransform_OGL, float percentAirPressure, glm::vec3 ambientFlow_stage) {
		if(!mSourceEngine.isActive()) {
			mMesh->setVisibility(false);
			return;
		}
		else {
			mMesh->setVisibility(true);
			
			glm::mat4 engineTransform_OGL = totalStageTransform_OGL * glm::mat4(mSourceEngine.getCompToStageTransform().getLocalToParent_total());
			mMesh->setModelTransform(engineTransform_OGL * glm::translate(glm::vec3(0.0f, -mSourceEngine.mTotalLength, 0.0f)));
			
			mShader->bind();
			mShader->setUniform("maxLength", recalcMaxLength());
			mShader->setUniform("percentAirPressure", percentAirPressure);
			mShader->setUniform("time", static_cast<float>(glfwGetTime()));
			mShader->setUniform("ambientFlow", glm::vec3(mSourceEngine.getCompToStageTransform().toLocalSpace_rotation(ambientFlow_stage)));
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

		mParentModel.addMesh(mMesh);
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

	float ExhaustJet::recalcMaxLength() {
		//TODO: Calculate maximum length of
		
		return 60.0f;
	}

}