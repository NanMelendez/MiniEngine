// #include "../include/MiniEngine/pch.hpp"
// #include "../include/MiniEngine/core/vao.hpp"
// #include "../include/MiniEngine/core/vbo.hpp"
// #include "../include/MiniEngine/core/ebo.hpp"
// #include "../include/MiniEngine/core/shader.hpp"
#include "../include/MiniEngine/app.hpp"

// void framebufferSizeCallback(GLFWwindow* window, int width, int height) {
//     glViewport(0, 0, width, height);
// }

// void processInput(GLFWwindow* window) {
//     if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
//         glfwSetWindowShouldClose(window, true);
// }

// struct Vertex {
//     glm::vec3 position;
//     glm::vec3 color;
// };

int main() {
    std::unique_ptr<MiniEngine::Application> app = std::make_unique<MiniEngine::Application>(800, 600, "MiniEngine [ALPHA]");

    app->run();
    
    // using namespace MiniEngine;

    // // Graphics libraries initialization
    // // ----------------------------------
    // if (!glfwInit()) {
    //     std::cerr << "Failed to initialize GLFW." << std::endl;
    //     return -1;
    // }

    // glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    // glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    // glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // GLFWwindow* window = glfwCreateWindow(800, 600, "Hello, triangle!", NULL, NULL);

    // if (!window) {
    //     std::cerr << "Failed to initialize GLFW window." << std::endl;
    //     return -1;
    // }

    // glfwMakeContextCurrent(window);
    // glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);

    // if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    //     std::cerr << "Failed to initialize GLAD." << std::endl;
    //     return -1;
    // }

    // // Shader setup
    // // -------------
    // Core::Shader shader("../assets/shaders/main.vert", "../assets/shaders/main.frag");

    // // Additional setup
    // // -----------------
    // std::vector<Vertex> vertices = {
    //     {{  0.5f, -0.5f, 0.0f }, { 1.0f, 0.0f, 0.0f }},
    //     {{  0.0f,  0.5f, 0.0f }, { 0.0f, 0.0f, 1.0f }},
    //     {{ -0.5f, -0.5f, 0.0f }, { 0.0f, 1.0f, 0.0f }}
    // };

    // std::vector<u32> indices = {
    //     0, 1, 2
    // };

    // Core::VAO vao; vao.bind();
    // Core::VBO vbo(vertices.size() * sizeof(Vertex), vertices.data()); vbo.bind();
    // Core::EBO ebo(indices.size() * sizeof(u32), indices.data()); ebo.bind();

    // vao.setAttribPointer(0, 3, sizeof(Vertex), (void*)offsetof(Vertex, position));
    // vao.enableIndex(0);
    // vao.setAttribPointer(1, 3, sizeof(Vertex), (void*)offsetof(Vertex, color));
    // vao.enableIndex(1);

    // vao.unbind();
    // vbo.unbind();
    // ebo.unbind();

    // // Core loop
    // // ----------
    // while (!glfwWindowShouldClose(window)) {
    //     processInput(window);
    //     glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    //     glClear(GL_COLOR_BUFFER_BIT);

    //     // glUseProgram(shaderID);
    //     shader.use();

    //     vao.bind();
    //     glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);

    //     glfwSwapBuffers(window);
    //     glfwPollEvents();
    // }

    // // End of program
    // // ---------------
    // glfwTerminate();
    return 0;
}