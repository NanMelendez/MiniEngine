#ifndef __MINIENGINE_SHADER_PROGRAM__
#define __MINIENGINE_SHADER_PROGRAM__

#include "../core/baseobject.hpp"

namespace MiniEngine {
    class ShaderProgram final : public Core::BaseObject {
    public:
        ShaderProgram() {}

        ShaderProgram(const std::string& pathVertex, const std::string& pathFragment, const std::string& pathGeometry = "") {
            load(pathVertex, pathFragment, pathGeometry);
        }

        void use() const {
            glUseProgram(id);
        }

        void unuse() const {
            glUseProgram(0);
        }

        void load(const std::string& pathVertex, const std::string& pathFragment, const std::string& pathGeometry = "") {
            clear();

            std::ifstream vertexFile(pathVertex);
            std::string vertexSrc;
            std::getline(vertexFile, vertexSrc, '\0');
            vertexFile.close();

            std::ifstream fragmentFile(pathFragment);
            std::string fragmentSrc;
            std::getline(fragmentFile, fragmentSrc, '\0');
            fragmentFile.close();

            u32 vertexID = createShader(GL_VERTEX_SHADER, vertexSrc.c_str());
            u32 fragmentID = createShader(GL_FRAGMENT_SHADER, fragmentSrc.c_str());

            u32 geometryID = 0;
            if (!pathGeometry.empty()) {
                std::ifstream geometryFile(pathGeometry);
                std::string geometrySrc;
                std::getline(geometryFile, geometrySrc, '\0');
                geometryFile.close();

                geometryID = createShader(GL_GEOMETRY_SHADER, geometrySrc.c_str());
            }

            id = glCreateProgram();
            glAttachShader(id, vertexID);
            glAttachShader(id, fragmentID);
            if (geometryID)
                glAttachShader(id, geometryID);
            
            glLinkProgram(id);
            
            checkLinkingStatus();

            glDeleteShader(vertexID);
            glDeleteShader(fragmentID);
            if (geometryID)
                glDeleteShader(geometryID);
        }

        void clear() override final {
            if (isAllocated()) {
                glDeleteProgram(id);
                id = 0;
            }
        }

        void setBool(std::string_view name, bool value) const {
            glUniform1i(getUniformLocation(name), value);
        }

        void setInt(std::string_view name, i32 value) const {
            glUniform1i(getUniformLocation(name), value);
        }

        void setFloat(std::string_view name, f32 value) const {
            glUniform1f(getUniformLocation(name), value);
        }

        void setVec2(std::string_view name, f32 x, f32 y) const {
            glUniform2f(getUniformLocation(name), x, y);
        }

        void setVec2(std::string_view name, const glm::vec2& value) const {
            glUniform2fv(getUniformLocation(name), 1, glm::value_ptr(value));
        }

        void setVec3(std::string_view name, f32 x, f32 y, f32 z) const {
            glUniform3f(getUniformLocation(name), x, y, z);
        }

        void setVec3(std::string_view name, const glm::vec3& value) const {
            glUniform3fv(getUniformLocation(name), 1, glm::value_ptr(value));
        }

        void setVec4(std::string_view name, f32 x, f32 y, f32 z, f32 w) const {
            glUniform4f(getUniformLocation(name), x, y, z, w);
        }

        void setVec4(std::string_view name, const glm::vec4& value) const {
            glUniform4fv(getUniformLocation(name), 1, glm::value_ptr(value));
        }

        void setMat2(std::string_view name, const glm::mat2& mat) const {
            glUniformMatrix2fv(getUniformLocation(name), 1, GL_FALSE, glm::value_ptr(mat));
        }

        void setMat3(std::string_view name, const glm::mat3& mat) const {
            glUniformMatrix3fv(getUniformLocation(name), 1, GL_FALSE, glm::value_ptr(mat));
        }

        void setMat4(std::string_view name, const glm::mat4& mat) const {
            glUniformMatrix4fv(getUniformLocation(name), 1, GL_FALSE, glm::value_ptr(mat));
        }

        void setSampler2D(std::string_view name, u32 slot) const {
            glUniform1i(getUniformLocation(name), slot);
        }

    private:
        u32 createShader(GLenum type, const i8* src) const {
            u32 shaderID = glCreateShader(type);
            glShaderSource(shaderID, 1, &src, NULL);
            glCompileShader(shaderID);
            checkCompileStatus(type, shaderID);
            return shaderID;
        }

        void checkCompileStatus(GLenum type, u32 shaderID) const {
            i32 success;

            glGetShaderiv(shaderID, GL_COMPILE_STATUS, &success);
            if (!success) {
                std::string shaderType = type == GL_VERTEX_SHADER ? "VERTEX" : (type == GL_FRAGMENT_SHADER ? "FRAGMENT" : (type == GL_GEOMETRY_SHADER ? "GEOMETRY" : "UNKNOWN_TYPE"));

                i32 maxLogLength = 0;
                glGetShaderiv(shaderID, GL_INFO_LOG_LENGTH, &maxLogLength);
                std::string infoLog(maxLogLength, '\0');
                glGetShaderInfoLog(shaderID, maxLogLength, &maxLogLength, &infoLog[0]);

                std::cerr << "ERROR::" << shaderType << "_SHADER::COMPILATION_FAILED\n" << infoLog << std::endl;
            }
        }

        void checkLinkingStatus() const {
            i32 success;

            glGetProgramiv(id, GL_LINK_STATUS, &success);
            if (!success) {
                i32 maxLogLength = 0;
                glGetProgramiv(id, GL_INFO_LOG_LENGTH, &maxLogLength);
                std::string infoLog(maxLogLength, '\0');
                glGetProgramInfoLog(id, maxLogLength, &maxLogLength, &infoLog[0]);
                std::cerr << "ERROR::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
            }
        }

        GLuint getUniformLocation(std::string_view name) const {
            return glGetUniformLocation(id, name.data());
        }
    };
}

#endif