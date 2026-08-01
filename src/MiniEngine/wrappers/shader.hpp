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
    };
}

#endif