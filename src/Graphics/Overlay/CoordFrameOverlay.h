#if 0
#ifndef GRAPHICS_COORDFRAME2DOVERLAY_H
#define GRAPHICS_COORDFRAME2DOVERLAY_H
#pragma once

#include <glm/vec3.hpp>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>

#include <GraphicsFramework/ResourceSet.h>
#include <GraphicsFramework/Renderer.h>

namespace Graphics {

	struct CoordFrame2D {
		const glm::vec3* mTargetPosition_world;
		const glm::mat4* mTargetOrientation_world;

		const float mSize = 0.0f;
		
		glm::vec3
			mOriginPosition_NDC,
			mXPosition_NDC,
			mYPosition_NDC,
			mZPosition_NDC;

		CoordFrame2D(const glm::vec3& position_world, const glm::mat4& targetOrientation_world, float size);
		~CoordFrame2D() = default;

		void recalc(glm::mat4 viewProjection, float windowAspect);

	};

	class CoordFrameOverlay {
	private:
		GF::ResourceSet& mResourceContainer;

		GF::Graphics::Renderer& mRenderer;

		std::vector<std::unique_ptr<CoordFrame2D>> mCoordFrames;

		GF::Graphics::Mesh* mMesh = nullptr;

		GF::Graphics::VertexBuffer
			*mPositionVBO = nullptr,
			*mColourVBO = nullptr;

		GF::Graphics::IndexBuffer* mIBO = nullptr;

		std::vector<float> 
			mPositions, 
			mColours;

		std::vector<unsigned> mIndices;

	public:
		CoordFrameOverlay(GF::ResourceSet& resourceContainer, GF::Graphics::Renderer& renderer);
		~CoordFrameOverlay() = default;

		void render(glm::mat4 viewProjection, float windowAspect);
		void addCoordFrame(glm::vec3& targetPosition, glm::mat4& targetOrientation, float size);

	private:
		void load();
		void updateAllCoordFrames(glm::mat4 viewProjection, float windowAspect);
		void rebuildBuffers();
		void updateBuffers();

	};

}

#endif

#endif