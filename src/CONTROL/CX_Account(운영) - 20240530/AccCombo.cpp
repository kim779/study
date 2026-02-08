// AccCombo.cpp : implementation file
//

#include "stdafx.h"
#include "CX_Account.h"
#include "AccCombo.h"
#include "AccountCtrl.h"
#include <afxtempl.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAccCombo
CMap<HWND,HWND,WNDPROC,WNDPROC> procMap;
CMap<HWND,HWND,CAccCombo*,CAccCombo*> comboMap;
CMap<HWND,HWND,CRect,CRect> imageMap;

CAccCombo::CAccCombo(CAccountCtrl* pAccountCtrl)
	: m_bGroup(FALSE)
	, m_bGroupList(TRUE)
	, m_nGroupKey(-1)
	, m_pParentAccountCtrl(pAccountCtrl)
	, m_bSearchType(FALSE)
	, m_nOldCount(-1)
{
	m_hListBox = nullptr;
}

CAccCombo::~CAccCombo()
{
}

BEGIN_MESSAGE_MAP(CAccCombo, CComboBox)
	//{{AFX_MSG_MAP(CAccCombo)
	ON_WM_MOUSEWHEEL()
	ON_WM_SIZE()
	ON_WM_SETFOCUS()
	ON_WM_KEYDOWN()
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_CTLCOLORLISTBOX, OnCtlColorListBox)
	ON_MESSAGE(WM_DELETE_COMBODATE, OnDeleteMouseButton)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAccCombo message handlers

void CAccCombo::AddAccString(CString strAcc)
{
	m_strAccArray.Add(strAcc);
	AddString(strAcc);
}

void CAccCombo::DeleteAllAccArray()
{
	m_strAccArray.RemoveAll();
	ResetContent();
}

CString CAccCombo::GetListCtrlText(int nIndex)
{
	if (m_strAccArray.GetSize() == nIndex)
		return "";

	return m_strAccArray.GetAt(nIndex);
}

