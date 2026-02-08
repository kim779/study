#pragma once
#ifdef	__cplusplus
extern "C" {
#endif
__declspec(dllexport) void WINAPI axCreateEx(void* root);
__declspec(dllexport) bool WINAPI axGetCode(int kind, char* user, char* code, int type, CPoint pt);
__declspec(dllexport) bool WINAPI axGetName(int kind, char* code, char* name, int* type);
__declspec(dllexport) void WINAPI axGetMiniJisu(char* link);
__declspec(dllexport) char* WINAPI getArray(int type);
__declspec(dllexport) const char* WINAPI getName(const char* sCode);
__declspec(dllexport) const char* WINAPI getCode(const char* sName);
#ifdef	__cplusplus
}
#endif


