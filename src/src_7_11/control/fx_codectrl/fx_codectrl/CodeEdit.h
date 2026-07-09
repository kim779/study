#pragma once


// CCodeEdit

class CCodeEdit : public CEdit
{
	DECLARE_DYNAMIC(CCodeEdit)

public:
	CCodeEdit();
	CCodeEdit(class CControlwnd *pParent, CWnd* pTarget);
	int		m_Unit;

	bool	isHexNumeric(CString str);
	bool	isNumeric(CString str);
	void	SetUnit(int unit) { m_Unit = unit; }
	void	ListCode(CString code);
	void	ChangeHangulMode(HWND hWnd, bool bHangle = true);
	void	LoadListCode();
	void	InitThreadCode();
	void	StopThreadCode();

	CString		FindCode();
	virtual ~CCodeEdit();
public:
	bool		m_bValidCheck;
	BOOL		m_bDrag;
	CWinThread*	m_pThread;
	HANDLE		m_hEvent;
	BOOL		m_bThread;
	class CControlwnd* m_pParent;
	CWnd*		m_pTarget;
	CString		m_preCode;
	bool		m_bInitFocus, m_bKeydown;
	BOOL		m_bLButtonDown;
	
protected:
	DECLARE_MESSAGE_MAP()
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
public:
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseHWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
};


