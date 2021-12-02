#include "vector2.hpp"

namespace sea::math::vectors
{
	vector2::vector2(int x, int y) noexcept
		:
		m_value{ .x = x, .y = y }
	{};

	vector2_raw vector2::operator()() const
	{
		return m_value;
	}
}