#pragma once

#include "Zarin/Core/Log.hpp"

extern zrn::Application* zrn::CreateApplication();

int main(int argc, char** argv) {
    zrn::Log::Init();
    auto app = zrn::CreateApplication();
    app->Run();
    delete app;
    return 0;
}