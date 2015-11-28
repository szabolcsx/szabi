#ifndef SZABI_EXTENSIBLE_EXPORT_H_INCLUDED
#define SZABI_EXTENSIBLE_EXPORT_H_INCLUDED

#ifdef _WINDLL
/* We are building this library */
#define EXTENSIBLE __declspec(dllexport)
#else
/* We are using this library */
#define EXTENSIBLE __declspec(dllimport)
#endif

#endif /* SZABI_EXTENSIBLE_EXPORT_H_INCLUDED */