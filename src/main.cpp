#include "MiniEngine/pch.hpp"
#include "MiniEngine/wrappers/shader.hpp"
#include "MiniEngine/wrappers/texture2d.hpp"
#include "MiniEngine/world/camera.hpp"
#include "MiniEngine/extras/time.hpp"
#include "MiniEngine/world/light.hpp"
#include "MiniEngine/extras/prefabs.hpp"
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
    glEnable(GL_CULL_FACE);

    Mesh mesh = Prefabs::cube(Transform());
    Transform transform;
    Texture2D texDiffuse(load2DImage("../assets/textures/container2.png"));
    Texture2D texSpecular(load2DImage("../assets/textures/container2_specular.png"));
    Texture2D texEmissive(load2DImage("../assets/textures/matrix.jpg"));
    MiniEngine::ShaderProgram mainShader("../assets/shaders/main.vert", "../assets/shaders/main.frag");

    LightSource light(
        new Transform(glm::vec3(1.2f, 1.0f, 2.0f), glm::identity<glm::quat>(), glm::vec3(0.2f)),
        glm::vec3(0.5f),
        glm::vec3(0.5f),
        glm::vec3(1.0f)
    );
    MiniEngine::ShaderProgram lightSrcShader("../assets/shaders/main.vert", "../assets/shaders/lightSrc.frag");
    
    // Core loop
    while (!glfwWindowShouldClose(window)) {
        Time::update();

        processInput(window);

        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        transform.rotate(glm::angleAxis(0.025f, glm::normalize(glm::vec3(0.25f, 1.0f, -0.5f))));

        glm::mat4 M = transform.model();
        
        mainShader.use();
        mainShader.setMat4("P", mainCamera->projection(800, 600));
        mainShader.setMat4("V", mainCamera->transform->view());
        mainShader.setMat4("M", M);
        mainShader.setMat3("mN", glm::mat3(glm::transpose(glm::inverse(M))));
        mainShader.setVec3("camera.position", mainCamera->transform->position);
        mainShader.setFloat("time", Time::current());

        mainShader.setInt("lights[0].type", static_cast<i32>(light.type));
        mainShader.setVec3("lights[0].position", light.transform->position);
        mainShader.setVec3("lights[0].direction", light.direction());
        mainShader.setVec3("lights[0].ambient", light.ambient);
        mainShader.setVec3("lights[0].diffuse", light.diffuse);
        mainShader.setVec3("lights[0].specular", light.specular);
        mainShader.setFloat("lights[0].constant", light.constant);
        mainShader.setFloat("lights[0].linear", light.linear);
        mainShader.setFloat("lights[0].quadratic", light.quadratic);
        mainShader.setFloat("lights[0].cutOff", light.cutOff());
        mainShader.setFloat("lights[0].outerCutOff", light.outerCutOff());

        texDiffuse.bind(0);
        mainShader.setSampler2D("material.diffuse", 0);

        texSpecular.bind(1);
        mainShader.setSampler2D("material.specular", 1);

        texEmissive.bind(2);
        mainShader.setSampler2D("material.emissive", 2);

        mainShader.setFloat("material.emissiveStrenght", 1.0f);
        mainShader.setFloat("material.shininess", 128.0f);
       
        mesh.getVAO().bind();
        glDrawElements(GL_TRIANGLES, mesh.getEBO().getCount(), GL_UNSIGNED_INT, 0);

        M = light.transform->model();

        lightSrcShader.use();
        lightSrcShader.setMat4("P", mainCamera->projection(800, 600));
        lightSrcShader.setMat4("V", mainCamera->transform->view());
        lightSrcShader.setMat4("M", M);
        lightSrcShader.setVec3("lightColor", light.ambient);
        
        mesh.getVAO().bind();
        glDrawElements(GL_TRIANGLES, mesh.getEBO().getCount(), GL_UNSIGNED_INT, 0);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    return 0;
}