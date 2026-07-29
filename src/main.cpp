#include <iostream>
#include <vector>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

void framebufferSizeCallback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

const char* vertexSrc =
    "#version 330 core\n\n"
    "layout (location = 0) in vec3 position;\n"
    "layout (location = 1) in vec3 color;\n\n"
    "out vec3 vColor;\n\n"
    "void main() {\n"
    "   gl_Position = vec4(position, 1.0);\n"
    "   vColor = color;\n"
    "}\n";

const char* fragmentSrc =
    "#version 330 core\n\n"
    "in vec3 vColor;\n\n"
    "out vec4 fColor;\n\n"
    "void main() {\n"
    "   fColor = vec4(vColor, 1.0);\n"
    "}\n";

int main() {
    // Window setup
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(800 ,600, "Hello Triangle", NULL, NULL);

    if (!window) {
        std::cerr << "Failed to create GLFW window." << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // Variables
    std::vector<float> vertices = {
         0.5f, -0.5f, 0.0f,   1.0f, 0.0f, 0.0f,
         0.0f,  0.5f, 0.0f,   0.0f, 0.0f, 1.0f,
        -0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f
    };

    std::vector<unsigned int> indices = {
        0, 1, 2
    };

    unsigned int vao, vbo, ebo;

    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

    glGenBuffers(1, &ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), 0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    unsigned int program = glCreateProgram();

    unsigned int vertexID = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexID, 1, &vertexSrc, NULL);
    glCompileShader(vertexID);

    int success;

    glGetShaderiv(vertexID, GL_COMPILE_STATUS, &success);
    if (!success) {
        int maxLogLength = 0;
        glGetShaderiv(vertexID, GL_INFO_LOG_LENGTH, &maxLogLength);
        std::string infoLog(maxLogLength, '\0');
        glGetShaderInfoLog(vertexID, maxLogLength, &maxLogLength, &infoLog[0]);

        std::cerr << "ERROR::VERTEX_SHADER::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    unsigned int fragmentID = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentID, 1, &fragmentSrc, NULL);
    glCompileShader(fragmentID);

    glGetShaderiv(fragmentID, GL_COMPILE_STATUS, &success);
    if (!success) {
        int maxLogLength = 0;
        glGetShaderiv(fragmentID, GL_INFO_LOG_LENGTH, &maxLogLength);
        std::string infoLog(maxLogLength, '\0');
        glGetShaderInfoLog(fragmentID, maxLogLength, &maxLogLength, &infoLog[0]);

        std::cerr << "ERROR::FRAGMENT_SHADER::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    glAttachShader(program, vertexID);
    glAttachShader(program, fragmentID);

    glLinkProgram(program);

    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if (!success) {
        int maxLogLength = 0;
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLogLength);
        std::string infoLog(maxLogLength, '\0');
        glGetProgramInfoLog(program, maxLogLength, &maxLogLength, &infoLog[0]);
        std::cerr << "ERROR::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }

    glDeleteShader(vertexID);
    glDeleteShader(fragmentID);

    // Core loop
    while (!glfwWindowShouldClose(window)) {
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(program);

        glBindVertexArray(vao);
        glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    return 0;
}