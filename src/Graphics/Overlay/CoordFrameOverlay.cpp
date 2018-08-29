#if 0
#include "CoordFrameOverlay.h"

//temp
#include <GraphicsFramework/Vendor/ImGui/imgui.h>

namespace Graphics {


	CoordFrame2D::CoordFrame2D(const glm::vec3& position_world, const glm::mat4& targetOrientation_world, float size) :
		mTargetPosition_world(&position_world),
		mTargetOrientation_world(&targetOrientation_world),
		mSize(size)
	{ }

	void CoordFrame2D::recalc(glm::mat4 viewProjection, float windowAspect) {
		using namespace glm;

		//vec4 clipSpace = viewProjection * vec4(*mTargetPosition_world, 1.0);
		vec4 clipSpace = viewProjection * vec4(0.0f, 0.0f, 0.0f, 1.0);
		
		//If the origin of the coordinate frame is outside the screen
		if (clipSpace.x > clipSpace.w || clipSpace.x < -clipSpace.w ||
			clipSpace.y > clipSpace.w || clipSpace.y < -clipSpace.w ||
			clipSpace.z > clipSpace.w || clipSpace.z < -clipSpace.w)
		{
			//then set the positions of the points way off the screen
			clipSpace.x = clipSpace.w * 2.0f;
		
			mOriginPosition_NDC = vec3(clipSpace) / clipSpace.w;
			mXPosition_NDC = mOriginPosition_NDC;
			mYPosition_NDC = mOriginPosition_NDC;
			mZPosition_NDC = mOriginPosition_NDC;
		
			return;
		}
		else {
			//Otherwise calculate the NDC coordinates of all the points in the 3 axes
			mOriginPosition_NDC = vec3(clipSpace) / clipSpace.w;
			mOriginPosition_NDC.x *= windowAspect;
		
			mat4
				translation = translate(mat4(), *mTargetPosition_world);
		
			vec4 xPosClipSpace = viewProjection * translation * *mTargetOrientation_world * vec4(mSize, 0.0f, 0.0f, 1.0f);
			mXPosition_NDC = vec3(xPosClipSpace) / xPosClipSpace.w;
			mXPosition_NDC.x *= windowAspect;
		
			vec4 yPosClipSpace = viewProjection * translation * *mTargetOrientation_world * vec4(0.0f, mSize, 0.0f, 1.0f);
			mYPosition_NDC = vec3(yPosClipSpace) / yPosClipSpace.w;
			mYPosition_NDC.x *= windowAspect;
		
			vec4 zPosClipSpace = viewProjection * translation * *mTargetOrientation_world * vec4(0.0f, 0.0f, mSize, 1.0f);
			mZPosition_NDC = vec3(zPosClipSpace) / zPosClipSpace.w;
			mZPosition_NDC.x *= windowAspect;
		}
	}

	CoordFrameOverlay::CoordFrameOverlay(GF::ResourceSet& resourceContainer, GF::Graphics::Renderer& renderer) :
		mResourceContainer(resourceContainer),
		mRenderer(renderer)
	{
		load();
	}

	void CoordFrameOverlay::render(glm::mat4 viewProjection, float windowAspect) {
		if (!mCoordFrames.empty()) {
			updateAllCoordFrames(viewProjection, windowAspect);

			mMesh->sendRenderCommand(mRenderer);
			glClear(GL_DEPTH_BUFFER_BIT);
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			mRenderer.flush();
		}
	}

	void CoordFrameOverlay::addCoordFrame(glm::vec3& targetPosition, glm::mat4& targetOrientation, float size) {
		mCoordFrames.push_back(std::make_unique<CoordFrame2D>(targetPosition, targetOrientation, size));
		rebuildBuffers();
	}

