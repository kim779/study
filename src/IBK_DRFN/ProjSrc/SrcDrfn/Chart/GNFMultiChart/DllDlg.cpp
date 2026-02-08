// DllDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "MultiChart.h"
#include "DllDlg.h"
#include ".\dlldlg.h"

const UINT RMSG_GETDLLSIZE = ::RegisterWindowMessage("RMSG_GETDLLSIZE");
const UINT RMSG_SETDLLSIZE = ::RegisterWindowMessage("RMSG_SETDLLSIZE");

// CDllDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(CDllDlg, CDialog)
CDllDlg::CDllDlg(LPCSTR szDllName, LPCSTR szTitle, void* pSTInterface, HINSTANCE hOldRsc, CWnd* pParent /*=NULL*/)
	: CDialog(CDllDlg::IDD, pParent)
{
	//m_szDllName = "HWStrategyAnalysis";
	m_szDllName = szDllName;
	m_szTitle = szTitle;
	m_pSTInterface = pSTInterface;
	m_hOldRsc = hOldRsc;
	m_hTradeInst = NULL;
	m_nOptimizerDiv = 0;

	m_pWndTrade = NULL;
}

CDllDlg::~CDllDlg()
{
}

void CDllDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDllDlg, CDialog)
	//{{AFX_MSG_MAP(CDlgAnalysis)
	ON_WM_DESTROY()
	//ON_WM_NCPAINT()
	//ON_WM_NCLBUTTONDOWN()
	//ON_WM_LBUTTONUP()
	//ON_WM_NCMOUSEMOVE()
	//ON_WM_MOUSEMOVE()
	//ON_WM_NCACTIVATE()
	//}}AFX_MSG_MAP
	ON_REGISTERED_MESSAGE(RMSG_SETDLLSIZE,   OnSetDllSize)
END_MESSAGE_MAP()


// CDllDlg 메시지 처리기입니다.

void CDllDlg::OnOk()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CDialog::OnOK();
}

BOOL CDllDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	BOOL bOK = FALSE;
	CBitmap bitmap;
	CString strBitmapFile;
	strBitmapFile.Format("%s\\%s", m_strImageDir, "dlg_cap_back.bmp");
	bOK = m_bmpCaption.Attach ((HBITMAP)::LoadImage(NULL, strBitmapFile, IMAGE_BITMAP, 0, 0, 
		LR_LOADFROMFILE | LR_CREATEDIBSECTION | LR_DEFAULTSIZE));

	if ( !m_ImgList_BtnClose.m_hImageList)
	{
		if ( !m_ImgList_BtnClose.Create( CX_BTNCLOSE, CY_BTNCLOSE, ILC_MASK | ILC_COLORDDB, 1, 1 ) )
		{
			AfxMessageBox ( "Can not Find dlg_cap_btn_close.bmp file" );			
		}
		strBitmapFile.Format("%s\\%s", m_strImageDir, "dlg_cap_btn_close.bmp");
		bOK = bitmap.Attach ((HBITMAP)::LoadImage(NULL, strBitmapFile, IMAGE_BITMAP, 0, 0, 
			LR_LOADFROMFILE | LR_CREATEDIBSECTION | LR_DEFAULTSIZE));
		m_ImgList_BtnClose.Add(&bitmap, RGB(255, 0, 255));
		bitmap.DeleteObject();
	}

	//AfxSetResourceHandle(m_hOldRsc);

	if(m_szTitle.GetLength()) SetWindowText(m_szTitle);

	BOOL bRet = OpenAUP();


	if(bRet==FALSE) OnCancel();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}


// usr/src/AupMain/AupFrm.cpp에서 루틴가져옴
//#include "../../../src/Shared/inc/AupAgentEventDef.h"
//#define WMU_GET_AUP_SIZE	WM_USER + 2243			// Aup의 기본 Size를 View에 요청한다.
#define WMU_SET_TRADING_INSTANCE WM_USER + 2248		// Trading화면에 Dll을 연결한다.

//#include "../../../src/Shared/inc/ISTitleBar.h"
#define		SIZE_TITLE_BAR			23			    ///> 타이틀바 높이.

