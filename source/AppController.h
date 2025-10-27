#pragma once
#include "AppModel.h"
#include "AppView.h"

// Controller - обрабатывает пользовательский ввод и обновляет модель
class AppController {
public:
    AppController(AppModel& model, AppView& view);
    ~AppController() = default;

    // Обработка UI событий и обновление модели
    void HandleInput();
    
    // Обновление логики
    void Update(float deltaTime);
    
    // Отрисовка через view
    void Render();

private:
    AppModel& m_model;
    AppView& m_view;
};
