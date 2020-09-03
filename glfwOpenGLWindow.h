#pragma once
#ifndef GLFW_OPENGL_WINDOW
#define GLFW_OPENGL_WINDOW

#include <glad/glad.h>
#include <glfw3.h>

#include <string>
#include <iostream>


class glfwOpenGLWindow
{
public:

    glfwOpenGLWindow(
        const unsigned int width,
        const unsigned int height,
        const unsigned int context_major = 3,
        const unsigned int context_minor = 3,
        char* window_title = (char*)"Empty Title"
    )
    {
        resolutionX = width;
        resolutionY = height;
        _context_major = context_major;
        _context_minor = context_minor;
        windowTitle = window_title;
    }

    void init()
    {
        glfwInit();

        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, _context_major);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, _context_minor);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

        // TODO: Check for Mac OS X env and set forward compat hint

        window = glfwCreateWindow(resolutionX, resolutionY, windowTitle, monitor, NULL);
        if (window == NULL)
        {
            std::cout << "ERROR::WINDOW::GLFW::CREATE_FAILED" << std::endl;
            glfwTerminate();
        }
        glfwMakeContextCurrent(window);

        /* Initialize GLAD to get function pointers */

        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
        {
            std::cout << "ERROR::GLAD::INIT_FAILED" << std::endl;
        }

        glViewport(0, 0, resolutionX, resolutionY);
        glfwSetFramebufferSizeCallback(window, &glfwOpenGLWindow::_framebuffer_size_callback);
    }

    bool shouldClose()
    {
        return glfwWindowShouldClose(window);
    }

    static void _framebuffer_size_callback(GLFWwindow* window, int width, int height)
    {
        glViewport(0, 0, width, height);
    }

    /* TEMP function until confirmed that main thread does not need access to window identifier */
    GLFWwindow* getWindow()
    {
        return window;
    }


private:
    GLFWmonitor* monitor;
    GLFWwindow* window;

    unsigned int _context_major;
    unsigned int _context_minor;

    unsigned int resolutionX;
    unsigned int resolutionY;

    char* windowTitle;
};

#endif
