#include "../../Public/Engine/Application.h"
#include "../../Public/Platform/Window.h"
#include "../../../Renderer/Public/Shader.h"
#include "../../../Renderer/Public/Renderer.h"

Application::Application()
    : m_Window(nullptr), m_Shader(nullptr), m_Renderer(nullptr), m_Running(false) {}

Application::~Application() { Shutdown(); }

bool Application::Initialize() {
    m_Window = new Window("Test Render", 800, 600);
    if (!m_Window->Initialize()) { return false; }

    m_Shader = new Shader("shaders/basic.vert", "shaders/basic.frag");
    if (!m_Shader->Compile()) { return false; }

    m_Renderer = new Renderer();
    m_Running = true;
    return true;
}

void Application::MainLoop() {
    while (m_Running && !m_Window->ShouldClose()) {
        m_Window->PollEvents();
        m_Renderer->Clear();
        m_Shader->Use();
        m_Renderer->DrawTriangle();
        m_Window->SwapBuffers();
    }
}

void Application::Shutdown() {
    delete m_Renderer; m_Renderer = nullptr;
    delete m_Shader;   m_Shader = nullptr;
    delete m_Window;   m_Window = nullptr;
}

int Application::Run() {
    if (!Initialize()) { Shutdown(); return -1; }
    MainLoop();
    Shutdown();
    return 0;
}


