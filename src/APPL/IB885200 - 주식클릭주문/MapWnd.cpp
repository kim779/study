// MapWnd.cpp : implementation file
//

#include "stdafx.h"
#include "IB885200.h"
#include "MapWnd.h"
#include "HogaWnd.h"
#include "AccountCtrl.h"
#include "ControlWnd.h"
#include "UserDefine.h"

//#include "WarningReserve.h"
#include "cx_Notify.h"
#include "Tool.h"
#include "../../control/fx_grid/memdc.h"
#include "../../control/fx_misc/fxCodeCtrl.h"
#include "../../control/fx_misc/fxImgButton.h"
#include "../../control/fx_misc/fxEdit.h"
#include "../../control/fx_misc/misctype.h"
#include "../../h/AxisVar.h"
#include <math.h>
#include "../../h/MistakeStopOrder.h"

#include "../H/interMSG.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define	_FILE_DEBUG		// time check 하기 위한 debug
#ifdef	_FILE_DEBUG
#include <fstream>
#include "Windows.h"
#endif


#define ID_CTRL_PASS		0x01
#define ID_CTRL_CODE		0x02
#define ID_CTRL_SEND		0x03
#define ID_CTRL_HSIZE		0x04
#define ID_CTRL_VSIZE		0x05
#define ID_CTRL_CODEBTN		0x06
#define ID_CTRL_HOGA3		0x07
#define ID_CTRL_HOGA1		0x08
#define ID_CTRL_SYMBOL		0x09
#define ID_CTRL_RESERVE		0x0a
#define ID_CTRL_CONFIG		0x0b
#define ID_CTRL_ACCPOP		0x0c
#define ID_CTRL_CHARTPOP	0x0d
#define ID_HOGA_WND		0x0e
#define ID_CTRL_WND		0x0f
#define ID_MAP_WND		0x10



#define	ENCTEST


// Timer ID
#define TM_GUIDE		0x01
#define TT_CHECKPSWD	0x02
#define TT_WAITCHECK	0x03
#define TT_CHKVSIZE		0x04
#define TT_INITDATA		0x05

/////////////////////////////////////////////////////////////////////////////
// CMapWnd
#ifdef	_FILE_DEBUG
	ofstream dFile("C:\\DH014000.trc",ios::app);
	LARGE_INTEGER frequency;
	LARGE_INTEGER startCount;
	LARGE_INTEGER curCount;

	DWORD	startTick;
	DWORD	endTick;
#endif

CMapWnd::CMapWnd(CWnd* pwndParent)
{
	m_bHogaSize = FALSE;
	m_pwndParent	= pwndParent;
	m_bWait	 = FALSE;
	m_bWarning	= true;

	m_pfont		= NULL;
	m_pfontB	= NULL;
	m_ppenLine	= NULL;

	m_pwndSymbol	= NULL;
	m_hiSymbol	= NULL;

	m_pTool		= NULL;
	m_pCtrlWnd	= NULL;
	m_pHogaWnd	= NULL;
	m_pAccount	= NULL;
	m_pPass		= NULL;
	m_pCodeCtrl	= NULL;
	m_pBtnSend	= NULL;
	m_pBtnVSize	= NULL;
	m_pBtnCode	= NULL;
	m_pBtnReserve	= NULL;
	m_pBtnConfig	= NULL;
	m_pNotify	= NULL;

	m_clrBG		= RGB(255, 255, 255);
	m_clrTH		= RGB(255, 255, 255);
	m_clrGH		= RGB(255, 255, 255);
	m_clrDT		= RGB(255, 255, 255);
	m_clrTFG	= RGB(0, 0, 0);

	m_eDepth	= hgDEPTH10;
	m_iHogaType = 10;

	m_ledger.m_pParent = pwndParent;
	m_nWaitTrType = 0;
	m_bChecking = FALSE;
	m_iDefaultFont = 9;

	
	m_lSizeRate[0] = 0.16;
	m_lSizeRate[1] = 0.07;
	m_bInit = FALSE;
	m_iOldSize[0] = m_iOldSize[1] = 0;
	m_bChangeFont = FALSE;
	m_iMmgb = 0;

}

CMapWnd::~CMapWnd()
{
	if (m_pAccount)
	{
		m_pAccount->DestroyWindow();
		m_pAccount = nullptr;
	}

	if (m_pHogaWnd)
		m_pHogaWnd = nullptr;

	if (m_pCtrlWnd)
		m_pCtrlWnd = nullptr;

	if (m_pTool)
	{
		m_pTool = nullptr;
	}

	if (m_pwndSymbol)
	{
		if (IsWindow(m_pwndSymbol->GetSafeHwnd()))
			m_pwndSymbol->SendMessage(WM_CLOSE);
		m_pwndSymbol->Detach();
		delete m_pwndSymbol;
		m_pwndSymbol = NULL;
	}
	if (m_hiSymbol)
	{
		AfxFreeLibrary(m_hiSymbol);
		m_hiSymbol = NULL;
	}

}

BEGIN_MESSAGE_MAP(CMapWnd, CWnd)
	//{{AFX_MSG_MAP(CMapWnd)
	ON_WM_PAINT()
	ON_WM_DESTROY()
	ON_BN_CLICKED(ID_CTRL_CODEBTN, OnButtonCode)
	ON_BN_CLICKED(ID_CTRL_SEND, OnButtonSend)
	ON_BN_CLICKED(ID_CTRL_VSIZE, OnButtonVSize)
	ON_BN_CLICKED(ID_CTRL_CONFIG, OnButtonConfig)
	ON_BN_CLICKED(ID_CTRL_RESERVE, OnButtonReserve)
	ON_WM_RBUTTONUP()
	ON_WM_CREATE()
	ON_WM_TIMER()
	ON_WM_SETCURSOR()
	ON_WM_KILLFOCUS()
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_USER, OnMessage)
	ON_MESSAGE(UM_MYMESSAGE, OnMyMessage)
	ON_MESSAGE(WM_USER+1111, OnMyMessage2)
	ON_BN_CLICKED(ID_CTRL_CODE, OnHistoryCode)
	ON_CBN_SELENDOK(ID_CTRL_CODE, OnCodeSelEndOk)
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CMapWnd message handlers
CFont* CMapWnd::getAxFont(CString fName, int point, bool bBold, bool bItalic)
{
	struct _fontR fontR;

	fontR.name = (LPSTR)fName.operator LPCTSTR();
	fontR.point = point;
	fontR.italic = bItalic; 
	fontR.bold = bBold ? FW_BOLD : FW_NORMAL;

	return (CFont*)m_pwndParent->SendMessage(WM_USER, getFONT, (long)&fontR);
}



int CMapWnd::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
#ifdef	_FILE_DEBUG
	dFile	<< "======> DH014000 Start  <<======" << endl;
	QueryPerformanceFrequency(&frequency);
#endif

	// TODO: Add your specialized creation code here
	
	//return 0;
	return Init();
}

