#if !defined(AFX_AXISWIZARD_H__6DEAD0C9_1C24_4B48_86A7_B3A8AD3D4C80__INCLUDED_)
#define AFX_AXISWIZARD_H__6DEAD0C9_1C24_4B48_86A7_B3A8AD3D4C80__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CAxisWizard : public CWnd
{
public:
	CAxisWizard();
	virtual ~CAxisWizard();
protected:
	DECLARE_DYNCREATE(CAxisWizard)

public:
	CLSID const& GetClsid()
	{
		static CLSID const clsid
		//	= { 0x7B26DE02, 0xE5EA, 0x4ED5, { 0xAA, 0x20, 0xFB, 0x9, 0x40, 0x62, 0x36, 0x6C } };
			= { 0xFE2FC2D9, 0x055B, 0x4095, { 0x88, 0x06, 0xC9, 0x83, 0x67, 0x18, 0x78, 0x68 } };
		return clsid;
	}
	virtual BOOL Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle,
		const RECT& rect, CWnd* pParentWnd, UINT nID, 
		CCreateContext* pContext = NULL)
	{ 
		int iret = CreateControl(GetClsid(), lpszWindowName, dwStyle, rect, pParentWnd, nID);
		return iret;
	}
	
    BOOL Create(LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, 
		UINT nID, CFile* pPersist = NULL, BOOL bStorage = FALSE,
		BSTR bstrLicKey = NULL)
	{ 
		return CreateControl(GetClsid(), lpszWindowName, dwStyle, rect, pParentWnd, nID,
			pPersist, bStorage, bstrLicKey); 
	}
	
	BOOL RunAxis(long mode, long pBytes, long nBytes)
	{
		BOOL result;
		static BYTE parms[] = VTS_I4 VTS_I4 VTS_I4 ;
		InvokeHelper(0x1, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, mode, pBytes, nBytes);
		return result;
	}
	BOOL axTR(long pBytes, long nBytes)
	{
		BOOL result;
		static BYTE parms[] = VTS_I4 VTS_I4 ;
		InvokeHelper(0x2, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, pBytes, nBytes);
		return result;
	}
	long axAttach(long view, long type, long key)
	{
		long result;
		static BYTE parms[] = VTS_I4 VTS_I4 VTS_I4 ;
		InvokeHelper(0x3, DISPATCH_METHOD, VT_I4, (void*)&result, parms, view, type, key);
		return result;
	}
	void axDetach(long key)
	{
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x4, DISPATCH_METHOD, VT_EMPTY, NULL, parms, key);
	}
	BOOL axWorkShop(long key, LPCTSTR maps, long size, long only)
	{
		BOOL result;
		static BYTE parms[] = VTS_I4 VTS_BSTR VTS_I4 VTS_I4 ;
		InvokeHelper(0x5, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, key, maps, size, only);
		return result;
	}
	long axWorkName(long key)
	{
		long result;
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x6, DISPATCH_METHOD, VT_I4, (void*)&result, parms, key);
		return result;
	}
	long axWorkView(LPCTSTR maps, long size)
	{
		long result;
		static BYTE parms[] = VTS_BSTR VTS_I4 ;
		InvokeHelper(0x7, DISPATCH_METHOD, VT_I4, (void*)&result, parms, maps, &size);
		return result;
	}
	long axWizard(long kind, long variant)
	{
		long result;
		static BYTE parms[] = VTS_I4 VTS_I4 ;
		InvokeHelper(0x8, DISPATCH_METHOD, VT_I4, (void*)&result, parms, kind, variant);
		return result;
	}
	long axInvoke(long wParam, long lParam)
	{
		long result;
		static BYTE parms[] = VTS_I4 VTS_I4 ;
		InvokeHelper(0x9, DISPATCH_METHOD, VT_I4, (void*)&result, parms, wParam, lParam);
		return result;
	}

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAxisWizard)
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CAxisWizard)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_AXISWIZARD_H__6DEAD0C9_1C24_4B48_86A7_B3A8AD3D4C80__INCLUDED_)
