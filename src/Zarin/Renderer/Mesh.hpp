#pragma once

#include "Zarin/Renderer/VertexArray.hpp"
#include "Zarin/Renderer/Buffer.hpp"

#include "glm/glm.hpp"

#include <vector>

namespace zrn {

class Mesh {
public:
    Mesh(std::vector<float>& vertex_data, std::vector<uint32_t>& indices, const BufferLayout& layout);

    glm::mat4 Transform{ 1.0f };
    glm::vec3 Position{ 0.0f };

    void Bind() const;
    void Unbind() const;

    Ref<VertexArray> GetVertexArray() const { return m_VertexArray; }

private:
    std::vector<float> m_VertexData;
    std::vector<uint32_t> m_Indices;
    
    Ref<VertexArray> m_VertexArray;
};

} // namespace zrn
