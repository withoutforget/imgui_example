#include "AppController.h"
#include "imgui.h"

AppController::AppController(AppModel& model, AppView& view)
    : m_model(model)
    , m_view(view)
{
}

void AppController::HandleInput() {
    // Здесь обрабатываются события, которые изменяют модель
    // В ImGui это происходит прямо во время рендеринга,
    // но мы можем проверять состояние после рендера
    
    // Пример: проверка нажатия кнопок через ImGui API
    // (в реальности кнопки обрабатываются в View, но логика остается здесь)
}

void AppController::Update(float deltaTime) {
    // Обновляем модель
    m_model.Update(deltaTime);
    
    // Здесь можно добавить дополнительную логику обновления
}

void AppController::Render() {
    // Начинаем рендер окна с кнопкой
    ImGui::Begin("Main Control Panel");
    
    ImGui::Text("This is a simple ImGui window using MVC pattern.");
    ImGui::Separator();

    // Слайдер
    float sliderValue = m_model.GetSliderValue();
    if (ImGui::SliderFloat("Float Slider", &sliderValue, 0.0f, 1.0f)) {
        m_model.SetSliderValue(sliderValue);
    }

    // Цветовой пикер
    float clearColor[3] = {
        m_model.GetClearColor()[0],
        m_model.GetClearColor()[1],
        m_model.GetClearColor()[2]
    };
    if (ImGui::ColorEdit3("Clear Color", clearColor)) {
        m_model.SetClearColor(clearColor[0], clearColor[1], clearColor[2]);
    }

    ImGui::Separator();

    // Обработка кнопки - здесь происходит связь View -> Controller -> Model
    if (ImGui::Button("Click Me!")) {
        m_model.IncrementCounter();
    }
    ImGui::SameLine();
    ImGui::Text("counter = %d", m_model.GetCounter());

    if (ImGui::Button("Reset Counter")) {
        m_model.ResetCounter();
    }

    ImGui::End();

    // Окно статистики
    ImGui::Begin("Statistics");
    
    ImGui::Text("Application Info:");
    ImGui::BulletText("FPS: %.1f", ImGui::GetIO().Framerate);
    ImGui::BulletText("Frame time: %.3f ms", 1000.0f / ImGui::GetIO().Framerate);
    ImGui::BulletText("Counter value: %d", m_model.GetCounter());
    ImGui::BulletText("Slider value: %.2f", m_model.GetSliderValue());
    
    ImGui::End();
}
