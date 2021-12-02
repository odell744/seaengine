#ifndef SEA_CORE_WINDOW
#define SEA_CORE_WINDOW

#include "window_include.hpp"

#include <string_view>
#include <cinttypes>

namespace sea
{
	namespace window
	{
		namespace core
		{
			class window_class
			{
			private:
				HINSTANCE m_h_instance;
				static constexpr const char* m_wnd_class_name = "S3AEngine Direct3D11 Window";
			public:
				window_class() noexcept;
				~window_class();
				window_class(const window_class&) = delete;
				window_class& operator=(const window_class&) = delete;

				std::string_view get_name() noexcept;
				HINSTANCE get_instance() noexcept;
			};
		}

		class window
		{
			friend class sea::window::core::window_class;

			core::window_class	m_wnd_class;

			HWND				m_hwnd;

			std::int32_t		m_width;
			std::int32_t		m_height;
			std::string_view	m_name;

			static LRESULT WINAPI HandleMessageSetup(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) noexcept;
			static LRESULT WINAPI HandleMessageThunk(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) noexcept;
			LRESULT WINAPI		  HandleMessage(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) noexcept;
		public:
			window(std::int32_t width, std::int32_t height, const char* name);
			~window();
			window(const window&) = delete;
			window& operator=(const window&) = delete;
		};
	}
}

#endif // !SEA_CORE_WINDOW