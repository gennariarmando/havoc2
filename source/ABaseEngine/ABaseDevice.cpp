#include "ABaseDevice.h"
#include "Camera.h"
#include "icon.h"
#include "Screen.h"
#include "DefaultShaders.h"

std::unique_ptr<ABaseDevice> BaseDevice;
tDeviceObjects DeviceObjects;

ABaseDevice::ABaseDevice() {
    DeviceObjects.m_pWindow = NULL;
    DeviceObjects.m_pMonitor = NULL;
    DeviceObjects.m_bQuitGame = false;
    DeviceObjects.m_nMaxFPS = 30;
    DeviceObjects.m_bFullscreen = false;
    DeviceObjects.m_WindowIcon.width = 0;
    DeviceObjects.m_WindowIcon.height = 0;
    DeviceObjects.m_WindowIcon.pixels = 0;
    DeviceObjects.m_fDeltaTime = 0.0f;
    DeviceObjects.m_fCurrentTime = glfwGetTime();
    DeviceObjects.m_fPreviousTime = DeviceObjects.m_fCurrentTime;
    DeviceObjects.m_fTimeDifference = 0.0;
    DeviceObjects.m_nFrameCounter = 0;
    DeviceObjects.m_fFramePerSecond = 0;
    DeviceObjects.m_vMousePosition = {};
    DeviceObjects.m_vOldMousePosition = {};
    DeviceObjects.m_vMouseDelta = {};
    DeviceObjects.m_vOldInputKeys.resize(GLFW_KEY_LAST, 0);
    DeviceObjects.m_vNewInputKeys.resize(GLFW_KEY_LAST, 0);

    DeviceObjects.m_pDefaultShader = 0;
}

ABaseDevice::~ABaseDevice() {

}

