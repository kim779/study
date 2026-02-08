#if !defined(AFX_CODEOCX_H__BY_JUNOK_LEE_451B_8521_61D6DE441756__INCLUDED_)
#define AFX_CODEOCX_H__BY_JUNOK_LEE_451B_8521_61D6DE441756__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//#include "./CodeOcx.h"

/////////////////////////////////////////////////////////////////////////////
// CCodeOcx wrapper class

class CDispCodeOcx : public COleDispatchDriver
{
public:
	CDispCodeOcx();
	~CDispCodeOcx();
	
//{{ Dispid 자동처리
public:
	LPDISPATCH m_pdispatch;

	// 1. 변수선언. m_pdID, m_osInterfaceName[]
	DISPID*			m_pdID;
	static OLECHAR *	m_osInterfaceName[];

	// 2. enum 추가, 필요한 것만 리스트업.
	typedef enum {
		// Variable

		// Method
		IIx_GetBase,
		IIx_SetInfo,
		IIx_ShowCodeTable,
		IIx_ShowModeCodeTable,
		IIx_SetDataString,
		IIx_InitData,
		IIx_GetCodeInfo,
		IIx_SetPosition,
		IIx_GetMaxHangSaGa,
		IIx_ShowCodeTableWithPosition,
		IIx_RemoveAllListData,
		IIx_InsertHistory,
		IIx_GetHistoryList,
		IIx_GetGlobalDataTypeList,
		IIx_GetCodePointer,
		IIx_GetBaseCodeTable,
		IIx_GetRscMng,
	};

	// 3. void InitInterfaceName(IDispatch *pDisp);
	void InitInterfaceName(IDispatch *pDisp);

	// 4.
//	COleDispatchDriver m_dispDriver;

//}} Dispid자동처리
public:
// Attributes

// Operations
public:
	long GetBaseCodeTable();
	void SetInfo(long _dwInfo);
	long ShowCodeTable(LPCTSTR _szCodeKey, const VARIANT& _hWnd);
	long ShowModeCodeTable(LPCTSTR _szCodeKey, const VARIANT& _hWnd);
	void SetDataString(short nKey, LPCTSTR szData);
	BOOL InitData(LPCTSTR szRootPath);
	CString GetCodeInfo(LPCTSTR _szCodeKey, short _nFlag, LPCTSTR _szKey);
	void SetPosition(LPCTSTR szKey, short nX, short nY);
	CString GetMaxHangSaGa();
	long ShowCodeTableWithPosition(LPCTSTR _szCodeKey, const VARIANT& _hWnd, short nX, short nY);
	BOOL RemoveAllListData();
	BOOL InsertHistory(short nMaxCount, long nType, LPCTSTR Code, LPCTSTR CodeName);
	CString GetHistoryList(short nMaxCount, long nType);
	long GetGlobalDataTypeList();
	long GetCodePointer(LPCTSTR szKey);
	void GetRscMng(long lKey);

// Implementation
};

/*
class CCodeOcx : public CWnd
{
//{{ Dispid 자동처리
public:
	LPDISPATCH m_pdispatch;

	// 1. 변수선언. m_pdID, m_osInterfaceName[]
	DISPID*			m_pdID;
	static OLECHAR *	m_osInterfaceName[];

	// 2. enum 추가, 필요한 것만 리스트업.
	typedef enum {
		// Variable
//		IIx_Rows=0,
			
		// Method
		IIx_GetBaseCodeTable,
	};

	// 3. void InitInterfaceName(IDispatch *pDisp);
	void InitInterfaceName(IDispatch *pDisp);

	// 4.
//	COleDispatchDriver m_dispDriver;

//}} Dispid자동처리

protected:
	DECLARE_DYNCREATE(CCodeOcx)
public:
	CLSID const& GetClsid()
	{
		// 사이트에 따라서 값이 달라진다.
		static CLSID const clsid
			= { 0xec0b67c2, 0xa751, 0x47b0, { 0x88, 0x3f, 0x77, 0x27, 0xd, 0x82, 0xc9, 0xc } };
		return clsid;
	}
	virtual BOOL Create(LPCTSTR lpszClassName,
		LPCTSTR lpszWindowName, DWORD dwStyle,
		const RECT& rect,
		CWnd* pParentWnd, UINT nID,
		CCreateContext* pContext = NULL)
	{ return CreateControl(GetClsid(), lpszWindowName, dwStyle, rect, pParentWnd, nID); }

    BOOL Create(LPCTSTR lpszWindowName, DWORD dwStyle,
		const RECT& rect, CWnd* pParentWnd, UINT nID,
		CFile* pPersist = NULL, BOOL bStorage = FALSE,
		BSTR bstrLicKey = NULL)
	{ return CreateControl(GetClsid(), lpszWindowName, dwStyle, rect, pParentWnd, nID,
		pPersist, bStorage, bstrLicKey); }

// Attributes
public:

// Operations
public:
	long GetBaseCodeTable();
	void SetInfo(long _dwInfo);
	long ShowCodeTable(LPCTSTR _szCodeKey, const VARIANT& _hWnd);
	long ShowModeCodeTable(LPCTSTR _szCodeKey, const VARIANT& _hWnd);
	void SetDataString(short nKey, LPCTSTR szData);
	BOOL InitData(LPCTSTR szRootPath);
	CString GetCodeInfo(LPCTSTR _szCodeKey, short _nFlag, LPCTSTR _szKey);
	void SetPosition(LPCTSTR szKey, short nX, short nY);
	CString GetMaxHangSaGa();
	long ShowCodeTableWithPosition(LPCTSTR _szCodeKey, const VARIANT& _hWnd, short nX, short nY);
	BOOL RemoveAllListData();
	BOOL InsertHistory(short nMaxCount, long nType, LPCTSTR Code, LPCTSTR CodeName);
	CString GetHistoryList(short nMaxCount, long nType);
	long GetGlobalDataTypeList();
	long GetCodePointer(LPCTSTR _szCodeKey);
};
*/

#endif // !defined(AFX_CODEOCX_H__BY_JUNOK_LEE_451B_8521_61D6DE441756__INCLUDED_)
