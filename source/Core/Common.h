#pragma once
#include "ABaseHeader.h"

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
