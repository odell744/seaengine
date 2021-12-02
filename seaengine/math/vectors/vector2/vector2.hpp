#ifndef SEA_VECTOR_2D
#define SEA_VECTOR_2D

namespace sea::math::vectors
{
	typedef struct Point2D
	{
		int x{};
		int y{};
	} vector2_raw;

	class vector2
	{
		vector2_raw m_value;
	public:
		vector2(int x, int y) noexcept;
		~vector2() noexcept = default;

		vector2_raw operator()() const;
	};
}

using sea::math::vectors::vector2;
using sea::math::vectors::Point2D;
using sea::math::vectors::vector2_raw;

#endif // !SEA_VECTOR_2D
