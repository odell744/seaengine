#ifndef SEA_ENGINE_MOUSE_INCLUDED
#define SEA_ENGINE_MOUSE_INCLUDED

#include "../../window/window.hpp"
#include "../../math/vectors/vector2/vector2.hpp"

#include <queue>
#include <optional>

#include <boost/noncopyable.hpp>

namespace sea
{
	namespace core
	{
		enum e_mouse_event
		{
			left_pressed,
			left_released,
			right_pressed,
			right_released,
			wheel_up,
			wheel_down,
			move,
			enter,
			leave,
		};
		class mouse_event
		{
			e_mouse_event m_type;
			bool m_left_is_pressed;
			bool m_right_is_pressed;
			vector2_raw m_pos;
		public:
			mouse_event(e_mouse_event type, sea::controls::mouse::mouse& mouse) noexcept;

			e_mouse_event type() const noexcept;

			vector2_raw pos() const noexcept;
			std::int32_t get_x() const noexcept;
			std::int32_t get_y() const noexcept;

			bool left_is_pressed() const noexcept;
			bool right_is_pressed() const noexcept;
		};
	}
	namespace controls::mouse
	{
		using sea::core::e_mouse_event;
		using sea::core::mouse_event;

		class mouse : boost::noncopyable
		{
			friend class sea::window::window;

			static constexpr std::uint8_t	m_buffer_size{ 16u };
			\
			vector2_raw						m_pos;

			std::int32_t					m_wheel_delta_carry{ 0 };

			bool							m_left_is_pressed{ 0 };
			bool							m_right_is_pressed{ 0 };
			bool							m_is_in_window{ 0 };
			bool							m_raw_enabled{ 0 };


			std::queue<mouse_event>			m_buffer;
			std::queue<vector2_raw>			m_raw_delta_buffer;

			void on_mouse_move(int x, int y) noexcept;
			void on_mouse_leave() noexcept;
			void on_mouse_enter() noexcept;
			
			void on_raw_delta(int dx, int dy) noexcept;
			
			void on_left_pressed(int x, int y) noexcept;
			void on_left_released(int x, int y) noexcept;
			
			void on_right_pressed(int x, int y) noexcept;
			void on_right_released(int x, int y) noexcept;

			void on_wheel_up(int x, int y) noexcept;
			void on_wheel_down(int x, int y) noexcept;
			void on_wheel_delta(int x, int y, int delta) noexcept;

			void trim_buffer() noexcept;
			void trim_raw_input_buffer() noexcept;
		public:
			mouse() = default;

			vector2_raw get_pos() const noexcept;
			std::optional<vector2_raw> read_raw_delta() noexcept;

			std::int32_t get_pos_x() const noexcept;
			std::int32_t get_pos_y() const noexcept;

			bool is_in_window() const noexcept;

			bool left_is_pressed() const noexcept;
			bool right_is_pressed() const noexcept;

			void enable_raw() noexcept;
			void disable_raw() noexcept;
			bool raw_enabled() const noexcept;

			bool is_empty() const noexcept;
			void flush() noexcept;

			std::optional<mouse_event> read() noexcept;
		};
	}
}

#endif // !SEA_ENGINE_MOUSE_INCLUDED
