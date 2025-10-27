#pragma once

struct ImGuiContext;

// Интерфейс для бекенда рендеринга
class IBackend {
public:
    virtual ~IBackend() = default;

    // Инициализация бекенда
    virtual bool Initialize(int width, int height, const char* title) = 0;
    
    // Очистка ресурсов
    virtual void Shutdown() = 0;
    
    // Начало нового кадра
    virtual void NewFrame() = 0;
    
    // Рендеринг ImGui
    virtual void Render() = 0;
    
    // Презентация кадра на экран
    virtual void Present() = 0;
    
    // Проверка, нужно ли закрыть окно
    virtual bool ShouldClose() = 0;
    
    // Обработка событий
    virtual void PollEvents() = 0;
};
