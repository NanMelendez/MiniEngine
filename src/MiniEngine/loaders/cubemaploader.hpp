#ifndef __MINIENGINE_CUBEMAP_LOADER__
#define __MINIENGINE_CUBEMAP_LOADER__

#include "../wrappers/cubemap.hpp"

namespace MiniEngine {
    template<>
    class Loader<Cubemap> {
    public:
        static Cubemap load(const std::unordered_map<CubemapFace, Texture2D*>& faces = {}, const std::vector<std::pair<GLenum, GLint>>& params = {{GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE}, {GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE}, {GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE}, {GL_TEXTURE_MIN_FILTER, GL_LINEAR}, {GL_TEXTURE_MAG_FILTER, GL_LINEAR}}) {
            Cubemap cubemap;

            glGenTextures(1, &cubemap.id);

            reload(cubemap, faces, params);

            return cubemap;
        }

        static void reload(Cubemap& cubemap, const std::unordered_map<CubemapFace, Texture2D*>& faces = {}, const std::vector<std::pair<GLenum, GLint>>& params = {{GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE}, {GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE}, {GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE}, {GL_TEXTURE_MIN_FILTER, GL_LINEAR}, {GL_TEXTURE_MAG_FILTER, GL_LINEAR}}) {
            cubemap.bind();
            for (const auto& [side, tex] : faces)
                cubemap.setFace({side, tex});

            for (const std::pair<GLenum, GLint>& param : params)
                cubemap.setTextureParameter(param.first, param.second);
            cubemap.unbind();
        }
    };
}

#endif