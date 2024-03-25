#include "ImGuiLayer.hpp"

#include "Core/Application.hpp"

namespace zrn {

SDL_GLContext g_ImGui_GL_context;

ImGuiLayer::ImGuiLayer()
: Layer("ImGuiLayer")
{ }

ImGuiLayer::~ImGuiLayer() {

}

void ImGuiLayer::OnAttach() {

    Application* app = &Application::Get();
    SDL_Window* window = (SDL_Window*)app->GetWindow().GetWindowHandle();

    // Decide GL+GLSL versions
#if defined(IMGUI_IMPL_OPENGL_ES2)
    // GL ES 2.0 + GLSL 100
    const char* glsl_version = "#version 100";
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, 0);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_ES);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
#elif defined(__APPLE__)
    // GL 2.2 Core + GLSL 150
    const char* glsl_version = "#version 150";
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG); // Always required on Mac
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
#else
    // GL 2.0 + GLSL 130
    const char* glsl_version = "#version 130";
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, 0);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
#endif

    // From 2.0.18: Enable native IME.
#ifdef SDL_HINT_IME_SHOW_UI
    SDL_SetHint(SDL_HINT_IME_SHOW_UI, "1");
#endif

    // Create window with graphics context
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
    SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
    // SDL_WindowFlags window_flags = (SDL_WindowFlags)(SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI);
    // SDL_Window* window = SDL_CreateWindow("Dear ImGui SDL2+OpenGL3 example", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1280, 720, window_flags);
    // if (window == nullptr)
    // {
    //     printf("Error: SDL_CreateWindow(): %s\n", SDL_GetError());
    //     return;
    // }

    g_ImGui_GL_context = SDL_GL_CreateContext(window);
    SDL_GL_MakeCurrent(window, g_ImGui_GL_context);
    SDL_GL_SetSwapInterval(1); // Enable vsync

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;         // Enable Docking
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;       // Enable Multi-Viewport / Platform Windows
    //io.ConfigViewportsNoAutoMerge = true;
    //io.ConfigViewportsNoTaskBarIcon = true;

    // Setup Dear ImGui style
    // ImGui::StyleColorsClassic();
    ImGui::StyleColorsLight();
    // ImGui::Spectrum::StyleColorsSpectrum();
    ImGui::Spectrum::LoadFont();

    {
        // ImGuizmo::
        auto& style = ImGuizmo::GetStyle();
        style.TranslationLineThickness   = 2.0f;
        style.TranslationLineArrowSize   = 5.0f;
        style.RotationLineThickness      = 2.0f;
        style.RotationOuterLineThickness = 2.0f;
        style.ScaleLineThickness         = 2.0f;
        style.ScaleLineCircleSize        = 5.0f;
        style.HatchedAxisLineThickness   = 5.0f;
        style.CenterCircleSize           = 5.0f;

        // initialize default colors
        style.Colors[ImGuizmo::DIRECTION_X]           = ImVec4(0.949f, 0.318f, 0.384f, 1.000f);
        style.Colors[ImGuizmo::DIRECTION_Y]           = ImVec4(0.549f, 0.824f, 0.263f, 1.000f);
        style.Colors[ImGuizmo::DIRECTION_Z]           = ImVec4(0.310f, 0.569f, 0.945f, 1.000f);
        style.Colors[ImGuizmo::PLANE_X]               = ImVec4(0.949f, 0.318f, 0.384f, 0.500f);
        style.Colors[ImGuizmo::PLANE_Y]               = ImVec4(0.549f, 0.824f, 0.263f, 0.500f);
        style.Colors[ImGuizmo::PLANE_Z]               = ImVec4(0.310f, 0.569f, 0.945f, 0.500f);
        style.Colors[ImGuizmo::SELECTION]             = ImVec4(0.900f, 0.900f, 0.900f, 0.900f);
        style.Colors[ImGuizmo::INACTIVE]              = ImVec4(0.600f, 0.600f, 0.600f, 0.600f);
        style.Colors[ImGuizmo::TRANSLATION_LINE]      = ImVec4(0.666f, 0.666f, 0.666f, 0.666f);
        style.Colors[ImGuizmo::SCALE_LINE]            = ImVec4(0.250f, 0.250f, 0.250f, 1.000f);
        style.Colors[ImGuizmo::ROTATION_USING_BORDER] = ImVec4(0.900f, 0.900f, 0.900f, 1.000f);
        style.Colors[ImGuizmo::ROTATION_USING_FILL]   = ImVec4(0.900f, 0.900f, 0.900f, 0.500f);
        style.Colors[ImGuizmo::HATCHED_AXIS_LINES]    = ImVec4(0.000f, 0.000f, 0.000f, 0.000f);
        style.Colors[ImGuizmo::TEXT]                  = ImVec4(1.000f, 1.000f, 1.000f, 1.000f);
        style.Colors[ImGuizmo::TEXT_SHADOW]           = ImVec4(0.000f, 0.000f, 0.000f, 1.000f);
    }

    // When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
    ImGuiStyle& style = ImGui::GetStyle();
    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        style.WindowRounding = 0.0f;
        style.Colors[ImGuiCol_WindowBg].w = 1.0f;
    }

    // Setup Platform/Renderer backends
    ImGui_ImplSDL2_InitForOpenGL(window, g_ImGui_GL_context);
    ImGui_ImplOpenGL3_Init(glsl_version);

    // Load Fonts
    // - If no fonts are loaded, dear imgui will use the default font. You can also load multiple fonts and use ImGui::PushFont()/PopFont() to select them.
    // - AddFontFromFileTTF() will return the ImFont* so you can store it if you need to select the font among multiple.
    // - If the file cannot be loaded, the function will return a nullptr. Please handle those errors in your application (e.g. use an assertion, or display an error and quit).
    // - The fonts will be rasterized at a given size (w/ oversampling) and stored into a texture when calling ImFontAtlas::Build()/GetTexDataAsXXXX(), which ImGui_ImplXXXX_NewFrame below will call.
    // - Use '#define IMGUI_ENABLE_FREETYPE' in your imconfig file to use Freetype for higher quality font rendering.
    // - Read 'docs/FONTS.md' for more instructions and details.
    // - Remember that in C/C++ if you want to include a backslash \ in a string literal you need to write a double backslash \\ !
    // - Our Emscripten build process allows embedding fonts to be accessible at runtime from the "fonts/" folder. See Makefile.emscripten for details.
    //io.Fonts->AddFontDefault();
    //io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\segoeui.ttf", 18.0f);
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/DroidSans.ttf", 16.0f);
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/Roboto-Medium.ttf", 16.0f);
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/Cousine-Regular.ttf", 15.0f);
    //ImFont* font = io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\ArialUni.ttf", 18.0f, nullptr, io.Fonts->GetGlyphRangesJapanese());
    //IM_ASSERT(font != nullptr);
}

