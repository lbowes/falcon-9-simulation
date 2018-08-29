#ifndef GRAPHICS_GRAPH_H
#define GRAPHICS_GRAPH_H
#pragma once

#include <vector>
#include <glm/vec2.hpp>
#include <glm/vec4.hpp>
#include <glm/geometric.hpp>
#include <string>

//Forward declarations of ImGui structs needed in this class because 
//#including <GraphicsFramework/ImGui/imgui.h> causes multiple-include errors
struct ImVec2;
struct ImDrawList;

namespace Graphics {
	
	class Graph {
	private:
		enum class WindowSpace { application, imgui };
		
		static constexpr float 
			mImGuiWindowPadding = 10.0f,
			mImGuiTitleBarHeight = 22.0f;
		
		glm::vec2
			mAppWindowSize,                //Dimensions of the application window
			mImGuiPosition_BL,             //Position of the ImGui window relative to bottom left of application window
			mImGuiSize,                    //Dimensions of the ImGui window
			mGraphRectSize,                //Dimensions of the graph rect
			mGraphRectMin_imguiSpace,      //Position of the bottom left corner of the graph relative to the bottom left of the ImGui window
			mGraphRectMax_imguiSpace,      //Position of the top right corner of the graph relative to the bottom left of the ImGui window
			mLowerLimit = glm::vec2(0.0f), 
			mUpperLimit = glm::vec2(1.0f);

		glm::vec4
			mBackgroundColour = glm::vec4(0.2f, 0.2f, 0.2f, 1.0f),
			mDataColour = glm::vec4(1.0f);

		std::vector<glm::vec2> mData;

	public:
		Graph() = default;
		~Graph() = default;

		void update();
		void render(std::string imguiWindowName, glm::vec2 appWindowSize);
		void addDataPoint(glm::vec2 point);

		size_t getNumPoints() const { return mData.size(); }
		void setLowerLimit(glm::vec2 limit) { mLowerLimit = limit; }
		void setUpperLimit(glm::vec2 limit) { mUpperLimit = limit; }
		void setBackgroundColour(glm::vec4 colour) { mBackgroundColour = colour; }
		void setDataColour(glm::vec4 colour) { mDataColour = colour; }

	private:
		void updateGraphRect_imguiSpace();
		void drawBackground(ImDrawList* drawList) const;
		void drawData(ImDrawList* drawList) const;

		//These two convert points between bottom left and top left window spaces
		glm::vec2 toBottomLeft(WindowSpace target, glm::vec2 topLeftPoint) const;
		glm::vec2 toTopLeft(WindowSpace target, glm::vec2 bottomLeftPoint) const;
		
		glm::vec2 toImGuiSpace_BL(glm::vec2 point_bottomLeftWindowSpace) const;   //Result is in bottom-left (ImGui) window space
		glm::vec2 toApplicationSpace_BL(glm::vec2 point_imguiWindowSpace) const;  //Result is in bottom-left window space
		
	};

}

#endif
