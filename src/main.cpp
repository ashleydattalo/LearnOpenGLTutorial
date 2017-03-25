// GLEW
#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <vector>

#include <glm/glm.hpp>

#define SHADER_PATH "/Users/ashleydattalo/graphics/LearnOpenGL/src/Shaders/"

#include "Shader.hpp"

void key_callback(GLFWwindow *window, int key, int scancode, int action, int mode);

const GLuint WIDTH = 800, HEIGHT = 600;

int main()
{
    glfwInit();

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "LearnOpenGL", nullptr, nullptr);    
    if (window == nullptr)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetKeyCallback(window, key_callback);

    glewExperimental = GL_TRUE;

    if (glewInit() != GLEW_OK)
    {
        std::cout << "Failed to initialize GLEW" << std::endl;
        return -1;
    }    

    // Define the viewport dimensions
    int width, height;
    glfwGetFramebufferSize(window, &width, &height);  
    glViewport(0, 0, width, height);

    Shader *shader = new Shader(SHADER_PATH "default.vert", SHADER_PATH "default.frag");

    // GLfloat vertices[] = {
    //      0.5f,  0.5f, 0.0f,  // Top Right
    //      0.5f, -0.5f, 0.0f,  // Bottom Right
    //     -0.5f, -0.5f, 0.0f,  // Bottom Left
    //     -0.5f,  0.5f, 0.0f   // Top Left 
    // };
    GLfloat vertices[] = {
        // Positions         // Colors
         0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,   // Bottom Right
        -0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,   // Bottom Left
         0.0f, 0.5f, 0.0f,  0.0f, 0.0f, 1.0f    // Top 
    };    
    GLuint indices[] = {  // Note that we start from 0!
        0, 1, 2  // First Triangle
        // 1, 2, 3   // Second Triangle
    };

    std::vector<GLfloat> verticesVec;
        verticesVec.push_back(0.5f);
        verticesVec.push_back(0.5f);
        verticesVec.push_back(0.0f);
        
        verticesVec.push_back(0.5f);
        verticesVec.push_back(-0.5f);
        verticesVec.push_back(0.0f);

        verticesVec.push_back(-0.5f);
        verticesVec.push_back(-0.5f);
        verticesVec.push_back(0.0f);

        verticesVec.push_back(-0.5f);
        verticesVec.push_back(0.5f);
        verticesVec.push_back(0.0f);

    // std::cout << sizeof(vertices) << std::endl;
    // std::cout << sizeof(verticesVec) << std::endl;

    // for (int i = 0 ; i < 12; i++) {
    //     std::cout << vertices[i] << "  " << verticesVec[i] << std::endl;
    // }

    GLuint VBO, VAO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    // Bind the Vertex Array Object first, then bind and set vertex buffer(s) and attribute pointer(s).
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // Position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);
    // Color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3* sizeof(GLfloat)));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0); // Note that this is allowed, the call to glVertexAttribPointer registered VBO as the currently bound vertex buffer object so afterwards we can safely unbind

    glBindVertexArray(0); // Unbind VAO (it's always a good thing to unbind any buffer/array to prevent strange bugs), remember: do NOT unbind the EBO, keep it bound to this VAO


    // Uncommenting this call will result in wireframe polygons.
    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        shader->use();
        glUniform1f(glGetUniformLocation(shader->getProg(), "offset"), 0.0f);
        // // Update the uniform color
        // GLfloat timeValue = glfwGetTime();
        // GLfloat greenValue = (glm::sin(timeValue) / 2) + 0.5;
        // GLint vertexColorLocation = glGetUniformLocation(shaderProgram, "ourColor");
        // glUniform4f(vertexColorLocation, 0.0f, greenValue, 0.0f, 1.0f);

        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);

        glfwSwapBuffers(window);
    }
    // Properly de-allocate all resources once they've outlived their purpose
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);

    glfwTerminate();
    return 0;
}

void key_callback(GLFWwindow *window, int key, int scancode, int action, int mode) {
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, GL_TRUE);
    }    
    if (key == GLFW_KEY_SLASH && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, GL_TRUE);
    }
}