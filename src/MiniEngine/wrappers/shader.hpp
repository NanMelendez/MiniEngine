#ifndef __MINIENGINE_SHADER_PROGRAM__
#define __MINIENGINE_SHADER_PROGRAM__

#include "../core/baseobject.hpp"
#include "../extras/loader.hpp"

namespace MiniEngine {
    enum class ShaderType : i32 {
        VERTEX   = 0,
        FRAGMENT = 1,
        GEOMETRY = 2
    };

    class ShaderProgram final : public Core::BaseObject {
    public:
        template<typename T>
        friend class Loader;

        ShaderProgram() {}

        ShaderProgram(const ShaderProgram&) = delete;
        ShaderProgram& operator=(const ShaderProgram&) = delete;

        ShaderProgram(ShaderProgram&&) noexcept = default;
        ShaderProgram& operator=(ShaderProgram&&) noexcept = default;

        ~ShaderProgram() {
            clear();
        }

        void use() const {
            glUseProgram(id);
        }

        void unuse() const {
            glUseProgram(0);
        }
        
        void clear() override final {
            if (isAllocated()) {
                glDeleteProgram(id);
                id = 0;
            }
        }

        std::string_view getShaderSource(ShaderType type) const {
            switch (type) {
            default:
            case ShaderType::VERTEX: return vertexSource;
            case ShaderType::FRAGMENT: return fragmentSource;
            case ShaderType::GEOMETRY: return geometrySource;
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
        std::string vertexSource;
        std::string fragmentSource;
        std::string geometrySource;

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

    template<>
    class Loader<ShaderProgram> {
    public:
        static ShaderProgram load(const std::string& vertexPath, const std::string& fragmentPath, const std::string& geometryPath = "") {
            ShaderProgram shader;

            shader.id = glCreateProgram();

            reload(shader, vertexPath, fragmentPath, geometryPath);

            return shader;
        }

        static void reload(ShaderProgram& shader, const std::string& vertexPath, const std::string& fragmentPath, const std::string& geometryPath = "") {
            constructSources(shader, vertexPath, fragmentPath, geometryPath);

            GLuint vertexID = createShader(GL_VERTEX_SHADER, shader.vertexSource.c_str());
            GLuint fragmentID = createShader(GL_FRAGMENT_SHADER, shader.fragmentSource.c_str());
            GLuint geometryID = 0;
            if (!shader.geometrySource.empty())
                geometryID = createShader(GL_GEOMETRY_SHADER, shader.geometrySource.c_str());
            
            glAttachShader(shader.id, vertexID);
            glAttachShader(shader.id, fragmentID);
            if (!!geometryID)
                glAttachShader(shader.id, geometryID);
            
            glLinkProgram(shader.id);
            checkLinkingStatus(shader);

            glDeleteShader(vertexID);
            glDeleteShader(fragmentID);
            if (geometryID)
                glDeleteShader(geometryID);
        }

    private:
        static void constructSources(ShaderProgram& shader, const std::string& vertexPath, const std::string& fragmentPath, const std::string& geometryPath = "") {
            std::ifstream vertexFile(vertexPath);
            std::getline(vertexFile, shader.vertexSource, '\0');
            vertexFile.close();

            std::ifstream fragmentFile(fragmentPath);
            std::getline(fragmentFile, shader.fragmentSource, '\0');
            fragmentFile.close();

            if (!geometryPath.empty()) {
                std::ifstream geometryFile(geometryPath);
                std::getline(geometryFile, shader.geometrySource, '\0');
                geometryFile.close();
            }
        }

        static GLuint createShader(GLenum type, const i8* src) {
            u32 shaderID = glCreateShader(type);
            glShaderSource(shaderID, 1, &src, NULL);
            glCompileShader(shaderID);
            checkCompileStatus(type, shaderID);
            return shaderID;
        }

        static void checkCompileStatus(GLenum type, u32 shaderID) {
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

        static void checkLinkingStatus(ShaderProgram& shader) {
            i32 success;

            glGetProgramiv(shader.id, GL_LINK_STATUS, &success);
            if (!success) {
                i32 maxLogLength = 0;
                glGetProgramiv(shader.id, GL_INFO_LOG_LENGTH, &maxLogLength);
                std::string infoLog(maxLogLength, '\0');
                glGetProgramInfoLog(shader.id, maxLogLength, &maxLogLength, &infoLog[0]);
                std::cerr << "ERROR::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
            }
        }
    };
}

#endif