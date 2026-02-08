
#include "stdafx.h"
#include "IB0000X1.h"
#include "MapWnd.h"

#include <vector>
#include <string>
#include <algorithm>
using namespace std;

#include "../../h/jmcode.h"
#include "../../h/interSt.h"
#include "sys/stat.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

struct FIELD_INFO FIELD_INFO[] = {
	{ "1301", "301", "CODE"      , TRUE,  FALSE,  0 },
	{ "1021", "021", "RTS코드"   , TRUE,  FALSE,  0 },
	{ "2934", "934", "전일종가"  , TRUE,  FALSE,  0 },
	{ "2033", "033", "등락률"    , TRUE,  FALSE,  0 },
	{ "1022", "022", "종목명"    , TRUE,  TRUE,  80 },
	{ "2023", "023", "현재가"    , FALSE, TRUE,  55 },
	{ "2024", "024", "전일대비"  , FALSE, TRUE,  55 },
	{ "2033", "033", "등락률"    , FALSE, TRUE,  55 },
	{ "2027", "027", "거래량"    , FALSE, TRUE,  65 },
	{ "2029", "029", "시가"      , FALSE, TRUE,  55 },
	{ "2030", "030", "고가"      , FALSE, TRUE,  55 },
	{ "2031", "031", "저가"      , FALSE, TRUE,  55 },
	{ "3101", "101", "매도총잔량", FALSE, TRUE,  55 },
	{ "3106", "106", "매수총잔량", FALSE, TRUE,  55 },
	{ NULL, NULL, NULL, NULL, NULL, NULL }
};

CMapWnd::CMapWnd(CWnd *pParent) : CAxisExt(pParent)
{
	m_transparent = 50;
	m_bFlash = TRUE;
	m_bSimple = TRUE;
	m_bTracking = TRUE;
	m_bTopMost = TRUE;
	m_bExit = FALSE;
	m_pOldHoverWnd = NULL;
	m_vtype = VT_HORIZONTAL;
	m_newX = DEF_POPUP_POSX;
	m_newY = DEF_POPUP_POSY;
	m_crPopupBack = COLOR_DEF_POPUP_BACK;
	m_crPopupFlash = COLOR_DEF_POPUP_FLASH;
	
	m_pParent->SendMessage(WM_USER, MAKEWPARAM(sizeDLL, NULL), MAKELPARAM(400,440));

	LoadCodes();
}

CMapWnd::~CMapWnd()
{
}


BEGIN_MESSAGE_MAP(CMapWnd, CWnd)
	//{{AFX_MSG_MAP(CMapWnd)
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_WM_TIMER()
	ON_WM_HSCROLL()
	ON_WM_PAINT()
	ON_WM_CLOSE()
	ON_WM_CTLCOLOR()
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BTN_FLASH, OnFlash)
	ON_BN_CLICKED(IDC_BTN_VIEWTYPE, OnViewType)
	ON_BN_CLICKED(IDC_BTN_CODEADD, OnCodeAdd)
	ON_BN_CLICKED(IDC_BTN_BKCOLOR, OnBkColorChange)
	ON_BN_CLICKED(IDC_BTN_FLASHCOLOR, OnFlashColorChange)
	ON_BN_CLICKED(IDC_BTN_SIMPLECOLOR, OnSimpleColor)
	ON_BN_CLICKED(IDC_BTN_TOPMOST, OnTopMost)
	ON_BN_CLICKED(IDC_BTN_INTERADD, OnInterAdd)
	ON_BN_CLICKED(IDC_BTN_LASTRUN, OnLastView)
	ON_BN_CLICKED(IDC_BTN_CODENEWS, OnCodeNews)
	ON_CONTROL_RANGE(BN_CLICKED, IDC_BTN_FIELD_START, IDC_BTN_FIELD_START+100, OnFieldChange)
END_MESSAGE_MAP()

// LRESULT CMapWnd::OnUser( WPARAM wParam, LPARAM lParam )
// {
// 	int msg = LOBYTE(LOWORD(wParam));
// 	int key = HIBYTE(LOWORD(wParam));
// 	int len = HIWORD(wParam);
// 	LPCSTR dat = (LPCSTR)lParam;
// 	
// 	switch(msg)
// 	{
//  	case DLL_INB:
// 		break;
// 		
// 	case DLL_OUB:
// // 		if (key>=TR_STARTKEY)
// // 		{
// // 			ParseSise(dat, len);
// // 		}
// 		break;
// 		
// 	case DLL_ALERT:
// 		//ParseAlert(dat, strlen(dat));
// 		break;
// 		
// 	case DLL_TRIGGER:
// 		break;
// 		
// 	case DLL_NOTICE:
// 		break;
// 		
// 	case DLL_SETPAL:
// 	case DLL_SETFONT:
// 	case DLL_SETFONTx:
// 		break;
// 		
// 	case DLL_GUIDE:
// 		return true;
// 		
// 	case DLL_DOMINO:
// 		break;
// 	}
// 	return 0;
// }

void CMapWnd::AddCode( LPCSTR code, LPRECT pRC )
{
	//if (strlen(code)!=6) return;

// 	int size = m_codes.GetSize();
// 	for(int n=0; n<size; ++n)
// 	{
// 		if (m_codes[n]==code) 
// 			return;
// 	}
// 
// 	m_codes.Add(code);
}

void CMapWnd::DelCode( LPCSTR code )
{
	//if (strlen(code)!=6) return;

// 	int size = m_codes.GetSize();
// 	for(int n=0; n<size; ++n)
// 	{
// 		if (m_codes[n]==code) 
// 		{
// 			m_codes.RemoveAt(n);
// 			return;
// 		}
// 	}
}


