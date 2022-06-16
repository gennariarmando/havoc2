#pragma once
#include "common.h"

class AFileLoader {
public:
	static bool LoadShadersFromFile(const std::string& vertexPath, const std::string& fragmentPath, std::string& vertexShader, std::string& fragmentShader);
	static bool LoadTGAFromFile(const std::string& path, glm::uint16& width, glm::uint16& height, std::shared_ptr<glm::uint8[]>& pixels);
};