	void CoordFrameOverlay::load() {
		using namespace GF::Graphics;

		Shader* shader = mResourceContainer.addShader("coordFrameOverlay2DShader", "res/shaders/coordFrameOverlay2D.vert", "res/shaders/coordFrameOverlay2D.frag");
		shader->addUniform("modelMatrix");
		shader->addUniform("viewMatrix");
		shader->addUniform("projectionMatrix");

		mMesh = mResourceContainer.addMesh("coordFrameOverlayMesh", GL_LINES, nullptr, shader);

		VertexFormat
			posFormat(0, 3, GL_FLOAT),
			colFormat(1, 3, GL_FLOAT);

		mPositionVBO = mResourceContainer.addVertexBuffer(GL_STATIC_DRAW, posFormat, mPositions, "coordFrameOverlay2DPositions");
		mColourVBO = mResourceContainer.addVertexBuffer(GL_STATIC_DRAW, colFormat, mColours, "coordFrameOverlay2DColours");
		mIBO = mResourceContainer.addIndexBuffer(GL_STATIC_DRAW, mIndices, "coordFrameOverlay2DIndices");

		mMesh->addBuffer(mPositionVBO);
		mMesh->addBuffer(mColourVBO);
		mMesh->addIndexBuffer(mIBO);
	}

	void CoordFrameOverlay::updateAllCoordFrames(glm::mat4 viewProjection, float windowAspect) {
		for (const auto& c : mCoordFrames)
			c->recalc(viewProjection, windowAspect);

		updateBuffers();
	}

	void CoordFrameOverlay::rebuildBuffers() {
		mPositions.clear();
		mColours.clear();
		mIndices.clear();

		for (unsigned i = 0; i < mCoordFrames.size(); i++) {
			//Position data
			for (unsigned j = 0; j < 18; j++)
				mPositions.push_back(0.0f);

			//Colour data
			for (int r = 0; r < 2; r++) {
				mColours.push_back(1.0f); //x axis is red
				mColours.push_back(0.0f);
				mColours.push_back(0.0f);
			}
			
			for (int g = 0; g < 2; g++) {
				mColours.push_back(0.0f); //y axis is green
				mColours.push_back(1.0f);
				mColours.push_back(0.0f);
			}

			for (int b = 0; b < 2; b++) {
				mColours.push_back(0.0f); //z axis is blue
				mColours.push_back(0.0f);
				mColours.push_back(1.0f);
			}

			//Index data
			mIndices.push_back(i * 6);
			mIndices.push_back(i * 6 + 1);
			mIndices.push_back(i * 6 + 2);
			mIndices.push_back(i * 6 + 3);
			mIndices.push_back(i * 6 + 4);
			mIndices.push_back(i * 6 + 5);
		}

		mPositionVBO->updateData(mPositions);
		mColourVBO->updateData(mColours);
		mIBO->updateData(mIndices);
	}

	void CoordFrameOverlay::updateBuffers() {
		for (unsigned i = 0; i < mCoordFrames.size(); i++) {
			const CoordFrame2D* c = mCoordFrames[i].get();

			//X axis
			mPositions[i * 18] = c->mOriginPosition_NDC.x;
			mPositions[i * 18 + 1] = c->mOriginPosition_NDC.y;
			mPositions[i * 18 + 2] = c->mOriginPosition_NDC.z;
			mPositions[i * 18 + 3] = c->mXPosition_NDC.x;
			mPositions[i * 18 + 4] = c->mXPosition_NDC.y;
			mPositions[i * 18 + 5] = c->mXPosition_NDC.z;

			//Y axis
			mPositions[i * 18 + 6] = c->mOriginPosition_NDC.x;
			mPositions[i * 18 + 7] = c->mOriginPosition_NDC.y;
			mPositions[i * 18 + 8] = c->mOriginPosition_NDC.z;
			mPositions[i * 18 + 9] = c->mYPosition_NDC.x;
			mPositions[i * 18 + 10] = c->mYPosition_NDC.y;
			mPositions[i * 18 + 11] = c->mYPosition_NDC.z;

			//Zs axis
			mPositions[i * 18 + 12] = c->mOriginPosition_NDC.x;
			mPositions[i * 18 + 13] = c->mOriginPosition_NDC.y;
			mPositions[i * 18 + 14] = c->mOriginPosition_NDC.z;
			mPositions[i * 18 + 15] = c->mZPosition_NDC.x;
			mPositions[i * 18 + 16] = c->mZPosition_NDC.y;
			mPositions[i * 18 + 17] = c->mZPosition_NDC.z;
		}

		mPositionVBO->updateData(mPositions);
	}

}
#endif