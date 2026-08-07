#include "MiniEngine/pch.hpp"
#include "MiniEngine/wrappers/vao.hpp"
#include "MiniEngine/wrappers/vbo.hpp"
#include "MiniEngine/wrappers/ebo.hpp"
#include "MiniEngine/wrappers/shader.hpp"
#include "MiniEngine/wrappers/texture2d.hpp"
#include "MiniEngine/world/camera.hpp"
using namespace MiniEngine;

Camera* mainCamera = new Camera(new Transform(glm::vec3(0.0f, 0.0f, 3.0f), glm::identity<glm::quat>(), glm::vec3(1.0f)));

bool firstMouse = true;
i32 lastX, lastY;

f32 deltaTime, currrentFrame = 0.0f, lastFrame = 0.0f;

void processInput(GLFWwindow* window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    
    f32 velocity = deltaTime * 2.5f;

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        mainCamera->transform->position += mainCamera->transform->front() * velocity;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        mainCamera->transform->position -= mainCamera->transform->front() * velocity;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        mainCamera->transform->position -= mainCamera->transform->right() * velocity;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        mainCamera->transform->position += mainCamera->transform->right() * velocity;
    if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS)
        mainCamera->transform->position += mainCamera->transform->up() * velocity;
    if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS)
        mainCamera->transform->position -= mainCamera->transform->up() * velocity;
}

void framebufferSizeCallback(GLFWwindow* window, i32 width, i32 height) {
    glViewport(0, 0, width, height);
}

void scrollCallback(GLFWwindow* window, f64 xOffset, f64 yOffset) {
    mainCamera->fov -= yOffset;

    if (mainCamera->fov < 0.1f)
        mainCamera->fov = 0.1f;
    if (mainCamera->fov > 89.0f)
        mainCamera->fov = 89.0f;
}

void cursorPosCallback(GLFWwindow* window, f64 x, f64 y) {
    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
        if (firstMouse) {
            lastX = x;
            lastY = y;
            firstMouse = false;
            return;
        }

        f32 xOffset = x - lastX;
        f32 yOffset = lastY - y;

        lastX = x;
        lastY = y;

        xOffset *= 0.005f;
        yOffset *= 0.005f;
        
        glm::quat yaw = glm::angleAxis(-xOffset, glm::vec3(0.0f, 1.0f, 0.0f));
        glm::quat pitch = glm::angleAxis(yOffset, glm::vec3(1.0f, 0.0f, 0.0f));

        mainCamera->transform->rotation = glm::normalize(yaw * mainCamera->transform->rotation * pitch);
    }
    else
        firstMouse = true;
}

void mouseButtonCallback(GLFWwindow* window, i32 button, i32 action, i32 mods) {
    if (button == GLFW_MOUSE_BUTTON_LEFT)
        if (action == GLFW_PRESS)
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        if (action == GLFW_RELEASE)
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
}

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
    glfwSetScrollCallback(window, scrollCallback);
    glfwSetCursorPosCallback(window, cursorPosCallback);
    glfwSetMouseButtonCallback(window, mouseButtonCallback);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    glEnable(GL_DEPTH_TEST);

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

    Transform transform;

    MiniEngine::ShaderProgram mainShader("../assets/shaders/main.vert", "../assets/shaders/main.frag");

    Texture2D myTex(load2DImage("../assets/textures/wall.jpg"));

    // Core loop
    while (!glfwWindowShouldClose(window)) {
        currrentFrame = (f32)glfwGetTime();
        deltaTime = currrentFrame - lastFrame;
        lastFrame = currrentFrame;

        processInput(window);

        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        transform.rotate(glm::quat(glm::vec3(0.0f, 0.02f, 0.0f)));
        
        mainShader.use();
        mainShader.setVec3("myColor", glm::vec3(1.0f, 0.5f, 0.2f));
        mainShader.setMat4("P", mainCamera->projection(800, 600));
        mainShader.setMat4("V", mainCamera->transform->view());
        mainShader.setMat4("M", transform.model());

        myTex.bind(0);
        mainShader.setSampler2D("myTex", 0);
       
        vao.bind();
        glDrawElements(GL_TRIANGLES, ebo.getCount(), GL_UNSIGNED_INT, 0);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    return 0;
}