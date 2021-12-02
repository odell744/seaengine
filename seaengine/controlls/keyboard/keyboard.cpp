#include "keyboard.hpp"

namespace sea
{
	namespace core
	{
		keyboard_event::keyboard_event(e_keyboard_event type, std::uint8_t key_code) noexcept
			:
			m_type(type),
			m_key_code(key_code)
		{};

		bool keyboard_event::is_pressed() const noexcept
		{
			return (m_type == e_keyboard_event::key_pressed);
		}

		bool keyboard_event::is_released() const noexcept
		{
			return (m_type == e_keyboard_event::key_released);
		}

		std::uint8_t keyboard_event::get_key_code() const noexcept
		{
			return m_key_code;
		}
	}
	namespace controls::keyboard
	{
		bool keyboard::key_is_pressed(unsigned char keycode) const noexcept
		{
			return m_key_states[keycode];
		}

		std::optional<keyboard_event> keyboard::read_key() noexcept
		{
			if (m_key_buffer.size() > 0u)
			{
				keyboard_event e = m_key_buffer.front();
				m_key_buffer.pop();
				return e;
			}
			return {};
		}

		bool keyboard::key_is_empty() const noexcept
		{
			return m_key_buffer.empty();
		}

		std::optional<char> keyboard::read_char() noexcept
		{
			if (m_char_buffer.size() > 0u)
			{
				unsigned char charcode = m_char_buffer.front();
				m_char_buffer.pop();
				return charcode;
			}
			return {};
		}

		bool keyboard::char_is_empty() const noexcept
		{
			return m_char_buffer.empty();
		}

		void keyboard::flush_key() noexcept
		{
			m_key_buffer = std::queue<keyboard_event>();
		}

		void keyboard::flush_char() noexcept
		{
			m_char_buffer = std::queue<char>();
		}

		void keyboard::flush() noexcept
		{
			flush_key();
			flush_char();
		}

		void keyboard::enable_autorepeat() noexcept
		{
			m_autorepeat_enabled = true;
		}

		void keyboard::disable_autorepeat() noexcept
		{
			m_autorepeat_enabled = false;
		}

		bool keyboard::auto_repeat_is_enabled() const noexcept
		{
			return m_autorepeat_enabled;
		}

		void keyboard::on_key_pressed(unsigned char keycode) noexcept
		{
			m_key_states[keycode] = true;
			m_key_buffer.push(
				keyboard_event(e_keyboard_event::key_pressed, keycode)
			);
			trim_buffer(m_key_buffer);
		}

		void keyboard::on_key_released(unsigned char keycode) noexcept
		{
			m_key_states[keycode] = false;
			m_key_buffer.push(
				keyboard_event(e_keyboard_event::key_released, keycode)
			);
			trim_buffer(m_key_buffer);
		}

		void keyboard::on_char(char character) noexcept
		{
			m_char_buffer.push(character);
			trim_buffer(m_char_buffer);
		}

		void keyboard::clear_state() noexcept
		{
			m_key_states.reset();
		}

		template<typename BufferType>
		void keyboard::trim_buffer(std::queue<BufferType>& buffer) noexcept
		{
			while (buffer.size() > m_buffer_size)
			{
				buffer.pop();
			}
		}
	}
}
