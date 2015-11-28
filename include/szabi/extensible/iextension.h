#ifndef SZABI_IEXTENSION_H_INCLUDED
#define SZABI_IEXTENSION_H_INCLUDED

#include <szabi\extensible\version.h>

namespace szabi
{
	namespace extensible
	{
		class iextension
		{
		public:
			static szabi::version api_version();
		};
	}
}

#endif /* SZABI_IEXTENSION_H_INCLUDED */