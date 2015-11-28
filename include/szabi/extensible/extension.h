#ifndef SZABI_EXTENSION_H_INCLUDED
#define SZABI_EXTENSION_H_INCLUDED

#ifdef _WIN32
#define EXTENSION_ATTACHER  extern "C" __declspec(dllexport)
#elif linux
#define EXTENSION_ATTACHER  extern "C" __attribute__((visibility("default")))
#endif

#include <string>
#include <memory>

namespace szabi
{
	namespace extensible
	{
		template<typename I>
		class extension : public I
		{
		public:
			extension() {}

			virtual ~extension() {}

			using interface_t = I;
		};
	}
}

#endif /* SZABI_EXTENSION_H_INCLUDED */