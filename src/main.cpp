// C++ standard libraries
#include <iostream>
#include <vector>

// 3rd-party libraries
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

void framebufferSizeCallback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

const char* vtxSrc = ""
"#version 330 core\n"
"layout (location = 0) in vec3 position;\n"
"layout (location = 1) in vec3 color;\n"
"out vec3 vColor;\n"
"void main() {\n"
"   gl_Position = vec4(position, 1.0);\n"
"   vColor = color;"
"}\n";

const char* frgSrc = ""
"#version 330 core\n"
"in vec3 vColor;\n"
"out vec4 fColor;\n"
"void main() {\n"
"   fColor = vec4(vColor, 1.0);\n"
"}\n";

struct Vertex {
    glm::vec3 position;
    glm::vec3 color;
};

int main() {
    // Graphics libraries initialization
    // ----------------------------------
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW." << std::endl;
        return -1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(800, 600, "Hello, triangle!", NULL, NULL);

    if (!window) {
        std::cerr << "Failed to initialize GLFW window." << std::endl;
        return -1;
    }

    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "Failed to initialize GLAD." << std::endl;
        return -1;
    }

    // Shader setup
    // -------------
    unsigned int vtxID, frgID;
    vtxID = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vtxID, 1, &vtxSrc, NULL);
    glCompileShader(vtxID);

    int success;
    char infoLog[512];
    glGetShaderiv(vtxID, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(vtxID, 512, NULL, infoLog);
        std::cerr << "ERROR::SHADER::VERTEX_COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    frgID = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(frgID, 1, &frgSrc, NULL);
    glCompileShader(frgID);

    glGetShaderiv(frgID, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(frgID, 512, NULL, infoLog);
        std::cerr << "ERROR::SHADER::FRAGMENT_COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    unsigned int shaderID = glCreateProgram();
    glAttachShader(shaderID, vtxID);
    glAttachShader(shaderID, frgID);
    glLinkProgram(shaderID);

    glGetProgramiv(shaderID, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderID, 512, NULL, infoLog);
        std::cerr << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }
    glDeleteShader(vtxID);
    glDeleteShader(frgID);

    // Additional setup
    // -----------------
    std::vector<Vertex> vertices = {
        {{  0.5f, -0.5f, 0.0f }, { 1.0f, 0.0f, 0.0f }},
        {{  0.0f,  0.5f, 0.0f }, { 0.0f, 0.0f, 1.0f }},
        {{ -0.5f, -0.5f, 0.0f }, { 0.0f, 1.0f, 0.0f }}
    };

    std::vector<unsigned int> indices = {
        0, 1, 2
    };

    unsigned int vaoID, vboID, eboID;

    glGenVertexArrays(1, &vaoID);
    glBindVertexArray(vaoID);

    glGenBuffers(1, &vboID);
    glBindBuffer(GL_ARRAY_BUFFER, vboID);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);

    glGenBuffers(1, &eboID);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, eboID);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, color));
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    // Core loop
    // ----------
    while (!glfwWindowShouldClose(window)) {
        processInput(window);
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(shaderID);

        glBindVertexArray(vaoID);
        glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // End of program
    // ---------------
    glfwTerminate();
    return 0;
}