void CMapWnd::LoadRoundBitmap()
{
	CString strHome = m_pTool->Variant(homeCC);
	CString sFile;
	sFile.Format("%s\\tab\\PALETTE.INI", strHome);

	CString sPalette = _T("");
	int readL = 0; 
	char readB[1024]; 
	readL = GetPrivateProfileString(_T("General"), _T("Palette"), _T("Blue"), readB, sizeof(readB), sFile);
	if (readL > 0)	sPalette.Format("%s", readB);

	CString sImage;
	sImage.Format("%s\\Image\\axpanel%s1_",  strHome, sPalette);
	for(int i=0; i<4; i++)
	{
		if(m_hRoundBitmap[i])
			DeleteObject(m_hRoundBitmap[i]);
	}

	CIB885200App* pApp = (CIB885200App*)AfxGetApp(); 
	m_hRoundBitmap[0] = (HBITMAP)::LoadImage( pApp->m_hInstance , sImage+"lt.bmp",
											IMAGE_BITMAP,0,0,LR_LOADFROMFILE );
	m_hRoundBitmap[1] = (HBITMAP)::LoadImage( pApp->m_hInstance, sImage+"rt.bmp",
											IMAGE_BITMAP,0,0,LR_LOADFROMFILE );
	m_hRoundBitmap[2] = (HBITMAP)::LoadImage( pApp->m_hInstance, sImage+"lb.bmp",
											IMAGE_BITMAP,0,0,LR_LOADFROMFILE );
	m_hRoundBitmap[3] = (HBITMAP)::LoadImage(pApp->m_hInstance, sImage+"rb.bmp",
											IMAGE_BITMAP,0,0,LR_LOADFROMFILE );

}
int CMapWnd::Init()
{
	m_pTool = std::make_unique<CTool>(m_pwndParent);
	m_pTool->SetTitle(MAP_TITLE);
	CRect wndrc;
	GetClientRect(&wndrc);

	m_pfont = m_pTool->RequestFont("굴림체", 9, false, 0);
	m_pfontB = m_pTool->RequestFont("굴림체", 9, false, FW_BOLD);

	m_clrBG = m_pTool->GetIndexColor(COLOR_BK);
	m_clrTH = m_pTool->GetIndexColor(COLOR_TABLEHEAD);
	m_clrGH = m_pTool->GetIndexColor(COLOR_GRIDHEAD);
	m_clrDT	= m_pTool->GetIndexColor(COLOR_DATA);
	m_clrTFG = m_pTool->GetIndexColor(COLOR_TEXT);
	m_clrGridHdrTxt = m_pTool->GetIndexColor(COLOR_GRIDHEADTXT);

	m_CmdBgColor = m_pTool->GetIndexColor(CMD_BACK_INDEX);
	m_ContentsBgColor = m_pTool->GetIndexColor(CONTENTS_BACK_INDEX);
	m_GuideBgColor = m_pTool->GetIndexColor(GUIDE_BACK_INDEX);
	m_ContentsBorderColor = m_pTool->GetIndexColor(CONTENTS_BORDER_INDEX);
	m_GuideTxtColor = m_pTool->GetIndexColor(GUIDE_TXT_INDEX);

	

	CString strHome = m_pTool->Variant(homeCC);
	CString strImgDir = "";
	strImgDir.Format("%s\\Image\\", strHome);

	m_hGuideBitmap = m_pTool->RequestHBitmap(strImgDir+_T("알림_ic.bmp"));
	LoadRoundBitmap();
	/*
	m_hRoundBitmap[0] = m_pTool->RequestHBitmap(strImgDir+_T("axpanel1_lt.bmp"));
	m_hRoundBitmap[1] = m_pTool->RequestHBitmap(strImgDir+_T("axpanel1_rt.bmp"));
	m_hRoundBitmap[2] = m_pTool->RequestHBitmap(strImgDir+_T("axpanel1_lb.bmp"));
	m_hRoundBitmap[3] = m_pTool->RequestHBitmap(strImgDir+_T("axpanel1_rb.bmp"));
	*/
	m_ppenLine = m_pTool->RequestPen(m_pTool->GetIndexColor(COLOR_LINE), 1, PS_SOLID);


// line 1
	const int cx = HORI_GAP + HORI_GAP;
	int cy = VERT_GAP + VERT_GAP;
	int ileft = cx;
	int itop = cy;

	CRect cRc;
	cRc.SetRect(cx, cy, cx + ACC_WIDTH, cy + CTRL_HEIGHT);
	m_pAccount = std::make_unique <CAccountCtrl>(m_pwndParent, this, m_pTool->GetHome());
	m_pAccount->CreateEx(WS_EX_TOPMOST, NULL, "ACCN", WS_VISIBLE | WS_CHILD | WS_TABSTOP, cRc, this, 0);
	// 2008.07.22 변경분
	//m_pAccount->createAccountCtrl("AN10", TK_ACCOUNT, m_pTool->GetIndexColor(7));
	m_pAccount->createAccountCtrl("AN1A", TK_ACCOUNT, m_pTool->GetIndexColor(7));
	m_rectAccName.SetRect(cRc.right + GAP, cRc.top, cRc.right + GAP + 88, cRc.bottom);

	cRc.SetRect(m_rectAccName.right + GAP, cRc.top, m_rectAccName.right + GAP + 60, cRc.bottom);
	m_pPass = std::make_unique <CfxEdit>(m_pfont);
	m_pPass->Create(WS_CHILD | WS_VISIBLE | ES_LEFT | ES_PASSWORD | WS_TABSTOP, cRc, this, ID_CTRL_PASS);
	m_pPass->Init(esNORMAL, NULL, "0", RGB(255, 255, 255), RGB(0, 0, 0), alLEFT);
	((CEdit*)m_pPass.get())->LimitText(8);
	m_pPass->SetWindowText(m_strPswd);

	const int ibtnw2 = 36;
	const int ibtngap = 1;

	cRc.SetRect(cRc.right + GAP, cRc.top, cRc.right + GAP + ibtnw2, cRc.bottom);
	m_pBtnSend = std::make_unique <CfxImgButton>();
	m_pBtnSend->Create("\n조회", cRc, this, ID_CTRL_SEND, FALSE, TRUE);
	m_pBtnSend->SetWindowText("조회");
	m_pBtnSend->SetFont(m_pfont);
	m_pBtnSend->SetDisableDraw(FALSE);
	m_pBtnSend->SetTextColor(m_pTool->GetIndexColor(69));

	cRc.SetRect(cRc.right + ibtngap, cRc.top, cRc.right + ibtngap + ibtnw2, cRc.bottom);
	m_pBtnReserve = std::make_unique <CfxImgButton>();
	m_pBtnReserve->Create("\nStop", cRc, this, ID_CTRL_RESERVE, FALSE, TRUE);
	m_pBtnReserve->SetWindowText("Stop");
	m_pBtnReserve->SetFont(m_pfont);
	m_pBtnReserve->SetDisableDraw(FALSE);
	m_pBtnReserve->SetTextColor(m_pTool->GetIndexColor(69));

	cRc.SetRect(cRc.right + ibtngap, cRc.top, cRc.right + ibtngap + ibtnw2, cRc.bottom);
	m_pBtnConfig = std::make_unique <CfxImgButton>();
	m_pBtnConfig->Create("\n설정", cRc, this, ID_CTRL_CONFIG, FALSE, TRUE);
	m_pBtnConfig->SetWindowText("설정");
	m_pBtnConfig->SetFont(m_pfont);
	m_pBtnConfig->SetDisableDraw(FALSE);
	m_pBtnConfig->SetTextColor(m_pTool->GetIndexColor(69));

	cRc.SetRect(cRc.right + GAP, cRc.top, cRc.right + GAP + 20, cRc.bottom);
	m_pBtnVSize = std::make_unique <CfxImgButton>();
	m_pBtnVSize->Create("\n5/10호가전환", cRc, this, ID_CTRL_VSIZE, FALSE, TRUE);
	m_pBtnVSize->SetFont(m_pfont);
	m_pBtnVSize->SetDisableDraw(FALSE);
	m_pBtnVSize->SetTextColor(m_pTool->GetIndexColor(69));


// line 2
	cy = cRc.bottom + GAP;
	cRc.SetRect(cx, cy, cx + 88, cy + CTRL_HEIGHT + 1);
	m_pCodeCtrl = std::make_unique <CfxCodeCtrl>(this, CString(m_pTool->GetHome()) + "\\tab");
	m_pCodeCtrl->Create(this, cRc, ID_CTRL_CODE);
	m_pCodeCtrl->SetUnit(GU_CODE);
	m_pCodeCtrl->SetFont(m_pfont);


	cRc.SetRect(cRc.right + GAP, cRc.top, cRc.right + GAP + 20, cRc.top + CTRL_HEIGHT);
	m_pBtnCode = std::make_unique <CfxImgButton>();
	m_pBtnCode->Create("\n종목코드", cRc, this, ID_CTRL_CODEBTN, FALSE, TRUE);
	m_pBtnCode->SetFont(m_pfont);
	m_pBtnCode->SetTextColor(m_pTool->GetIndexColor(69));


	m_rectCurrS.SetRect(cRc.right + GAP, cRc.top, cRc.right + GAP + 75, cRc.bottom + 3);
	m_rectDiffS.SetRect(m_rectCurrS.right - 1, m_rectCurrS.top, m_rectCurrS.right + 80, m_rectCurrS.bottom);
	m_rectRateS.SetRect(m_rectDiffS.right - 1, m_rectDiffS.top, m_rectDiffS.right + 75, m_rectDiffS.bottom);
	m_rectGvolS.SetRect(m_rectRateS.right - 1, m_rectRateS.top, ileft+HOGA_WIDTH, m_rectRateS.bottom);

// line 3
	cy = cRc.bottom + GAP + 1;
	cRc.SetRect(cx, cy, cx + 110, cy + CTRL_HEIGHT + 1);
	InitSymbol(cRc);

	m_rectCurrD = m_rectCurrS;	m_rectCurrD.OffsetRect(0, 23);	m_rectCurrD.DeflateRect(0, -1, 0, 1);
	m_rectDiffD = m_rectDiffS;	m_rectDiffD.OffsetRect(0, 23);	m_rectDiffD.DeflateRect(0, -1, 0, 1);
	m_rectRateD = m_rectRateS;	m_rectRateD.OffsetRect(0, 23);	m_rectRateD.DeflateRect(0, -1, 0, 1);
	m_rectGvolD = m_rectGvolS;	m_rectGvolD.OffsetRect(0, 23);	m_rectGvolD.DeflateRect(0, -1, 0, 1);

// hoga wnd
	ileft = 4;
	itop = m_rectGvolD.bottom + INNER_FRAME_SPACE + OUTER_FRAME_SPACE + INNER_FRAME_SPACE;
	const int ihogabottom = wndrc.bottom - GUIDE_HEIGHT - INNER_FRAME_SPACE - OUTER_FRAME_SPACE -1;
//		
	cRc.SetRect(OUTER_FRAME_SPACE, itop, OUTER_FRAME_SPACE+HOGA_WIDTH, ihogabottom);
	const CRect rectHoga = cRc;


// control wnd
	int ix = cRc.right+INNER_FRAME_SPACE + OUTER_FRAME_SPACE + INNER_FRAME_SPACE;
	ix = rectHoga.right + INNER_FRAME_SPACE + OUTER_FRAME_SPACE + INNER_FRAME_SPACE+2;
	itop = OUTER_FRAME_SPACE;
	cRc.SetRect(ix, itop, wndrc.right -FRAME_SPACE-INNER_FRAME_SPACE, cRc.bottom);
	m_pCtrlWnd = std::make_unique <CControlWnd>(m_pwndParent, m_pTool.get());
	m_pCtrlWnd->Create(NULL, "control", WS_CHILD | WS_VISIBLE | WS_CLIPCHILDREN | WS_CLIPSIBLINGS, cRc, this, ID_CTRL_WND);

// hoga wnd
//	m_pHogaWnd->Init(m_pCtrlWnd);
	m_pHogaWnd = std::make_unique <CHogaWnd>(m_pTool.get(), m_pCtrlWnd.get(), this);
	m_pHogaWnd->Create(NULL, "hoga", WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN, rectHoga, this, ID_HOGA_WND);


// guide
//	m_rcGuide.SetRect(HORI_GAP, cRc.bottom + itop, MAP_WIDTH - GAP, MAP_HEIGHT - BOT_GAP);
	itop = wndrc.bottom - GUIDE_HEIGHT - INNER_FRAME_SPACE;
	m_rcGuide.SetRect(FRAME_SPACE, itop, wndrc.right-FRAME_SPACE, itop + GUIDE_HEIGHT);

// notify wnd
	m_pNotify = std::make_unique <CNotify>(m_pwndParent, TK_NOTIFYCTRL);
	m_pNotify->CreateNotify(this, CRect(0,0,0,0), m_pTool->GetHome());
	m_pNotify->SetCtrlWnd(m_pCtrlWnd.get());


// button bitmap
	SetButtonBitmaps();

// Init Focus
	if (m_strAcc.IsEmpty())
		m_pAccount->SetFocus();
	else
		m_pPass->SetFocus();

	m_bInit = TRUE;
	m_iOldFont = 9;
	LoadBuffer();
	m_iHogaRowH = m_pHogaWnd->GetRowheight();

	return 0;
}

void CMapWnd::InitSymbol(CRect rect)
{
	CString	text;
	CWnd* (APIENTRY * axCreate)(CWnd*, void*) {};

	text.Format("%s/%s/cx_symbol.dll", m_pTool->GetHome(), DEVDIR);
	m_hiSymbol = AfxLoadLibrary(text);
	if (m_hiSymbol != NULL)
	{
		axCreate = (CWnd* (APIENTRY*)(CWnd*, void*))GetProcAddress(m_hiSymbol, _T("axCreate"));
		if (axCreate != NULL)
		{
			
			symbolparam.key   = 0;
			symbolparam.name  = _T("17413");
			symbolparam.rect  = rect;
			symbolparam.fonts = "굴림체";
			symbolparam.point = 9;
			symbolparam.style = 1;
			symbolparam.tRGB  = 69;
			symbolparam.pRGB  = 90;
			symbolparam.options = _T("/a89/b91/c92/d69/i99/s8852");

			m_pwndSymbol = (*axCreate)(m_pwndParent, &symbolparam);
			m_pwndSymbol->SetWindowPos(&wndTop, 0, 0, 0, 0, SWP_NOSIZE|SWP_NOMOVE);
			if (m_pwndSymbol == NULL)
			{
				AfxFreeLibrary(m_hiSymbol);
				m_hiSymbol = NULL;
			}
		}
	}
}

void CMapWnd::SetButtonBitmaps()
{
	CString strPath = "";
	strPath.Format("%s\\%s\\", m_pTool->GetHome(), IMAGEDIR);

	HBITMAP hbmp	= m_pTool->RequestBitmap(strPath + "2btn.bmp")->operator HBITMAP();
	HBITMAP hbmp_dn	= m_pTool->RequestBitmap(strPath + "2btn_dn.bmp")->operator HBITMAP();
	HBITMAP hbmp_hv = m_pTool->RequestBitmap(strPath + "2btn_en.bmp")->operator HBITMAP();
	m_pBtnSend->SetImgBitmap(hbmp, hbmp_dn, hbmp_hv);
	m_pBtnReserve->SetImgBitmap(hbmp, hbmp_dn, hbmp_hv);
	m_pBtnConfig->SetImgBitmap(hbmp, hbmp_dn, hbmp_hv);

	hbmp	= m_pTool->RequestBitmap(strPath + "투자매매종합200.bmp")->operator HBITMAP();
	hbmp_dn	= m_pTool->RequestBitmap(strPath + "투자매매종합200_dn.bmp")->operator HBITMAP();
	hbmp_hv	= m_pTool->RequestBitmap(strPath + "투자매매종합200_en.bmp")->operator HBITMAP();
	m_pBtnVSize->SetImgBitmap(hbmp, hbmp_dn, hbmp_hv);

	hbmp    = m_pTool->RequestBitmap(strPath + "S검색.bmp")->operator HBITMAP();
	hbmp_dn = m_pTool->RequestBitmap(strPath + "S검색_dn.bmp")->operator HBITMAP();
	hbmp_hv = m_pTool->RequestBitmap(strPath + "S검색_en.bmp")->operator HBITMAP();
	m_pBtnCode->SetImgBitmap(hbmp, hbmp_dn, hbmp_hv);
}