int CMapWnd::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;


	LoadConfig();

	SetFont(m_pFont);

	{
		m_sbTransparent.Create(WS_CHILD|WS_VISIBLE, CRect(10, 10, 210, 30), this, IDC_SB_TRANSPARENT);
		m_sbTransparent.SetRange(0, 100, FALSE);
		m_sbTransparent.SetPos(m_transparent);
	}
	{
		m_btnFlash.Create("시세 수신시 깜박임 처리", WS_CHILD|WS_VISIBLE|BS_AUTOCHECKBOX, CRect(10, 35, 105, 55), this, IDC_BTN_FLASH);
		m_btnFlash.SetFont(m_pFont);
		m_btnFlash.SetCheck(m_bFlash);
	}
	{
		m_btnSimpleColor.Create("색상동기화", WS_CHILD|WS_VISIBLE|BS_AUTOCHECKBOX, CRect(105, 35, 210, 55), this, IDC_BTN_SIMPLECOLOR);
		m_btnSimpleColor.SetFont(m_pFont);
		m_btnSimpleColor.SetCheck(m_bSimple);

		m_btnSimpleColor.ShowWindow(SW_HIDE);
	}
	{
		m_btnViewType.Create("세로보기", WS_CHILD|WS_VISIBLE|BS_AUTOCHECKBOX, CRect(10, 60, 105, 80), this, IDC_BTN_VIEWTYPE);
		m_btnViewType.SetFont(m_pFont);
		m_btnViewType.SetCheck((m_vtype==VT_HORIZONTAL) ? FALSE : TRUE);
	}

	{
		m_btnLastRun.Create("시작시 마지막 조회종목 실행", WS_CHILD|WS_VISIBLE|BS_AUTOCHECKBOX, CRect(10, 60, 105, 80), this, IDC_BTN_LASTRUN);
		m_btnLastRun.SetFont(m_pFont);
		m_btnLastRun.SetCheck(m_bLastRun);
	}

	{
		m_btnNews.Create("종목뉴스 발생시 표시", WS_CHILD|WS_VISIBLE|BS_AUTOCHECKBOX, CRect(10, 60, 105, 80), this, IDC_BTN_CODENEWS);
		m_btnNews.SetFont(m_pFont);
		m_btnNews.SetCheck(m_bCodeNews);
	}
	{
		m_btnTopMost.Create("실행시 항상 맨위에 처리", WS_CHILD|WS_VISIBLE|BS_AUTOCHECKBOX, CRect(105, 60, 210, 80), this, IDC_BTN_TOPMOST);
		m_btnTopMost.SetFont(m_pFont);
		m_btnTopMost.SetCheck(m_bTopMost);
	}
	{
		m_edCode.Create(WS_CHILD|WS_VISIBLE|WS_BORDER, CRect(10, 85, 160, 105), this, IDC_EDIT_CODE);
		m_edCode.SetFont(m_pFontB);
		m_btnAdd.Create("추가", WS_CHILD|WS_VISIBLE, CRect(165, 85, 210, 105), this, IDC_BTN_CODEADD);
		m_btnAdd.SetFont(m_pFont);

		m_edCode.ShowWindow(SW_HIDE);
		m_btnAdd.ShowWindow(SW_HIDE);
	}
// 	{
// 		m_cbInterest.Create(WS_CHILD|WS_VISIBLE|CBS_DROPDOWNLIST|CBS_SIMPLE, CRect(10, 110, 160, 300), this, IDC_CB_INTEREST);
// 		m_cbInterest.SetFont(m_pFont);
// 		for(int n=0; n<m_gpnm.GetSize(); ++n)
// 			m_cbInterest.AddString(m_gpnm[n]);
// 		m_btnInterAdd.Create("추가", WS_CHILD|WS_VISIBLE, CRect(165, 110, 210, 130), this, IDC_BTN_INTERADD);
// 		m_btnInterAdd.SetFont(m_pFont);
// 	}
	{
		m_btnBkColor.Create("배경색상", WS_CHILD|WS_VISIBLE|BS_OWNERDRAW, CRect(10, 135, 110, 155), this, IDC_BTN_BKCOLOR);
		m_btnBkColor.SetFont(m_pFont);
		m_btnBkColor.SetColor(m_crPopupBack);

		m_btnFlashColor.Create("깜박임색상", WS_CHILD|WS_VISIBLE|BS_OWNERDRAW, CRect(120, 135, 210, 155), this, IDC_BTN_FLASHCOLOR);
		m_btnFlashColor.SetFont(m_pFont);
		m_btnFlashColor.SetColor(m_crPopupFlash);
		
	}
	{
		int n{}, ypos = 160;
		for(n=0; FIELD_INFO[n].name!=NULL; ++n)
		{
			m_btnFields[n].Create(FIELD_INFO[n].name, WS_CHILD|WS_VISIBLE|BS_AUTOCHECKBOX, CRect(10, ypos, 210, ypos+20), this, IDC_BTN_FIELD_START+n);
			m_btnFields[n].SetFont(m_pFont);
			if (FIELD_INFO[n].must) 
			{
				m_btnFields[n].SetCheck(TRUE);
				m_btnFields[n].EnableWindow(FALSE);
				m_btnFields[n].ShowWindow(SW_HIDE);
			}
			else
			{
				m_btnFields[n].SetCheck(FALSE);
			}
			ypos += 20;
		}
		for(n=0; n<m_fields.GetSize(); ++n)
		{
			const int idx = GetFieldIndex(m_fields[n]->rsym);
			if (idx>=0)
				m_btnFields[n].SetCheck(TRUE);
		}
	}

// 	m_pMiniWnd = FindWindow(NULL,"제목없음");
// 
// 	//SetWindowText("MINI_WIZ");
// 
// 	CString s="TEST";
// 	CString t;
// 	GetParent()->GetParent()->GetParent()->GetWindowText(t);
// 	s.Format("TITLE : %s\n",t);
// 	OutputDebugString(s);
// 
// 	if(!m_pMiniWnd)
// 	{
// 		GetParent()->GetParent()->GetParent()->DestroyWindow();
// 		return 0;
// 	}

	Variant(titleCC,"미니HTS설정");

	SetTimer(0, 50, NULL);
	
//	TestSetting();
	
	return 0;
}

void CMapWnd::RefreshWnd( LPCSTR key, CMapStringToString *pd )
{
	CSisePopup *pwnd;
	if (m_wnds.Lookup(key, pwnd))
		pwnd->UpdateData(pd);
}

