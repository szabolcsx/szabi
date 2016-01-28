#ifndef SZABI_NON_COPYABLE_H_INCLUDED
#define SZABI_NON_COPYABLE_H_INCLUDED

namespace szabi
{
	class non_copyable
	{
	protected:
		non_copyable() = default;
		~non_copyable() = default;

	public:
		non_copyable(non_copyable const&) = delete;
		non_copyable(non_copyable&&) = delete;
		non_copyable& operator = (non_copyable const&) = delete;
		non_copyable& operator = (non_copyable&&) = delete;
	};
}

#endif /* SZABI_NON_COPYABLE_H_INCLUDED */