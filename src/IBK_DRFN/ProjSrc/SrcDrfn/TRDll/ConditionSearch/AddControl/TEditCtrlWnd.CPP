// TEditCtrlWnd.cpp : implementation file
//

#include "stdafx.h"
#include "TEditCtrlWnd.h"

#include "TEditCtrl.h"

/************************************************************/
/*																		                      */
/*							Edit Wnd  Control							              */
/*																		                      */
/*																		                      */
/*							작성일 : 2004. 11~							            */
/*						작성자 : 우동우 (두리 정보 통신)				      */
/*																		                      */
/************************************************************/

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTEditCtrlWnd
#define IDC_EDIT_MAPLOADEDIT        20000

CTEditCtrlWnd::CTEditCtrlWnd()
{
  LOGFONT	  logFont;
  GETFONT(logFont, 12, 400, m_fontDefault);

	m_clrFace			= RGB(255, 255, 255);
	
	m_clrTopLeft		= GetSysColor(COLOR_3DHILIGHT);
	m_clrBottomRight	= GetSysColor(COLOR_3DSHADOW);
	
	m_pEditCtrl = new CTEditCtrl;

	m_bIgnoreComma = FALSE;
	m_bTimeType = FALSE;
}

CTEditCtrlWnd::~CTEditCtrlWnd()
{
  m_fontDefault.DeleteObject();
	delete m_pEditCtrl;
}

BEGIN_MESSAGE_MAP(CTEditCtrlWnd, CWnd)
	//{{AFX_MSG_MAP(CTEditCtrlWnd)
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTEditCtrlWnd message handlers

BOOL CTEditCtrlWnd::Create(const RECT& rect, CWnd* pParentWnd, UINT nID, 
								COLORREF clrFace, int nIntegerLength, int nRealLength, 
								BOOL bEnable) 
{
	BOOL bResult = TRUE;
	bResult = CWnd::Create(NULL, "", WS_CHILD | WS_VISIBLE, rect, pParentWnd, nID);

	m_clrFace = clrFace;

	m_nIntegerLength = nIntegerLength;
	m_nRealLength = nRealLength;
	m_bEnable = bEnable;
	
	return bResult;
}


void CTEditCtrlWnd::OnPaint() 

{
	CPaintDC dc(this);
	
	CString strWindow, strCurreny;
	CRect	rcClient;
	
	GetClientRect(rcClient);

	dc.FillSolidRect(rcClient, m_clrFace);
	dc.SetBkMode(TRANSPARENT);
	/*dc.Draw3dRect(rcClient, m_clrBottomRight, m_clrTopLeft);*/
	CPen pen;
	pen.CreatePen(PS_SOLID, 1, RGB(200, 200, 200));
	CPen* pOldPen = (CPen*)dc.SelectObject(&pen);
	dc.Rectangle(rcClient);
	dc.SelectObject(pOldPen);
	pen.DeleteObject();
	
	
	rcClient.DeflateRect(2, 2, 2, 2);
	GetWindowText(strWindow);
	strCurreny = GetCurrencyText(strWindow);
	strCurreny = strWindow;
	
	CFont *pOldFont = dc.SelectObject(&m_fontDefault);
	dc.DrawText(strCurreny, rcClient, DT_VCENTER | DT_SINGLELINE | DT_RIGHT);
	dc.SelectObject(pOldFont);
}

CString CTEditCtrlWnd::GetCurrencyText(CString strSource)
{
	int nCommaPos;
	int nEndPos  = strSource.GetLength();
	int nNegative = 0;
	
	if(strSource.Find('-') != -1)
		nNegative = 1;
	
	if (nEndPos >= 4)
	{
		nCommaPos = nEndPos;
		
		do
		{
			nCommaPos = nCommaPos - 3;
			strSource.Insert(nCommaPos,',');
		}
		while(nCommaPos >= (4 + nNegative));
	}
	
	return strSource;
}

