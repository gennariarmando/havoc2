#pragma once
#include "ABaseHeader.h"
#include "Shader.h"

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

struct tDeviceObjects {
    GLFWwindow* m_pWindow;
    GLFWmonitor* m_pMonitor;
    bool m_bQuitGame;
    glm::int32 m_nMaxFPS;
    bool m_bFullscreen;
    GLFWimage m_WindowIcon;
    double m_fDeltaTime;
    double m_fCurrentTime;
    double m_fPreviousTime;
    double m_fTimeDifference;
    glm::uint32 m_nFrameCounter;
    double m_fFramePerSecond;
    glm::vec2 m_vMousePosition;
    glm::vec2 m_vOldMousePosition;
    glm::vec2 m_vMouseDelta;
    std::vector<glm::int32> m_vOldInputKeys;
    std::vector<glm::int32> m_vNewInputKeys;
    std::shared_ptr<CShader> m_pDefaultShader;
};

extern tDeviceObjects DeviceObjects;

class ABaseDevice {
public:
    ABaseDevice();
    ~ABaseDevice();

    void Init();
    void Update();

    void Shutdown();
    void CreateDefaultTextures();
    void RemoveSprite2DTextures();
    void SetFullscreen(bool on);
    bool WindowShouldClose();
    void CenterWindowPosition();
    void SetWindowSize(glm::int32 width, glm::int32 height);
    void PollEvents();
    void ClearColorBuffer();
    void SwapBuffers();

    static void UpdateViewport();
    static void ErrorCallback(glm::int32 error, const char* description);
    static void KeyCallback(GLFWwindow* window, glm::int32 key, glm::int32 scancode, glm::int32 action, glm::int32 mods);
    static void CursorCallback(GLFWwindow* window, double x, double y);
    static void FrameBufferCallBack(GLFWwindow* window, int width, int height);

    void BeginDrawing();
    void EndDrawing();
    void BeginScene3D();
    void EndScene3D();
    bool GetKeyDown(glm::uint32 key);
    bool GetKeyJustDown(glm::uint32 key);
    bool GetKeyUp(glm::uint32 key);
};

extern std::unique_ptr<ABaseDevice> BaseDevice;
