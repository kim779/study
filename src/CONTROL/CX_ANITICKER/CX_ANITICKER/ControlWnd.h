#pragma once

// ControlWnd

class ControlWnd : public CWnd
{
	DECLARE_DYNCREATE(ControlWnd)

public:
	ControlWnd();
	virtual ~ControlWnd();

// Overrides
	//{{AFX_VIRTUAL(ControlWnd)
public:
	virtual void OnFinalRelease();
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(ControlWnd)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	// Generated OLE dispatch map functions
	//{{AFX_DISPATCH(ControlWnd)
	afx_msg SHORT SetData(int gubn, BSTR sVal);
	//}}AFX_DISPATCH
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()

	enum
	{
		dispidSetData = 1L
	};
};
