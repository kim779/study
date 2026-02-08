// HistoryCombo.cpp : implementation file
//

#include "stdafx.h"
#include "HistoryComboCtrl.h"

#include <comdef.h>
#include "../resource.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CHistoryComboCtrl
#define HISTORY_MAX_CNT					20

extern HINSTANCE g_hInstance;
CHistoryComboCtrl::CHistoryComboCtrl()
{
}

CHistoryComboCtrl::~CHistoryComboCtrl()
{
	
}

BEGIN_MESSAGE_MAP(CHistoryComboCtrl, CComboBox)
	//{{AFX_MSG_MAP(CHistoryComboCtrl)
	ON_WM_DESTROY()
	ON_WM_CREATE()
	//}}AFX_MSG_MAP

END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CHistoryComboCtrl message handlers
/*
CString CHistoryComboCtrl::GetAppFileRoot(void)
{
	char buf[255];
	GetModuleFileName(NULL, buf, 255); //# 실행되는 위치보기 
	
	int lastlocationofbackslash = 0;
	
	int nBufCnt = strlen(buf);
	for (int i = 0; i < nBufCnt; i++)
	{
		if (buf[i] == '\\') 
			lastlocationofbackslash = i;
	}
	
	CString strAppPath = _T("");
	for (int j = 0; j < lastlocationofbackslash; j++)
		strAppPath += buf[j];

	for (int nIndex = strAppPath.GetLength() - 1; nIndex > 0; nIndex--)
	{
		if (strAppPath[nIndex] == '\\') 
		{
			strAppPath = strAppPath.Mid(0, nIndex);
			break;
		}
	}
	
	return strAppPath;
}
*/

int CHistoryComboCtrl::Create(CWnd* pParentWnd, 
															 UINT nID,
															 LPCTSTR strHistoryFilePath,
															 LPCTSTR strTagName) 
{
	int nResult = -1;
	
	m_strFileRoot = strHistoryFilePath;
	m_strTagName = strTagName; 
	
	HINSTANCE hInstSave = AfxGetResourceHandle();
	AfxSetResourceHandle(g_hInstance);
	
	nResult = CComboBox::Create(WS_CHILD | WS_VISIBLE | WS_VSCROLL | CBS_DROPDOWN | CBS_AUTOHSCROLL,
		CRect(0, 0, 0, 0), pParentWnd, nID);
	
	m_nREdit.SubclassWindow(GetWindow(GW_CHILD)->m_hWnd);
	AfxSetResourceHandle(hInstSave);
	
	return nResult;
}


int CHistoryComboCtrl::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CComboBox::OnCreate(lpCreateStruct) == -1)
		return -1;

	m_strDefList = "";

	LoadHistoryFile();	
	
	
	return 0;
}

void CHistoryComboCtrl::OnDestroy() 
{
//	CComboBox::OnDestroy();
	
	WriteHistoryFile();

	CComboBox::OnDestroy();
}

void CHistoryComboCtrl::SetAddString(CString strText)
{
	int nIndex;
	CString strIndexText, strTemp;

	if (strText == "")
	  return;

	strTemp = strText;
	strTemp.MakeUpper();
	
  int nLength;
	int nSameIndex = -1;
  for (nIndex = 0; nIndex < GetCount(); nIndex++)
  {
		nLength = GetLBTextLen(nIndex);
		GetLBText(nIndex, strIndexText.GetBuffer(nLength) );
		strIndexText.ReleaseBuffer();
		
		strIndexText.MakeUpper();
		if (strTemp == strIndexText)
		{
			nSameIndex = nIndex;
			DeleteString(nIndex);
			break;
		}
  }
	InsertString(0, strText);
	Invalidate();

	SetCurSel(0);
}


void CHistoryComboCtrl::LoadHistoryFile()
{
	CFileFind cFind;
	char chValue[BUFSIZ];
	
	try 
	{
		memset(chValue, ' ', sizeof(chValue));
		BOOL bFileState = cFind.FindFile(m_strFileRoot);
		if(bFileState == TRUE)
		{
			CString strIndex;
			for(int nIndex = 0; nIndex < (int)HISTORY_MAX_CNT; nIndex++)
			{
				strIndex.Format("%d", nIndex);
				GetPrivateProfileString(m_strTagName, strIndex, "", chValue, BUFSIZ, m_strFileRoot);
				
				if(CString(chValue) != "" )
				{
					AddString((CString)chValue);
					m_strDefList += (CString)chValue;
					m_strDefList += "\n";
					m_arrDefList.Add((CString)chValue);				
				}
			}
			
			SetWindowText("");
		}
	}
	catch(_com_error e) { cFind.Close(); }

	cFind.Close();
}

void CHistoryComboCtrl::WriteHistoryFile()
{
	CFileFind cFind;
	
	try 
	{
		BOOL bFileState = cFind.FindFile(m_strFileRoot);
		if (bFileState == TRUE)
		{
			CString strText, strIndex;
			for (int nIndex = 0; nIndex < GetCount(); nIndex++)
			{
				strIndex.Format("%d", nIndex);
				GetLBText(nIndex, strText);
				WritePrivateProfileString(m_strTagName, strIndex, strText, m_strFileRoot);
			}
		}
	} 
	catch(_com_error e) { cFind.Close();	}

	cFind.Close();
}

CStringArray* CHistoryComboCtrl::GetComboStringArray()
{
	return &m_arrDefList;
}

CString CHistoryComboCtrl::GetComboString()
{
	return m_strDefList;
}

LRESULT CHistoryComboCtrl::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	// TODO: Add your specialized code here and/or call the base class
// 	if (message == WM_SETTEXT)
// 	{
// 		//TRACE("\n#CHistoryComboCtrl::WindowProc");
// 	}
// 	else 
	if(message==CB_GETDROPPEDSTATE)
	{
		OutputDebugString("\n#CHistoryComboCtrl::PreTranslateMessage");
	}

	return CComboBox::WindowProc(message, wParam, lParam);
}

/////////////////////////////////////////////////////////////////////////////
// CRetEdit

CRetEdit::CRetEdit()
{
}

CRetEdit::~CRetEdit()
{
}


BEGIN_MESSAGE_MAP(CRetEdit, CEdit)
//{{AFX_MSG_MAP(CRetEdit)
ON_WM_CHAR()
//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRetEdit message handlers
//@Solomon:091123SM066
void CRetEdit::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	// TODO: Add your message handler code here and/or call default
	if(nChar==VK_RETURN)
	{
		GetParent()->GetParent()->SendMessage(WM_COMMAND, IDC_BTN_TAB_SEARCH, 0);
	}
	CEdit::OnChar(nChar, nRepCnt, nFlags);
}