void CMapWnd::OnDestroy() 
{
	
	if (m_pBtnCode)
	{
		::DestroyWindow(m_pBtnCode->m_hWnd);
		m_pBtnCode = nullptr;
	}
	if (m_pCodeCtrl)
	{
		::DestroyWindow(m_pCodeCtrl->m_hWnd);
		m_pCodeCtrl = nullptr;
	}

	if (m_pBtnVSize)
	{
		::DestroyWindow(m_pBtnVSize->m_hWnd);
		m_pBtnVSize = nullptr;
	}
	if (m_pBtnConfig)
	{
		::DestroyWindow(m_pBtnConfig->m_hWnd);
		m_pBtnConfig = nullptr;
	}
	if (m_pBtnReserve)
	{
		::DestroyWindow(m_pBtnReserve->m_hWnd);
		m_pBtnReserve = nullptr;
	}
	if (m_pBtnSend)
	{
		::DestroyWindow(m_pBtnSend->m_hWnd);
		m_pBtnSend = nullptr;
	}
	if (m_pPass)
	{
		::DestroyWindow(m_pPass->m_hWnd);
		m_pPass = nullptr;
	}

	if (m_pNotify)
	{
		m_pNotify->DestroyWindow();
	}
#ifdef	_FILE_DEBUG
	dFile	<< "======> DH014000 End   <<======" << endl << endl << endl;
#endif
	CWnd::OnDestroy();
}

void CMapWnd::Transaction()
{
	if (m_strJCOD.IsEmpty())
	{
		SetGuide("종목을 선택하십시오");	
		return;
	}

	m_pCtrlWnd->ClearMaip();
	m_pCtrlWnd->ClearJango();

	struct	_sise_mid Mid;
	FillMemory(&Mid, sz_SISEMID, ' ');

	CopyMemory(Mid.code, m_strJCOD, m_strJCOD.GetLength());
	m_pTool->SendTR(TN_SISE, (char*)&Mid, sizeof(Mid), TK_SISE);

	SendSymbol();

	if (m_strAcc.IsEmpty())
	{
		SetGuide("계좌번호를 입력하십시오");	
		return;
	}
	/*
	m_pPass->GetWindowText(m_strPswd);
	m_strPswd.TrimLeft(), m_strPswd.TrimRight();
	
	if (m_strPswd.IsEmpty())
	{
		SetGuide("비밀번호를 입력하십시오");	
		return;
	}
	*/

	SaveBuffer(m_strAcc, m_strPswd);
	if (m_pNotify)
	{
		m_pNotify->SendJango(m_strAcc, m_strPswd);
	}

//	m_nWaitTrType = 1;
//	CheckPswd(m_strAcc, m_strPswd);
	
	m_strJName = m_pTool->GetCodeName(m_strJCOD);

	CString strParam;
	strParam.Format("1301\t%s", m_strJCOD);

	m_pTool->SetHistory(strParam);
	m_pTool->Trigger(strParam);

	m_pCtrlWnd->AddJCod(m_strJCOD, m_strJName);
}

void CMapWnd::DrawBitmap(CDC* pDC, CRect rc, HBITMAP hBitmap)
{

	HDC hDC = ::CreateCompatibleDC (NULL);
	SelectObject (hDC, hBitmap); //icon
//	::BitBlt( pDC->m_hDC, rc.left, rc.top, rc.Width(), rc.Height(),	hDC, 0, 0, SRCCOPY);
	::TransparentBlt(pDC->m_hDC, rc.left, rc.top, rc.Width(), rc.Height(), hDC, 0, 0, rc.Width(), rc.Height(), 0xFF00FF);

	::DeleteDC (hDC);
}

void CMapWnd::DrawCornerRoundBitmap(CDC* pDC, CRect rc)
{
	const int iPoint = 5;
	DrawBitmap(pDC, CRect(rc.left, rc.top, rc.left+iPoint, rc.top +iPoint), m_hRoundBitmap[0]);
	DrawBitmap(pDC, CRect(rc.right-iPoint, rc.top, rc.right, rc.top +iPoint), m_hRoundBitmap[1]);
	DrawBitmap(pDC, CRect(rc.left, rc.bottom-iPoint, rc.left+iPoint, rc.bottom), m_hRoundBitmap[2]);
	DrawBitmap(pDC, CRect(rc.right-iPoint, rc.bottom-iPoint, rc.right, rc.bottom), m_hRoundBitmap[3]);
}

void CMapWnd::OnPaint() 
{
	CPaintDC dc(this); // device context for painting

	if(!m_pHogaWnd) return;
	CMemDC memDC(&dc);

	CRect rc, cRc, rc2, rc3, wrc, rc4;
	GetClientRect(&rc);

	m_pHogaWnd->GetWindowRect(&wrc);
	ScreenToClient(wrc);
	const int iOldBkMode = memDC.SetBkMode(TRANSPARENT);
	memDC.FillSolidRect(rc, m_clrBG);

	rc2 = CRect(FRAME_SPACE, INNER_FRAME_SPACE, wrc.right + FRAME_SPACE, wrc.bottom  + FRAME_SPACE);
	rc3 = rc2;

	//m_rectGvolD:거래량
	rc2.bottom = m_rectGvolD.bottom + INNER_FRAME_SPACE;
	rc2.right = m_rectGvolD.right + OUTER_FRAME_SPACE;
	
	rc3.top = rc2.bottom + OUTER_FRAME_SPACE;
	rc3.bottom = wrc.bottom + INNER_FRAME_SPACE;
	rc3.right = rc2.right ;
	
	rc4 = rc2;
	rc4.left = rc2.right + OUTER_FRAME_SPACE;
	rc4.bottom = rc3.bottom;
	rc4.right = rc.right - FRAME_SPACE;

	memDC.FillSolidRect(rc2, m_CmdBgColor);
	memDC.FillSolidRect(rc3, m_ContentsBgColor);
	memDC.FillSolidRect(rc4, m_ContentsBgColor);

	DrawCornerRoundBitmap(&memDC, rc2);
	DrawCornerRoundBitmap(&memDC, rc3);
	DrawCornerRoundBitmap(&memDC, rc4);

	CPen penName;
	penName.CreatePen(PS_SOLID, 1, m_pTool->GetIndexColor(89));

	CPen *ppenOld = memDC.SelectObject(&penName);
//	CPen *ppenOld = memDC.SelectObject(m_ppenLine);
	memDC.SelectStockObject(NULL_BRUSH);

// line 1
	cRc = m_rectAccName;	
	cRc.DeflateRect(2, 0);
	memDC.FillSolidRect(&m_rectAccName, m_pTool->GetIndexColor(160));
	memDC.Rectangle(&m_rectAccName);
	DrawSiseText(&memDC, m_strAccName, cRc, DT_LEFT, 0, m_clrTFG);

	memDC.SelectObject(m_ppenLine);
	penName.DeleteObject();
// line 3

// sise table
	memDC.FillSolidRect(&m_rectCurrS, m_clrGH);
	memDC.Rectangle(&m_rectCurrS);
	memDC.FillSolidRect(&m_rectDiffS, m_clrGH);
	memDC.Rectangle(&m_rectDiffS);
	memDC.FillSolidRect(&m_rectRateS, m_clrGH);
	memDC.Rectangle(&m_rectRateS);
	memDC.FillSolidRect(&m_rectGvolS, m_clrGH);
	memDC.Rectangle(&m_rectGvolS);

	DrawSiseText(&memDC, "현재가", m_rectCurrS, DT_CENTER, 0, m_clrGridHdrTxt);
	DrawSiseText(&memDC, "대  비", m_rectDiffS, DT_CENTER, 0, m_clrGridHdrTxt);
	DrawSiseText(&memDC, "등락률", m_rectRateS, DT_CENTER, 0, m_clrGridHdrTxt);
	DrawSiseText(&memDC, "거래량", m_rectGvolS, DT_CENTER, 0, m_clrGridHdrTxt);
//, 0, m_clrGridHdrTxt
	memDC.FillSolidRect(&m_rectCurrD, m_clrDT);
	memDC.Rectangle(&m_rectCurrD);
	memDC.FillSolidRect(&m_rectDiffD, m_clrDT);
	memDC.Rectangle(&m_rectDiffD);
	memDC.FillSolidRect(&m_rectRateD, m_clrDT);
	memDC.Rectangle(&m_rectRateD);
	memDC.FillSolidRect(&m_rectGvolD, m_clrDT);
	memDC.Rectangle(&m_rectGvolD);

	DrawSiseText(&memDC, m_strCurr, m_rectCurrD, DT_RIGHT, ST_COLOR | ST_COMMA | ST_REMOVE | ST_NORMAL | ST_BOLD, m_clrTFG);
	DrawSiseText(&memDC, m_strDiff, m_rectDiffD, DT_RIGHT, ST_COMMA | ST_REMOVE | ST_NORMAL | ST_BOLD | ST_SHGA, m_clrTFG);
	DrawSiseText(&memDC, m_strRate, m_rectRateD, DT_RIGHT, ST_COLOR | ST_COMMA | ST_REMOVE | ST_NORMAL | ST_BOLD, m_clrTFG);
	DrawSiseText(&memDC, m_strGvol, m_rectGvolD, DT_RIGHT, ST_COLOR | ST_COMMA | ST_REMOVE | ST_NORMAL | ST_BOLD, m_clrTFG);

// guide
	CPen penGuide;
	penGuide.CreatePen(PS_SOLID, 1, m_pTool->GetIndexColor(75));
	memDC.SelectObject(&penGuide);
	
	memDC.FillSolidRect(&m_rcGuide, m_GuideBgColor);
	memDC.Rectangle(&m_rcGuide);

	CRect alarmr = m_rcGuide;

	if( alarmr.Height()  > 15)
	{
		const int igap = (alarmr.Height() - GUIDE_ALARM_BITMAP) / 2;
		if(igap > 0)
		{
			alarmr.top += igap;
		}
	}

	alarmr.bottom = alarmr.top  + GUIDE_ALARM_BITMAP;
	alarmr.left = OUTER_FRAME_SPACE;
	alarmr.right = alarmr.left + GUIDE_ALARM_BITMAP;
	DrawBitmap(&memDC, alarmr, m_hGuideBitmap);
	CRect guideTxtrc = m_rcGuide;
	guideTxtrc.left = alarmr.right;


	DrawCornerRoundBitmap(&memDC, m_rcGuide);
	memDC.SetTextColor(m_GuideTxtColor);
	DrawSiseText(&memDC, m_strGuide, guideTxtrc, DT_LEFT|DT_SINGLELINE|DT_VCENTER, 0, m_clrTFG);

	memDC.SelectObject(ppenOld);
	memDC.SetBkMode(iOldBkMode);

	penGuide.DeleteObject();
}

