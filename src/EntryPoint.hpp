#include "Log.hpp"

extern zrn::Application* zrn::CreateApplication();

#define SDL_MAIN_HANDLED

int main(int argc, char** argv) {
    zrn::Log::Init();

    ZRN_CORE_WARN("Initialized Core Logger.");
    
    auto a = 10;
    ZRN_INFO("Initialized Client Logger. Var = {0}", a);

    auto app = zrn::CreateApplication();
    app->Run();
    delete app;
}