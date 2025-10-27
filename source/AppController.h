#pragma once
#include "AppModel.h"
#include "AppView.h"

// Controller - связывает Model и View, обрабатывает события
class AppController {
public:
    AppController(AppModel& model, AppView& view);
    ~AppController() = default;

    // Обновление логики
    void Update(float deltaTime);

    // Отрисовка через view
    void Render();

private:
    void SetupEventHandlers();

    AppModel& m_model;
    AppView& m_view;
};