LRESULT CMapWnd::OnMessage(WPARAM wParam, LPARAM lParam)
{
	int key = 0;
	int nLen = 0;
	CString stmp;

	switch(LOBYTE(LOWORD(wParam)))
	{
	case DLL_OUB:
		key = HIBYTE(LOWORD(wParam));
		stmp.Format("[memo][IB885200]  key =[%s]   len=[%d]\r\n", GetMemoTrKeyType(key), HIWORD(wParam));
		OutputDebugString(stmp);
		if ((key >= TRKEY_MEMO_POPUPOOP && key <= TRKEY_MEMO_CHECK))
		{
			m_pwndSymbol->SendMessage(WM_USER, wParam, lParam);
			return 1;
		}

		if (key == 0)
		{
			int	value;
			CString	text;

			text = CString((char*)lParam, HIWORD(wParam));
			value = text.Find('\t');
			if (value != -1)
			{
				WPARAM	wParam{};
				CString m_rts;
				if (value == 7)
				{
					m_rts = text.Left(value++);
					text = text.Mid(value);
					wParam = MAKEWPARAM(MAKEWORD(DLL_OUB, 0), (text.GetLength() > 6) ? text.GetLength() : 0);
					m_pwndSymbol->SendMessage(WM_USER, wParam, (LPARAM)text.operator LPCTSTR());
				}
				else
				{
					m_rts = text.Left(value++);
					//				text   = text.Mid(value);
					wParam = MAKEWPARAM(MAKEWORD(DLL_OUB, 0), (text.GetLength() > 6) ? text.GetLength() : 0);
					m_pwndSymbol->SendMessage(WM_USER, wParam, (LPARAM)text.operator LPCTSTR());
				}
			}
			return 1;
		}
		switch (HIBYTE(LOWORD(wParam)))
		{
		case 255:		// CX_Account
		case 254:
			m_pAccount->m_pAccountCtrl->SendMessage(WM_USER, wParam, lParam);
			break;
		case TK_SISE:
			ReceiveSise((char*)lParam, HIWORD(wParam));
			break;
		case TK_CHECKPSWD:
			ParsingCheckPswd((char*)lParam, HIWORD(wParam));
			break;
		case TK_JUMUN:
#ifdef	_FILE_DEBUG
			{	
				nLen = HIWORD(wParam);
				CString strData((char*) (lParam + L_ledgerH), nLen - L_ledgerH);
				dFile << "주문 Out : " << strData << endl;
				
				if (strData.GetLength() < 21)
					return 0;
				//AfxMessageBox("ecod: "+strData.Mid(16,4)+"\n");
				if (strData.Mid(16,4)=="4273")
				{
					MessageBox("투자성향 미등록 및 등급이 낮은 고객이십니다.\n성향 등록 및 변경후 재시도 바랍니다.","IBK투자증권", MB_OK);
					m_pTool->CreateView(typeVIEW, "IB821200", "/t");
				}
				
				strData.Delete(0, 21);
				
				SetGuide(strData);
				
/*				endTick = GetTickCount();
				QueryPerformanceCounter(&curCount);
				CTime curr = CTime::GetCurrentTime();
				CString timeS, gapS, msgS, tickS;
				double dTime = (double)(curCount.QuadPart - startCount.QuadPart) / (double)frequency.QuadPart;
				gapS.Format("%.4fs", dTime);
				tickS.Format("%5dms", endTick-startTick);
				msgS = CString((char*)(lParam + m_iLenLedger), sizeof(struct _jumunmod));
				timeS.Format("[%02d/%02d %02d:%02d:%02d]  ", curr.GetMonth(), curr.GetDay(), curr.GetHour(), curr.GetMinute(), curr.GetSecond());
				dFile	<< "주문 out" << timeS << m_strCODX << "gap[" << gapS << "]tick[" << tickS << "] ==> " << msgS << endl;
*/
			}
#endif

			TRACE("%s\n", (char*)(lParam + m_iLenLedger));
			

			break;
		case TK_MDJMCN:
		case TK_MSJMCN:
		case TK_MAXMSJMCN:
		case TK_MICHE:
			{
				struct _ledgerH ledger;
				CString strTemp = (char*)lParam;
				CopyMemory(&ledger, (void*)lParam, L_ledgerH);
				CString strLedger = CString((char*)&ledger, L_ledgerH);
				CString strErrCode = CString((char*)&ledger.emsg, 4);
				CString strErrText = CString((char*)&ledger.emsg, 98);
								
				lParam += L_ledgerH;
				CString strMsg = "ERR\t" + strErrText;

				if (strErrCode.GetAt(0) != _T('0'))	// 오류
				{
					strMsg.Format("[%s]%s", strErrCode, strErrText.Right(strErrText.GetLength() - 4));
					strMsg.TrimRight();

					if (strErrCode == _T("1801") || strErrCode == _T("3669"))
						MessageBox(strMsg);
					else
						SetGuide(strMsg);

					return 0;
				}
				/*
				else
				{
					MessageBox(strMsg,"IBK투자증권-미체결내역조회",MB_OK);
				}
				*/

				if (key == TK_MICHE)
				{
					m_pHogaWnd->ReceiveMiche((char *)(lParam), HIWORD(wParam));
					
					if (ledger.next[0] == _T('Y'))
						SendMicheg(CString((char*)&ledger, L_ledgerH));
				}
				else if (key == TK_MDJMCN)
				{
					TRACE("KEY_MDJMCN\n");
					m_pCtrlWnd->SetMaxJmcn((char*)(lParam + L_maxjmct_mid), TK_MDJMCN);
				}
				else if (key == TK_MSJMCN)
				{
					TRACE("KEY_MSJMCN\n");
					m_pCtrlWnd->SetMaxJmcn((char*)(lParam + L_maxjmct_mid), TK_MSJMCN);
				}
				else if (key == TK_MAXMSJMCN)
				{
					TRACE("KEY_MAXMSJMCN\n");
					m_pCtrlWnd->SetMaxJmcn((char*)(lParam + L_maxjmct_mid), TK_MAXMSJMCN);
				}
			}
			break;
		case TK_SYMBOL:
			m_pwndSymbol->SendMessage(WM_USER, wParam, lParam);
			if (HIWORD(wParam) >= L_jinfo)
			{
				struct _jinfo jinfo;
				CopyMemory(&jinfo, (void*)lParam, L_jinfo);
				m_strListedQuantity = CString(jinfo.sjjs, sizeof(jinfo.sjjs));
			}
			break;
		}
		break;
	case DLL_ALERT:
		Alert((char*)lParam);
		m_pHogaWnd->ReceiveAlert((char*)lParam);
		break;
	case DLL_NOTICE:
#ifdef	_FILE_DEBUG0
			{
				endTick = GetTickCount();
				QueryPerformanceCounter(&curCount);
				CTime curr = CTime::GetCurrentTime();
				CString timeS, gapS, msgS,tickS;
				double dTime = (double)(curCount.QuadPart - startCount.QuadPart) / (double)frequency.QuadPart;
				gapS.Format("%.4fs", dTime);
				tickS.Format("%5dms", endTick-startTick);
				timeS.Format("[%02d/%02d %02d:%02d:%02d]  ", curr.GetMonth(), curr.GetDay(), curr.GetHour(), curr.GetMinute(), curr.GetSecond());
				msgS = CString((char*)lParam);
				int iPos = msgS.Find('\n');
				msgS = msgS.Mid(iPos);
				msgS.Replace('\r', '\t');
				dFile	<< "notice  " << timeS << "       gap[" << gapS << "]tick[" << tickS << "] ==> " << msgS << endl;
			}
#endif
		m_pHogaWnd->ReceiveNotice((char*)lParam);
		break;
	case DLL_TRIGGER:
		Trigger((char*)lParam);
		break;
	case DLL_SETPAL:
		Palette();
		break;
	case DLL_DOMINO:
		Domino((char*)lParam);
		break;

//	case DLL_SETFONT:
	case DLL_SETFONTx:
		OnDLLSetFont( HIWORD(wParam), CString((char*)lParam) );
		break;
	case DLL_GUIDE:
		SetGuide((char *)lParam);
		return true;
	}
	return 0;
}

void CMapWnd::OnDLLSetFont(int point, CString sFont)
{
	if(!m_bInit) return;
	if(m_iOldFont == point) return;

	m_pfont = m_pTool->RequestFont(sFont, point, false, 0);
	m_pfontB = m_pTool->RequestFont(sFont, point, false, FW_BOLD);
	if(!m_pfont) return;

	const double ldif = float(point - 9);
	const int iWndWidth = MAP_WIDTH + (int)(m_lSizeRate[0] * (float)MAP_WIDTH * ldif);
	const int iWndHeight = MAP_HEIGHT +(int) (m_lSizeRate[1] * (float)MAP_HEIGHT * ldif);

	m_iOldFont = point;

	SetWindowPos(NULL, 0, 0, iWndWidth, iWndHeight, SWP_NOZORDER | SWP_NOMOVE);
	m_pwndParent->SendMessage(WM_USER, MAKEWPARAM(sizeDLL, 0), MAKELPARAM(iWndWidth, iWndHeight) );
}

LRESULT CMapWnd::OnMyMessage(WPARAM wParam, LPARAM lParam)
{
	switch(LOWORD(wParam))
	{
	case 100:	// CAccountCtrl 계좌변경시 수신 Message
		{
			CString data = (char*)lParam;

			CString acc = m_pTool->Parser(data, '\t');
			m_strAccName = m_pTool->Parser(data, '\t');
			m_strPswd = m_pTool->Parser(data, '\t');
			m_strPswd.TrimLeft(), m_strPswd.TrimRight();
			if(m_strPswd.IsEmpty() ) m_strPswd = GetBuffer(acc);

			if (m_strAcc != acc)
			{
				if (m_pHogaWnd)
					m_pHogaWnd->ClearJumun();

				if (m_pCtrlWnd)
					m_pCtrlWnd->ClearJango();
			}
			m_strAcc = acc;
			if (m_pPass)
			{
				const bool bFocus = (m_pAccount && IsChildWindow(m_pAccount->GetSafeHwnd(), GetFocus()->m_hWnd)) ? true : false;
				
				m_pPass->SetWindowText(m_strPswd);
				if (bFocus)
				{
					m_pPass->SetFocus();
					((CEdit*)m_pPass.get())->SetSel(0, -1);
				}
			}

			Transaction();
			InvalidateRect(&m_rectAccName);
		}
		break;

	case UP_GET_JCOD:
		return (LRESULT)GetCode().operator LPCTSTR();

	case UP_GET_CODX:
		return (LRESULT)m_strCODX.operator LPCTSTR();

	case UP_GET_CONDITION:
		return (LRESULT)(LPCTSTR)m_pCtrlWnd->GetJumunCondition();

	case UP_GET_ACCOUNT:
		return (LRESULT)GetAccount().operator LPCTSTR();

	case UP_GET_PASSWORD:
		return (LRESULT)GetPswd().operator LPCTSTR();

	case UP_GET_NAME:
		return (LRESULT)GetName().operator LPCTSTR();

	case UP_GET_CODENAME:
		return (LRESULT)GetCodeName().operator LPCTSTR();

	case UP_SET_GUIDE:
		SetGuide((char*)lParam);
		break;

	case UP_SEND_JUMUN:
		{
		struct _jumun* pJumun = (struct _jumun*)lParam;
		SendJumun(pJumun->cMmgb, pJumun->iPrice, pJumun->iJuno, pJumun->iJqty);
		}
		break;
		
	case UP_SET_CODE:
		SaveBuffer(_T("SELCODE"), CString((char*)lParam));
		SetCode(CString((char*)lParam));
		break;

	case UP_SEND_MAXJMCN:
		SendMaxJmcn((char*)lParam, HIWORD(wParam));
		break;

	case UP_INVALIDATE_HOGA:
		if (IsWindow(m_pHogaWnd->m_hWnd))
			m_pHogaWnd->Invalidate();	
		break;
	}
	return 0;
}


