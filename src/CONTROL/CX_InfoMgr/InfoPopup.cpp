// InfoPopup.cpp : implementation file
//

#include "stdafx.h"
#include "CX_InfoMgr.h"
#include "InfoPopup.h"
#include "InfoWnd.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define	ID_LISTCTRL	1010
/////////////////////////////////////////////////////////////////////////////
// CInfoPopup

CInfoPopup::CInfoPopup(CWnd *pParent)
{
	m_pParent = pParent;
	m_pInfoList = nullptr;
	m_fDone = false;
}

CInfoPopup::~CInfoPopup()
{
}


BEGIN_MESSAGE_MAP(CInfoPopup, CWnd)
	//{{AFX_MSG_MAP(CInfoPopup)
	ON_WM_SETFOCUS()
	ON_WM_DESTROY()
	ON_WM_CREATE()
	ON_NOTIFY(NM_DBLCLK, ID_LISTCTRL, OnDblclkList)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CInfoPopup message handlers

int CInfoPopup::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	CRect	rc;

	GetClientRect(&rc);
	m_pInfoList = std::make_unique<CListCtrl>();
	m_pInfoList->Create(WS_CHILD|WS_VISIBLE|WS_BORDER|LVS_REPORT, rc, this, ID_LISTCTRL);
	m_pInfoList->SetExtendedStyle(LVS_EX_FULLROWSELECT);

	m_pInfoList->GetClientRect(rc);
	CInfoWnd* pInfoWnd = (CInfoWnd*)m_pParent;
	addColumn(m_pInfoList.get(), pInfoWnd->GetListHdrKey(), 0, rc.Width()/2);
	addColumn(m_pInfoList.get(), pInfoWnd->GetListHdrVal(), 1, rc.Width()/2);

	const	int	size = pInfoWnd->GetListCount();
	for (int ii = 0; ii < size; ii++)
	{
		addItem(m_pInfoList.get(), pInfoWnd->GetListDataKey(ii), ii, 0);
		addItem(m_pInfoList.get(), pInfoWnd->GetListDataVal(ii), ii, 1);
	}

	return 0;
}

BOOL CInfoPopup::addColumn(CListCtrl *list, LPCTSTR columns, int col, int width)
{
	LV_COLUMN lvc{};

	lvc.mask     = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT;
	lvc.fmt      = LVCFMT_LEFT;
	lvc.pszText  = (LPTSTR)columns;
	lvc.cx	     = width;
	lvc.iSubItem = -1;

	return list->InsertColumn(col, &lvc);
}

BOOL CInfoPopup::addItem(CListCtrl *list, CString items, int item, int subi)
{
	LV_ITEM lvi{};

	lvi.mask     = LVIF_TEXT;
	lvi.iItem    = item;
	lvi.iSubItem = subi;
	lvi.pszText  = (LPTSTR)items.operator LPCTSTR();

	if (!subi)
		return list->InsertItem(&lvi);
	return list->SetItem(&lvi);
}

void CInfoPopup::OnDblclkList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	_Info	info{};
	const	int	item = m_pInfoList->GetSelectionMark();

	info.key = m_pInfoList->GetItemText(item, 0);
	info.val = m_pInfoList->GetItemText(item, 1);
	((CInfoWnd*)m_pParent)->SelectData(info);

	*pResult = 0;
}


void CInfoPopup::OnSetFocus(CWnd* pOldWnd) 
{
	CWnd::OnSetFocus(pOldWnd);

	MSG	msg{};

	for (m_fDone = FALSE; !m_fDone; WaitMessage())
	{
		while (PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
		{
			if ((msg.message == WM_KILLFOCUS && msg.hwnd == this->m_hWnd) || 
				((msg.message == WM_LBUTTONDOWN || msg.message == WM_NCLBUTTONDOWN) && (msg.hwnd != this->m_hWnd && msg.hwnd != m_pInfoList->m_hWnd)) ||
				(msg.message == WM_USER && msg.hwnd == this->m_hWnd))
			{
				if (msg.message == WM_LBUTTONDOWN || msg.message == WM_NCLBUTTONDOWN)
					::PostMessage(msg.hwnd, msg.message, msg.wParam, msg.lParam);
				m_fDone = TRUE;
				break;
			}

			if (!IsDialogMessage(&msg))
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
		if (m_fDone)
			break;
	}
	//PostMessage(WM_CLOSE);
	ShowWindow(SW_HIDE);
}

void CInfoPopup::OnDestroy() 
{
	if (m_pInfoList)
		m_pInfoList.reset();

	CWnd::OnDestroy();
}