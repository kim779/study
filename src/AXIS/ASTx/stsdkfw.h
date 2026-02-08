#ifndef __STSDKFW_H_
#define __STSDKFW_H_


/*****************************************************************************
 *
 *	FW Interface
 *
 ***************************************************************************/

class IAstxFw
{
public:
	virtual DWORD _stdcall Initialize() = 0;
	virtual DWORD _stdcall Uninitialize() = 0;
	virtual DWORD _stdcall StartA(LPCSTR szCustomerID, LPCSTR szCustomPolicyURL) = 0;
	virtual DWORD _stdcall StartW(LPCWSTR szCustomerID, LPCWSTR szCustomPolicyURL) = 0;
	virtual DWORD _stdcall StopA(LPCSTR szCustomerID) = 0;
	virtual DWORD _stdcall StopW(LPCWSTR szCustomerID) = 0;
	virtual DWORD  _stdcall IsRunning() = 0;

};

#endif // __STSDKFW_H_