void ImGuiLayer::OnDetach() {
    Application* app = &Application::Get();
    SDL_Window* window = (SDL_Window*)app->GetWindow().GetWindowHandle();

    // Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();

    SDL_GL_DeleteContext(g_ImGui_GL_context);
}

void ImGuiLayer::OnImGuiRender() {
    
}

void ImGuiLayer::OnEvent(Event& event) {
    if (m_BlockEvents) {
        ImGuiIO& io = ImGui::GetIO();
        event.Handled |= event.IsInCategory(EventCategoryMouse) & io.WantCaptureMouse;
        event.Handled |= event.IsInCategory(EventCategoryKeyboard) & io.WantCaptureKeyboard;
    }
}

void ImGuiLayer::Begin() {
    Application* app = &Application::Get();
    SDL_Window* window = (SDL_Window*)app->GetWindow().GetWindowHandle();

    const uint32_t event_buffer_size = 16;
    SDL_Event events[event_buffer_size];
    SDL_PumpEvents();
    // TODO: fix this
    // event_count equals 5 when there is 7 events in the queue
    uint32_t event_count = SDL_PeepEvents(events, event_buffer_size, SDL_PEEKEVENT, SDL_FIRSTEVENT, SDL_LASTEVENT);
    for (uint32_t i = 0;
        i < (event_count > event_buffer_size ? event_buffer_size : event_count);
        ++i) {
        ImGui_ImplSDL2_ProcessEvent(&events[i]);
    }

    // Start the Dear ImGui frame
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplSDL2_NewFrame();
    ImGui::NewFrame();
    ImGuizmo::BeginFrame();
}

void ImGuiLayer::End() {
    Application* app = &Application::Get();
    SDL_Window* window = (SDL_Window*)app->GetWindow().GetWindowHandle();
    ImGuiIO& io = ImGui::GetIO();
    
    // Rendering
    ImGui::Render();
    glViewport(0, 0, (int)io.DisplaySize.x, (int)io.DisplaySize.y);
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    // Update and Render additional Platform Windows
    // (Platform functions may change the current OpenGL context, so we save/restore it to make it easier to paste this code elsewhere.
    //  For this specific demo app we could also call SDL_GL_MakeCurrent(window, gl_context) directly)
    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        SDL_Window* backup_current_window = SDL_GL_GetCurrentWindow();
        SDL_GLContext backup_current_context = SDL_GL_GetCurrentContext();
        ImGui::UpdatePlatformWindows();
        ImGui::RenderPlatformWindowsDefault();
        SDL_GL_MakeCurrent(backup_current_window, backup_current_context);
    }
}

} // namespace zrn
