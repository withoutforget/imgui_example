#pragma once
#include "AppModel.h"

// View - отвечает за отображение UI
class AppView {
public:
    AppView();
    ~AppView() = default;

    // Отрисовка UI
    void Render(AppModel& model);

private:
    void RenderMainWindow(AppModel& model);
    void RenderStatsWindow(AppModel& model);
};