BOOL CMapWnd::OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult) 
{
	if (wParam == ID_CTRL_PASS)
	{

		Transaction();
	}
	
	return CWnd::OnNotify(wParam, lParam, pResult);
}

void CMapWnd::OnHistoryCode()
{
	m_pCodeCtrl->SetHistory(m_pTool->GetHistory("1301"));
}

void CMapWnd::OnCodeSelEndOk()
{
	CString strJCode, strJName;
	strJCode = m_pCodeCtrl->GetEditData();
	if (strJCode.IsEmpty())
		return;

	strJName = m_pTool->GetCodeName(strJCode);
	if (strJName.IsEmpty())
		return;

TRACE("OnCodeSelEndOk()\n");
	m_strJCOD = strJCode;
	m_strJCOD.TrimLeft();
	m_strJCOD.TrimRight();

	Transaction();
}

void CMapWnd::OnButtonCode()
{
	CRect rect;
	GetDlgItem(ID_CTRL_CODEBTN)->GetWindowRect(rect);
	CString strCODE = (char*) m_pwndParent->SendMessage(WM_USER, MAKEWPARAM(codeDLL, MAKEWORD(allCODE, 0)), MAKELPARAM(rect.left, rect.bottom + 2));

	const int iPos = strCODE.Find('\t');
	if (iPos > 0)
	{
		strCODE = strCODE.Left(iPos);

		m_strJCOD = strCODE;
		m_strJCOD.TrimLeft();
		m_strJCOD.TrimRight();
		m_pCodeCtrl->SetEditData(m_strJCOD);
		m_pCodeCtrl->SetFocus();

		Transaction();
	}
}

void CMapWnd::OnButtonSend()
{
	Transaction();
}

void CMapWnd::OnButtonVSize()
{
	m_bHogaSize = TRUE; 
	CRect cRc, hRc, rc;
	GetClientRect(&rc);
	m_pHogaWnd->GetClientRect(&hRc);
	m_pCtrlWnd->GetClientRect(&cRc);

	const int ichangeH = m_iHogaRowH * 10;
	if (m_eDepth == hgDEPTH10)
	{
		m_eDepth = hgDEPTH5;
		m_iHogaType = 5;
		const int itmp = rc.bottom - ichangeH;
		
		SetWindowPos(NULL, 0, 0, rc.Width(), itmp, SWP_NOZORDER | SWP_NOMOVE);
		m_pwndParent->SendMessage(WM_USER, MAKEWPARAM(sizeDLL, NULL), MAKELPARAM(rc.Width(), itmp));

		m_rcGuide.OffsetRect(0, -ichangeH);
		m_pHogaWnd->SetWindowPos(NULL, 0, 0, hRc.Width(), hRc.Height() - ichangeH, SWP_NOZORDER | SWP_NOMOVE);
		m_pCtrlWnd->SetWindowPos(NULL, 0, 0, cRc.Width(), cRc.Height() - ichangeH, SWP_NOZORDER | SWP_NOMOVE);
	}
	else
	{
		m_iHogaType = 10;
		int itmp = m_iOldSize[1];
		if(itmp <= 0) itmp = MAP_HEIGHT;

		m_eDepth = hgDEPTH10;
		SetWindowPos(NULL, 0, 0, rc.Width(), itmp, SWP_NOZORDER | SWP_NOMOVE);
		m_pwndParent->SendMessage(WM_USER, MAKEWPARAM(sizeDLL, NULL), MAKELPARAM(rc.Width(), itmp));

		m_rcGuide.OffsetRect(0, ichangeH);
		m_pHogaWnd->SetWindowPos(NULL, 0, 0, hRc.Width(), hRc.Height() + ichangeH, SWP_NOZORDER | SWP_NOMOVE);
		m_pCtrlWnd->SetWindowPos(NULL, 0, 0, cRc.Width(), hRc.Height() + ichangeH, SWP_NOZORDER | SWP_NOMOVE);
	}

	CString sval;
	sval.Format("%d", m_eDepth);
	SaveBuffer(_T("VTYPE"), sval);

	m_pHogaWnd->SetHogaDepth(m_eDepth);
	m_pCtrlWnd->SetSize(m_eDepth);

//	m_iHogaRowH = m_pHogaWnd->GetRowheight();
	if( !IsWindowVisible()) ShowWindow(SW_SHOW);
}

void CMapWnd::OnButtonConfig()
{
	m_pHogaWnd->DoModalConfigDlg();
}

void CMapWnd::OnButtonReserve()
{
	const int iRet = m_pHogaWnd->SetHogaReserve();
	if (iRet)
		m_pBtnReserve->SetWindowText("호가"); 
	else
		m_pBtnReserve->SetWindowText("Stop");
}

void CMapWnd::ReceiveSise(char* pcData, int iLen)
{
	struct _sise_mod* pMod = (struct _sise_mod *)pcData;

	m_strCODX = m_pTool->ToString(pMod->codx, sizeof(pMod->codx));
	m_strCurr = m_pTool->ToString(pMod->curr, sizeof(pMod->curr));
	m_strDiff = m_pTool->ToString(pMod->diff, sizeof(pMod->diff));
	m_strRate = m_pTool->ToString(pMod->rate, sizeof(pMod->rate));
	m_strGvol = m_pTool->ToString(pMod->gvol, sizeof(pMod->gvol));

//	if (m_bChecking)
//		SetTimer(TT_WAITCHECK, 1000, NULL);
//	else
		SendMicheg();

	m_pHogaWnd->ReceiveSise(pcData, iLen);
	m_pCtrlWnd->ReceiveSise(pcData, iLen);

	Invalidate();
}

/*
void CMapWnd::Alert(CString alert)
{
	int index = alert.Find('\t');
	if (index <= 0)
		return;

	CString val, code = alert.Left(index++);
	code = code.Mid(1);

	if (m_strJCOD != code)
		return;

	alert = alert.Mid(index);
	CMapStringToString	rtmStore;
	CString	symbol;
	for (; !alert.IsEmpty(); )
	{
		index = alert.Find('\t');
		if (index < 0)
			break;
		symbol = alert.Left(index++);
		alert = alert.Mid(index);

		index = alert.Find('\t');
		if (index < 0)
			index = alert.GetLength();
		val = alert.Left(index++);

		rtmStore.SetAt(symbol, val);

		if (alert.GetLength() <= index)
			break;
		alert = alert.Mid(index);
	}

	for (POSITION pos = rtmStore.GetStartPosition(); pos;)
	{
		rtmStore.GetNextAssoc(pos, symbol, val);
		if (symbol == "023")			// 현재가
		{
			m_strCurr = val;
			InvalidateRect(m_rectCurrD);
			m_pCtrlWnd->SetCurr(m_strCurr);
		}
		else if (symbol == "024")		// 전일대비
		{
			m_strDiff = val;
			InvalidateRect(m_rectDiffD);
		}
		else if (symbol == "033")		// 등락율
		{
			m_strRate = val;
			InvalidateRect(m_rectRateD);
		}
		else if (symbol == "027")		// 거래량
		{
			m_strGvol = val;
			InvalidateRect(m_rectGvolD);
		}
	}
}
*/
void CMapWnd::Alert(char* pcData)
{
	CString strAlert = CString(pcData);
	CString total_str = strAlert;
	strAlert = SplitString(total_str,"\n");
	while(!strAlert.IsEmpty())
	{
		CString strCODX = m_pTool->Parser(strAlert, '\t');
		if (strCODX != m_strCODX)
			return;

		CMap<int, int, CString, CString> mapAlert;
		m_pTool->Parsing(strAlert, mapAlert);
		CString strVal;
		if (mapAlert.Lookup(23, strVal))		// 현재가
		{
			m_strCurr = strVal;
			InvalidateRect(m_rectCurrD);
			m_pCtrlWnd->SetCurr(m_strCurr);
		}
		if (mapAlert.Lookup(24, strVal))		// 전일대비
		{
			m_strDiff = strVal;
			InvalidateRect(m_rectDiffD);
		}
		if (mapAlert.Lookup(33, strVal))		// 등락율
		{
			m_strRate = strVal;
			InvalidateRect(m_rectRateD);
		}
		if (mapAlert.Lookup(27, strVal))		// 거래량
		{
			m_strGvol = strVal;
			InvalidateRect(m_rectGvolD);
		}
		strAlert = SplitString(total_str,"\n");
	}
}

void CMapWnd::DrawSiseText(CDC* pDC, CString strData, CRect rect, UINT nFormat, UINT nType, COLORREF color)
{
	if (strData.IsEmpty())
		return;

	nFormat = DT_SINGLELINE | DT_VCENTER | nFormat;
	

	CFont* pfontOld{};
	if (nType & ST_BOLD)
		pfontOld = pDC->SelectObject(m_pfontB);
	else
		pfontOld = pDC->SelectObject(m_pfont);

	rect.DeflateRect(5, 0);
	if (nType & ST_TEXT)
	{
		pDC->DrawText(strData, rect, nFormat);
		pDC->SelectObject(pfontOld);
		return;
	}

	CString strGiho;
	COLORREF clrText = color;//m_clrTFG;
	if (nType & ST_SHGA)
	{
		if (strData[0] == '1')
		{
			strGiho = "↑ ";
			clrText = m_pTool->GetIndexColor(94);
		}
		else if (strData[0] == '2')
		{
			strGiho = "▲ ";
			clrText = m_pTool->GetIndexColor(94);
		}
		else if (strData[0] == '4')
		{
			strGiho = "↓ ";
			clrText = m_pTool->GetIndexColor(95);
		}
		else if (strData[0] == '5')
		{
			strGiho = "▼ ";	
			clrText = m_pTool->GetIndexColor(95);
		}

		strData = strData.Mid(1);
	}

	int iPos{};
	if (nType & ST_COLOR)
	{
		iPos = strData.Find('+');
		if (iPos >= 0)
			clrText = m_pTool->GetIndexColor(94);

		iPos = strData.Find('-');
		if (iPos >= 0)
			clrText = m_pTool->GetIndexColor(95);
	}

	CString strPercent;
	if (nType & ST_PERCENT)
		strPercent = " %";

	if (nType & ST_REMOVE)
	{
		strData.Remove('+');
		strData.Remove('-');
	}

//	if (nType & ST_NORMAL)
//	{
//		CString strPre = _T("");
//		iPos = strData.Find('+');
//		if (iPos >= 0)
//		{
//			strPre = "+";
//			strData.Remove('+');
//		}

//		iPos = strData.Find('-');
//		if (iPos >= 0)
//		{
//			strPre = "-";
//			strData.Remove('-');
//		}

//		strData.Remove(' ');
//		iPos = strData.Find('.');
//		CString strValue;
//		if (iPos >= 0)
//			strValue.Format("%.2f", atof(strData));
//		else
//			strValue.Format("%d", atoi(strData));
//
//		if (atof(strData) == 0.0)
//			strData = "";
//		else
//			strData = strPre + strValue;
//	}

	if (nType & ST_COMMA)
	{
		strData = m_pTool->AddComma(strData);
	}

	const COLORREF clrTextOld = pDC->SetTextColor(clrText);
	pDC->DrawText(strGiho + strData + strPercent, rect, nFormat);

	pDC->SelectObject(pfontOld);
	pDC->SetTextColor(clrTextOld);
}

