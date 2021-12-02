#include "window.hpp"
#include "../error/error.hpp"

namespace sea
{
	namespace window
	{
        namespace core
        {
            window_class::window_class() noexcept
                :
                m_h_instance(GetModuleHandle(nullptr))
            {
                WNDCLASSEX wc = {  
                    .cbSize = sizeof(wc),
                    .style = CS_OWNDC,
                    .lpfnWndProc = sea::window::window::HandleMessageSetup,
                    .cbClsExtra = 0,
                    .cbWndExtra = 0,
                    .hInstance = m_h_instance,
                    .hIcon = nullptr,
                    .hCursor = nullptr,
                    .hbrBackground = nullptr,
                    .lpszMenuName = nullptr,
                    .lpszClassName = m_wnd_class_name,
                    .hIconSm = nullptr
                };
                RegisterClassEx(&wc);
            }

            window_class::~window_class()
            {
                UnregisterClass(m_wnd_class_name, m_h_instance);
            }

            HINSTANCE window_class::get_instance() noexcept
            {
                return m_h_instance;
            }
            std::string_view window_class::get_name() noexcept
            {
                return m_wnd_class_name;
            }
        }
		
        window::window(std::int32_t width, std::int32_t height, const char* name)
            :
            m_width(width), m_height(height),
            m_wnd_class(),
            m_name(name)
        {
            RECT wr{
                .left = 100,
                .top = 100,
                .right = m_width + 100,
                .bottom = m_height + 100
            };

            if (AdjustWindowRect(&wr, WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU, FALSE) == 0)
            {
                throw sea_hresult_last_exception(sea_throw_window);
            }

            m_hwnd = CreateWindowEx(
                NULL,
                m_wnd_class.get_name().data(),
                m_name.data(),
                WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU,
                CW_USEDEFAULT, CW_USEDEFAULT, 
                wr.right - wr.left, 
                wr.bottom - wr.top,
                nullptr, nullptr,
                m_wnd_class.get_instance(), this
            );

            if (!m_hwnd)
            {
                throw sea_hresult_last_exception(sea_throw_window);
            }

            ShowWindow(m_hwnd, SW_SHOW);
        }

        window::~window()
        {
            DestroyWindow(m_hwnd);
        }

        LRESULT WINAPI window::HandleMessageSetup(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) noexcept
        {
            if (msg == WM_NCCREATE)
            {
                const CREATESTRUCTW* const pCreate = reinterpret_cast<CREATESTRUCTW*>(lparam);
                window* const pWnd = static_cast<window*>(pCreate->lpCreateParams);
                SetWindowLongPtr(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pWnd));
                SetWindowLongPtr(hwnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(&window::HandleMessageThunk));
                return pWnd->HandleMessage(hwnd, msg, wparam, lparam);
            }

            return DefWindowProc(hwnd, msg, wparam, lparam);
        }

        LRESULT WINAPI window::HandleMessageThunk(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) noexcept
        {
            window* const pWnd = reinterpret_cast<window*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));
            return pWnd->HandleMessage(hwnd, msg, wparam, lparam);
        }

        LRESULT WINAPI window::HandleMessage(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) noexcept
        {
            switch (msg)
            {
            case WM_CLOSE:
                PostQuitMessage(0);
                break;
            }
            return DefWindowProc(hwnd, msg, wparam, lparam);
        }
	}
}
