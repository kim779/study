// InnerFuncLoader.h: interface for the CInnerFuncLoader class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_INNERFUNCLOADER_H__AFF105BF_F4C1_4673_8CA9_429FEF6C89A6__INCLUDED_)
#define AFX_INNERFUNCLOADER_H__AFF105BF_F4C1_4673_8CA9_429FEF6C89A6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

typedef  long (WINAPI* _DLL_LoadInnerFunction)(long lIndex);
typedef  BOOL (WINAPI* _DLL_GetInputs)(long lType, long lIndex, CString &rStrFuncName, CString &rStrInputs);

class CInnerFuncLoader  
{
public:
	CInnerFuncLoader();
	virtual ~CInnerFuncLoader();
	long LoadInnerFunction(long lIndex);
	BOOL GetInputs(long lType, long lIndex, CString &rStrFuncName, CString &rStrInputs);
	BOOL LoadInnerFuncDLL(LPCTSTR lpInnerFuncDll);
	BOOL UnloadInnerFuncDLL();
	CString GetErrorMessage() { return m_strError;	}
private:	
	_DLL_LoadInnerFunction m_fnLoadInnerFunction;
	_DLL_GetInputs m_fnGetInputs;
	HINSTANCE m_hSTInnerFunctionDll;
	CString m_strSTInnerFunctionDLL;
	CString m_strError;
};

#endif // !defined(AFX_INNERFUNCLOADER_H__AFF105BF_F4C1_4673_8CA9_429FEF6C89A6__INCLUDED_)