void CMapWnd::SendJumun(char cJflg, int iPirce, int iJuno, int iJqty)
{
	
	//본사직원 주문 안나가도록 예외 처리 [ Start ]
	if (!(long)m_pwndParent->SendMessage(WM_USER, MAKEWPARAM(variantDLL, orderCC), 0L))
	{
		if (!(long)m_pwndParent->SendMessage(WM_USER, MAKEWPARAM(variantDLL, orderCCx), 0L))
		{
			MessageBox("본사직원은 주문이 불가합니다.","IBK투자증권");
			return;
		}	
	}
	//본사직원 주문 안나가도록 예외 처리 [ End ]

	

	m_pPass->GetWindowText(m_strPswd);
	m_strPswd.TrimLeft(), m_strPswd.TrimRight();
	/*
	if (m_strPswd.GetLength()<4)
	{
		MessageBox("비밀번호를 확인하십시오.","IBK투자증권");
		return;
	}
	*/

	if (iJqty == 0 && (cJflg == '1' || cJflg == '2'))	// 주문수량이 0이고 매도나 매수인 경우
	{	
		iJqty = m_pCtrlWnd->GetJumunCount();
		
	}
	if (cJflg == '1' || cJflg == '2')
	{
		//MessageBox(m_strListedQuantity);
		const BOOL bRet = CheckupMistakeOrder(this, iPirce, iJqty, CString(m_strCODX.Mid(1,6)), m_strListedQuantity);
		
		if (!bRet)
			return;
	}

	CString strLedger = m_ledger.LedgerTR(MAP_NO, m_strAcc.Left(3), "");
	
	m_iLenLedger = strLedger.GetLength();

	CString strTemp;
	struct _jumun_mid	Mid;
	FillMemory(&Mid, sizeof(Mid), ' ');

	CopyMemory(Mid.rcnt, "1", 1);
	Mid.odgb = '4';
	Mid.mkgb = m_pHogaWnd->GetMarket();
	Mid.mmgb = cJflg;
	Mid.mdgb = '2';	// 정정취소시 전부
	CopyMemory(Mid.acno, m_strAcc.operator LPCTSTR(), min(sizeof(Mid.acno), m_strAcc.GetLength()));

#ifdef	ENCTEST
	CString passX;
	if (m_pTool)
	{
		passX = m_pTool->GetEncPswd(m_strPswd, m_strAcc.Left(8));
		CopyMemory(Mid.pswd, passX, min(sizeof(Mid.pswd), passX.GetLength()));
	}

#else
	CopyMemory(Mid.pswd, m_strPswd.operator LPCTSTR(), min(sizeof(Mid.pswd), m_strPswd.GetLength()));
#endif
	
	CopyMemory(Mid.code, m_strCODX.operator LPCTSTR(), min(sizeof(Mid.code), m_strCODX.GetLength()));
	
	if (cJflg == '1' || cJflg == '2')
		strTemp.Format("%d", iJqty);
	else
		strTemp = "0";		// fixed by schbang 2006.03.17, 정정취소시 전부체크이므로 수량을 0으로 세팅
	CopyMemory(Mid.jqty, strTemp.operator LPCTSTR(), min(sizeof(Mid.jqty), strTemp.GetLength()));
	strTemp.Format("%d", iPirce);
	CopyMemory(Mid.jprc, strTemp.operator LPCTSTR(), min(sizeof(Mid.jprc), strTemp.GetLength()));
	strTemp = m_pCtrlWnd->GetJumunCondition();
	Mid.jmgb[0] = strTemp.GetAt(0);
//	CopyMemory(Mid.jmgb, strTemp.operator LPCTSTR(), min(sizeof(Mid.jmgb), strTemp.GetLength()));
//	if (cJflg == '3') //정정
//		CopyMemory(Mid.hogb, "99", sizeof(Mid.hogb));
//	else
		CopyMemory(Mid.hogb, "00", sizeof(Mid.hogb));
	strTemp.Format("%d", iJuno);
	CopyMemory(Mid.ogno, strTemp.operator LPCTSTR(), min(sizeof(Mid.ogno), strTemp.GetLength()));
	Mid.prgb = 'X';

	CString jmid((char *)&Mid, sz_JUMUNMID);
#ifdef	_FILE_DEBUG0
	CTime curr = CTime::GetCurrentTime();
	CString timeS, gubnS;
	timeS.Format("[%02d/%02d %02d:%02d:%02d]  ", curr.GetMonth(), curr.GetDay(), curr.GetHour(), curr.GetMinute(), curr.GetSecond());
	switch (cJflg)
	{
	case '1':	gubnS = "매도";	break;
	case '2':	gubnS = "매수";	break;
	case '3':	gubnS = "정정";	break;
	case '4':	gubnS = "취소";	break;
	}
	dFile	<< endl << "주문send" << timeS << m_strCODX << "  " << gubnS << "                    ==> " << jmid << endl;
	startTick = GetTickCount();
	QueryPerformanceCounter(&startCount);
	
#endif
	strLedger += jmid;

	TRACE("[@@다중주문!! : %s\n", jmid);
	m_pTool->SendTR(TN_JUMUN, strLedger, TK_JUMUN, US_ENC | US_CA);
}

void CMapWnd::SetCode(CString strJCOD)
{
	
	if (m_pTool->GetCodeName(strJCOD).IsEmpty())
		return;

	if (m_pCodeCtrl)
		m_pCodeCtrl->SetEditData(strJCOD);
	m_strJCOD = strJCOD;
	Transaction();
}

void CMapWnd::SendMaxJmcn(CString strJmdg, int iMmgb)
{
	if (m_strAcc.IsEmpty())
	{
		SetGuide("계좌번호를 입력하세요.");
		return;
	}
	/*
	m_pPass->GetWindowText(m_strPswd);
	m_strPswd.TrimLeft(), m_strPswd.TrimRight();
	if (m_strPswd.IsEmpty())
	{
		SetGuide("비밀번호를 입력하세요.");
		return;
	}
	*/

//@@ pswd
//	m_nWaitTrType = 2;
//	m_iMmgb = iMmgb;
//	m_strJmdg = strJmdg;
//	CheckPswd(m_strAcc, m_strPswd);


	CString strData = m_ledger.LedgerTR(MAP_NO, m_strAcc.Left(3), "", SN_MAXJMCT);
	m_iLenLedger = strData.GetLength();
	
	struct _maxjmct_mid mid;
	FillMemory(&mid, L_maxjmct_mid, ' ');
	
	CString strVal(_T(""));
	
	CopyMemory(&mid.in, _T("00001"), sizeof(mid.in));
	mid.zBnsTp[0] = iMmgb == 1 ? _T('1') : _T('2');
	CopyMemory(&mid.zAcntNo, (LPCTSTR)m_strAcc, m_strAcc.GetLength());
	CopyMemory(&mid.zPwd, (LPCTSTR)m_strPswd, m_strPswd.GetLength());
	
	if (m_pTool->GetCodeType(m_strJCOD) == elwType)
		strVal.Format("J%s", m_strJCOD);
	else
		strVal.Format("A%s", m_strJCOD);
	
	CopyMemory(&mid.zIsuNo, (LPCTSTR)strVal, strVal.GetLength());
	
	strVal.Format("+%09s", strJmdg);
	strVal += _T(".00");
	CopyMemory(&mid.dOrdPrc, (LPCTSTR)strVal, strVal.GetLength());
	
	strData += CString((char *)&mid, L_maxjmct_mid);
	
	int nTrKey = 0;
	
	switch (iMmgb)
	{
	case 1:	// 매도
		nTrKey = TK_MDJMCN;
		break;
	case 2:	// 매수
		nTrKey = TK_MSJMCN;
		break;
	case 3:	// 최대
		nTrKey = TK_MAXMSJMCN;
		break;
	default:
		return;
	}
	
	m_pTool->SendTR(TN_MAXJMCT, strData, nTrKey);


/*
	CString strMid;
	strMid.Empty();
	strMid += m_strAcc + '\t' + m_strAcc.Right(2) + '\t';

#ifdef	ENCTEST
	CString passX;
	if (m_pTool)
	{
		passX = m_pTool->GetEncPswd(m_strPswd, m_strAcc.Left(8));
		strMid += passX + '\t';
	}
#else
	strMid += m_strPswd + '\t';
#endif

	
	int iTrKey;
	switch (iMmgb)
	{
	case 1:	// 매도
		strMid += "2\t";
		iTrKey = TK_MDJMCN;
		break;
	case 2:	// 매수
		strMid += "1\t";
		iTrKey = TK_MSJMCN;
		break;
	case 3:	// 최대
		strMid += "1\t";
		iTrKey = TK_MAXMSJMCN;
		break;
	default:
		return;
	}

	strMid += m_strCODX + '\t';
	strMid += strJmdg + '\t';
	strMid += "00\t";
	strMid += "00\t";	// 시간외 구분 -> 호가구분 
TRACE("SendMaxJmcn = %s\n", strMid);
	m_pTool->SendTR(TN_MAXJMCT, strLedger + strMid, iTrKey);
*/
}

CString CMapWnd::GetPswd()
{
	m_pPass->GetWindowText(m_strPswd);
	m_strPswd.TrimLeft(), m_strPswd.TrimRight();
	return m_strPswd;
}

void CMapWnd::SetGuide(CString strGuide)
{
	KillTimer(TM_GUIDE);
	strGuide.TrimLeft();
	strGuide.TrimRight();
	//AfxMessageBox(strGuide);
	m_strGuide = strGuide;

	InvalidateRect(&m_rcGuide);

//	@@ 지우지 말아 달라네.. ㅡㅡ
//	SetTimer(TM_GUIDE, 3000, NULL);
}

void CMapWnd::OnTimer(UINT nIDEvent) 
{
	if (nIDEvent == TM_GUIDE)
	{
		KillTimer(TM_GUIDE);
		
		m_strGuide = "";
		InvalidateRect(&m_rcGuide);
	}
	else if (nIDEvent == TT_WAITCHECK)
	{
		KillTimer(TT_WAITCHECK);

/*		if (m_bChecking)
			SetTimer(TT_WAITCHECK, 1000, NULL);
		else
			SendMicheg();
*/	}

	if(nIDEvent == TT_CHKVSIZE)
	{
		KillTimer(TT_CHKVSIZE);
		CString sval = GetBuffer(_T("VTYPE"));
		if(!sval.IsEmpty())
		{
			if(sval == "1")
			{
				m_eDepth = hgDEPTH10;
				OnButtonVSize();
			}
		}
	}

	CWnd::OnTimer(nIDEvent);
}

BOOL CMapWnd::IsChildWindow(HWND hParentWnd, HWND hChild)
{
	HWND hTempChild = ::GetWindow(hParentWnd, GW_CHILD);

	while(hTempChild)
	{
		if(hTempChild == hChild)
			return TRUE;

		if(IsChildWindow(hTempChild, hChild) != FALSE)
			return TRUE;

		hTempChild = ::GetWindow(hTempChild, GW_HWNDNEXT);
	}

	return FALSE;
}

