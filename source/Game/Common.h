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

#define PI 3.14159265358979323846
#define PI_2 PI / 2
#define RADTODEG(x) x * (180.0f / PI)
#define DEGTORAD(x) x * (PI / 180.0f)

#define ABS(a)  (((a) < 0) ? (-(a)) : (a))
#define interpF(a, b, f) a = a + (f) * (b - a)
#define clamp(v, low, high) ((v)<(low) ? (low) : (v)>(high) ? (high) : (v))
#define isNearlyEqualF(a, b, t) (ABS(a - b) <= t) ? true : false
#define arraySize(array)(sizeof(array) / sizeof(array[0]))
#define elif else if

static float DegToRad(float x) {
    return (x * PI / 180.0f);
}

static float RadToDeg(float x) {
    return (x * 180.0f / PI);
}
