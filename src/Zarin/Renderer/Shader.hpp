#pragma once

#include "Zarin/Core.hpp"

#include <string>

#include <glm/glm.hpp>

namespace zrn {

class Shader {
public:
    virtual ~Shader() = default;

    virtual void Bind() const = 0;
    virtual void Unbind() const = 0;

    static Ref<Shader> Create(const std::string& filepath);
    static Ref<Shader> Create(const std::string& vertex_source, const std::string& fragment_source);
};

} // namespace zrn