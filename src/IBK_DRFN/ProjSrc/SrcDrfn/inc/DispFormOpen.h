#if !defined(AFX_DTSPFORMOPEN_H__BY_JUNOK_LEE__20051228_57ECF7257A__INCLUDED_)
#define AFX_DTSPFORMOPEN_H__BY_JUNOK_LEE__20051228_57ECF7257A__INCLUDED_

// #include "../../_Include/DispFormData.h"

/////////////////////////////////////////////////////////////////////////////
// CDispFormOpen wrapper class

class CDispFormOpen : public COleDispatchDriver
{
public:
	CDispFormOpen();
	
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
		IIx_ReLoadMap=0,	
		IIx_OpenMap,	
		IIx_LoadMap,	
		IIx_DialogMap,	
		IIx_PopupMap,	
		IIx_CloseMap,	
		IIx_SinglePopupMap,
	};

	// 3. void InitInterfaceName(IDispatch *pDisp);
	void InitInterfaceName(IDispatch *pDisp);

	// 4.
//	COleDispatchDriver m_dispDriver;

//}} Dispid자동처리
public:
// Attributes

// Operations
	void ReLoadMap(LPCTSTR szMap, LPCTSTR szOpenData, short nDataLen);
	void OpenMap(LPCTSTR szMap, LPCTSTR szOpenData, short nDataLen);
	void LoadMap(LPCTSTR szMap, LPCTSTR szOpenData, short nDataLen);
	void DialogMap(LPCTSTR szMap, LPCTSTR szOpenData, short nDataLen);
	void PopupMap(LPCTSTR szMap, LPCTSTR szOpenData, short nDataLen);
	void CloseMap();
	void SinglePopupMap(LPCTSTR szMap, LPCTSTR szOpenData, short nDataLen);



// Implementation
};

#endif // !defined(AFX_DTSPFORMOPEN_H__BY_JUNOK_LEE__20051228_57ECF7257A__INCLUDED_)

