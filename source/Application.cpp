#include "Application.h"
#include "DX11Backend.h"
#include "imgui.h"

Application::Application() = default;

Application::~Application() {
    Shutdown();
}

bool Application::Initialize(int width, int height, const char* title) {
    // Создаем и инициализируем бекенд
    m_backend = std::make_unique<DX11Backend>();
    if (!m_backend->Initialize(width, height, title)) {
        return false;
    }

    // Инициализируем ImGui
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    ImGui::StyleColorsDark();

    // Создаем компоненты MVC
    m_model = std::make_unique<AppModel>();
    m_view = std::make_unique<AppView>();
    m_controller = std::make_unique<AppController>(*m_model, *m_view);

    return true;
}

void Application::Run() {
    while (!m_backend->ShouldClose()) {
        m_backend->PollEvents();
        m_controller->Update(ImGui::GetIO().DeltaTime);

        m_backend->NewFrame();
        m_controller->Render();
        m_backend->Render();
        m_backend->Present();
    }
}

void Application::Shutdown() {
    // Удаляем компоненты MVC в обратном порядке
    m_controller.reset();
    m_view.reset();
    m_model.reset();

    // Завершаем работу бекенда
    if (m_backend) {
        m_backend->Shutdown();
        m_backend.reset();
    }

    ImGui::DestroyContext();
}
