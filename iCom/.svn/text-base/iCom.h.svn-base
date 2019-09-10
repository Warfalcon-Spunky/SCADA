#ifndef _ICOM_H_
#define _ICOM_H_

#ifdef _ICOM_EXPORT_
#	define ICOM_API		__declspec(dllexport)
#else
#	define ICOM_API		__declspec(dllimport)
// setup the library we need to link with...
#	if defined(_DEBUG)
#pragma comment(lib, "LKComD.lib")
#	else
#pragma comment(lib, "LKCom.lib")
#	endif// End of #if defined(_DEBUG)
#endif// End of #ifdef _ICOM_EXPORT_

#endif// End of #ifndef _ICOM_H_