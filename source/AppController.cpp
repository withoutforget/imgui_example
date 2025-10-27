#include "AppController.h"

AppController::AppController(AppModel& model, AppView& view)
    : m_model(model)
    , m_view(view)
{
    SetupEventHandlers();
}

void AppController::SetupEventHandlers() {
    // Регистрируем обработчики событий от View
    m_view.SetOnIncrementCounter([this]() {
        m_model.IncrementCounter();
    });

    m_view.SetOnResetCounter([this]() {
        m_model.ResetCounter();
    });

    m_view.SetOnSliderChanged([this](float value) {
        m_model.SetSliderValue(value);
    });

    m_view.SetOnColorChanged([this](float r, float g, float b) {
        m_model.SetClearColor(r, g, b);
    });
}

void AppController::Update(float deltaTime) {
    m_model.Update(deltaTime);
}

void AppController::Render() {
    m_view.Render(m_model);
}
