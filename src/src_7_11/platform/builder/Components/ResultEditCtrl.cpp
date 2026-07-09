// ResultEditCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "ResultEditCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CResultEditCtrl

CResultEditCtrl::CResultEditCtrl()
{
	m_pRMenu = NULL;
	m_pRMenu = new CMenu;
	m_pRMenu->CreatePopupMenu();
	m_pRMenu->AppendMenu(MF_STRING | MF_ENABLED, 1, "Copy");
	m_pRMenu->AppendMenu(MF_SEPARATOR);
	m_pRMenu->AppendMenu(MF_STRING | MF_ENABLED, 2, "Clear");
	m_pRMenu->AppendMenu(MF_SEPARATOR);
	m_pRMenu->AppendMenu(MF_STRING | MF_ENABLED, 4, "RTM Data");
	m_pRMenu->AppendMenu(MF_STRING | MF_ENABLED, 3, "SND/RCV Data");
	m_pRMenu->AppendMenu(MF_STRING | MF_ENABLED, 5, "BIN Mode");
	
	m_bSNDRCV = false;
	m_bRTM    = false;
	m_bMode   = true;
	m_hcursor = AfxGetApp()->LoadStandardCursor(IDC_IBEAM);
}

CResultEditCtrl::~CResultEditCtrl()
{
	if (m_pRMenu)
		delete m_pRMenu;
}


BEGIN_MESSAGE_MAP(CResultEditCtrl, CRichEditCtrl)
	//{{AFX_MSG_MAP(CResultEditCtrl)
	ON_WM_RBUTTONDOWN()
	ON_WM_SETCURSOR()
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_USER, OnMessage)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CResultEditCtrl message handlers

void CResultEditCtrl::OnRButtonDown(UINT nFlags, CPoint point) 
{
	int idx = -1;
	long start, end;
	
	ClientToScreen(&point);
	GetSel(start, end);

	if (start == end)
		m_pRMenu->EnableMenuItem(0, MF_BYPOSITION | MF_DISABLED | MF_GRAYED);
	else
		m_pRMenu->EnableMenuItem(0, MF_BYPOSITION | MF_ENABLED);

	if (m_bSNDRCV)
		m_pRMenu->CheckMenuItem(3, MF_CHECKED);
	else
		m_pRMenu->CheckMenuItem(3, MF_UNCHECKED);
	
	if (m_bRTM)
		m_pRMenu->CheckMenuItem(4, MF_CHECKED);
	else
		m_pRMenu->CheckMenuItem(4, MF_UNCHECKED);

	if (m_bMode)
		m_pRMenu->ModifyMenu(5, MF_BYCOMMAND, 5, "Change TEXT Mode");
	else
		m_pRMenu->ModifyMenu(5, MF_BYCOMMAND, 5, "Change BIN Mode");

	m_hcursor = AfxGetApp()->LoadStandardCursor(IDC_ARROW);
	SetCursor(m_hcursor);
	
	idx = m_pRMenu->TrackPopupMenu(TPM_LEFTALIGN | TPM_BOTTOMALIGN | TPM_RETURNCMD , point.x, point.y, this);
	switch(idx)
	{
	case 1:	// copy
		Copy();			break;
	case 2:	// clear
		Clear();		break;
	case 3:	// SND/RCV
		m_bSNDRCV = !m_bSNDRCV;	break;
	case 4:	// RTM
		m_bRTM = !m_bRTM;	break;
	case 5:	// Mode
		m_bMode = !m_bMode;	break;
	}
	m_hcursor = AfxGetApp()->LoadStandardCursor(IDC_IBEAM);
	
	CRichEditCtrl::OnRButtonDown(nFlags, point);
}

BOOL CResultEditCtrl::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message) 
{
	HCURSOR hcursor;
	if (pWnd->m_hWnd == m_hWnd)
	{
		switch(nHitTest)
		{
		case HTCLIENT:
			m_hcursor = AfxGetApp()->LoadStandardCursor(IDC_IBEAM);
			hcursor = SetCursor(m_hcursor);
			break;
		case HTVSCROLL:
		case HTHSCROLL:
		case HTMENU:
			m_hcursor = AfxGetApp()->LoadStandardCursor(IDC_ARROW);
			hcursor = SetCursor(m_hcursor);
			break;
		}
	}

	return TRUE;	
}


BOOL CResultEditCtrl::PreTranslateMessage(MSG* pMsg) 
{
	if (IsDialogMessage(pMsg))
	{
		if ((GetKeyState(VK_CONTROL) & 0x80) && (pMsg->wParam == 'c' || pMsg->wParam == 'C'))
		{
			long start, end;
			
			GetSel(start, end);
			
			if (start == end)
				m_pRMenu->EnableMenuItem(0, MF_BYPOSITION | MF_DISABLED | MF_GRAYED);
			else
				m_pRMenu->EnableMenuItem(0, MF_BYPOSITION | MF_ENABLED);
			
			Copy();
		}
		if (((GetKeyState(VK_CONTROL) & 0x80) && 
			pMsg->message == WM_KEYDOWN &&
			(pMsg->wParam == 'f' || pMsg->wParam == 'F')) ||
			(pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_F3))
			return CRichEditCtrl::PreTranslateMessage(pMsg);
		return TRUE;
	}
	else
		return CRichEditCtrl::PreTranslateMessage(pMsg);
}

void CResultEditCtrl::Clear()
{
	HideSelection(TRUE, FALSE);
	SetSel(0, -1);
	ReplaceSel("");
	HideSelection(FALSE, FALSE);
}

int CResultEditCtrl::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CRichEditCtrl::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	CHARFORMAT cf;
	GetDefaultCharFormat(cf);
	cf.yHeight = 180;
	SetDefaultCharFormat(cf);
	return 0;
}

long CResultEditCtrl::OnMessage(WPARAM wParam, LPARAM lParam)
{
	CHARRANGE cr;

	GetSel(cr);
	CString tmp;
	if (cr.cpMin == cr.cpMax)
	{
		HideSelection(TRUE, FALSE);
		cr.cpMin--;
		SetSel(cr);
		tmp = GetSelText();
		if (!tmp.IsEmpty())
		{
			while(isalnum(tmp.GetAt(0)))
			{
				cr.cpMin--;
				if (cr.cpMin < 0)
				{
					cr.cpMin = 0;
					break;
				}
				SetSel(cr);
				tmp = GetSelText();
			}
		}
		else
			cr.cpMin = 0;

		cr.cpMax++;
		SetSel(cr);
		tmp = GetSelText();
		while(isalnum(tmp.GetAt(tmp.GetLength() - 1)))
		{
			cr.cpMax++;
			SetSel(cr);
			tmp = GetSelText();
			if (tmp.GetLength() < cr.cpMax - cr.cpMin)
				break;
		}
		if (cr.cpMin > 0)
			cr.cpMin++;
		cr.cpMax--;
		HideSelection(FALSE, FALSE);
		SetSel(cr);
		m_sRtn = GetSelText();
	}
	else
		m_sRtn = GetSelText();
	return (long)m_sRtn.operator LPCTSTR();
}
