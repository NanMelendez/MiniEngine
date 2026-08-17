#ifndef __MINIENGINE_SHADER_PROGRAM_LOADER__
#define __MINIENGINE_SHADER_PROGRAM_LOADER__

#include "../wrappers/shader.hpp"

namespace MiniEngine {
    template<>
    class Loader<ShaderProgram> {
    public:
        static ShaderProgram load(std::string_view vertexPath, std::string_view fragmentPath, std::string_view geometryPath = "", const std::vector<std::pair<std::string, GLuint>>& bindings = {}) {
            ShaderProgram shader;

            shader.id = glCreateProgram();

            reload(shader, vertexPath, fragmentPath, geometryPath, bindings);

            return shader;
        }

        static void reload(ShaderProgram& shader, std::string_view vertexPath, std::string_view fragmentPath, std::string_view geometryPath = "", const std::vector<std::pair<std::string, GLuint>>& bindings = {}) {
            shader.uniforms.clear();
            shader.uniformBlocks.clear();
            
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
            
            linkUBOBindings(shader, bindings);
            
            reflectUniforms(shader);
            reflectUniformBlocks(shader);
        }

    private:
        static void constructSources(ShaderProgram& shader, std::string_view vertexPath, std::string_view fragmentPath, std::string_view geometryPath = "") {
            std::ifstream vertexFile(std::string(vertexPath).c_str());
            std::getline(vertexFile, shader.vertexSource, '\0');
            vertexFile.close();

            std::ifstream fragmentFile(std::string(fragmentPath).c_str());
            std::getline(fragmentFile, shader.fragmentSource, '\0');
            fragmentFile.close();

            if (!geometryPath.empty()) {
                std::ifstream geometryFile(std::string(geometryPath).c_str());
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

        static void linkUBOBindings(ShaderProgram& shader, const std::vector<std::pair<std::string, GLuint>>& bindings) {
            for (const auto& [name, bindingPoint] : bindings)
                shader.linkUniformBlock(name, bindingPoint);
        }

        static void reflectUniforms(ShaderProgram& shader) {
            i32 uniformCount;
            i32 uniformMaxVarLength;

            glGetProgramiv(shader.id, GL_ACTIVE_UNIFORMS, &uniformCount);
            glGetProgramiv(shader.id, GL_ACTIVE_UNIFORM_MAX_LENGTH, &uniformMaxVarLength);

            std::vector<i8> nameBuffer(uniformMaxVarLength);

            for (i32 i = 0; i < uniformCount; i++) {
                UniformInfo info;

                GLsizei length;

                glGetActiveUniform(shader.id, i, uniformMaxVarLength, &length, &info.size, &info.type, nameBuffer.data());

                std::string name(nameBuffer.data(), length);
                info.location = glGetUniformLocation(shader.id, name.c_str());

                if (info.location == -1)
                    continue;

                shader.uniforms.emplace(name, std::move(info));
            }
        }

        static void reflectUniformBlocks(ShaderProgram& shader) {
            i32 uniformBlockCount;
            i32 uniformBlockMaxVarLength;

            glGetProgramiv(shader.id, GL_ACTIVE_UNIFORM_BLOCKS, &uniformBlockCount);
            glGetProgramiv(shader.id, GL_ACTIVE_UNIFORM_BLOCK_MAX_NAME_LENGTH, &uniformBlockMaxVarLength);

            std::vector<i8> nameBuffer(uniformBlockMaxVarLength);

            for (i32 i = 0; i < uniformBlockCount; i++) {
                UniformBlockInfo info;

                GLsizei length;

                glGetActiveUniformBlockName(shader.id, i, uniformBlockMaxVarLength, &length, nameBuffer.data());

                std::string name(nameBuffer.data(), length);
                info.index = glGetUniformBlockIndex(shader.id, name.c_str());

                if (info.index == -1)
                    continue;
                
                glGetActiveUniformBlockiv(shader.id, info.index, GL_UNIFORM_BLOCK_BINDING, &info.binding);
                glGetActiveUniformBlockiv(shader.id, info.index, GL_UNIFORM_BLOCK_DATA_SIZE, &info.size);
                glGetActiveUniformBlockiv(shader.id, info.index, GL_UNIFORM_BLOCK_ACTIVE_UNIFORMS, &info.activeUniforms);

                shader.uniformBlocks.emplace(name, std::move(info));
            }
        }
    };
}

#endif