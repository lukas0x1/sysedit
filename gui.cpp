#include <fstream>
#include <vector>
#include "gui.h"
#include "systemdata.h"

#include "include/imgui/imgui.h"
#include "include/imgui/backends/imgui_impl_dx9.h"
#include "include/imgui/backends/imgui_impl_win32.h"

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(
        HWND window,
UINT message,
        WPARAM wideParameter,
LPARAM longParameter
);

long __stdcall WindowProcess(
        HWND window,
        UINT message,
        WPARAM wideParameter,
        LPARAM longParameter)
{
    if (ImGui_ImplWin32_WndProcHandler(window, message, wideParameter, longParameter))
        return true;

    switch (message)
    {
        case WM_SIZE: {
            if (gui::device && wideParameter != SIZE_MINIMIZED)
            {
                gui::presentParameters.BackBufferWidth = LOWORD(longParameter);
                gui::presentParameters.BackBufferHeight = HIWORD(longParameter);
                gui::ResetDevice();
            }
        }return 0;

        case WM_SYSCOMMAND: {
            if ((wideParameter & 0xfff0) == SC_KEYMENU) // Disable ALT application menu
                return 0;
        }break;

        case WM_DESTROY: {
            PostQuitMessage(0);
        }return 0;

        case WM_LBUTTONDOWN: {
            gui::position = MAKEPOINTS(longParameter); // set click points
        }return 0;

        case WM_MOUSEMOVE: {
            if (wideParameter == MK_LBUTTON)
            {
                const auto points = MAKEPOINTS(longParameter);
                auto rect = ::RECT{ };

                GetWindowRect(gui::window, &rect);

                rect.left += points.x - gui::position.x;
                rect.top += points.y - gui::position.y;

                if (gui::position.x >= 0 &&
                    gui::position.x <= gui::WIDTH &&
                    gui::position.y >= 0 && gui::position.y <= 19)
                    SetWindowPos(
                            gui::window,
                            HWND_TOPMOST,
                            rect.left,
                            rect.top,
                            0, 0,
                            SWP_SHOWWINDOW | SWP_NOSIZE | SWP_NOZORDER
                    );
            }

        }return 0;

    }

    return DefWindowProc(window, message, wideParameter, longParameter);
}

void gui::CreateHWindow(const char* windowName) noexcept
{
    windowClass.cbSize = sizeof(WNDCLASSEX);
    windowClass.style = CS_CLASSDC;
    windowClass.lpfnWndProc = reinterpret_cast<WNDPROC>(WindowProcess);
    windowClass.cbClsExtra = 0;
    windowClass.cbWndExtra = 0;
    windowClass.hInstance = GetModuleHandleA(0);
    windowClass.hIcon = 0;
    windowClass.hCursor = 0;
    windowClass.hbrBackground = 0;
    windowClass.lpszMenuName = 0;
    windowClass.lpszClassName = "class001";
    windowClass.hIconSm = 0;

    RegisterClassEx(&windowClass);

    window = CreateWindowEx(
            0,
            "class001",
            windowName,
            WS_POPUP,
            100,
            100,
            WIDTH,
            HEIGHT,
            0,
            0,
            windowClass.hInstance,
            0
    );

    ShowWindow(window, SW_SHOWDEFAULT);
    UpdateWindow(window);
}

void gui::DestroyHWindow() noexcept
{
    DestroyWindow(window);
    UnregisterClass(windowClass.lpszClassName, windowClass.hInstance);
}

bool gui::CreateDevice() noexcept
{
    d3d = Direct3DCreate9(D3D_SDK_VERSION);

    if (!d3d)
        return false;

    ZeroMemory(&presentParameters, sizeof(presentParameters));

    presentParameters.Windowed = TRUE;
    presentParameters.SwapEffect = D3DSWAPEFFECT_DISCARD;
    presentParameters.BackBufferFormat = D3DFMT_UNKNOWN;
    presentParameters.EnableAutoDepthStencil = TRUE;
    presentParameters.AutoDepthStencilFormat = D3DFMT_D16;
    presentParameters.PresentationInterval = D3DPRESENT_INTERVAL_ONE;

    if (d3d->CreateDevice(
            D3DADAPTER_DEFAULT,
            D3DDEVTYPE_HAL,
            window,
            D3DCREATE_HARDWARE_VERTEXPROCESSING,
            &presentParameters,
            &device) < 0)
        return false;

    return true;
}

void gui::ResetDevice() noexcept
{
    ImGui_ImplDX9_InvalidateDeviceObjects();

    const auto result = device->Reset(&presentParameters);

    if (result == D3DERR_INVALIDCALL)
        IM_ASSERT(0);

    ImGui_ImplDX9_CreateDeviceObjects();
}

void gui::DestroyDevice() noexcept
{
    if (device)
    {
        device->Release();
        device = nullptr;
    }

    if (d3d)
    {
        d3d->Release();
        d3d = nullptr;
    }
}

void gui::CreateImGui() noexcept
{
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ::ImGui::GetIO();

    io.IniFilename = NULL;

    ImGui::StyleColorsDark();

    ImGui_ImplWin32_Init(window);
    ImGui_ImplDX9_Init(device);
}

