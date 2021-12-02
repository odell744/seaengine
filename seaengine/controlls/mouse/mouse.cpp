#include "mouse.hpp"

namespace sea
{
	namespace core
	{
		mouse_event::mouse_event(e_mouse_event type, sea::controls::mouse::mouse& mouse) noexcept
			:
			m_type(type),
			m_left_is_pressed(mouse.left_is_pressed()),
			m_right_is_pressed(mouse.right_is_pressed()),
			m_pos(mouse.get_pos())
		{}

		e_mouse_event mouse_event::type() const noexcept
		{
			return m_type;
		}
		vector2_raw mouse_event::pos() const noexcept
		{
			return m_pos;
		}
		int mouse_event::get_x() const noexcept
		{
			return m_pos.x;
		}
		int mouse_event::get_y() const noexcept
		{
			return m_pos.y;
		}
		bool mouse_event::left_is_pressed() const noexcept
		{
			return m_left_is_pressed;
		}
		bool mouse_event::right_is_pressed() const noexcept
		{
			return m_right_is_pressed;
		}
	}
	namespace controls::mouse
	{
		vector2_raw mouse::get_pos() const noexcept
		{
			return m_pos;
		}

		std::optional<vector2_raw> mouse::read_raw_delta() noexcept
		{
			if (m_raw_delta_buffer.empty())
			{
				return std::nullopt;
			}
			const vector2_raw d = m_raw_delta_buffer.front();
			m_raw_delta_buffer.pop();
			return d;
		}

		int mouse::get_pos_x() const noexcept
		{
			return m_pos.x;
		}

		int mouse::get_pos_y() const noexcept
		{
			return m_pos.y;
		}

		bool mouse::is_in_window() const noexcept
		{
			return m_is_in_window;
		}

		bool mouse::left_is_pressed() const noexcept
		{
			return m_left_is_pressed;
		}

		bool mouse::right_is_pressed() const noexcept
		{
			return m_right_is_pressed;
		}

		std::optional<mouse_event> mouse::read() noexcept
		{
			if (m_buffer.size() > 0u)
			{
				mouse_event e = m_buffer.front();
				m_buffer.pop();
				return e;
			}
			return {};
		}

		void mouse::flush() noexcept
		{
			m_buffer = std::queue<mouse_event>();
		}

		void mouse::enable_raw() noexcept
		{
			m_raw_enabled = true;
		}

		void mouse::disable_raw() noexcept
		{
			m_raw_enabled = false;
		}

		bool mouse::raw_enabled() const noexcept
		{
			return m_raw_enabled;
		}

		void mouse::on_mouse_move(int newx, int newy) noexcept
		{
			m_pos = {
				.x = newx,
				.y = newy
			};

			m_buffer.push(mouse_event(e_mouse_event::move, *this));
			trim_buffer();
		}

		void mouse::on_mouse_leave() noexcept
		{
			m_is_in_window = false;
			m_buffer.push(mouse_event(e_mouse_event::leave, *this));
			trim_buffer();
		}

		void mouse::on_mouse_enter() noexcept
		{
			m_is_in_window = true;
			m_buffer.push(mouse_event(e_mouse_event::enter, *this));
			trim_buffer();
		}

		void mouse::on_raw_delta(int dx, int dy) noexcept
		{
			m_raw_delta_buffer.push({ dx,dy });
			trim_buffer();
		}

		void mouse::on_left_pressed(int x, int y) noexcept
		{
			m_left_is_pressed = true;

			m_buffer.push(mouse_event(e_mouse_event::left_pressed, *this));
			trim_buffer();
		}

		void mouse::on_left_released(int x, int y) noexcept
		{
			m_left_is_pressed = false;

			m_buffer.push(mouse_event(e_mouse_event::left_released, *this));
			trim_buffer();
		}

		void mouse::on_right_pressed(int x, int y) noexcept
		{
			m_right_is_pressed = true;

			m_buffer.push(mouse_event(e_mouse_event::right_pressed, *this));
			trim_buffer();
		}

		void mouse::on_right_released(int x, int y) noexcept
		{
			m_right_is_pressed = false;

			m_buffer.push(mouse_event(e_mouse_event::right_released, *this));
			trim_buffer();
		}

		void mouse::on_wheel_up(int x, int y) noexcept
		{
			m_buffer.push(mouse_event(e_mouse_event::wheel_up, *this));
			trim_buffer();
		}

		void mouse::on_wheel_down(int x, int y) noexcept
		{
			m_buffer.push(mouse_event(e_mouse_event::wheel_down, *this));
			trim_buffer();
		}

		void mouse::trim_buffer() noexcept
		{
			while (m_buffer.size() > m_buffer_size)
			{
				m_buffer.pop();
			}
		}

		void mouse::trim_raw_input_buffer() noexcept
		{
			while (m_raw_delta_buffer.size() > m_buffer_size)
			{
				m_raw_delta_buffer.pop();
			}
		}

		void mouse::on_wheel_delta(int x, int y, int delta) noexcept
		{
			m_wheel_delta_carry += delta;
			// generate events for every 120 
			while (m_wheel_delta_carry >= WHEEL_DELTA)
			{
				m_wheel_delta_carry -= WHEEL_DELTA;
				on_wheel_up(x, y);
			}
			while (m_wheel_delta_carry <= -WHEEL_DELTA)
			{
				m_wheel_delta_carry += WHEEL_DELTA;
				on_wheel_down(x, y);
			}
		}

		bool mouse::is_empty() const noexcept
		{
			return m_buffer.empty();
		}
	}
}
