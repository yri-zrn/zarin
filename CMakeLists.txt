cmake_minimum_required(VERSION 3.19)

set(CMAKE_CXX_STANDARD 20)

project(zarin)

add_library(
    zarin STATIC
        src/EntryPoint.hpp

        src/Application.hpp
        src/Log.hpp
        src/Layer.hpp
        src/LayerStack.hpp
        
        src/Application.cpp
        src/Log.cpp
        src/Layer.cpp
        src/LayerStack.cpp
        
        src/Events/Event.hpp
        src/Events/ApplicationEvent.hpp
        src/Events/MouseEvent.hpp
        src/Events/KeyEvent.hpp
        
        src/Window.hpp
        src/Platform/Windows/WindowsWindow.hpp
        
        src/Platform/Windows/WindowsWindow.cpp
    )

target_include_directories(zarin PRIVATE src)

find_package(spdlog CONFIG)
find_package(SDL REQUIRED)

target_link_libraries(
    zarin
        PRIVATE
        spdlog::spdlog
        SDL2main
        SDL2
        SDL2_image
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