void gui::DestroyImGui() noexcept
{
    ImGui_ImplDX9_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();
}

void gui::BeginRender() noexcept
{
    MSG message;
    while (PeekMessage(&message, 0, 0, 0, PM_REMOVE))
    {
        TranslateMessage(&message);
        DispatchMessage(&message);

        if (message.message == WM_QUIT)
        {
            isRunning = !isRunning;
            return;
        }
    }

    // Start the Dear ImGui frame
    ImGui_ImplDX9_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();
}

void gui::EndRender() noexcept
{
    ImGui::EndFrame();

    device->SetRenderState(D3DRS_ZENABLE, FALSE);
    device->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
    device->SetRenderState(D3DRS_SCISSORTESTENABLE, FALSE);

    device->Clear(0, 0, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_RGBA(0, 0, 0, 255), 1.0f, 0);

    if (device->BeginScene() >= 0)
    {
        ImGui::Render();
        ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());
        device->EndScene();
    }

    const auto result = device->Present(0, 0, 0, 0);

    // Handle loss of D3D9 device
    if (result == D3DERR_DEVICELOST && device->TestCooperativeLevel() == D3DERR_DEVICENOTRESET)
        ResetDevice();
}










void load(){
    ImGui::InputText("##", save::path, MAX_PATH);
    ImGui::SameLine();
    if(ImGui::Button("open")){
        save::selectFile();
    }
    if(ImGui::Button("submit")) {
        save::loadSave();
    }
    return;
}

void Android(){
    ImGui::Text("Android");

    static const char* items[5]{
            "Episode 1", "Episode 2", "Episode 3", "Episode 4", "Episode 5"
    };
    static int current_gs1 = (int)save::systemDataAndroid->sce_data.GS1_Scenario_enable / 16 - 1; //sce_data is represented as 0x10 to 0x50 in 0x10 steps
    static int current_gs2 = (int)save::systemDataAndroid->sce_data.GS2_Scenario_enable / 16 - 1;
    static int current_gs3 = (int)save::systemDataAndroid->sce_data.GS3_Scenario_enable / 16 - 1;

    static const float width = ImGui::GetWindowWidth();
    static const float combo_width = width * 0.25f;
    ImGui::SetNextItemWidth(combo_width);
    ImGui::Combo("##", &current_gs1, items, 5, -1);
    ImGui::SetNextItemWidth(combo_width);
    ImGui::SameLine();
    ImGui::Combo("###", &current_gs2, items, 4, -1);
    ImGui::SetNextItemWidth(combo_width);
    ImGui::SameLine();
    ImGui::Combo("Game progress", &current_gs3, items, 5, -1);

    save::systemDataAndroid->sce_data.GS1_Scenario_enable = (std::byte)((current_gs1 + 1) * 16);
    save::systemDataAndroid->sce_data.GS2_Scenario_enable = (std::byte)((current_gs2 + 1) * 16);
    save::systemDataAndroid->sce_data.GS3_Scenario_enable = (std::byte)((current_gs3 + 1) * 16);
}

void Steam(){






    ImGui::Text("Steam");

    static const char* items[5]{
            "Episode 1", "Episode 2", "Episode 3", "Episode 4", "Episode 5"
    };
    static int current_gs1 = (int)save::systemData->sce_data.GS1_Scenario_enable / 16 - 1; //sce_data is represented as 0x10 to 0x50 in 0x10 steps
    static int current_gs2 = (int)save::systemData->sce_data.GS2_Scenario_enable / 16 - 1;
    static int current_gs3 = (int)save::systemData->sce_data.GS3_Scenario_enable / 16 - 1;

    static const float width = ImGui::GetWindowWidth();
    static const float combo_width = width * 0.25f;
    ImGui::SetNextItemWidth(combo_width);
    ImGui::Combo("##", &current_gs1, items, 5, -1);
    ImGui::SetNextItemWidth(combo_width);
    ImGui::SameLine();
    ImGui::Combo("###", &current_gs2, items, 4, -1);
    ImGui::SetNextItemWidth(combo_width);
    ImGui::SameLine();
    ImGui::Combo("Game progress", &current_gs3, items, 5, -1);

    save::systemData->sce_data.GS1_Scenario_enable = (std::byte)((current_gs1 + 1) * 16);
    save::systemData->sce_data.GS2_Scenario_enable = (std::byte)((current_gs2 + 1) * 16);
    save::systemData->sce_data.GS3_Scenario_enable = (std::byte)((current_gs3 + 1) * 16);

    if(ImGui::Button("Save")) save::SaveData();


}

void gui::Render() noexcept
{
    ImGui::SetNextWindowPos({ 0, 0 });
    ImGui::SetNextWindowSize({ WIDTH, HEIGHT });
    ImGui::Begin(
            "SaveEditor",
            &isRunning,
            ImGuiWindowFlags_NoResize |
            ImGuiWindowFlags_NoSavedSettings |
            ImGuiWindowFlags_NoCollapse |
            ImGuiWindowFlags_NoMove
    );


    switch(save::save_type){
        case save::save_type::Invalid:
            load();
            break;
        case save::save_type::Android:
            Android();
            break;
        case save::save_type::Steam:
            Steam();
            break;
        default:
            break;
    }

    ImGui::End();
}





