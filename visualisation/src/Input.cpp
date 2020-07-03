#include "Input.h"

#include <GLFW/glfw3.h>


namespace F9Sim {


const uint16_t Input::s_maxKeys;
const uint8_t Input::s_maxButtons;
bool Input::s_cursorHidden = false;
bool Input::s_zeroMouseDelta = true;
glm::ivec2 Input::s_mousePos_last = {0, 0};
glm::ivec2 Input::s_mousePos = {0, 0};
glm::ivec2 Input::s_mouseDelta = {0, 0};
GLFWwindow* Input::s_window = nullptr;
bool Input::s_keysDown[s_maxKeys];
bool Input::s_keysDown_last[s_maxKeys];
bool Input::s_keysPressed[s_maxKeys];
bool Input::s_keysReleased[s_maxKeys];
bool Input::s_buttonsDown[s_maxButtons];
bool Input::s_buttonsPressed[s_maxButtons];
bool Input::s_buttonsReleased[s_maxButtons];
bool Input::s_buttonsDown_last[s_maxButtons];


static void setCallbacks(GLFWwindow* win);
static void keyCallback(GLFWwindow* win, int key, int scanCode, int action, int mods);
static void buttonCallback(GLFWwindow* win, int button, int scanCode, int action);
static void mousePosCallback(GLFWwindow* win, double x, double y);


void Input::init(GLFWwindow& win) {
    s_window = &win;

    for(uint16_t i = 0; i < s_maxKeys; i++) {
        s_keysDown[i] = false;
        s_keysPressed[i] = false;
        s_keysReleased[i] = false;
        s_keysDown_last[i] = false;
    }

    for(uint8_t i = 0; i < s_maxButtons; i++) {
        s_buttonsDown[i] = false;
        s_buttonsPressed[i] = false;
        s_buttonsReleased[i] = false;
        s_buttonsDown_last[i] = false;
    }

    setCallbacks(s_window);
}


void Input::setCallbacks(GLFWwindow* win) {
    glfwSetKeyCallback(win, keyCallback);
    glfwSetMouseButtonCallback(win, buttonCallback);
    glfwSetCursorPosCallback(win, mousePosCallback);
}


void Input::keyCallback(GLFWwindow* win, int key, int scanCode, int action, int mods) {
    s_keysDown[key] = (key >= 0) && (key < s_maxKeys) && (action == GLFW_PRESS);
}


void Input::buttonCallback(GLFWwindow* win, int button, int action, int mods) {
    s_buttonsDown[button] = (button >= 0) && (button < s_maxButtons) && (action == GLFW_PRESS);
}


void Input::mousePosCallback(GLFWwindow* win, double x, double y) {
    s_mousePos = {x, y};

    if(s_zeroMouseDelta) {
        s_zeroMouseDelta = false;
        s_mousePos_last = s_mousePos;
    }
}


void Input::update() {
    s_mouseDelta = s_mousePos - s_mousePos_last;
    s_mousePos_last = s_mousePos;

    for(uint16_t i = 0; i < s_maxKeys; i++) {
        s_keysPressed[i] = !s_keysDown_last[i] && s_keysDown[i];
        s_keysReleased[i] = s_keysDown_last[i] && !s_keysDown[i];
        s_keysDown_last[i] = s_keysDown[i];
    }

    for(uint8_t i = 0; i < s_maxButtons; i++) {
        s_buttonsPressed[i] = !s_buttonsDown_last[i] && s_buttonsDown[i];
        s_buttonsReleased[i] = s_buttonsDown_last[i] && !s_buttonsDown[i];
        s_buttonsDown_last[i] = s_buttonsDown[i];
    }
}


bool Input::isKeyDown(uint16_t key) {
    return key < s_maxKeys && s_keysDown[key];
}


bool Input::isKeyUp(uint16_t key) {
    return key < s_maxKeys && !s_keysDown[key];
}


bool Input::isKeyPressed(uint16_t key) {
    return key < s_maxKeys && s_keysPressed[key];
}


bool Input::isKeyReleased(uint16_t key) {
    return key < s_maxKeys && s_keysReleased[key];
}


bool Input::isMouseButtonDown(uint8_t button) {
    return button < s_maxButtons && s_buttonsDown[button];
}


bool Input::isMouseButtonUp(uint8_t button) {
    return button < s_maxButtons && !s_buttonsDown[button];
}


bool Input::isMouseButtonPressed(uint8_t button) {
    return button < s_maxButtons && s_buttonsPressed[button];
}


bool Input::isMouseButtonReleased(uint8_t button) {
    return button < s_maxButtons && s_buttonsReleased[button];
}


bool Input::isCursorHidden() {
    return s_cursorHidden;
}


glm::ivec2 Input::getMousePos() {
    return s_mousePos;
}


glm::ivec2 Input::getMouseDelta() {
    return s_mouseDelta;
}


void Input::hideMouseCursor() {
    glfwSetInputMode(s_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    if(s_cursorHidden == false)
        s_zeroMouseDelta = true;

    s_cursorHidden = true;
}


void Input::showMouseCursor() {
    glfwSetInputMode(s_window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

    // If the cursor was visible and has only just been commanded to be hidden, then for this frame
    // the mouse delta should be 0 (otherwise an erroneous delta would be generated when the mouse
    // position changed after it was shown).
    if(s_cursorHidden == true)
        s_zeroMouseDelta = true;

    s_cursorHidden = false;
}


void Input::setCursorPos(glm::ivec2 pos) {
    glfwSetCursorPos(s_window, pos.x, pos.y);
    s_mousePos = pos;
}


} // namespace F9Sim
