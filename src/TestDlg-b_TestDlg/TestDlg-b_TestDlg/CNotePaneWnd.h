#pragma once


// CNotePaneWnd
#include "NoteDefine.h"

class CNotePaneWnd : public CWnd
{
	DECLARE_DYNAMIC(CNotePaneWnd)

public:
	CNotePaneWnd() {};
	CNotePaneWnd(bool (*callback)(int, WPARAM, LPARAM));
	virtual ~CNotePaneWnd();
	bool	(*m_sticker_call)(int, WPARAM, LPARAM);

	CString m_slog{};

	int m_totalHeight{};

	CPoint		m_StartPnt{};

	std::vector<class CNoteWnd*> m_vNoteWindows; // 자식 윈도우들
	void AddNoteWnd(CString strData = "");
	void CNotePaneWnd::ArrangNoteWnd(bool btop = true);


	int	m_wDisableBar = SB_VERT | SB_HORZ;

	void	adjustScroll(int wBar = SB_BOTH);
	long	GetScrollMax32(int nBar);
	long	GetScrollPos32(int nBar, BOOL bGetTrackPos = FALSE);
	BOOL	SetScrollPos32(int nBar, int nPos, BOOL bRedraw = TRUE);


	void WriteDataOnFunc(CString sdata);
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnPaint();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
};


