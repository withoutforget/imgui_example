#include "AppView.h"
#include "imgui.h"

void AppView::Render(const AppModel& model) {
    RenderMainWindow(model);
    RenderStatsWindow(model);
}

void AppView::RenderMainWindow(const AppModel& model) {
    ImGui::Begin("Main Control Panel");

    ImGui::Text("This is a simple ImGui window using MVC pattern.");
    ImGui::Separator();

    // Слайдер - вызываем callback при изменении
    float sliderValue = model.GetSliderValue();
    if (ImGui::SliderFloat("Float Slider", &sliderValue, 0.0f, 1.0f)) {
        if (m_onSliderChanged) {
            m_onSliderChanged(sliderValue);
        }
    }

    // Цветовой пикер - вызываем callback при изменении
    float clearColor[3] = {
        model.GetClearColor()[0],
        model.GetClearColor()[1],
        model.GetClearColor()[2]
    };
    if (ImGui::ColorEdit3("Clear Color", clearColor)) {
        if (m_onColorChanged) {
            m_onColorChanged(clearColor[0], clearColor[1], clearColor[2]);
        }
    }

    ImGui::Separator();

    // Кнопки - вызываем callback при нажатии
    if (ImGui::Button("Click Me!")) {
        if (m_onIncrementCounter) {
            m_onIncrementCounter();
        }
    }
    ImGui::SameLine();
    ImGui::Text("counter = %d", model.GetCounter());

    if (ImGui::Button("Reset Counter")) {
        if (m_onResetCounter) {
            m_onResetCounter();
        }
    }

    ImGui::End();
}

void AppView::RenderStatsWindow(const AppModel& model) {
    ImGui::Begin("Statistics");

    ImGui::Text("Application Info:");
    ImGui::BulletText("FPS: %.1f", ImGui::GetIO().Framerate);
    ImGui::BulletText("Frame time: %.3f ms", 1000.0f / ImGui::GetIO().Framerate);
    ImGui::BulletText("Counter value: %d", model.GetCounter());
    ImGui::BulletText("Slider value: %.2f", model.GetSliderValue());

    ImGui::End();
}