void CMapWnd::SendMicheg(CString strLedger)
{
	if (m_strAcc.IsEmpty())
	{
		SetGuide("계좌번호를 입력하세요.");
		return;
	}

	m_pPass->GetWindowText(m_strPswd);
	m_strPswd.TrimLeft(), m_strPswd.TrimRight();
	/*
	if (m_strPswd.IsEmpty())
	{
		SetGuide("비밀번호를 입력하세요.");
		return;
	}
	if (m_strPswd.GetLength()<4)
	{
		MessageBox("비밀번호를 확인하십시오.","IBK투자증권");
		return;
	}
	*/

//@@ pswd
//	CString m_strLedger = strLedger;
//	m_nWaitTrType = 3;
//	CheckPswd(m_strAcc, m_strPswd);

	struct _ledgerH ledger;
	
	if (strLedger.IsEmpty())
	{
		CopyMemory(&ledger, m_ledger.LedgerTR(MAP_NO, m_strAcc.Left(3), ""), L_ledgerH);
	}
	else
	{
		CopyMemory(&ledger, (LPCTSTR)strLedger, L_ledgerH);
		CString strNext = CString((char*)&ledger.nkey, sizeof(ledger.nkey));
		CopyMemory(&ledger, m_ledger.LedgerTR(MAP_NO, m_strAcc.Left(3), ""), L_ledgerH);

		CopyMemory(&ledger.nkey, (LPCTSTR)strNext, sizeof(ledger.nkey));
		ledger.fkey[0] = '7';
	}

	CString strData = CString((char*)&ledger, L_ledgerH);
	m_iLenLedger = strData.GetLength();

	struct _miche_mid Mid;
	FillMemory(&Mid, sz_MICHEMID, ' ');
	CopyMemory(Mid.accn, m_strAcc, sizeof(Mid.accn));

#ifdef	ENCTEST
	CString passX;
	if (m_pTool)
	{
		passX = m_pTool->GetEncPswd(m_strPswd, m_strAcc.Left(8));
		CopyMemory(Mid.pswd, passX, passX.GetLength());
	}
#else
	CopyMemory(Mid.pswd, m_strPswd, m_strPswd.GetLength());
#endif
	
	Mid.dsgb[0] = '0';
	Mid.otim[0] = '0';
	Mid.dlgb[0] = '2';
	Mid.sort[0] = '1';
	CString strCode = m_strCODX.Right(m_strCODX.GetLength() - 1);
	CopyMemory(Mid.cod2, strCode, min(sizeof(Mid.cod2), strCode.GetLength()));
	CopyMemory(Mid.tcod, _T("00"), sizeof(Mid.tcod));

	CString strTR = strData + CString((char *)&Mid, sizeof(Mid));
	m_pTool->SendTR(TN_MICHE, strTR, TK_MICHE, US_ENC);
}

void CMapWnd::OnRButtonUp(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	CPoint ptScreen = point;
	ClientToScreen(&ptScreen);
	m_pTool->PopupMenu(MAP_NAME, ptScreen.x, ptScreen.y);
	
	CWnd::OnRButtonUp(nFlags, point);
}

void CMapWnd::SendSymbol()
{
	CString strText;
	strText.Format("1301%c%s\t17413\t", 0x7f, m_strJCOD);
	m_pTool->SendTR(TN_SYMBOL, strText, TK_SYMBOL);
}

void CMapWnd::Trigger(CString strTrigger)
{
	while (!strTrigger.IsEmpty())
	{
		CString str = m_pTool->Parser(strTrigger, '\n'), sym = m_pTool->Parser(str, '\t');
		if (!sym.CompareNoCase("CODX") || !sym.CompareNoCase("1301"))
		{
			 ((CIB885200App*)AfxGetApp())->DeleteKey(_T("SELCODE"));
			SetCode(str);
		}
		else if (!sym.CompareNoCase("AN1A"))  // 2008.07.22 변경분  "AN10=>AN01"
		{
			m_pAccount->TriggerAccount(str);
		}
	}
}

void CMapWnd::Domino(CString strDomino)
{
	while (!strDomino.IsEmpty())
	{
		CString str = m_pTool->Parser(strDomino, '\n'), sym = m_pTool->Parser(str, '\t');
		if (!sym.CompareNoCase("1301"))
		{
			CString sCode = GetBuffer(_T("SELCODE"));
			if(!sCode.IsEmpty()) str = sCode;
			SetCode(str);
		}
	}
}

void CMapWnd::Palette()
{
	LoadRoundBitmap();
	m_clrBG = m_pTool->GetIndexColor(COLOR_BK);
	m_clrTH = m_pTool->GetIndexColor(COLOR_TABLEHEAD);
	m_clrGH = m_pTool->GetIndexColor(COLOR_GRIDHEAD);
	m_clrDT	= m_pTool->GetIndexColor(COLOR_DATA);
	m_clrTFG = m_pTool->GetIndexColor(COLOR_TEXT);
	m_ppenLine = m_pTool->RequestPen(m_pTool->GetIndexColor(COLOR_LINE), 1, PS_SOLID);
	m_clrGridHdrTxt = m_pTool->GetIndexColor(COLOR_GRIDHEADTXT);



	m_CmdBgColor = m_pTool->GetIndexColor(CMD_BACK_INDEX);
	m_ContentsBgColor = m_pTool->GetIndexColor(CONTENTS_BACK_INDEX);
	m_GuideBgColor = m_pTool->GetIndexColor(GUIDE_BACK_INDEX);
	m_ContentsBorderColor = m_pTool->GetIndexColor(CONTENTS_BORDER_INDEX);
	m_GuideTxtColor = m_pTool->GetIndexColor(GUIDE_TXT_INDEX);


	m_pHogaWnd->Palette();
	m_pCtrlWnd->Palette();
	m_pwndSymbol->SendMessage(WM_USER, MAKEWPARAM(DLL_SETPAL, 0));

	Invalidate();
}

BOOL CMapWnd::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message) 
{
	if (m_bWait)
	{
		SetCursor(LoadCursor(NULL, IDC_WAIT));
		return TRUE;
	}
	
	return CWnd::OnSetCursor(pWnd, nHitTest, message);
}

void CMapWnd::SetWaitMode(BOOL bWait)
{
	m_bWait = bWait;

	if (bWait)
	{
		SetChildWait(bWait);
		SetCursor(LoadCursor(NULL, IDC_WAIT));
	}
	else
	{
		SetChildWait(bWait);
		SetCursor(LoadCursor(NULL, IDC_ARROW));
	}
}

void CMapWnd::SetChildWait(BOOL bWait)
{
	CWnd* pWnd = GetWindow(GW_CHILD);

	while (pWnd)
	{
		pWnd->EnableWindow(!bWait);
		pWnd = pWnd->GetWindow(GW_HWNDNEXT);
	}
}

void CMapWnd::CheckPswd(CString strAccount, CString strPassword)
{
	struct	pmid mid;
	FillMemory(&mid, L_pmid, ' ');

	CopyMemory(mid.accn, strAccount, strAccount.GetLength());
	CopyMemory(mid.pswd, strPassword, strPassword.GetLength());
	
	m_bChecking = TRUE;
	SetTimer(TT_CHECKPSWD, 1000, NULL);
	m_pTool->SendTR(TN_CHECKPSWD, (char*)&mid, L_pmid, TK_CHECKPSWD);
}

void CMapWnd::ParsingCheckPswd(CString strData, int nLen)
{
	if (strData.IsEmpty())
		return;

	if (strData.GetAt(0) != _T('1'))
	{
		CString strMsg = strData.Mid(1);
		strMsg.TrimRight();
		strMsg.TrimLeft();
		SetGuide(strMsg);

		m_pPass->SetText(_T(""));
		m_pPass->SetFocus();
		((CIB885200App*)AfxGetApp())->DeleteKey(m_strAcc);
		((CIB885200App*)AfxGetApp())->DeleteKey(_T("PASS"));
		
		return;
	}

	m_bChecking = FALSE;

	switch (m_nWaitTrType)
	{
	case 1:
		{
			if (m_pNotify)
			{
				m_pNotify->SendJango(m_strAcc, m_strPswd);
				
				if (!m_strPswd.IsEmpty())
					SetWaitMode(TRUE);
			}
		}
		break;
	case 2:
		{			
			CString strData = m_ledger.LedgerTR(MAP_NO, m_strAcc.Left(3), "", SN_MAXJMCT);
			m_iLenLedger = strData.GetLength();
			
			struct _maxjmct_mid mid;
			FillMemory(&mid, L_maxjmct_mid, ' ');
			
			CString strVal(_T(""));
			
			CopyMemory(&mid.in, _T("00001"), sizeof(mid.in));
			mid.zBnsTp[0] = m_iMmgb == 1 ? _T('1') : _T('2');
			CopyMemory(&mid.zAcntNo, (LPCTSTR)m_strAcc, m_strAcc.GetLength());
			CopyMemory(&mid.zPwd, (LPCTSTR)m_strPswd, m_strPswd.GetLength());
			
			if (m_pTool->GetCodeType(m_strJCOD) == elwType)
				strVal.Format("J%s", m_strJCOD);
			else
				strVal.Format("A%s", m_strJCOD);
			
			CopyMemory(&mid.zIsuNo, (LPCTSTR)strVal, strVal.GetLength());
			
			strVal.Format("+%09s", m_strJmdg);
			strVal += _T(".00");
			CopyMemory(&mid.dOrdPrc, (LPCTSTR)strVal, strVal.GetLength());
			
			strData += CString((char *)&mid, L_maxjmct_mid);
			
			int nTrKey = 0;
			
			switch (m_iMmgb)
			{
			case 1:	// 매도
				nTrKey = TK_MDJMCN;
				break;
			case 2:	// 매수
				nTrKey = TK_MSJMCN;
				break;
			case 3:	// 최대
				nTrKey = TK_MAXMSJMCN;
				break;
			default:
				return;
			}
			
			m_pTool->SendTR(TN_MAXJMCT, strData, nTrKey);
		}
		break;

	case 3:
		{

		}
		break;
	}

	KillTimer(TT_CHECKPSWD);
}

LRESULT CMapWnd::OnMyMessage2(WPARAM wParam, LPARAM lParam)
{
	//CString rcb = (char*)lParam;
	//MessageBox((char*)lParam);
	m_pPass->SetWindowText((char*)lParam);
	//MessageBox("Received: "+rcb);
	return 1;
}

bool CMapWnd::IsNumber(CString str)
{
    bool bRet = true;
    const int nLen = str.GetLength();
    for( int i=0 ; i < nLen ; ++i)
    {
        if( isdigit(str.GetAt(i)) == FALSE)
        {
            bRet = false;
            break;
        }
    }

    return bRet;
}

CString CMapWnd::SplitString(CString &strData, CString strToken)
{
	CString sResult = strData;
	if(strToken == "") return sResult;
	const int iFind = strData.Find(strToken);
	if(iFind > -1 ) 
	{
		sResult = strData.Left(iFind);
		strData = strData.Right(strData.GetLength() - iFind - strToken.GetLength() );  
	}
	return sResult;
}

void CMapWnd::OnSize(UINT nType, int cx, int cy) 
{
	CWnd::OnSize(nType, cx, cy);
	if(m_bHogaSize)
	{
		m_bHogaSize = FALSE;
		return;
	}

	// TODO: Add your message handler code here
//	m_bChangeFont = FALSE;
	if(m_iOldSize[0] == 0 && m_iOldSize[1] == 0)
	{
		m_iOldSize[0] = cx ;
		m_iOldSize[1] = cy ;
		return;
	}

	Resize(cx, cy);
	SetTimer(TT_CHKVSIZE, 10, NULL);
}


