#include <szabi\extensible\version.h>

#include <tuple>

namespace szabi
{
	EXTENSIBLE bool operator>(const version& a, const version& b)
	{
		return std::tie(a.major, a.minor) > std::tie(b.major, b.minor);
	}

	EXTENSIBLE bool operator<(const version& a, const version& b)
	{
		return std::tie(a.major, a.minor) < std::tie(b.major, b.minor);
	}

	EXTENSIBLE bool operator>=(const version& a, const version& b)
	{
		return std::tie(a.major, a.minor) >= std::tie(b.major, b.minor);
	}

	EXTENSIBLE bool operator<=(const version& a, const version& b)
	{
		return std::tie(a.major, a.minor) <= std::tie(b.major, b.minor);
	}

	EXTENSIBLE bool operator==(const version& a, const version& b)
	{
		return std::tie(a.major, a.minor) == std::tie(b.major, b.minor);
	}

	EXTENSIBLE bool operator!=(const version& a, const version& b)
	{
		return std::tie(a.major, a.minor) != std::tie(b.major, b.minor);
	}
}