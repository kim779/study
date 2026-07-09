// Controlwnd.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "CX_POP.h"
#include "Controlwnd.h"

#include "PopInfo.h"
#include "TriDlg.h"


// CControlwnd

IMPLEMENT_DYNAMIC(CControlwnd, CWnd)

CControlwnd::CControlwnd()
{
	EnableAutomation();
	m_pBitmap[MsNo] = NULL;
	m_pBitmap[MsOver] = NULL;
	m_pBitmap[MsDown] = NULL;

	m_bDown = FALSE;
	m_bHover = FALSE;
	m_bTracking = FALSE;
	m_MouseState = MsNo;
	m_dlgTri = NULL;
}

CControlwnd::~CControlwnd()
{
	if (m_dlgpop)	
	{
		m_dlgpop->ShowWindow(SW_SHOW);
		m_dlgpop->OnBnClickedCancel();
		delete m_dlgpop;
	}
}

void CControlwnd::OnFinalRelease()
{
	// 자동화 개체에 대한 마지막 참조가 해제되면
	// OnFinalRelease가 호출됩니다. 기본 클래스에서 자동으로 개체를 삭제합니다.
	// 기본 클래스를 호출하기 전에 개체에 필요한 추가 정리 작업을
	// 추가하십시오.

	CWnd::OnFinalRelease();
}


BEGIN_MESSAGE_MAP(CControlwnd, CWnd)
	ON_WM_PAINT()
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_MOUSEMOVE()
	ON_MESSAGE(WM_MOUSEHOVER, OnMouseHover)
	ON_MESSAGE(WM_MOUSELEAVE, OnMouseLeave)
	ON_MESSAGE(WM_USER, OnMessage)
	ON_MESSAGE(WM_USER + 999, OnWndMsg)
//	ON_WM_SETFOCUS()
ON_WM_SETCURSOR()
//ON_WM_DESTROY()
//ON_WM_TIMER()
ON_WM_DESTROY()
END_MESSAGE_MAP()


BEGIN_DISPATCH_MAP(CControlwnd, CWnd)
	DISP_FUNCTION_ID(CControlwnd, "Setdata", dispidSetdata, Setdata, VT_EMPTY, VTS_BSTR)
	DISP_FUNCTION_ID(CControlwnd, "SetVisible", dispidSetVisible, SetVisible, VT_EMPTY, VTS_I4)
END_DISPATCH_MAP()

// 참고: IID_IControlwnd에 대한 지원을 추가하여
//  VBA에서 형식 안전 바인딩을 지원합니다. 
//  이 IID는 .IDL 파일에 있는 dispinterface의 GUID와 일치해야 합니다.

// {51297536-E912-4C77-96AB-5C2C18A4A295}
static const IID IID_IControlwnd =
{ 0x51297536, 0xE912, 0x4C77, { 0x96, 0xAB, 0x5C, 0x2C, 0x18, 0xA4, 0xA2, 0x95 } };

BEGIN_INTERFACE_MAP(CControlwnd, CWnd)
	INTERFACE_PART(CControlwnd, IID_IControlwnd, Dispatch)
END_INTERFACE_MAP()


// CControlwnd 메시지 처리기입니다.
LRESULT CControlwnd::OnWndMsg(WPARAM wParam, LPARAM lParam)
{
	switch(wParam)
	{
		case 999:
				return m_wndPOS;
		case 666:
			if(m_dlgTri == NULL)
				return 0;
			return (long)m_dlgTri;
	}
	return 0;
}

LRESULT CControlwnd::OnMessage(WPARAM wParam, LPARAM lParam)
{
	switch(LOBYTE(LOWORD(wParam)))
	{
	case DLL_SETFONT:
	case DLL_SETFONTx:
		{
			CString tmp( (LPCTSTR)lParam );
			int ipoint = HIWORD(wParam);

			_fontR font;
			font.name = (LPSTR)(LPCTSTR)m_Param.fonts;
			font.point = ipoint;
			font.italic = false;
			font.bold = FW_NORMAL;
			m_dlgpop->m_table->maxX = 0;
		
			m_dlgpop->m_table->Setfont(ipoint, m_Param.fonts);
			Invalidate();
		}
		break;
	}
	
	return 0;
}

