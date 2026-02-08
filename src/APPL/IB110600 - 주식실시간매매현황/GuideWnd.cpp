// GuideWnd.cpp : implementation file
//

#include "stdafx.h"
#include "ib110600.h"
#include "GuideWnd.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CGuideWnd

CGuideWnd::CGuideWnd(class CMapWnd *pParent)
	:m_pParent(pParent)
{
	m_pBtnSigned = NULL;
	m_pBtnBalance = NULL;
	m_pBtnReceiveAcc = NULL;
	m_pBtnAttention = NULL;

	m_strGuide = _T("");
}

CGuideWnd::~CGuideWnd()
{
//	SAFE_DELETE(m_pBtnSigned);
//	SAFE_DELETE(m_pBtnBalance);
//	SAFE_DELETE(m_pBtnReceiveAcc);
//	SAFE_DELETE(m_pBtnAttention);
}


BEGIN_MESSAGE_MAP(CGuideWnd, CWnd)
	//{{AFX_MSG_MAP(CGuideWnd)
	ON_WM_CREATE()
	ON_WM_PAINT()
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CGuideWnd message handlers

int CGuideWnd::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	m_pFont = m_pParent->GetFont();
	m_strPal = m_pParent->GetPalette();
	SetBitmap();
	CreateControl();
	// TODO: Add your specialized creation code here
	
	return 0;
}

void CGuideWnd::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	CRect mapRc, cRc;
	GetClientRect(mapRc);
	cRc = mapRc;

	CPen	*pOldPen = dc.SelectObject(m_pParent->GetAxPen(m_pParent->GetIndexColor(clPanelLine), 1, PS_SOLID));
	dc.SelectStockObject(NULL_BRUSH);
	
	const int bkMode = dc.SetBkMode(TRANSPARENT);
	CFont *pOldFont = dc.SelectObject(m_pFont);

	m_pParent->DrawOuterSolidRect(&dc, cRc, GAP_SIDE, GAP_TOP, GAP_SIDE, GAP_TOP, m_pParent->GetIndexColor(clBack));

	cRc.DeflateRect(GAP_SIDE, GAP_TOP, GAP_SIDE, GAP_TOP);
	dc.FillSolidRect(cRc, m_pParent->GetIndexColor(clGuide));
	m_pParent->DrawRoundBitmap(&dc, cRc, m_hBmpRoundGuide);

	dc.SetTextColor(m_pParent->GetIndexColor(clreferenceText));
	cRc.DeflateRect(GAP_CTRLS, 6, 150, 24);
	dc.DrawText("실시간 반영이 안될 경우", cRc, DT_LEFT | DT_VCENTER | DT_SINGLELINE);

	cRc.OffsetRect(260, 0);
	dc.DrawText("화면을 이용하시기 바랍니다.", cRc, DT_LEFT | DT_VCENTER | DT_SINGLELINE);

	cRc.OffsetRect(-260, 21);
	cRc.right = cRc.left + 20;
	m_pParent->DrawBitmap(&dc, cRc, m_hBmpGuide);

	dc.SetTextColor(m_pParent->GetIndexColor(clText));
	cRc.OffsetRect(18, 0);
	cRc.right = cRc.left + 400;
	dc.DrawText(m_strGuide, cRc, DT_LEFT | DT_VCENTER | DT_SINGLELINE);
	
	dc.SelectObject(pOldFont);
	dc.SelectObject(pOldPen);
	dc.SetBkMode(bkMode);
}

void CGuideWnd::CreateControl()
{
	CRect panelRc, cRc;

	GetClientRect(panelRc);
	panelRc.top = GAP_TOP;

	cRc.SetRect(panelRc.left + 145, panelRc.top + GAP_TOP,
				panelRc.left + 145 + BTN9_WIDTH, panelRc.top + GAP_TOP + CTRL_HEIGHT);
	m_pBtnSigned = std::make_unique<CfxImgButton>();
	m_pBtnSigned->Create("\n주문체결내역(원장)", cRc, this, IDC_BTN_SIGNED);
	m_pBtnSigned->SetWindowText("주문체결내역(원장)");
	m_pBtnSigned->SetTextColor(m_pParent->GetIndexColor(clBtnText));
	m_pBtnSigned->SetFont(m_pFont);
	m_pBtnSigned->SetDrawFocus();	
	m_pBtnSigned->SetImgBitmap(m_hBmpBtn9, m_hBmpBtn9_dn, m_hBmpBtn9_hv);

	cRc.right = panelRc.right - GAP_SIDE - GAP_CTRLS;
	cRc.left  = cRc.right - BTN4_WIDTH;
	m_pBtnAttention = std::make_unique<CfxImgButton>();
	m_pBtnAttention->Create("\n주의사항", cRc, this, IDC_BTN_ATTENTION);
	m_pBtnAttention->SetWindowText("주의사항");
	m_pBtnAttention->SetTextColor(m_pParent->GetIndexColor(clBtnText));
	m_pBtnAttention->SetFont(m_pFont);
	m_pBtnAttention->SetDrawFocus();	
	m_pBtnAttention->SetImgBitmap(m_hBmpBtn4, m_hBmpBtn4_dn, m_hBmpBtn4_hv);

	cRc.right = cRc.left - GAP_BTN;
	cRc.left  = cRc.right - BTN5_WIDTH;
	m_pBtnReceiveAcc = std::make_unique<CfxImgButton>();
	m_pBtnReceiveAcc->Create("\n예수금현황", cRc, this, IDC_BTN_RECIEVEACC);
	m_pBtnReceiveAcc->SetWindowText("예수금현황");
	m_pBtnReceiveAcc->SetTextColor(m_pParent->GetIndexColor(clBtnText));
	m_pBtnReceiveAcc->SetFont(m_pFont);
	m_pBtnReceiveAcc->SetDrawFocus();	
	m_pBtnReceiveAcc->SetImgBitmap(m_hBmpBtn5, m_hBmpBtn5_dn, m_hBmpBtn5_hv);

	cRc.right = cRc.left - GAP_BTN;
	cRc.left  = cRc.right - BTN4_WIDTH;
	m_pBtnBalance = std::make_unique<CfxImgButton>();
	m_pBtnBalance->Create("\n신용잔고", cRc, this, IDC_BTN_BALANCE);
	m_pBtnBalance->SetWindowText("신용잔고");
	m_pBtnBalance->SetTextColor(m_pParent->GetIndexColor(clBtnText));
	m_pBtnBalance->SetFont(m_pFont);
	m_pBtnBalance->SetDrawFocus();	
	m_pBtnBalance->SetImgBitmap(m_hBmpBtn4, m_hBmpBtn4_dn, m_hBmpBtn4_hv);
}

