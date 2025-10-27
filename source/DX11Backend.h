#pragma once
#include "IBackend.h"
#include <d3d11.h>
#include <Windows.h>

// Реализация бекенда на DirectX11
class DX11Backend : public IBackend {
public:
    DX11Backend();
    ~DX11Backend() override;

    bool Initialize(int width, int height, const char* title) override;
    void Shutdown() override;
    void NewFrame() override;
    void Render() override;
    void Present() override;
    bool ShouldClose() override;
    void PollEvents() override;

private:
    bool CreateDeviceD3D();
    void CleanupDeviceD3D();
    void CreateRenderTarget();
    void CleanupRenderTarget();
    
    static LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

private:
    HWND m_hwnd = nullptr;
    WNDCLASSEXW m_wc = {};
    
    ID3D11Device* m_pd3dDevice = nullptr;
    ID3D11DeviceContext* m_pd3dDeviceContext = nullptr;
    IDXGISwapChain* m_pSwapChain = nullptr;
    ID3D11RenderTargetView* m_pMainRenderTargetView = nullptr;
    
    bool m_shouldClose = false;
    float m_clearColor[4] = { 0.45f, 0.55f, 0.60f, 1.00f };
};
