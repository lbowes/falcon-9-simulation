#ifndef GRAPHICS_EXHAUSTJET_H
#define GRAPHICS_EXHAUSTJET_H
#pragma once

#include "Physics/Hardware/Common/Propulsion/ThrustGeneratorGroup.h"

#include <GraphicsFramework/Renderer.h>
#include <GraphicsFramework/ResourceSet.h>

namespace Physics {
	namespace Hardware {
		class Engine;
	}
}

namespace Graphics {

	class ExhaustJet {
	private:
		static constexpr unsigned char
			mNumSegments = 16,
			mNumRings = 24;
		
		static unsigned char mNumInstances;
		
		const Physics::Hardware::Engine& mSourceEngine;

		const Physics::Hardware::ThrustGeneratorGroup& mNearbyEngines;

		GF::ResourceSet& mResourceBucket;
		GF::Graphics::Renderer& mRenderer;
		GF::Graphics::Mesh* mMesh = nullptr;
		GF::Graphics::Shader* mShader = nullptr;

		std::vector<float> mPositionData;
		std::vector<unsigned> mIndexData;

	public:
		ExhaustJet(const Physics::Hardware::Engine& sourceEngine, const Physics::Hardware::ThrustGeneratorGroup& nearbyEngines, GF::ResourceSet& resourceBucket, GF::Graphics::Renderer& renderer);
		~ExhaustJet() = default;

		void render();
		void updateTransform(glm::mat4 totalStageTransform_OGL);
		void update(float percentAirPressure, glm::vec3 ambientFlow_stage);

	private:
		void loadResources();
		void initBufferData();
		float recalcMaxLength();
		glm::vec3 recalcAmbPressureDir_stage();

	};

}

#endif
