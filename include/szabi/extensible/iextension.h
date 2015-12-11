#ifndef SZABI_IEXTENSION_H_INCLUDED
#define SZABI_IEXTENSION_H_INCLUDED

#include <szabi\extensible\version.h>

#include <string>

namespace szabi
{
	namespace extensible
	{
		class iextension
		{
		public:
			static szabi::version api_version();

			static std::string name();
		};
	}
}

#endif /* SZABI_IEXTENSION_H_INCLUDED */