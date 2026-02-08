

extern "C" BSTR __declspec (dllexport) __stdcall CallSetSiteCodeFuncB(BSTR bstrSiteCode, short &nRtn);
extern "C" BSTR __declspec (dllexport) __stdcall CallExecPhonePadFuncB(BSTR bstrPath, short &nRtn);
extern "C" BSTR __declspec (dllexport) __stdcall CallGetPhonePadStateFuncB(BSTR bstrProcess, short &nRtn);
extern "C" BSTR __declspec (dllexport) __stdcall CallExitPhonePadFuncB(BSTR bstrProcess, short &nRtn);
//extern "C" void __declspec (dllexport) __stdcall CallRequestPasswdFuncA(HWND m_Handle, short nCntDigits, short nTerminated, short nSinarioNum, short nTimeout, short &nRtn);
extern "C" BSTR __declspec (dllexport) __stdcall CallRequestPasswdFuncB(short nCntDigits, short nTerminated, short nSinarioNum, short nTimeout, short &nRtn);
extern "C" BSTR __declspec (dllexport) __stdcall CallStopPasswdFuncB(short &nRtn);
