// DllCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "IB310600.h"
#include "DllCtrl.h"
#include "ScreenWnd.h"
#include "MapWnd.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDllCtrl

CDllCtrl::CDllCtrl(class CScreenWnd *pParent)
	:m_pScreenWnd(pParent)
{
	m_pMapWnd = m_pScreenWnd->GetMapWnd();
	m_pFont = m_pMapWnd->GetFont();

	m_nkey = 0;

	m_pBtnUp = NULL;
	m_pScreen = NULL;

	m_bBtnState = FALSE;
	m_bCreated = FALSE;

	m_nHeight= 180;
}

CDllCtrl::~CDllCtrl()
{
	SAFE_DELETE(m_pBtnUp);
	CloseMap();
	SAFE_DELETE(m_pScreen);
}


BEGIN_MESSAGE_MAP(CDllCtrl, CWnd)
	//{{AFX_MSG_MAP(CDllCtrl)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CDllCtrl message handlers

int CDllCtrl::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	CreateControl();
	
	return 0;
}

void CDllCtrl::CreateControl()
{
	CRect mapRc, cRc;
	GetClientRect(mapRc);
	cRc.left = GAP_CTRLS + GAP_SIDE;
	cRc.top	= GAP_TOP;
	cRc.right = cRc.left + CTRL_HEIGHT;
	cRc.bottom = cRc.top + CTRL_HEIGHT;
	m_pBtnUp = std::make_unique<CfxImgButton>();
	m_pBtnUp->Create("\n확장/축소", cRc, this, IDC_BTN_UP);
	m_pBtnUp->SetDrawFocus();	
	HBITMAP hBmpBtnUp = m_pMapWnd->GetAxBitmap("UPMOVE.BMP");
	HBITMAP hBmpBtnUp_dn = m_pMapWnd->GetAxBitmap("UPMOVE_DN.BMP");
	HBITMAP hBmpBtnUp_hv = m_pMapWnd->GetAxBitmap("UPMOVE_EN.BMP");
	m_pBtnUp->SetImgBitmap(hBmpBtnUp, hBmpBtnUp_dn, hBmpBtnUp_hv);

	m_bCreated = TRUE;
}

BOOL CDllCtrl::OnCommand(WPARAM wParam, LPARAM lParam) 
{
	if(HIWORD(wParam) == BN_CLICKED)
	{
		const int id = LOWORD(wParam);
		switch(id)
		{
		case IDC_BTN_UP:
			m_bBtnState = !m_bBtnState;
			m_pScreenWnd->SetBtnState();

			if(m_bBtnState)
				m_pScreen->ShowWindow(SW_HIDE);
			else
				m_pScreen->ShowWindow(SW_SHOW);

			m_pMapWnd->SendMessage(WM_BTNMSG, wParam, lParam);
			break;
		}
	}
	
	return CWnd::OnCommand(wParam, lParam);
}

int CDllCtrl::GetMapHeight()
{
	return m_nHeight;
}

void CDllCtrl::LoadMap(CString strMapN, CString strName)
{
	CRect cRc;

	m_strName = strName;

	const int type = m_pMapWnd->GetWizardWnd()->SendMessage(WM_USER, MAKEWPARAM(typeDLL, 0), 
		(long) strMapN.operator LPCTSTR());
	const int size = m_pMapWnd->GetWizardWnd()->SendMessage(WM_USER, MAKEWPARAM(sizeMAP, 0), 
		(long)(char *) strMapN.operator LPCTSTR());
	
	GetClientRect(cRc);

	m_pScreen = std::make_unique<CWnd>();
	m_pScreen->Create(NULL, NULL, WS_CHILD|WS_VISIBLE|WS_CLIPSIBLINGS, cRc, this, (int)m_pScreen.get());
	m_nkey = m_pMapWnd->GetWizardWnd()->SendMessage(WM_USER, MAKEWPARAM(attachMAP, type), LPARAM(m_pScreen.get()));

	if (m_nkey != 0)
	{
		const int nLoad = m_pMapWnd->GetWizardWnd()->SendMessage(WM_USER, MAKEWPARAM(loadMAP, m_nkey),
			(long) strMapN.operator LPCTSTR());
		if (nLoad)
			m_pScreen->Invalidate();
	}
}

void CDllCtrl::CloseMap()
{
	if(IsWindow(m_pMapWnd->GetWizardWnd()->m_hWnd) && m_nkey != 0)
		m_pMapWnd->GetWizardWnd()->SendMessage(WM_USER, MAKEWPARAM(detachMAP, m_nkey), 0);
}

void CDllCtrl::OnSize(UINT nType, int cx, int cy) 
{
	CWnd::OnSize(nType, cx, cy);
	
	CRect cRc, dllRc;
	GetClientRect(cRc);

	CString strParam;
	if(cx == 0 || cy == 0)
		return;
		
	strParam.Format("%d\t%d", cx, cy);
	
	if (m_pScreen)
	{
		dllRc = cRc;
		dllRc.right = cRc.right;
		m_pScreen->MoveWindow(dllRc);
		SetScreenParam(PARAM_SIZE, strParam);
	}

	if(!m_bBtnState)
		m_nHeight = cRc.Height();
}

BOOL CDllCtrl::GetBtnState()
{
	return m_bBtnState;
}

void CDllCtrl::SetScreenParam(CString strParam, CString strData)
{
	CString strBuffer = "";
	
	strBuffer.Format("%s\t%s\t%s\t", MAP_PROC, strParam, strData);
	m_pMapWnd->Procedure(strBuffer, m_nkey, 0);
}

void CDllCtrl::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	CRect cRc;
	GetClientRect(cRc);

	if(m_bBtnState)
		Draw(&dc, cRc);
	
	cRc.right = 1;
	dc.FillSolidRect(cRc, m_pMapWnd->GetIndexColor(clBack));
}

void CDllCtrl::Draw(CDC *dc, CRect mapRc)
{
	CRect cRc;
	GetClientRect(mapRc);

	CPen	*pOldPen = dc->SelectObject(m_pMapWnd->GetAxPen(m_pMapWnd->GetIndexColor(clPanelLine), 1, PS_SOLID));
	dc->SelectStockObject(NULL_BRUSH);
	
	const int bkMode = dc->SetBkMode(TRANSPARENT);
	CFont *pOldFont = dc->SelectObject(m_pFont);
	
	cRc = mapRc;
	cRc.DeflateRect(1, 0, 0, 0);
	dc->FillSolidRect(cRc, m_pMapWnd->GetIndexColor(clPanel));
//	DrawRoundBitmap(&dc, cRc, m_hBmpRoundGuide);

	dc->SetTextColor(m_pMapWnd->GetIndexColor(clText));
	cRc.SetRect(mapRc.left + 33, mapRc.top + 3, mapRc.right/2, mapRc.top + 23);
	dc->DrawText(m_strName, cRc, DT_LEFT | DT_VCENTER | DT_SINGLELINE);

	dc->SelectObject(pOldFont);
	dc->SelectObject(pOldPen);
	dc->SetBkMode(bkMode);
}

void CDllCtrl::SetBitmap()
{
	CStringArray strImageArr;

	strImageArr.Add("AXPANEL0_LT.BMP");
	strImageArr.Add("AXPANEL0_RT.BMP");
	strImageArr.Add("AXPANEL0_LB.BMP");
	strImageArr.Add("AXPANEL0_RB.bmp");

	for(int i = 0; i < 4; i++)
	{	
		m_hBmpRoundPanel[i] = m_pMapWnd->GetAxBitmap(strImageArr.GetAt(i));
	}
}