void CTEditCtrlWnd::OnLButtonDown(UINT nFlags, CPoint point) 
{
	if(m_pEditCtrl)
	{
		CString strBeforeText;
		GetWindowText(strBeforeText);
		m_pEditCtrl->SetBeforText(strBeforeText);
	}
	
	int nGap = 1;
	CRect   rcClient;
	CString strWindow;
	
	if (m_bEnable == TRUE)
		return;

	GetClientRect(rcClient);
	rcClient.DeflateRect(nGap, nGap, nGap, nGap);
	
	CRect rt;
	GetWindowRect(rt);
	ScreenToClient(rt);
	
	if( ((rt.left <= point.x) && (point.x <= rt.left + nGap)) ||
		((rt.right - nGap <= point.x) && (point.x <= rt.right)) ||
		((rt.top <= point.y) && (point.y <= rt.top + nGap)) ||
		((rt.bottom - nGap<= point.y) && (point.y <= rt.bottom)) )
	{
		return;
	}

	m_pEditCtrl->Create(rcClient, this, IDC_EDIT_MAPLOADEDIT,
											m_nIntegerLength,
											m_nRealLength);
	m_pEditCtrl->SetFont(&m_fontDefault);
	m_pEditCtrl->IngnoreComma(m_bIgnoreComma);
	
	if (m_pEditCtrl->GetSafeHwnd() != NULL)
	{
		GetWindowText(strWindow);
		m_pEditCtrl->SetInit(strWindow);
	}
	else
		::MessageBox(m_hWnd, "Map Load EditWnd Error!", PROGRAM_TITLE, MB_OK | MB_TOPMOST);
	
	
	CWnd::OnLButtonDown(nFlags, point);
}



void CTEditCtrlWnd::InsertComma(CString & str1)
{
	if(m_bIgnoreComma)
		return;

	CString str,strTemp1,strTemp2,strTemp3;
	str = str1;
	//부호 처리.
	if(str.GetLength() > 0)
	{
		if(str.GetAt(0) == '-' || str.GetAt(0) == '+')
		{
			strTemp1 = str.Left(1);
			str = str.Mid(1);//부호제거한 문자열.
		}
	}

	int pos ;
	int x,y,i;
	//소수점 부분을 제거한다.
	if((pos = str.Find(".")) != -1)
	{
		strTemp3 = str.Mid(pos);//소수점부터 이하값을 가지고 있는 변수.
		str = str.Left(pos);//소수점 위의 내용을 가지고 있다.
	}
	//문자열의 길이를 구한다.
	int nLength = str.GetLength();
	int max;//콤마의 최대갯수

	//정수 3자리 이상의 값일 경우.
	if((x = nLength / 3) > 0)//몫->x
	{
		if((y = nLength % 3) > 0) //정확히 나누어 떨어지지 않을 때.
			max = x;
		else if(y == 0)
			max = x-1;
		
		for(i = 0 ; i < max ; i++)//실제로 콤마를 삽입하는 부분.
		{
			if((pos = str.Find(','))== -1)//콤마가 문자열에 없을 경우.
			{
				str = str.Left(nLength-3)+","+str.Mid(nLength-3);					
			}
			else
			{
				str = str.Left(pos-3)+","+str.Mid(pos-3);					
			}
		}
		str1 = strTemp1 + str + strTemp3;		
	}
}

BOOL CTEditCtrlWnd::TimeFormat(CString str)
{
	if(!m_bTimeType)
		return FALSE;		
	
	CString strTime;
	if(m_pEditCtrl != NULL)
		GetWindowText(strTime);
	
	if(strTime.GetLength() == 6)
	{
		strTime.Insert(2, _T(':'));
		strTime.Insert(5, _T(':'));

		m_pEditCtrl->SetWindowText((LPCTSTR)strTime);
		m_pEditCtrl->SetTimeType(TRUE);
	}
	else
	{
		CString strErrorMsg;
		strErrorMsg.Format("시분초 각 두자리씩 숫자로만 표시해주십시요.");
		::MessageBox(m_hWnd, strErrorMsg, PROGRAM_TITLE, MB_OK);
		SetFocus();
		return FALSE;						
	}

	return TRUE;
}

void CTEditCtrlWnd::SetTimeType(BOOL bTimeType)
{
	if(m_pEditCtrl)
		m_pEditCtrl->SetTimeType(bTimeType);
}