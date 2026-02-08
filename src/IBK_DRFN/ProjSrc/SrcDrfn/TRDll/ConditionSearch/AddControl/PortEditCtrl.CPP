// PortEditCtrl.cpp : implementation file
//

/**********************************************************************/
/*																																		*/
/*											PortPolio 조합 Edit Control										*/
/*													(Mouse Control)														*/
/*																																		*/
/*												 작성일 : 2002. 5~													*/
/*							       작성자 : 우동우 (Doori)												*/
/*																																		*/
/**********************************************************************/

#include "stdafx.h"
#include "PortEditCtrl.h"
#include "../resource.h"		//<= Cursor

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define	FONT_STYLE								"굴림체"

#define COLOR_NORMAL           			RGB(0, 0, 0)
#define COLOR_OPERATION							RGB(0, 0, 255)
#define COLOR_OPERATION_SELECT			RGB(255, 0, 0)
#define COLOR_PORTPOLIO							RGB(0, 0, 0)

const   COLORREF  clrOperation          = COLOR_OPERATION;
const   COLORREF  clrPortPolio          = COLOR_PORTPOLIO;
const   COLORREF  clrOperationSelect  = COLOR_OPERATION_SELECT;

#define OPERATION_SEARCH_COUNT					5
/////////////////////////////////////////////////////////////////////////////
// CPortEditCtrl

CPortEditCtrl::CPortEditCtrl()
{
  LOGFONT	  logFont;
  GETFONT(logFont, 12, 400, m_fontDefault);

	m_bMouseOver = FALSE;
}

CPortEditCtrl::~CPortEditCtrl()
{
  m_fontDefault.DeleteObject();
}


BEGIN_MESSAGE_MAP(CPortEditCtrl, CRichEditCtrl)
//{{AFX_MSG_MAP(CPortEditCtrl)
  ON_WM_CREATE()
  ON_WM_MOUSEMOVE()
  ON_WM_LBUTTONDBLCLK()
//}}AFX_MSG_MAP
ON_MESSAGE(WM_MOUSELEAVE, OnMouseLeave)

END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPortEditCtrl message handlers
int CPortEditCtrl::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CRichEditCtrl::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	SetReadOnly(TRUE);
	
	//## Cursor ##
	m_hOperationSelectCursor = AfxGetApp()->LoadCursor(IDC_CURSOR_ANDOR);
	
	DeleteAll();
	
	return 0;
}

BOOL CPortEditCtrl::AddPortData(PortPolio  stPortPolio)
{
	if (IsExistPortCode(stPortPolio.strPortNumber)) 
		return FALSE;
	
	stPortPolio.strOperation = m_strAndOr;
	m_listPort.AddTail(stPortPolio);

	ReLoadAll();
	
	return TRUE;
}


BOOL CPortEditCtrl::IsExistPortCode(CString strPortNumber)
{
	BOOL bFlag = FALSE;

	CString strPort;
	POSITION pos;
	PortPolio  stPortPolio;
	
	int nCount = m_listPort.GetCount();
	
	for (int i = 0; i < nCount; i++)
	{
		pos = m_listPort.FindIndex(i);
		stPortPolio = m_listPort.GetAt(pos);
		
		if (stPortPolio.strPortNumber == strPortNumber)
		{
			return true;
		}
	}	
	
	return false;
}


BOOL CPortEditCtrl::DeletePortData(CString strPortNumber)
{
	BOOL bFlag = FALSE;
	CString strPort;
	POSITION pos;
	PortPolio  stPortPolio;
	
	int nCount = m_listPort.GetCount();
	
	for (int i = 0; i < nCount; i++)
	{
		pos = m_listPort.FindIndex(i);
		stPortPolio = m_listPort.GetAt(pos);
		
		if (stPortPolio.strPortNumber == strPortNumber)
		{
			m_listPort.RemoveAt(pos);
			bFlag = TRUE;
			break;
		}
	}
	
	if (bFlag == TRUE)
		ReLoadAll();
	
	return bFlag;
}


