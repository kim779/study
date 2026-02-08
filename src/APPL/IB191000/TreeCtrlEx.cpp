// TreeWnd.cpp : implementation file
//

#include "stdafx.h"
#include "IB202200.h"
#include "TreeWnd.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTreeWnd

CTreeWnd::CTreeWnd(CWnd* pMainWnd)
{
	m_pMainWnd = pMainWnd;
	m_hCheck = NULL;
	m_rcCheck.SetRectEmpty();	
}

CTreeWnd::~CTreeWnd()
{
}


BEGIN_MESSAGE_MAP(CTreeWnd, CTreeCtrl)
	//{{AFX_MSG_MAP(CTreeWnd)
	ON_WM_LBUTTONDOWN()
	ON_NOTIFY_REFLECT(NM_CLICK, OnClick)
	ON_WM_MOUSEMOVE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTreeWnd message handlers
HTREEITEM CTreeWnd::AddItem(CString text, int param, HTREEITEM hAt, CString szOpt)
{
	if (text.IsEmpty()) return NULL;
	CStringArray	arItem;
	CString	str, szItem;
	int	ncnt = 0;
	
	str.Format("%s", text);
	while (1)
	{
		szItem = IH::Parser(str, "\t");
		arItem.Add(szItem);
		if (str.IsEmpty())
			break;
	}
	
	ncnt = arItem.GetSize();

	if (ncnt > 1)
	{
		HTREEITEM	hPos = hAt;
		HTREEITEM	hInput = NULL;

		for ( int ii = 0 ; ii < ncnt ; ii++ )
		{
			str = arItem.GetAt(ii);
			if (ii == (ncnt - 1))
			{
				text = str;
				break;
			}
			hInput = FindChild(hPos, str);
			if (hInput)
				hPos = hInput;
			else
				hPos = InsertItem(str, hPos);
		}
		hAt = hPos;
		if (szOpt.CompareNoCase("PREV") == 0 || szOpt.CompareNoCase("NEXT") == 0)
			szOpt = "LAST";
	}
	

	HTREEITEM	hItem = NULL;
	
	if (hAt == NULL || hAt == 0)
	{
		hItem = InsertItem(text);
		this->SetItemData(hItem, (DWORD)param);
	}
	else
	{
		if (szOpt.CompareNoCase("FIRST") == 0)
		{
			hItem = InsertItem(text, hAt, TVI_FIRST);
		}
		else if (szOpt.CompareNoCase("SORT") == 0)
		{
			hItem = InsertItem(text, hAt, TVI_SORT);
		}
		else if (szOpt.CompareNoCase("LAST") == 0)
		{
			hItem = InsertItem(text, hAt, TVI_LAST);
		}
		else if (szOpt.CompareNoCase("PREV") == 0)
		{
			HTREEITEM	hPrev = GetNextItem(hAt, TVGN_PREVIOUS);
			HTREEITEM	hParent = GetParentItem(hAt);
			if (hPrev == NULL)
				hItem = InsertItem(text, hParent, TVI_FIRST);	
			else
				hItem = InsertItem(text, hParent, hPrev);
		}
		else
		{
			HTREEITEM	hParent = GetParentItem(hAt);
			hItem = InsertItem(text, hParent, hAt);
		}

		SetItemData(hItem, param);
	}

	return hItem;
}

int CTreeWnd::GetChildCount(HTREEITEM hItem)
{
	if (!this->ItemHasChildren(hItem))
		return 0;

	HTREEITEM	hChild = GetChildItem(hItem);
	int		ncnt = 0;

	while (hChild)
	{
		ncnt++;
		hChild = GetNextItem(hChild, TVGN_NEXT);
	}

	return ncnt;
}

CString CTreeWnd::CutCount(CString text)
{
	int	nPos = text.Find("(");
	if (nPos >= 0)
	{
		text = text.Left(nPos);
		text.TrimRight();
	}

	return text;
}

void CTreeWnd::Expand(HTREEITEM hItem, CString szOpt)
{
	int	nOpt = 0;
	if (szOpt.CompareNoCase("Open") == 0)
		nOpt = TVE_EXPAND;
	else
		nOpt = TVE_COLLAPSE;

	Expand(hItem, nOpt);
}

BOOL CTreeWnd::IsFolder(HTREEITEM hItem)
{
	return this->ItemHasChildren(hItem);
}

BOOL CTreeWnd::IsItem(HTREEITEM hItem)
{
	return !this->ItemHasChildren(hItem);
}


void CTreeWnd::OnLButtonDown(UINT nFlags, CPoint point) 
{
	if (!(GetStyle() & TVS_CHECKBOXES))
	{
		CTreeCtrl::OnLButtonDown(nFlags, point);
		return;
	}

	HTREEITEM hItem = HitTest(point);
	CString	szItem = GetItemText(hItem);
	CRect	rect;

	GetItemRect(hItem, m_rcCheck, FALSE);
	GetItemRect(hItem, rect, TRUE);
	m_rcCheck.right = rect.left;
	m_rcCheck.left = m_rcCheck.right - m_rcCheck.Height();
	m_hCheck = hItem;
	
	CTreeCtrl::OnLButtonDown(nFlags, point);
}

void CTreeWnd::OnMouseMove(UINT nFlags, CPoint point) 
{
	if (m_hCheck)
	{
		if (!m_rcCheck.PtInRect(point))
		{
			m_hCheck = NULL;
			m_rcCheck.SetRectEmpty();
		}
	}
	CTreeCtrl::OnMouseMove(nFlags, point);
}

void CTreeWnd::OnClick(NMHDR* pNMHDR, LRESULT* pResult) 
{
	*pResult = 0;
	
	if (!m_hCheck) 
		return;

	CPoint	point;
	::GetCursorPos(&point);
	ScreenToClient(&point);

	if (m_rcCheck.PtInRect(point))
	{
		BOOL		bCheck = !GetCheck(m_hCheck);
			
		if (IsFolder(m_hCheck))
		{
		
			HTREEITEM	hChild = GetChildItem(m_hCheck);
			
			while (hChild)
			{
				SetCheck(hChild, bCheck);
				hChild = GetNextItem(hChild, TVGN_NEXT);
			}
		}
		else
		{
			HTREEITEM	hParent = GetParentItem(m_hCheck);
			HTREEITEM	hChild = GetChildItem(hParent);
			BOOL		bParent = TRUE, bTemp = FALSE;

			while (hChild)
			{
				if (hChild == m_hCheck)
					bTemp = bCheck;
				else
					bTemp = GetCheck(hChild);

				if (!bTemp)
				{
					bParent = FALSE;
					break;
				}
				hChild = GetNextItem(hChild, TVGN_NEXT);
			}

			SetCheck(hParent, bParent);
		}
	}

	m_hCheck = NULL;
	m_rcCheck.SetRectEmpty();	
}

HTREEITEM CTreeWnd::FindChild(HTREEITEM hItem, CString text)
{
	HTREEITEM	hRet = NULL;
	HTREEITEM	hChild = GetChildItem(hItem);
	CString		str = _T("");

	while (hChild)
	{
		str = GetItemText(hChild);

		if (str.CompareNoCase(text) == 0)
		{
			hRet = hChild;
			break;
		}
		hChild = GetNextItem(hChild, TVGN_NEXT);
	}



	return hRet;
}