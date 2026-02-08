#ifndef	_LIBDLL_H
#define	_LIBDLL_H

#if _MSC_VER > 1000
#pragma once
#endif

//__declspec(dllexport) char* axEncrypt(char* src, char * keys, int mode);
//__declspec(dllexport) char* axDecrypt(char* src, char * keys, int mode);
char* axEncrypt(char* src, char * keys, int mode);
char* axDecrypt(char* src, char * keys, int mode);

#endif	// _LIBDLL_H
