#include "Graph.h"
#include <GraphicsFramework/Vendor/ImGui/imgui.h>

namespace Graphics {

	void Graph::update() {
		ImVec2 windowPos_TL = ImGui::GetWindowPos();
		mImGuiPosition_BL = toBottomLeft(WindowSpace::application, glm::vec2(windowPos_TL.x, windowPos_TL.y + ImGui::GetWindowSize().y));
		mImGuiSize = glm::vec2(ImGui::GetWindowSize().x, ImGui::GetWindowSize().y);

		updateGraphRect_imguiSpace();

		mGraphRectSize = abs(mGraphRectMax_imguiSpace - mGraphRectMin_imguiSpace);

		glm::vec2
			clipRectMin = toTopLeft(WindowSpace::application, mImGuiPosition_BL + mGraphRectMin_imguiSpace),
			clipRectMax = toTopLeft(WindowSpace::application, mImGuiPosition_BL + mGraphRectMax_imguiSpace);

		ImGui::PushClipRect(ImVec2(clipRectMin.x, clipRectMax.y), ImVec2(clipRectMax.x, clipRectMin.y), false);
	}

	void Graph::render(std::string imguiWindowName, glm::vec2 appWindowSize) {
		mAppWindowSize = appWindowSize;

		ImGui::Begin(imguiWindowName.c_str());
		ImDrawList* drawList = ImGui::GetWindowDrawList();

		update();

		drawBackground(drawList);
		
		if(!mData.empty()) 
			drawData(drawList);

		ImGui::End();
	}

	void Graph::addDataPoint(glm::vec2 point) {
		////X
		//if (point.x < mLowerLimit.x) 
		//	mLowerLimit.x = point.x;
		//else if (point.x > mUpperLimit.x) 
		//	mUpperLimit.x = point.x;
		//
		////Y
		//if (point.y < mLowerLimit.y) 
		//	mLowerLimit.y = point.y;
		//else if (point.y > mUpperLimit.y) 
		//	mUpperLimit.y = point.y;

		mData.push_back(point);
	}

	void Graph::updateGraphRect_imguiSpace() {
		mGraphRectMin_imguiSpace = glm::vec2(mImGuiWindowPadding);
		mGraphRectMax_imguiSpace = glm::vec2(mImGuiSize.x - mImGuiWindowPadding, mImGuiSize.y - mImGuiWindowPadding - mImGuiTitleBarHeight);
	}

	void Graph::drawBackground(ImDrawList* drawList) const {
		ImU32 colour = ImColor(mBackgroundColour.r, mBackgroundColour.g, mBackgroundColour.b, mBackgroundColour.a);
		
		glm::vec2
			rectMin = toTopLeft(WindowSpace::application, mImGuiPosition_BL + mGraphRectMin_imguiSpace),
			rectMax = toTopLeft(WindowSpace::application, mImGuiPosition_BL + mGraphRectMax_imguiSpace);

		drawList->AddRectFilled(ImVec2(rectMin.x, rectMin.y), ImVec2(rectMax.x, rectMax.y), colour);
	}

	void Graph::drawData(ImDrawList* drawList) const {
		ImU32 colour = ImColor(mDataColour.r, mDataColour.g, mDataColour.b, mDataColour.a);

		glm::vec2
			first,
			second;

		for (unsigned int i = 0; i < mData.size() - 1; i++) {
			first = mImGuiPosition_BL + mGraphRectMin_imguiSpace + ((mData[i] - mLowerLimit) / (mUpperLimit - mLowerLimit)) * mGraphRectSize;
			first = toTopLeft(WindowSpace::application, first);

			second = mImGuiPosition_BL + mGraphRectMin_imguiSpace + ((mData[i + 1] - mLowerLimit) / (mUpperLimit - mLowerLimit)) * mGraphRectSize;
			second = toTopLeft(WindowSpace::application, second);

			drawList->AddLine(ImVec2(first.x, first.y), ImVec2(second.x, second.y), colour);
		}
	}

	glm::vec2 Graph::toBottomLeft(WindowSpace target, glm::vec2 topLeftPoint) const {
		return glm::vec2(topLeftPoint.x, (target == WindowSpace::application ? mAppWindowSize : mImGuiSize).y - topLeftPoint.y);
	}

	glm::vec2 Graph::toTopLeft(WindowSpace target, glm::vec2 bottomLeftPoint) const {
		return glm::vec2(bottomLeftPoint.x, (target == WindowSpace::application ? mAppWindowSize : mImGuiSize).y - bottomLeftPoint.y);
	}

	glm::vec2 Graph::toImGuiSpace_BL(glm::vec2 applicationSpacePoint_BL) const {
		return applicationSpacePoint_BL - mImGuiPosition_BL;
	}
	
	glm::vec2 Graph::toApplicationSpace_BL(glm::vec2 imguiSpacePoint_BL) const {
		return imguiSpacePoint_BL + mImGuiPosition_BL;
	}

}