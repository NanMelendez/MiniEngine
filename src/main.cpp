#include "MiniEngine/miniengine.hpp"
using namespace MiniEngine;

Camera* mainCamera = new Camera(new Transform(glm::vec3(0.0f, 0.0f, 3.0f), glm::identity<glm::quat>(), glm::vec3(1.0f)));

bool firstMouse = true;
i32 lastX, lastY;

i32 wWidth = 1600, wHeight = 1200;

FBO mainFBO;

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
    wWidth = width;
    wHeight = height;
    mainFBO.resize(width, height);
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
    if (button == GLFW_MOUSE_BUTTON_LEFT) {
        if (action == GLFW_PRESS)
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        if (action == GLFW_RELEASE)
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    }
}

struct alignas(16) GlobalData {
    glm::ivec2 resolution;
    f32 time;
    i32 frame;
};

int main() {
    // Window setup
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(wWidth, wHeight, "MiniEngine", NULL, NULL);

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

    glm::vec3 bgColor = glm::vec3(0.0f, 0.0f, 0.0f);

    mainFBO.load(wWidth, wHeight);
    
    ShaderProgram mainShader = Loader<ShaderProgram>::load("../assets/shaders/main.glsl");
    ShaderProgram lightSrcShader = Loader<ShaderProgram>::load("../assets/shaders/lightSrc.glsl");
    ShaderProgram fboShader = Loader<ShaderProgram>::load("../assets/shaders/fbo.glsl");

    Material mat(&mainShader);
    Material lightSrcMat(&lightSrcShader);
    Material fboMat(&fboShader);

    UBO uboMatrices(2 * sizeof(glm::mat4));
    UBO uboCamera(sizeof(CameraRawData));
    UBO uboLights(MAX_LIGHT_SOURCES * sizeof(LightRawData));
    UBO uboGlobal(sizeof(GlobalData));

    uboMatrices.linkBase(0);
    uboCamera.linkBase(1);
    uboLights.linkBase(2);
    uboGlobal.linkBase(3);

    mainShader.linkUniformBlock("_uMatrices", 0);
    mainShader.linkUniformBlock("_uCamera", 1);
    mainShader.linkUniformBlock("_uLights", 2);
    mainShader.linkUniformBlock("_uGlobal", 3);

    lightSrcShader.linkUniformBlock("_uMatrices", 0);
    lightSrcShader.linkUniformBlock("_uCamera", 1);
    lightSrcShader.linkUniformBlock("_uLights", 2);
    lightSrcShader.linkUniformBlock("_uGlobal", 3);

    fboShader.linkUniformBlock("_uMatrices", 0);
    fboShader.linkUniformBlock("_uCamera", 1);
    fboShader.linkUniformBlock("_uLights", 2);
    fboShader.linkUniformBlock("_uGlobal", 3);

    /*
    std::unique_ptr<Texture2D> texCubemapXPos = std::make_unique<Texture2D>(Loader<Texture2D>::load("../assets/textures/skybox1/right.jpg"));
    std::unique_ptr<Texture2D> texCubemapXNeg = std::make_unique<Texture2D>(Loader<Texture2D>::load("../assets/textures/skybox1/left.jpg"));
    std::unique_ptr<Texture2D> texCubemapYPos = std::make_unique<Texture2D>(Loader<Texture2D>::load("../assets/textures/skybox1/top.jpg"));
    std::unique_ptr<Texture2D> texCubemapYNeg = std::make_unique<Texture2D>(Loader<Texture2D>::load("../assets/textures/skybox1/bottom.jpg"));
    std::unique_ptr<Texture2D> texCubemapZPos = std::make_unique<Texture2D>(Loader<Texture2D>::load("../assets/textures/skybox1/front.jpg"));
    std::unique_ptr<Texture2D> texCubemapZNeg = std::make_unique<Texture2D>(Loader<Texture2D>::load("../assets/textures/skybox1/back.jpg"));
    */

    Mesh mesh = Prefabs::cube(Transform());
    std::unique_ptr<Texture2D> texDiffuse = std::make_unique<Texture2D>(Loader<Texture2D>::load("../assets/textures/container2.png"));
    std::unique_ptr<Texture2D> texSpecular = std::make_unique<Texture2D>(Loader<Texture2D>::load("../assets/textures/container2_specular.png"));
    std::unique_ptr<Texture2D> texEmissive = std::make_unique<Texture2D>(Loader<Texture2D>::load("../assets/textures/matrix.jpg"));

    mat.set<Texture2D*>("diffuse", texDiffuse.get());
    mat.set<Texture2D*>("specular", texSpecular.get());
    mat.set<Texture2D*>("emissive", texEmissive.get());
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

    glm::vec3 pointLightColors[] = {
        glm::vec3(0.1f, 0.1f, 0.1f),
        glm::vec3(0.1f, 0.1f, 0.1f),
        glm::vec3(0.1f, 0.1f, 0.1f),
        glm::vec3(0.3f, 0.1f, 0.1f)
    };
    
    std::vector<LightSource> lightSorces = {
        // Directional light 1
        LightSource(
            new Transform(glm::vec3(0.0f), glm::quatLookAt(glm::normalize(glm::vec3(-0.2f, -1.0f, -0.3f)), glm::vec3(0.0f, 1.0f, 0.0f)), glm::vec3(0.2f)),
            glm::vec3(0.0f, 0.0f, 0.0f),
            glm::vec3(0.05f, 0.05f, 0.05f),
            glm::vec3(0.2f, 0.2f, 0.2f)
        ),
        // Point light 1
        LightSource(
            new Transform(glm::vec3(0.7f, 0.2f, 2.0f), glm::identity<glm::quat>(), glm::vec3(0.2f)),
            pointLightColors[0] * 0.1f,
            pointLightColors[0],
            pointLightColors[0],
            1.0f,
            0.14f,
            0.07f
        ),
        // Point light 2
        LightSource(
            new Transform(glm::vec3(2.3f, -3.3f, -4.0f), glm::identity<glm::quat>(), glm::vec3(0.2f)),
            pointLightColors[1] * 0.1f,
            pointLightColors[1],
            pointLightColors[1],
            1.0f,
            0.14f,
            0.07f
        ),
        // Point light 3
        LightSource(
            new Transform(glm::vec3(-4.0f, 2.0f, -12.0f), glm::identity<glm::quat>(), glm::vec3(0.2f)),
            pointLightColors[2] * 0.1f,
            pointLightColors[2],
            pointLightColors[2],
            1.0f,
            0.22f,
            0.2f
        ),
        // Point light 4
        LightSource(
            new Transform(glm::vec3(0.0f, 0.0f, -3.0f), glm::identity<glm::quat>(), glm::vec3(0.2f)),
            pointLightColors[3] * 0.1f,
            pointLightColors[3],
            pointLightColors[3],
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
    
    // Core loop
    while (!glfwWindowShouldClose(window)) {
        Time::update();

        glfwSetWindowTitle(window, (std::string("MiniEngine") + " - FPS: " + std::to_string((i32)Time::getFPS())).c_str());

        processInput(window);
        glfwPollEvents();

        mainFBO.bind();
        // glViewport(0, 0, wWidth, wHeight);
        glClearColor(bgColor.r, bgColor.g, bgColor.b, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        uboMatrices.bind();
        uboMatrices.update(sizeof(glm::mat4), glm::value_ptr(mainCamera->projection(wWidth, wHeight)), 0);
        uboMatrices.update(sizeof(glm::mat4), glm::value_ptr(mainCamera->transform->view()), sizeof(glm::mat4));
        // uboMatrices.update(sizeof(glm::mat4), glm::value_ptr(glm::mat4(glm::mat3(mainCamera->transform->view()))), 2 * sizeof(glm::mat4));
        uboMatrices.unbind();

        uboCamera.bind();
        CameraRawData cameraRaw = mainCamera->getRawData();
        uboCamera.update(sizeof(CameraRawData), &cameraRaw, 0);
        uboCamera.unbind();

        uboLights.bind();
        for (i32 i = 0; i < lightSorces.size(); i++) {
            LightRawData lightRaw = lightSorces[i].getRawData();
            uboLights.update(sizeof(LightRawData), &lightRaw, i * sizeof(LightRawData));
        }
        uboLights.unbind();

        uboGlobal.bind();
        GlobalData globalRaw = { glm::ivec2(wWidth, wHeight), Time::now(), 0 };
        uboGlobal.update(sizeof(GlobalData), &globalRaw, 0);
        uboGlobal.unbind();
        
        glm::mat4 M = glm::mat4(1.0f);
        
        for (i32 i = 0; i < transforms.size(); i++) {
            mat.set<f32>("blinkOffset", blinkOffsets[i]);

            mat.bind();

            M = transforms[i].model();
            mat.getShader()->setUniform("M", M);
            mat.getShader()->setUniform("mN", glm::mat3(glm::transpose(glm::inverse(M))));

            mesh.getVAO().bind();
            glDrawElements(GL_TRIANGLES, mesh.getEBO().getCount(), GL_UNSIGNED_INT, 0);
        }
        mat.unbind();

        lightSrcMat.getShader()->use();
        for (i32 i = 0; i < lightSorces.size(); i++) {
            if (i == lightSorces.size() - 1)
                continue;
            
            lightSrcMat.set<glm::vec3>("color", lightSorces[i].diffuse);

            lightSrcMat.bind();

            M = lightSorces[i].transform->model();
            lightSrcMat.getShader()->setUniform("M", M);
            
            mesh.getVAO().bind();
            glDrawElements(GL_TRIANGLES, mesh.getEBO().getCount(), GL_UNSIGNED_INT, 0);
        }
        lightSrcMat.unbind();
        
        mainFBO.unbind();
        glViewport(0, 0, wWidth, wHeight);
        mainFBO.render(fboMat);

        glfwSwapBuffers(window);
    }

    return 0;
}