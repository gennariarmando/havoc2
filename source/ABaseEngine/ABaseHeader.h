#pragma once
#include <list>
#include <stdio.h>
#include <memory>
#include <format>

#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <chrono>
#include <thread>
#include <map>
#include <bitset>

// OpenGL Mathematics
#define GLM_FORCE_LEFT_HANDED
#include "glm\glm.hpp"
#include "glm\fwd.hpp"
#include "glm\gtc\matrix_transform.hpp"
#include "glm\gtc\type_ptr.hpp"
#include "glm\gtx\matrix_decompose.hpp"
#include "glm\gtx\rotate_vector.hpp"

// GLAD - GL Loader: https://github.com/Dav1dde/glad
#include "include\glad\glad.h"

// GLFW - Multiplatform library for OpenGL: https://github.com/glfw/glfw
#define GLFW_INCLUDE_NONE
#include "include\GLFW\glfw3.h"
