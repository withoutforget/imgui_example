#include "AppModel.h"

AppModel::AppModel()
    : m_sliderValue(0.0f)
    , m_counter(0)
    , m_clearColor{ 0.45f, 0.55f, 0.60f, 1.00f }
{
}

void AppModel::Update(float deltaTime) {
    // Здесь можно добавить логику обновления, например:
    // - автоматическое изменение значений
    // - проверка условий
    // - вычисления
}