BOOL CDllDlg::OpenAUP()
{
	if(!m_pWndTrade)
		m_pWndTrade = LoadTradeAup(m_szDllName, this);

	//CWnd *pTrade = LoadTradeAup(m_szDllName, GetParentFrame());
	if(m_pWndTrade == NULL)
	{
		// Error 처리
		return FALSE;
	}
	else
	{   
		//((CAUPExMainView *)m_pInfoAgent->m_pActiveView)->SetTradingWnd(pTrade);
		CPoint size;
		size.x = 800;
		size.y = 620;
		//pTrade->SendMessage(WMU_GET_AUP_SIZE,(WPARAM)&size,0);
		m_pWndTrade->SendMessage(RMSG_GETDLLSIZE,(WPARAM)&size,0);

		CRect frameRect;
		CRect viewRect;
		GetWindowRect(&frameRect);
		GetClientRect(&viewRect);

		int nTheme = nTheme = GetSystemMetrics(SM_CYCAPTION);

		frameRect.right = frameRect.right - 
			GetSystemMetrics(SM_CXEDGE) - 
			GetSystemMetrics(SM_CXDLGFRAME) -
			GetSystemMetrics(SM_CYBORDER);

		frameRect.bottom = frameRect.bottom - GetSystemMetrics(SM_CYEDGE) -
			GetSystemMetrics(SM_CYDLGFRAME) - nTheme -
			GetSystemMetrics(SM_CYBORDER) + SIZE_TITLE_BAR;

		int iNewWidth = frameRect.Width() - viewRect.Width() + size.x;
		int iNewHeight = frameRect.Height() - viewRect.Height() + size.y + 8;

		SetWindowPos(NULL,0,0,iNewWidth,iNewHeight,SWP_NOMOVE);
	}
	return TRUE;
}


// usr/src/AupMain/MainFrm.cpp에서 루틴가져옴
// #include "../../../src/TradingAupDll/TradingAupDef.h
typedef	CWnd * (CreateTradingAup)(UINT nDldresId, CWnd* pParent);
typedef	CWnd * (CreateTradingAupEx)(UINT nDldresId, CWnd* pParent, void* pInterface);
typedef	BOOL (SA_ApplyChart)(LONG dwChartOcx, CListCtrl* pDivList, LONG dwKey);

CWnd *CDllDlg::LoadTradeAup(CString strFile, CWnd *pParent)
{
	CString strTradingName = strFile + ".dll";

	HINSTANCE hTradeInst = LoadLibrary(strTradingName);
	m_hTradeInst = hTradeInst;

	if(hTradeInst == NULL)
	{
		CString strError;
		strError.Format(_T("%s File Load Faile"),strTradingName);
		MessageBox(strError);
		return NULL;
	}

	CreateTradingAup *pCreateTrading = NULL;
	CreateTradingAupEx *pCreateTradingEx = NULL;
	CWnd *pTrade = NULL;

	if(m_pSTInterface)
	{
		pCreateTradingEx = (CreateTradingAupEx *)GetProcAddress(hTradeInst,_T("CREATE_TRADING_EX"));
		if(pCreateTradingEx == NULL)
		{
			MessageBox("특화화면 로드 에러");
			FreeLibrary(hTradeInst);
			return NULL;
		}

		pTrade = pCreateTradingEx(m_nOptimizerDiv,pParent, m_pSTInterface);
		if(pTrade != NULL)
		{
			pParent->SendMessage(WMU_SET_TRADING_INSTANCE,(WPARAM)hTradeInst,0);
		}
		else
		{
			FreeLibrary(hTradeInst);
		}
	}	
	else
	{
		pCreateTrading = (CreateTradingAup *)GetProcAddress(hTradeInst,_T("CREATE_TRADING"));
		if(pCreateTrading == NULL)
		{
			MessageBox("특화화면 로드 에러");
			FreeLibrary(hTradeInst);
			return NULL;
		}

		pTrade = pCreateTrading(m_nOptimizerDiv,pParent);
		if(pTrade != NULL)
		{
			pParent->SendMessage(WMU_SET_TRADING_INSTANCE,(WPARAM)hTradeInst,0);
		}
		else
		{
			FreeLibrary(hTradeInst);
		}
	}
	//if(pTrade)
	//	pTrade->DestroyWindow();
	return pTrade;						
}

LONG CDllDlg::OnSetDllSize(WPARAM wParam, LPARAM lParam)
{
	CPoint *pSize = (CPoint*)wParam;

	CRect frameRect;
	CRect viewRect;
	GetWindowRect(&frameRect);
	GetClientRect(&viewRect);

	int nTheme = nTheme = GetSystemMetrics(SM_CYCAPTION);

	frameRect.right = frameRect.right - 
		GetSystemMetrics(SM_CXEDGE) - 
		GetSystemMetrics(SM_CXDLGFRAME) -
		GetSystemMetrics(SM_CYBORDER);

	frameRect.bottom = frameRect.bottom - GetSystemMetrics(SM_CYEDGE) -
		GetSystemMetrics(SM_CYDLGFRAME) - nTheme -
		GetSystemMetrics(SM_CYBORDER) + SIZE_TITLE_BAR;

	int iNewWidth = frameRect.Width() - viewRect.Width() + pSize->x;
	int iNewHeight = frameRect.Height() - viewRect.Height() + pSize->y;

	SetWindowPos(NULL,0,0,iNewWidth,iNewHeight,SWP_NOMOVE);

	return 0L;
}

