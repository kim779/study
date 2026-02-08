#ifndef __IHFORMS_0X00000001_H__
#define __IHFORMS_0X00000001_H__

class CIHBoardDialog : public CDialog
{
public:
	CIHBoardDialog(UINT nID, CWnd* pParent = NULL); 
	void ShowMessage(CString message, int nSize = -1);

	enum { IHBD_SHOWMSG = 1 };
	
	//{{AFX_DATA(CIHBoardDialog)
	enum { IDD = 0 };
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CIHBoardDialog)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CIHBoardDialog)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnDestroy();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnTimer(UINT nIDEvent);
	//}}AFX_MSG
	LONG OnManage(WPARAM wParam, LPARAM lParam);

	DECLARE_MESSAGE_MAP()
private:
	void SetMsgFont(int nSize);

	CStatic*	m_pMessage;
	CBrush		m_brMessage;
	CFont		m_ftDefault;
	CRect		m_rcOrigin;
	int		m_nGab;
	int		m_messageID;
};

#endif