void CPortEditCtrl::DeleteAll()
{
	m_strAndOr = "or";
	m_listPort.RemoveAll(); 
	SetWindowText("");
	
	m_noldStartOperation = -1;
	m_noldEndOperation = -1;
}


void CPortEditCtrl::ChangeAllOpt()
{
	POSITION pos;
	PortPolio  stPortPolio;
	
	if (m_strAndOr == "and")
		m_strAndOr = "or";
	else if (m_strAndOr == "or")
		m_strAndOr = "and";
	
	int nCount = m_listPort.GetCount();
	for (int i = 0; i < nCount; i++)
	{
		pos = m_listPort.FindIndex(i);
		stPortPolio = m_listPort.GetAt(pos);
		
		stPortPolio.strOperation = m_strAndOr;
		m_listPort.SetAt(pos, stPortPolio);
	}
	
	ReLoadAll();
}


void CPortEditCtrl::ChangeAllOpt(BOOL bAnd)
{
	POSITION pos;
	PortPolio  stPortPolio;
	
	if (bAnd == TRUE)
		m_strAndOr = "and";
	else if (bAnd == FALSE)
		m_strAndOr = "or";
	
	int nCount = m_listPort.GetCount();
	for (int i = 0; i < nCount; i++)
	{
		pos = m_listPort.FindIndex(i);
		stPortPolio = m_listPort.GetAt(pos);
		
		stPortPolio.strOperation = m_strAndOr;
		m_listPort.SetAt(pos, stPortPolio);
	}
	
	ReLoadAll();
}

int CPortEditCtrl::GetPortDataCount()
{
	int nCount = m_listPort.GetCount();
	return nCount;
}


void CPortEditCtrl::ReLoadAll()
{
	CString strPort;
	POSITION pos;
	PortPolio  stPortPolio;
	
	int nCount = GetPortDataCount();
	
	for(int i = 0; i < nCount; i++)
	{
		pos = m_listPort.FindIndex(i);
		stPortPolio = m_listPort.GetAt(pos);
		
		/*
		//# Display : Name
		if (i == 0)
			strPort += stPortPolio.strPortName + " ";
		else
			strPort += stPortPolio.strOperation + " " + stPortPolio.strPortName + " ";
		*/

		//# Display : Number
		if (i == 0)
			strPort += stPortPolio.strPortNumber + " ";
		else
			strPort += stPortPolio.strOperation + " " + stPortPolio.strPortNumber + " ";
	}
	
	SetWindowText(strPort);
	Invalidate();
	
	m_noldStartOperation = -1;
	m_noldEndOperation = -1;
	
	SetPortPolioColor();
	SendMessage(WM_KILLFOCUS, 0, 0);
}


void CPortEditCtrl::SetPortPolioColor()
{
	CString strPort;
	GetWindowText(strPort);
	
	BOOL bOperation;
	for (int i = 0; i < strPort.GetLength(); i++)
	{
		bOperation = FALSE;
		if ( ((CString)strPort[i] == "a") ||((CString)strPort[i] == "o") )
			bOperation = TRUE;
		
		if (bOperation == TRUE)
		{
			//## Color Setting ##
			if ((CString)strPort[i] == "a")
				SetSel(i, i + 3);
			else if ((CString)strPort[i] == "o")
				SetSel(i, i + 2);
			
			Invalidate();
      SetSelectionCharFormat(GetFontFormat(clrOperation));
		}
		SetSel(0, 0);
	}
}

