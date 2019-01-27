#include "GUILayer.h"
#include <IMGUI/imgui.h>

namespace Graphics {
	namespace GUI {

		GUILayer::GUILayer() {
			loadImGuiStyle();
		}

		void GUILayer::render() {
			ImGui::Begin("test window");
			ImGui::Text("Hello world!");
			ImGui::End();
		}

		void GUILayer::loadImGuiStyle() {
			using namespace ImGui;
		
			//Style
			ImGuiStyle* s = &ImGui::GetStyle();
			s->FrameBorderSize = 1.0f;
			s->FramePadding = {4.0f,2.0f};
			s->ItemSpacing = {8.0f,2.0f};
			s->WindowBorderSize = 1.0f;
			s->TabBorderSize = 1.0f;
			s->WindowRounding = 1.0f;
			s->ChildRounding = 1.0f;
			s->FrameRounding = 1.0f;
			s->ScrollbarRounding = 1.0f;
			s->GrabRounding = 1.0f;
			s->TabRounding = 1.0f;

			//Colours
    		ImVec4* colors = s->Colors;
			colors[ImGuiCol_Text] =                  {1.00f, 1.00f, 1.00f, 0.95f};
			colors[ImGuiCol_TextDisabled] =          {0.50f, 0.50f, 0.50f, 1.00f};
			colors[ImGuiCol_WindowBg] =              {0.13f, 0.12f, 0.12f, 0.60f};
			colors[ImGuiCol_ChildBg] =               {1.00f, 1.00f, 1.00f, 0.00f};
			colors[ImGuiCol_PopupBg] =               {0.05f, 0.05f, 0.05f, 0.94f};
			colors[ImGuiCol_Border] =                {0.53f, 0.53f, 0.53f, 0.46f};
			colors[ImGuiCol_BorderShadow] =          {0.00f, 0.00f, 0.00f, 0.00f};
			colors[ImGuiCol_FrameBg] =               {0.00f, 0.00f, 0.00f, 0.85f};
			colors[ImGuiCol_FrameBgHovered] =        {0.22f, 0.22f, 0.22f, 0.40f};
			colors[ImGuiCol_FrameBgActive] =         {0.16f, 0.16f, 0.16f, 0.53f};
			colors[ImGuiCol_TitleBg] =               {0.00f, 0.00f, 0.00f, 1.00f};
			colors[ImGuiCol_TitleBgActive] =         {0.00f, 0.00f, 0.00f, 1.00f};
			colors[ImGuiCol_TitleBgCollapsed] =      {0.00f, 0.00f, 0.00f, 0.51f};
			colors[ImGuiCol_MenuBarBg] =             {0.12f, 0.12f, 0.12f, 1.00f};
			colors[ImGuiCol_ScrollbarBg] =           {0.02f, 0.02f, 0.02f, 0.53f};
			colors[ImGuiCol_ScrollbarGrab] =         {0.31f, 0.31f, 0.31f, 1.00f};
			colors[ImGuiCol_ScrollbarGrabHovered] =  {0.41f, 0.41f, 0.41f, 1.00f};
			colors[ImGuiCol_ScrollbarGrabActive] =   {0.48f, 0.48f, 0.48f, 1.00f};
			colors[ImGuiCol_CheckMark] =             {0.79f, 0.79f, 0.79f, 1.00f};
			colors[ImGuiCol_SliderGrab] =            {0.48f, 0.47f, 0.47f, 0.91f};
			colors[ImGuiCol_SliderGrabActive] =      {0.56f, 0.55f, 0.55f, 0.62f};
			colors[ImGuiCol_Button] =                {0.50f, 0.50f, 0.50f, 0.63f};
			colors[ImGuiCol_ButtonHovered] =         {0.67f, 0.67f, 0.68f, 0.63f};
			colors[ImGuiCol_ButtonActive] =          {0.26f, 0.26f, 0.26f, 0.63f};
			colors[ImGuiCol_Header] =                {0.54f, 0.54f, 0.54f, 0.58f};
			colors[ImGuiCol_HeaderHovered] =         {0.64f, 0.65f, 0.65f, 0.80f};
			colors[ImGuiCol_HeaderActive] =          {0.25f, 0.25f, 0.25f, 0.80f};
			colors[ImGuiCol_Separator] =             {0.58f, 0.58f, 0.58f, 0.50f};
			colors[ImGuiCol_SeparatorHovered] =      {0.81f, 0.81f, 0.81f, 0.64f};
			colors[ImGuiCol_SeparatorActive] =       {0.81f, 0.81f, 0.81f, 0.64f};
			colors[ImGuiCol_ResizeGrip] =            {0.87f, 0.87f, 0.87f, 0.53f};
			colors[ImGuiCol_ResizeGripHovered] =     {0.87f, 0.87f, 0.87f, 0.74f};
			colors[ImGuiCol_ResizeGripActive] =      {0.87f, 0.87f, 0.87f, 0.74f};
			colors[ImGuiCol_Tab] =                   {0.01f, 0.01f, 0.01f, 0.86f};
			colors[ImGuiCol_TabHovered] =            {0.29f, 0.29f, 0.29f, 1.00f};
			colors[ImGuiCol_TabActive] =             {0.31f, 0.31f, 0.31f, 1.00f};
			colors[ImGuiCol_TabUnfocused] =          {0.02f, 0.02f, 0.02f, 1.00f};
			colors[ImGuiCol_TabUnfocusedActive] =    {0.19f, 0.19f, 0.19f, 1.00f};
			colors[ImGuiCol_PlotLines] =             {0.61f, 0.61f, 0.61f, 1.00f};
			colors[ImGuiCol_PlotLinesHovered] =      {0.68f, 0.68f, 0.68f, 1.00f};
			colors[ImGuiCol_PlotHistogram] =         {0.90f, 0.77f, 0.33f, 1.00f};
			colors[ImGuiCol_PlotHistogramHovered] =  {0.87f, 0.55f, 0.08f, 1.00f};
			colors[ImGuiCol_TextSelectedBg] =        {0.47f, 0.60f, 0.76f, 0.47f};
			colors[ImGuiCol_DragDropTarget] =        {0.58f, 0.58f, 0.58f, 0.90f};
			colors[ImGuiCol_NavHighlight] =          {0.60f, 0.60f, 0.60f, 1.00f};
			colors[ImGuiCol_NavWindowingHighlight] = {1.00f, 1.00f, 1.00f, 0.70f};
			colors[ImGuiCol_NavWindowingDimBg] =     {0.80f, 0.80f, 0.80f, 0.20f};
			colors[ImGuiCol_ModalWindowDimBg] =      {0.80f, 0.80f, 0.80f, 0.35f};
		}

	}
}