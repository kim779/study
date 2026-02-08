// ButtonEx.cpp : implementation file
//

#include "stdafx.h"
#include "ButtonEx.h"

#include "../Include_Chart/Dll_Load/IMetaTable.h"		// for _MTEXT()
#include "Definition.h"
#include "ToolBarDlg.h"
#include "ObjTool.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CButtonEx

CButtonEx::CButtonEx()
{
	m_pParent = NULL;
	m_strOption1 = _T("");
	m_strOption2 = _T("");
	m_strParam = _T("");
	m_strToolTipText = _T("");
	m_bState = false;
}

CButtonEx::~CButtonEx()
{
}


BEGIN_MESSAGE_MAP(CButtonEx, CButton)
	//{{AFX_MSG_MAP(CButtonEx)
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CButtonEx message handlers


/*-----------------------------------------------------------------------------------------
 - Function    :  DrawItem
 - Created at  :  2004-11-08   13:59
 - Author      :  최종찬
 - Description :  비트맵이미지에서 해당 버튼들을 그린다.
				  (IMPORTANT: CToolOptionInfo의 Enum값의 위치가 바뀌면 절대! 안됨. 버튼이 매치가
					안될것임.)
 -----------------------------------------------------------------------------------------*/
void CButtonEx::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct) 
{
	HDC hdcMem = CreateCompatibleDC(lpDrawItemStruct->hDC); 
	SelectObject(hdcMem, CToolBarDlg::m_bitmapButton); 

	int nX = 0;	// 비트맵에서 버튼의 시작점 X.
	int nY = 0;	// 비트맵에서 버튼의 시작점 Y.

	if(m_enumToolOption == CToolOptionInfo::T_INFLATE)	// 확대/축소
	{
		if(m_strParam == _MTEXT( C4_ZOOM_IN))	// 확대
			nX = 0;	// 이미지파일에서 (0, y) 버튼.
		else if(m_strParam = _MTEXT( C4_ZOOM_OUT))	// 축소
			nX = TB_BUTTON_WIDTH;	// 이미지파일에서 (1, y)버튼
	}
	else
		nX = (m_enumToolOption + 1 ) * TB_BUTTON_WIDTH;

	if(m_enumToolOption == CToolOptionInfo::T_AUTOSCROLL)
	{ // 자동스크롤은 버튼이 세개 생성된다.
		if(m_strParam == _MTEXT( C4_STOP_AUTO_SCROLL))
			nX += TB_BUTTON_WIDTH;
		if(m_strParam == _MTEXT( C4_RIGHT_AUTO_SCROLL))
			nX += (TB_BUTTON_WIDTH * 2);
	}
	else if(m_enumToolOption > CToolOptionInfo::T_AUTOSCROLL)	
	{ // 자동스크롤버튼이후 생성되는 버튼들은 자동스크롤버튼 생성(3개)된 만큼 빼고 생성한다.
		nX += (TB_BUTTON_WIDTH * 2);
	}

	if( m_enumToolOption == CToolOptionInfo::T_WHOLEVIEW)
	{
		TRACE( "%d\r\n", m_bState);
	}

	if(m_bState) // || lpDrawItemStruct->itemState & ODS_SELECTED)	// if Selected
	{
		nY = TB_BUTTON_HEIGHT;	// (x, 1) 버튼
	}

	// 50번째버튼부터는 이미지파일에서 세번째 줄에 있는 버튼부터 시작한다.
	// (2006/11/24 - Seung-Won, Bae) for Third Tool Button Image Line
	int nLineIndex = m_enumToolOption / 50;
	nX -= nLineIndex * 50 * TB_BUTTON_WIDTH;
	nY += TB_BUTTON_HEIGHT * 2 * nLineIndex;

	// Destination 
	StretchBlt( 
		lpDrawItemStruct->hDC,         // destination DC 
		lpDrawItemStruct->rcItem.left, // x upper left 
		lpDrawItemStruct->rcItem.top,  // y upper left 

		// The next two lines specify the width and 
		// height. 
		lpDrawItemStruct->rcItem.right - lpDrawItemStruct->rcItem.left, 
		lpDrawItemStruct->rcItem.bottom - lpDrawItemStruct->rcItem.top, 
		hdcMem,    // source device context 
		nX, nY,      // x and y upper left 
		TB_BUTTON_WIDTH,
		TB_BUTTON_HEIGHT,
		SRCCOPY);  // raster operation 

	DeleteDC(hdcMem);	
}

/*-----------------------------------------------------------------------------------------
 - Function    :  GetToolOptionInfo
 - Created at  :  2004-11-09   15:56
 - Author      :  최종찬
 - Description :  툴옵션정보를 리턴한다.
 -----------------------------------------------------------------------------------------*/
CToolOptionInfo::TOOLOPTION CButtonEx::GetToolOptionInfo()
{
	return m_enumToolOption;
}


/*-----------------------------------------------------------------------------------------
 - Function    :  SetToolObject
 - Created at  :  2004-11-08   14:11
 - Author      :  최종찬
 - Parameters  :  pObjTool - 버튼 설정에 필요한 정보를 담고 있는 Object.(Type, Name, Option1, Option2)
 -                strParam - 한 옵션정보(ex: 확대/축소, 파노라마, 지표연속조회..)에 두개이상의 버튼을 생성해야
							 하는 경우에 그 구분값 역할을 하는 파라미터.
 - Return Value:  None
 - Description :  ToolObject(Type, Name, Option1,2가 필요)와 한 옵션에 두개이상의 버튼을 위한
				  strParam을 세팅한다.
 -----------------------------------------------------------------------------------------*/
