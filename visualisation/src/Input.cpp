#include "Input.h"

#include <GLFW/glfw3.h>


namespace F9Sim {


const uint16_t Input::s_maxKeys;
const uint8_t Input::s_maxButtons;
bool Input::m_cursorHidden = false;
glm::ivec2 Input::m_mousePos_last = {0, 0};
glm::ivec2 Input::m_mousePos = {0, 0};
glm::ivec2 Input::m_mouseDelta = {0, 0};
GLFWwindow* Input::m_window = nullptr;
bool Input::m_keysDown[s_maxKeys];
bool Input::m_keysDown_last[s_maxKeys];
bool Input::m_keysPressed[s_maxKeys];
bool Input::m_keysReleased[s_maxKeys];
bool Input::m_buttonsDown[s_maxButtons];
bool Input::m_buttonsPressed[s_maxButtons];
bool Input::m_buttonsReleased[s_maxButtons];
bool Input::m_buttonsDown_last[s_maxButtons];


static void setCallbacks(GLFWwindow* win);
static void keyCallback(GLFWwindow* win, int key, int scanCode, int action, int mods);
static void buttonCallback(GLFWwindow* win, int button, int scanCode, int action);
static void mousePosCallback(GLFWwindow* win, double x, double y);


void Input::init(GLFWwindow& win) {
    m_window = &win;

    for(uint16_t i = 0; i < s_maxKeys; i++) {
        m_keysDown[i] = false;
        m_keysPressed[i] = false;
        m_keysReleased[i] = false;
        m_keysDown_last[i] = false;
    }

    for(uint8_t i = 0; i < s_maxButtons; i++) {
        m_buttonsDown[i] = false;
        m_buttonsPressed[i] = false;
        m_buttonsReleased[i] = false;
        m_buttonsDown_last[i] = false;
    }

    setCallbacks(m_window);
}


void Input::setCallbacks(GLFWwindow* win) {
    glfwSetKeyCallback(win, keyCallback);
    glfwSetMouseButtonCallback(win, buttonCallback);
    glfwSetCursorPosCallback(win, mousePosCallback);
}


void Input::keyCallback(GLFWwindow* win, int key, int scanCode, int action, int mods) {
    m_keysDown[key] = (key >= 0) && (key < s_maxKeys) && (action == GLFW_PRESS);
}


void Input::buttonCallback(GLFWwindow* win, int button, int action, int mods) {
    m_buttonsDown[button] = (button >= 0) && (button < s_maxButtons) && (action == GLFW_PRESS);
}


void Input::mousePosCallback(GLFWwindow* win, double x, double y) {
    m_mousePos = {x, y};

    static bool firstCall = true;
    if(firstCall) {
        firstCall = false;
        m_mousePos_last = m_mousePos;
    }
}


void Input::update() {
    m_mouseDelta = m_mousePos - m_mousePos_last;
    m_mousePos_last = m_mousePos;

    for(uint16_t i = 0; i < s_maxKeys; i++) {
        m_keysPressed[i] = !m_keysDown_last[i] && m_keysDown[i];
        m_keysReleased[i] = m_keysDown_last[i] && !m_keysDown[i];
        m_keysDown_last[i] = m_keysDown[i];
    }

    for(uint8_t i = 0; i < s_maxButtons; i++) {
        m_buttonsPressed[i] = !m_buttonsDown_last[i] && m_buttonsDown[i];
        m_buttonsReleased[i] = m_buttonsDown_last[i] && !m_buttonsDown[i];
        m_buttonsDown_last[i] = m_buttonsDown[i];
    }
}


bool Input::isKeyDown(uint16_t key) {
    return key < s_maxKeys && m_keysDown[key];
}


bool Input::isKeyUp(uint16_t key) {
    return key < s_maxKeys && !m_keysDown[key];
}


bool Input::isKeyPressed(uint16_t key) {
    return key < s_maxKeys && m_keysPressed[key];
}


bool Input::isKeyReleased(uint16_t key) {
    return key < s_maxKeys && m_keysReleased[key];
}


bool Input::isMouseButtonDown(uint8_t button) {
    return button < s_maxButtons && m_buttonsDown[button];
}


bool Input::isMouseButtonUp(uint8_t button) {
    return button < s_maxButtons && !m_buttonsDown[button];
}


bool Input::isMouseButtonPressed(uint8_t button) {
    return button < s_maxButtons && m_buttonsPressed[button];
}


bool Input::isMouseButtonReleased(uint8_t button) {
    return button < s_maxButtons && m_buttonsReleased[button];
}


bool Input::isCursorHidden() {
    return m_cursorHidden;
}


glm::ivec2 Input::getMousePos() {
    return m_mousePos;
}


glm::ivec2 Input::getMouseDelta() {
    return m_mouseDelta;
}


void Input::hideMouseCursor() {
    glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    m_cursorHidden = true;
}


void Input::showMouseCursor() {
    glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    m_cursorHidden = false;
}


void Input::setCursorPos(glm::ivec2 pos) {
    glfwSetCursorPos(m_window, pos.x, pos.y);
    m_mousePos = pos;
}


} // namespace F9Sim