void CGuideWnd::SetPalette(CString strPal)
{
	m_strPal = strPal;
}

void CGuideWnd::SetBitmap()
{
	CStringArray strImageArr;
	CString sImageDir;

	//알림바
	strImageArr.Add("axpanel" + m_strPal + "0_lt.bmp");
	strImageArr.Add("axpanel" + m_strPal + "0_rt.bmp");
	strImageArr.Add("axpanel" + m_strPal + "0_lb.bmp");
	strImageArr.Add("axpanel" + m_strPal + "0_rb.bmp");

	for(int i = 0; i < 4; i++)
	{	
		m_hBmpRoundGuide[i] = m_pParent->GetAxBitmap(strImageArr.GetAt(i));
	}

	m_hBmpGuide = m_pParent->GetAxBitmap("알림_IC.BMP");

	m_hBmpBtn4 = m_pParent->GetAxBitmap("4btn.bmp");
	m_hBmpBtn4_dn = m_pParent->GetAxBitmap("4btn_dn.bmp");
	m_hBmpBtn4_hv = m_pParent->GetAxBitmap("4btn_en.bmp");
	
	m_hBmpBtn5 = m_pParent->GetAxBitmap("5btn.bmp");
	m_hBmpBtn5_dn = m_pParent->GetAxBitmap("5btn_dn.bmp");
	m_hBmpBtn5_hv = m_pParent->GetAxBitmap("5btn_en.bmp");

	m_hBmpBtn9 = m_pParent->GetAxBitmap("9btn.bmp");
	m_hBmpBtn9_dn = m_pParent->GetAxBitmap("9btn_dn.bmp");
	m_hBmpBtn9_hv = m_pParent->GetAxBitmap("9btn_en.bmp");
}

void CGuideWnd::OnSize(UINT nType, int cx, int cy) 
{
	CWnd::OnSize(nType, cx, cy);
	
	CRect panelRc, cRc;
	GetClientRect(panelRc);

	cRc.right = panelRc.right - GAP_SIDE - GAP_CTRLS;
	cRc.left  = cRc.right - BTN4_WIDTH;
	cRc.top = panelRc.top + GAP_TOP*2;
	cRc.bottom = cRc.top + CTRL_HEIGHT;
	m_pBtnAttention->MoveWindow(cRc);

	cRc.right = cRc.left - GAP_BTN;
	cRc.left  = cRc.right - BTN5_WIDTH;
	m_pBtnReceiveAcc->MoveWindow(cRc);

	cRc.right = cRc.left - GAP_BTN;
	cRc.left  = cRc.right - BTN4_WIDTH;
	m_pBtnBalance->MoveWindow(cRc);
}

BOOL CGuideWnd::OnCommand(WPARAM wParam, LPARAM lParam) 
{
	CString strView;

	if(HIWORD(wParam) == BN_CLICKED)
	{
		const int id = LOWORD(wParam);
		switch(id)
		{
		case IDC_BTN_SIGNED:
			strView.Format("IB110400/d");
			m_pParent->SetView(typeVIEW, strView);
			break;
		case IDC_BTN_BALANCE:
			strView.Format("IB150100/d");
			m_pParent->SetView(typeVIEW, strView);
			break;
		case IDC_BTN_RECIEVEACC:
			strView.Format("IB120600/d");
			m_pParent->SetView(typeVIEW, strView);
			break;
		case IDC_BTN_ATTENTION:
			strView.Format("IB110699/d");
			m_pParent->SetView(typeVIEW, strView);
			break;
		}
	}
	
	return CWnd::OnCommand(wParam, lParam);
}

void CGuideWnd::SetGuide(CString strGuide)
{
	CString strGroup = CString(strGuide, 8);
	if (!strGroup.CompareNoCase("그룹계좌"))
		return;
	m_strGuide = strGuide;
	Invalidate();
}