void CAccCombo::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct) 
{
	// TODO: Add your code to draw the specified item
	CString strColour;
	CDC	dcContext;
	CRect	rBlockRect, rItemRect;
	CBrush	brFrameBrush;
 	const	int	iItem = lpDrawItemStruct->itemID;

	const	COLORREF crNormal = GetSysColor(COLOR_WINDOW);
	const	COLORREF crSelected = GetSysColor(COLOR_HIGHLIGHT);
	const	COLORREF crText = GetSysColor(COLOR_WINDOWTEXT);
	
	if (!dcContext.Attach(lpDrawItemStruct->hDC))
		return;
	
	if (m_strAccArray.GetSize() == 0)
	{
		dcContext.Detach();
		return;
	}

	rBlockRect = rItemRect = CRect(lpDrawItemStruct->rcItem);
	brFrameBrush.CreateStockObject(BLACK_BRUSH);

	if (iItem != -1)
		strColour.Format("%s", m_strAccArray[iItem]);
	else
		strColour = "";

	if (lpDrawItemStruct->itemState & ODS_SELECTED) // 마우스 선택 항목 표시
	{
		rComboImageRect = rItemRect;
		rComboImageRect.top = rItemRect.top + 2;
		rComboImageRect.left = rItemRect.right - 15;

		if (m_hListBox)
			imageMap.SetAt(m_hListBox, rComboImageRect);

		dcContext.SetTextColor(	(0x00FFFFFF & ~(crText)));
		dcContext.SetBkColor(crSelected);
		dcContext.FillSolidRect(&rBlockRect, crSelected);
		
		if (!m_bSearchType)		//2012.02.27 KSJ 검색된 리스트를 보여줄때는 'X'버튼 없앤다.
		{
			if (strColour != "" && strColour != STR_ALL_DELECT && m_bGroup == FALSE) 
			{
				DrawIconEx(dcContext, rComboImageRect.left, rComboImageRect.top, AfxGetApp()->LoadIcon(IDR_LIST_DELETE),
					12,12,0,NULL,DI_NORMAL);
				//strColour = strColour.Left(30);
			}
			else  // "전체 삭제"는 X 표시 안 함
				DrawIconEx(dcContext, rComboImageRect.left, rComboImageRect.top, AfxGetApp()->LoadIcon(IDR_LIST_NULL),
				12,12,0,NULL,DI_NORMAL);
		}
	}
	else
	{
		dcContext.SetTextColor(crText);
		
		if (strColour != STR_ALL_DELECT) 
		{
			dcContext.SetBkColor(crNormal);
			dcContext.FillSolidRect(&rBlockRect, crNormal);
		}
		else // "전체 삭제"는 회색으로
		{
			if (!m_bSearchType)		//2012.02.27 KSJ 검색된 리스트를 보여줄때는 'X'버튼 없앤다.
			{
				dcContext.SetBkColor(GetSysColor(COLOR_ACTIVEBORDER));
				dcContext.FillSolidRect(&rBlockRect, GetSysColor(COLOR_ACTIVEBORDER));
			}
		}
		DrawIconEx(dcContext, rComboImageRect.left, rComboImageRect.top, AfxGetApp()->LoadIcon(IDR_LIST_NULL),
				   12,12,0,NULL,DI_NORMAL);
	}

	// draw text and block.
	if (iItem != -1)
	{
#ifdef DF_ACC_EMPHASIS
		CString stmp;
		stmp = strColour;
		stmp.Replace(_T("-"), _T(""));
		CFont* pfont{}, * poldfont{};
		if (stmp.Find(m_pParentAccountCtrl->GetEditData()) >= 0)
		{
			CString sfont{};
			int ifontsize{}, ifontstyle{};
			m_pParentAccountCtrl->GetFontInfo(sfont, ifontsize, ifontstyle);
			pfont = m_pParentAccountCtrl->getAxFont(sfont, ifontsize, 3);
			poldfont = dcContext.SelectObject(pfont);
			dcContext.SetBkMode(TRANSPARENT);
			dcContext.TextOut(rItemRect.left + 3, rItemRect.top + 2, strColour);
			dcContext.SelectObject(poldfont);
		}
		else
		{
			dcContext.SetBkMode(TRANSPARENT);
			dcContext.TextOut(rItemRect.left + 3, rItemRect.top + 2, strColour);
		}
#else
		dcContext.SetBkMode(TRANSPARENT);
		dcContext.TextOut(rItemRect.left + 3, rItemRect.top + 2, strColour);
#endif
	}
	
	dcContext.Detach();

	Invalidate();
}

extern "C" LRESULT FAR PASCAL ComboBoxListBoxProc(HWND hWnd, UINT nMsg, WPARAM wParam, LPARAM lParam)
{
	switch (nMsg)
	{
	case WM_LBUTTONUP:
		{
			CRect rc;
			GetClientRect(hWnd, rc);
			
			CPoint pt;
			pt.x = LOWORD(lParam);
			pt.y = HIWORD(lParam);

			
			CAccCombo *pCombo;
			CRect      rcImg;

			if (!comboMap.Lookup(hWnd, pCombo))
				break;
			if (!imageMap.Lookup(hWnd, rcImg))
				break;
			if (!pCombo)
				break;
			
			if (rcImg.PtInRect(pt) && pCombo->GetSafeHwnd()) // 삭제 이미지를 클릭 했을 때
			{
				pCombo->SendMessage(WM_DELETE_COMBODATE, 0 , 0);
				::PostMessage(hWnd, WM_LBUTTONDOWN, 0, lParam);
				return 0;
			}
		}
	}
	
	WNDPROC defProc;
	if (procMap.Lookup(hWnd, defProc))
		return CallWindowProc(defProc, hWnd, nMsg, wParam, lParam);

	return DefWindowProc(hWnd, nMsg, wParam, lParam);
}