void CControlwnd::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	CRect	WinRC;
	GetClientRect(&WinRC);
	
	if (m_pBitmap[m_MouseState])
	{
		dc.FillSolidRect(WinRC, RGB(100, 100, 100));
		WinRC.DeflateRect(1, 1, 1, 1);
		dc.FillSolidRect(WinRC, RGB(255, 255, 255));

		if (m_bDown)
			DrawBitmap(&dc, m_btnRect, m_pBitmap[MsDown]);
		else if (m_bHover)
			DrawBitmap(&dc, m_btnRect, m_pBitmap[MsOver]);
		else
			DrawBitmap(&dc, m_btnRect, m_pBitmap[m_MouseState]);
	}
	else
	{
		dc.FillSolidRect(WinRC, GetSysColor(COLOR_WINDOW));
		dc.DrawEdge(&WinRC, EDGE_SUNKEN, BF_RECT);   

		if (m_bDown)
			dc.DrawFrameControl(&m_btnRect, DFC_SCROLL, DFCS_SCROLLCOMBOBOX|DFCS_PUSHED);
		else
			dc.DrawFrameControl(&m_btnRect, DFC_SCROLL, DFCS_SCROLLCOMBOBOX);
	}
}


void CControlwnd::DrawBitmap(CDC *pDC, CRect rc, CBitmap *pBitmap)
{
	CString tmpS;
	if (pBitmap)
	{
		CDC		memDC;
		CBitmap		*pSBitmap;

		BITMAP stBitmapInfo;
		pBitmap->GetBitmap(&stBitmapInfo);

		memDC.CreateCompatibleDC(pDC);	
		if (memDC.m_hDC != NULL) 
			pSBitmap = (CBitmap*)memDC.SelectObject(pBitmap);

		//::TransparentBlt(pDC->m_hDC, rc.left, rc.top, rc.Width(), rc.Height(), memDC.m_hDC, 0, 0, stBitmapInfo.bmWidth, stBitmapInfo.bmHeight, 
		//	RGB(255, 0, 255));
	
		pDC->StretchBlt(rc.left, rc.top, rc.Width(), rc.Height(), &memDC, 0, 0, stBitmapInfo.bmWidth, stBitmapInfo.bmHeight, SRCCOPY);

		memDC.SelectObject(pSBitmap);
		memDC.DeleteDC();
	}
	else
	{
		pDC->FillSolidRect(rc, RGB(196,196,196));
	}
}


void CControlwnd::SetParam(_param *pParam)
{
	m_Param.key   = pParam->key;
	m_Param.name  = CString(pParam->name, pParam->name.GetLength());
	m_Param.rect  = CRect(pParam->rect.left, pParam->rect.top, pParam->rect.right, pParam->rect.bottom);
	m_Param.fonts = CString(pParam->fonts, pParam->fonts.GetLength());
	m_Param.point = pParam->point;
	m_Param.style = pParam->style;
	m_Param.tRGB  = pParam->tRGB;
	m_Param.pRGB  = pParam->pRGB;
	m_Param.options = CString(pParam->options, pParam->options.GetLength());

	CString tmps, strdata;
	tmps = m_Param.options;
	strdata = Parser(tmps, _T("/g"));

	m_wndPOS = (_wndPOS)atoi(tmps);
}

CString CControlwnd::Parser(CString &srcstr, CString substr)
{
	if (srcstr.Find(substr) == -1)
	{
		CString temp = srcstr;
		srcstr.Empty();
		return temp;
	}
	else
	{
		CString  temp = srcstr.Left(srcstr.Find(substr));
		srcstr = srcstr.Mid(srcstr.Find(substr) + substr.GetLength());
		return temp;
	}
	return "";
}

int CControlwnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  여기에 특수화된 작성 코드를 추가합니다.
	//CClientDC dc(this);
	//m_pctrlwnd = this;
	m_strHome = (char*)m_pWizard ->SendMessage(WM_USER, MAKEWPARAM(variantDLL, homeCC), NULL);
	//help_small.bmp BTN_help.bmp BTN_help_EN BTN_help_DN
	if (m_pBitmap[MsNo] == NULL)
		m_pBitmap[MsNo] = GetBitmap(m_strHome + _T("\\image\\help_small.bmp"));
	if (m_pBitmap[MsOver] == NULL)
		m_pBitmap[MsOver] = GetBitmap(m_strHome + _T("\\image\\help_small.bmp"));
	if (m_pBitmap[MsDown] == NULL)
		m_pBitmap[MsDown] = GetBitmap(m_strHome + _T("\\image\\help_small.bmp"));

	mh_ncursor = AfxGetApp()->LoadStandardCursor(IDC_ARROW);
	mh_qcursor = AfxGetApp()->LoadStandardCursor(IDC_HELP);

	
	m_dlgpop = new CPopInfo(_T(""), this);
	if (!m_dlgpop->Create(IDD_DLG_POP))
	{
		delete m_dlgpop;
		m_dlgpop = NULL;
	}
	else	
		m_dlgpop->Init();
		
	m_dlgTri = new CTriDlg(this);
	if (!m_dlgTri->Create(IDD_DLG_TRI))
	{
		delete m_dlgTri;
		m_dlgTri = NULL;
	}
	m_dlgTri->ShowWindow(SW_HIDE);
	
	m_dlgpop->m_table->Setfont(m_Param.point, m_Param.fonts);
	return 0;
}

