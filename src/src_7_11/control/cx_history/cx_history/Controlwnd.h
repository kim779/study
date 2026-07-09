#pragma once


// CControlwnd

class CControlwnd : public CWnd
{
	DECLARE_DYNAMIC(CControlwnd)

public:
	CControlwnd();
	virtual ~CControlwnd();

	virtual void OnFinalRelease();

	CString Parser(CString &srcstr, CString substr);
	CWnd* m_pWizard;
protected:
	DECLARE_MESSAGE_MAP()
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()
	BSTR Gethistory(void);

	enum 
	{
		dispidGethistoryCode = 2L,
		dispidGethistory = 1L
	};
	BSTR GethistoryCode(void);
};