void CAccCombo::DeleteAllAcc() 
{
	CString strAccString;
	const	int	nCount = m_strAccArray.GetSize();

	for (int nIndex = 0; nIndex < nCount; nIndex++)
	{
		strAccString = m_strAccArray.GetAt(nIndex);
		if (strAccString != STR_ALL_DELECT)
			m_pParentAccountCtrl->AddDeleteAccList(strAccString.Mid(0,ACC_NUMBER_LEN));
	}
}

LRESULT CAccCombo::OnDeleteMouseButton(WPARAM, LPARAM lParam) 
{
	CString nSelStr;
	const	int	nSel = GetCurSel();

	if (nSel == -1)
		return 0L;
	
	nSelStr = GetListCtrlText(nSel);
	if (nSelStr == "" || m_bGroup == TRUE)
		return 0L;

	if (nSelStr == STR_ALL_DELECT)
	{
		DeleteAllAcc();
		SendMessage(CB_SHOWDROPDOWN, FALSE);
		return 0L;
	}

	if (m_pParentAccountCtrl && GetCount())
	{
		if (!m_bSearchType)	//2012.02.27 KSJ 검색시에는 삭제되지 않게
		{
			m_pParentAccountCtrl->AddDeleteAccList(nSelStr.Mid(0,ACC_NUMBER_LEN));
			m_pParentAccountCtrl->DeleteAcc(nSel);
		}
	}

	return 0L;
}

LRESULT CAccCombo::OnCtlColorListBox(WPARAM, LPARAM lParam) 
{
	if (m_hListBox == nullptr)
	{
		HWND hWnd = reinterpret_cast<HWND>(lParam);
		
		if (hWnd != 0 && hWnd != m_hWnd)
		{
			// Save the listbox handle
			m_hListBox = hWnd;
			
			comboMap.SetAt(m_hListBox, this);
			procMap.SetAt(m_hListBox, reinterpret_cast<WNDPROC>(GetWindowLong(m_hListBox, GWL_WNDPROC)));
			imageMap.SetAt(m_hListBox, rComboImageRect);

			// Do the subclassing
			SetWindowLong(m_hListBox, GWL_WNDPROC, reinterpret_cast<long>(ComboBoxListBoxProc));
		}
	}
	
	return Default();
}

void CAccCombo::MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct) 
{
	return;
}

LRESULT CAccCombo::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	return CComboBox::WindowProc(message, wParam, lParam);
}

BOOL CAccCombo::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt) 
{
	if (zDelta > 0)
		PostMessage(WM_VSCROLL, SB_LINEUP, 0);		
	else
		PostMessage(WM_VSCROLL, SB_LINEDOWN, 0);
	
	return CComboBox::OnMouseWheel(nFlags, zDelta, pt);
}

void CAccCombo::OnSize(UINT nType, int cx, int cy) 
{
	const	int	nItemCount = m_strAccArray.GetSize();
	
	if (m_nOldCount == nItemCount)
		return;

	m_nOldCount = nItemCount;

	CComboBox::OnSize(nType, cx, cy);
}

void CAccCombo::OnSetFocus(CWnd* pOldWnd) 
{
	CComboBox::OnSetFocus(pOldWnd);
	
	// TODO: Add your message handler code here
	Invalidate();
}

void CAccCombo::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	// TODO: Add your message handler code here and/or call default
	
	if (nChar == VK_DOWN)
	{
		((CAccountCtrl*)GetParent())->PushDownKey();
		return;
	}
	else if (nChar == VK_UP)
	{
		((CAccountCtrl*)GetParent())->PushUpKey();
		return;
	}
	else if (nChar == VK_LEFT)
	{
		((CAccountCtrl*)GetParent())->PushLeftKey();
		return;
	}
	else if (nChar == VK_RIGHT)
	{
		((CAccountCtrl*)GetParent())->PushRightKey();
		return;
	}

	CComboBox::OnKeyDown(nChar, nRepCnt, nFlags);
}
