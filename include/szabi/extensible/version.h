#ifndef SZABI_VERSION_H_INCLUDED
#define SZABI_VERSION_H_INCLUDED

#include <szabi\extensible\extensible_export.h>

namespace szabi
{
	struct EXTENSIBLE version
	{
		int major;
		int minor;
	};

	EXTENSIBLE bool operator>(const version&, const version&);

	EXTENSIBLE bool operator<(const version&, const version&);

	EXTENSIBLE bool operator>=(const version&, const version&);

	EXTENSIBLE bool operator<=(const version&, const version&);

	EXTENSIBLE bool operator==(const version&, const version&);

	EXTENSIBLE bool operator!=(const version&, const version&);
}

#endif /* SZABI_VERSION_H_INCLUDED */