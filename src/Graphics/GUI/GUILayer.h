#ifndef GRAPHICS_GUI_GUILAYER_H
#define GRAPHICS_GUI_GUILAYER_H
#pragma once

namespace Graphics {
	namespace GUI {

		class GUILayer {
		private:

		public:
			GUILayer();
			~GUILayer() = default;

			void render();

		private:
			void loadImGuiStyle();

		};

	}
}

#endif
