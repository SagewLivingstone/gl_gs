#include <glad/glad.h>
#include <glfw3.h>
#include <iostream>

#include "Shader.h"
#include "glfwOpenGLWindow.h"


// const string shader -- change later to separate file !!!
const char* vertexShaderSource = "\
#version 330 core \n\
layout (location = 0) in vec3 vertex_position; \n\
layout (location = 1) in vec3 vertex_color; \n\
\n\
out vec3 vertexColor; \n\
\n\
void main () {\n\
  vertexColor = vertex_color; \n\
  gl_Position = vec4(vertex_position.x, vertex_position.y, vertex_position.z, 1.0);\n\
}\n\
\0\
";

const char* fragmentShaderSource = "\
#version 330 core\n\
in vec3 vertexColor; \n\
\n\
uniform vec4 customColor; \n\
\n\
out vec4 FragColor;\n\
\n\
void main() \n\
{ \n\
  FragColor = vec4(customColor); \n\
} \n\
\0\
";

float tri_vertices[] = {
     0.5f,  0.5f,  0.0f, // Top Right
     0.5f, -0.5f,  0.0f, // Bot Right
    -0.5f, -0.5f,  0.0f, // Bot Left
    -0.5f,  0.5f,  0.0f, // Top Left
};

float tri_colors[] = {
     1.0f,  0.0f,  0.0f,
     0.0f,  1.0f,  0.0f,
     0.0f,  0.0f,  1.0f,
     0.3f,  0.3f,  0.3f,
};

unsigned int indices[] = {
    0, 1, 3,
    1, 2, 3,
};

// Callback on window resize to register to OpenGL Viewport object
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}


// Called each frame to poll OpenGL input
void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS)
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS)
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
}


int main()
{
    glfwOpenGLWindow window(800, 600);
    window.init();

    Shader testShader("shaders/vertex/test.vs", "shaders/fragment/test.fs");

    /* Buffer instancing */
    unsigned int VAO, VBO, CBO, EBO;
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &CBO);
    glGenBuffers(1, &EBO);
    glGenVertexArrays(1, &VAO);
    
    /* Render Initialization */
    glBindVertexArray(VAO);

    // Copy vertex data into a buffer for opengl to use
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(tri_vertices), tri_vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glBindBuffer(GL_ARRAY_BUFFER, CBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(tri_colors), tri_colors, GL_STATIC_DRAW);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    // Copy element data (indices) into a buffer for opengl to use
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    // Set vertex attribute pointers and enable
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);

    /* Begin Main Loop */
    while (!window.shouldClose())
    {
        /* Input handling */
        processInput(window.getWindow()); // Should this be post-frame to minimize input-delay?

        // Clear frame
        glClearColor(0.2f, 0.3f, 0.33f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // Color
        float timeValue = (float)glfwGetTime();
        float greenValue = (sin(timeValue) / 2.0f) + 0.5f;

        /* Rendering */
        testShader.use();
        testShader.setFloat("greenValue", greenValue);
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        /* Render and update */
        glfwSwapBuffers(window.getWindow());
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}