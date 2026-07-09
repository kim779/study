#pragma once


// CControlwnd

class CControlwnd : public CWnd
{
	DECLARE_DYNAMIC(CControlwnd)

public:
	CControlwnd();
	virtual ~CControlwnd();

	virtual void OnFinalRelease();

	CWnd* m_pWizard;
	CString Variant(int nComm, CString strData = _T(""));
	CString Parser(CString &strSrc, CString strSub = _T("\t"));
protected:
	DECLARE_MESSAGE_MAP()
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()
	BSTR GetGroupName(void);

	enum 
	{
		dispidNotToday = 3L,
		dispidGetGroupCode = 2L,
		dispidGetGroupName = 1L
	};
	BSTR GetGroupCode(LPCTSTR strgroup);
	void NotToday(LPCTSTR title);
};


