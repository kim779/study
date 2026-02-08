#ifndef __STSDKVERIFY_H_
#define __STSDKVERIFY_H_


class IAstxVerify
{
public:
	virtual DWORD _stdcall Initialize() = 0;
	virtual DWORD _stdcall Uninitialize() = 0;
	virtual DWORD _stdcall IsRunningFuncA(LPCSTR szCustomerID, DWORD dwFuncCode) = 0;
	virtual DWORD _stdcall IsRunningFuncW(LPCWSTR szCustomerID, DWORD dwFuncCode) = 0;

};

#endif // __STSDKVERIFY_H_
