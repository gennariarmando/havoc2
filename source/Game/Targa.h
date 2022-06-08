#pragma once
#include "Common.h"

class CTarga {
public:
    glm::uint8 imageTypeCode;
    glm::int16 imageWidth;
    glm::int16 imageHeight;
    glm::uint8 bitCount;
    std::shared_ptr<glm::uint8[]> imageData;

public:
    bool LoadTexture(std::string filename);
    void Free();
};
