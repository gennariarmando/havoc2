#pragma once
#include "common.h"

class AShader {
public:
    glm::uint32 m_nShaderProgram;

public:
    AShader();
    AShader(int i) { m_nShaderProgram = i; }
    AShader(std::string const& vertexPath, std::string const& fragmentPath);
    ~AShader();

    void CreateShader(std::string const& vertexShader, std::string const& fragmentShader);
    void Apply();
    void SetShaderUniform(const std::string& name, bool value);
    void SetShaderUniform(const std::string& name, glm::int32 value);
    void SetShaderUniform(const std::string& name, float value);
    void SetShaderUniform(const std::string& name, const glm::vec2& value);
    void SetShaderUniform(const std::string& name, float x, float y);
    void SetShaderUniform(const std::string& name, const glm::vec3& value);
    void SetShaderUniform(const std::string& name, float x, float y, float z);
    void SetShaderUniform(const std::string& name, const glm::vec4& value);
    void SetShaderUniform(const std::string& name, float x, float y, float z, float w);
    void SetShaderUniform(const std::string& name, const glm::mat2& mat);
    void SetShaderUniform(const std::string& name, const glm::mat3& mat);
    void SetShaderUniform(const std::string& name, const glm::mat4& mat);
};
