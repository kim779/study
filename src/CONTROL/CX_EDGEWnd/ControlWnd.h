#pragma once


// ControlWnd


class ControlWnd : public CWnd
{
	DECLARE_DYNAMIC(ControlWnd)

public:
	ControlWnd();
	virtual ~ControlWnd();
	virtual void OnFinalRelease();

	CWnd* m_pParent;
	class CAxBrowser* m_pHtml;
	void	SetParam(struct _param* pParam);
	CParam	m_Param;

	int m_mode{};
	enum  m_mode{ BROWSER_POPUP = 0, EMBEDE_IE, INSTALL_EDGE };
private:
	CFont* m_pNFont, * m_pBFont;
	CRect	m_Htmlrc;
	CString	m_strurl;
	CString m_slog;
	bool	m_bScroll;
	int	m_scrGap;

	void Browserpopup(CString strurl);
protected:
	DECLARE_MESSAGE_MAP()
	afx_msg void Seturl2(LPCTSTR url);
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
protected:
	void Navigate(BSTR sUrl);

	enum
	{
		dispidNavigate = 2L
	};
};


