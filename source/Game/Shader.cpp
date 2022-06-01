#include "Shader.h"
#include "ABaseGL.h"

CShader::CShader() {
    m_nShaderProgram = NULL;
}

CShader::CShader(const std::string& vertexPath, const std::string& fragmentPath, bool text) {
    std::string vertexCode;
    std::string fragmentCode;

    if (text) {
        vertexCode = vertexPath;
        fragmentCode = fragmentPath;
    }
    else {
        std::ifstream vShaderFile;
        std::ifstream fShaderFile;
        vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
        fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
        try {
            vShaderFile.open(vertexPath);
            fShaderFile.open(fragmentPath);
            std::stringstream vShaderStream, fShaderStream;
            vShaderStream << vShaderFile.rdbuf();
            fShaderStream << fShaderFile.rdbuf();
            vShaderFile.close();
            fShaderFile.close();
            vertexCode = vShaderStream.str();
            fragmentCode = fShaderStream.str();
        }
        catch (std::ifstream::failure& e) {
            std::cout << "ERROR: Error reading shader file " << e.what() << std::endl;
        }        
    }

    const char* vShaderCode = vertexCode.c_str();
    const char* fShaderCode = fragmentCode.c_str();

    glm::uint32 vertex, fragment;
    vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vShaderCode, NULL);
    glCompileShader(vertex);

    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fShaderCode, NULL);
    glCompileShader(fragment);

    m_nShaderProgram = glCreateProgram();
    glAttachShader(m_nShaderProgram, vertex);
    glAttachShader(m_nShaderProgram, fragment);
    glLinkProgram(m_nShaderProgram);

    glDeleteShader(vertex);
    glDeleteShader(fragment);
}

CShader::~CShader() {

}

void CShader::Apply() {
    glUseProgram(m_nShaderProgram);
}

void CShader::SetShaderUniform(const std::string& name, bool value) {
    glUniform1i(glGetUniformLocation(m_nShaderProgram, name.c_str()), static_cast<glm::int32>(value));
}

void CShader::SetShaderUniform(const std::string& name, glm::int32 value) {
    glUniform1i(glGetUniformLocation(m_nShaderProgram, name.c_str()), value);
}

void CShader::SetShaderUniform(const std::string& name, float value) {
    glUniform1f(glGetUniformLocation(m_nShaderProgram, name.c_str()), value);
}

void CShader::SetShaderUniform(const std::string& name, const glm::vec2& value) {
    glUniform2fv(glGetUniformLocation(m_nShaderProgram, name.c_str()), 1, &value[0]);
}

void CShader::SetShaderUniform(const std::string& name, float x, float y) {
    glUniform2f(glGetUniformLocation(m_nShaderProgram, name.c_str()), x, y);
}

void CShader::SetShaderUniform(const std::string& name, const glm::vec3& value) {
    glUniform3fv(glGetUniformLocation(m_nShaderProgram, name.c_str()), 1, &value[0]);
}

void CShader::SetShaderUniform(const std::string& name, float x, float y, float z) {
    glUniform3f(glGetUniformLocation(m_nShaderProgram, name.c_str()), x, y, z);
}

void CShader::SetShaderUniform(const std::string& name, const glm::vec4& value) {
    glUniform4fv(glGetUniformLocation(m_nShaderProgram, name.c_str()), 1, &value[0]);
}

void CShader::SetShaderUniform(const std::string& name, float x, float y, float z, float w) {
    glUniform4f(glGetUniformLocation(m_nShaderProgram, name.c_str()), x, y, z, w);
}

void CShader::SetShaderUniform(const std::string& name, const glm::mat2& mat) {
    glUniformMatrix2fv(glGetUniformLocation(m_nShaderProgram, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}

void CShader::SetShaderUniform(const std::string& name, const glm::mat3& mat) {
    glUniformMatrix3fv(glGetUniformLocation(m_nShaderProgram, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}

void CShader::SetShaderUniform(const std::string& name, const glm::mat4& mat) {
    glUniformMatrix4fv(glGetUniformLocation(m_nShaderProgram, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}
