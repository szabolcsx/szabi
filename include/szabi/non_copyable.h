#ifndef SZABI_NON_COPYABLE_H_INCLUDED
#define SZABI_NON_COPYABLE_H_INCLUDED

namespace szabi
{
	class non_copyable
	{
	protected:
		non_copyable() = default;
		~non_copyable() = default;

		non_copyable(non_copyable const&) = delete;
		void operator = (non_copyable const&) = delete;
	};
}

#endif /* SZABI_NON_COPYABLE_H_INCLUDED */