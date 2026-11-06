#include "pch.h"
#include "TestDlg.h"
#include "CBtnPanel.h"

namespace
{
	const int SCROLL_LINE = 30;	// pixels per line / wheel notch
}

CBtnPanel::CBtnPanel()
	: CDialogEx(IDD)
{
}

BEGIN_MESSAGE_MAP(CBtnPanel, CDialogEx)
	ON_WM_SIZE()
	ON_WM_VSCROLL()
	ON_WM_MOUSEWHEEL()
	ON_BN_CLICKED(IDC_BTN_ACC_CHECK, &CBtnPanel::OnBnClickedBtnAccCheck)
	ON_BN_CLICKED(IDC_BTN_NEWTEST, &CBtnPanel::OnBnClickedBtnNewtest)
END_MESSAGE_MAP()

BOOL CBtnPanel::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Content height = lowest child bottom + bottom margin
	for (CWnd* pChild = GetWindow(GW_CHILD); pChild; pChild = pChild->GetNextWindow())
	{
		CRect rc;
		pChild->GetWindowRect(rc);
		ScreenToClient(rc);
		m_nContentHeight = max(m_nContentHeight, (int)rc.bottom);
	}
	CRect rcMargin(0, 0, 0, 7);
	MapDialogRect(rcMargin);
	m_nContentHeight += rcMargin.bottom;

	UpdateScrollInfo();
	return TRUE;
}

// Own message map first; otherwise forward control notifications to CTestDlgDlg
BOOL CBtnPanel::OnCommand(WPARAM wParam, LPARAM lParam)
{
	if (CDialogEx::OnCommand(wParam, lParam))
		return TRUE;

	if (lParam != 0 && GetParent())
	{
		GetParent()->SendMessage(WM_COMMAND, wParam, lParam);
		return TRUE;
	}
	return FALSE;
}

void CBtnPanel::UpdateScrollInfo()
{
	CRect rc;
	GetClientRect(rc);

	SCROLLINFO si{};
	si.cbSize = sizeof(si);
	si.fMask = SIF_RANGE | SIF_PAGE | SIF_DISABLENOSCROLL;
	si.nMin = 0;
	si.nMax = max(0, m_nContentHeight - 1);
	si.nPage = rc.Height();
	SetScrollInfo(SB_VERT, &si);

	ScrollTo(m_nScrollPos);	// clamp after resize
}

void CBtnPanel::ScrollTo(int nPos)
{
	CRect rc;
	GetClientRect(rc);

	const int nMaxPos = max(0, m_nContentHeight - rc.Height());
	nPos = max(0, min(nPos, nMaxPos));
	if (nPos != m_nScrollPos)
	{
		ScrollWindow(0, m_nScrollPos - nPos);	// moves child buttons too
		m_nScrollPos = nPos;
	}
	SetScrollPos(SB_VERT, m_nScrollPos);
}

void CBtnPanel::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	if (m_nContentHeight > 0)	// skip WM_SIZE sent before OnInitDialog
		UpdateScrollInfo();
}

void CBtnPanel::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	CRect rc;
	GetClientRect(rc);

	int nNew = m_nScrollPos;
	switch (nSBCode)
	{
	case SB_LINEUP:		nNew -= SCROLL_LINE;	break;
	case SB_LINEDOWN:	nNew += SCROLL_LINE;	break;
	case SB_PAGEUP:		nNew -= rc.Height();	break;
	case SB_PAGEDOWN:	nNew += rc.Height();	break;
	case SB_TOP:		nNew = 0;				break;
	case SB_BOTTOM:		nNew = m_nContentHeight;	break;
	case SB_THUMBTRACK:
	case SB_THUMBPOSITION:
	{
		SCROLLINFO si{};
		si.cbSize = sizeof(si);
		si.fMask = SIF_TRACKPOS;	// nPos is only 16-bit
		GetScrollInfo(SB_VERT, &si);
		nNew = si.nTrackPos;
		break;
	}
	default:
		break;
	}
	ScrollTo(nNew);

	CDialogEx::OnVScroll(nSBCode, nPos, pScrollBar);
}

BOOL CBtnPanel::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	ScrollTo(m_nScrollPos - zDelta * 3 * SCROLL_LINE / WHEEL_DELTA);
	return TRUE;
}


enum AcctTypeBit
{
	ACCT_OWNER = 0x01,	// 1: owner,    0: agent
	ACCT_WRAP = 0x02,	// 1: wrap,     0: normal
	ACCT_CORP = 0x04,	// 1: corporate, 0: personal
};

int     ParseAcctType(LPCTSTR sVal);	// -1 if invalid
CString GetAcctTypeText(LPCTSTR sVal);	// "" if invalid

inline bool IsAcctCorp(int nType) { return nType >= 0 && (nType & ACCT_CORP) != 0; }
inline bool IsAcctWrap(int nType) { return nType >= 0 && (nType & ACCT_WRAP) != 0; }
inline bool IsAcctOwner(int nType) { return nType >= 0 && (nType & ACCT_OWNER) != 0; }

int ParseAcctType(LPCTSTR sVal)
{
	CString str(sVal);
	str.Trim();
	if (str.GetLength() != 1 || str[0] < _T('0') || str[0] > _T('7'))
		return -1;

	return str[0] - _T('0');
}

// Flag string -> description text, e.g. "5" -> corporate/normal/owner
CString GetAcctTypeText(LPCTSTR sVal)
{
	int nType = ParseAcctType(sVal);
	if (nType < 0)
		return _T("");

	CString sText;
	sText.Format(_T("%s %s %s"),
		IsAcctCorp(nType) ? _T("법인") : _T("개인"),
		IsAcctWrap(nType) ? _T("wrap") : _T("일반"),
		IsAcctOwner(nType) ? _T("본인") : _T("대리인"));
	return sText;
}

/*
0 000  개인 일반 대리인
1 001  개인 일반 본인
2 010  개인 wrap 대리인
3 011  개인 wrap 본인
4 100  법인 일반 대리인
5 101 법인 일반 본인
6 110 법인 wrap 대리인
7 111 법인 wrap 본인
*/
void CBtnPanel::OnBnClickedBtnAccCheck()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	AfxMessageBox(GetAcctTypeText("4"));
}


void CBtnPanel::OnBnClickedBtnNewtest()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}
