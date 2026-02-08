// MsgWnd.cpp : implementation file
//

#include "stdafx.h"
#include "MsgWnd.h"
// #include "tspack.h"	//sizeof(HOST_MSG)을 사용하기 위해 포함

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define MESSAGE_WND_ID			2000
#define MESSAGE_BUTTON1_ID		2100
#define MESSAGE_BUTTON2_ID		2101
#define MESSAGE_BUTTON3_ID		2102
#define MESSAGE_BUTTON4_ID		2103
#define MESSAGE_COMBO_ID		2104

//#define BTN_FIRST			201
//#define BTN_PRIOR			202
//#define BTN_NEXT			203
//#define BTN_LAST			204
//#define BTN_H				24
//#define BTN_H2			23
//#define BTN_W				28
#define BTN_HEIGHT			23

#define	MESSAGE_MAX			255

/////////////////////////////////////////////////////////////////////////////
// CMessageWindow

CMessageWindow::CMessageWindow() : m_nMaxLine(5)
{	
}

CMessageWindow::~CMessageWindow()
{
	m_Font.DeleteObject();
	DestroyWindow();
}


BEGIN_MESSAGE_MAP(CMessageWindow, CWnd)
	//{{AFX_MSG_MAP(CMessageWindow)
	ON_WM_CREATE()
	ON_WM_ERASEBKGND()
	//}}AFX_MSG_MAP
	ON_CONTROL_RANGE(BN_CLICKED, MESSAGE_BUTTON1_ID, MESSAGE_BUTTON4_ID, OnClickedButton)
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CMessageWindow message handlers
BOOL CMessageWindow::CreateMessageWnd(CWnd * pWnd)
{
	CRect rect,rcc;
	pWnd->GetClientRect(rcc);
	DWORD	dwStyle=WS_VISIBLE|SS_CENTER|SS_NOTIFY|WS_CHILD;  

	rect.SetRect(0, rcc.bottom-(BTN_HEIGHT), rcc.right, rcc.bottom);

	return CWnd::Create(NULL, NULL,	dwStyle, 
					rect,	(pWnd != NULL) ? pWnd : NULL,
					MESSAGE_WND_ID);
}

int CMessageWindow::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	CRect rect(0, 0, 0, 0),rcc;
	GetClientRect(rcc);
	DWORD	dwStyle=WS_VISIBLE|SS_CENTER|WS_BORDER|SS_NOTIFY|WS_BORDER|WS_CHILD;  
	
	LOGFONT lf;
	memset(&lf, 0, sizeof(LOGFONT));       // zero out structure
	lf.lfHeight = 13;                      // request a 12-pixel-height font
	lf.lfWeight = FW_BOLD;
	lf.lfCharSet = 0x81;				   // 한글 Character Set
	strcpy(lf.lfFaceName, "고딕체");
	VERIFY(m_Font.CreateFontIndirect(&lf));  // create the font

	m_combobox.CreateEx(WS_EX_CLIENTEDGE,"COMBOBOX",NULL,
		WS_VISIBLE | WS_BORDER | WS_CHILD | CBS_DROPDOWNLIST ,
		rect,this,MESSAGE_COMBO_ID,NULL);

	m_combobox.SetFont(&m_Font);
	return 0;
}

void CMessageWindow::OnClickedButton(UINT nID)
{
	switch (nID)
	{
	case MESSAGE_BUTTON1_ID:
		if (m_combobox.GetCurSel()!=0) 
		m_combobox.SetCurSel(m_combobox.GetCurSel()-1);
		break;
	case MESSAGE_BUTTON2_ID:
		m_combobox.SetCurSel(m_combobox.GetCount()-1);
		break;
	}
}

////////////////////////////////////////////////////////////
//	lpstr ==> Message
//	errOpp ==> Message 우선 순위 ( 작은 값 : 높은 우선순위, 10 : 정상 Message
////////////////////////////////////////////////////////////
void CMessageWindow::AddMessage(LPSTR lpstr, UINT errOpp/*=0*/)
{
	if(lpstr == NULL || errOpp < 0)
	{
		DelMessage();
		return;
	}
	CString strMessage = lpstr;
	TRACE(strMessage);

	int nMin = min( MESSAGE_MAX, strMessage.GetLength());
	strMessage.Left(nMin);
	strMessage.TrimRight();
	for(int i=0; i<m_ErrOppArray.GetSize(); i++)
	{
		if((UINT)m_ErrOppArray.GetAt(i) >= errOpp)
		{
			m_ErrOppArray.InsertAt(i, errOpp);
			m_combobox.InsertString(i, strMessage);
			m_combobox.SetCurSel(0);
			CheckMaxLines();
			return;
		}
	}
	m_ErrOppArray.Add(errOpp);
	m_combobox.AddString(strMessage);
	m_combobox.SetCurSel(0);
	CheckMaxLines();
}



int CMessageWindow::GetMessage(CString &Message)
{
	Message = "";
	int index = m_combobox.GetCurSel();
	if(index != CB_ERR)
		m_combobox.GetLBText(index, Message);
	return Message.GetLength();
}

void CMessageWindow::DelMessage()
{
	m_ErrOppArray.RemoveAll();
    m_combobox.ResetContent();
}


void CMessageWindow::ResizeMessageWnd(UINT nType, int cx, int cy) 
{
	CRect rect, rcc;
	GetParent()->GetClientRect(&rect);

	int nGapX = 0;
	int nGapY = 0;
	
	if(nType == 1)
	{
		nGapX = 6;
		nGapY = 4;
	}

	MoveWindow(0, rect.bottom-(BTN_HEIGHT) - nGapX, rect.Width() - nGapY, BTN_HEIGHT);
	GetClientRect(&rcc);

	m_combobox.MoveWindow(1, rcc.top+1, rcc.right-2, 200);
}

void CMessageWindow::MoveMessageWnd(CRect rect) 
{
	MoveWindow(0, rect.bottom-(BTN_HEIGHT), rect.Width(), BTN_HEIGHT);

	CRect rcc;
	GetClientRect(&rcc);
	m_combobox.MoveWindow(1, rcc.top+1, rcc.right-2, 200);
}

BOOL CMessageWindow::OnEraseBkgnd(CDC* pDC) 
{
	CRect rect;
	GetClientRect(&rect);

	//pDC->FillSolidRect(rect, RGB(255, 0, 0));
	pDC->FillSolidRect(rect, RGB(253, 253, 253));
	
	return TRUE;
}

int CMessageWindow::GetWndHeight()
{
	return BTN_HEIGHT;
}

void CMessageWindow::CheckMaxLines()
{
	// { Check MaxLines
	int nLine = m_combobox.GetCount();
	if(m_nMaxLine<nLine) 
	{
		m_combobox.DeleteString(nLine-1);
		m_ErrOppArray.RemoveAt(0);
	}
	// }
}

void CMessageWindow::SetConfigFile(LPCSTR _szFile)
{
	m_szCfgFile=_szFile;
	m_nMaxLine = ::GetPrivateProfileInt("MESSAGELINE", "LINES", 5, m_szCfgFile);
}

