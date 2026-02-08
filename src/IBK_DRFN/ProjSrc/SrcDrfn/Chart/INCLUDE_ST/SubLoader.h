// SubLoader.h: interface for the CSubLoader class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SUBLOADER_H__486E5E91_1EAD_47DA_981C_1D008A6856AB__INCLUDED_)
#define AFX_SUBLOADER_H__486E5E91_1EAD_47DA_981C_1D008A6856AB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

typedef  double (WINAPI* _DLL_SubInFuncST)(long lSTActor,long lIndex, BOOL bTemp0 , BOOL bTemp1 ,BOOL bTemp2 , BOOL bTemp3 ,BOOL bTemp4 , BOOL ,BOOL bTemp6 , BOOL bTemp7 ,BOOL bTemp8 , BOOL bTemp9 );
typedef  CString (WINAPI* _DLL_GetInputs)(long lIndex);

class CSubLoader  
{
public:
	CSubLoader();
	virtual ~CSubLoader();
	BOOL LoadDLL(LPCTSTR lpSTFileName);
	BOOL UnloadDLL();
	double SubInFuncST(long lSTActor, long lIndex, BOOL bTemp0 , BOOL bTemp1 ,BOOL bTemp2 , BOOL bTemp3 ,BOOL bTemp4 , BOOL bTemp5 ,BOOL bTemp6 , BOOL bTemp7 ,BOOL bTemp8 , BOOL bTemp9 );
	CString GetErrorMessage() { return m_strError;}
private:
	_DLL_SubInFuncST m_fnSubInFuncST;
	_DLL_GetInputs m_fnInputs;
	CStringArray m_arrayInputs;
	HINSTANCE m_hSTDll;
	CString m_strSTDll;
	long m_lCntOfInput;
	BOOL m_bCheckOfUseful;
	CString m_strError;
};

#endif // !defined(AFX_SUBLOADER_H__486E5E91_1EAD_47DA_981C_1D008A6856AB__INCLUDED_)
