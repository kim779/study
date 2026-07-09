#pragma once


// CControlWnd
class CParam
{
public:
	int		key;			// screen key
	CString		name;			// control symbol name
	CRect		rect;			// control rect
	CString		fonts;			// font name
	int		point;			// font point
	int		style;			// font style
	DWORD		tRGB;			// text color index
	DWORD		pRGB;			// paint color index
	CString		options;		// properties
};

class CControlWnd : public CWnd
{
	DECLARE_DYNAMIC(CControlWnd)
	CWnd* m_pWizard;
	CParam		m_Param;
public:
	CControlWnd();
	virtual ~CControlWnd();

	virtual void OnFinalRelease();
	void	SetParam(struct _param *pParam);
protected:
	DECLARE_MESSAGE_MAP()
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()
	void SetType(LPCTSTR gubn, LPCTSTR type);

	enum 
	{
		dispidSetBalListType = 2L,
		dispidSetType = 1L
	};
	void SetBalListType(LPCTSTR balcode, LPCTSTR type);
};


