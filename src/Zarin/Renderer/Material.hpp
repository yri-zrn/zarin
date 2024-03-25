#pragma once

#include "Texture.hpp"
#include "Shader.hpp"

namespace zrn {

class Material {
public:
    Material() = default;

    void SetShader(Ref<Shader> shader) { m_Shader = shader; }
    Ref<Shader> GetShader() const { return m_Shader; }

    void SetAlbedo(Ref<Texture2D> albedo) { m_AlbedoTexture = albedo; }
    Ref<Texture2D> GetAlbedo() const { return m_AlbedoTexture; }

private:
    Ref<Shader> m_Shader;
    Ref<Texture2D> m_AlbedoTexture;

    // std::vector<Texture2D> m_Maps;
    // MaterialLayout m_Layout;
};

} // namespace zrn
