#pragma once
#include "common.h"
#include "ATexture2D.h"
#include "AShader.h"

enum eDefaultShaders {
	SHADER_SPRITES,
	SHADER_MESHES,
	NUM_DEFAULT_SHADERS,
};

enum eRenderStates {
	RENDERSTATE_SRCBLEND,
	RENDERSTATE_DESTBLEND,
	RENDERSTATE_VERTEXALPHAENABLE,
	RENDERSTATE_STENCILTESTENABLE,
	RENDERSTATE_DEPTHTESTENABLE,
	RENDERSTATE_DEPTHMODE,
	RENDERSTATE_CULLMODE,
	RENDERSTATE_CULLENABLE,
};

enum eDepthMode {
	DEPTHTEST_NEVER = 0x0200,
	DEPTHTEST_LESS = 0x0201,
	DEPTHTEST_EQUAL = 0x0202,
	DEPTHTEST_LEQUAL = 0x0203,
	DEPTHTEST_GREATER = 0x0204,
	DEPTHTEST_NOTEQUAL = 0x0205,
	DEPTHTEST_GEQUAL = 0x0206,
	DEPTHTEST_ALWAYS = 0x0207,
};

enum eCullMode {
	CULLMODE_NONE = 0,
	CULLMODE_FRONT_LEFT = 0x0400,
	CULLMODE_FRONT_RIGHT = 0x0401,
	CULLMODE_BACK_LEFT = 0x0402,
	CULLMODE_BACK_RIGHT = 0x0403,
	CULLMODE_FRONT = 0x0404,
	CULLMODE_BACK = 0x0405,
	CULLMODE_LEFT = 0x0406,
	CULLMODE_RIGHT = 0x0407,
	CULLMODE_FRONT_AND_BACK = 0x0408,
};

class AGraphicDevice {
public:
	GLFWwindow* m_pWindow;
	GLFWmonitor* m_pMonitor;
	std::vector<AShader*> m_vShaders;

public:
	AGraphicDevice();
	bool Init();
	void Shutdown();
	
	void SetWindowSize(glm::uint32 width, glm::uint32 height);
	void CenterWindowPosition();
	void SetFullscreen(bool on);
	void BeginFrame();
	void EndFrame();
	void SwapBuffers();
	void SetCursorOnOff(bool on);

public:
	static void UpdateViewport(glm::uint32 width, glm::uint32 height);

};

extern AGraphicDevice GraphicDevice;

template <typename T>
static bool SetGraphicDeviceRenderState(eRenderStates state, T value) {
	switch (state) {
	case RENDERSTATE_SRCBLEND:
		glBlendFunc(GL_SRC_ALPHA, value);
		break;
	case RENDERSTATE_DESTBLEND:
		glBlendFunc(GL_DST_ALPHA, value);
		break;
	case RENDERSTATE_VERTEXALPHAENABLE:
		if (value)
			glEnable(GL_BLEND);
		else
			glDisable(GL_BLEND);
		break;
	case RENDERSTATE_STENCILTESTENABLE:
		if (value)
			glEnable(GL_STENCIL_TEST);
		else
			glDisable(GL_STENCIL_TEST);
		break;
	case RENDERSTATE_DEPTHTESTENABLE:
		if (value)
			glEnable(GL_DEPTH_TEST);
		else
			glDisable(GL_DEPTH_TEST);
		break;
	case RENDERSTATE_DEPTHMODE:
		glDepthFunc(value);
		break;
	case RENDERSTATE_CULLENABLE:
		if (value)
			glEnable(GL_CULL_FACE);
		else
			glDisable(GL_CULL_FACE);
		break;
	case RENDERSTATE_CULLMODE:
		glCullFace(value);
		break;
	default:
		return false;
	}
	return true;
}

#define SetRenderState(a, b) SetGraphicDeviceRenderState(a, b)