CBitmap* CControlwnd::GetBitmap(CString path)
{
	return (CBitmap*)m_pWizard->SendMessage(WM_USER, getBITMAP, (long)path.operator LPCTSTR());
}

void CControlwnd::OnSize(UINT nType, int cx, int cy)
{
	CWnd::OnSize(nType, cx, cy);

	if (cx > 0 && cy > 0)
		ResizeControls();
}

void CControlwnd::ResizeControls()
{
	CRect	rect, editRC, btnRC;
	GetClientRect(&m_btnRect);
}

void CControlwnd::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	if(!m_bTracking)
	{
		TRACKMOUSEEVENT tme;
		tme.cbSize = sizeof(tme);
		tme.hwndTrack = m_hWnd;
		tme.dwFlags = TME_LEAVE|TME_HOVER;
		tme.dwHoverTime =1;
		m_bTracking = _TrackMouseEvent(&tme);
	}

	CRect rc;
	CPoint pnt;
	GetCursorPos(&pnt);

	ScreenToClient(&pnt);

	GetClientRect(&rc);

	
	if (rc.PtInRect(pnt))
	{
		SetCursor(mh_qcursor);
	}
	

	CWnd::OnMouseMove(nFlags, point);
}

LRESULT CControlwnd::OnMouseHover(WPARAM wparam, LPARAM lparam)
{
	m_bHover = true;
	InvalidateRect(&m_btnRect);
	//SetCursor(mh_qcursor);
	return 0;
}

LRESULT CControlwnd::OnMouseLeave(WPARAM wparam, LPARAM lparam)
{
	m_bTracking = false;
	m_bHover = false;
	InvalidateRect(&m_btnRect);
	m_dlgpop->ShowWindow(SW_HIDE);
	m_dlgTri->ShowWindow(SW_HIDE);
	return 0;
}


//void CControlwnd::OnSetFocus(CWnd* pOldWnd)
//{
//	CWnd::OnSetFocus(pOldWnd);
//
//	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
//}


BOOL CControlwnd::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	if(nHitTest == HTCLIENT)
	{
		CRect rc;
		CPoint pnt;
		GetCursorPos(&pnt);

		ScreenToClient(&pnt);

		GetClientRect(&rc);

	
		if (rc.PtInRect(pnt))
		{
			CRect mRc;
			GetWindowRect(mRc);
			SetCursor(mh_ncursor);
			//m_dlgpop->ShowWindow(SW_SHOW);
			if(!m_dlgpop->IsWindowVisible() && !m_dlgTri->IsWindowVisible())
			{
				m_dlgpop->SetData(m_data, 1,0);
				m_dlgpop->ShowSlide(mRc);
				CRect rc;
				rc.SetRect(mRc.left, mRc.bottom, mRc.left + mRc.Width()/2, mRc.bottom + 10);
				rc.OffsetRect(5, -10);
				m_dlgTri->SetWindowPos(&wndTop, rc.left, rc.bottom, rc.Width(), rc.Height(), SWP_NOACTIVATE|SWP_SHOWWINDOW);
				m_dlgTri->ShowWindow(SW_SHOW);
				m_dlgTri->Invalidate();
			}	
		}
		else
		{
			SetCursor(mh_qcursor);
			
		}
	}
	return CWnd::OnSetCursor(pWnd, nHitTest, message);
}


//void CControlwnd::OnDestroy()
//{
//	CWnd::OnDestroy();
//
//	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
//}


//void CControlwnd::OnTimer(UINT_PTR nIDEvent)
//{
//	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
//
//	CWnd::OnTimer(nIDEvent);
//}


void CControlwnd::Setdata(LPCTSTR data)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CString strResult;

	// TODO: 여기에 디스패치 처리기를 추가합니다.
	m_data.Format(_T("%s"), data);
	m_dlgpop->SetData(m_data, 1,0);

}


void CControlwnd::OnDestroy()
{
	if (m_dlgpop)	
	{
	//	m_dlgpop->ShowWindow(SW_SHOW);
	//	m_dlgpop->OnBnClickedCancel();
		delete m_dlgpop;
		m_dlgpop = NULL;
	}
	CWnd::OnDestroy();
	
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
}


void CControlwnd::SetVisible(LONG bVisible)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	if(bVisible == -1)
		ShowWindow(SW_SHOW);
	else
		ShowWindow(SW_HIDE);
	Invalidate();
	// TODO: 여기에 디스패치 처리기를 추가합니다.
}
