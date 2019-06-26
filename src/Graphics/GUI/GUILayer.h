#ifndef GRAPHICS_GUI_GUILAYER_H
#define GRAPHICS_GUI_GUILAYER_H
#pragma once

namespace Graphics {
	struct PlaybackConfig;
}

namespace Graphics {
	namespace GUI {

		class GUILayer {
		private:
			PlaybackConfig& mPlaybackHandle;
			const double mSimDuration_s;

		public:
			GUILayer(PlaybackConfig& playbackConfigHandle, double simDuration_s);
			~GUILayer() = default;

			void render();
			void loadImGuiStyle();

		private:
			void playbackControlPanel();

		};

	}
}

#endif // GRAPHICS_GUI_GUILAYER_H
