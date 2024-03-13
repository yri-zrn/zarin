#include "AssetManager.hpp"

#include <vector>
#include <iterator>

namespace zrn {

Assimp::Importer* AssetManager::m_Importer = new Assimp::Importer();

Ref<Mesh> AssetManager::LoadModel(const std::string& filepath) {
    std::vector<float> vertex_data;
    std::vector<uint32_t> indices;
    BufferLayout layout;

    auto import_flags = aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_GenSmoothNormals | aiProcess_JoinIdenticalVertices;
    const aiScene* scene = m_Importer->ReadFile(filepath, import_flags);

    if (scene == nullptr) {
        ZRN_CORE_ERROR("Failed to load object: '{0}'", m_Importer->GetErrorString());
        return nullptr;
    }

    std::vector<aiMesh*> meshes{ scene->mMeshes, scene->mMeshes + scene->mNumMeshes };

    uint32_t mesh_id = 0;
    for(auto mesh : meshes) {
        std::vector<aiVector3D> vertices{ mesh->mVertices, mesh->mVertices + mesh->mNumVertices };

        uint32_t vertex_id = 0;
        for(const auto& vertex : vertices) {
            const aiVector3D* tex_coords = mesh->mTextureCoords[mesh_id];

            vertex_data.insert(vertex_data.end(), {
                vertex.x, vertex.y, vertex.z,
                tex_coords[vertex_id].x, 1.0f - tex_coords[vertex_id].y
            });

            ++vertex_id;
        }

        std::vector<aiFace> faces{ mesh->mFaces, mesh->mFaces + mesh->mNumFaces };
        for(const auto& face : faces) {
            indices.insert(indices.end(), face.mIndices, face.mIndices + face.mNumIndices);
        }

        ++mesh_id;
    }

    layout = {
        { zrn::ShaderDataType::Float3, "a_Position" },
        { zrn::ShaderDataType::Float2, "a_TexCoord" }
    };

    auto mesh = CreateRef<Mesh>(vertex_data, indices, layout);
    return mesh;
}

} // namespace zrn
