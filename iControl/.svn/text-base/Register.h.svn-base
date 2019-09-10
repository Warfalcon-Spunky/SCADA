#ifndef __REGISTER_H__
#define __REGISTER_H__

#ifdef __REGISTER_EXPORT__
#	define REGISTER_API		__declspec(dllexport)
#else
#	define REGISTER_API		__declspec(dllimport)

// setup the library we need to link with...
#	if defined(_DEBUG)
#pragma comment(lib, "Register_d.lib")
#	else
#pragma comment(lib, "Register.lib")
#	endif// End of #if defined(_DEBUG)
#endif// End of #ifdef __REGISTER_EXPORT__

#endif	__REGISTER_H__