#pragma once

#ifndef __MINIENGINE_CORE_SHADER_PROGRAM__
#define __MINIENGINE_CORE_SHADER_PROGRAM__

#include "coreBaseObject.hpp"

namespace MiniEngine {
    namespace Core {
        class Shader : public Object {
        public:
            Shader() {}

            Shader(const std::string& vertexPath, const std::string& fragmentPath, const std::string& geometryPath = "") { load(vertexPath, fragmentPath, geometryPath); }

            void use() const {
                glUseProgram(id);
            }

            void unuse() const {
                glUseProgram(0);
            }

            void load(const std::string& vertexPath, const std::string& fragmentPath, const std::string& geometryPath = "") {
                clear();

                std::ifstream vtxFile(vertexPath);
                std::string vtxSrc;
                std::getline(vtxFile, vtxSrc, '\0');
                vtxFile.close();

                std::ifstream frgFile(fragmentPath);
                std::string frgSrc;
                std::getline(frgFile, frgSrc, '\0');
                frgFile.close();

                u32 vtxID = createShader(GL_VERTEX_SHADER, vtxSrc.c_str());
                u32 frgID = createShader(GL_FRAGMENT_SHADER, frgSrc.c_str());

                u32 geoID = 0;
                if (!geometryPath.empty()) {
                    std::ifstream geoFile(geometryPath);
                    std::string geoSrc;
                    std::getline(geoFile, geoSrc, '\0');
                    geoFile.close();

                    geoID = createShader(GL_GEOMETRY_SHADER, geoSrc.c_str());
                }

                id = glCreateProgram();
                glAttachShader(id, vtxID);
                glAttachShader(id, frgID);
                if (!geoID)
                    glAttachShader(id, geoID);
                glLinkProgram(id);

                checkLinkingStatus();

                glDeleteShader(vtxID);
                glDeleteShader(frgID);
                if (geoID)
                    glDeleteShader(geoID);
            }

            void loadFromSrc(const std::string& vtxSrc, const std::string& frgSrc, const std::string& geoSrc = "") {
                clear();

                u32 vtxID = createShader(GL_VERTEX_SHADER, vtxSrc.c_str());
                u32 frgID = createShader(GL_FRAGMENT_SHADER, frgSrc.c_str());
                u32 geoID = !geoSrc.empty() ? createShader(GL_GEOMETRY_SHADER, geoSrc.c_str()) : 0;

                id = glCreateProgram();
                glAttachShader(id, vtxID);
                glAttachShader(id, frgID);
                if (geoID)
                    glAttachShader(id, geoID);
                glLinkProgram(id);

                checkLinkingStatus();

                glDeleteShader(vtxID);
                glDeleteShader(frgID);
                if (geoID)
                    glDeleteShader(geoID);
            }

            void clear() override {
                if (id) {
                    glDeleteProgram(id);
                    id = 0;
                }
            }

            void setBool(const std::string& name, bool value) const {
                glUniform1i(glGetUniformLocation(id, name.c_str()), value);
            }

            void setInt(const std::string& name, i32 value) const {
                glUniform1i(glGetUniformLocation(id, name.c_str()), value);
            }

            void setFloat(const std::string& name, f32 value) const {
                glUniform1f(glGetUniformLocation(id, name.c_str()), value);
            }

            void setVec2(const std::string& name, f32 x, f32 y) const {
                glUniform2f(glGetUniformLocation(id, name.c_str()), x, y);
            }

            void setVec2(const std::string& name, glm::vec2 value) const {
                glUniform2fv(glGetUniformLocation(id, name.c_str()), 1, glm::value_ptr(value));
            }

            void setVec3(const std::string& name, f32 x, f32 y, f32 z) const {
                glUniform3f(glGetUniformLocation(id, name.c_str()), x, y, z);
            }

            void setVec3(const std::string& name, glm::vec3 value) const {
                glUniform3fv(glGetUniformLocation(id, name.c_str()), 1, glm::value_ptr(value));
            }

            void setVec4(const std::string& name, f32 x, f32 y, f32 z, f32 w) const {
                glUniform4f(glGetUniformLocation(id, name.c_str()), x, y, z, w);
            }

            void setVec4(const std::string& name, glm::vec4 value) const {
                glUniform4fv(glGetUniformLocation(id, name.c_str()), 1, glm::value_ptr(value));
            }

            void setMat2(const std::string& name, glm::mat2 value, GLboolean transpose = GL_FALSE) const {
                glUniformMatrix2fv(glGetUniformLocation(id, name.c_str()), 1, transpose, glm::value_ptr(value));
            }

            void setMat3(const std::string& name, glm::mat3 value, GLboolean transpose = GL_FALSE) const {
                glUniformMatrix3fv(glGetUniformLocation(id, name.c_str()), 1, transpose, glm::value_ptr(value));
            }

            void setMat4(const std::string& name, glm::mat4 value, GLboolean transpose = GL_FALSE) const {
                glUniformMatrix4fv(glGetUniformLocation(id, name.c_str()), 1, transpose, glm::value_ptr(value));
            }

        private:
            u32 id = 0;

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
                    std::cout << "ERROR::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
                }
            }
        };
    }
}

#endif