void CButtonEx::SetToolObject(CObjTool* pObjTool, const CString& strParam /*= _T("")*/)
{
	m_enumToolOption = pObjTool->GetType();
	pObjTool->GetOption(m_strOption1, m_strOption2);
	m_strParam = strParam;

	_MHWND( m_pParent->GetParent()->GetSafeHwnd());
	// 툴팁텍스트 세팅.
	if(m_enumToolOption == CToolOptionInfo::T_INFLATE || m_enumToolOption == CToolOptionInfo::T_AUTOSCROLL)
		SetToolTipText( _MTOL4( m_strParam));
	else
		SetToolTipText( _MTOL4( pObjTool->GetName()));
}



/*-----------------------------------------------------------------------------------------
 - Function    :  AddToolInTooltip
 - Created at  :  2004-11-08   17:38
 - Description :  툴팁을 추가한다.
 -----------------------------------------------------------------------------------------*/
void CButtonEx::AddToolInTooltip(const CString& strText, const bool bActivate)
{
	// tooltip의 text가 없을 경우에 추가 한다.
	if(m_ToolTip.GetToolCount() == 0)
	{
		CRect rectBtn; 
		GetClientRect(rectBtn);
		m_ToolTip.AddTool(this, (LPCTSTR)strText, rectBtn, 1);
	}

	// tooltip text 를 set
	m_ToolTip.UpdateTipText((LPCTSTR)strText, this, 1);
	m_ToolTip.Activate(bActivate);
}


/*-----------------------------------------------------------------------------------------
 - Function    :  SetToolTipText
 - Created at  :  2004-11-08   17:37
 - Description :  툴팁텍스트를 세팅한다.
 -----------------------------------------------------------------------------------------*/
void CButtonEx::SetToolTipText(const CString& strText, const bool bActivate)
{
	if(strText.IsEmpty())
		return;

	if(m_ToolTip.m_hWnd != NULL)
		return;

	m_ToolTip.Create(this);
	m_ToolTip.Activate(FALSE);
	
	AddToolInTooltip(strText, bActivate);
}


/*-----------------------------------------------------------------------------------------
 - Function    :  SetToolTipText
 - Created at  :  2004-11-08   17:37
 - Description :  툴팁 텍스트를 세팅한다.
 -----------------------------------------------------------------------------------------*/
void CButtonEx::SetToolTipText(const CString& strText)
{
	m_strToolTipText = strText;
}


/*-----------------------------------------------------------------------------------------
 - Function    :  OnMouseMove
 - Created at  :  2004-11-08   16:43
 - Author      :  최종찬
 - Description :  마우스 움직일때마다 툴팁을 보여준다.
 -----------------------------------------------------------------------------------------*/
void CButtonEx::OnMouseMove(UINT nFlags, CPoint point) 
{
	if(m_strToolTipText.IsEmpty())
		return;

	if(m_ToolTip.m_hWnd == NULL)
		SetToolTipText(m_strToolTipText, true);

	MSG msg;
	msg.hwnd = m_hWnd;
	msg.message = WM_MOUSEMOVE;
	msg.wParam = 0;
	msg.lParam = 0;
	m_ToolTip.RelayEvent(&msg);
	
	CButton::OnMouseMove(nFlags, point);
}



/*-----------------------------------------------------------------------------------------
 - Function    :  SetState
 - Created at  :  2004-11-19   11:44
 - Author      :  최종찬
 - Description :  버튼상태를 설정한다.
 -----------------------------------------------------------------------------------------*/
void CButtonEx::SetState(bool bState /* = true */)
{
	m_bState = bState;
	Invalidate( FALSE);
}

/*-----------------------------------------------------------------------------------------
 - Function    :  GetState
 - Created at  :  2004-11-19   11:25
 - Author      :  최종찬
 - Description :  버튼상태를 리턴한다.
 -----------------------------------------------------------------------------------------*/
bool CButtonEx::GetState()
{
	return m_bState;
}


/*-----------------------------------------------------------------------------------------
 - Function    :  GetOption1
 - Created at  :  2004-12-02   14:43
 - Author      :  최종찬
 - Description :  버튼의 첫번째 옵션값을 리턴한다.
 -----------------------------------------------------------------------------------------*/
const CString& CButtonEx::GetOption1()
{
	return m_strOption1;
}


/*-----------------------------------------------------------------------------------------
 - Function    :  GetOption2
 - Created at  :  2004-12-02   14:44
 - Author      :  최종찬
 - Description :  버튼의 두번째 옵션값을 리턴한다.
 -----------------------------------------------------------------------------------------*/
const CString& CButtonEx::GetOption2()
{
	return m_strOption2;
}


/*-------------------------------------------------------------------------------
 - Function    :  GetParam
 - Created at  :  2005-04-15   10:57
 - Author      :  최종찬
 - Description :  버튼의 파라미터값을 리턴한다.
 -------------------------------------------------------------------------------*/
const CString& CButtonEx::GetParam()
{
	return m_strParam;
}

// (2006/11/16 - Seung-Won, Bae) Support Auto Repeat and Focus Return.
void CButtonEx::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	CButton::OnLButtonDown(nFlags, point);
	SetCapture();
}
void CButtonEx::OnLButtonUp(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	ReleaseCapture();
	CButton::OnLButtonUp(nFlags, point);

	CWnd *pParent = GetParent();			// Getting ToolBar DLG
	if( pParent)
	{
		pParent = pParent->GetParent();
		if( pParent) pParent->SetFocus();	// Getting Chart OCX
	}
}
