// ListCtrlForFlatTab.cpp : implementation file
//
// This file is a part of the XTREME TOOLKIT PRO MFC class library.
// ?004 Codejock Software, All Rights Reserved.
//
// THIS SOURCE FILE IS THE PROPERTY OF CODEJOCK SOFTWARE AND IS NOT TO 
// BE RE-DISTRIBUTED BY ANY MEANS WHATSOEVER WITHOUT THE EXPRESSED 
// WRITTEN CONSENT OF CODEJOCK SOFTWARE.
//
// THIS SOURCE CODE CAN ONLY BE USED UNDER THE TERMS AND CONDITIONS 
// OUTLINED IN THE XTREME TOOLKIT PRO LICENSE AGREEMENT.  CODEJOCK SOFTWARE 
// GRANTS TO YOU (ONE SOFTWARE DEVELOPER) THE LIMITED RIGHT TO USE 
// THIS SOFTWARE ON A SINGLE COMPUTER.
//
// CONTACT INFORMATION:
// support@codejock.com
// http://www.codejock.com
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ListCtrlForFlatTab.h"
#include "SystemTradingWorkBar.h"
#include "../include_ST/DeclareDefine.h"
#include "../include_ST/MessageDefine.h"
#include "../Common_ST/STConfigDef.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CListCtrlForFlatTab

CListCtrlForFlatTab::CListCtrlForFlatTab()
{
}

CListCtrlForFlatTab::~CListCtrlForFlatTab()
{
}


BEGIN_MESSAGE_MAP(CListCtrlForFlatTab, CListCtrl)
	//{{AFX_MSG_MAP(CListCtrlForFlatTab)
	ON_WM_WINDOWPOSCHANGED()
	ON_NOTIFY_REFLECT(NM_DBLCLK, OnDblclk)
	ON_WM_CREATE()
	ON_WM_RBUTTONDOWN()
	ON_COMMAND(IDM_GOTO, OnGoto)
	ON_COMMAND(IDM_COPYTEXT, OnCopytext)
	ON_COMMAND(IDM_CLEAR, OnClear)
	ON_UPDATE_COMMAND_UI(IDM_GOTO, OnUpdateGoto)
	ON_UPDATE_COMMAND_UI(IDM_COPYTEXT, OnUpdateCopytext)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CListCtrlForFlatTab message handlers

void CListCtrlForFlatTab::OnWindowPosChanged(WINDOWPOS FAR* lpwndpos) 
{
	CListCtrl::OnWindowPosChanged(lpwndpos);
	
	SetColumnWidth(0, lpwndpos->cx - ::GetSystemMetrics(SM_CXHTHUMB));
	
    ShowScrollBar(SB_HORZ, FALSE);
    ModifyStyle(WS_HSCROLL, 0, SWP_DRAWFRAME);
}

void CListCtrlForFlatTab::GotoError(int nItem)
{
	DWORD dwLineNumber = GetItemData(nItem);
	if(dwLineNumber >= 0)
	{
		CString strItem = GetItemText(nItem, 0);
		int nErrorNum = atoi(strItem.Right(5));
		if(nErrorNum < 20000 || nErrorNum >= 60000)
			return;
		
		CString strFileType = strItem.Right(3).Left(1);
		
		UINT nFileType = (strFileType == _T("0")) ? FT_STRATEGY : FT_USERFUNCTION;
		
		char szPath[MAX_PATH] = {NULL, };
		::GetSTFile(szPath, strFileType, theApp.GetRootPath());
//@유진삭제::wsprintf(szPath, _T("%s%s%s.sfg"), theApp.GetRootPath(), PATH_DLLFILES, strFileType);
		::wsprintf(szPath, _T("%s%s%s.sfg"), theApp.GetUserPath(), PATH_DLLFILES2, strFileType);

		int nValue = ::GetPrivateProfileInt(BASIC, LANGTYPE, 0, szPath);
		CErrorInfomation* pErroInfo = new CErrorInfomation(nFileType, nValue);
		
		strItem.Delete(0, 1);
		AfxExtractSubString(strFileType, strItem, 0, ']');
		
		if(nFileType == FT_STRATEGY)
//@유진삭제 strItem.Format(_T("%s%s%s.sts"), theApp.GetRootPath(), PATH_SCRIPTFILES, strFileType);
			strItem.Format(_T("%s%s%s.sts"), theApp.GetUserPath(), PATH_SCRIPTFILES2, strFileType);
		else if(nFileType == FT_USERFUNCTION)
//@유진삭제 strItem.Format(_T("%s%s%s.uft"), theApp.GetRootPath(), PATH_SCRIPTFILES, strFileType);
			strItem.Format(_T("%s%s%s.uft"), theApp.GetUserPath(), PATH_SCRIPTFILES2, strFileType);

		pErroInfo->SetErrorLineInfomation2(strItem, dwLineNumber);
		
		theApp.GetFrameMainWnd()->SendMessage(RMSG_DBLCLICKOUTPUT, 0, (LPARAM)pErroInfo);
		
		delete pErroInfo;
	}
}

