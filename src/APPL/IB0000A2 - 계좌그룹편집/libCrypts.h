#pragma once


#ifdef	__cplusplus
extern "C" {
#endif

__declspec(dllexport) char* APIENTRY axEncrypt(char* src, char* keys, int mode);
__declspec(dllexport) char* APIENTRY axDecrypt(char* src, char* keys, int mode);

#ifdef	__cplusplus
}
#endif

