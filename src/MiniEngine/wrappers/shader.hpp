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

    struct UniformInfo {
        GLint location;
        GLenum type;
        GLint size;
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

        const std::unordered_map<std::string, UniformInfo>& getUniformList() const {
            return uniforms;
        }

        void setBool(std::string_view name, bool value) const {
            const UniformInfo* info = getUniform(name);

            if (validateUniformPtr(info, GL_BOOL))
                glUniform1i(info->location, value);
        }

        void setInt(std::string_view name, i32 value) const {
            const UniformInfo* info = getUniform(name);

            if (validateUniformPtr(info, GL_INT))
                glUniform1i(info->location, value);
        }

        void setUInt(std::string_view name, u32 value) const {
            const UniformInfo* info = getUniform(name);

            if (validateUniformPtr(info, GL_UNSIGNED_INT))
                glUniform1ui(info->location, value);
        }

        void setFloat(std::string_view name, f32 value) const {
            const UniformInfo* info = getUniform(name);

            if (validateUniformPtr(info, GL_FLOAT))
                glUniform1f(info->location, value);
        }

        void setVec2(std::string_view name, f32 x, f32 y)  const {
            const UniformInfo* info = getUniform(name);

            if (validateUniformPtr(info, GL_FLOAT_VEC2))
                glUniform2f(info->location, x, y);
        }

        void setVec2(std::string_view name, glm::vec2 value) const {
            const UniformInfo* info = getUniform(name);

            if (validateUniformPtr(info, GL_FLOAT_VEC2))
                glUniform2fv(info->location, 1, glm::value_ptr(value));
        }

        void setIVec2(std::string_view name, i32 x, i32 y) const {
            const UniformInfo* info = getUniform(name);

            if (validateUniformPtr(info, GL_INT_VEC2))
                glUniform2i(info->location, x, y);
        }

        void setIVec2(std::string_view name, const glm::ivec2& value) const {
            const UniformInfo* info = getUniform(name);

            if (validateUniformPtr(info, GL_INT_VEC2))
                glUniform2iv(info->location, 1, glm::value_ptr(value));
        }

        void setUVec2(std::string_view name, u32 x, u32 y) const {
            const UniformInfo* info = getUniform(name);

            if (validateUniformPtr(info, GL_UNSIGNED_INT_VEC2))
                glUniform2ui(info->location, x, y);
        }

        void setUVec2(std::string_view name, const glm::uvec2& value) const {
            const UniformInfo* info = getUniform(name);

            if (validateUniformPtr(info, GL_UNSIGNED_INT_VEC2))
                glUniform2uiv(info->location, 1, glm::value_ptr(value));
        }

        void setVec3(std::string_view name, f32 x, f32 y, f32 z) const {
            const UniformInfo* info = getUniform(name);

            if (validateUniformPtr(info, GL_FLOAT_VEC3))
                glUniform3f(info->location, x, y, z);
        }

        void setVec3(std::string_view name, glm::vec3 value) const {
            const UniformInfo* info = getUniform(name);

            if (validateUniformPtr(info, GL_FLOAT_VEC3))
                glUniform3fv(info->location, 1, glm::value_ptr(value));
        }

        void setIVec3(std::string_view name, i32 x, i32 y, i32 z) const {
            const UniformInfo* info = getUniform(name);

            if (validateUniformPtr(info, GL_INT_VEC3))
                glUniform3i(info->location, x, y, z);
        }

        void setIVec3(std::string_view name, const glm::ivec3& value) const {
            const UniformInfo* info = getUniform(name);

            if (validateUniformPtr(info, GL_INT_VEC3))
                glUniform3iv(info->location, 1, glm::value_ptr(value));
        }

        void setUVec3(std::string_view name, u32 x, u32 y, u32 z) const {
            const UniformInfo* info = getUniform(name);

            if (validateUniformPtr(info, GL_UNSIGNED_INT_VEC3))
                glUniform3ui(info->location, x, y, z);
        }

        void setUVec3(std::string_view name, const glm::uvec3& value) const {
            const UniformInfo* info = getUniform(name);

            if (validateUniformPtr(info, GL_UNSIGNED_INT_VEC3))
                glUniform3uiv(info->location, 1, glm::value_ptr(value));
        }

        void setVec4(std::string_view name, f32 x, f32 y, f32 z, f32 w) const {
            const UniformInfo* info = getUniform(name);

            if (validateUniformPtr(info, GL_FLOAT_VEC4))
                glUniform4f(info->location, x, y, z, w);
        }

        void setVec4(std::string_view name, glm::vec4 value) const {
            const UniformInfo* info = getUniform(name);

            if (validateUniformPtr(info, GL_FLOAT_VEC4))
                glUniform4fv(info->location, 1, glm::value_ptr(value));
        }

        void setIVec4(std::string_view name, i32 x, i32 y, i32 z, i32 w) const {
            const UniformInfo* info = getUniform(name);

            if (validateUniformPtr(info, GL_INT_VEC4))
                glUniform4i(info->location, x, y, z, w);
        }

        void setIVec4(std::string_view name, const glm::ivec4& value) const {
            const UniformInfo* info = getUniform(name);

            if (validateUniformPtr(info, GL_INT_VEC4))
                glUniform4iv(info->location, 1, glm::value_ptr(value));
        }

        void setUVec4(std::string_view name, u32 x, u32 y, u32 z, u32 w) const {
            const UniformInfo* info = getUniform(name);

            if (validateUniformPtr(info, GL_UNSIGNED_INT_VEC4))
                glUniform4ui(info->location, x, y, z, w);
        }

        void setUVec4(std::string_view name, const glm::uvec4& value) const {
            const UniformInfo* info = getUniform(name);

            if (validateUniformPtr(info, GL_UNSIGNED_INT_VEC4))
                glUniform4uiv(info->location, 1, glm::value_ptr(value));
        }

        void setMat2(std::string_view name, glm::mat2 value) const {
            const UniformInfo* info = getUniform(name);

            if (validateUniformPtr(info, GL_FLOAT_MAT2))
                glUniformMatrix2fv(info->location, 1, GL_FALSE, glm::value_ptr(value));
        }

        void setMat3(std::string_view name, glm::mat3 value) const {
            const UniformInfo* info = getUniform(name);

            if (validateUniformPtr(info, GL_FLOAT_MAT3))
                glUniformMatrix3fv(info->location, 1, GL_FALSE, glm::value_ptr(value));
        }

        void setMat4(std::string_view name, glm::mat4 value) const {
            const UniformInfo* info = getUniform(name);

            if (validateUniformPtr(info, GL_FLOAT_MAT4))
                glUniformMatrix4fv(info->location, 1, GL_FALSE, glm::value_ptr(value));
        }

        void setMat2x3(std::string_view name, const glm::mat2x3& value) const {
            const UniformInfo* info = getUniform(name);

            if (validateUniformPtr(info, GL_FLOAT_MAT2x3))
                glUniformMatrix2x3fv(info->location, 1, GL_FALSE, glm::value_ptr(value));
        }

        void setMat3x2(std::string_view name, const glm::mat3x2& value) const {
            const UniformInfo* info = getUniform(name);

            if (validateUniformPtr(info, GL_FLOAT_MAT3x2))
                glUniformMatrix3x2fv(info->location, 1, GL_FALSE, glm::value_ptr(value));
        }

        void setMat2x4(std::string_view name, const glm::mat2x4& value) const {
            const UniformInfo* info = getUniform(name);

            if (validateUniformPtr(info, GL_FLOAT_MAT2x4))
                glUniformMatrix2x4fv(info->location, 1, GL_FALSE, glm::value_ptr(value));
        }

        void setMat4x2(std::string_view name, const glm::mat4x2& value) const {
            const UniformInfo* info = getUniform(name);

            if (validateUniformPtr(info, GL_FLOAT_MAT4x2))
                glUniformMatrix4x2fv(info->location, 1, GL_FALSE, glm::value_ptr(value));
        }

        void setMat3x4(std::string_view name, const glm::mat3x4& value) const {
            const UniformInfo* info = getUniform(name);

            if (validateUniformPtr(info, GL_FLOAT_MAT3x4))
                glUniformMatrix3x4fv(info->location, 1, GL_FALSE, glm::value_ptr(value));
        }

        void setMat4x3(std::string_view name, const glm::mat4x3& value) const {
            const UniformInfo* info = getUniform(name);

            if (validateUniformPtr(info, GL_FLOAT_MAT4x3))
                glUniformMatrix4x3fv(info->location, 1, GL_FALSE, glm::value_ptr(value));
        }

        void setSampler1D(std::string_view name, i32 location) const {
            const UniformInfo* info = getUniform(name);

            if (validateUniformPtr(info, GL_SAMPLER_1D))
                glUniform1i(info->location, location);
        }

        void setSampler2D(std::string_view name, i32 location) const {
            const UniformInfo* info = getUniform(name);

            if (validateUniformPtr(info, GL_SAMPLER_2D))
                glUniform1i(info->location, location);
        }

        void setSampler3D(std::string_view name, i32 location) const {
            const UniformInfo* info = getUniform(name);

            if (validateUniformPtr(info, GL_SAMPLER_3D))
                glUniform1i(info->location, location);
        }

        void linkUniformBlock(std::string_view name, u32 location) const {
            glUniformBlockBinding(id, glGetUniformBlockIndex(id, std::string(name).c_str()), location);
        }

    private:
        std::string vertexSource;
        std::string fragmentSource;
        std::string geometrySource;

        std::unordered_map<std::string, UniformInfo> uniforms;
        
        const UniformInfo* getUniform(std::string_view name) const {
            auto it= uniforms.find(std::string(name));

            return it == uniforms.end() ? nullptr : &it->second;
        }

        bool validateUniformPtr(const UniformInfo* info, GLenum type) const {
            return info != nullptr && info->type == type;
        }
    };

    template<>
    class Loader<ShaderProgram> {
    public:
        static ShaderProgram load(std::string_view vertexPath, std::string_view fragmentPath, std::string_view geometryPath = "") {
            ShaderProgram shader;

            shader.id = glCreateProgram();

            reload(shader, vertexPath, fragmentPath, geometryPath);

            return shader;
        }

        static void reload(ShaderProgram& shader, std::string_view vertexPath, std::string_view fragmentPath, std::string_view geometryPath = "") {
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
            
            reflectUniforms(shader);
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

        /*
        static void reflectUniformBlocks(ShaderProgram& shader) {
            i32 uniformBlockCount;
            i32 uniformBlockMaxVarLength;

            glGetProgramiv(shader.id, GL_ACTIVE_UNIFORM_BLOCKS, &uniformBlockCount);
            glGetProgramiv(shader.id, GL_ACTIVE_UNIFORM_BLOCK_MAX_NAME_LENGTH, &uniformBlockMaxVarLength);

            std::vector<i8> nameBuffer(uniformBlockMaxVarLength);

            for (i32 i = 0; i < uniformBlockCount; i++) {
                // ...

                GLsizei length;

                glgetactiveuniform
            }
        }
        */
    };
}

#endif