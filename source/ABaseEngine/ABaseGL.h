#pragma once
#include "Common.h"

enum eShaderUniformType {
    UTYPE_BOOL,
    UTYPE_INT,
    UTYPE_FLOAT,
    UTYPE_VEC2,
    UTYPE_VEC3,
    UTYPE_VEC4,
    UTYPE_MAT4,
};

enum eDefaultTextures {
    DEF_TEX_RECTANGLE,
};

class AGLObjects {
public:
    GLFWwindow* window;
    GLFWmonitor* monitor;
    bool quitGame;
    glm::int32 maxFPS;
    bool fullScreen;
    GLFWimage windowIcon;
    double deltaTime;
    double currentTime;
    double previousTime;
    double timeDifference;
    glm::uint32 frameCounter;
    double framePerSecond;
    glm::vec2 mousePosition;
    glm::vec2 oldMousePosition;
    glm::vec2 mouseDelta;
    glm::uint32 oldInputKeys[GLFW_KEY_MENU];
    glm::uint32 newInputKeys[GLFW_KEY_MENU];
    std::unique_ptr<class CShader> defaultShader;
};

class ABaseGL {
private:
    static AGLObjects m_Objects;

public:
    ABaseGL();
    ~ABaseGL();

    static void Init();
    static void Shutdown();
    static void CreateDefaultTextures();
    static void RemoveSprite2DTextures();
    static void SetFullscreen(bool on);
    static bool WindowShouldClose();
    static void CenterWindowPosition();
    static void SetWindowSize(glm::int32 width, glm::int32 height);
    static void UpdateViewport();
    static void PollEvents();
    static void ClearColorBuffer();
    static void SwapBuffers();
    static void ErrorCallback(glm::int32 error, const char* description);
    static void KeyCallback(GLFWwindow* window, glm::int32 key, glm::int32 scancode, glm::int32 action, glm::int32 mods);
    static void CursorCallback(GLFWwindow* window, double x, double y);
    static void FrameBufferCallBack(GLFWwindow* window, int width, int height);
    static void BeginDrawing();
    static void EndDrawing();
    static void BeginScene3D();
    static void EndScene3D();
    static bool GetKeyDown(glm::uint32 key);
    static bool GetKeyUp(glm::uint32 key);

public:
    static AGLObjects& GetObjects() { return m_Objects; }
    static GLFWwindow* const& GetWindow() { return GetObjects().window; }
    static GLFWmonitor* const& GetMonitor() { return GetObjects().monitor; }
    static bool const& GetQuitGame() { return GetObjects().quitGame; }
    static glm::int32 const& GetMaxFPS() { return GetObjects().maxFPS; }
    static bool const& GetFullScreen() { return GetObjects().fullScreen; }
    static GLFWimage const& GetWindowIcon() { return GetObjects().windowIcon; }
    static double const& GetDeltaTime() { return GetObjects().deltaTime; }
    static double const& GetCurrentTime() { return GetObjects().currentTime; }
    static double const& GetPreviousTime() { return GetObjects().previousTime; }
    static double const& GetTimeDifference() { return GetObjects().timeDifference; }
    static glm::uint32 const& GetFrameCounter() { return GetObjects().frameCounter; }
    static double const& GetFramePerSecond() { return GetObjects().framePerSecond; }
    static glm::vec2 const& GetMousePosition() { return GetObjects().mousePosition; }
    static glm::vec2 const& GetPreviousMousePosition() { return GetObjects().oldMousePosition; }
    static glm::vec2 const& GetMouseDelta() { return GetObjects().mouseDelta; }
    static glm::uint32* const GetOldInputKeys() { return GetObjects().oldInputKeys; }
    static glm::uint32* const GetNewInputKeys() { return GetObjects().newInputKeys; }
    static std::unique_ptr<class CShader> const& GetDefaultShader() { return GetObjects().defaultShader; }

};
