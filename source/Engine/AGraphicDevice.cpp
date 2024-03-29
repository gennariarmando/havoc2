#include "AGraphicDevice.h"
#include "AScreen.h"
#include "AConsole.h"
#include "AEngineSettings.h"
#include "version.h"
#include "icon.h"
#include "AFileLoader.h"
#include "AInput.h"
#include "ATime.h"

AGraphicDevice GraphicDevice;

std::vector<std::string> defaultShadersNames = {
	"sprites",
	"meshes",
};

AGraphicDevice::AGraphicDevice() {
	m_pWindow = nullptr;
	m_pMonitor = nullptr;
	m_vShaders.resize(NUM_DEFAULT_SHADERS);
	m_bPreviousCursorMode = true;
	m_bCursorMode = true;
	m_vVideoModes = {};
}

bool AGraphicDevice::Init() {
	glfwSetErrorCallback([](glm::int32 errorCode, const char* description) {
		Console.WriteLine(std::format("GLFW error occurred: {}", description));
		});

	if (!glfwInit()) {
		Console.WriteLine("GLFW error intializing library");
		return false;
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	Console.WriteLine(std::format("GLFW {}", glfwGetVersionString()));

	GLFWwindow* window = glfwCreateWindow(Screen.m_nDefaultWidth, Screen.m_nDefaultHeight, GAME_TITLE, NULL, NULL);	
	if (!window) {
		Console.WriteLine("GLFW error creating OpenGL window context");
		return false;
	}
	m_pWindow = window;

	GLFWmonitor* monitor = glfwGetPrimaryMonitor();
	if (!monitor) {
		Console.WriteLine("GLFW error finding primary monitor");
		return false;
	}
	m_pMonitor = monitor;

	GLFWimage image = {};
	image.width = icon.width;
	image.height = icon.width;
	image.pixels = icon.pixel_data;

	if (image.pixels)
		glfwSetWindowIcon(window, 1, &image);
	else
		Console.WriteLine("GLFW error creating an icon for the executable");

	glfwMakeContextCurrent(window);

	glfwSetKeyCallback(window, [](GLFWwindow* window, glm::int32 key, glm::int32 scancode, glm::int32 action, glm::int32 mods){
		Input.m_TempKeyboard.key[key] = action;
		});

	glfwSetCursorPosCallback(window, [](GLFWwindow* window, double x, double y) {
		Input.m_TempMouse.pos.x = static_cast<float>(x);
		Input.m_TempMouse.pos.y = static_cast<float>(y);
		});

	glfwSetMouseButtonCallback(window, [](GLFWwindow* window, glm::int32 button, glm::int32 action, glm::int32 mods) {
		Input.m_TempMouse.button[button] = action;
		});

	glfwSetScrollCallback(window, [](GLFWwindow* window, double xoffset, double yoffset) {
		Input.m_TempMouse.wheel.x = static_cast<float>(xoffset);
		Input.m_TempMouse.wheel.y = static_cast<float>(yoffset);
		});

	glfwSetFramebufferSizeCallback(window, ([](GLFWwindow* window, glm::int32 width, glm::int32 height) {
		UpdateViewport(width, height);
		}));

	gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

	SetCursorOnOff(true);

	MakeListOfVideoModes();
	SetWindowSize(EngineSettings.m_nScreenWidth, EngineSettings.m_nScreenHeight);
	SetFullscreen(EngineSettings.m_bFullscreen);
	SetVSync(EngineSettings.m_bVSync);

	glm::uint32 i = 0;
	for (auto& shaderName : defaultShadersNames) {
		std::string fs;
		std::string vs;
		if (AFileLoader::LoadShadersFromFile("shaders/" + shaderName + ".vs", "shaders/" + shaderName + ".fs", vs, fs)) {
			AShader* s = new AShader();
			s->CreateShader(vs, fs);
			m_vShaders.at(i) = s;;
		}
		i++;
	}

	SetRefreshRate(-1);

	BeginFrame();
	EndFrame();

	CenterMousePosition();

	return true;
}

void AGraphicDevice::Shutdown() {
	for (auto& it : m_vShaders)
		delete it;

	glfwDestroyWindow(m_pWindow);
	glfwTerminate();
}

void AGraphicDevice::UpdateViewport(glm::uint32 width, glm::uint32 height) {
	Screen.m_nScreenWidth = width;
	Screen.m_nScreenHeight = height;
	Screen.m_fAspectRatio = width / static_cast<float>(height);

	glViewport(0, 0, width, height);
}

void AGraphicDevice::SetWindowSize(glm::uint32 width, glm::uint32 height) {
	glfwHideWindow(m_pWindow);
	glfwSetWindowSizeLimits(m_pWindow, Screen.m_nDefaultWidth, Screen.m_nDefaultHeight, GLFW_DONT_CARE, GLFW_DONT_CARE);
	glfwSetWindowSize(m_pWindow, width, height);
	CenterWindowPosition();
	glfwShowWindow(m_pWindow);
	UpdateViewport(width, height);
}

void AGraphicDevice::CenterWindowPosition() {
	if (!m_pMonitor || !m_pWindow)
		return;

	const GLFWvidmode* mode = glfwGetVideoMode(m_pMonitor);
	if (!mode)
		return;

	glm::int32 x, y;
	glfwGetMonitorPos(m_pMonitor, &x, &y);

	glm::int32 w, h;
	glfwGetWindowSize(m_pWindow, &w, &h);

	glfwSetWindowPos(m_pWindow,
		x + (mode->width - w) / 2,
		y + (mode->height - h) / 2);
}

void AGraphicDevice::CenterMousePosition() {
	if (!m_pWindow)
		return;

	glm::int32 w, h;
	glfwGetWindowSize(m_pWindow, &w, &h);

	glfwSetCursorPos(m_pWindow, (w * 0.5), (h * 0.5));
}

void AGraphicDevice::SetFullscreen(bool on) {
	glfwSetWindowMonitor(m_pWindow, on ? m_pMonitor : NULL, 0, 0, Screen.m_nScreenWidth, Screen.m_nScreenHeight, GLFW_DONT_CARE);
	CenterWindowPosition();
	Screen.m_bFullscreen = on;
}

void AGraphicDevice::SetVSync(bool on) {
	glfwSwapInterval(on);
}

void AGraphicDevice::BeginFrame() {
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void AGraphicDevice::EndFrame() {
	glfwPollEvents();
	SwapBuffers();
}

void AGraphicDevice::SwapBuffers() {
	glfwSwapBuffers(m_pWindow);
}

void AGraphicDevice::SetCursorOnOff(bool on) {
	m_bCursorMode = on;
}

void AGraphicDevice::SetRefreshRate(glm::int32 rate) {
	if (rate == -1) {
		const GLFWvidmode* mode = glfwGetVideoMode(m_pMonitor);
		rate = mode->refreshRate;
	}
	Screen.m_nRefreshRate = rate;
}

void AGraphicDevice::SetWindowTitleBarCounter() {
	std::string fps = std::to_string(static_cast<glm::uint32>(Time.GetFramePerSecond()));
	std::string ms = std::to_string((Time.GetTimeDifference() / Time.GetFrameCounter()) * 1000.0);
	std::string title = GAME_TITLE " - " + fps + "FPS / " + ms + "ms";
	glfwSetWindowTitle(GraphicDevice.m_pWindow, title.c_str());
}

void AGraphicDevice::Update() {
	SetWindowTitleBarCounter();

	if (Input.GetKeyDown(KEY_LEFT_ALT) && Input.GetKeyJustDown(KEY_ENTER)) {
		SetFullscreen(!Screen.m_bFullscreen);
		EngineSettings.m_bFullscreen = Screen.m_bFullscreen;
		Input.Clear();
	}

	if (m_bCursorMode != m_bPreviousCursorMode) {
		if (m_bCursorMode)
			glfwSetInputMode(m_pWindow, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		else
			glfwSetInputMode(m_pWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

		m_bPreviousCursorMode = m_bCursorMode;
	}
}

void AGraphicDevice::MakeListOfVideoModes() {
	glm::int32 count;
	const GLFWvidmode* modes = glfwGetVideoModes(m_pMonitor, &count);

	for (glm::int32 i = 0; i < count; i++) {
		GLFWvidmode mode = modes[i];

		bool found = false;
		for (auto v : m_vVideoModes) {
			if (v.width == mode.width && v.height == mode.height) {
				found = true;
				break;
			}
		}

		if (!found && mode.width >= DEFAULT_SCREEN_WIDTH && mode.height >= DEFAULT_SCREEN_HEIGHT) {
			tVideoModes v;

			v.width = mode.width;
			v.height = mode.height;
			v.str = std::format("{}x{}", v.width, v.height);
			m_vVideoModes.push_back(v);
		}
	}

	if (ResToIndex(EngineSettings.m_nScreenWidth, EngineSettings.m_nScreenHeight) == -1) {
		tVideoModes v;

		v.width = EngineSettings.m_nScreenWidth;
		v.height = EngineSettings.m_nScreenHeight;
		v.str = std::format("{}x{}", v.width, v.height);
		m_vVideoModes.push_back(v);
		EngineSettings.m_nVideoMode = m_vVideoModes.size() - 1;
	}
}

glm::int32 AGraphicDevice::ResToIndex(glm::int32 width, glm::int32 height) {
	glm::int32 i = 0;
	for (auto& v : m_vVideoModes) {
		if (v.width == width && v.height == height) {
			return i;
		}
		i++;
	}
	return -1;
}

void AGraphicDevice::SetVideoMode(glm::uint32 index) {
	SetWindowSize(m_vVideoModes.at(index).width, m_vVideoModes.at(index).height);
}