typedef	void (DeleteTradingAupEx)(CWnd* pWndTrade);
void CDllDlg::OnDestroy()
{
	m_bmpCaption.DeleteObject();
	m_ImgList_BtnClose.DeleteImageList();
	
	if(m_hTradeInst && m_pWndTrade) 
	{
		DeleteTradingAupEx *pDeleteTradingEx = NULL;
		pDeleteTradingEx = (DeleteTradingAupEx *)GetProcAddress(m_hTradeInst,_T("DESTROY_TRADING"));
		pDeleteTradingEx(m_pWndTrade);
		//if(m_szDllName=="HWStrategyAnalysis")
		//{
			FreeLibrary(m_hTradeInst);//KHD : dll이 안내려가는 오류 잡음 : 2006.11.17
			m_hTradeInst = NULL;
		//}
		m_pWndTrade = NULL;
	}

	CDialog::OnDestroy();
}

void CDllDlg::OnNcPaint()
{
	CDialog::OnNcPaint();

	CWindowDC dc(this);
	CDC dcMem;
	dcMem.CreateCompatibleDC(&dc);

	CRect rcCaptionBar;
	CalcCaptionBarRect(rcCaptionBar);

	CBitmap bitmap;
	bitmap.CreateCompatibleBitmap(&dc, rcCaptionBar.Width(), rcCaptionBar.Height());

	CBitmap* pOldBitmap = dcMem.SelectObject(&bitmap);

	DrawBkgrnd(dcMem, rcCaptionBar);
	DrawTitle(dcMem, rcCaptionBar);
	DrawBtnClose(dcMem, rcCaptionBar);

	dc.BitBlt(rcCaptionBar.left, rcCaptionBar.top, rcCaptionBar.Width(), rcCaptionBar.Height(), 
		&dcMem, 0, 0, SRCCOPY);
	
	dcMem.SelectObject(pOldBitmap);
	ReleaseDC(&dc);
}

void CDllDlg::OnNcLButtonDown(UINT nHitTest, CPoint point) 
{
	SetActiveWindow();

	m_nHitTest = HitTestWindow(point);
	if(m_nHitTest)
	{
		SetCapture();
		m_bLButtonDown = TRUE;
	}
	OnNcPaint();
}

void CDllDlg::OnLButtonUp(UINT nHitTest, CPoint point) 
{
	ReleaseCapture();
	m_nHitTest = HitTestClient(point);
	if(m_nHitTest == HT_CLOSE)
		EndDialog(IDCANCEL);

	m_bLButtonDown = FALSE;
	OnNcPaint();
    CDialog::OnLButtonUp(nHitTest, point);
}

void CDllDlg::OnNcMouseMove(UINT nHitTest, CPoint point)
{
	m_nHitTest = HitTestWindow(point);
	OnNcPaint();
	CDialog::OnNcMouseMove(nHitTest, point);
}

void CDllDlg::OnMouseMove(UINT nFlags, CPoint point) 
{
	m_nHitTest = HitTestClient(point);
	OnNcPaint();
	CDialog::OnMouseMove(nFlags, point);
}

BOOL CDllDlg::OnNcActivate(BOOL bActive) 
{
    OnNcPaint(); 
	return TRUE;
}

void CDllDlg::CalcCaptionBarRect(CRect& rcCaptionBar)
{
	DWORD dwStyle = GetStyle();
	CSize szFrame = (dwStyle & WS_THICKFRAME) ?
		CSize(GetSystemMetrics(SM_CXSIZEFRAME),	GetSystemMetrics(SM_CYSIZEFRAME)) :
		CSize(GetSystemMetrics(SM_CXFIXEDFRAME), GetSystemMetrics(SM_CYFIXEDFRAME));

	if (dwStyle & WS_DLGFRAME)
		szFrame = CSize(GetSystemMetrics(SM_CXDLGFRAME), GetSystemMetrics(SM_CYDLGFRAME));

	GetWindowRect(&rcCaptionBar);
	rcCaptionBar -= CPoint(rcCaptionBar.left, rcCaptionBar.top);

	NONCLIENTMETRICS ncm;
    ncm.cbSize = sizeof(NONCLIENTMETRICS);
	SystemParametersInfo(SPI_GETNONCLIENTMETRICS,sizeof(NONCLIENTMETRICS), &ncm, 0);

	rcCaptionBar.bottom = rcCaptionBar.top + ncm.iCaptionHeight + 4; //보정값 적용
}

