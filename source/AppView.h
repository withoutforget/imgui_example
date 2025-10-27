#pragma once
#include "AppModel.h"
#include <functional>

// View - отвечает только за отображение UI, не модифицирует данные напрямую
class AppView {
public:
    using ButtonCallback = std::function<void()>;
    using ValueCallback = std::function<void(float)>;
    using ColorCallback = std::function<void(float, float, float)>;

    AppView() = default;
    ~AppView() = default;

    // Отрисовка UI - принимает модель только для чтения
    void Render(const AppModel& model);

    // Регистрация обработчиков событий
    void SetOnIncrementCounter(ButtonCallback callback) { m_onIncrementCounter = callback; }
    void SetOnResetCounter(ButtonCallback callback) { m_onResetCounter = callback; }
    void SetOnSliderChanged(ValueCallback callback) { m_onSliderChanged = callback; }
    void SetOnColorChanged(ColorCallback callback) { m_onColorChanged = callback; }

private:
    void RenderMainWindow(const AppModel& model);
    void RenderStatsWindow(const AppModel& model);

    ButtonCallback m_onIncrementCounter;
    ButtonCallback m_onResetCounter;
    ValueCallback m_onSliderChanged;
    ColorCallback m_onColorChanged;
};
