#include "Event.h"
#include <iostream>

Event::Event(GLFWwindow &window) : window{window}
{
    glfwSetKeyCallback(&window, Event::keyPressedCallback);
    glfwSetCursorPosCallback(&window, Event::cursorPositionCallback);
    if (glfwRawMouseMotionSupported())
    {
        glfwSetInputMode(&window, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);
        std::cout << "RawInput" << std::endl;
    }
}

void Event::keyPressedCallback(GLFWwindow *window, int key, int scancode, int action, int mods)
{

    if (action == GLFW_REPEAT)
        return;
    Event *event = static_cast<Event *>(glfwGetWindowUserPointer(window));
    if (!event)
        return;

    event->lastKeyPressedInfo.key = key;
    event->lastKeyPressedInfo.action = action;

    event->keyDown[key] = (action == GLFW_PRESS);
}
void Event::cursorPositionCallback(GLFWwindow *window, double xpos, double ypos)
{
    Event *event = static_cast<Event *>(glfwGetWindowUserPointer(window));
    if (!event)
        return;

    // std::cout << "MouseCB" << time(0) << std::endl;

    event->mouseX = xpos;
    event->mouseY = ypos;
}
// TODO cursor callback this wont work
void Event::eventLoop()
{
    glfwPollEvents();
}

bool Event::getKeyPressed(int key)
{
    return keyDown[key];
}

Event::~Event()
{
}
