#ifndef SZABI_VARIADIC_PLACEHOLDER_H_INCLUDED
#define SZABI_VARIADIC_PLACEHOLDER_H_INCLUDED

#include <functional>

namespace szabi
{
	namespace functors
	{
		template <std::size_t N>
		struct placeholder {};
	}

	struct placeholder_factory
	{
		template <std::size_t N>
		static functors::placeholder<N + 1> make()
		{
			return functors::placeholder<N + 1>();
		}
	};
}

namespace std
{
	template<size_t N>
	struct is_placeholder<szabi::functors::placeholder<N>> : public integral_constant<int, N> {};
}

#endif /* SZABI_VARIADIC_PLACEHOLDER_H_INCLUDED */