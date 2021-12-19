#ifndef COMPLEX_SHADER_H
#define COMPLEX_SHADER
#include<glfw/glfw3.h>
#include<glad/glad.h>
#include<glm/glm.hpp>

#include<vector>
#include<string>
#include<sstream>
#include<iostream>
#include<fstream>
#include<type_traits>

void checkCompilingError(GLuint id) {
    int success;
    glGetShaderiv(id, GL_COMPILE_STATUS, &success);

    if (!success) {
        constexpr int max_log_size = 512;
        char info_log[max_log_size];
        glGetShaderInfoLog(id, max_log_size, nullptr,
            static_cast<char*>(info_log));
        /*throw std::runtime_error{ fmt::format(
            "Shader compilation error for shader {}: {}", shader_id, info_log) };*/
        printf("Shader compilation error for shader %d : %s", id, info_log);
    }
}

class ComplexShader {
public:
	enum class Type :GLenum {
        Vertex = GL_VERTEX_SHADER,
        Fragment = GL_FRAGMENT_SHADER,
        TessControl = GL_TESS_CONTROL_SHADER,
        TessEval = GL_TESS_EVALUATION_SHADER,
        Geometry = GL_GEOMETRY_SHADER,
        Compute = GL_COMPUTE_SHADER
	};
    /*variables*/
    Type type;
    GLuint id;

    /*functions*/
    ComplexShader(const char* source,Type type) {
        type = type;
        //test 
        std::cout << std::underlying_type_t<Type>(type) << '\n';
        //
        id = glCreateShader(std::underlying_type_t<Type>(type));
        glShaderSource(id, 1, &source, nullptr);
        glCompileShader(id);
        checkCompilingError(id);
    }
    ~ComplexShader() {
        glDeleteShader(id);
    }
};

class ComplexShaderProgram {
public:
    /*variables*/
    GLuint id;

    ComplexShaderProgram(std::vector<ComplexShader>& shaders) {
        id = glCreateProgram();
        for (auto& shader : shaders) {
            glAttachShader(id, shader.id);
        }
        glLinkProgram(id);
    }
    /*functions*/
    void use() {
        glUseProgram(id);
    }
    void setBool(const std::string& name, bool value) const
    {
        glUniform1i(glGetUniformLocation(id, name.c_str()),
            static_cast<int>(value));
    }
    void setInt(const std::string& name, int value) const
    {
        glUniform1i(glGetUniformLocation(id, name.c_str()), value);
    }
    void setFloat(const std::string& name, float value) const
    {
        glUniform1f(glGetUniformLocation(id, name.c_str()), value);
    }
    void setVec2(const std::string& name, const glm::vec2& value) const
    {
        glUniform2fv(glGetUniformLocation(id, name.c_str()), 1, &value[0]);
    }
    void setVec2(const std::string& name, float x, float y) const
    {
        glUniform2f(glGetUniformLocation(id, name.c_str()), x, y);
    }
    void setVec3(const std::string& name, const glm::vec3& value) const
    {
        glUniform3fv(glGetUniformLocation(id, name.c_str()), 1, &value[0]);
    }
    void setVec3(const std::string& name, float x, float y, float z) const
    {
        glUniform3f(glGetUniformLocation(id, name.c_str()), x, y, z);
    }
    void setVec4(const std::string& name, const glm::vec4& value) const
    {
        glUniform4fv(glGetUniformLocation(id, name.c_str()), 1, &value[0]);
    }
    void setVec4(const std::string& name, float x, float y, float z,
        float w) const
    {
        glUniform4f(glGetUniformLocation(id, name.c_str()), x, y, z, w);
    }
    void setMat2(const std::string& name, const glm::mat2& mat) const
    {
        glUniformMatrix2fv(glGetUniformLocation(id, name.c_str()), 1, GL_FALSE,
            &mat[0][0]);
    }
    void setMat3(const std::string& name, const glm::mat3& mat) const
    {
        glUniformMatrix3fv(glGetUniformLocation(id, name.c_str()), 1, GL_FALSE,
            &mat[0][0]);
    }
    void setMat4(const std::string& name, const glm::mat4& mat) const
    {
        glUniformMatrix4fv(glGetUniformLocation(id, name.c_str()), 1, GL_FALSE,
            &mat[0][0]);
    }

};

class ShaderBuilder {
public:
    std::vector<ComplexShader> shaders;

    ShaderBuilder& load(std::string filename, ComplexShader::Type type) {
        filename = "../shader/grass/" + filename;
        std::ifstream file;
        file.open(filename);
        std::stringstream shaderStream;
        shaderStream << file.rdbuf();
        file.close();
        std::string code = shaderStream.str();
        ComplexShader shader(code.c_str(), type);
        shaders.push_back(shader);
        //test
        std::cout << "successful" << '\n';

        return *this;
    }
    ComplexShaderProgram* build() {
        ComplexShaderProgram* program = new ComplexShaderProgram(shaders);
        return program;
    }
};
#endif // !COMPLEX_SHADER_H

