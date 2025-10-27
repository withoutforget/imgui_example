cmake_minimum_required(VERSION 3.10)

project(MyProject VERSION 1.0)

# Устанавливаем стандарт C++
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Указываем директории для временных файлов и бинарников
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/tmp)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/tmp)

# Устанавливаем директорию для временных файлов CMake
set(CMAKE_BINARY_DIR ${CMAKE_SOURCE_DIR}/tmp)

# Подключаем FetchContent для загрузки ImGui
include(FetchContent)

FetchContent_Declare(
    imgui
    GIT_REPOSITORY https://github.com/ocornut/imgui.git
    GIT_TAG v1.91.5
)

FetchContent_MakeAvailable(imgui)

# Создаем библиотеку ImGui с DirectX11 и Win32 бекендами
add_library(imgui STATIC
    ${imgui_SOURCE_DIR}/imgui.cpp
    ${imgui_SOURCE_DIR}/imgui_demo.cpp
    ${imgui_SOURCE_DIR}/imgui_draw.cpp
    ${imgui_SOURCE_DIR}/imgui_tables.cpp
    ${imgui_SOURCE_DIR}/imgui_widgets.cpp
    ${imgui_SOURCE_DIR}/backends/imgui_impl_dx11.cpp
    ${imgui_SOURCE_DIR}/backends/imgui_impl_win32.cpp
)

target_include_directories(imgui PUBLIC 
    ${imgui_SOURCE_DIR}
    ${imgui_SOURCE_DIR}/backends
)

# Линкуем DirectX11 библиотеки
target_link_libraries(imgui PUBLIC d3d11 d3dcompiler dxgi)

# Собираем все исходные файлы из папки source
file(GLOB_RECURSE SOURCES "${CMAKE_SOURCE_DIR}/source/*.cpp")
file(GLOB_RECURSE HEADERS "${CMAKE_SOURCE_DIR}/source/*.h" "${CMAKE_SOURCE_DIR}/source/*.hpp")

# Создаём исполняемый файл
add_executable(${PROJECT_NAME} 
    ${SOURCES} 
    ${HEADERS}
)

# Добавляем директорию source для заголовочных файлов
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/source)

# Линкуем ImGui
target_link_libraries(${PROJECT_NAME} PRIVATE imgui)