void CListCtrlForFlatTab::GotoTag(int nItem)
{
	CString strItem = GetItemText(nItem, 0);
	CString strName;
	if(AfxExtractSubString(strName, strItem, 0, ']') && strName.GetLength())
	{
		strName.Delete(0, 1);	// '['삭제

		DWORD dwFileType = GetItemData(nItem);
		char szScript[MAX_PATH] = {NULL, };

		if(dwFileType == FT_STRATEGY)
//@유진삭제 ::wsprintf(szScript, _T("%s%s%s.sts"), theApp.GetRootPath(), PATH_SCRIPTFILES, strName);
			::wsprintf(szScript, _T("%s%s%s.sts"), theApp.GetUserPath(), PATH_SCRIPTFILES2, strName);
		else if(dwFileType == FT_USERFUNCTION)
//@유진삭제 ::wsprintf(szScript, _T("%s%s%s.uft"), theApp.GetRootPath(), PATH_SCRIPTFILES, strName);
			::wsprintf(szScript, _T("%s%s%s.uft"), theApp.GetUserPath(), PATH_SCRIPTFILES2, strName);
		else
			return;

		if(AfxExtractSubString(strName, strItem, 1, '[') && strName.GetLength())
			strItem.Delete(0, strName.GetLength() + 2);

		if(AfxExtractSubString(strName, strItem, 0, ')') && strName.GetLength())
		{
			short nLine = 0, nCol = 0;
			::sscanf(strName, _T("%d(%d"), &nLine, &nCol);

			theApp.GetFrameMainWnd()->SendMessage(RMSG_DBLCLICKFIND, MAKEWPARAM(nLine, nCol), (LPARAM)szScript);
		}
	}
}

void CListCtrlForFlatTab::OnDblclk(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	if(pNMListView->iItem >= 0)
	{
		//CXTFlatTabCtrl* pFlatTabCtrl = (CXTFlatTabCtrl*)GetParent();
		CXTExcelTabCtrl* pFlatTabCtrl = (CXTExcelTabCtrl*)GetParent();
		
		if(pFlatTabCtrl->GetCurSel() == 0)
			GotoError(pNMListView->iItem);
		else if(pFlatTabCtrl->GetCurSel() == 1)
			GotoTag(pNMListView->iItem);
	}
	
	*pResult = 0;
}

int CListCtrlForFlatTab::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CListCtrl::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	m_mnuPopup.LoadMenu(IDR_MENU_POPUP);
	
	return 0;
}

void CListCtrlForFlatTab::OnRButtonDown(UINT nFlags, CPoint point) 
{
	CListCtrl::OnRButtonDown(nFlags, point);

	CMenu* pPopupMenu = m_mnuPopup.GetSubMenu(1);
	ClientToScreen(&point);

	CXTPCommandBars::TrackPopupMenu(pPopupMenu, TPM_LEFTALIGN|TPM_LEFTBUTTON|TPM_RIGHTBUTTON, point.x, point.y, this);
}

void CListCtrlForFlatTab::OnGoto() 
{
	//CXTFlatTabCtrl* pFlatTabCtrl = (CXTFlatTabCtrl*)GetParent();
	CXTExcelTabCtrl* pFlatTabCtrl = (CXTExcelTabCtrl*)GetParent();
	
	POSITION pos = GetFirstSelectedItemPosition();
	int nSel = GetNextSelectedItem(pos);

	if(pFlatTabCtrl->GetCurSel() == 0)
		GotoError(nSel);
	else if(pFlatTabCtrl->GetCurSel() == 1)
		GotoTag(nSel);
}

void CListCtrlForFlatTab::OnCopytext() 
{
	POSITION pos = GetFirstSelectedItemPosition();
	int nSel = GetNextSelectedItem(pos);
	
	CString strText = GetItemText(nSel, 0);

	if(strText.IsEmpty()) 
		return;

	CWaitCursor wc;

	if(OpenClipboard())
	{
		EmptyClipboard();
		HGLOBAL hData = GlobalAlloc(GMEM_MOVEABLE | GMEM_DDESHARE, strText.GetLength() + 1);
		if(hData)
		{
			LPSTR pszData = (LPSTR)::GlobalLock(hData);
			USES_CONVERSION;
			::lstrcpy(pszData, T2A((LPTSTR)(LPCTSTR)strText));
			GlobalUnlock(hData);
			SetClipboardData(CF_TEXT, hData);
		}
		CloseClipboard();
	}
}

void CListCtrlForFlatTab::OnClear() 
{
	DeleteAllItems();
}

void CListCtrlForFlatTab::OnUpdateGoto(CCmdUI* pCmdUI) 
{
	POSITION pos = GetFirstSelectedItemPosition();
	int nSel = GetNextSelectedItem(pos);
	if(nSel == LB_ERR)
	{
		pCmdUI->Enable(FALSE);
		return;
	}

	//CXTFlatTabCtrl* pFlatTabCtrl = (CXTFlatTabCtrl*)GetParent();
	CXTExcelTabCtrl* pFlatTabCtrl = (CXTExcelTabCtrl*)GetParent();

	if(pFlatTabCtrl->GetCurSel() != 0)
		return;

	CString strItem = GetItemText(nSel, 0);
	int nErrorNum = atoi(strItem.Right(5));
	if(nErrorNum < 20000 || nErrorNum >= 60000)
	{
		pCmdUI->Enable(FALSE);
		return;
	}
}

void CListCtrlForFlatTab::OnUpdateCopytext(CCmdUI* pCmdUI) 
{
	POSITION pos = GetFirstSelectedItemPosition();
	int nSel = GetNextSelectedItem(pos);
	if(nSel == LB_ERR)
	{
		pCmdUI->Enable(FALSE);
		return;
	}
}