// CSisePopup* CMapWnd::CreatePopup( LPCSTR code, LPRECT pRC )
// {
// 	BOOL bRet;
// 	CRect rc;
// 	CSisePopup *pwnd;
// 	
// 	if (m_wnds.Lookup(code, pwnd)) return pwnd;
// 	
// 	pwnd = new CSisePopup(m_pParent);
// 	if (pRC==NULL)
// 	{
// 		int nWidth  = GetSystemMetrics(SM_CXVIRTUALSCREEN);
// 		int nHeight = GetSystemMetrics(SM_CYVIRTUALSCREEN);
// 		switch(m_vtype)
// 		{
// 		case VT_HORIZONTAL:
// 			rc.SetRect(m_newX, m_newY, m_newX+GetPopupWidth(), m_newY+GetPopupHeight()); 
// 			m_newY += POPUP_HEIGHT;
// 			if (m_newY>nHeight)
// 			{
// 				m_newY = DEF_POPUP_POSY;
// 				m_newX += GetPopupWidth();
// 			}
// 			break;
// 		case VT_VERTICAL:
// 		default:
// 			rc.SetRect(m_newX, m_newY, m_newX+GetPopupWidth(), m_newY + GetPopupHeight());
// 			m_newX += MAX_FIELD_WIDTH;
// 			if (m_newX>nWidth)
// 			{
// 				m_newX = DEF_POPUP_POSX;
// 				m_newY += GetPopupHeight();
// 			}
// 			break;
// 		}
// 		bRet = pwnd->CreateEx(WS_EX_LAYERED|WS_EX_TOOLWINDOW, ::AfxRegisterWndClass(0), "", WS_POPUP|WS_VISIBLE|WS_CLIPCHILDREN, rc, 0, NULL);
// 	}
// 	else
// 	{
// 		pRC->right = pRC->left + GetPopupWidth();
// 		pRC->bottom = pRC->top + GetPopupHeight();
// 		bRet = pwnd->CreateEx(WS_EX_LAYERED|WS_EX_TOOLWINDOW, ::AfxRegisterWndClass(0), "", WS_POPUP|WS_VISIBLE|WS_CLIPCHILDREN, *pRC, 0, NULL);
// 		m_newX = pRC->left;
// 		m_newY = pRC->bottom;
// 	}
// 
// 	if (!bRet)
// 	{
// 		delete pwnd;
// 		return NULL;
// 	}
// 
// 	if (m_bTopMost)	pwnd->SetWindowPos(&wndTopMost, 0, 0, 0, 0, SWP_NOMOVE|SWP_NOSIZE);
// 	else			pwnd->SetWindowPos(&wndNoTopMost, 0, 0, 0, 0, SWP_NOMOVE|SWP_NOSIZE);
// 	pwnd->SetTransparent(m_transparent);
// 	pwnd->SetCode(code);
// 	pwnd->SetFields(&m_fields, m_vtype);
// 	pwnd->SetSearchData(&m_search);
// 	pwnd->SetOwner(this);
// 	pwnd->SetFlash(m_bFlash);
// 	pwnd->SetBkColor(m_crPopupBack);
// 	pwnd->SetFlashColor(m_crPopupFlash);
// 
// 	m_wnds.SetAt(code, pwnd);
// 
// 	return pwnd;
// }

