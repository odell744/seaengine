#include "window/window.hpp"
#include "error/error.hpp"
#include <iostream>

INT WINAPI WinMain(
    _In_ HINSTANCE hInstance, 
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPSTR lpCmdLine, 
    _In_ INT nCmdShow)
try
{
    const auto pClassName = "seaengine";
    const auto pWindowName = "seaengine - window";

    sea::window::window wnd(640, 480, pWindowName);

    MSG msg;
    BOOL gResult;

    while ((gResult = GetMessage(&msg, nullptr, 0, 0)) > 0)
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    if (gResult == -1)
    {
        return -1;
    }

    return 0;
}
catch (const sea::core::sea_exception& exception)
{
    MessageBox(nullptr, exception.what(), exception.type().data(), MB_OK | MB_ICONERROR);
    return -1;
}
catch (const std::exception& exception)
{
    MessageBox(nullptr, exception.what(), "ERROR", MB_OK | MB_ICONERROR);
    return -1;
}
catch (...)
{
    MessageBox(nullptr, "Undefined Exception", "ERROR", MB_OK | MB_ICONERROR);
    return -1;
}