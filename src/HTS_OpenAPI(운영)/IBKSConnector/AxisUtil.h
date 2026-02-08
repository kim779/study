#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

extern LPCSTR OCX[];
bool RegisterOCX(CString path);
bool UnregisterOCX(CString path);
bool SetAxisRegistry(CString key, CString root);
CString GetGlbIP(CString usid, CString uspwd, CString uscpw);
CString GetMacAddress(CString ip);
CString GetLocalIP(CString ip, int port);
CString GetRootPath();
