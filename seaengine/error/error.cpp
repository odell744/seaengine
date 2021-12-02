#include "error.hpp"
#include <string>
#include <format>

namespace sea::core
{
	sea_exception::sea_exception(std::string_view file, std::uint32_t line, std::string_view reason) noexcept
		:
		m_file(file),
		m_line(std::to_string(line)),
		m_reason(reason)
	{}
	sea_window_exception::sea_window_exception(std::string_view file, std::uint32_t line, std::string_view reason) noexcept
		:
		sea_exception(file, line, reason)
	{
	};

	const char* sea_exception::what() const noexcept
	{
		size_t buffer_size = type().size() + m_file.size() + m_line.size() + m_reason.size() + 7;
		if (what_buffer.size() < buffer_size)
		{
			what_buffer.reserve(buffer_size);

			std::format_to(std::back_inserter(what_buffer), "[{}:{}] {}: {}", m_file, m_line, type(), m_reason);
		}
		return what_buffer.c_str();
	}

	std::string_view sea_exception::type() const
	{
		return "SeaException";
	}
	std::string_view sea_window_exception::type() const
	{
		return "SeaWindowException";
	};

	std::string_view sea_exception::file() const
	{
		return m_file;
	}
	std::string_view sea_exception::line() const
	{
		return m_line;
	}
	std::string_view sea_exception::reason() const
	{
		return m_reason;
	}


	std::string hresult_error_string(HRESULT h_result)
	{
		char* l_buffer = nullptr;

		DWORD l_msg_len = FormatMessage(
			FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
			nullptr, h_result, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
			reinterpret_cast<LPSTR>(&l_buffer), 0, nullptr
		);

		if (l_msg_len == 0)
		{
			return "Undefined Error Code";
		}

		std::string error = l_buffer;
		LocalFree(l_buffer);

		return error;
	}
}