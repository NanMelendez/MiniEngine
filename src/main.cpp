#include "MiniEngine/pch.hpp"
#include "MiniEngine/wrappers/vao.hpp"
#include "MiniEngine/wrappers/vbo.hpp"
#include "MiniEngine/wrappers/ebo.hpp"
#include "MiniEngine/wrappers/shader.hpp"
#include "MiniEngine/wrappers/texture2d.hpp"

void framebufferSizeCallback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

int main() {
    using namespace MiniEngine;
    
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
    std::vector<f32> vertices = {
         0.5f, -0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 0.0f,
         0.0f,  0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.5f, 1.0f,
        -0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   0.0f, 0.0f
    };

    std::vector<u32> indices = {
        0, 1, 2
    };
    
    MiniEngine::VAO vao(true); vao.bind();
    MiniEngine::VBO vbo(vertices.size() * sizeof(f32), vertices.data()); vbo.bind();
    MiniEngine::EBO ebo(indices.size() * sizeof(u32), indices.data()); ebo.bind();

    vao.setAttribPointer(0, 3, 8 * sizeof(f32), 0);
    vao.enableIndex(0);
    vao.setAttribPointer(1, 3, 8 * sizeof(f32), (void*)(3 * sizeof(f32)));
    vao.enableIndex(1);
    vao.setAttribPointer(2, 2, 8 * sizeof(f32), (void*)(6 * sizeof(f32)));
    vao.enableIndex(2);

    vao.unbind();
    vbo.unbind();
    ebo.unbind();

    MiniEngine::ShaderProgram mainShader("../assets/shaders/main.vert", "../assets/shaders/main.frag");

    Texture2D myTex(load2DImage("../assets/textures/wall.jpg"));

    // Core loop
    while (!glfwWindowShouldClose(window)) {
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // glUseProgram(program);
        mainShader.use();
        mainShader.setVec3("myColor", glm::vec3(1.0f, 0.5f, 0.2f));

        myTex.bind(0);
        mainShader.setSampler2D("myTex", 0);
       
        vao.bind();
        glDrawElements(GL_TRIANGLES, ebo.getCount(), GL_UNSIGNED_INT, 0);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    return 0;
}