void CMapWnd::Resize(int cx, int cy)
{
	if(!m_bInit) return;
	if(m_iOldSize[0] == cx && m_iOldSize[1] == cy) return;
	double lcalcrate[2] = {( (float)cx - (float)m_iOldSize[0] ) / (float)m_iOldSize[0], 
							( (float)cy - (float)m_iOldSize[1] ) /(float)m_iOldSize[1]};
	
	m_iOldSize[0] = cx ;
	m_iOldSize[1] = cy ;


	CRect rc, wrc;
	GetClientRect(&rc);
	int iw{}, ih{}, itop{};

	m_pAccount->GetWindowRect( &wrc);
	ScreenToClient(&wrc);
	iw = wrc.Width() , ih = wrc.Height() ;
	iw = iw + (int)(lcalcrate[0] * (float)iw );
	ih = ih + (int)(lcalcrate[1] * (float)ih);

	//계좌컨트롤
	itop = wrc.top;
	wrc.right = wrc.left + iw;
	wrc.bottom = wrc.top + ih;
	m_pAccount->MoveWindow(wrc); 
	m_pAccount->ChangeFont(m_iOldFont, _T("굴림체") ); 
	

	//계좌명
	iw = m_rectAccName.Width() , ih = m_rectAccName.Height() ;
	iw = iw + (int)(lcalcrate[0] * (float)iw);
	ih = ih + (int)(lcalcrate[1] * (float)ih);

	m_rectAccName.SetRect(wrc.right + GAP, wrc.top, wrc.right + iw, wrc.top+ih);
	int iright = wrc.right + iw, ibottom = wrc.top+ih;
	const int igap = 3;

	//패스워드
	m_pPass->GetWindowRect( &wrc);
	ScreenToClient(&wrc);
	iw = wrc.Width() , ih = wrc.Height() ;

	iw = iw + (int)(lcalcrate[0] * (float)iw);
	ih = ih + (int)(lcalcrate[1] * (float)ih);
	wrc.SetRect(iright + GAP, wrc.top, iright + GAP + iw, wrc.top+ih);

	m_pPass->SetFont(m_pfont);
	m_pPass->MoveWindow(wrc); 
	iright = wrc.right;
	
	const int ibtngap = 1;
	//조회버튼
	m_pBtnSend->GetWindowRect( &wrc);
	ScreenToClient(&wrc);
	iw = wrc.Width() , ih = wrc.Height() ;
	iw = iw + (int)(lcalcrate[0] * (float)iw);
	ih = ih + (int)(lcalcrate[1] * (float)ih);
	wrc.SetRect(iright + GAP, wrc.top, iright + ibtngap + iw, wrc.top+ih);
	m_pBtnSend->SetFont(m_pfont);
	m_pBtnSend->MoveWindow(wrc); 

	//스탑버튼
	iright = wrc.right;
	m_pBtnReserve->GetWindowRect( &wrc);
	ScreenToClient(&wrc);
	iw = wrc.Width() , ih = wrc.Height() ;
	iw = iw + (int)(lcalcrate[0] * (float)iw);
	ih = ih + (int)(lcalcrate[1] * (float)ih);
	wrc.SetRect(iright + ibtngap, wrc.top, iright + ibtngap + iw, wrc.top+ih);
	m_pBtnReserve->SetFont(m_pfont);
	m_pBtnReserve->MoveWindow(wrc); 
	iright = wrc.right;

	////설정버튼
	m_pBtnConfig->GetWindowRect( &wrc);
	ScreenToClient(&wrc);
	iw = wrc.Width() , ih = wrc.Height() ;
	iw = iw + (int)(lcalcrate[0] * (float)iw);
	ih = ih + (int)(lcalcrate[1] * (float)ih);
	wrc.SetRect(iright + ibtngap, wrc.top, iright + ibtngap + iw, wrc.top+ih);
	m_pBtnConfig->SetFont(m_pfont);
	m_pBtnConfig->MoveWindow(wrc); 
	iright = wrc.right;
	
	//5/10 호가 버튼
	m_pBtnVSize->GetWindowRect( &wrc);
	ScreenToClient(&wrc);
	iw = wrc.Width() , ih = wrc.Height() ;
	iw = iw + (int)(lcalcrate[0] * (float)iw);
	ih = ih + (int)(lcalcrate[1] * (float)ih);
	wrc.SetRect(iright + GAP, wrc.top, iright + GAP + iw, wrc.top+ih);
	m_pBtnVSize->SetFont(m_pfont);
	m_pBtnVSize->MoveWindow(wrc); 
	iright = wrc.right;
	ibottom = wrc.top+ih + GAP;


	//종목코드 컨트롤
	ih = m_rectCurrS.Height() ;
	ih = ih + (int)(lcalcrate[1] * (float)ih);

	int isymbolleft = 0, isymboltop=0;
	m_pCodeCtrl->GetWindowRect( &wrc);
	ScreenToClient(&wrc);
	wrc.top = ibottom;
	iw = wrc.Width() ;//, ih = wrc.Height() ;
	iw = iw + (int)(lcalcrate[0] * (float)iw);
//	ih = ih + (int)(lcalcrate[1] * (float)ih);
	isymbolleft = wrc.left;
	isymboltop = ibottom;
	wrc.SetRect(isymbolleft, ibottom, wrc.left+iw, ibottom+ih);
	m_pCodeCtrl->SetFont(m_pfont);
	m_pCodeCtrl->MoveWindow(wrc); 
	iright = wrc.right;

	//종목검색버튼
	m_pBtnCode->GetWindowRect( &wrc);
	ScreenToClient(&wrc);
//	ih = wrc.Height() ;
//	ih = ih + (int)(lcalcrate[1] * (float)ih);
	iw = wrc.Width();
	iw = iw + (int)(lcalcrate[0] * (float)iw);
	wrc.SetRect(iright, ibottom, iright+iw, ibottom+ih-1);
	m_pBtnCode->SetFont(m_pfont);
	m_pBtnCode->MoveWindow(wrc); 


	//종목명 컨트롤
	ibottom = isymboltop+ih;
	wrc.SetRect(isymbolleft, ibottom, wrc.right, ibottom+ih);
	m_pwndSymbol->MoveWindow(wrc);
	m_pwndSymbol->SendMessage(WM_USER, MAKEWPARAM(DLL_SETFONT, m_iOldFont), (LPARAM)_T("굴림체"));
	iright = wrc.right;
	

	//현재가 lbl
	iw = m_rectCurrS.Width() , ih = m_rectCurrS.Height() ;
	iw = iw + (int)(lcalcrate[0] * (float)iw );
	ih = ih + (int)(lcalcrate[1] * (float)ih);
	m_rectCurrS.SetRect(iright + GAP, isymboltop, iright + iw + GAP, isymboltop+ih);

	//대비 lbl
	iw = m_rectDiffS.Width();
	iw = iw + (int)(lcalcrate[0] * (float)iw);
	m_rectDiffS.SetRect(m_rectCurrS.right - 1, m_rectCurrS.top, m_rectCurrS.right + iw, isymboltop+ih);

	//등락률 lbl
	iw = m_rectRateS.Width();
	iw = iw + (int)(lcalcrate[0] * (float)iw );
	m_rectRateS.SetRect(m_rectDiffS.right - 1, m_rectCurrS.top, m_rectDiffS.right + iw, isymboltop+ih);

	//거래량 lbl
	iw = m_rectGvolS.Width();
	iw = iw + (int)(lcalcrate[0] * (float)iw);
	m_rectGvolS.SetRect(m_rectRateS.right - 1, m_rectCurrS.top, HOGA_WIDTH, isymboltop+ih);

	ibottom = isymboltop+ih;
	//현재가 ,대비, 등락률, 거래량 시세
	m_rectCurrD = m_rectCurrS;	m_rectCurrD.top = ibottom; m_rectCurrD.bottom = ibottom+ih;
	m_rectDiffD = m_rectDiffS;	m_rectDiffD.top = ibottom; m_rectDiffD.bottom = ibottom+ih;
	m_rectRateD = m_rectRateS;	m_rectRateD.top = ibottom; m_rectRateD.bottom = ibottom+ih;
	m_rectGvolD = m_rectGvolS;	m_rectGvolD.top = ibottom; m_rectGvolD.bottom = ibottom+ih;
	ibottom += ih;

	//하단메세지창
	iw = m_rcGuide.Width();
	ih = m_rcGuide.Height() ;
	iw = iw + (int)(lcalcrate[0] * (float)iw);
	ih = ih + (int)(lcalcrate[1] * (float)ih);
//	m_rcGuide.SetRect(HORI_GAP, ibottom, rc.right-GAP, rc.bottom-BOT_GAP);
	itop = cy - INNER_FRAME_SPACE - ih;
	m_rcGuide.SetRect(FRAME_SPACE, itop, cx-FRAME_SPACE, cy-INNER_FRAME_SPACE);


	const int icontentbottom = m_rcGuide.top - INNER_FRAME_SPACE-INNER_FRAME_SPACE;
//	icontentbottom = cy - m_rectGvolD.bottom + INNER_FRAME_SPACE -m_rcGuide.top - INNER_FRAME_SPACE-INNER_FRAME_SPACE;

	//호가윈도우
	m_pHogaWnd->GetWindowRect( &wrc);
	ScreenToClient(&wrc);
	iw = wrc.Width(), ih = wrc.Height() ;
	iw = iw + (int)(lcalcrate[0] * (float)iw);
//	ih = ih + (int)(lcalcrate[1] * (float)ih);

	itop = m_rectGvolD.bottom + INNER_FRAME_SPACE + OUTER_FRAME_SPACE + INNER_FRAME_SPACE;
	wrc.SetRect(wrc.left, itop, wrc.left+iw, icontentbottom);

	m_pHogaWnd->ChangeFont(lcalcrate, m_pfont, m_pfontB);
	m_pHogaWnd->MoveWindow(wrc);
	iright = m_rectGvolS.right = m_rectGvolD.right = wrc.right;

	//우측 윈도우
	m_pCtrlWnd->GetWindowRect( &wrc);
	ScreenToClient(&wrc);
	iw = wrc.Width();
	iw = iw + (int)(lcalcrate[0] * (float)iw);

//	iright += INNER_FRAME_SPACE + OUTER_FRAME_SPACE+ INNER_FRAME_SPACE+INNER_FRAME_SPACE;
	const int icontroliright = cx - INNER_FRAME_SPACE - FRAME_SPACE;
	const int ileft = iright + OUTER_FRAME_SPACE + INNER_FRAME_SPACE+INNER_FRAME_SPACE;
	wrc.SetRect(ileft, OUTER_FRAME_SPACE, icontroliright, icontentbottom);
	m_pCtrlWnd->MoveWindow(wrc);
	m_pCtrlWnd->ChangeFont(lcalcrate, m_pfont, m_pfontB);

	m_iHogaRowH = m_pHogaWnd->GetRowheight();
}


void CMapWnd::LoadBuffer()
{
	m_strPswd = GetBuffer(_T("PASS"));
	m_pPass->SetWindowText(m_strPswd);
	CString sval = GetBuffer(_T("VTYPE"));
	if(sval.IsEmpty() || sval == "0") ShowWindow(SW_SHOW);

}

void CMapWnd::SaveBuffer(CString sKey, CString sData)
{
	CIB885200App* pApp = (CIB885200App*)AfxGetApp();
	pApp->SaveBuffer(sKey, sData);
}

CString CMapWnd::GetBuffer(CString sKey)
{
	CIB885200App* pApp = (CIB885200App*)AfxGetApp();
	return pApp->GetBuffer(sKey);
}