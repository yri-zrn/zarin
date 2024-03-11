#pragma once

#include "Zarin/Renderer/Mesh.hpp"

#include <string>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

namespace zrn
{

class AssetManager {
public:
    static Ref<Mesh> LoadModel(const std::string& filepath);

private:
    static Assimp::Importer* m_Importer;
};

} // namespace zrn
