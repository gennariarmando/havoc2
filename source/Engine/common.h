#pragma once
// STD - Standard library
#include <list>
#include <stdio.h>
#include <memory>
#include <format>
#include <functional>
#include <filesystem>
#include <cstdint>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <chrono>
#include <thread>

// GLM - OpenGL Mathematics: https://github.com/g-truc/glm
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

// Pugixml - XML parser: https://github.com/zeux/pugixml
#include "..\..\vendor\pugixml\src\pugixml.hpp"

// Engine
#include "config.h"

// Utilities
template<typename T>
static T Abs(T a) {
    return (((a) < 0) ? (-(a)) : (a));
}

static void InterpF(float a, float b, float f) {
    a = a + (f) * (b - a);
}

template<typename T, typename T2, typename T3>
static T Clamp(T v, T2 low, T3 high) {
    return ((v) < (low) ? (low) : (v) > (high) ? (high) : (v));
}

template<typename T, typename T2, typename T3>
static T ClampInverse(T v, T2 low, T3 high) {
    return ((v) < (low) ? (high) : (v) > (high) ? (low) : (v));
}

static float IsNearlyEqualF(float a, float b, float t) {
    return (Abs(a - b) <= t) ? true : false;
}

template<typename T>
static glm::int32 ArraySize(T a) {
    return (sizeof(a) / sizeof(a[0]));
}

static float DegToRad(float x) {
    return static_cast<float>(x * glm::pi<float>() / 180.0f);
}

static float RadToDeg(float x) {
    return static_cast<float>(x * 180.0f / glm::pi<float>());
}

static glm::int32 SetBit(glm::int32 const& num, glm::int32 const& position) {
    glm::int32 mask = 1 << position;
    return num | mask;
}

static glm::uint8 GetBit(glm::int32 const& num, glm::int32 const& position) {
    glm::uint8 bit = (num & (1 << position)) == (1 << position);
    return bit;
}

static glm::int32 ClearBit(glm::int32 const& num, glm::int32 const& position) {
    glm::int32 mask = 1 << position;
    return num & ~mask;
}

static glm::vec2 RotateUV(glm::vec2 uv, float rotation, glm::vec2 center) {
    glm::vec2 v = {
        glm::cos(rotation) * (uv.x - center.x) + glm::sin(rotation) * (uv.y - center.y) + center.x,
        glm::cos(rotation) * (uv.y - center.y) - glm::sin(rotation) * (uv.x - center.x) + center.y
    };

    return v;
}

static glm::vec4 ToVec4(glm::uint8 r, glm::uint8 g, glm::uint8 b, glm::uint8 a) {
    return { r / 255.0f, g / 255.0f, b / 255.0f, a / 255.0f };
}

template<typename ... Args>
std::string string_format(const std::string& format, Args ... args) {
    int size_s = std::snprintf(nullptr, 0, format.c_str(), args ...) + 1;
    if (size_s <= 0)
        throw std::runtime_error("Error during formatting.");
    auto size = static_cast<size_t>(size_s);
    std::unique_ptr<char[]> buf(new char[size]);
    std::snprintf(buf.get(), size, format.c_str(), args ...);
    return std::string(buf.get(), buf.get() + size - 1);
}

static float LimitAngle(float angle) {
    float result = angle;

    while (result >= 180.0f) {
        result -= 2 * 180.0f;
    }

    while (result < -180.0f) {
        result += 2 * 180.0f;
    }

    return result;
}

static bool DecomposeMatrix(glm::mat4 const& transformation, glm::vec3& scale, glm::quat& rotation, glm::vec3& translation, glm::vec3& skew, glm::vec4& perspective) {
    glm::decompose(transformation, scale, rotation, translation, skew, perspective);
    return true;
}