CHARFORMAT CPortEditCtrl::GetFontFormat(COLORREF clrColor)
{
  CHARFORMAT cfResult;
	cfResult.dwMask = CFM_CHARSET | CFM_FACE | CFM_SIZE | CFM_OFFSET | CFM_COLOR;
	cfResult.dwMask ^= CFM_ITALIC ^ CFM_BOLD ^ CFM_STRIKEOUT ^ CFM_UNDERLINE;

	cfResult.dwEffects = 0;
	cfResult.yHeight = 180;   //# 10pts * 20 twips/point = 200 twips
	cfResult.bPitchAndFamily = FIXED_PITCH | FF_MODERN;
	cfResult.yOffset = 0;
	cfResult.crTextColor = clrColor;

	cfResult.bCharSet = DEFAULT_CHARSET;

	strcpy(cfResult.szFaceName, (const char*)FONT_STYLE);
	cfResult.cbSize = sizeof(cfResult);

  return cfResult;
}

int CPortEditCtrl::PointToStrPosition(CPoint point)
{
	CString strPort;
	GetWindowText(strPort);
	
	int nPositionChar = -1;
	
	CSize	szFont;
	if (point.x == 1)
		nPositionChar = 0;
	else
	{
		szFont = GetFontSize();
		
		int nFontWidth = 0;
		for(int i = 0; i < strPort.GetLength(); i++)
		{
			nFontWidth += szFont.cx;
			
			if(nFontWidth >= point.x)
			{
				nPositionChar = i;
				break;
			}
		}
	}
	
	int nLineCnt = GetLineCount();
	for (int nLineIndex = 0; nLineIndex < nLineCnt; nLineIndex++)
	{
		if (point.y >= szFont.cy * nLineIndex)
		{
			if (LineIndex(nLineIndex) != -1)
				nPositionChar += LineIndex(nLineIndex);
		}
	}
	
	return nPositionChar;
}

CSize CPortEditCtrl::GetFontSize()
{
	CClientDC	dc(this);
	CFont*	pOldFont = dc.SelectObject(&m_fontDefault);
	
	CSize	sizeFont;
	TEXTMETRIC	tm;
	
	dc.GetTextMetrics(&tm);
	sizeFont.cx	=	tm.tmAveCharWidth;
	sizeFont.cy	=	tm.tmHeight;
	
	dc.SelectObject(pOldFont);
	
	return	sizeFont;
}

void CPortEditCtrl::OnMouseMove(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	if (!m_bMouseOver)
	{
		m_bMouseOver = TRUE;
		
		TRACKMOUSEEVENT tme;
		tme.cbSize = sizeof(tme);
		tme.dwFlags = TME_LEAVE;// | TME_HOVER;
		tme.hwndTrack = m_hWnd;
		//tme.dwHoverTime = HOVER_DEFAULT;
		::_TrackMouseEvent(&tme);
	}
	
	CString strPort;
	int nPositionChar = -1;
	GetWindowText(strPort);
	
	nPositionChar = PointToStrPosition(point);
	int nSearchResult, nSearchStart;
	int nAndOrTextCount;  //# 0: None
	//# 3: and
	//# 2: or
	
	if (nPositionChar == -1)
		return;
	
	nSearchStart = nPositionChar - 1;
	if(nSearchStart < 0)
		nSearchStart = 0;
	
	if (strPort.GetLength() < nSearchStart)
		return;
	
	CString strTempPort;
	strTempPort = strPort.Mid(nSearchStart, OPERATION_SEARCH_COUNT);
	
	nAndOrTextCount = 0;
	if (strTempPort.Find("and", 0) != -1)
	{
		nSearchResult = strTempPort.Find("and", 0);
		nAndOrTextCount = 3; //#<- and
	}
	else if (strTempPort.Find("or", 0) != -1)
	{
		nSearchResult = strTempPort.Find("or", 0);
		nAndOrTextCount = 2; //<- or
	}
	else  //#<= Not Have Operation
		return;
	
	//## 전체 논리 조합에서 연산자위치 복귀
	nSearchResult = nSearchStart + nSearchResult;
	
	GetWindowText(strPort);
	if (nAndOrTextCount != 0) 
	{
		if ((m_noldStartOperation > 0) && (m_noldEndOperation > 0))
		{
			SetSel(m_noldStartOperation, m_noldEndOperation);
			Invalidate();
      SetSelectionCharFormat(GetFontFormat(clrOperation));
		}
		
		//## Now Select Operation Position ##
		SetSel(nSearchResult, nSearchResult + nAndOrTextCount);
		Invalidate();
    SetSelectionCharFormat(GetFontFormat(clrOperationSelect));

		//## Operation Color Setting END ##
		
		//## Now Select Operation Position Save ##
		m_noldStartOperation = nSearchResult;
		m_noldEndOperation = nSearchResult + nAndOrTextCount;
		
		TRACE(_T("* Now Position START: %d, END: %d \n"),  
			nSearchResult, nSearchResult+nAndOrTextCount);
		
		SetCursor(m_hOperationSelectCursor);
		SendMessage(WM_KILLFOCUS, 0, 0);
	}
	
	CRichEditCtrl::OnMouseMove(nFlags, point);
}


