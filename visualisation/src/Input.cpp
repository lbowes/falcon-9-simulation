#include "Input.h"

#include <GLFW/glfw3.h>
// temp
#include <iostream>
//


namespace F9Sim {


static const uint16_t MAX_KEYS = 512;
static const uint8_t MAX_BUTTONS = 16;
static bool cursorHidden = false;
static bool keysDown[MAX_KEYS];
static bool keysDown_last[MAX_KEYS];
static bool keysPressed[MAX_KEYS];
static bool keysReleased[MAX_KEYS];
static bool buttonsDown[MAX_BUTTONS];
static bool buttonsPressed[MAX_BUTTONS];
static bool buttonsReleased[MAX_BUTTONS];
static bool buttonsDown_last[MAX_BUTTONS];
static glm::ivec2 mousePos_last = {0, 0};
static glm::ivec2 mousePos = {0, 0};
static glm::ivec2 mouseDelta = {0, 0};
static GLFWwindow* window = nullptr;


static void setCallbacks();
static void keyCallback(GLFWwindow* win, int key, int scanCode, int action, int mods);
static void buttonCallback(GLFWwindow* win, int button, int scanCode, int action);
static void mousePosCallback(GLFWwindow* win, double x, double y);


void Input_init(GLFWwindow& win) {
    window = &win;

    for(uint16_t i = 0; i < MAX_KEYS; i++) {
        keysDown[i] = false;
        keysPressed[i] = false;
        keysReleased[i] = false;
        keysDown_last[i] = false;
    }

    for(uint8_t i = 0; i < MAX_BUTTONS; i++) {
        buttonsDown[i] = false;
        buttonsPressed[i] = false;
        buttonsReleased[i] = false;
        buttonsDown_last[i] = false;
    }

    setCallbacks();
}


static void setCallbacks() {
    glfwSetKeyCallback(window, keyCallback);
    glfwSetMouseButtonCallback(window, buttonCallback);
    glfwSetCursorPosCallback(window, mousePosCallback);
}


static void keyCallback(GLFWwindow* win, int key, int scanCode, int action, int mods) {
    keysDown[key] = (key >= 0) && (key < MAX_KEYS) && (action == GLFW_PRESS);
}


static void buttonCallback(GLFWwindow* win, int button, int action, int mods) {
    buttonsDown[button] = (button >= 0) && (button < MAX_BUTTONS) && (action == GLFW_PRESS);
}


static void mousePosCallback(GLFWwindow* win, double x, double y) {
    mousePos = {x, y};

    static bool firstCall = true;
    if(firstCall) {
        firstCall = false;
        mousePos_last = mousePos;
    }
}


void Input_update() {
    mouseDelta = mousePos - mousePos_last;
    mousePos_last = mousePos;

    for(uint16_t i = 0; i < MAX_KEYS; i++) {
        keysPressed[i] = !keysDown_last[i] && keysDown[i];
        keysReleased[i] = keysDown_last[i] && !keysDown[i];
        keysDown_last[i] = keysDown[i];
    }

    for(uint8_t i = 0; i < MAX_BUTTONS; i++) {
        buttonsPressed[i] = !buttonsDown_last[i] && buttonsDown[i];
        buttonsReleased[i] = buttonsDown_last[i] && !buttonsDown[i];
        buttonsDown_last[i] = buttonsDown[i];
    }
}


bool Input_isKeyDown(uint16_t key) {
    return key < MAX_KEYS && keysDown[key];
}


bool Input_isKeyUp(uint16_t key) {
    return key < MAX_KEYS && !keysDown[key];
}


bool Input_isKeyPressed(uint16_t key) {
    return key < MAX_KEYS && keysPressed[key];
}


bool Input_isKeyReleased(uint16_t key) {
    return key < MAX_KEYS && keysReleased[key];
}


bool Input_isMouseButtonDown(uint8_t button) {
    return button < MAX_BUTTONS && buttonsDown[button];
}


bool Input_isMouseButtonUp(uint8_t button) {
    return button < MAX_BUTTONS && !buttonsDown[button];
}


bool Input_isMouseButtonPressed(uint8_t button) {
    return button < MAX_BUTTONS && buttonsPressed[button];
}


bool Input_isMouseButtonReleased(uint8_t button) {
    return button < MAX_BUTTONS && buttonsReleased[button];
}


bool Input_isCursorHidden() {
    return cursorHidden;
}


glm::ivec2 Input_getMousePos() {
    return mousePos;
}


glm::ivec2 Input_getMouseDelta() {
    return mouseDelta;
}


void Input_hideMouseCursor() {
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    cursorHidden = true;
}


void Input_showMouseCursor() {
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    cursorHidden = false;
}


void Input_setCursorPos(glm::ivec2 pos) {
    glfwSetCursorPos(window, pos.x, pos.y);
    mousePos = pos;
}


} // namespace F9Sim
