#ifndef SEA_CORE_EXCEPTION
#define SEA_CORE_EXCEPTION

#include "../window/window_include.hpp"
#include <cinttypes>
#include <string_view>

#define __FILENAME__ strrchr(__FILE__, '\\') + 1

namespace sea::core
{
	class sea_exception : public std::exception
	{
		std::string_view		m_file;
		std::string				m_line;
		std::string				m_reason;
	public:
		sea_exception(std::string_view file, std::uint32_t line, std::string_view reason) noexcept;
		~sea_exception() noexcept = default;

		const char* what() const noexcept override;
		virtual std::string_view type() const;
		std::string_view file() const;
		std::string_view line() const;
		std::string_view reason() const;
	protected:
		mutable std::string what_buffer;
	};

	class sea_window_exception : public sea::core::sea_exception
	{
	public:
		sea_window_exception(std::string_view file, std::uint32_t line, std::string_view reason) noexcept;
		~sea_window_exception() noexcept = default;

		std::string_view type() const override;
	};

	std::string hresult_error_string(HRESULT h_result);
}

#define sea_throw(reason)								sea::core::sea_exception(__FILENAME__, __LINE__, reason)
#define sea_throw_window(reason)						sea::core::sea_window_exception(__FILENAME__, __LINE__, reason)

#define sea_hresult_to_string(hr)						sea::core::hresult_error_string(hr)
#define sea_hresult_last_exception(throw_func)			throw_func(sea_hresult_to_string(GetLastError()))

#endif // !SEA_CORE_EXCEPTION