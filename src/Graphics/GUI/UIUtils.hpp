#ifndef GRAPHICS_GUI_UIUTILS_HPP
#define GRAPHICS_GUI_UIUTILS_HPP
#pragma once

#include <IMGUI/imgui.h>

namespace Graphics {

    static void fixWinInViewport_imgui() {
		const ImVec2
            winDims_scr = ImGui::GetIO().DisplaySize,
			imguiWinPos_scr = ImGui::GetWindowPos(),
			imguiWinDims_scr = ImGui::GetWindowSize();

		if(imguiWinPos_scr.x + imguiWinDims_scr.x > winDims_scr.x)
			ImGui::SetWindowPos(ImVec2(winDims_scr.x - imguiWinDims_scr.x, imguiWinPos_scr.y));
		else if(imguiWinPos_scr.x < 0)
			ImGui::SetWindowPos(ImVec2(0, imguiWinPos_scr.y));

		if(imguiWinPos_scr.y + imguiWinDims_scr.y > winDims_scr.y)
			ImGui::SetWindowPos(ImVec2(imguiWinPos_scr.x, winDims_scr.y - imguiWinDims_scr.y));
		else if(imguiWinPos_scr.y < 0)
			ImGui::SetWindowPos(ImVec2(imguiWinPos_scr.x, 0));
	}

}

#endif // GRAPHICS_GUI_UIUTILS_HPP

