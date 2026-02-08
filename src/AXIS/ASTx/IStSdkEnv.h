#ifndef __ISTSDKENV_H_
#define __ISTSDKENV_H_



/*****************************************************************************
 *
 *	Env Interface
 *
 ***************************************************************************/


class IStSdkEnv
{
public:

	virtual DWORD SetOptionA(DWORD dwEnvOption, LPCSTR lpszValue) = 0;
	virtual DWORD SetOptionW(DWORD dwEnvOption, LPCWSTR lpszValue) = 0;

};

// HTS에 노출되는 인터페이스는 아님
class IStSdkEnv2
{
public:

	virtual DWORD SetStrOptionA(DWORD dwEnvOption, LPCSTR lpszValue) = 0;
	virtual DWORD GetStrOptionA(DWORD dwEnvOption, LPSTR lpszValue, DWORD dwLen) = 0;
	virtual DWORD SetDWORDOption(DWORD dwEnvOption, DWORD dwValue) = 0;
	virtual DWORD GetDWORDOption(DWORD dwEnvOption) = 0;
	virtual DWORD SetVPOption(DWORD dwEnvOption, void * pValue) = 0;
	virtual void* GetVPOption(DWORD dwEnvOption) = 0;

};




#endif // __ISTSDKENV_H_