void ABaseDevice::Init() {
    glfwSetErrorCallback(ErrorCallback);

    if (!glfwInit())
        exit(EXIT_FAILURE);

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    DeviceObjects.m_pWindow = glfwCreateWindow(DEFAULT_SCREEN_WIDTH, DEFAULT_SCREEN_HEIGHT, "havoc2", NULL, NULL);

    if (!DeviceObjects.m_pWindow) {
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    DeviceObjects.m_pMonitor = glfwGetPrimaryMonitor();

    if (!DeviceObjects.m_pMonitor) {
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    DeviceObjects.m_WindowIcon.width = icon.width;
    DeviceObjects.m_WindowIcon.height = icon.height;

    glm::uint32 size = DeviceObjects.m_WindowIcon.width * DeviceObjects.m_WindowIcon.height * 4;
    DeviceObjects.m_WindowIcon.pixels = new glm::uint8[size + 1];

    for (glm::uint32 i = 0; i < size + 1; i++) {
        DeviceObjects.m_WindowIcon.pixels[i] = icon.pixel_data[i];
    }

    glfwSetWindowIcon(DeviceObjects.m_pWindow, 1, &DeviceObjects.m_WindowIcon);

    glfwSetKeyCallback(DeviceObjects.m_pWindow, KeyCallback);
    glfwSetCursorPosCallback(DeviceObjects.m_pWindow, CursorCallback);
    glfwMakeContextCurrent(DeviceObjects.m_pWindow);
    glfwSetFramebufferSizeCallback(DeviceObjects.m_pWindow, FrameBufferCallBack);
    gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
    glfwSwapInterval(0);
    SetWindowSize(DEFAULT_SCREEN_WIDTH, DEFAULT_SCREEN_HEIGHT);
    SetFullscreen(DeviceObjects.m_bFullscreen);
    ClearColorBuffer();

    glfwSetInputMode(DeviceObjects.m_pWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    CreateDefaultTextures();

    DeviceObjects.m_pDefaultShader = std::make_shared<CShader>(vertexShader, fragmentShader, true);

}

void ABaseDevice::Update() {
    bool altPressed = false;
    bool enterPressed = false;
    static bool thisFrame = true;

    if (glfwGetKey(DeviceObjects.m_pWindow, GLFW_KEY_LEFT_ALT) == GLFW_PRESS)
        altPressed = true;

    if (altPressed && glfwGetKey(DeviceObjects.m_pWindow, GLFW_KEY_ENTER) == GLFW_PRESS)
        enterPressed = true;
    else
        thisFrame = true;

    if (thisFrame) {
        if (altPressed && enterPressed) {
            CenterWindowPosition();
            SetFullscreen(!DeviceObjects.m_bFullscreen);
            thisFrame = false;
        }
    }
}

void ABaseDevice::Shutdown() {
    RemoveSprite2DTextures();

    delete[] DeviceObjects.m_WindowIcon.pixels;
    glfwDestroyWindow(DeviceObjects.m_pWindow);
    glfwTerminate();
}

void ABaseDevice::CreateDefaultTextures() {
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

void ABaseDevice::RemoveSprite2DTextures() {
    glm::uint32 i = 0;
    glDeleteTextures(1, &i);
}

void ABaseDevice::UpdateViewport() {
    glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
}

void ABaseDevice::PollEvents() {
    glfwPollEvents();
}

void ABaseDevice::ClearColorBuffer() {
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClearDepth(1.0f);
    glDepthFunc(GL_LEQUAL);
    glEnable(GL_DEPTH_TEST);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void ABaseDevice::SwapBuffers() {
    glfwSwapBuffers(DeviceObjects.m_pWindow);
}

void ABaseDevice::SetFullscreen(bool on) {
    glfwSetWindowMonitor(DeviceObjects.m_pWindow, on ? DeviceObjects.m_pMonitor : NULL, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, GLFW_DONT_CARE);
    CenterWindowPosition();
    DeviceObjects.m_bFullscreen = on;
}

bool ABaseDevice::WindowShouldClose() {
    return static_cast<bool>(glfwWindowShouldClose(DeviceObjects.m_pWindow)) || DeviceObjects.m_bQuitGame;
}

void ABaseDevice::CenterWindowPosition() {
    if (!DeviceObjects.m_pMonitor)
        return;

    const GLFWvidmode* mode = glfwGetVideoMode(DeviceObjects.m_pMonitor);
    if (!mode)
        return;

    glm::int32 x, y;
    glfwGetMonitorPos(DeviceObjects.m_pMonitor, &x, &y);

    glm::int32 w, h;
    glfwGetWindowSize(DeviceObjects.m_pWindow, &w, &h);

    glfwSetWindowPos(DeviceObjects.m_pWindow,
        x + (mode->width - w) / 2,
        y + (mode->height - h) / 2);

    DeviceObjects.m_nMaxFPS = mode->refreshRate;
}

void ABaseDevice::SetWindowSize(glm::int32 width, glm::int32 height) {
    glfwHideWindow(DeviceObjects.m_pWindow);
    glfwSetWindowSizeLimits(DeviceObjects.m_pWindow, DEFAULT_SCREEN_WIDTH, DEFAULT_SCREEN_HEIGHT, GLFW_DONT_CARE, GLFW_DONT_CARE);
    glfwSetWindowSize(DeviceObjects.m_pWindow, width, height);
    CenterWindowPosition();
    glfwShowWindow(DeviceObjects.m_pWindow);
    FrameBufferCallBack(DeviceObjects.m_pWindow, width, height);
}

void ABaseDevice::ErrorCallback(glm::int32 error, const char* description) {
    std::cout << "Error: " << description << std::endl;
}

void ABaseDevice::KeyCallback(GLFWwindow* window, glm::int32 key, glm::int32 scancode, glm::int32 action, glm::int32 mods) {
    DeviceObjects.m_vOldInputKeys.at(key) = DeviceObjects.m_vNewInputKeys.at(key);
    DeviceObjects.m_vNewInputKeys.at(key) = action;
}

void ABaseDevice::CursorCallback(GLFWwindow* window, double x, double y) {
    DeviceObjects.m_vOldMousePosition = DeviceObjects.m_vMousePosition;
    DeviceObjects.m_vMousePosition.x = static_cast<float>(x);
    DeviceObjects.m_vMousePosition.y = static_cast<float>(y);
}

void ABaseDevice::FrameBufferCallBack(GLFWwindow* window, int width, int height) {
    Screen.GetScreenWidth() = width;
    Screen.GetScreenHeight() = height;
    Screen.GetAspectRatio() = width / (float)height;

    UpdateViewport();
}
       
void ABaseDevice::BeginDrawing() {
    DeviceObjects.m_fCurrentTime = glfwGetTime();
    DeviceObjects.m_fTimeDifference = DeviceObjects.m_fCurrentTime - DeviceObjects.m_fPreviousTime;

    DeviceObjects.m_nFrameCounter++;
    DeviceObjects.m_fFramePerSecond = (1.0 / DeviceObjects.m_fTimeDifference) * DeviceObjects.m_nFrameCounter;
    DeviceObjects.m_fDeltaTime = (DeviceObjects.m_fTimeDifference / DeviceObjects.m_nFrameCounter);

    std::string fps = std::to_string(static_cast<glm::uint32>(DeviceObjects.m_fFramePerSecond));
    std::string ms = std::to_string((DeviceObjects.m_fTimeDifference / DeviceObjects.m_nFrameCounter) * 1000.0);
    std::string title = "havoc2 - " + fps + "FPS / " + ms + "ms";
    glfwSetWindowTitle(DeviceObjects.m_pWindow, title.c_str());

    if (Abs(distance(DeviceObjects.m_vOldMousePosition, DeviceObjects.m_vMousePosition)) > 0.0f) {
        DeviceObjects.m_vMouseDelta.x = DeviceObjects.m_vMousePosition.x - DeviceObjects.m_vOldMousePosition.x;
        DeviceObjects.m_vMouseDelta.y = DeviceObjects.m_vOldMousePosition.y - DeviceObjects.m_vMousePosition.y;

        DeviceObjects.m_vOldMousePosition = DeviceObjects.m_vMousePosition;
    }

    //std::fill(DeviceObjects.oldInputKeys->begin(), DeviceObjects.oldInputKeys->end(), 0);
    //std::fill(DeviceObjects.newInputKeys->begin(), DeviceObjects.oldInputKeys->end(), 0);

    ClearColorBuffer();
}

void ABaseDevice::EndDrawing() {
    PollEvents();
    SwapBuffers();
    DeviceObjects.m_vMouseDelta = glm::vec2(0.0f, 0.0f);

    glDisable(GL_BLEND);

    if (DeviceObjects.m_fTimeDifference > 1.0 / static_cast<double>(DeviceObjects.m_nMaxFPS)) {
        DeviceObjects.m_fPreviousTime = DeviceObjects.m_fCurrentTime;
        DeviceObjects.m_nFrameCounter = 0;
    }

    while (glfwGetTime() < DeviceObjects.m_fPreviousTime + 1.0 / static_cast<double>(DeviceObjects.m_nMaxFPS));
}

void ABaseDevice::BeginScene3D() {
    Camera.GetScene3D() = true;
    Camera.ComputeProjection();

    glDisable(GL_STENCIL_TEST);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

}

void ABaseDevice::EndScene3D() {
    Camera.GetScene3D() = false;
    Camera.ComputeProjection();

    glDisable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);
}

bool ABaseDevice::GetKeyDown(glm::uint32 key) {
    return DeviceObjects.m_vNewInputKeys.at(key);
}

bool ABaseDevice::GetKeyJustDown(glm::uint32 key) {
    return DeviceObjects.m_vNewInputKeys.at(key) && !DeviceObjects.m_vOldInputKeys.at(key);
}

bool ABaseDevice::GetKeyUp(glm::uint32 key) {
    return !DeviceObjects.m_vNewInputKeys.at(key) && DeviceObjects.m_vOldInputKeys.at(key);
}
