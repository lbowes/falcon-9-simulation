#include "Marker2DOverlay.h"

#include <GraphicsFramework/Vendor/ImGui/imgui.h>
#include <GraphicsFramework/Input.h>

namespace Graphics {
	
	Marker2DOverlay::Marker2DOverlay(GF::ResourceSet& resourceContainer, GF::Graphics::Renderer& renderer) :
		mResourceContainer(resourceContainer),
		mRenderer(renderer)
	{ 
		load();
	}

	void Marker2DOverlay::render(glm::mat4 viewProjection, float windowAspect, glm::vec2 windowDimensions) {
		if (!mMarkers.empty()){
			updateAllMarkers(viewProjection, windowAspect);

			mMesh->sendRenderCommand(mRenderer);
			glClear(GL_DEPTH_BUFFER_BIT);
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			mRenderer.flush();

			checkCursorOverMarkers(windowAspect, windowDimensions);
		}
	}

	void Marker2DOverlay::addMarker(glm::vec3& target, Marker2D::Type type, const std::string& name) {
		mMarkers.push_back({ &target, type, name });
		rebuildBuffers();
	}

	void Marker2DOverlay::load() {
		using namespace GF::Graphics;

		Shader* shader = mResourceContainer.addShader("markerOverlay2DShader", "res/shaders/marker2DOverlay.vert", "res/shaders/marker2DOverlay.frag");
		shader->addUniform("modelMatrix");
		shader->addUniform("viewMatrix");
		shader->addUniform("projectionMatrix");

		Texture2D* texture = mResourceContainer.addTexture2D("markersTexture", "res/textures/markers.png");

		mMesh = mResourceContainer.addMesh("markerOverlayMesh", GL_TRIANGLES, texture, shader);

		VertexFormat
			vertPosFormat(0, 3, GL_FLOAT, false),
			vertTexCoordFormat(1, 2, GL_FLOAT, false);

		mPositionVBO = mResourceContainer.addVertexBuffer("marker2DOverlayPositions", GL_STATIC_DRAW, vertPosFormat, mPositions);
		mTexCoordVBO = mResourceContainer.addVertexBuffer("marker2DOverlayTexCoords", GL_STATIC_DRAW, vertTexCoordFormat, mTexCoords);
		mIBO = mResourceContainer.addIndexBuffer("marker2DOverlayIndices", GL_STATIC_DRAW, mIndices);

		mMesh->addBuffer(mPositionVBO);
		mMesh->addBuffer(mTexCoordVBO);
		mMesh->addIndexBuffer(mIBO);
	}

	void Marker2DOverlay::updateAllMarkers(glm::mat4 viewProjection, float windowAspect) {
		for (Marker2D& m : mMarkers)
			m.recalc(viewProjection, windowAspect);

		updateBuffers();
	}

	void Marker2DOverlay::rebuildBuffers() {
		mPositions.clear();
		mTexCoords.clear();
		mIndices.clear();

		for (unsigned i = 0; i < mMarkers.size(); i++) {
			//Position data
			for (int i = 0; i < 12; i++)
				mPositions.push_back(0.0f);

			if (mMarkers[i].mType == Marker2D::Type::centreMass) {
				//Texture coordinate data for origin symbol
				mTexCoords.push_back(0.0f);
				mTexCoords.push_back(0.0f);
				mTexCoords.push_back(0.0f);
				mTexCoords.push_back(0.5f);
				mTexCoords.push_back(0.5f);
				mTexCoords.push_back(0.5f);
				mTexCoords.push_back(0.5f);
				mTexCoords.push_back(0.0f);
			}
			else if (mMarkers[i].mType == Marker2D::Type::origin) {
				//Texture coordinate data for centre of mass symbol
				mTexCoords.push_back(0.5f);
				mTexCoords.push_back(0.0f);
				mTexCoords.push_back(0.5f);
				mTexCoords.push_back(0.5f);
				mTexCoords.push_back(1.0f);
				mTexCoords.push_back(0.5f);
				mTexCoords.push_back(1.0f);
				mTexCoords.push_back(0.0f);
			}
			else {
				for (int i = 0; i < 8; i++)
					mTexCoords.push_back(0.0f);
			}

			//Index data
			mIndices.push_back(i * 4);
			mIndices.push_back(i * 4 + 1);
			mIndices.push_back(i * 4 + 2);
			mIndices.push_back(i * 4 + 2);
			mIndices.push_back(i * 4 + 3);
			mIndices.push_back(i * 4);
		}

		mPositionVBO->updateData(mPositions);
		mTexCoordVBO->updateData(mTexCoords);
		mIBO->updateData(mIndices);
	}

	void Marker2DOverlay::updateBuffers() {
		for (unsigned i = 0; i < mMarkers.size(); i++) {

			//v1
			mPositions[i * 12] = mMarkers[i].mResult_NDC.x - Marker2D::mRadius;
			mPositions[i * 12 + 1] = mMarkers[i].mResult_NDC.y - Marker2D::mRadius;
			mPositions[i * 12 + 2] = mMarkers[i].mResult_NDC.z;

			//v2
			mPositions[i * 12 + 3] = mMarkers[i].mResult_NDC.x - Marker2D::mRadius;
			mPositions[i * 12 + 4] = mMarkers[i].mResult_NDC.y + Marker2D::mRadius;
			mPositions[i * 12 + 5] = mMarkers[i].mResult_NDC.z;

			//v3
			mPositions[i * 12 + 6] = mMarkers[i].mResult_NDC.x + Marker2D::mRadius;
			mPositions[i * 12 + 7] = mMarkers[i].mResult_NDC.y + Marker2D::mRadius;
			mPositions[i * 12 + 8] = mMarkers[i].mResult_NDC.z;

			//v4
			mPositions[i * 12 + 9] = mMarkers[i].mResult_NDC.x + Marker2D::mRadius;
			mPositions[i * 12 + 10] = mMarkers[i].mResult_NDC.y - Marker2D::mRadius;
			mPositions[i * 12 + 11] = mMarkers[i].mResult_NDC.z;
		}

		mPositionVBO->updateData(mPositions);
	}

	void Marker2DOverlay::checkCursorOverMarkers(float windowAspect, glm::vec2 windowDimensions) {
		glm::vec2 NDCMouseCoords = glm::vec2(GF::Input::getMousePosition().x / windowDimensions.x, GF::Input::getMousePosition().y / windowDimensions.y);
		NDCMouseCoords.y = 1.0f - NDCMouseCoords.y;
		NDCMouseCoords = glm::vec2((NDCMouseCoords.x - 0.5f) * 2.0f, (NDCMouseCoords.y - 0.5f) * 2.0f);
		NDCMouseCoords.x *= windowAspect;

		for (Marker2D& m : mMarkers) {
			if (glm::length(NDCMouseCoords - glm::vec2(m.mResult_NDC)) < Marker2D::mRadius && !GF::Input::isCursorHidden()) {
				ImVec2 mousePos = ImGui::GetMousePos();
				ImGui::SetNextWindowPos(ImVec2(mousePos.x + 20, mousePos.y));

				ImGuiWindowFlags windowFlags = ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_AlwaysAutoResize;
				if (m.mName == "")
					windowFlags |= ImGuiWindowFlags_NoTitleBar;

				ImGui::Begin(m.mName.c_str(), 0, windowFlags);
				ImGui::Value("x", m.mTarget_world->x); ImGui::SameLine();
				ImGui::Value("y", m.mTarget_world->y); ImGui::SameLine();
				ImGui::Value("z", m.mTarget_world->z); ImGui::SameLine();
				ImGui::End();
				break;
			}
		}
	}

}