#include "AppView.h"
#include "imgui.h"

AppView::AppView() {
}

void AppView::Render(AppModel& model) {
    RenderMainWindow(model);
    RenderStatsWindow(model);
}

void AppView::RenderMainWindow(AppModel& model) {
    ImGui::Begin("Main Control Panel");
    
    ImGui::Text("This is a simple ImGui window using MVC pattern.");
    ImGui::Separator();

    // Слайдер
    float sliderValue = model.GetSliderValue();
    if (ImGui::SliderFloat("Float Slider", &sliderValue, 0.0f, 1.0f)) {
        model.SetSliderValue(sliderValue);
    }

    // Цветовой пикер
    float clearColor[3] = {
        model.GetClearColor()[0],
        model.GetClearColor()[1],
        model.GetClearColor()[2]
    };
    if (ImGui::ColorEdit3("Clear Color", clearColor)) {
        model.SetClearColor(clearColor[0], clearColor[1], clearColor[2]);
    }

    ImGui::Separator();

    // Кнопка и счетчик
    if (ImGui::Button("Click Me!")) {
        // Это событие будет обработано в контроллере
    }
    ImGui::SameLine();
    ImGui::Text("counter = %d", model.GetCounter());

    if (ImGui::Button("Reset Counter")) {
        // Это событие будет обработано в контроллере
    }

    ImGui::End();
}

void AppView::RenderStatsWindow(AppModel& model) {
    ImGui::Begin("Statistics");
    
    ImGui::Text("Application Info:");
    ImGui::BulletText("FPS: %.1f", ImGui::GetIO().Framerate);
    ImGui::BulletText("Frame time: %.3f ms", 1000.0f / ImGui::GetIO().Framerate);
    ImGui::BulletText("Counter value: %d", model.GetCounter());
    ImGui::BulletText("Slider value: %.2f", model.GetSliderValue());
    
    ImGui::End();
}
