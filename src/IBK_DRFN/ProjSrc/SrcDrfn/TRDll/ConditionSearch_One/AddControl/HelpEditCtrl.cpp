// HelpEditCtrl.cpp : implementation file


#include "stdafx.h"
#include "../Resource.h"
#include "HelpEditCtrl.h"
#include <afxadv.h>


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

///////////////////////////////////////////////////////////////////
// CHelpEditCtrl

#define ID_COPY_MENU		1

CHelpEditCtrl::CHelpEditCtrl()
{
	m_DefaultCursor = AfxGetApp()->LoadCursor(IDC_NORMAL_CURSOR);
}

CHelpEditCtrl::~CHelpEditCtrl()
{
}

BEGIN_MESSAGE_MAP(CHelpEditCtrl, CRichEditCtrl)
	//{{AFX_MSG_MAP(CHelpEditCtrl)
	ON_WM_CREATE()
	ON_WM_RBUTTONUP()
	ON_WM_INITMENUPOPUP()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

///////////////////////////////////////////////////////////////////
//  CHelpEditCtrl message handlers

BOOL CHelpEditCtrl::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	HideCaret();
  
	if(m_DefaultCursor != NULL)
		::SetCursor(m_DefaultCursor);

	if( (pMsg->message == WM_LBUTTONDBLCLK)
		||(pMsg->message == WM_LBUTTONDOWN)
		||(pMsg->message == WM_LBUTTONUP)
		||(pMsg->message == WM_RBUTTONDOWN) )
	{
		return TRUE;
	}

	return CRichEditCtrl::PreTranslateMessage(pMsg);
}

BOOL CHelpEditCtrl::OnCommand(WPARAM wParam, LPARAM lParam) 
{
	// TODO: Add your specialized code here and/or call the base class
	if((UINT)wParam == ID_COPY_MENU)
 		CopyTextToClipboard();

	return CRichEditCtrl::OnCommand(wParam, lParam);
}

int CHelpEditCtrl::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CRichEditCtrl::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// TODO: Add your specialized creation code here
	m_Menu.CreatePopupMenu();
	m_Menu.AppendMenu(MF_STRING, ID_COPY_MENU, (LPCTSTR)"내용복사");
	SetMenu(&m_Menu);

	return 0;
}

void CHelpEditCtrl::OnInitMenuPopup(CMenu* pPopupMenu, UINT nIndex, BOOL bSysMenu) 
{
	CRichEditCtrl::OnInitMenuPopup(pPopupMenu, nIndex, bSysMenu);
	
	::SetCursor(m_DefaultCursor);
}

void CHelpEditCtrl::OnRButtonUp(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	CString strData = "";
	GetWindowText(strData);
	
	//설명창에 아무런 Text가 없는 경우 PopupMenu를 띄우지 않는다.
	if(strData == "")
		return;

	ClientToScreen(&point);
	m_Menu.TrackPopupMenu(TPM_LEFTALIGN |TPM_RIGHTBUTTON, 
						  point.x, point.y, 
						  this);

	CRichEditCtrl::OnRButtonUp(nFlags, point);
}

BOOL CHelpEditCtrl::CopyTextToClipboard()
{
	CString strData = ""; 
	CString strExplainData;
	GetWindowText(strExplainData);

	int nTextLength = strExplainData.GetLength();

	int nTextSpace = 50;
	int nStartPos = 0;
	CString strGetText;

	while (nStartPos < nTextLength)
	{
		SetSel(nStartPos, nStartPos + nTextSpace);
		strGetText = GetSelText();

		int nFindPos = strGetText.Find('\n');
		if (nFindPos > 0)
		{
			strData += strGetText.Mid(0, nFindPos) + "\r\n";
			nStartPos = nStartPos + nFindPos + 1;
		}
		else
		{
			strData += strGetText + "\r\n";
			nStartPos = nStartPos + nTextSpace;
		}
	}

	SetSel(nStartPos, nTextLength);
	strData += GetSelText() + "\r\n";

	//Insert Data in Clipboard
	if(OpenClipboard() == TRUE)
	{
		char* buffer;
		HGLOBAL clipbuffer;
		
		EmptyClipboard();
		
		clipbuffer = GlobalAlloc(GMEM_DDESHARE, strlen(strData) + 1);
		buffer = (char*)GlobalLock(clipbuffer);
		strcpy(buffer, LPCSTR(strData));
		GlobalUnlock(clipbuffer);
		
		SetClipboardData(CF_TEXT, clipbuffer);
		CloseClipboard();
	}

	SendMessage(WM_KILLFOCUS, 0, 0);

	return TRUE;
}

BOOL CHelpEditCtrl::CopyText(LPSTR lpszBuffer, HWND hWnd)
{
	HGLOBAL hGlobal;		//Global memory handle
	LPSTR lpszData;		//Pointer to clipboard data
	unsigned long nSize;	//Size of clipboard data
	
	if(OpenClipboard() == TRUE)
	{
		EmptyClipboard();
		
		nSize = lstrlen(lpszBuffer);
		
		// Allocate the memory for the string.
		hGlobal = GlobalAlloc(GMEM_ZEROINIT, nSize+1);
		
		if(hGlobal == NULL) 
			return FALSE;
		
		lpszData = (LPSTR)GlobalLock(hGlobal);
		
		for(UINT i=0; i<nSize + 1; ++i)
		{
			*(lpszData + i) = *(lpszBuffer + i);
		}
		
		GlobalUnlock(hGlobal);
		SetClipboardData(CF_TEXT, hGlobal);
		CloseClipboard();
	}
	
	return TRUE;
}

void CHelpEditCtrl::CopyTextToClipboard(CString strData)
{
	int nSize = strData.GetLength();
	char* buf = new char[nSize + 1];
	strcpy(buf, strData);

	CopyText(buf);

	delete[] buf;
}
