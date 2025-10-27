#include "Application.h"
#include "DX11Backend.h"
#include "imgui.h"

Application::Application()
    : m_backend(nullptr)
    , m_model(nullptr)
    , m_view(nullptr)
    , m_controller(nullptr)
{
}

Application::~Application() {
    Shutdown();
}

bool Application::Initialize(int width, int height, const char* title) {
    // Создаем бекенд (можно легко заменить на другой)
    m_backend = std::make_unique<DX11Backend>();
    
    if (!m_backend->Initialize(width, height, title)) {
        return false;
    }

    // Инициализируем ImGui контекст
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

    // Устанавливаем стиль
    ImGui::StyleColorsDark();

    // Создаем компоненты MVC
    m_model = std::make_unique<AppModel>();
    m_view = std::make_unique<AppView>();
    m_controller = std::make_unique<AppController>(*m_model, *m_view);

    return true;
}

void Application::Run() {
    // Главный цикл приложения
    while (!m_backend->ShouldClose()) {
        // Обработка событий
        m_backend->PollEvents();

        // Обновление логики
        m_controller->Update(ImGui::GetIO().DeltaTime);

        // Начало нового кадра
        m_backend->NewFrame();

        // Рендеринг UI через контроллер
        m_controller->Render();

        // Финальный рендеринг
        m_backend->Render();
        m_backend->Present();
    }
}

void Application::Shutdown() {
    m_controller.reset();
    m_view.reset();
    m_model.reset();

    if (m_backend) {
        m_backend->Shutdown();
        m_backend.reset();
    }

    ImGui::DestroyContext();
}
