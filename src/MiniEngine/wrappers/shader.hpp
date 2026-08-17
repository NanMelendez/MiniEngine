#ifndef __MINIENGINE_SHADER_PROGRAM__
#define __MINIENGINE_SHADER_PROGRAM__

#include "../core/baseobject.hpp"
#include "../loaders/loaderbase.hpp"

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

    struct UniformBlockInfo {
        GLuint index;
        GLint binding;
        GLint size;
        GLint activeUniforms;
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
            currentlyUsedID = id;
        }

        void unuse() const {
            glUseProgram(0);
            currentlyUsedID = 0;
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

        const std::unordered_map<std::string, UniformBlockInfo>& getUniformBlockList() const {
            return uniformBlocks;
        }
        
        void setUniform(std::string_view name, bool value) const {
            const UniformInfo* info = getUniform(name);

            if (validateUniformPtr(info, GL_BOOL))
                glUniform1i(info->location, value);
        }

        void setUniform(std::string_view name, i32 value) const {
            const UniformInfo* info = getUniform(name);
            
            if (validateUniformPtr(info, GL_INT))
                glUniform1i(info->location, value);
        }

        void setUniform(std::string_view name, u32 value) const {
            const UniformInfo* info = getUniform(name);

            if (validateUniformPtr(info, GL_UNSIGNED_INT))
                glUniform1ui(info->location, value);
        }

        void setUniform(std::string_view name, f32 value) const {
            const UniformInfo* info = getUniform(name);

            if (validateUniformPtr(info, GL_FLOAT))
                glUniform1f(info->location, value);
        }

        void setUniform(std::string_view name, f32 x, f32 y)  const {
            const UniformInfo* info = getUniform(name);

            if (validateUniformPtr(info, GL_FLOAT_VEC2))
                glUniform2f(info->location, x, y);
        }

        void setUniform(std::string_view name, glm::vec2 value) const {
            const UniformInfo* info = getUniform(name);

            if (validateUniformPtr(info, GL_FLOAT_VEC2))
                glUniform2fv(info->location, 1, glm::value_ptr(value));
        }

        void setUniform(std::string_view name, i32 x, i32 y) const {
            const UniformInfo* info = getUniform(name);

            if (validateUniformPtr(info, GL_INT_VEC2))
                glUniform2i(info->location, x, y);
        }

        void setUniform(std::string_view name, const glm::ivec2& value) const {
            const UniformInfo* info = getUniform(name);

            if (validateUniformPtr(info, GL_INT_VEC2))
                glUniform2iv(info->location, 1, glm::value_ptr(value));
        }

        void setUniform(std::string_view name, u32 x, u32 y) const {
            const UniformInfo* info = getUniform(name);

            if (validateUniformPtr(info, GL_UNSIGNED_INT_VEC2))
                glUniform2ui(info->location, x, y);
        }

        void setUniform(std::string_view name, const glm::uvec2& value) const {
            const UniformInfo* info = getUniform(name);

            if (validateUniformPtr(info, GL_UNSIGNED_INT_VEC2))
                glUniform2uiv(info->location, 1, glm::value_ptr(value));
        }

        void setUniform(std::string_view name, f32 x, f32 y, f32 z) const {
            const UniformInfo* info = getUniform(name);

            if (validateUniformPtr(info, GL_FLOAT_VEC3))
                glUniform3f(info->location, x, y, z);
        }

        void setUniform(std::string_view name, glm::vec3 value) const {
            const UniformInfo* info = getUniform(name);

            if (validateUniformPtr(info, GL_FLOAT_VEC3))
                glUniform3fv(info->location, 1, glm::value_ptr(value));
        }

        void setUniform(std::string_view name, i32 x, i32 y, i32 z) const {
            const UniformInfo* info = getUniform(name);

            if (validateUniformPtr(info, GL_INT_VEC3))
                glUniform3i(info->location, x, y, z);
        }

        void setUniform(std::string_view name, const glm::ivec3& value) const {
            const UniformInfo* info = getUniform(name);

            if (validateUniformPtr(info, GL_INT_VEC3))
                glUniform3iv(info->location, 1, glm::value_ptr(value));
        }

        void setUniform(std::string_view name, u32 x, u32 y, u32 z) const {
            const UniformInfo* info = getUniform(name);

            if (validateUniformPtr(info, GL_UNSIGNED_INT_VEC3))
                glUniform3ui(info->location, x, y, z);
        }

        void setUniform(std::string_view name, const glm::uvec3& value) const {
            const UniformInfo* info = getUniform(name);

            if (validateUniformPtr(info, GL_UNSIGNED_INT_VEC3))
                glUniform3uiv(info->location, 1, glm::value_ptr(value));
        }

        void setUniform(std::string_view name, f32 x, f32 y, f32 z, f32 w) const {
            const UniformInfo* info = getUniform(name);

            if (validateUniformPtr(info, GL_FLOAT_VEC4))
                glUniform4f(info->location, x, y, z, w);
        }

        void setUniform(std::string_view name, glm::vec4 value) const {
            const UniformInfo* info = getUniform(name);

            if (validateUniformPtr(info, GL_FLOAT_VEC4))
                glUniform4fv(info->location, 1, glm::value_ptr(value));
        }

        void setUniform(std::string_view name, i32 x, i32 y, i32 z, i32 w) const {
            const UniformInfo* info = getUniform(name);

            if (validateUniformPtr(info, GL_INT_VEC4))
                glUniform4i(info->location, x, y, z, w);
        }

        void setUniform(std::string_view name, const glm::ivec4& value) const {
            const UniformInfo* info = getUniform(name);

            if (validateUniformPtr(info, GL_INT_VEC4))
                glUniform4iv(info->location, 1, glm::value_ptr(value));
        }

        void setUniform(std::string_view name, u32 x, u32 y, u32 z, u32 w) const {
            const UniformInfo* info = getUniform(name);

            if (validateUniformPtr(info, GL_UNSIGNED_INT_VEC4))
                glUniform4ui(info->location, x, y, z, w);
        }

        void setUniform(std::string_view name, const glm::uvec4& value) const {
            const UniformInfo* info = getUniform(name);

            if (validateUniformPtr(info, GL_UNSIGNED_INT_VEC4))
                glUniform4uiv(info->location, 1, glm::value_ptr(value));
        }

        void setUniform(std::string_view name, glm::mat2 value) const {
            const UniformInfo* info = getUniform(name);

            if (validateUniformPtr(info, GL_FLOAT_MAT2))
                glUniformMatrix2fv(info->location, 1, GL_FALSE, glm::value_ptr(value));
        }

        void setUniform(std::string_view name, glm::mat3 value) const {
            const UniformInfo* info = getUniform(name);

            if (validateUniformPtr(info, GL_FLOAT_MAT3))
                glUniformMatrix3fv(info->location, 1, GL_FALSE, glm::value_ptr(value));
        }

        void setUniform(std::string_view name, glm::mat4 value) const {
            const UniformInfo* info = getUniform(name);

            if (validateUniformPtr(info, GL_FLOAT_MAT4))
                glUniformMatrix4fv(info->location, 1, GL_FALSE, glm::value_ptr(value));
        }

        void setUniform(std::string_view name, const glm::mat2x3& value) const {
            const UniformInfo* info = getUniform(name);

            if (validateUniformPtr(info, GL_FLOAT_MAT2x3))
                glUniformMatrix2x3fv(info->location, 1, GL_FALSE, glm::value_ptr(value));
        }

        void setUniform(std::string_view name, const glm::mat3x2& value) const {
            const UniformInfo* info = getUniform(name);

            if (validateUniformPtr(info, GL_FLOAT_MAT3x2))
                glUniformMatrix3x2fv(info->location, 1, GL_FALSE, glm::value_ptr(value));
        }

        void setUniform(std::string_view name, const glm::mat2x4& value) const {
            const UniformInfo* info = getUniform(name);

            if (validateUniformPtr(info, GL_FLOAT_MAT2x4))
                glUniformMatrix2x4fv(info->location, 1, GL_FALSE, glm::value_ptr(value));
        }

        void setUniform(std::string_view name, const glm::mat4x2& value) const {
            const UniformInfo* info = getUniform(name);

            if (validateUniformPtr(info, GL_FLOAT_MAT4x2))
                glUniformMatrix4x2fv(info->location, 1, GL_FALSE, glm::value_ptr(value));
        }

        void setUniform(std::string_view name, const glm::mat3x4& value) const {
            const UniformInfo* info = getUniform(name);

            if (validateUniformPtr(info, GL_FLOAT_MAT3x4))
                glUniformMatrix3x4fv(info->location, 1, GL_FALSE, glm::value_ptr(value));
        }

        void setUniform(std::string_view name, const glm::mat4x3& value) const {
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

        bool isCurrentlyInUse() const {
            return currentlyUsedID == id && isAllocated();
        }

    private:
        std::string vertexSource;
        std::string fragmentSource;
        std::string geometrySource;

        std::unordered_map<std::string, UniformInfo> uniforms;
        std::unordered_map<std::string, UniformBlockInfo> uniformBlocks;

        inline static GLuint currentlyUsedID = 0;
        
        const UniformInfo* getUniform(std::string_view name) const {
            auto it= uniforms.find(std::string(name));

            return it == uniforms.end() ? nullptr : &it->second;
        }

        bool validateUniformPtr(const UniformInfo* info, GLenum type) const {
            return info != nullptr && info->type == type;
        }
    };
}

#endif