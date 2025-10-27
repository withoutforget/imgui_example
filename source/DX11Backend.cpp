#include "DX11Backend.h"
#include "imgui.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_dx11.h"
#include <tchar.h>

// Forward declare message handler from imgui_impl_win32.cpp
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

// Глобальный указатель для доступа в WndProc
static DX11Backend* g_backendInstance = nullptr;

DX11Backend::DX11Backend() {
    g_backendInstance = this;
}

DX11Backend::~DX11Backend() {
    Shutdown();
    g_backendInstance = nullptr;
}

bool DX11Backend::Initialize(int width, int height, const char* title) {
    // Создаем класс окна
    m_wc = { sizeof(m_wc), CS_CLASSDC, WndProc, 0L, 0L, GetModuleHandle(nullptr),
             nullptr, nullptr, nullptr, nullptr, L"ImGuiApp", nullptr };
    ::RegisterClassExW(&m_wc);

    // Конвертируем title в wide string
    int wideLen = MultiByteToWideChar(CP_UTF8, 0, title, -1, nullptr, 0);
    wchar_t* wideTitle = new wchar_t[wideLen];
    MultiByteToWideChar(CP_UTF8, 0, title, -1, wideTitle, wideLen);

    // Создаем окно
    m_hwnd = ::CreateWindowW(m_wc.lpszClassName, wideTitle,
                             WS_OVERLAPPEDWINDOW, 100, 100, width, height,
                             nullptr, nullptr, m_wc.hInstance, nullptr);
    delete[] wideTitle;

    if (!m_hwnd) {
        return false;
    }

    // Инициализируем Direct3D
    if (!CreateDeviceD3D()) {
        CleanupDeviceD3D();
        ::DestroyWindow(m_hwnd);
        ::UnregisterClassW(m_wc.lpszClassName, m_wc.hInstance);
        return false;
    }

    // Показываем окно
    ::ShowWindow(m_hwnd, SW_SHOWDEFAULT);
    ::UpdateWindow(m_hwnd);

    // Инициализируем бекенды ImGui
    ImGui_ImplWin32_Init(m_hwnd);
    ImGui_ImplDX11_Init(m_pd3dDevice, m_pd3dDeviceContext);

    return true;
}

void DX11Backend::Shutdown() {
    ImGui_ImplDX11_Shutdown();
    ImGui_ImplWin32_Shutdown();

    CleanupDeviceD3D();
    
    if (m_hwnd) {
        ::DestroyWindow(m_hwnd);
        m_hwnd = nullptr;
    }
    
    ::UnregisterClassW(m_wc.lpszClassName, m_wc.hInstance);
}

void DX11Backend::NewFrame() {
    ImGui_ImplDX11_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();
}

void DX11Backend::Render() {
    ImGui::Render();
    m_pd3dDeviceContext->OMSetRenderTargets(1, &m_pMainRenderTargetView, nullptr);
    m_pd3dDeviceContext->ClearRenderTargetView(m_pMainRenderTargetView, m_clearColor);
    ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
}

void DX11Backend::Present() {
    m_pSwapChain->Present(1, 0);
}

bool DX11Backend::ShouldClose() {
    return m_shouldClose;
}

void DX11Backend::PollEvents() {
    MSG msg;
    while (::PeekMessage(&msg, nullptr, 0U, 0U, PM_REMOVE)) {
        ::TranslateMessage(&msg);
        ::DispatchMessage(&msg);
        if (msg.message == WM_QUIT) {
            m_shouldClose = true;
        }
    }
}

bool DX11Backend::CreateDeviceD3D() {
    DXGI_SWAP_CHAIN_DESC sd;
    ZeroMemory(&sd, sizeof(sd));
    sd.BufferCount = 2;
    sd.BufferDesc.Width = 0;
    sd.BufferDesc.Height = 0;
    sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    sd.BufferDesc.RefreshRate.Numerator = 60;
    sd.BufferDesc.RefreshRate.Denominator = 1;
    sd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
    sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    sd.OutputWindow = m_hwnd;
    sd.SampleDesc.Count = 1;
    sd.SampleDesc.Quality = 0;
    sd.Windowed = TRUE;
    sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

    UINT createDeviceFlags = 0;
    D3D_FEATURE_LEVEL featureLevel;
    const D3D_FEATURE_LEVEL featureLevelArray[2] = { D3D_FEATURE_LEVEL_11_0, D3D_FEATURE_LEVEL_10_0 };
    
    HRESULT res = D3D11CreateDeviceAndSwapChain(
        nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, createDeviceFlags,
        featureLevelArray, 2, D3D11_SDK_VERSION, &sd,
        &m_pSwapChain, &m_pd3dDevice, &featureLevel, &m_pd3dDeviceContext
    );

    if (res != S_OK) {
        return false;
    }

    CreateRenderTarget();
    return true;
}

void DX11Backend::CleanupDeviceD3D() {
    CleanupRenderTarget();
    if (m_pSwapChain) { m_pSwapChain->Release(); m_pSwapChain = nullptr; }
    if (m_pd3dDeviceContext) { m_pd3dDeviceContext->Release(); m_pd3dDeviceContext = nullptr; }
    if (m_pd3dDevice) { m_pd3dDevice->Release(); m_pd3dDevice = nullptr; }
}

void DX11Backend::CreateRenderTarget() {
    ID3D11Texture2D* pBackBuffer;
    m_pSwapChain->GetBuffer(0, IID_PPV_ARGS(&pBackBuffer));
    m_pd3dDevice->CreateRenderTargetView(pBackBuffer, nullptr, &m_pMainRenderTargetView);
    pBackBuffer->Release();
}

void DX11Backend::CleanupRenderTarget() {
    if (m_pMainRenderTargetView) {
        m_pMainRenderTargetView->Release();
        m_pMainRenderTargetView = nullptr;
    }
}

LRESULT WINAPI DX11Backend::WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam)) {
        return true;
    }

    switch (msg) {
    case WM_SIZE:
        if (wParam == SIZE_MINIMIZED)
            return 0;
        if (g_backendInstance && g_backendInstance->m_pd3dDevice != nullptr) {
            g_backendInstance->CleanupRenderTarget();
            g_backendInstance->m_pSwapChain->ResizeBuffers(0, (UINT)LOWORD(lParam), (UINT)HIWORD(lParam), DXGI_FORMAT_UNKNOWN, 0);
            g_backendInstance->CreateRenderTarget();
        }
        return 0;
    case WM_SYSCOMMAND:
        if ((wParam & 0xfff0) == SC_KEYMENU)
            return 0;
        break;
    case WM_DESTROY:
        ::PostQuitMessage(0);
        return 0;
    }
    return ::DefWindowProcW(hWnd, msg, wParam, lParam);
}
