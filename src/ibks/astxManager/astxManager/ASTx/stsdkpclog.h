#ifndef __STSDKPCLOG_H_
#define __STSDKPCLOG_H_


class IAstxPcLog
{
public:
	virtual DWORD _stdcall Initialize() = 0;
	virtual DWORD _stdcall Uninitialize() = 0;
	virtual DWORD _stdcall GetPcLogA(LPCSTR szEnvType, LPCSTR szOutputValue, DWORD* pdwSize, BOOL bReload) = 0;
	virtual DWORD _stdcall GetPcLogW(LPCWSTR szEnvType, LPCWSTR szOutputValue, DWORD* pdwSize, BOOL bReload) = 0;

};

#endif // __STSDKPCLOG_H_
