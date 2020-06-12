#ifndef F9SIM_INPUT_H_
#define F9SIM_INPUT_H_
#pragma once


#include <GLFW/glfw3.h>
#include <glm/vec2.hpp>
#include <stdint.h>


namespace F9Sim {


void Input_init(GLFWwindow& win);
void Input_update();

// Keyboard
bool Input_isKeyDown(uint16_t key);
bool Input_isKeyUp(uint16_t key);
bool Input_isKeyPressed(uint16_t key);
bool Input_isKeyReleased(uint16_t key);

// Mouse
bool Input_isMouseButtonDown(uint8_t button);
bool Input_isMouseButtonUp(uint8_t button);
bool Input_isMouseButtonPressed(uint8_t button);
bool Input_isMouseButtonReleased(uint8_t button);
bool Input_isCursorHidden();
glm::ivec2 Input_getMousePos();
glm::ivec2 Input_getMouseDelta();
void Input_hideMouseCursor();
void Input_showMouseCursor();
void Input_setCursorPos(glm::ivec2 pos);


} // namespace F9Sim


#endif // F9SIM_INPUT_H_

