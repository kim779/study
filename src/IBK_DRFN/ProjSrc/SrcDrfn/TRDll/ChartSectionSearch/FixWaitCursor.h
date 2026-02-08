// FixWaitCursor.h: interface for the CFixWaitCursor class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_FIXWAITCURSOR_H__D55C15A7_FC04_4D13_BDCC_F8516D9E4ADE__INCLUDED_)
#define AFX_FIXWAITCURSOR_H__D55C15A7_FC04_4D13_BDCC_F8516D9E4ADE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define	WMU_EWAIT_CURSOR		WM_USER + 3051

class CFixWaitCursor
{
public:

	CFixWaitCursor();
	CFixWaitCursor(CWnd * pParentWnd);
	virtual ~CFixWaitCursor();

public:

	int		m_nWaitCount;
	
	BOOL	m_bIntelli;

	//20080331 이문수 [1] >>
	CUIntArray	m_nRqIndexArray;
	//20080331 이문수 <<

public:

	CWnd * m_pParentWnd;//실제 커서를 제어하는 윈도우

public:

	void SetParentWnd(CWnd * pParentWnd)
	{
		m_pParentWnd = pParentWnd;
	}


	BOOL WaitCursorProc(MSG* pMsg);
	
	BOOL BWaitCursor(UINT nRqIndex, CWnd * pFocusWnd = NULL, int nTime = 120000);

	BOOL BWaitCursorIntelli(CWnd * pFocusWnd = NULL, int nTime = 120000);

	BOOL EWaitCursor(UINT nReceiveIndex, BOOL bTimerEnd = FALSE );

	BOOL EWaitCursor(BOOL bTimerEnd  = FALSE );

	void OnTimer(UINT nIDEvent);

};

#endif // !defined(AFX_FIXWAITCURSOR_H__D55C15A7_FC04_4D13_BDCC_F8516D9E4ADE__INCLUDED_)
