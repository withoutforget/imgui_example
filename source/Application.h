#pragma once
#include "IBackend.h"
#include "AppModel.h"
#include "AppView.h"
#include "AppController.h"
#include <memory>

// Главный класс приложения
class Application {
public:
    Application();
    ~Application();

    // Инициализация приложения
    bool Initialize(int width, int height, const char* title);
    
    // Главный цикл приложения
    void Run();
    
    // Завершение работы
    void Shutdown();

private:
    std::unique_ptr<IBackend> m_backend;
    std::unique_ptr<AppModel> m_model;
    std::unique_ptr<AppView> m_view;
    std::unique_ptr<AppController> m_controller;
};
