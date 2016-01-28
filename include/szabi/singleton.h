#ifndef SZABI_SINGLETON_H_
#define SZABI_SINGLETON_H_

#include "non_copyable.h"

#include <utility>

namespace szabi
{
	template <typename T>
	class singleton// : public non_copyable
	{
	public:
		template <typename... A>
		static T& instance(A... args)
		{
			static T inst{ std::forward<A>(args)... };
			return inst;
		}

	protected:
		explicit singleton<T>() {}
		~singleton<T>() {}
	};
}

#endif /* SZABI_SINGLETON_H_ */