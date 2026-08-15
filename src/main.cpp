#include <glm/gtc/random.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/string_cast.hpp>

#include "MiniEngine/pch.hpp"
#include "MiniEngine/wrappers/shader.hpp"
#include "MiniEngine/wrappers/texture2d.hpp"
#include "MiniEngine/world/camera.hpp"
#include "MiniEngine/extras/time.hpp"
#include "MiniEngine/world/light.hpp"
#include "MiniEngine/extras/prefabs.hpp"
#include "MiniEngine/world/material.hpp"

using namespace MiniEngine;

Camera* mainCamera = new Camera(new Transform(glm::vec3(0.0f, 0.0f, 3.0f), glm::identity<glm::quat>(), glm::vec3(1.0f)));

bool firstMouse = true;
i32 lastX, lastY;

void processInput(GLFWwindow* window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    
    f32 velocity = Time::delta() * 2.5f;

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

    GLFWwindow* window = glfwCreateWindow(800 ,600, "MiniEngine", NULL, NULL);

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
    glEnable(GL_CULL_FACE);

    Mesh mesh = Prefabs::cube(Transform());
    Texture2D texDiffuse = Loader<Texture2D>::load("../assets/textures/container2.png");
    Texture2D texSpecular = Loader<Texture2D>::load("../assets/textures/container2_specular.png");
    Texture2D texEmissive = Loader<Texture2D>::load("../assets/textures/matrix.jpg");
    ShaderProgram mainShader = Loader<ShaderProgram>::load("../assets/shaders/main.vert", "../assets/shaders/main.frag");

    Material mat(&mainShader);
    mat.set<Texture2D*>("diffuse", &texDiffuse);
    mat.set<Texture2D*>("specular", &texSpecular);
    mat.set<Texture2D*>("emissive", &texEmissive);
    mat.set<f32>("emissiveStrenght", 1.0f);
    mat.set<f32>("shininess", 32.0f);

    std::vector<Transform> transforms = {
        Transform(glm::vec3(0.0f, 0.0f, 0.0f), glm::identity<glm::quat>(), glm::vec3(1.0f)),
        Transform(glm::vec3(2.0f, 5.0f, -15.0f), glm::identity<glm::quat>(), glm::vec3(1.0f)),
        Transform(glm::vec3(-1.5f, -2.2f, -2.5f), glm::identity<glm::quat>(), glm::vec3(1.0f)),
        Transform(glm::vec3(-3.8f, -2.0f, -12.3f), glm::identity<glm::quat>(), glm::vec3(1.0f)),
        Transform(glm::vec3(2.4f, -0.4f, -3.5f), glm::identity<glm::quat>(), glm::vec3(1.0f)),
        Transform(glm::vec3(-1.7f, 3.0f, -7.5f), glm::identity<glm::quat>(), glm::vec3(1.0f)),
        Transform(glm::vec3(1.3f, -2.0f, -2.5f), glm::identity<glm::quat>(), glm::vec3(1.0f)),
        Transform(glm::vec3(1.5f, 2.0f, -2.5f), glm::identity<glm::quat>(), glm::vec3(1.0f)),
        Transform(glm::vec3(1.5f, 0.2f, -1.5f), glm::identity<glm::quat>(), glm::vec3(1.0f)),
        Transform(glm::vec3(-1.3f, 1.0f, -1.5f), glm::identity<glm::quat>(), glm::vec3(1.0f))
    };
    for (i32 i = 0; i < transforms.size(); i++) {
        transforms[i].rotate(glm::angleAxis(20.0f * i, glm::normalize(glm::vec3(1.0f, 0.3f, 0.5f))));
    }

    std::vector<f32>blinkOffsets;
    for (i32 i = 0; i < transforms.size(); i++)
        blinkOffsets.push_back(glm::linearRand(0.0f, 100.0f));

    ShaderProgram lightSrcShader = Loader<ShaderProgram>::load("../assets/shaders/main.vert", "../assets/shaders/lightSrc.frag");
    
    Material lightSrcMat(&lightSrcShader);
    
    std::vector<LightSource> lightSorces = {
        // Directional light 1
        LightSource(
            new Transform(glm::vec3(0.0f), glm::identity<glm::quat>(), glm::vec3(0.2f)),
            glm::vec3(0.0f, 0.0f, 0.0f),
            glm::vec3(0.05f, 0.05f, 0.05f),
            glm::vec3(0.2f, 0.2f, 0.2f)
        ),
        // Point light 1
        LightSource(
            new Transform(glm::vec3(0.7f, 0.2f, 2.0f), glm::identity<glm::quat>(), glm::vec3(0.2f)),
            glm::vec3(0.1f, 0.1f, 0.1f) * 0.1f,
            glm::vec3(0.1f, 0.1f, 0.1f),
            glm::vec3(0.1f, 0.1f, 0.1f),
            1.0f,
            0.14f,
            0.07f
        ),
        // Point light 2
        LightSource(
            new Transform(glm::vec3(2.3f, -3.3f, -4.0f), glm::identity<glm::quat>(), glm::vec3(0.2f)),
            glm::vec3(0.1f, 0.1f, 0.1f) * 0.1f,
            glm::vec3(0.1f, 0.1f, 0.1f),
            glm::vec3(0.1f, 0.1f, 0.1f),
            1.0f,
            0.14f,
            0.07f
        ),
        // Point light 3
        LightSource(
            new Transform(glm::vec3(-4.0f, 2.0f, -12.0f), glm::identity<glm::quat>(), glm::vec3(0.2f)),
            glm::vec3(0.1f, 0.1f, 0.1f) * 0.1f,
            glm::vec3(0.1f, 0.1f, 0.1f),
            glm::vec3(0.1f, 0.1f, 0.1f),
            1.0f,
            0.22f,
            0.2f
        ),
        // Point light 4
        LightSource(
            new Transform(glm::vec3(0.0f, 0.0f, -3.0f), glm::identity<glm::quat>(), glm::vec3(0.2f)),
            glm::vec3(0.3f, 0.1f, 0.1f) * 0.1f,
            glm::vec3(0.3f, 0.1f, 0.1f),
            glm::vec3(0.3f, 0.1f, 0.1f),
            1.0f,
            0.14f,
            0.07f
        ),
        // Spot light
        LightSource(
            mainCamera->transform,
            glm::vec3(0.0f, 0.0f, 0.0f),
            glm::vec3(1.0f, 1.0f, 1.0f),
            glm::vec3(1.0f, 1.0f, 1.0f),
            1.0f,
            0.09f,
            0.032f,
            10.0f,
            15.0f
        )
    };

    lightSorces[0].transform->lookAt(glm::vec3(-0.2f, -1.0f, -0.3f));
    
    // Core loop
    while (!glfwWindowShouldClose(window)) {
        Time::update();

        processInput(window);

        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glm::mat4 M = glm::mat4(1.0f);
        
        mat.getShader()->use();
        mat.getShader()->setMat4("P", mainCamera->projection(800, 600));
        mat.getShader()->setMat4("V", mainCamera->transform->view());
        mat.getShader()->setVec3("camera.position", mainCamera->transform->position);
        mat.getShader()->setFloat("time", Time::current());
        
        for (i32 i = 0; i < lightSorces.size(); i++) {
            mat.getShader()->setInt("lights[" + std::to_string(i) + "].lightType", static_cast<i32>(lightSorces[i].type));
            mat.getShader()->setVec3("lights[" + std::to_string(i) + "].position", lightSorces[i].transform->position);
            mat.getShader()->setVec3("lights[" + std::to_string(i) + "].direction", lightSorces[i].direction());
            mat.getShader()->setVec3("lights[" + std::to_string(i) + "].ambient", lightSorces[i].ambient);
            mat.getShader()->setVec3("lights[" + std::to_string(i) + "].diffuse", lightSorces[i].diffuse);
            mat.getShader()->setVec3("lights[" + std::to_string(i) + "].specular", lightSorces[i].specular);
            mat.getShader()->setFloat("lights[" + std::to_string(i) + "].constant", lightSorces[i].constant);
            mat.getShader()->setFloat("lights[" + std::to_string(i) + "].linear", lightSorces[i].linear);
            mat.getShader()->setFloat("lights[" + std::to_string(i) + "].quadratic", lightSorces[i].quadratic);
            mat.getShader()->setFloat("lights[" + std::to_string(i) + "].cutOff", glm::cos(glm::radians(lightSorces[i].cutOff)));
            mat.getShader()->setFloat("lights[" + std::to_string(i) + "].outerCutOff", glm::cos(glm::radians(lightSorces[i].outerCutOff)));
        }
        
        mat.bind();
        for (i32 i = 0; i < transforms.size(); i++) {
            M = transforms[i].model();

            mat.getShader()->setMat4("M", M);
            mat.getShader()->setMat3("mN", glm::mat3(glm::transpose(glm::inverse(M))));
            mat.getShader()->setFloat("blinkOffset", blinkOffsets[i]);

            mesh.getVAO().bind();
            glDrawElements(GL_TRIANGLES, mesh.getEBO().getCount(), GL_UNSIGNED_INT, 0);
        }
        mat.unbind();

        lightSrcMat.bind();
        for (i32 i = 0; i < lightSorces.size(); i++) {
            if (i == lightSorces.size() - 1)
                continue;
            
            M = lightSorces[i].transform->model();
            
            lightSrcMat.getShader()->setMat4("P", mainCamera->projection(800, 600));
            lightSrcMat.getShader()->setMat4("V", mainCamera->transform->view());
            lightSrcMat.getShader()->setMat4("M", M);

            lightSrcMat.set<glm::vec3>("color", lightSorces[i].diffuse);
            
            mesh.getVAO().bind();
            glDrawElements(GL_TRIANGLES, mesh.getEBO().getCount(), GL_UNSIGNED_INT, 0);
        }
        lightSrcMat.unbind();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    return 0;
}