void CMapWnd::OnDestroy() 
{
	m_pOldHoverWnd = NULL;
	m_bExit = TRUE;
	KillTimer(0);

	{
		MSG msg;
		while(PeekMessage(&msg, m_hWnd, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	SaveConfig();

	POSITION pos{};

	{
		CString key;
		CSisePopup* val{};
		pos = m_wnds.GetStartPosition();
		while(pos!=NULL)
		{
			m_wnds.GetNextAssoc(pos, key, val);
			val->DestroyWindow();
			delete val;
		}
		m_wnds.RemoveAll();
	}

	{
		CString key;
		CMapStringToString* val{};
		pos = m_datas.GetStartPosition();
		while(pos!=NULL) 
		{
			m_datas.GetNextAssoc(pos, key, val);
			delete val;
		}
		m_datas.RemoveAll();
	}

	// do not delete pointer, pointer is defined unique_ptr
	{
		CString key;
		CStringArray* val{};	
		pos = m_search.GetStartPosition();
		while(pos!=NULL)
		{
			m_search.GetNextAssoc(pos, key, val);
//			delete val;
		}
	}

	CWnd::OnDestroy();
}

struct FIELD_INFO * CMapWnd::GetFieldInfo( LPCSTR rsym )
{
	for(int n=0; FIELD_INFO[n].name!=NULL; ++n)
	{
		if (strcmp(FIELD_INFO[n].rsym, rsym)==0) return &FIELD_INFO[n];
	}
	return NULL;
}

int CMapWnd::GetFieldIndex( LPCSTR rsym )
{
	for(int n=0; FIELD_INFO[n].name!=NULL; ++n)
	{
		if (strcmp(FIELD_INFO[n].rsym, rsym)==0) return n;
	}
	return -1;
}

DWORD CMapWnd::GetPopupWidth()
{
	if (m_vtype == VT_HORIZONTAL)
	{
		DWORD size = 0;
		const DWORD fcnt = m_fields.GetSize();
		for(DWORD n=0; n<fcnt; ++n)
			size += m_fields[n]->width;
		return size;
	}
	else if (m_vtype == VT_VERTICAL)
	{
		return MAX_FIELD_WIDTH;
	}
	else 
	{
		return 0;
	}
}


DWORD CMapWnd::GetPopupHeight()
{
	if (m_vtype == VT_HORIZONTAL)
	{
		return POPUP_HEIGHT;
	}
	else if (m_vtype == VT_VERTICAL)
	{
		DWORD size = 0;
		const DWORD fcnt = m_fields.GetSize();
		for(DWORD n=0; n<fcnt; ++n)
			size += m_fields[n]->width ? POPUP_HEIGHT : 0;
		return size;
	}
	else 
	{
		return 0;
	}
}

CSisePopup* CMapWnd::FindPopup( LPCSTR code )
{
	CSisePopup *pwnd;
	if (!m_wnds.Lookup(code, pwnd)) pwnd = NULL;
	return pwnd;
}

void CMapWnd::ChangeTransparent( int val )
{
	if (val<0) return;
	if (val>100) return;
	if (m_transparent != val)	
	{
		m_transparent = val;

		CString key;
		CSisePopup* wnd{};
		POSITION pos = m_wnds.GetStartPosition();
		while(pos!=NULL)
		{
			m_wnds.GetNextAssoc(pos, key, wnd);
			if (wnd) CLayer::SetTransparent(wnd, val);
		}
	}
}

void CMapWnd::OnTimer(UINT nIDEvent) 
{
	if (m_bExit) return;

	CRect rc;
	CString key;
	CSisePopup* pwnd{};
	const DWORD tick = GetTickCount();
	POSITION pos = m_wnds.GetStartPosition();
	while(pos)
	{
		m_wnds.GetNextAssoc(pos, key, pwnd);
		if(pwnd)
		pwnd->UpdateFlash(tick);
	}

	if (m_bTracking && !GetAsyncKeyState(VK_LBUTTON) && !GetAsyncKeyState(VK_RBUTTON))
	{
		CPoint pt;
		GetCursorPos(&pt);
		pos = m_wnds.GetStartPosition();
		while(pos)
		{
			m_wnds.GetNextAssoc(pos, key, pwnd);
			if (pwnd)
			{
				pwnd->GetWindowRect(&rc);
				if (rc.PtInRect(pt))
				{
					if (pwnd != m_pOldHoverWnd)
					{
						if (m_pOldHoverWnd) { m_pOldHoverWnd->LeaveWindow(); }
						if (pwnd) { pwnd->HoverWindow(); m_pOldHoverWnd = pwnd; }
					}
					return;
				}
			}
		}
		
		if (m_pOldHoverWnd) 
		{
			m_pOldHoverWnd->LeaveWindow();
			m_pOldHoverWnd = NULL;
		}
	}

	CWnd::OnTimer(nIDEvent);
}

void CMapWnd::DeletePopup( CSisePopup *pwnd )
{
	CString key;
	CSisePopup* val{};
	CMapStringToString* val2{};
	POSITION pos;

	pos = m_wnds.GetStartPosition();
	while(pos!=NULL)
	{
		m_wnds.GetNextAssoc(pos, key, val);
		if (val==pwnd)
		{
			m_wnds.RemoveKey(key);

			pwnd->DestroyWindow();
			delete pwnd;

			DelCode(key);
			if (m_datas.Lookup(key, val2))
			{
				delete val2;
				m_datas.RemoveKey(key);
			}
			break;
		}
	}
}

void CMapWnd::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	if (pScrollBar->GetDlgCtrlID()==IDC_SB_TRANSPARENT)
	{
		m_transparent = m_sbTransparent.GetPos();

		CString data;
		
		data.Format("$TRANS\t%d",m_transparent);
		
		Variant(triggerCC,data);
	}
	
	CWnd::OnHScroll(nSBCode, nPos, pScrollBar);
}

void CMapWnd::OnFlash()
{	
	m_bFlash = m_btnFlash.GetCheck()!=0;

	//m_pMiniWnd->SendMessage(WF_FLASH,(WPARAM)0,(LPARAM)m_bFlash);
	CString data;

	data.Format("$FLASH\t%d",m_bFlash);

	Variant(triggerCC,data);
}

void CMapWnd::OnPaint() 
{
	CPaintDC memDC(this); // device context for painting

	SetBkColor(memDC,RGB(255,255,255));

	CRect rc;
	GetClientRect(&rc);

	CFont *pOldFont = memDC.SelectObject(m_pFontB);

	memDC.FillSolidRect(&rc, m_crBodyBk);

	//memDC.FillSolidRect(10,10,380,20,RGB(43,63,249));
	DrawGradient(&memDC,CRect(10,10,390,30),RGB(41,148,243),RGB(255,255,255),true);

	memDC.SetBkColor(RGB(41,148,243));
	memDC.SetTextColor(RGB(255,255,255));
	memDC.DrawText( "기본 설정", CRect(20,10,390,30), DT_SINGLELINE|DT_VCENTER|DT_LEFT|DT_NOPREFIX );

	//memDC.FillSolidRect(10,160,380,20,RGB(255,125,100));
	DrawGradient(&memDC,CRect(10,160,390,180),RGB(41,148,243),RGB(255,255,255),true);

	memDC.DrawText( "색상 선택", CRect(20,160,390,180), DT_SINGLELINE|DT_VCENTER|DT_LEFT|DT_NOPREFIX );

	//memDC.FillSolidRect(10,210,380,20,RGB(255,125,100));
	DrawGradient(&memDC,CRect(10,210,390,230),RGB(41,148,243),RGB(255,255,255),true);

	memDC.DrawText( "투명도 조절", CRect(20,210,390,230), DT_SINGLELINE|DT_VCENTER|DT_LEFT|DT_NOPREFIX );

	memDC.SetBkColor(m_crBodyBk);
	memDC.SetTextColor(RGB(0,0,0));
	memDC.DrawText( "투 명", CRect(35,255,95,275), DT_SINGLELINE|DT_VCENTER|DT_NOPREFIX );

	memDC.DrawText( "불 투 명", CRect(330,255,390,275), DT_SINGLELINE|DT_VCENTER|DT_NOPREFIX );

	memDC.DrawText( "참 고 : 투명 100% 사용시 바탕화면에 완전 감추기 가능", CRect(35,290,355,310), DT_SINGLELINE|DT_VCENTER|DT_NOPREFIX );

	//memDC.FillSolidRect(10,325,380,20,RGB(255,125,100));
	DrawGradient(&memDC,CRect(10,325,390,345),RGB(41,148,243),RGB(255,255,255),true);

	memDC.SetBkColor(RGB(41,148,243));
	memDC.SetTextColor(RGB(255,255,255));
	memDC.DrawText( "필드 편집", CRect(20,325,390,345), DT_SINGLELINE|DT_VCENTER|DT_LEFT|DT_NOPREFIX );

	memDC.SelectObject(pOldFont);

	// 2022.07.01 - 오류확인 필요
	//memDC.DeleteDC();
}

void CMapWnd::DrawGradient(CDC *m_dc,CRect drawRC, COLORREF sColor, COLORREF eColor, bool bRight)
{
	CRect	rect;
	int	nWidth{}, nHeight{};

	drawRC.right += 1;
	drawRC.bottom += 1;
	nWidth = drawRC.Width(); 
	nHeight = drawRC.Height();

	int rs{}, gs{}, bs{};
	rs = GetRValue(sColor);
	gs = GetGValue(sColor);
	bs = GetBValue(sColor);

	int re{}, ge{}, be{};
	re = GetRValue(eColor);
	ge = GetGValue(eColor);
	be = GetBValue(eColor);

	float	rStep{}, gStep{}, bStep{};
	CPen* sPen{};
	if (bRight)
	{
		if (!nWidth)
		{
			CPen	cPen(PS_SOLID, 1, sColor);
			sPen = m_dc->SelectObject(&cPen);
			
			m_dc->MoveTo(drawRC.left, drawRC.top);
			m_dc->LineTo(drawRC.right, drawRC.bottom);
			m_dc->SelectObject(sPen);
			return;
		}

		rStep = float(rs - re)/float(nWidth);
		gStep = float(gs - ge)/float(nWidth);
		bStep = float(bs - be)/float(nWidth);

		if (!nHeight)
		{
			for (int ii = 0; ii < nWidth; ++ii)
			{
				CPen	cPen(PS_SOLID, 1, RGB(rs - rStep*ii, gs - gStep*ii, bs - bStep*ii));
				sPen = m_dc->SelectObject(&cPen);

				m_dc->MoveTo(drawRC.left + ii, drawRC.top);
				m_dc->LineTo(drawRC.left + ii + 1, drawRC.top);
				m_dc->SelectObject(sPen);
			}
		}
		else
		{
			for (int ii = 0; ii < nWidth; ++ii)
			{
				rect.SetRect(drawRC.left + ii, drawRC.top, drawRC.left + ii + 1, drawRC.bottom);
				
				CBrush brush;
				brush.CreateSolidBrush(RGB(rs - rStep*ii, gs - gStep*ii, bs - bStep*ii));
				m_dc->FillRect(&rect, &brush);
				brush.DeleteObject();
			}
		}
	}
	else
	{
		if (!nHeight)
		{
			CPen	cPen(PS_SOLID, 1, sColor);
			sPen = m_dc->SelectObject(&cPen);
			
			m_dc->MoveTo(drawRC.left, drawRC.top);
			m_dc->LineTo(drawRC.right, drawRC.bottom);
			m_dc->SelectObject(sPen);
			return;
		}

		rStep = float(rs - re)/float(nHeight);
		gStep = float(gs - ge)/float(nHeight);
		bStep = float(bs - be)/float(nHeight);

		if (!nWidth)
		{
			for (int ii = 0; ii < nHeight; ++ii)
			{
				CPen	cPen(PS_SOLID, 1, RGB(rs - rStep*ii, gs - gStep*ii, bs - bStep*ii));
				sPen = m_dc->SelectObject(&cPen);

				m_dc->MoveTo(drawRC.left, drawRC.top + ii);
				m_dc->LineTo(drawRC.left, drawRC.top + ii + 1);
				m_dc->SelectObject(sPen);
			}
		}
		else
		{
			for (int ii = 0; ii < nHeight; ++ii)
			{
				rect.SetRect(drawRC.left, drawRC.top + ii, drawRC.right, drawRC.top + ii + 1);
				
				CBrush brush;
				brush.CreateSolidBrush(RGB(rs - rStep*ii, gs - gStep*ii, bs - bStep*ii));
				m_dc->FillRect(&rect, &brush);
				brush.DeleteObject();
			}
		}
	}
}

// int CMapWnd::DrawBitmap(CDC* pDC, int index, CRect bRC, bool bDown)
// {
// 	if (bRC.IsRectEmpty())	return 0;
// 	return DrawBitmapByMask(pDC, index, bRC, bDown);
// 	
// 	CDC	memDC;
// 	if (!memDC.CreateCompatibleDC(pDC))
// 		return 0;
// 	
// 	BITMAP	bm;
// 	CBitmap* bitmap	= GetButtonImage(index);
// 	CBitmap* pbitmap= memDC.SelectObject(bitmap);
// 	bitmap->GetBitmap(&bm);
// 	
// 	if (pbitmap == (CBitmap *) 0)
// 	{
// 		memDC.DeleteDC();
// 		return 0 ;
// 	}
// 	
// 	pDC->BitBlt(bRC.left, bRC.top, bx, by, &memDC, sx, 0, SRCCOPY);
// 	memDC.SelectObject(pbitmap);
// 	memDC.DeleteDC();
// 	
// 	return bx;
// }

void CMapWnd::OnCodeAdd()
{
	CString code;
	m_edCode.GetWindowText(code);

	//m_pMiniWnd->SendMessage(WD_ADD_CODE,(WPARAM)0,(LPARAM)(LPCSTR)code);
	CString data;
	
	data.Format("$ADDCODE\t%s",code);
	
	Variant(triggerCC,data);
}

void CMapWnd::OnFieldChange(UINT nID)
{
	 //m_pMiniWnd->SendMessage(WF_FIELDCHANGE,(WPARAM)0,(LPARAM)nID);
	CString data;
	
	data.Format("$FIELD\t%d",nID);
	
	Variant(triggerCC,data);
}

void CMapWnd::AlignCustom( struct FIELD_INFO *fi, CSisePopup *pbase, bool bUpper, VIEW_TYPE vtype )
{
	if (!fi || !pbase) return;

	if (!fi->sort) return;

	vector<SORT_OBJECT> vSort;
	
	if ( strcmp(fi->rsym, "022") == 0 )	// 종목명 정렬
	{
		CString key;
		CMapStringToString* val = nullptr;
		POSITION pos = m_datas.GetStartPosition();
		while(pos!=nullptr)
		{
			m_datas.GetNextAssoc(pos, key, val);
			vSort.push_back( SORT_OBJECT(key, (*val)[fi->rsym]) );
		}
		sort(vSort.begin(), vSort.end(), struct stringCompare(bUpper));	
	}
	else									// 숫자형
	{
		CString key;
		CMapStringToString* val{};
		POSITION pos = m_datas.GetStartPosition();
		while(pos!=NULL)
		{
			m_datas.GetNextAssoc(pos, key, val);
			vSort.push_back( SORT_OBJECT(key, (*val)[fi->rsym]) );
		}
		
		if ( strcmp(fi->rsym, "033")==0 )		// 등락률은 -가 있어!!
			sort(vSort.begin(), vSort.end(), struct priceCompare(bUpper, false));
		else
			sort(vSort.begin(), vSort.end(), struct priceCompare(bUpper, true));
	}
	
	CRect rc;
	pbase->GetWindowRect(rc);
	if (vSort.size() == m_datas.GetCount())
	{
		CSisePopup* pwnd{};

		const int nWidth  = GetSystemMetrics(SM_CXVIRTUALSCREEN);
		const int nHeight = GetSystemMetrics(SM_CYVIRTUALSCREEN);
		const int nBaseX = rc.left;
		const int nBaseY = rc.top;
		const int cx = rc.Width();
		const int cy = rc.Height();

		for(size_t n=0; n<vSort.size(); ++n)
		{
			if (m_wnds.Lookup(vSort[n].first, pwnd))
			{
				pwnd->MoveWindow(&rc, TRUE);
				if (vtype==VT_HORIZONTAL)
				{
					rc.OffsetRect(cx, 0);
					if (rc.right>nWidth)
					{
						rc.OffsetRect(0, cy);
						rc.left = nBaseX;
						rc.right = nBaseX + cx;
					}
						
				}
				else 
				{
					rc.OffsetRect(0, cy);
					if (rc.bottom>nHeight)
					{
						rc.OffsetRect(cx, 0);
						rc.top = nBaseY;
						rc.bottom = nBaseY + cy;
					}
				}
			}
		}
	}
}

void CMapWnd::SaveConfig()
{
	LPCSTR sct = "MINIPOPUP";
	CString path, val;
	path.Format("%s\\user\\%s\\userconf.ini", Variant(homeCC,""), Variant(nameCC,""));

	int n = 0;
	CRect rc;
	CString key;
	CSisePopup* pwnd{};
	POSITION pos{};
	
	val = "";
	for(n=0; n<m_fields.GetSize(); ++n)
	{
		if (m_fields[n]->must) continue;
		val += CString(m_fields[n]->rsym) + ",";
	}
	if (val.GetLength()>0) val.Delete(val.GetLength()-1);	// "마지막 콤마 없애기
	WritePrivateProfileString(sct, "FIELDS", val, path);


	val.Format("%d", m_wnds.GetCount());
	WritePrivateProfileString(sct, "SCOUNT", val, path);
	
	n = 0;
	pos = m_wnds.GetStartPosition();
	while(pos!=NULL)
	{
		m_wnds.GetNextAssoc(pos, key, pwnd);
		pwnd->GetWindowRect(&rc);
		val.Format("%s,%d,%d,%d,%d", key, rc.left, rc.top, rc.right, rc.bottom);
		key.Format("S%03d", ++n);
		WritePrivateProfileString(sct, key, val, path);
	}

	val.Format("%d", m_transparent);
	WritePrivateProfileString(sct, "TRANSPARENT", val, path);
	val.Format("%d", m_bFlash);
	WritePrivateProfileString(sct, "FLASH", val, path);
 	val.Format("%d", m_bSimple);
 	WritePrivateProfileString(sct, "SIMPLE_COLOR", val, path);
	val.Format("%d", m_crPopupBack);
	WritePrivateProfileString(sct, "POP_BK_COLOR", val, path);
	val.Format("%d", m_crPopupFlash);
	WritePrivateProfileString(sct, "POP_FL_COLOR", val, path);
	val.Format("%d", (int)m_vtype);
	WritePrivateProfileString(sct, "VIEW_TYPE", val, path);
	val.Format("%d", m_bTopMost);
 	WritePrivateProfileString(sct, "TOPMOST", val, path);
	val.Format("%d",m_bLastRun);
	WritePrivateProfileString(sct, "LASTVIEW", val, path);
	val.Format("%d",m_bCodeNews);
	WritePrivateProfileString(sct, "CODENEWS", val, path);

//	OutputDebugString("SAVECNFIG");
}

void CMapWnd::LoadConfig()
{
	int n{}, m{};
	char buff[1024]{};
//	CRect rc{};
	LPCSTR sct = "MINIPOPUP";
	CString path{}, key{}, val{};
	path.Format("%s\\user\\%s\\userconf.ini", Variant(homeCC,""), Variant(nameCC,""));

	m_fields.RemoveAll();
	m_codes.RemoveAll();

	// 기본필드 세팅
	for(n=0; FIELD_INFO[n].name!=NULL; ++n)
	{
		if (FIELD_INFO[n].must)	
			m_fields.Add(&FIELD_INFO[n]);
	}

	vector<StringProxy> fields;
	GetPrivateProfileString(sct, "FIELDS", "022,023,024,033,027", buff, sizeof(buff), path);
	ParseString(buff, buff+strlen(buff), ',', fields);
	for(size_t n=0; n<fields.size(); ++n)
	{
		for(m=0; FIELD_INFO[m].name!=NULL; ++m)
		{
			if (fields[n].ToCStr()==FIELD_INFO[m].rsym && !FIELD_INFO[m].must)
			{
				m_fields.Add(&FIELD_INFO[m]);
				break;
			}
		}
	}

	m_transparent = GetPrivateProfileInt(sct, "TRANSPARENT", 80, path);
	m_bFlash = GetPrivateProfileInt(sct, "FLASH", TRUE, path);
	m_bSimple = GetPrivateProfileInt(sct, "SIMPLE_COLOR", TRUE, path);
	m_crPopupBack = GetPrivateProfileInt(sct, "POP_BK_COLOR", COLOR_DEF_POPUP_BACK, path);
	m_crPopupFlash = GetPrivateProfileInt(sct, "POP_FL_COLOR", COLOR_DEF_POPUP_FLASH, path);
	m_vtype = (VIEW_TYPE)GetPrivateProfileInt(sct, "VIEW_TYPE", VT_HORIZONTAL, path);
	m_bTopMost = GetPrivateProfileInt(sct, "TOPMOST", m_bTopMost, path);
	m_bLastRun = GetPrivateProfileInt(sct, "LASTVIEW", 0, path);
	m_bCodeNews = GetPrivateProfileInt(sct, "CODENEWS", 1, path);

// 	int scnt = GetPrivateProfileInt(sct, "SCOUNT", 0, path);
// 	for(n=0; n<scnt; ++n)
// 	{
// 		key.Format("S%03d", n+1);
// 		GetPrivateProfileString(sct, key, "", buff, sizeof(buff), path);
// 		ParseString(buff, buff+strlen(buff), ',', fields);
// 		if (fields.size()==5)	// 종목코드, left, top, right, bottom
// 		{
// 			CString code = fields[0].ToCStr();
// 			rc.SetRect(atoi(fields[1].ToCStr()),atoi(fields[2].ToCStr()),atoi(fields[3].ToCStr()),atoi(fields[4].ToCStr()));
// 			AddCode(code, &rc);
// 		}
// 	}
}
 
void CMapWnd::AddSearchData( const CString &code, const CString &name )
{
	CStringArray* obj1{}, * obj2{};
	CString val, key;
	
	key = name.Left( IsDBCSLeadByte(name[0]) ? 2 : 1 ); 	// 한글이면 2글자, 영문이면 1글자
	if (!m_search.Lookup(key, obj1))
	{

		auto& item = _vSearch.emplace_back(std::make_unique<CStringArray>());
		obj1 = item.get();
		m_search.SetAt(key, obj1);
	}
	val.Format("%s|%s@", name, code);
	obj1->Add(val);
	
	key = code.Left(1);
	if (!m_search.Lookup(key, obj2))
	{
		auto& item = _vSearch.emplace_back(std::make_unique<CStringArray>());
		obj2 = item.get();
		m_search.SetAt(key, obj2);
	}
	val.Format("%s|%s#", code, name);
	obj2->Add(val);
}

void CMapWnd::LoadCodes()
{
	vector<char> buff;
	CString path;
	
	// 업종
	{
		path.Format("%s\\tab\\upcode.dat", Variant(homeCC, ""));
		ReadFile( path, buff );
		struct upcode *st = (struct upcode *)&buff[0];
		const struct upcode *ed = st + buff.size()/sizeof(struct upcode);
		for(; st<ed; ++st)
		{
			CString code, name;
			code.Format("%d%02d", st->jgub, st->ucod);
			name.Format("%.*s", min(strlen(st->hnam),UNameLen), st->hnam); name.Replace(" ", "");
			AdjustHangul(name);
//			TRACE("[%s][%s]\n", code, name);
			AddSearchData(code, name);
		}
	}

	// 현물
	{
		path.Format("%s\\tab\\hjcode3.dat", Variant(homeCC, ""));
		ReadFile( path, buff );
		struct hjcodex *st = (struct hjcodex *)&buff[0];
		const struct hjcodex *ed = st + buff.size()/sizeof(struct hjcodex);
		for(; st<ed; ++st)
		{
			CString code(&st->code[0], sizeof(st->code));
			CString name(&st->hnam[1], sizeof(st->hnam)-1);
			code.TrimRight();
			name.TrimRight();
			AdjustHangul(name);
			if (strlen(code)==7)
			{
				if (code[0]=='A' || code[0]=='J')
					code = code.Mid(1, 6);
			}
			AddSearchData(code, name);
		}
	}

	// 선물
	{
		path.Format("%s\\tab\\fjcode.dat", Variant(homeCC, ""));
		ReadFile( path, buff );
		const struct fjcode *st = (struct fjcode *)&buff[0];
		const struct fjcode *ed = st + buff.size()/sizeof(struct fjcode);
		for(; st<ed; ++st)
		{
			CString code(st->cod2, sizeof(st->cod2));
			CString name = st->hnam;
			AdjustHangul(name);
			AddSearchData(code, name);
		}
	}

	// 옵션
	{
		path.Format("%s\\tab\\opcode.dat", Variant(homeCC, ""));
		ReadFile( path, buff );
		buff.erase(buff.begin(), buff.begin()+sizeof(struct ojcodh));
		const struct ojcode *st = (struct ojcode *)&buff[0];
		const struct ojcode *ed = st + buff.size()/sizeof(struct ojcode);
		for(; st<ed; ++st)
		{
			for(int n=0; n<4; ++n)
			{
				if (st->call[n].yorn=='1')
				{
					CString code(st->call[n].cod2, OCodeLen);
					CString name(st->call[n].hnam);
					AdjustHangul(name);
					AddSearchData(code, name);
				}
				if (st->put[n].yorn=='1')
				{
					CString code(st->put[n].cod2, OCodeLen);
					CString name(st->put[n].hnam, ONameLen); name.TrimRight();
					AdjustHangul(name);
					AddSearchData(code, name);
				}
			}
		}
	}
}

void CMapWnd::ReadFile( LPCSTR path, vector<char> &dst )
{
	dst.clear();

	FILE *fp = fopen(path, "rb");
	if (!fp) return;

	struct stat st;
	fstat(fileno(fp), &st);
	
	dst.resize(st.st_size);
	int tlen = 0;
	while(tlen < st.st_size)
		tlen += fread(&dst[tlen], 1, st.st_size-tlen, fp);
	fclose(fp);
}

void CMapWnd::OnViewType()
{
	const BOOL bCheck = m_btnViewType.GetCheck();
	
	//m_pMiniWnd->SendMessage(WF_VIEWTYPE,(WPARAM)0,(LPARAM)bCheck);
	CString data;
	
	data.Format("$VIEWTYPE\t%d",bCheck);
	
	Variant(triggerCC,data);
}

void CMapWnd::OnBkColorChange()
{
	CColorDialog dlg(m_crPopupBack, 0, this);
	if (dlg.DoModal()==IDOK)
	{
		m_crPopupBack = dlg.GetColor();
		m_btnBkColor.SetColor(m_crPopupBack);
		
		//m_pMiniWnd->SendMessage(WF_BKCOLOR,(WPARAM)0,(LPARAM)m_crPopupBack)	;
		CString data;
		
		data.Format("$BKCOLOR\t%d",m_crPopupBack);
		
		Variant(triggerCC,data);
	}
}

void CMapWnd::OnFlashColorChange()
{
	CColorDialog dlg(m_crPopupFlash, 0, this);
	if (dlg.DoModal()==IDOK)
	{
		m_crPopupFlash = dlg.GetColor();
		m_btnFlashColor.SetColor(m_crPopupFlash);
		
		//m_pMiniWnd->SendMessage(WF_FLASHCOLOR,(WPARAM)0,(LPARAM)m_crPopupFlash);
		CString data;
		
		data.Format("$FLASHCOLOR\t%d",m_crPopupFlash);
		
		Variant(triggerCC,data);
	}
}

void CMapWnd::AdjustHangul( CString &text )
{
	BOOL bLead = FALSE;
	const int nSize = text.GetLength();
	for(int n=0; n<nSize; ++n)
	{
		if (IsDBCSLeadByte(text[n]) && !bLead)
		{
			bLead = TRUE;
		}
		else
		{
			bLead = FALSE;
		}
	}
	if (bLead) text.Delete(nSize-1);
}

void CMapWnd::OnSimpleColor()
{
	const BOOL bSimple = m_btnSimpleColor.GetCheck();

	if (m_bSimple!=bSimple)
	{
		//m_pMiniWnd->SendMessage(WF_SIMPLECOLOR,(WPARAM)0,(LPARAM)m_bSimple);
		m_bSimple = bSimple;

		CString data;
		
		data.Format("$SIMPLECOLOR\t%d",bSimple);
		
		Variant(triggerCC,data);
	}
}

void CMapWnd::OnTopMost()
{
	const BOOL bTopMost = m_btnTopMost.GetCheck();
	
	if (m_bTopMost!=bTopMost)
	{
		//m_pMiniWnd->SendMessage(WF_TOPMOST,(WPARAM)0,(LPARAM)bTopMost);
		m_bTopMost = bTopMost;

		CString data;
		
		data.Format("$TOPMOST\t%d",m_bTopMost);
		
		Variant(triggerCC,data);
	}
}


void CMapWnd::OnClose() 
{
	m_pOldHoverWnd = NULL;
	m_bExit = TRUE;
	KillTimer(0);
	CWnd::OnClose();
}

void CMapWnd::OnInterAdd()
{
// 	int gpno;
// 	CString path;
// 	
// 	gpno = m_cbInterest.GetCurSel();
// 	if (gpno<0) return;
// 
// 	path.Format("%s\\user\\%s\\portfolio.i%s", Variant(homeCC,""), Variant(nameCC,""), m_gpno[gpno]);
// 
// 	FILE *fp = fopen(path, "rb");
// 	if (!fp) return;
// 
// 	struct _inters inters;
// 	while( fread(&inters, 1, sizeof(inters), fp) == sizeof(inters) )
// 	{
// 		CString code(inters.code, sizeof(inters.code));
// 		code.Remove(0x00); code.Remove(' ');
// 		if (code.GetLength()>=3 && code.GetLength()<=8 && inters.gubn[0]!='m')
// 		{
// 			AddCode(code);
// 		}
// 	}
// 	RequestCode();
// 	fclose(fp);
}

HBRUSH CMapWnd::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CWnd::OnCtlColor(pDC, pWnd, nCtlColor);
	
	// TODO: Change any attributes of the DC here
	if (nCtlColor == CTLCOLOR_DLG || nCtlColor == CTLCOLOR_STATIC)
	{
		pDC->SetBkMode(TRANSPARENT);
		HBRUSH nhbr = CreateSolidBrush(RGB(255,255,255));
		return nhbr;
	}
	
	// TODO: Return a different brush if the default is not desired
	
	return hbr;
}

void CMapWnd::OnSize(UINT nType, int cx, int cy) 
{
	CWnd::OnSize(nType, cx, cy);
	
	int x=0,y=0;

	x = 15;
	y = 35;

	m_btnLastRun.MoveWindow(x,y,300,20,FALSE);

	y += 25;

	m_btnFlash.MoveWindow(x,y,300,20,FALSE);

	y += 25;

	m_btnNews.MoveWindow(x,y,300,20,FALSE);

	y += 25;

	m_btnTopMost.MoveWindow(x,y,300,20,FALSE);

	y += 25;

	m_btnViewType.MoveWindow(x,y,300,20,FALSE);

	y += 50;

	m_btnBkColor.MoveWindow(x,y,100,20,FALSE);

	m_btnFlashColor.MoveWindow(x + 105,y,90,20,FALSE);

	y += 70;

	m_sbTransparent.MoveWindow(x + 60,y,250,20,FALSE);
	
	y += 95;
	
	int nindex = 1;

	x = 25;

	for(int n=5; FIELD_INFO[n].name!=NULL; ++n)
	{
		CString s;
		s.Format("FIELD : %d\n",n);
//		OutputDebugString(s);
		m_btnFields[n].MoveWindow(x,y,120,20,FALSE);

		x += 125;

		if((nindex%3) == 0)
		{
			nindex = 1;
			x = 25;
			y += 25;
		}
		else
		{
			nindex++;
		}
	}
}

void CMapWnd::OnLastView()
{
	const BOOL bLastRun = m_btnLastRun.GetCheck();
	
	if (m_bLastRun != bLastRun)
	{
		//m_pMiniWnd->SendMessage(WF_TOPMOST,(WPARAM)0,(LPARAM)bTopMost);
		m_bLastRun = bLastRun;
		
		CString data;
		
		data.Format("$LASTVIEW\t%d",m_bLastRun);
		
		Variant(triggerCC,data);
	}
}

void CMapWnd::OnCodeNews()
{
	const BOOL bCodeNews = m_btnNews.GetCheck();
	
	if (m_bCodeNews != bCodeNews)
	{
		//m_pMiniWnd->SendMessage(WF_TOPMOST,(WPARAM)0,(LPARAM)bTopMost);
		m_bCodeNews = bCodeNews;
		
		CString data;
		
		data.Format("$CODENEWS\t%d",m_bCodeNews);
		
		Variant(triggerCC,data);
	}
}
