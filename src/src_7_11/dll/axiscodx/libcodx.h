#ifndef _LIBCODX_H_
#define	_LIBCODX_H_

#ifdef	__cplusplus
extern "C" {
#endif

__declspec(dllexport) char*  APIENTRY axGetMaster(char* root, int* ncode);
__declspec(dllexport) bool   APIENTRY axGetCategory(char* root, int kind, int index, char* code, bool prev);
__declspec(dllexport) int    APIENTRY axGetSpinData(DWORD type, int nfloat, int xfloat, char* data, double value, bool prev);
__declspec(dllexport) double APIENTRY axGetDelta(DWORD type, double value, bool prev);


#ifdef	__cplusplus
}
#endif

#endif