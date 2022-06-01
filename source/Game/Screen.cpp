#include "Screen.h"

CScreen Screen;

float CScreen::ScaleX(float x) {
    float f = ((x) * (float)SCREEN_WIDTH / DEFAULT_SCREEN_WIDTH) * DEFAULT_ASPECT_RATIO / SCREEN_ASPECT_RATIO;
    return f;
}

float CScreen::ScaleY(float y) {
    float f = ((y) * (float)SCREEN_HEIGHT / DEFAULT_SCREEN_HEIGHT);
    return f;
}

float CScreen::ScaleW(float w) {
    float f = ((w) * (float)SCREEN_WIDTH / DEFAULT_SCREEN_WIDTH) * DEFAULT_ASPECT_RATIO / SCREEN_ASPECT_RATIO;
    return f;
}

float CScreen::ScaleH(float h) {
    float f = ((h) * (float)SCREEN_HEIGHT / DEFAULT_SCREEN_HEIGHT);
    return f;
}

float CScreen::ScaleFromRight(float x) {
    return SCREEN_WIDTH - ScaleX(x);
}

float CScreen::ScaleFromBottom(float y) {
    return SCREEN_HEIGHT - ScaleY(y);
}