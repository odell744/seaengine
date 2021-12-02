#ifndef SEA_ENGINE_KEYBOARD_INCLUDED
#define SEA_ENGINE_KEYBOARD_INCLUDED

#include "../../window/window.hpp"

#include <optional>
#include <queue>
#include <bitset>

#include <boost/noncopyable.hpp>

namespace sea
{
	namespace core
	{
		enum e_keyboard_event : std::uint8_t
		{
			key_pressed = 0x01,
			key_released = 0x02,
		};
		class keyboard_event
		{
			e_keyboard_event	m_type;
			std::uint8_t		m_key_code;

		public:
			keyboard_event(e_keyboard_event type, std::uint8_t key_code) noexcept;
			bool is_pressed() const noexcept;
			bool is_released() const noexcept;
			std::uint8_t get_key_code() const noexcept;
		};
	}
	namespace controls::keyboard
	{
		using core::keyboard_event;
		using core::e_keyboard_event;

		class keyboard : boost::noncopyable
		{
			friend class sea::window::window;

			template<typename BufferType>
			static void trim_buffer(std::queue<BufferType>& buffer) noexcept;
			
			void on_key_pressed(std::uint8_t keycode) noexcept;
			void on_key_released(std::uint8_t keycode) noexcept;

			void on_char(char character) noexcept;

			void clear_state() noexcept;

			static constexpr unsigned int m_keys = 256u;
			static constexpr unsigned int m_buffer_size = 16u;

			bool m_autorepeat_enabled = false;
			
			std::bitset<m_keys> m_key_states;
			std::queue<keyboard_event> m_key_buffer;
			std::queue<char> m_char_buffer;
		public:
			keyboard() = default;

			bool key_is_pressed(std::uint8_t key_code) const noexcept;

			void enable_autorepeat() noexcept;
			void disable_autorepeat() noexcept;
			bool auto_repeat_is_enabled() const noexcept;

			bool key_is_empty() const noexcept;
			bool char_is_empty() const noexcept;

			void flush_key() noexcept;
			void flush_char() noexcept;
			void flush() noexcept;

			std::optional<keyboard_event> read_key() noexcept;
			std::optional<char> read_char() noexcept;
		};
	}
}

#endif // !SEA_ENGINE_KEYBOARD_INCLUDED