void CPortEditCtrl::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	TRACE("\n# PortPlio LButton Db Click!");
	
	CString strPort;
	GetWindowText(strPort);
	
	int nPositionChar = -1;
	nPositionChar = PointToStrPosition(point);
	int nSearchStart;	
	if (nPositionChar == -1)
		return;
	
	nSearchStart = nPositionChar - 1;
	if(nSearchStart < 0)
		nSearchStart = 0;
	
	if (strPort.GetLength() < nSearchStart)
		return;
	
	CString strTempPort;
	strTempPort = strPort;
	strTempPort = strTempPort.Mid(nSearchStart, OPERATION_SEARCH_COUNT);
	TRACE("* Move Search String => " + strTempPort + "\n");
	
	if ((strTempPort.Find("and", 0) != -1) || (strTempPort.Find("or", 0) != -1))
		ChangeAllOpt();
	else
		return;
	
	ReLoadAll();
	CRichEditCtrl::OnLButtonDblClk(nFlags, point);
}


LRESULT CPortEditCtrl::OnMouseLeave(WPARAM wParam, LPARAM lParam)
{
	//## Operation Color restoration ##
	if ((m_noldStartOperation > -1) || (m_noldEndOperation > -1))
	{
		SetSel(m_noldStartOperation, m_noldEndOperation);
		Invalidate();
    SetSelectionCharFormat(GetFontFormat(clrOperation));
		
		m_noldStartOperation = -1;
		m_noldEndOperation = -1;
	}
	
	m_bMouseOver = FALSE;
	
	return 0;
}


BOOL CPortEditCtrl::GetPortGroupList(CStringArray &saPortGroup, BOOL &bAnd)
{
	BOOL bResult = FALSE;

	saPortGroup.RemoveAll();

	CString strPort;
	POSITION pos;
	PortPolio  stPortPolio;
	
	CString strAdddItem;
	int nCount = m_listPort.GetCount();
	if (nCount > 0)
	{
		bResult = TRUE;

		for (int nIndex = 0; nIndex < nCount; nIndex++)
		{
			pos = m_listPort.FindIndex(nIndex);
			stPortPolio = m_listPort.GetAt(pos);

			strAdddItem = stPortPolio.strPortNumber;
			if (stPortPolio.nType == 1)			//# Account 
				strAdddItem += "#";

			saPortGroup.Add(strAdddItem);
		}

		bAnd = (m_strAndOr == "and") ? TRUE : FALSE;
	}

	return bResult;
}

BOOL CPortEditCtrl::GetPortInsertCheck(CStringArray &saPort, CString strJongCode)
{
	BOOL bResult = FALSE;

	BOOL bState = FALSE;
	for (int nIndex = 0; nIndex < saPort.GetSize(); nIndex++)
	{
		if (saPort.GetAt(nIndex) == strJongCode)
		{
			bState = TRUE;
			break;
		}
	}
	
	if (m_strAndOr == "and")
	{
		if (bState == TRUE)
			bResult = TRUE;
	}
	else if (m_strAndOr == "or")
	{
		if (bState == FALSE)
			bResult = TRUE;
	}

	return bResult;
}


	
	
	