void CDllDlg::DrawBkgrnd(CDC& dc, CRect& rcWindowText)
{
	CWindowDC screenDC(NULL);
	CDC dcMem;
	dcMem.CreateCompatibleDC(&screenDC);
	CBitmap* pOldBitmap = dcMem.SelectObject(&m_bmpCaption);

	BITMAP bmp;
	m_bmpCaption.GetBitmap(&bmp);
	dc.StretchBlt(rcWindowText.left, rcWindowText.top, rcWindowText.Width(), rcWindowText.Height(),
		&dcMem, 0, 0, bmp.bmWidth, bmp.bmHeight, SRCCOPY);
	dcMem.SelectObject(pOldBitmap);
	::ReleaseDC(NULL, screenDC.m_hDC);
}

void CDllDlg::DrawTitle(CDC& dc, CRect& rcWindowText)
{
	CString szTitleText;
	GetWindowText(szTitleText);

	int nBkMode = dc.SetBkMode(TRANSPARENT);
	CFont* defFont;
	CFont boldfont;

	LOGFONT lfont;
	memset(&lfont, 0x00,sizeof(LOGFONT));
	lfont.lfWidth = 0; 
	lfont.lfEscapement = 0; 
	lfont.lfOrientation = 0; 
	lfont.lfWeight = FW_NORMAL; 
	lfont.lfItalic = 0; 
	lfont.lfUnderline = 0; 
	lfont.lfStrikeOut = 0; 
	lfont.lfCharSet = DEFAULT_CHARSET; 
	lfont.lfOutPrecision = OUT_DEFAULT_PRECIS; 
	lfont.lfClipPrecision = CLIP_DEFAULT_PRECIS; 
	lfont.lfQuality = DEFAULT_QUALITY;
	lfont.lfPitchAndFamily = FF_ROMAN; 
	strcpy(lfont.lfFaceName, "굴림체");
	lfont.lfHeight = -12;

	lfont.lfWeight = FW_BOLD;
	boldfont.CreateFontIndirect(&lfont);
	defFont = dc.SelectObject(&boldfont);
	dc.SetTextColor(RGB(255,255,255));

	CRect rcScreenTitle(rcWindowText);
	rcScreenTitle.OffsetRect(2, 2);
	dc.DrawText(szTitleText, rcScreenTitle, DT_LEFT|DT_VCENTER|DT_SINGLELINE);
	dc.SetBkMode(nBkMode);
	dc.SelectObject(defFont);
}

void CDllDlg::DrawBtnClose(CDC& dc, CRect& rcWindowText)
{
	DWORD dwStyle = GetStyle();
	CSize szFrame = (dwStyle & WS_THICKFRAME) ?
		CSize(GetSystemMetrics(SM_CXSIZEFRAME), GetSystemMetrics(SM_CYSIZEFRAME)) :
		CSize(GetSystemMetrics(SM_CXFIXEDFRAME), GetSystemMetrics(SM_CYFIXEDFRAME));

    m_rcBtnClose.left = rcWindowText.right - CX_BTNCLOSE;
    m_rcBtnClose.top = 0;
    m_rcBtnClose.right = m_rcBtnClose.left + CX_BTNCLOSE;
    m_rcBtnClose.bottom = GetSystemMetrics(SM_CYSIZE);
	m_rcBtnClose.OffsetRect(-2, 2);

	CPoint pt(m_rcBtnClose.TopLeft());
	if(m_nHitTest)
	{
		if(m_bLButtonDown)			m_ImgList_BtnClose.Draw(&dc, 2, pt, ILD_NORMAL);
		else						m_ImgList_BtnClose.Draw(&dc, 1, pt, ILD_NORMAL);
	}
	else							m_ImgList_BtnClose.Draw(&dc, 0, pt, ILD_NORMAL);
}

int CDllDlg::HitTestClient(CPoint point)
{
	ClientToScreen(&point);
	return HitTestWindow(point);
}

int CDllDlg::HitTestWindow(CPoint point)
{
	CRect rcWindow;
	GetWindowRect(rcWindow);
	CRect rcBtnClose(m_rcBtnClose);
	rcBtnClose.OffsetRect(rcWindow.TopLeft());

	if (rcBtnClose.PtInRect(point))
		return HT_CLOSE;
	return HT_NOWHERE;
}