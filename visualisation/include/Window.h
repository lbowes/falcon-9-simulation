#ifndef GF_WINDOW_H
#define GF_WINDOW_H
#pragma once

#include "Vendor/stb_image/stb_image.h"

#define GLEW_STATIC
#include <GL/glew.h>

#include <iostream>
#include <GLFW/glfw3.h>
#include <glm/vec4.hpp>
#include <glm/vec2.hpp>

namespace GF {

	class Window {
		friend class Application;
	public:
		static bool mOpenGLInitialised;

	private:
		static constexpr float mDefaultWindowToScreenScale = 0.5f;
		const char* mTitle = "";
		glm::ivec2 mDimensions = glm::vec2(-1, -1);
		GLFWwindow* mWindow = nullptr;
		GLFWmonitor* mMonitor = nullptr;
		glm::vec4 mClearColour = glm::vec4(0.5f, 0.5f, 0.5f, 1.0f);

	public:
		Window(const std::string& title, glm::ivec2 dimensions = glm::ivec2(-1, -1));
		~Window();

		GLFWwindow* getHandle() const { return mWindow; }
		glm::ivec2 getDimensions() const { return mDimensions; }
		float getAspect() const { return static_cast<float>(mDimensions.x) / mDimensions.y; }
		bool isClosed() const { return glfwWindowShouldClose(mWindow) == 1;  }

		void setDimensions(glm::ivec2 dimensions);
		void setClearColour(glm::vec4 newClearColour) { mClearColour = newClearColour; }
		void close() { glfwSetWindowShouldClose(mWindow, GL_TRUE); }
		void setIcon(const std::string& iconPath);

	private:
		bool init();
		glm::ivec2 getMonitorResolution();
		void clear() const;
		void update() const;

	};

}

#endif
