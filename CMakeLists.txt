cmake_minimum_required(VERSION 3.19)
project(zarin)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(
    zarin STATIC
# User Header
        src/zarin.hpp
        src/entry.hpp

# Zarin
    # Core
        # Definitions
        src/Zarin/Core/Core.hpp
        src/Zarin/Core/Timestep.hpp
        # Headers
        src/Zarin/Core/Application.hpp
        src/Zarin/Core/Window.hpp
        src/Zarin/Core/Log.hpp
        src/Zarin/Core/Layer.hpp
        src/Zarin/Core/LayerStack.hpp
        # Sources
        src/Zarin/Core/Application.cpp
        src/Zarin/Core/Window.cpp
        src/Zarin/Core/Log.cpp
        src/Zarin/Core/Layer.cpp
        src/Zarin/Core/LayerStack.cpp

    # Input
        src/Zarin/Input/Input.hpp
        src/Zarin/Input/KeyCodes.hpp
        src/Zarin/Input/MouseCodes.hpp

    # Events
        src/Zarin/Events/Event.hpp
        src/Zarin/Events/ApplicationEvent.hpp
        src/Zarin/Events/MouseEvent.hpp
        src/Zarin/Events/KeyEvent.hpp
        src/Zarin/Events/TextEvent.hpp

    # Renderer
        # Definitions
        src/Zarin/Renderer/GraphicsContext.hpp
        src/Zarin/Renderer/Camera.hpp
        # Headers
        src/Zarin/Renderer/Shader.hpp
        src/Zarin/Renderer/Buffer.hpp
        src/Zarin/Renderer/Renderer.hpp
        src/Zarin/Renderer/VertexArray.hpp
        src/Zarin/Renderer/RendererAPI.hpp
        src/Zarin/Renderer/RenderCommand.hpp
        src/Zarin/Renderer/Texture.hpp
        src/Zarin/Renderer/Mesh.hpp
        src/Zarin/Renderer/Framebuffer.hpp
        src/Zarin/Renderer/Material.hpp
        # Sources
        src/Zarin/Renderer/Shader.cpp
        src/Zarin/Renderer/Buffer.cpp
        src/Zarin/Renderer/Renderer.cpp
        src/Zarin/Renderer/VertexArray.cpp
        src/Zarin/Renderer/RendererAPI.cpp
        src/Zarin/Renderer/RenderCommand.cpp
        src/Zarin/Renderer/Texture.cpp
        src/Zarin/Renderer/Mesh.cpp
        src/Zarin/Renderer/Framebuffer.cpp
        src/Zarin/Renderer/Material.cpp

    # Scene
        # Definitions
        src/Zarin/Scene/Components.hpp
        # Headers
        src/Zarin/Scene/Scene.hpp
        src/Zarin/Scene/SceneCamera.hpp
        src/Zarin/Scene/ViewportCamera.hpp
        src/Zarin/Scene/ViewportCameraController.hpp
        src/Zarin/Scene/Entity.hpp
        src/Zarin/Scene/ScriptableEntity.hpp
        src/Zarin/Scene/AssetManager.hpp
        # Sources
        src/Zarin/Scene/Scene.cpp
        src/Zarin/Scene/SceneCamera.cpp
        src/Zarin/Scene/ViewportCamera.cpp
        src/Zarin/Scene/ViewportCameraController.cpp
        src/Zarin/Scene/Entity.cpp
        src/Zarin/Scene/ScriptableEntity.cpp
        src/Zarin/Scene/AssetManager.cpp

    # OpenGL platform
        # Headers
        src/Zarin/Platform/OpenGL/OpenGLContext.hpp
        src/Zarin/Platform/OpenGL/OpenGLBuffer.hpp
        src/Zarin/Platform/OpenGL/OpenGLVertexArray.hpp
        src/Zarin/Platform/OpenGL/OpenGLRendererAPI.hpp
        src/Zarin/Platform/OpenGL/OpenGLShader.hpp
        src/Zarin/Platform/OpenGL/OpenGLTexture.hpp
        src/Zarin/Platform/OpenGL/OpenGLFramebuffer.hpp
        # Sources
        src/Zarin/Platform/OpenGL/OpenGLContext.cpp
        src/Zarin/Platform/OpenGL/OpenGLBuffer.cpp
        src/Zarin/Platform/OpenGL/OpenGLVertexArray.cpp
        src/Zarin/Platform/OpenGL/OpenGLRendererAPI.cpp
        src/Zarin/Platform/OpenGL/OpenGLShader.cpp
        src/Zarin/Platform/OpenGL/OpenGLTexture.cpp
        src/Zarin/Platform/OpenGL/OpenGLFramebuffer.cpp

    # Windows platform
        src/Zarin/Platform/Windows/WindowsWindow.hpp
        src/Zarin/Platform/Windows/WindowsWindow.cpp
        src/Zarin/Platform/Windows/WindowsInput.cpp

# ImGui
        src/ImGui/ImGuiLayer.hpp
        src/ImGui/ImGuiLayer.cpp
        dependencies/imgui/backends/imgui_impl_sdl2.h
        dependencies/imgui/backends/imgui_impl_opengl3.h
        dependencies/imgui/backends/imgui_impl_sdl2.cpp
        dependencies/imgui/backends/imgui_impl_opengl3.cpp

# Editor
        src/Editor/EditorLayer.hpp
        src/Editor/Widgets/SceneHierarchyPanel.hpp
        src/Editor/EditorLayer.cpp
        src/Editor/Widgets/SceneHierarchyPanel.cpp
    )

target_include_directories(zarin
    PUBLIC
        src
        src/Zarin
        src/ImGui
        src/Editor
    )

target_compile_features(zarin PUBLIC cxx_std_20)

add_subdirectory(dependencies)

find_package(OpenGL REQUIRED)

# better add these libraries into dependencies
find_package(spdlog REQUIRED)
find_package(SDL    REQUIRED)
find_package(glm    REQUIRED)
find_package(assimp REQUIRED)

target_link_libraries(
    zarin
        PRIVATE
        SDL2main
        SDL2
        SDL2_image
        ${OPENGL_LIBRARY}
        assimp::assimp

        PUBLIC
        spdlog::spdlog
        glad
        imgui
        glm
        entt
    )

target_precompile_headers(zarin
    PRIVATE
        <vector>
        <array>
        <string>
        <sstream>
        <unordered_map>
        <unordered_set>
    
        <iostream>
        <memory>
        <utility>
        <algorithm>
        <functional>
    )