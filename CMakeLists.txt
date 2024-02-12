cmake_minimum_required(VERSION 3.19)
project(zarin)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(
    zarin STATIC
        src/Zarin/EntryPoint.hpp

        src/Zarin/Application.hpp
        src/Zarin/Log.hpp
        src/Zarin/Layer.hpp
        src/Zarin/LayerStack.hpp
        
        src/Zarin/Application.cpp
        src/Zarin/Log.cpp
        src/Zarin/Layer.cpp
        src/Zarin/LayerStack.cpp
        
        src/Zarin/Events/Event.hpp
        src/Zarin/Events/ApplicationEvent.hpp
        src/Zarin/Events/MouseEvent.hpp
        src/Zarin/Events/KeyEvent.hpp
        
        src/Zarin/Window.hpp

        src/Zarin/Platform/Windows/WindowsWindow.hpp
        src/Zarin/Platform/Windows/WindowsWindow.cpp

        # src/Zarin/Platform/Vulkan/ImGuiVulkanRenderer.hpp
        # src/Zarin/Platform/Vulkan/ImGuiVulkanRenderer.cpp
        dependencies/imgui/backends/imgui_impl_sdl2.cpp
        dependencies/imgui/backends/imgui_impl_sdl2.h
        dependencies/imgui/backends/imgui_impl_vulkan.cpp
        dependencies/imgui/backends/imgui_impl_vulkan.h

        src/ImGui/ImGuiLayer.hpp
        src/ImGui/ImGuiLayer.cpp
    )

target_include_directories(zarin
    PUBLIC
        src
    )

add_subdirectory(dependencies)

find_package(spdlog CONFIG)
find_package(SDL REQUIRED)
find_package(Vulkan REQUIRED)

# find_package(imgui REQUIRED)

target_link_libraries(
    zarin
        PRIVATE
        SDL2main
        SDL2
        SDL2_image
        Vulkan::Vulkan
        PUBLIC
        spdlog::spdlog
        imgui
    )

target_precompile_headers(zarin
    PRIVATE
        <iostream>
        <memory>
        <utility>
        <algorithm>
        <functional>

        <string>
        <sstream>
        <unordered_map>
        <unordered_set>
  )