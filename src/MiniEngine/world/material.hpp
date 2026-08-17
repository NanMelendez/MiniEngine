#ifndef __MINIENGINE_MATERIAL__
#define __MINIENGINE_MATERIAL__

#include "../wrappers/shader.hpp"
#include "../wrappers/texture2d.hpp"

namespace MiniEngine {
    using MaterialVarType = std::variant<
        bool,
        i32, u32,
        f32,
        glm::vec2, glm::ivec2, glm::uvec2,
        glm::vec3, glm::ivec3, glm::uvec3,
        glm::vec4, glm::ivec4, glm::uvec4,
        glm::mat2, glm::mat3, glm::mat4,
        glm::mat2x3, glm::mat2x4,
        glm::mat3x2, glm::mat3x4,
        glm::mat4x2, glm::mat4x3,
        Texture2D*
    >;

    class Material {
    public:
        Material(ShaderProgram* shader = nullptr) {
            setShader(shader);
        }

        void setShader(ShaderProgram* shader) {
            variables.clear();

            this->shader = shader;

            if (!shader) return;

            for (const auto& [name, info] : shader->getUniformList()) {
                u64 pos = name.find("material.");
                
                if (name.starts_with("material."))
                    variables.emplace(name.substr(9), initDefault(info));
            }
        }

        const ShaderProgram* getShader() const {
            return shader;
        }

        template<typename T>
        const T* get(std::string_view name) const {
            auto it = variables.find(std::string(name));

            if (it == variables.end()) return nullptr;

            if constexpr (std::is_pointer_v<T>)
                return std::get_if<T>(it->second);
            else
                return std::get_if<T>(&it->second);
        }

        template<typename T>
        void set(std::string_view name, const T& value) {
            auto it = variables.find(std::string(name));

            if (it == variables.end()) {
                std::cerr << "Could not find variable \"" << name << "\"\n";
                return;
            }

            if (!std::holds_alternative<T>(it->second)) {
                std::cerr << "Type mismatch for variable \"" << name << "\"\n";
                return;
            }

            it->second = value;
        }

        void bind() const {
            i32 texture2DCount = 0;

            std::string matBase = "material.";

            shader->use();

            for (const auto& [name, variable] : variables) {
                std::visit([&](const auto& value) {
                    using T = std::decay_t<decltype(value)>;

                    if constexpr (std::is_same_v<T, Texture2D*>) {
                        if (!value) return;

                        value->bind(texture2DCount);
                        shader->setSampler2D(matBase + name, texture2DCount);

                        texture2DCount++;
                    }
                    else
                        shader->setUniform(matBase + name, value);
                }, variable);
            }
        }

        void unbind() const {
            i32 texture2DCount = 0;

            for (const auto& [name, variable] : variables) {
                if (std::holds_alternative<Texture2D*>(variable)) {
                    Texture2D* temp = std::get<Texture2D*>(variable);
                    if (temp)
                        temp->unbind(texture2DCount);
                    texture2DCount++;
                }
            }

            shader->unuse();
        }
        
    private:
        ShaderProgram* shader;
        std::unordered_map<std::string, MaterialVarType> variables;

        MaterialVarType initDefault(UniformInfo info) const {
            switch(info.type) {
            default:
            case GL_BOOL: return false;
            case GL_INT: return 0;
            case GL_UNSIGNED_INT: return 0u;
            case GL_FLOAT: return 0.0f;
            case GL_FLOAT_VEC2: return glm::vec2(0.0f);
            case GL_INT_VEC2: return glm::ivec2(0);
            case GL_UNSIGNED_INT_VEC2: return glm::uvec2(0u);
            case GL_FLOAT_VEC3: return glm::vec3(0.0f);
            case GL_INT_VEC3: return glm::ivec3(0);
            case GL_UNSIGNED_INT_VEC3: return glm::uvec3(0u);
            case GL_FLOAT_VEC4: return glm::vec4(0.0f);
            case GL_INT_VEC4: return glm::ivec4(0);
            case GL_UNSIGNED_INT_VEC4: return glm::uvec4(0u);
            case GL_FLOAT_MAT2: return glm::mat2(1.0f);
            case GL_FLOAT_MAT3: return glm::mat3(1.0f);
            case GL_FLOAT_MAT4: return glm::mat4(1.0f);
            case GL_FLOAT_MAT2x3: return glm::mat2x3(1.0f);
            case GL_FLOAT_MAT2x4: return glm::mat2x4(1.0f);
            case GL_FLOAT_MAT3x2: return glm::mat3x2(1.0f);
            case GL_FLOAT_MAT3x4: return glm::mat3x4(1.0f);
            case GL_FLOAT_MAT4x2: return glm::mat4x2(1.0f);
            case GL_FLOAT_MAT4x3: return glm::mat4x3(1.0f);
            case GL_SAMPLER_2D: return static_cast<Texture2D*>(nullptr);
            }
        }
    };
}

#endif