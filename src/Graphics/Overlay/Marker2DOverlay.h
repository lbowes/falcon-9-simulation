#ifndef GRAPHICS_MARKER2DOVERLAY_H
#define GRAPHICS_MARKER2DOVERLAY_H
#pragma once

#include <GraphicsFramework/ResourceSet.h>
#include <GraphicsFramework/Renderer.h>
#include <GraphicsFramework/Shader.h>
#include <GraphicsFramework/Texture2D.h>

namespace Graphics {

	struct Marker2D {
		enum class Type { origin, centreMass };

		static constexpr float mRadius = 0.013f;

		glm::vec3
			*mTarget_world,
			mResult_NDC;

		std::string mName;

		Type mType;

		Marker2D(glm::vec3* target_world, Type type, const std::string& name = "") :
			mTarget_world(target_world),
			mName(name),
			mType(type)
		{
			switch (type) {
			case Type::origin:
				mName.append(" (origin)");
				break;
			case Type::centreMass:
				mName.append(" (CoM)");
				break;
			default:
				break;
			}
		}

		void recalc(glm::mat4 viewProjection, float windowAspect) {
			glm::vec4 clipSpace = viewProjection * glm::vec4(*mTarget_world, 1.0);
			
			if (clipSpace.x > clipSpace.w || clipSpace.x < -clipSpace.w ||
				clipSpace.y > clipSpace.w || clipSpace.y < -clipSpace.w ||
				clipSpace.z > clipSpace.w || clipSpace.z < -clipSpace.w
			) 
				clipSpace.x = clipSpace.w * 2.0f;
			
			mResult_NDC = glm::vec3(clipSpace) / clipSpace.w;
			mResult_NDC.x *= windowAspect;
		}

	};

	class Marker2DOverlay {
	private:
		std::vector<Marker2D> mMarkers;

		GF::ResourceSet& mResourceContainer;
		
		GF::Graphics::Renderer& mRenderer;

		GF::Graphics::Mesh* mMesh = nullptr;
		
		GF::Graphics::VertexBuffer
			*mPositionVBO = nullptr,
			*mTexCoordVBO = nullptr;
		
		GF::Graphics::IndexBuffer* mIBO = nullptr;

		std::vector<float> mPositions;
		std::vector<float> mTexCoords;
		std::vector<unsigned> mIndices;

	public:
		Marker2DOverlay(GF::ResourceSet& resourceContainer, GF::Graphics::Renderer& renderer);
		~Marker2DOverlay() = default;

		void render(glm::mat4 viewProjection, float windowAspect, glm::vec2 windowDimensions);
		void addMarker(glm::vec3& target, Marker2D::Type type, const std::string& name = "");

	private:
		void load();
		void updateAllMarkers(glm::mat4 viewProjection, float windowAspect);
		void rebuildBuffers();
		void updateBuffers();
		void checkCursorOverMarkers(float windowAspect, glm::vec2 windowDimensions);

	};

}

#endif
