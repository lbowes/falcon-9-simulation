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
			const double mSimDuration;

		public:
			GUILayer(PlaybackConfig& playbackConfigHandle, double simDuration);
			~GUILayer() = default;

			void render();

		private:
			void loadImGuiStyle();
			void playbackControlPanel();

		};

	}
}

#endif
