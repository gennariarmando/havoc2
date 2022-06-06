#include "ABaseGL.h"
#include "Camera.h"
#include "icon.h"
#include "Shader.h"
#include "Screen.h"
#include "DefaultShaders.h"

AGLObjects ABaseGL::m_Objects;

ABaseGL::ABaseGL() {
    m_Objects.window = NULL;
    m_Objects.monitor = NULL;
    m_Objects.quitGame = false;
    m_Objects.maxFPS = 30;
    m_Objects.fullScreen = false;
    m_Objects.windowIcon.width = 0;
    m_Objects.windowIcon.height = 0;
    m_Objects.windowIcon.pixels = 0;
    m_Objects.deltaTime = 0.0f;
    m_Objects.currentTime = glfwGetTime();
    m_Objects.previousTime = m_Objects.currentTime;
    m_Objects.timeDifference = 0.0;
    m_Objects.frameCounter = 0;
    m_Objects.framePerSecond = 0;
    m_Objects.mousePosition = {};
    m_Objects.oldMousePosition = {};
    m_Objects.mouseDelta = {};
    *m_Objects.oldInputKeys = {};
    *m_Objects.newInputKeys = {};
    m_Objects.defaultShader = 0;
}

ABaseGL::~ABaseGL() {

}

void ABaseGL::Init() {
    glfwSetErrorCallback(ErrorCallback);

    if (!glfwInit())
        exit(EXIT_FAILURE);

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    m_Objects.window = glfwCreateWindow(DEFAULT_SCREEN_WIDTH, DEFAULT_SCREEN_HEIGHT, "havoc2", NULL, NULL);

    if (!m_Objects.window) {
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    m_Objects.monitor = glfwGetPrimaryMonitor();

    if (!m_Objects.monitor) {
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    m_Objects.windowIcon.width = icon.width;
    m_Objects.windowIcon.height = icon.height;

    glm::uint32 size = m_Objects.windowIcon.width * m_Objects.windowIcon.height * 4;
    m_Objects.windowIcon.pixels = new glm::uint8[size + 1];

    for (glm::uint32 i = 0; i < size + 1; i++) {
        m_Objects.windowIcon.pixels[i] = icon.pixel_data[i];
    }

    glfwSetWindowIcon(m_Objects.window, 1, &m_Objects.windowIcon);

    glfwSetKeyCallback(m_Objects.window, KeyCallback);
    glfwSetCursorPosCallback(m_Objects.window, CursorCallback);
    glfwMakeContextCurrent(m_Objects.window);
    glfwSetFramebufferSizeCallback(m_Objects.window, FrameBufferCallBack);
    gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
    glfwSwapInterval(0);
    SetWindowSize(DEFAULT_SCREEN_WIDTH, DEFAULT_SCREEN_HEIGHT);
    SetFullscreen(m_Objects.fullScreen);
    ClearColorBuffer();

    glfwSetInputMode(m_Objects.window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    CreateDefaultTextures();

    m_Objects.defaultShader = std::make_unique<CShader>(vertexShader, fragmentShader, true);
}

void ABaseGL::Update() {
    bool altPressed = false;
    bool enterPressed = false;
    static bool thisFrame = true;

    if (glfwGetKey(GetWindow(), GLFW_KEY_LEFT_ALT) == GLFW_PRESS)
        altPressed = true;

    if (altPressed && glfwGetKey(GetWindow(), GLFW_KEY_ENTER) == GLFW_PRESS)
        enterPressed = true;
    else
        thisFrame = true;

    if (thisFrame) {
        if (altPressed && enterPressed) {
            CenterWindowPosition();
            SetFullscreen(!m_Objects.fullScreen);
            thisFrame = false;
        }
    }
}

void ABaseGL::Shutdown() {
    RemoveSprite2DTextures();

    delete[] m_Objects.windowIcon.pixels;
    glfwDestroyWindow(m_Objects.window);
    glfwTerminate();
}

void ABaseGL::CreateDefaultTextures() {
    glm::uint8 rectData[8 * 8 * 4];
    for (glm::uint32 i = 0; i < 8 * 8 * 4; i++) {
        rectData[i] = 255;
    }

    glGenTextures(1, 0);
    glBindTexture(GL_TEXTURE_2D, 0);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 8, 8, 0, GL_RGBA, GL_UNSIGNED_BYTE, &rectData);
}

void ABaseGL::RemoveSprite2DTextures() {
    glm::uint32 i = 0;
    glDeleteTextures(1, &i);
}

void ABaseGL::UpdateViewport() {
    glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
}

void ABaseGL::PollEvents() {
    glfwPollEvents();
}

void ABaseGL::ClearColorBuffer() {
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClearDepth(1.0f);
    glDepthFunc(GL_LEQUAL);
    glEnable(GL_DEPTH_TEST);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void ABaseGL::SwapBuffers() {
    glfwSwapBuffers(m_Objects.window);
}

void ABaseGL::SetFullscreen(bool on) {
    glfwSetWindowMonitor(m_Objects.window, on ? m_Objects.monitor : NULL, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, GLFW_DONT_CARE);
    CenterWindowPosition();
    m_Objects.fullScreen = on;
}

bool ABaseGL::WindowShouldClose() {
    return static_cast<bool>(glfwWindowShouldClose(m_Objects.window)) || m_Objects.quitGame;
}

void ABaseGL::CenterWindowPosition() {
    if (!m_Objects.monitor)
        return;

    const GLFWvidmode* mode = glfwGetVideoMode(m_Objects.monitor);
    if (!mode)
        return;

    glm::int32 x, y;
    glfwGetMonitorPos(m_Objects.monitor, &x, &y);

    glm::int32 w, h;
    glfwGetWindowSize(m_Objects.window, &w, &h);

    glfwSetWindowPos(m_Objects.window,
        x + (mode->width - w) / 2,
        y + (mode->height - h) / 2);

    m_Objects.maxFPS = mode->refreshRate;
}

void ABaseGL::SetWindowSize(glm::int32 width, glm::int32 height) {
    glfwHideWindow(m_Objects.window);
    glfwSetWindowSizeLimits(m_Objects.window, DEFAULT_SCREEN_WIDTH, DEFAULT_SCREEN_HEIGHT, GLFW_DONT_CARE, GLFW_DONT_CARE);
    glfwSetWindowSize(m_Objects.window, width, height);
    CenterWindowPosition();
    glfwShowWindow(m_Objects.window);
    FrameBufferCallBack(m_Objects.window, width, height);
}

void ABaseGL::ErrorCallback(glm::int32 error, const char* description) {
    std::cout << "Error: " << description << std::endl;
}

void ABaseGL::KeyCallback(GLFWwindow* window, glm::int32 key, glm::int32 scancode, glm::int32 action, glm::int32 mods) {
    m_Objects.oldInputKeys[key] = m_Objects.newInputKeys[key];
    m_Objects.newInputKeys[key] = action;
}

void ABaseGL::CursorCallback(GLFWwindow* window, double x, double y) {
    m_Objects.oldMousePosition = m_Objects.mousePosition;
    m_Objects.mousePosition.x = static_cast<float>(x);
    m_Objects.mousePosition.y = static_cast<float>(y);
}

void ABaseGL::FrameBufferCallBack(GLFWwindow* window, int width, int height) {
    Screen.GetScreenWidth() = width;
    Screen.GetScreenHeight() = height;
    Screen.GetAspectRatio() = width / (float)height;

    UpdateViewport();
}
       
void ABaseGL::BeginDrawing() {
    m_Objects.currentTime = glfwGetTime();
    m_Objects.timeDifference = m_Objects.currentTime - m_Objects.previousTime;

    m_Objects.frameCounter++;
    m_Objects.framePerSecond = (1.0 / m_Objects.timeDifference) * m_Objects.frameCounter;
    m_Objects.deltaTime = (m_Objects.timeDifference / m_Objects.frameCounter);

    std::string fps = std::to_string(static_cast<glm::uint32>(m_Objects.framePerSecond));
    std::string ms = std::to_string((m_Objects.timeDifference / m_Objects.frameCounter) * 1000.0);
    std::string title = "havoc2 - " + fps + "FPS / " + ms + "ms";
    glfwSetWindowTitle(m_Objects.window, title.c_str());

    if (ABS(distance(m_Objects.oldMousePosition, m_Objects.mousePosition)) > 0.0f) {
        m_Objects.mouseDelta.x = m_Objects.mousePosition.x - m_Objects.oldMousePosition.x;
        m_Objects.mouseDelta.y = m_Objects.oldMousePosition.y - m_Objects.mousePosition.y;

        m_Objects.oldMousePosition = m_Objects.mousePosition;
    }

    ClearColorBuffer();
}

void ABaseGL::EndDrawing() {
    PollEvents();
    SwapBuffers();
    m_Objects.mouseDelta = glm::vec2(0.0f, 0.0f);

    glDisable(GL_BLEND);

    if (m_Objects.timeDifference > 1.0 / static_cast<double>(m_Objects.maxFPS)) {
        m_Objects.previousTime = m_Objects.currentTime;
        m_Objects.frameCounter = 0;
    }

    while (glfwGetTime() < m_Objects.previousTime + 1.0 / static_cast<double>(m_Objects.maxFPS));
}

void ABaseGL::BeginScene3D() {
    Camera.GetScene3D() = true;
    Camera.ComputeProjection();

    glDisable(GL_STENCIL_TEST);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
}

void ABaseGL::EndScene3D() {
    Camera.GetScene3D() = false;
    Camera.ComputeProjection();

    glDisable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);
}

bool ABaseGL::GetKeyDown(glm::uint32 key) {
    return m_Objects.newInputKeys[key];
}

bool ABaseGL::GetKeyUp(glm::uint32 key) {
    return 0;
}
