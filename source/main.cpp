#include "Application.h"
#include <memory>

int main(int argc, char** argv) {
    // Создаем приложение
    auto app = std::make_unique<Application>();

    // Инициализируем
    if (!app->Initialize(1280, 800, "ImGui MVC Application")) {
        return 1;
    }

    // Запускаем главный цикл
    app->Run();

    // Завершаем работу
    app->Shutdown();

    return 0;
}
