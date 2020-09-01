#include <glad/glad.h>
#include <glfw3.h>
#include <iostream>


// const string shader -- change later to separate file !!!
const char* vertexShaderSource = "\
#version 330 core \n\
layout (location = 0) in vec3 vertex_position; \n\
layout (location = 1) in vec3 vertex_color; \n\
\n\
out vec3 color; \n\
\n\
void main () {\n\
  color = vertex_color; \n\
  gl_Position = vec4(vertex_position.x, vertex_position.y, vertex_position.z, 1.0);\n\
}\n\
\0\
";

const char* fragmentShaderSource = "\
#version 330 core\n\
in vec3 color; \n\
\n\
out vec4 FragColor;\n\
\n\
void main() \n\
{ \n\
  FragColor = vec4(color, 1.0f); \n\
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
    glfwInit();
    
    // Set window hints
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Needed for backwards compat on Mac OS X -- not using now...
    //glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);


    /* Create GLFW Window Object */

    GLFWmonitor* monitor = NULL;
    GLFWwindow* window = glfwCreateWindow(800, 600, "Window Title", monitor, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window); // Not sure why we need this?
    // Note: i think a lot of this could be encapsulated into a "glfwOpenGLWindow" class with init functions


    /* Initialize GLAD to get function pointers */

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD!" << std::endl;
        return -1;
    }

    /* Setting up handling viewport size */
    glViewport(0, 0, 800, 600);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);


    /* Shader Instancing */
    unsigned int vertexShader, fragmentShader;
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

    // Debug holders
    int success;
    char infolog[512];

    /* Compile Shaders */
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);
    // Check for issues compiling
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(vertexShader, 512, NULL, infolog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infolog << std::endl;
    }

    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);
    // Check for issues compiling
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infolog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infolog << std::endl;
    }

    /* Shader Program Instancing */
    unsigned int shaderProgram;
    shaderProgram = glCreateProgram();

    // Link the two shaders together into a shader program
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    // Check for issues linking
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infolog);
        std::cout << "ERROR::SHADER_PROGRAM::LINKING::LINKING_FAILED\n" << infolog << std::endl;
    }
    // Cleanup - we don't need shader obj anymore
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);


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
    while (!glfwWindowShouldClose(window))
    {
        /* Input handling */
        processInput(window); // Should this be post-frame to minimize input-delay?

        // Clear frame
        glClearColor(0.2f, 0.3f, 0.33f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        /* Rendering */
        glUseProgram(shaderProgram);
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0); // Unbind once we're done

        /* Render and update */
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}