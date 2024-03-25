#pragma once

#include "Zarin/Renderer/VertexArray.hpp"
#include "Zarin/Renderer/Buffer.hpp"

#include "glm/glm.hpp"

#include <vector>

namespace zrn {

struct Vertex {
    glm::vec3 Position;
    glm::vec3 Normal;
    glm::vec2 TexCoord;

    // Editor-only
    int EntityID;
};

class Mesh {
public:
    Mesh(std::vector<Vertex>& vertex_data, std::vector<uint32_t>& indices, const BufferLayout& layout);

    void Bind() const;
    void Unbind() const;

    Ref<VertexArray> GetVertexArray() const { return m_VertexArray; }

private:
    std::vector<Vertex> m_VertexData;
    std::vector<uint32_t> m_Indices;
    
    Ref<VertexArray> m_VertexArray;
};

} // namespace zrn
