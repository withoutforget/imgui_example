#pragma once

// Model - содержит бизнес-логику и данные приложения
class AppModel {
public:
    AppModel();
    ~AppModel() = default;

    // Геттеры для данных
    float GetSliderValue() const { return m_sliderValue; }
    int GetCounter() const { return m_counter; }
    const float* GetClearColor() const { return m_clearColor; }

    // Сеттеры для данных
    void SetSliderValue(float value) { m_sliderValue = value; }
    void SetClearColor(float r, float g, float b) {
        m_clearColor[0] = r;
        m_clearColor[1] = g;
        m_clearColor[2] = b;
    }

    // Бизнес-логика
    void IncrementCounter() { m_counter++; }
    void ResetCounter() { m_counter = 0; }

    // Обновление состояния (если нужна логика по таймеру и т.д.)
    void Update(float deltaTime);

private:
    float m_sliderValue;
    int m_counter;
    float m_clearColor[4];
};
