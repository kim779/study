// MapWnd.cpp : implementation file
//

#include "stdafx.h"
#include "IB100300.h"
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
#include "grid/GridCtrl.h"
#include <math.h>
#include <comutil.h>

#include "../../H/interMSG.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/*
#define	_FILE_DEBUG		// time check 하기 위한 debug
#ifdef	_FILE_DEBUG
#include <fstream.h>
#include "Windows.h"
#endif
*/

struct  _jinfo3 {             /* 신 HTS 종목정보 (사이즈 줄인것)   */
    char    codx[12];       /* 종목코드         		*/
    char    hnam[50];       /* 종목명           		*/   
	char    jgub[1];		// 장구분 : '1':거래소 '2':코스닥, '3' :제3시장, '4':ETF, '6':ELW, '7':ELW-조기종료
	char	jrat[3];		// 증거금율
	char	nrat[3];		// 신용증거금율 20070718 
    char    krgb[12];       /* 관리구분         		*/   	
	char	rned[2];		// 임의종료
	char	dist[2];		// 배분정보
	char    vist[1];		// 변동성 완화 장치
	char    vitime[6];		// 변동성 완화 장치 발동 시각
	char	vitype[1];		// 변동성 완화 장치 타입(정적,동적,정/동적)
	char	fill[12];
};
#define L_jinfo3	sizeof(struct _jinfo3)

struct	_jinfo4 {
	char    codx[12];		// 종목코드
	char    hnam[50];		// 종목명
	char    jgub[1];		// 장구분 : '1':거래소 '2':코스닥, '3' :제3시장, '4':ETF, '6':ELW, '7':ELW-조기종료 
	char    size[12];		// 기업규모
	char    k2gb[20];		// 소구분 전기,전자
	char    usun[10];		// 보통주/우선주
	char    ksmm[2];		// 결산월
	char    amga[20];		// 액면가 8->20 단위포함
	char    jbkm[20];		// 자본금(단위:억) 12->20 단위포함
	char    siga[12];		// 전일시가총액(단위:억)
	char    sjjs[12];		// 상장주식수(단위:천주)
	char	jrat[3];		// 증거금율
	char	nrat[3];		// 신용증거금율 20070718 
	char    krgb[12];		// "거래정지", "거래중단", "관리종목", "이상급등", "투자유의", "정상"
	char    jggb[12];		// 장운영 구분
	char    lock[12];		// 락구분
	char    dyga[8];		// 대용가
	char    ujgb[12];		// 업종구분
	char    ujcd[6];		// RTS 업종코드
	char    jisu[8];		// 업종지수
	char    diff[8];		// 업종전일대비
	char    rate[8];		// 업종등략율
	char    gvol[12];		// 업종거래량(단위:천주)
	char    unio[6];		// KRX 100 (0:일반 1:통합지수)
	char	rned[2];		// 임의종료
	char	dist[2];		// 배분정보
	char	frgb[8];		// 외국인 보유비중
	char	rlnk[1];		// 대출등급('A'~'E', 'X', ' ')
	char    vist[1];		// 변동성 완화 장치
	char    vitime[6];		// 변동성 완화 장치 발동 시각
	char	vitype[1];		// 변동성 완화 장치 타입(정적,동적,정/동적)
	char	sijang[40];		// 시장조치
};

#define L_jinfo4	sizeof(struct _jinfo4)

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
#define ID_GRID_JANGO	0x11

#define	ENCTEST

// Timer ID
#define TM_GUIDE		0x01
#define TT_CHECKPSWD	0x02
#define TT_WAITCHECK	0x03
#define TT_CHKVSIZE		0x04
#define TT_INITDATA		0x05
#define TT_SENDTR		0x06


struct _chkPwdMid {
	char in [5];
	char acctNo[20];
	char password[8];
	//char zTrxTp[1];
};
#define L_chkPwdMid	sizeof(struct _chkPwdMid)

struct _chkPwdMod {
	struct _chkPwdMid in;
	char out[5];
	char zAvalYn;
	//char lPwdContErrCnt[9];
	//char lAtmPwdContErrCnt[9];
	//char lMtrsfPwdContErrCnt[9];
	//char zAcntNm[40];
};

/* 2013.07.24 KSJ 펀드 반영보류
//2013.07.10 KSJ SACMQ227용으로 변경
struct _chkPwdMid {
	char in [5];
	char acntClssTp[1];	//2013.07.10 KSJ 계층구분 추가 종합계좌 1, 개별계좌 2
	char acctNo[20];
	char password[8];
	//char zTrxTp[1];
};
#define L_chkPwdMid	sizeof(struct _chkPwdMid)

struct _chkPwdMod {
	struct _chkPwdMid in;
	char out[5];
	char zAcntNm[40]; //2013.07.10 KSJ 
	char zAvalYn;	//1 일치, 0 불일치
	char zAppCnt[6]; //2013.07.10 KSJ 적용횟수
	char aPwdContErrCnt[10]; //2013.07.10 KSJ 비밀번호연속오류횟수
	//char lPwdContErrCnt[9];
	//char lAtmPwdContErrCnt[9];
	//char lMtrsfPwdContErrCnt[9];
	//char zAcntNm[40];
};
*/

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

#define CNT_JANGO_COL	12

#define DEFAULT_JANGO_CNT	1

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

	m_hiSha256 = NULL;

	m_pJangoGrid = NULL;
	m_bShowJango = false;
	m_iSelectRow = -1;
	m_strLoanDate.Empty();
	m_strJangoType = "00";
	m_bCheckPass = false;
	m_bClickGrid = false;
	m_strCreditType = "00";


}

CMapWnd::~CMapWnd()
{
	if (m_pAccount)
	{
		m_pAccount->DestroyWindow();
	}

	//if (m_pHogaWnd) {
	//	m_pHogaWnd.reset();
	//	m_pHogaWnd = nullptr;
	//}

	if (m_pwndSymbol)
	{
		if (IsWindow(m_pwndSymbol->GetSafeHwnd()))
			m_pwndSymbol->SendMessage(WM_CLOSE);
		m_pwndSymbol->Detach();
		delete m_pwndSymbol;
// 		m_pwndSymbol = NULL;
	}
	if (m_hiSymbol)
	{
		AfxFreeLibrary(m_hiSymbol);
		m_hiSymbol = NULL;
	}
	if (m_hiSha256)
	{
		AfxFreeLibrary(m_hiSha256);
		m_hiSha256 = NULL;
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

	CIB100300App* pApp = (CIB100300App*)AfxGetApp(); 
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
	int cy = VERT_GAP+VERT_GAP;
	int ileft = cx;
	int itop = cy;

	CRect cRc;
	cRc.SetRect(cx, cy, cx + ACC_WIDTH, cy + CTRL_HEIGHT);
	m_pAccount = std::make_unique<CAccountCtrl>(m_pwndParent, this, m_pTool->GetHome());
	m_pAccount->CreateEx(WS_EX_TOPMOST, NULL, "ACCN", WS_VISIBLE | WS_CHILD | WS_TABSTOP, cRc, this, 0);
	// 2008.07.22 변경분
	//m_pAccount->createAccountCtrl("AN10", TK_ACCOUNT, m_pTool->GetIndexColor(7));

	m_pAccount->createAccountCtrl("AN1A", TK_ACCOUNT, m_pTool->GetIndexColor(7));
	m_rectAccName.SetRect(cRc.right + GAP, cRc.top, cRc.right + GAP + 88, cRc.bottom);

	cRc.SetRect(m_rectAccName.right + GAP, cRc.top, m_rectAccName.right + GAP + 60, cRc.bottom);
	m_pPass = std::make_unique<CfxEdit>(m_pfont);
	m_pPass->Create(WS_CHILD | WS_VISIBLE | ES_LEFT | ES_PASSWORD | WS_TABSTOP, cRc, this, ID_CTRL_PASS);
	m_pPass->Init(esNORMAL, NULL, "0", RGB(255, 255, 255), RGB(0, 0, 0), alLEFT);
	((CEdit*)m_pPass.get())->LimitText(8);
	m_pPass->SetWindowText(m_strPswd);

	const int ibtnw2 = 36;
	const int ibtngap = 1;

	cRc.SetRect(cRc.right + GAP, cRc.top, cRc.right + GAP + ibtnw2, cRc.bottom);
	m_pBtnSend = std::make_unique<CfxImgButton>();
	m_pBtnSend->Create("\n조회", cRc, this, ID_CTRL_SEND, FALSE, TRUE);
	m_pBtnSend->SetWindowText("조회");
	m_pBtnSend->SetFont(m_pfont);
	m_pBtnSend->SetDisableDraw(FALSE);
	m_pBtnSend->SetTextColor(m_pTool->GetIndexColor(69)); 

	cRc.SetRect(cRc.right + ibtngap, cRc.top, cRc.right + ibtngap + ibtnw2, cRc.bottom);
	m_pBtnReserve = std::make_unique<CfxImgButton>();
	m_pBtnReserve->Create("\nStop", cRc, this, ID_CTRL_RESERVE, FALSE, TRUE);
	m_pBtnReserve->SetWindowText("Stop");
	m_pBtnReserve->SetFont(m_pfont);
	m_pBtnReserve->SetDisableDraw(FALSE);
	m_pBtnReserve->SetTextColor(m_pTool->GetIndexColor(69));

	cRc.SetRect(cRc.right + ibtngap, cRc.top, cRc.right + ibtngap + ibtnw2, cRc.bottom);
	m_pBtnConfig = std::make_unique<CfxImgButton>();
	m_pBtnConfig->Create("\n설정", cRc, this, ID_CTRL_CONFIG, FALSE, TRUE);
	m_pBtnConfig->SetWindowText("설정");
	m_pBtnConfig->SetFont(m_pfont);
	m_pBtnConfig->SetDisableDraw(FALSE);
	m_pBtnConfig->SetTextColor(m_pTool->GetIndexColor(69));

	cRc.SetRect(cRc.right + GAP, cRc.top, cRc.right + GAP + 20, cRc.bottom);
	cRc.SetRect(cRc.right + GAP, cRc.top, cRc.right + GAP + 20, cRc.bottom);
	m_pBtnVSize = std::make_unique<CfxImgButton>();
	m_pBtnVSize->Create("\n5/10호가전환", cRc, this, ID_CTRL_VSIZE, FALSE, TRUE);
	m_pBtnVSize->SetFont(m_pfont);
	m_pBtnVSize->SetDisableDraw(FALSE);
	m_pBtnVSize->SetTextColor(m_pTool->GetIndexColor(69));


// line 2
	cy = cRc.bottom + GAP;
	cRc.SetRect(cx, cy, cx + 88, cy + CTRL_HEIGHT + 1);
	m_pCodeCtrl = std::make_unique<CfxCodeCtrl>(this, CString(m_pTool->GetHome()) + "\\tab");
	m_pCodeCtrl->Create(this, cRc, ID_CTRL_CODE);
	m_pCodeCtrl->SetUnit(GU_CODE);
	m_pCodeCtrl->SetFont(m_pfont);


	cRc.SetRect(cRc.right + GAP, cRc.top, cRc.right + GAP + 20, cRc.top + CTRL_HEIGHT);
	m_pBtnCode = std::make_unique<CfxImgButton>();
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
	m_pCtrlWnd = std::make_unique<CControlWnd>(m_pwndParent, m_pTool.get());
	m_pCtrlWnd->Create(NULL, "control", WS_CHILD | WS_VISIBLE | WS_CLIPCHILDREN | WS_CLIPSIBLINGS, cRc, this, ID_CTRL_WND);

// hoga wnd
//	m_pHogaWnd->Init(m_pCtrlWnd);
	m_pHogaWnd = std::make_unique<CHogaWnd>(m_pTool.get(), m_pCtrlWnd.get(), this);
	m_pHogaWnd->Create(NULL, "hoga", WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN, rectHoga, this, ID_HOGA_WND);


//jango wnd
	m_pJangoGrid = std::make_unique<CGridCtrl>();
	m_pJangoGrid->Create(cRc, this, ID_GRID_JANGO);
	m_pJangoGrid->ShowWindow(SW_SHOW);
	m_pJangoGrid->SetFont(m_pfont);
	SetJangoGrid();

// guide
//	m_rcGuide.SetRect(HORI_GAP, cRc.bottom + itop, MAP_WIDTH - GAP, MAP_HEIGHT - BOT_GAP);
	itop = wndrc.bottom - GUIDE_HEIGHT - INNER_FRAME_SPACE;
	m_rcGuide.SetRect(FRAME_SPACE, itop, wndrc.right-FRAME_SPACE, itop + GUIDE_HEIGHT);

// notify wnd
	m_pNotify = std::make_unique< CNotify>(m_pwndParent, TK_NOTIFYCTRL);
	m_pNotify->CreateNotify(this, CRect(0,0,0,0), m_pTool->GetHome());
	m_pNotify->SetCtrlWnd(m_pCtrlWnd.get());
	m_pNotify->SetMainWnd(this);


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

	LoadIni();	//2012.10.30 KSJ 버튼 설정 로드

	m_sDept = m_pTool->Variant(deptCC);

	return 0;
}

void CMapWnd::LoadIni()
{
	CString data = "", path = "";
	path.Format("%s\\%s\\%s\\%s.ini", m_pTool->GetHome(), USRDIR, m_pTool->GetName(), MAP_NAME);
	
	//load BTN select 0이면 스탑, 1이면 호가
	const int iBtn = GetPrivateProfileInt("BTN", "SELECT", 0, (LPCSTR)path);

	if(iBtn != 0)
		OnButtonReserve();
}

void CMapWnd::SaveIni()
{
	CString data = "", path = "", sBtn = "";
	
	path.Format("%s\\%s\\%s\\%s.ini", m_pTool->GetHome(), USRDIR, m_pTool->GetName(), MAP_NAME);

	m_pBtnReserve->GetWindowText(data);

	if(!data.Compare("Stop"))
		sBtn = "0";
	else
		sBtn = "1";

	WritePrivateProfileString("BTN", "SELECT", sBtn, path);
}

void CMapWnd::InitSymbol(CRect rect)
{
	CString	text;
	CWnd*	(APIENTRY* axCreate)(CWnd*, void*) = NULL;

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
			symbolparam.options = _T("/a89/b91/c92/d69/i99/s1003");

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

CString CMapWnd::GetEncPassword(CString sPswd)
{
	CString dllPath;
	dllPath.Format(_T("%s%s"), m_pTool->GetHome(), _T("\\dev\\CX_SHA256.DLL"));
	CString strRetrun;
	
	if(m_hiSha256 == NULL)
	{
		m_hiSha256 = LoadLibrary(dllPath);
		
		if (!m_hiSha256)
		{
			TRACE("CX_SHA256 컨트롤 생성 실패1");
			return "";
		}
	}
	
	if(m_hiSha256)
	{
		typedef long (WINAPI *GETSHAFUNC)(char*, int);	
		GETSHAFUNC func = (GETSHAFUNC)GetProcAddress(m_hiSha256, "axEncrypt");
		
		if (func)
		{
			strRetrun =  (char*)func(sPswd.GetBuffer(sPswd.GetLength()),sPswd.GetLength());
		}


	}	

	return strRetrun;
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
	}
	if (m_pCodeCtrl)
	{
		::DestroyWindow(m_pCodeCtrl->m_hWnd);
	}

	if (m_pBtnVSize)
	{
		::DestroyWindow(m_pBtnVSize->m_hWnd);
	}
	if (m_pBtnConfig)
	{
		::DestroyWindow(m_pBtnConfig->m_hWnd);
	}
	if (m_pBtnReserve)
	{
		::DestroyWindow(m_pBtnReserve->m_hWnd);
	}
	if (m_pBtnSend)
	{
		::DestroyWindow(m_pBtnSend->m_hWnd);
	}
	if (m_pPass)
	{
		::DestroyWindow(m_pPass->m_hWnd);
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

void CMapWnd::Transaction(bool bjango)
{
CString slog;
slog.Format("[1003] Transaction bjango=[%d] m_bClickGrid=[%d] m_strJCOD =[%s]\n", bjango, m_bClickGrid, m_strJCOD);
OutputDebugString(slog);

	if (m_strJCOD.IsEmpty())
	{
		SetGuide("종목을 선택하십시오");	
		return;
	}
	
	if(bjango || !m_bClickGrid)
	{
		m_pCtrlWnd->SetPrgbRadio(0);
		ClearAllSelectRow();
		m_strJangoType = "00";
		m_strLoanDate.Empty();
		m_strCreditType = "00";
	}
	m_bClickGrid = false;

	m_pCtrlWnd->ClearMaip();
	m_pCtrlWnd->ClearJango();
	m_pCtrlWnd->ClearChegGrid();
	struct	_sise_mid Mid;
	FillMemory(&Mid, sz_SISEMID, ' ');
	//m_strCODX = "A"+m_strJCOD;
	CopyMemory(Mid.code, m_strJCOD, m_strJCOD.GetLength());
	m_pTool->SendTR(TN_SISE, (char*)&Mid, sizeof(Mid), TK_SISE);

	SendSymbol();

	//////////////////////////////////////////////////////////////////////////
	//2014.05.20 KSJ 상장주식수 가져오기
	CString strText;
	strText.Format("1301%c%s\t12374\t", 0x7f, m_strJCOD);
	m_pTool->SendTR(TN_SYMBOL, strText, TK_LISTEDQUANTITY);
	//////////////////////////////////////////////////////////////////////////

	if (m_strAcc.IsEmpty())
	{
		SetGuide("계좌번호를 입력하십시오");	
		return;
	}

	m_pPass->GetWindowText(m_strPswd);
	m_strPswd.TrimLeft(), m_strPswd.TrimRight();
	
	if (m_strPswd.IsEmpty())
	{
		SetGuide("비밀번호를 입력하십시오");	
		return;
	}

	if (m_pPass->IsWindowEnabled())
		SendPSWD("","");
	else
	{
		if (m_pNotify && bjango)
			m_pNotify->SendJango(m_strAcc, m_strPswd, "0", "0", true);
	}
	
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

	CRect rc, cRc, rc2, rc3, wrc, rc4, rc5;
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

	rc5 = rc3;
	rc5.top = rc3.bottom + OUTER_FRAME_SPACE;
	rc5.right = rc4.right;
	rc5.bottom = rc5.top + BT_JANGO_HEIGHT - OUTER_FRAME_SPACE;

	if(!m_bShowJango)
		rc5.SetRect(0,0,0,0);
	
	memDC.FillSolidRect(rc2, m_CmdBgColor);
	memDC.FillSolidRect(rc3, m_ContentsBgColor);
	memDC.FillSolidRect(rc4, m_ContentsBgColor);
	memDC.FillSolidRect(rc5, m_ContentsBgColor);

	DrawCornerRoundBitmap(&memDC, rc2);
	DrawCornerRoundBitmap(&memDC, rc3);
	DrawCornerRoundBitmap(&memDC, rc4);
	DrawCornerRoundBitmap(&memDC, rc5);

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
	CString ecod;
	CString strMsg;
	CString stmp;
	switch(LOBYTE(LOWORD(wParam)))
	{
	case DLL_OUB:
		key = HIBYTE(LOWORD(wParam));

		stmp.Format("[memo][IB100300]  key =[%s]   len=[%d]\r\n", GetMemoTrKeyType(key), HIWORD(wParam));
		OutputDebugString(stmp);
		if ((key >= TRKEY_MEMO_POPUPOOP && key <= TRKEY_MEMO_CHECK))
		{
			m_pwndSymbol->SendMessage(WM_USER, wParam, lParam);
			break;
		}

		switch (key)
		{
		case 255:		// CX_Account
		case 254:
		case 250:
			m_pAccount->m_pAccountCtrl->SendMessage(WM_USER, wParam, lParam);
			break;
		case TK_SISE:
			ReceiveSise((char*)lParam, HIWORD(wParam));
			break;
		case TK_CHECKPSWD:
			ParsingCheckPswd((char*)lParam, HIWORD(wParam));
			break;
		case TK_JUMUN:
			{	
				nLen = HIWORD(wParam);

				{
					// 2011.05.19 기업은행 임직원 주문제한 처리 - by warship
					_ledgerH *ledger = (_ledgerH*)lParam;
					if (ledger->ecod[0]!='0')
					{
						CString strEcod(ledger->ecod, sizeof(ledger->ecod));
						CString strData(&ledger->emsg[4], sizeof(ledger->emsg)-4);
						strData.TrimLeft(); strData.TrimRight();
						
						CString emsg = "[" + strEcod + "] " + strData;
						if (strEcod=="9997")
							MessageBox(emsg, "IBK투자증권");
						else if(strEcod == "4131")
						{
							strMsg.Format("	 <매수/매도 주문금액 한도 조과>   \n"\
								"주문금액이 개인 30억, 법인60억을 초과하는 이상주문입니다.\n"\
								"가격, 수량 등 착오주문 여부를 재확인하시기 바랍니다.\n"\
								"주문이 거부되었습니다.");
								
							::MessageBox(m_hWnd,strMsg,MAP_TITLE,MB_OK);
						}
						else if (strEcod == "3735" || strEcod == "4727")  //최선집행미등록 고객
						{
							m_pTool->CreateView(typeVIEW, "IB999963", "/t");
						}
						SetGuide(emsg);
					}

				}
				
				{
					CString strData(((char*)lParam) + L_ledgerH, nLen - L_ledgerH);
					if (strData.GetLength() < 21)
						return 0;
					
					ecod = strData.Mid(16,4);

					if (ecod=="4273")
					{
						MessageBox("투자성향 미등록 및 등급이 낮은 고객이십니다.\n성향 등록 및 변경후 재시도 바랍니다.","IBK투자증권", MB_OK);
						m_pTool->CreateView(typeVIEW, "IB821200", "/t");
					}
					
					strData.Delete(0, 21);
					
					SetGuide(strData);
				}
			}

			TRACE("%s\n", (char*)(lParam + m_iLenLedger));
			

			break;
		case TK_MDJMCN:
		case TK_MSJMCN:
		case TK_MAXMSJMCN:
		case TK_MICHE:
		case TK_CHECK207:
		case TK_FEE:
		case TK_CRMDJMCN:
		case TK_CRMSJMCN:
		case TK_CRMAXMSJMCN:
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
					{
						MessageBox(strMsg,"IBK투자증권",MB_OK);
						if (m_pNotify)
							m_pNotify->m_acnt = "";
					}
					else
						SetGuide(strMsg);
					if (key == TK_CHECK207)
					{
						char* dat = (char*)lParam;
						struct _chkPwdMod* mod = (struct _chkPwdMod*)&dat[0];
						CString valYn = "";
						valYn = mod->zAvalYn;

						if ((!valYn.IsEmpty())&&(valYn == "Y"))
						{

							m_pPass->EnableWindow(true);
							m_pPass->SetFocus();
						}
					}
					//SetWaitMode(FALSE);

					return 0;
				}
				/* 2013.07.24 KSJ 펀드 반영 보류
				else if(!strErrCode.Compare("0136"))	//2013.07.10 KSJ SACMQ227은 조회완료로 오고 zAvalYn를 구분해서 실제 비번이 맞는지 체크해야함.
				{
					if (key == TK_CHECK227)
					{
						char* dat = (char*)lParam;
						struct _chkPwdMod* mod = (struct _chkPwdMod*)&dat[0];
						CString valYn = "";
						valYn = mod->zAvalYn;
												
						if ((!valYn.IsEmpty())&&(valYn == "0"))
						{
							m_pPass->EnableWindow(true);
							m_pPass->SetFocus();
							return 0;
						}
					}
				}
				*/
				/*
				else
				{
					MessageBox(strMsg,"IBK투자증권-미체결내역조회",MB_OK);
				}
				*/
				if (key == TK_CHECK207)  
				{
					m_bCheckPass = true;
					//계좌인증 성공
					if (m_pNotify)
						m_pNotify->SendJango(m_strAcc, m_strPswd, "0", "0", true);   //test 수정해줘야 함
		//				m_pNotify->SendJango(m_strAcc, m_strPswd, "0", "0");


					m_pPass->EnableWindow(false);
				}

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
					//SetWaitMode(FALSE);
				}
				else if (key == TK_MSJMCN)
				{
					TRACE("KEY_MSJMCN\n");
					m_pCtrlWnd->SetMaxJmcn((char*)(lParam + L_maxjmct_mid), TK_MSJMCN);
					//SetWaitMode(FALSE);
				}
				else if (key == TK_MAXMSJMCN)
				{
					TRACE("KEY_MAXMSJMCN\n");
					m_pCtrlWnd->SetMaxJmcn((char*)(lParam + L_maxjmct_mid), TK_MAXMSJMCN);
					//SetWaitMode(FALSE);
				}
				else if (key == TK_FEE)	//2015.03.16 KSJ 매체수수료
				{
					m_pCtrlWnd->SetFee((char*)(lParam + L_pihocfee_mid), TK_FEE);
				}
				else if (key == TK_CRMDJMCN)
				{
					TRACE("KEY_MDJMCN\n");
					m_pCtrlWnd->SetMaxJmcn((char*)(lParam + L_maxCrjmct_mid), TK_CRMDJMCN);
					//SetWaitMode(FALSE);
				}
				else if (key == TK_CRMSJMCN)
				{
					TRACE("KEY_MSJMCN\n");
					m_pCtrlWnd->SetMaxJmcn((char*)(lParam + L_maxCrjmct_mid), TK_CRMSJMCN);
					//SetWaitMode(FALSE);
				}
				else if (key == TK_CRMAXMSJMCN)
				{
					TRACE("KEY_MAXMSJMCN\n");
					m_pCtrlWnd->SetMaxJmcn((char*)(lParam + L_maxCrjmct_mid), TK_CRMAXMSJMCN);
					//SetWaitMode(FALSE);
				}
			}
			break;
		case TK_VISYM:
			{
				char* pdata = (char*)lParam;
				CString stmp;
				stmp.Format("%s", (char*)lParam);
				m_pHogaWnd->m_sVIUp = m_pTool->Parser(stmp, '\t');
				stmp.TrimRight();
			//	m_pHogaWnd->m_sVIDown = stmp;
				m_pHogaWnd->m_sVIDown = m_pTool->Parser(stmp, '\t');;
				stmp.Format("[1003] %s  %s", m_pHogaWnd->m_sVIUp, m_pHogaWnd->m_sVIDown);
				OutputDebugString(stmp);
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
			else
			{
				struct _jinfo3 jinfo;
				ZeroMemory(&jinfo, L_jinfo3);
				CopyMemory(&jinfo, (void*)lParam, L_jinfo3);
				m_sMarketgubn = CString(jinfo.krgb, sizeof(jinfo.krgb));
				m_sMarketgubn.TrimRight();
				m_pHogaWnd->m_strKrgb = m_sMarketgubn;
				m_pHogaWnd->m_strMkms = CString(jinfo.fill, 1);
			}
			break;
		case TK_SEARCH_MARKETMSG:
			{
				struct _jinfo4 jinfo;
				ZeroMemory(&jinfo, L_jinfo4);
				CopyMemory(&jinfo, (void*)lParam, L_jinfo4);	
				
				m_pHogaWnd->m_strMkms = CString(jinfo.sijang, sizeof(jinfo.sijang));
			
			}
			break;
		case TK_LISTEDQUANTITY:	//2014.05.20 KSJ 상장주식수 가져오기
			m_strListedQuantity = (char*)lParam;
			m_strListedQuantity = m_strListedQuantity.Mid(0, m_strListedQuantity.Find('\t'));
			break;
		case oKEY:	//2012.11.13 KSJ 클릭시 종목명 컨트롤 나오게 하기
			{
				int	value;
				CString	text;

				text  = CString((char *)lParam, HIWORD(wParam));
				value = text.Find('\t');
				if (value != -1)
				{
					WPARAM	wParam{};
					
					if(value == 7)
					{
						m_rts  = text.Left(value++);
						text   = text.Mid(value);
						wParam = MAKEWPARAM(MAKEWORD(DLL_OUB, oKEY), (text.GetLength() > 6) ? text.GetLength() : 0);
						m_pwndSymbol->SendMessage(WM_USER, wParam, (LPARAM)text.operator LPCTSTR());
					}
					else
					{
						m_rts  = text.Left(value++);
						//				text   = text.Mid(value);
						wParam = MAKEWPARAM(MAKEWORD(DLL_OUB, oKEY), (text.GetLength() > 6) ? text.GetLength() : 0);
						m_pwndSymbol->SendMessage(WM_USER, wParam, (LPARAM)text.operator LPCTSTR());
					}
				}
			}
			break;
		}
		break;
	//case DLL_ALERT:
 //		Alert((char*)lParam);
 //		m_pHogaWnd->ReceiveAlert((char*)lParam);
	//	break;
	case DLL_ALERTx:
		{
			CString strCode;
			struct _alertR* alertR;
			alertR = (struct _alertR*)lParam;

			strCode = alertR->code;

			Alert(alertR);
			m_pHogaWnd->ReceiveAlert(alertR);

// 			if(!strCode.IsEmpty() &&)
// 			DWORD* data;
// 			CString strTemp, strData;
// 			data = (DWORD*)alertR->ptr[0];
// 
// 			for(int ii = 0; ii < maxSYMBOL; ii++)
// 			{
// 				if(data[ii])
// 				{
// 					strTemp.Format("%03d\t", ii);
// 					strData += strTemp;
// 					strData += (char *)data[ii];
// 					strData += "\t";
// 				}
// 			}
// 
			//2016.02.16 KSJ 종목명컨트롤에 실시간 이벤트 전달
			m_pwndSymbol->SendMessage(WM_USER, wParam, lParam);

		}
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
		{
	//			CString ss;
	//			ss.Format("%d==%s",  HIBYTE(LOWORD(wParam)), lParam);
	//			AfxMessageBox(ss);

		}
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
	case UP_GET_PRGBTYPE:
		{
			return GetCondition();
		}
		break;
	case UP_SHOW_JANGO:
		{
			m_bHogaSize = TRUE;
			
			if(HIWORD(wParam) == 1)
			{
				if(m_bShowJango)
					return 1;
				else
					m_bShowJango = true;
			}
			else
				m_bShowJango = !m_bShowJango;

			CString data = "", path = "", sBtn = "";
			path.Format("%s\\%s\\%s\\%s.ini", m_pTool->GetHome(), USRDIR, m_pTool->GetName(), MAP_NAME);
			WritePrivateProfileString("JANGO", "SHOW", m_bShowJango==1?"1":"0", path);

			CRect rc;
			GetClientRect(&rc);
			int itmp  = 0;

			if(m_bShowJango)
				itmp = rc.bottom + BT_JANGO_HEIGHT ;
			else
				itmp = rc.bottom - BT_JANGO_HEIGHT ;

			SetWindowPos(NULL, 0, 0, rc.Width(), itmp, SWP_NOZORDER | SWP_NOMOVE);
			m_pwndParent->SendMessage(WM_USER, MAKEWPARAM(sizeDLL, NULL), MAKELPARAM(rc.Width(), itmp));

			if(m_bShowJango)
				m_pJangoGrid->ShowWindow(SW_SHOW);
			else
				m_pJangoGrid->ShowWindow(SW_HIDE);

			if(m_bShowJango)
				m_rcGuide.OffsetRect(0, BT_JANGO_HEIGHT);
			else
				m_rcGuide.OffsetRect(0, -BT_JANGO_HEIGHT);

			Invalidate();
		}
		break;
	case 100:	// CAccountCtrl 계좌변경시 수신 Message
		{
			m_pPass->EnableWindow(true);
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

				DeleteAllJangoRow();   //계좌변경시
			}

			m_strAcc = acc;
			if (m_pPass)
			{
				m_pPass->SetWindowText(m_strPswd);

				CWnd *focusWnd = GetFocus();
				if (focusWnd)
				{
					const bool bFocus = (m_pAccount && IsChildWindow(m_pAccount->GetSafeHwnd(), focusWnd->GetSafeHwnd())) ? true : false;
					if (bFocus)
					{
						m_pPass->SetFocus();
						((CEdit*)m_pPass.get())->SetSel(0, -1);
					}
				}
			}

			Transaction(true);
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

	case UP_GET_LISTEDQUANTITY:	//2014.06.23 KSJ 상장주식수
		return (LRESULT)m_strListedQuantity.operator LPCTSTR();

	case UP_SEND_JUMUN:
		{
		struct _jumun* pJumun = (struct _jumun*)lParam;
		SendJumun(pJumun->cMmgb, pJumun->iPrice, pJumun->iJuno, pJumun->iJqty, pJumun->bMarketPrice);
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
		Transaction(true);
	}
	else if(wParam == ID_GRID_JANGO)
	{
		CString stemp;
		CString slog;
	
		NM_GRIDVIEW *nmgv = (NM_GRIDVIEW *)lParam;
		const int imessae = nmgv->hdr.code;
		const int nRow = nmgv->iRow;
		const int nCol = nmgv->iColumn;
		
		if(imessae != GVN_SELCHANGING)
			return 0;

		if(nRow == 0)
			return 0;

	//	if(!m_bCheckPass)
	//		return 0;
		if(m_pJangoGrid->GetItemText(nRow, 1).IsEmpty())   
			return 0;

//slog.Format("[1003] onnotify row=[%d] col=[%d] message=[%d] m_iSelectRow=[%d]\n", nRow, nCol, imessae,m_iSelectRow);
//OutputDebugString(slog);

		if(m_pJangoGrid->GetItemText(nRow, 11) == "05")  //유통대주
		{
			SetGuide("유통대주 주문화면[1019]을 이용하십시요!!");
			ClearAllSelectRow();
			m_iSelectRow = nRow;	
			SelectJangoRow(m_iSelectRow);
			m_pJangoGrid->Invalidate();
			
			return 1;
		}

		if(m_iSelectRow != nRow)
		{
			ClearAllSelectRow();
			m_iSelectRow = nRow;	
			SelectJangoRow(m_iSelectRow);
			m_pJangoGrid->Invalidate();
		}

		//신용인지 대출인지 확인 
		if(m_pJangoGrid->GetItemText(nRow, 10) == "00")   // 보통
		{
			m_strLoanDate.Empty();
			m_pCtrlWnd->SetPrgbRadio(0);
		}	
		else 
		{
			
			if(m_pJangoGrid->GetItemText(nRow, 11) == "03" || m_pJangoGrid->GetItemText(nRow, 11) == "01")  //유통융자 01 ,  자기융자 03
			{	
				stemp.Format("%s", m_pJangoGrid->GetItemText(nRow, 8));
				stemp.TrimRight();
				m_strLoanDate = stemp;
				m_strLoanDate.Replace("/", "");
				m_pCtrlWnd->SetPrgbRadio(1, m_pJangoGrid->GetItemText(nRow, 11));
			}
			else if(m_pJangoGrid->GetItemText(nRow, 11) == "80")  //대출
			{	
				stemp.Format("%s", m_pJangoGrid->GetItemText(nRow, 8));
				stemp.TrimRight();
				m_strLoanDate = stemp;
				m_strLoanDate.Replace("/", "");
				m_pCtrlWnd->SetPrgbRadio(2);
			}
			
		}
		
		m_strJangoType = m_pJangoGrid->GetItemText(nRow, 10);
		m_strCreditType = m_pJangoGrid->GetItemText(nRow, 11);
		
		stemp.Format("%s", m_pJangoGrid->GetItemText(nRow, 9));
		if(!stemp.IsEmpty() && nRow > 0)
		{
			if (stemp.GetAt(0) == 'J' || stemp.GetAt(0) == 'A' || stemp.GetAt(0) == 'Q')
				stemp = stemp.Mid(1);

			m_bClickGrid = true;
			SetCode(stemp);
		}
	}
	
	return CWnd::OnNotify(wParam, lParam, pResult);
}

void CMapWnd::OnHistoryCode()
{
	m_pCodeCtrl->SetHistory(m_pTool->GetHistory("1301"));
}

void CMapWnd::OnCodeSelEndOk()
{
CString slog;
	CString strJCode, strJName;
	strJCode = m_pCodeCtrl->GetEditData();
slog.Format("[1003] OnCodeSelEndOk [%s]\n", strJCode);
OutputDebugString(slog);
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

	CString strParam;
	strParam.Format("1301\t%s", m_strJCOD);
	
	m_pTool->SetHistory(strParam);
	m_pTool->Trigger(strParam);
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

		CString strParam;
		strParam.Format("1301\t%s", m_strJCOD);
		
		m_pTool->SetHistory(strParam);
		m_pTool->Trigger(strParam);
		//2012.11.19 KSJ 종목다이얼로그를 띄워서 선택했을때 리스트에 추가해준다.
		m_strJName = m_pTool->GetCodeName(m_strJCOD);
		m_pCtrlWnd->AddJCod(m_strJCOD, m_strJName);
		//KSJ
	}
}

void CMapWnd::OnButtonSend()
{
	Transaction(true);
}

void CMapWnd::OnButtonVSize()
{
	m_bHogaSize = TRUE; 
	CRect cRc, hRc, rc, jRc;
	GetClientRect(&rc);
	m_pHogaWnd->GetClientRect(&hRc);
	m_pCtrlWnd->GetClientRect(&cRc);
	m_pJangoGrid->GetWindowRect(&jRc);
	ScreenToClient(jRc);

	const int ichangeH = m_iHogaRowH * 10;
	if (m_eDepth == hgDEPTH10)
	{
		m_eDepth = hgDEPTH5;
		m_iHogaType = 5;
		const int itmp = rc.bottom - ichangeH;

		jRc.OffsetRect(0, -ichangeH);
		m_pJangoGrid->MoveWindow(jRc);

		if(m_bShowJango)
			m_pJangoGrid->ShowWindow(SW_SHOW);
		else
			m_pJangoGrid->ShowWindow(SW_HIDE);

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

		itmp += m_bShowJango * BT_JANGO_HEIGHT;

		jRc.OffsetRect(0, ichangeH);
		m_pJangoGrid->MoveWindow(jRc);

		if(m_bShowJango)
			m_pJangoGrid->ShowWindow(SW_SHOW);
		else
			m_pJangoGrid->ShowWindow(SW_HIDE);

		m_eDepth = hgDEPTH10;
	
		SetWindowPos(NULL, 0, 0, rc.Width(), itmp, SWP_NOZORDER | SWP_NOMOVE);
		m_pwndParent->SendMessage(WM_USER, MAKEWPARAM(sizeDLL, NULL), MAKELPARAM(rc.Width(), itmp));

		m_rcGuide.OffsetRect(0, ichangeH);
		m_pHogaWnd->SetWindowPos(NULL, 0, 0, hRc.Width(), hRc.Height() + ichangeH, SWP_NOZORDER | SWP_NOMOVE);
		m_pCtrlWnd->SetWindowPos(NULL, 0, 0, cRc.Width(), /*hRc.Height() + ichangeH*/itmp-32-(BT_JANGO_HEIGHT * m_bShowJango) , SWP_NOZORDER | SWP_NOMOVE);
	}

	CString sval;
	sval.Format("%d", m_eDepth);
	SaveBuffer(_T("VTYPE"), sval);

	m_pHogaWnd->SetHogaDepth(m_eDepth);
	m_pCtrlWnd->SetSize(m_eDepth);

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

	SaveIni();	//2012.10.30 KSJ 버튼 설정 저장
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
	
	if (!m_pPass->IsWindowEnabled())
	{
		if(m_pNotify)	m_pNotify->SetJango();
	}
	
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
	CString s023, s034, s032;
	
	s023 = s034 = s032 = "";
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
			s023 = strVal;
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

		if (mapAlert.Lookup(34, strVal))		// 시간
		{
			s034 = strVal;
			InvalidateRect(m_rectGvolD);
		}

		if (mapAlert.Lookup(32, strVal))		// 체결량
		{
			s032 = strVal;
			InvalidateRect(m_rectGvolD);
		}

		strAlert = SplitString(total_str,"\n");
	}

	if(s023.IsEmpty() == FALSE && s034.IsEmpty() == FALSE && s032.IsEmpty() == FALSE)
		m_pCtrlWnd->SetRealData(s034, s023, s032);
}

void CMapWnd::Alert(struct _alertR* alertR)
{
	CString s;
	CString s023, s034, s032;

	if (alertR->code != m_strCODX)
		return; 
	
	DWORD* data{};
	s023 = s034 = s032 = "";
	for(int i=alertR->size-1 ; i>-1 ; i--)
	{
		data = (DWORD*)alertR->ptr[i];

		CString strVal;
		if (data[23])		// 현재가
		{

			m_strCurr = (char*)data[23];
// 			s.Format("[%s]curr:%s\n",alertR->code, m_strCurr);
// 			TRACE(s);

			InvalidateRect(m_rectCurrD);
			m_pCtrlWnd->SetCurr(m_strCurr);
			s023 = m_strCurr;
		}
		if (data[24])		// 전일대비
		{
			m_strDiff = (char*)data[24];
			InvalidateRect(m_rectDiffD);
		}
		if (data[33])		// 등락율
		{
			m_strRate = (char*)data[33];
			InvalidateRect(m_rectRateD);
		}
		if (data[27])		// 거래량
		{
			m_strGvol = (char*)data[27];
			InvalidateRect(m_rectGvolD);
		}
		if (data[34])		// 시간
		{
			s034 = (char*)data[34];
		}
		if (data[32])		// 체결량
		{
			s032 = (char*)data[32];
			s.Format("[%s]cheg:%s\n",alertR->code, s032);
			TRACE(s);
		}

		if(s023.IsEmpty() == FALSE && s034.IsEmpty() == FALSE && s032.IsEmpty() == FALSE)
			m_pCtrlWnd->SetRealData(s034, s023, s032);

// 		s.Format("[%s]%s,%s,%s\n", alertR->code, s034, s023, s032);
// 		TRACE(s);
	}
//	OutputDebugString(s);


	
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

	int iPos=0;
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

void CMapWnd::SendJumun(char cJflg, int iPirce, int iJuno, int iJqty, int bMaketPrice)
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
	
	if (m_pPass->IsWindowEnabled())
	{
		SetGuide("비밀번호를 확인 후 이용하시기 바랍니다. (비밀번호입력 -> [조회]클릭)");
		return;
	}

	m_pPass->GetWindowText(m_strPswd);
	m_strPswd.TrimLeft(), m_strPswd.TrimRight();
 	if (m_strPswd.GetLength()<4)
	{
		MessageBox("비밀번호를 확인하십시오.","IBK투자증권");
		return;
	}

	if (iJqty == 0 && (cJflg == '1' || cJflg == '2'))	// 주문수량이 0이고 매도나 매수인 경우
	{	
		iJqty = m_pCtrlWnd->GetJumunCount();
	}

	CString strLedger = m_ledger.LedgerTR(MAP_NO, m_strAcc.Left(3), "", "", GetEncPassword(m_strPswd));
	
	m_iLenLedger = strLedger.GetLength();

	CString strTemp;
		
	struct _jumun_mid	Mid;
	FillMemory(&Mid, sizeof(Mid), ' ');

	CopyMemory(Mid.rcnt, "1", 1);

	Mid.odgb = '1';

	/* 2013.07.24 KSJ 펀드 반영보류
 	if(m_strAcc.Left(5) == "99991")	//2013.07.10 KSJ IBK회사용 상품주식 계좌 일때
 		Mid.odgb = '4';	//pibosodr에 odgb가 4이면 상품주식 주문(SONAT080)을 사용
	*/

	Mid.mkgb = m_pHogaWnd->GetMarket();
	Mid.mmgb = cJflg;
	Mid.mdgb = '2';	// 정정취소시 전부
	CopyMemory(Mid.acno, m_strAcc.operator LPCTSTR(), min(sizeof(Mid.acno), m_strAcc.GetLength()));

	CopyMemory(Mid.pswd, "HEAD", 4);	//2013.12.23 KSJ 일방향암호화

/*
#ifdef	ENCTEST
	CString passX;
	if (m_pTool)
	{
		passX = m_pTool->GetEncPswd(m_strPswd, m_strAcc.Left(8));
		CopyMemory(Mid.pswd, passX, min(sizeof(Mid.pswd), passX.GetLength()));
	}

#else
	//2013.12.23 KSJ 일방향암호화 추가
	//CopyMemory(Mid.pswd, m_strPswd.operator LPCTSTR(), min(sizeof(Mid.pswd), m_strPswd.GetLength()));
	CopyMemory(Mid.pswd, "HEAD", 4);
#endif
*/
	
	CopyMemory(Mid.code, m_strCODX.operator LPCTSTR(), min(sizeof(Mid.code), m_strCODX.GetLength()));
	
	if (cJflg == '1' || cJflg == '2')
		strTemp.Format("%d", iJqty);
	else
		strTemp = "0";		// fixed by schbang 2006.03.17, 정정취소시 전부체크이므로 수량을 0으로 세팅
	CopyMemory(Mid.jqty, strTemp.operator LPCTSTR(), min(sizeof(Mid.jqty), strTemp.GetLength()));

	//2012.11.07 KSJ 시장가일때의 세팅
	if(bMaketPrice == 1)
	{
		CopyMemory(Mid.jprc, "0000000000", 10);
		CopyMemory(Mid.hogb, "03", 2);
		Mid.jmgb[0] = '0';
	}
	else
	{
		strTemp.Format("%d", iPirce);
		CopyMemory(Mid.jprc, strTemp.operator LPCTSTR(), min(sizeof(Mid.jprc), strTemp.GetLength()));
		strTemp = m_pCtrlWnd->GetJumunCondition();
		Mid.jmgb[0] = strTemp.GetAt(0);
		
		CopyMemory(Mid.hogb, "00", sizeof(Mid.hogb));
	}
	//KSJ

//	CopyMemory(Mid.jmgb, strTemp.operator LPCTSTR(), min(sizeof(Mid.jmgb), strTemp.GetLength()));
//	if (cJflg == '3') //정정
//		CopyMemory(Mid.hogb, "99", sizeof(Mid.hogb));
//	else
	strTemp.Format("%d", iJuno);
	CopyMemory(Mid.ogno, strTemp.operator LPCTSTR(), min(sizeof(Mid.ogno), strTemp.GetLength()));

	CString	Path;
	Path.Format("%s\\tab\\ACCNTDEPT.INI", m_pTool->Variant(homeCC,""));
	
	char readB[1024];
	int readL;
	
	readL = GetPrivateProfileString("ACCNTDEPT","DEPT"," ",readB,sizeof(readB),Path);
	
	CString tDept(readB,readL);
	tDept.TrimLeft();tDept.TrimRight();

	//if(m_sDept == "813" || m_sDept == tDept)
	const int igubn = GetCondition();   //0 현금 1 신용(03) 2 대출(80)

	if(m_sDept == tDept)  //법인영업 811
	{
		if(m_pCtrlWnd->GetChaik())
			Mid.prgb = 'Y';
		else
		{
			if(igubn == 1 || igubn == 2)
				Mid.prgb = 'B';	
			else
				Mid.prgb = 'X';
		}
	}
	else
	{
		if(igubn == 1 || igubn == 2)
			Mid.prgb = 'B';	
		else
			Mid.prgb = 'X';
	}


	if(igubn == 1)   //신용 //매도1 매수2 
	{
		if(cJflg  == '1')  //매도
		{
			if(m_pCtrlWnd->GetCrdType() == "03")  //자기융자
			{
				Mid.gmo[0] = '1'; 
				Mid.gmo[1] = '0'; 
				Mid.gmo[2] = '3'; 
			}
			else if(m_pCtrlWnd->GetCrdType() == "01")  //유통융자
			{
				Mid.gmo[0] = '1'; 
				Mid.gmo[1] = '0'; 
				Mid.gmo[2] = '1'; 
			}

			if(m_strLoanDate.IsEmpty())
			{
				SetGuide("매도하실 신용잔고를 선택하여주시기 바랍니다.");
				return;
			}
			strncpy((char*)&Mid.gmo[3], m_strLoanDate, m_strLoanDate.GetLength());
		}
		else if(cJflg  == '2')  //매수
		{
			if(m_pCtrlWnd->GetCrdType() == "03")  //자기융자
			{
				Mid.gmo[0] = '0'; 
				Mid.gmo[1] = '0'; 
				Mid.gmo[2] = '3'; 
			}
			else if(m_pCtrlWnd->GetCrdType() == "01")  //유통융자
			{
				Mid.gmo[0] = '0'; 
				Mid.gmo[1] = '0'; 
				Mid.gmo[2] = '1'; 
			}

			const CTime curr = CTime::GetCurrentTime();
			CString timeS;

			timeS.Format("%04d%02d%02d", curr.GetYear(), curr.GetMonth(), curr.GetDay());
			strncpy((char*)&Mid.gmo[3], timeS, timeS.GetLength());
		}
		
	}
	else if(igubn == 2)  //대출
	{
		if(cJflg  == '1')  //매도
		{
			Mid.gmo[0] = '1'; 
			Mid.gmo[1] = '8'; 
			Mid.gmo[2] = '0'; 
		}
		if(m_strLoanDate.IsEmpty())
		{
			SetGuide("매도하실 대출잔고를 선택하여주시기 바랍니다.");
			return;
		}
		strncpy((char*)&Mid.gmo[3], m_strLoanDate, m_strLoanDate.GetLength());
	}

	

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

//CString slog;
//slog.Format("[1003] prgb=[%c] gmo=[%.20s] code=[%.12s] acc=[%.11s] prc=[%.10s] cnt=[%.8s]", Mid.prgb, Mid.gmo, Mid.code, Mid.acno, Mid.jprc, Mid.jqty);
//OutputDebugString(slog);

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

	m_pPass->GetWindowText(m_strPswd);
	m_strPswd.TrimLeft(), m_strPswd.TrimRight();

	if (m_strPswd.IsEmpty())
	{
		SetGuide("비밀번호를 입력하세요.");
		return;
	}

	//SetWaitMode(TRUE);

	const int igubn = GetCondition();   //0 현금 1 신용(03) 2 대출(80)
	if(igubn == 0)
	{
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
		else if (m_pTool->GetCodeType(m_strJCOD) == etnType)
			strVal.Format("Q%s", m_strJCOD);
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
		SetGuide("");
		m_pTool->SendTR(TN_MAXJMCT, strData, nTrKey);
	}
	else if(igubn == 1)    //신용
	{
		CString strData = m_ledger.LedgerTR(MAP_NO, m_strAcc.Left(3), "", SN_MAXCRJMCT);
		m_iLenLedger = strData.GetLength();

		struct _maxCrjmct_mid mid;
		FillMemory(&mid, L_maxCrjmct_mid, ' ');

		CString strVal(_T(""));
		
		CopyMemory(&mid.in, _T("00001"), sizeof(mid.in));

		if (m_pTool->GetCodeType(m_strJCOD) == elwType)
			strVal.Format("J%s", m_strJCOD);
		else if (m_pTool->GetCodeType(m_strJCOD) == etnType)
			strVal.Format("Q%s", m_strJCOD);
		else
			strVal.Format("A%s", m_strJCOD);

		CopyMemory(&mid.zAcntNo, (LPCTSTR)m_strAcc, m_strAcc.GetLength());
		CopyMemory(&mid.zPwd, (LPCTSTR)m_strPswd, m_strPswd.GetLength());
		mid.zBnsTp[0] = iMmgb == 1 ? _T('1') : _T('2');

		//CopyMemory(&mid.LoadDtlClssCode, (LPCTSTR)"03", 2);

		if (m_pTool->GetCodeType(m_strJCOD) == elwType)
			strVal.Format("J%s", m_strJCOD);
		else if (m_pTool->GetCodeType(m_strJCOD) == etnType)
			strVal.Format("Q%s", m_strJCOD);
		else
			strVal.Format("A%s", m_strJCOD);
		
		CopyMemory(&mid.zIsuNo, (LPCTSTR)strVal, strVal.GetLength());

		strVal.Format("+%09s", strJmdg);
		strVal += _T(".00");
		CopyMemory(&mid.dOrdPrc, (LPCTSTR)strVal, strVal.GetLength());

		if(iMmgb == 1)
			strncpy((char*)&mid.LoadDt, m_strLoanDate, m_strLoanDate.GetLength());
		else
		{
			const CTime curr = CTime::GetCurrentTime();
			CString timeS;
			
			timeS.Format("%04d%02d%02d", curr.GetYear(), curr.GetMonth(), curr.GetDay());
			strncpy((char*)&mid.LoadDt, timeS, timeS.GetLength());
		}

		if(m_pCtrlWnd->GetCrdType() == "03")  //자기융자
			strncpy((char*)&mid.LoadDtlClssCode, "03", 2);
		else if(m_pCtrlWnd->GetCrdType() == "01")  //유통융자
			strncpy((char*)&mid.LoadDtlClssCode, "01", 2);

		strData += CString((char *)&mid, L_maxCrjmct_mid);

		int nTrKey = 0;
		
		switch (iMmgb)
		{
		case 1:	// 매도
			nTrKey = TK_CRMDJMCN;  
			break;
		case 2:	// 매수
			nTrKey = TK_CRMSJMCN;
			break;
		case 3:	// 최대
			nTrKey = TK_CRMAXMSJMCN;
			break;
		default:
			return;
		}

	
		SetGuide("");
		m_pTool->SendTR(TN_MAXJMCT, strData, nTrKey);
	}
	else if(igubn == 2)
	{

	}

	


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

void CMapWnd::SendFee()
{
	if (m_strAcc.IsEmpty())
	{
		SetGuide("계좌번호를 입력하세요.");
		return;
	}

	m_pPass->GetWindowText(m_strPswd);
	m_strPswd.TrimLeft(), m_strPswd.TrimRight();

	if (m_strPswd.IsEmpty())
	{
		SetGuide("비밀번호를 입력하세요.");
		return;
	}

	CString strData = m_ledger.LedgerTR(MAP_NO, m_strAcc.Left(3), "");
	m_iLenLedger = strData.GetLength();
	
	struct _pihocfee_mid mid;
	FillMemory(&mid, L_pihocfee_mid, ' ');
	
	CString strVal(_T(""));
	
	CopyMemory(&mid.in, _T("00001"), sizeof(mid.in));
	CopyMemory(&mid.zAcntNo, (LPCTSTR)m_strAcc, m_strAcc.GetLength());
	CopyMemory(&mid.zPwd, (LPCTSTR)m_strPswd, m_strPswd.GetLength());
	mid.zShTp[0] = _T('2');
	
	strData += CString((char *)&mid, L_pihocfee_mid);
			
	m_pTool->SendTR(TN_FEE, strData, TK_FEE);
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
	
	if( strGuide.Find("그룹계좌") > -1 && strGuide.Find("존재") > -1 )
		return;

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
	Mid.gubn[0] = '>';	//2015.05.28 KSJ 구분값 추가 
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

	strText.Format("1301%c%s\t2241\t2258\t", 0x7f, m_strJCOD);
	m_pTool->SendTR(TN_SYMBOL, strText, TK_VISYM);
}

void CMapWnd::Trigger(CString strTrigger)
{
	while (!strTrigger.IsEmpty())
	{
		CString str = m_pTool->Parser(strTrigger, '\n'), sym = m_pTool->Parser(str, '\t');
		if (!sym.CompareNoCase("CODX") || !sym.CompareNoCase("1301"))
		{
			 ((CIB100300App*)AfxGetApp())->DeleteKey(_T("SELCODE"));
			if (m_strJCOD != str)
			{
				SetCode(str);
			}
		}
		else if (!sym.CompareNoCase("AN01"))  // 2008.07.22 변경분  "AN10=>AN01"
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
		((CIB100300App*)AfxGetApp())->DeleteKey(m_strAcc);
		((CIB100300App*)AfxGetApp())->DeleteKey(_T("PASS"));
		
		return;
	}

	m_bChecking = FALSE;

	switch (m_nWaitTrType)
	{
	case 1:
		{
			if (m_pNotify)
			{
				m_pNotify->SendJango(m_strAcc, m_strPswd, "0", "0");
				
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
			else if (m_pTool->GetCodeType(m_strJCOD) == etnType)
				strVal.Format("Q%s", m_strJCOD);
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
	if(!m_bInit) 
		return;

	if(m_iOldSize[0] == cx && m_iOldSize[1] == cy) 
		return;

	double lcalcrate[2] = {( (float)cx - (float)m_iOldSize[0] ) / (float)m_iOldSize[0],
							( (float)cy - (float)m_iOldSize[1] ) /(float)m_iOldSize[1]};

CString slogResize;
slogResize.Format("[CTRLWND_DIAG] Resize cx=[%d] cy=[%d] oldCx=[%d] oldCy=[%d] rate0=[%f] rate1=[%f]", cx, cy, m_iOldSize[0], m_iOldSize[1], lcalcrate[0], lcalcrate[1]);
OutputDebugString(slogResize);

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

	//잔고 그리드
	CRect rctmp;
	m_pHogaWnd->GetWindowRect( &rctmp);
	rctmp.top = rctmp.bottom + OUTER_FRAME_SPACE;
	rctmp.bottom = rctmp.top + BT_JANGO_HEIGHT;
	rctmp.right = wrc.right;
	m_pJangoGrid->MoveWindow(rctmp);

	m_iHogaRowH = m_pHogaWnd->GetRowheight();
}


void CMapWnd::LoadBuffer()
{
	m_strPswd = GetBuffer(_T("PASS"));
	m_pPass->SetWindowText(m_strPswd);

	CString sval = GetBuffer(_T("VTYPE"));
	/*if(sval.IsEmpty() || sval == "0")*/	//2012.11.15 KSJ 화면을 줄여놓고 다시띄우면 화면이 그려지지 않아서 주석처리함.
	ShowWindow(SW_SHOW);

}

void CMapWnd::SaveBuffer(CString sKey, CString sData)
{
	CIB100300App* pApp = (CIB100300App*)AfxGetApp();
	pApp->SaveBuffer(sKey, sData);
}

CString CMapWnd::GetBuffer(CString sKey)
{
	CIB100300App* pApp = (CIB100300App*)AfxGetApp();
	return pApp->GetBuffer(sKey);
}

void CMapWnd::SendPSWD(CString accn, CString pswd)
{
	if (m_strAcc.IsEmpty())
	{
		SetGuide("계좌번호를 입력하세요.");
		return;
	}

	m_pPass->GetWindowText(m_strPswd);
	m_strPswd.TrimLeft(), m_strPswd.TrimRight();
	if (m_strPswd.IsEmpty())
	{
		SetGuide("비밀번호를 입력하세요.");
		return;
	}

	//2014.01.13 KSJ 비밀번호 체크시 4자리 이하면 넘어감.
	if (m_strPswd.GetLength()<4)
	{
		SetGuide("비밀번호를 확인하십시오.");
		return;
	}

//@@ pswd
//	m_nWaitTrType = 2;
//	m_iMmgb = iMmgb;
//	m_strJmdg = strJmdg;
//	CheckPswd(m_strAcc, m_strPswd);

	//2013.13.23 KSJ 일방향암호화 추가
	CString strData = m_ledger.LedgerTR(MAP_NO, m_strAcc.Left(3), "", SN_SACMT207, GetEncPassword(m_strPswd));
	m_iLenLedger = strData.GetLength();
	struct _chkPwdMid mid;
	FillMemory(&mid, L_chkPwdMid, ' ');
	CString acnt = m_strAcc;
	
	memset(&mid, ' ', sizeof(mid));
	strncpy(mid.in, "00001", 5);
// 	strncpy(mid.acntClssTp, "2", 1);	2013.07.24 KSJ 펀드 반영 보류
	strncpy(mid.acctNo, acnt, acnt.GetLength());
	//strncpy(mid.password, m_strPswd, m_strPswd.GetLength());		
	strncpy(mid.password, "HEAD", 4);	//2013.13.23 KSJ 일방향암호화 추가
	//strncpy(mid.zTrxTp, tp, tp.GetLength());
	
	strData += CString((char *)&mid, L_chkPwdMid);
	
	int nTrKey = 0;
	
	m_bCheckPass = false;
	nTrKey = TK_CHECK207;
	DeleteAllJangoRow();   //계좌비번 체크시
	m_pTool->SendTR(TN_MAXJMCT, strData, nTrKey);
}

const char* head[] = { "종목명", "구분", "가능수량", "평균단가", "평가금액", "평가손익", "수익률", "신용구분", "대출일" , "종목코드", "구분R", "신용구분R" };

void CMapWnd::SetJangoGrid()
{
	m_clrSelectRow =  m_pTool->GetIndexColor(78);
	CGridCellBase *pc;
	m_pJangoGrid->SetEditable(FALSE);
	m_pJangoGrid->EnableSelection(FALSE);
	m_pJangoGrid->SetFrameFocusCell(FALSE);
	m_pJangoGrid->SetColumnResize(TRUE);
	m_pJangoGrid->SetRowResize(FALSE);
	m_pJangoGrid->EnableTitleTips(FALSE);
	m_pJangoGrid->SetDefCellHeight(20);
	m_pJangoGrid->SetFixedRowCount(1);

	
	m_pJangoGrid->SetRowCount(DEFAULT_JANGO_CNT);
	m_pJangoGrid->SetColumnCount(CNT_JANGO_COL);
//	m_pJangoGrid->SetFixedRowCount(CNT_JANGO_COL);

	pc = m_pJangoGrid->GetDefaultCell(FALSE, FALSE);
	if (pc)	
	{
		pc->SetFormat( DT_VCENTER|DT_SINGLELINE|DT_RIGHT );
		pc->SetMargin(5);
	}
	
	pc = m_pJangoGrid->GetDefaultCell(TRUE, FALSE);
	if (pc) {
		
		pc->SetBackClr( m_pTool->GetIndexColor(COLOR_GRIDHEAD) );
		pc->SetTextClr( m_pTool->GetIndexColor(COLOR_GRIDHEADTXT) );
	}

	m_pJangoGrid->SetBkColor(RGB(255, 255, 255));
	m_pJangoGrid->SetColumnCount(CNT_JANGO_COL);

	
	const int  width[] = {    98,      36,       59,         78,        78,         82,         46,       79,         68,     0,           0,       0  };
	
	for(int n=0; n<CNT_JANGO_COL; ++n)
	{
		m_pJangoGrid->SetItemText(0, n, head[n]);
		m_pJangoGrid->SetItemFormat(0, n, DT_CENTER|DT_VCENTER|DT_SINGLELINE);
		m_pJangoGrid->SetColumnWidth(n, width[n]);
		m_pJangoGrid->SetItemBkColour(0, n, m_pTool->GetIndexColor(COLOR_GRIDHEAD) );
		m_pJangoGrid->SetItemFgColour(0, n, m_pTool->GetIndexColor(COLOR_GRIDHEADTXT) );
	}

	//잔고 그리드
	CRect rctmp, wrc;
	m_pHogaWnd->GetWindowRect( &rctmp);
	m_pCtrlWnd->GetWindowRect( &wrc);
	ScreenToClient(rctmp);
	ScreenToClient(wrc);
	rctmp.top = rctmp.bottom + OUTER_FRAME_SPACE * 2;
	rctmp.bottom = rctmp.top + BT_JANGO_HEIGHT - OUTER_FRAME_SPACE * 2 ;
	rctmp.right = wrc.right;
	m_pJangoGrid->MoveWindow(rctmp);

	if(m_bShowJango)
		m_pJangoGrid->ShowWindow(SW_SHOW);
	else
		m_pJangoGrid->ShowWindow(SW_HIDE);

	CString data = "", path = "", sShow = "0";
	path.Format("%s\\%s\\%s\\%s.ini", m_pTool->GetHome(), USRDIR, m_pTool->GetName(), MAP_NAME);
	char	wb[32];
	GetPrivateProfileString("JANGO", "SHOW", "0", wb, sizeof(wb), path);
	sShow.Format("%s", wb); sShow.TrimRight();
	if(sShow == "1")
		PostMessage(UM_MYMESSAGE, MAKEWPARAM(UP_SHOW_JANGO, NULL), NULL);

}

void CMapWnd::SetJangoRow(CString sflag, int iRow, CString strJCod, CString strCodeName, CString strJggb2, CString strPossCnt, CString strPgdg, CString strPprice, 
						CString strPgsi,  CString strPpercent, CString strJggb, CString strSdate)
{
	strCodeName.TrimRight();
	m_pJangoGrid->SetItemText(iRow, 0, strCodeName);	//종목명
	m_pJangoGrid->SetItemFormat(iRow, 0, DT_LEFT|DT_VCENTER|DT_SINGLELINE);
	
	strJCod.TrimRight();
	m_pJangoGrid->SetItemText(iRow, 9, strJCod);
	
	m_pJangoGrid->SetItemText(iRow, 10, strJggb2);
	if(strJggb2 == "00")
		strJggb2 = "보통";
	else if(strJggb2 == "01")
		strJggb2 = "신용";
	else if(strJggb2 == "02")
		strJggb2 = "대용";
	else if(strJggb2 == "03")
		strJggb2 = "대용";
	else if(strJggb2 == "04")
		strJggb2 = "대용";
	else if(strJggb2 == "05")
		strJggb2 = "기타";
	else 
		strJggb2 = "보통";
	
	m_pJangoGrid->SetItemFormat(iRow, 1, DT_CENTER|DT_VCENTER|DT_SINGLELINE);
	strJggb2.TrimRight();
	m_pJangoGrid->SetItemText(iRow, 1, strJggb2);		//구분
	
	strPossCnt = m_pTool->AddComma(strPossCnt);
	strPossCnt.TrimRight();
	m_pJangoGrid->SetItemText(iRow, 2, strPossCnt);		//가능수량
	
	strPgdg = m_pTool->AddComma(strPgdg);	
	strPgdg.TrimRight();
	m_pJangoGrid->SetItemText(iRow, 3, strPgdg);		//평균단가
	
	strPprice = m_pTool->AddComma(strPprice);	
	strPprice.TrimRight();
	m_pJangoGrid->SetItemText(iRow, 4, strPprice);		//평가금액
	
	if(atof(strPgsi) > 0 )
		m_pJangoGrid->SetItemFgColour(iRow, 5, RGB(255,0,0));
	else if(atof(strPgsi) == 0 )
		m_pJangoGrid->SetItemFgColour(iRow, 5, RGB(0,0,0));
	else if(atof(strPgsi) < 0 )
		m_pJangoGrid->SetItemFgColour(iRow, 5, RGB(0,0,255));
	
	strPgsi = m_pTool->AddComma(strPgsi);	
	strPgsi.Replace("+","");
	strPgsi.TrimRight();
	m_pJangoGrid->SetItemText(iRow, 5, strPgsi);		//평가손익
	
	strPpercent.Replace("--", "-");
	if(atof(strPpercent) > 0 )
		m_pJangoGrid->SetItemFgColour(iRow, 6, RGB(255,0,0));
	else if(atof(strPpercent) == 0 )
		m_pJangoGrid->SetItemFgColour(iRow, 6, RGB(0,0,0));
	else if(atof(strPpercent) < 0 )
		m_pJangoGrid->SetItemFgColour(iRow, 6, RGB(0,0,255));
	
	strPpercent.Replace("+", "");
	strPpercent.Replace("++", "");
	strPpercent.Replace("--", "-");
	strPpercent.TrimRight();
	m_pJangoGrid->SetItemText(iRow, 6, strPpercent);	//수익률

	m_pJangoGrid->SetItemText(iRow, 11, strJggb);
	if(strJggb == "01")
		strJggb = "유통융자";
	else if(strJggb == "03")
		strJggb = "자기융자";
	else if(strJggb == "05")
		strJggb = "유통대주";
	else if(strJggb == "07")
		strJggb = "자기대주";
	else if(strJggb == "40")
		strJggb = "예탁담보자동대출";
	else if(strJggb == "51")
		strJggb = "증권금융예탁담보신용대출";
	else if(strJggb == "52")
		strJggb = "증권금융예탁담보자동대출";
	else if(strJggb == "55")
		strJggb = "증권금융미수담보대출";
	else if(strJggb == "60")
		strJggb = "수익증권담보대출";
	else if(strJggb == "70")
		strJggb = "매도대금담보대출";
	else if(strJggb == "80")
		strJggb = "예탁주식담보신용대출";
	else if(strJggb == "82")
		strJggb = "예탁채권담보신용대출";
	else if(strJggb == "90")
		strJggb = "매입자금담보대출";
	else if(strJggb == "95")
		strJggb = "자기공모주청약대출";
	else if(strJggb == "97")
		strJggb = "유통공모주청약대출";
	else if(strJggb == "91")
		strJggb = "자기유상청약대출";
	else if(strJggb == "00")
		strJggb = "";
	else
		strJggb = "";	
	
	m_pJangoGrid->SetItemText(iRow, 7, strJggb);		//신용구분
	
	strSdate.TrimRight();
	m_pJangoGrid->SetItemText(iRow, 8, strSdate);		//대출일
	m_pJangoGrid->Invalidate();
	
	int cnt = GetGridRowCnt();
	cnt = m_pJangoGrid->GetRowCount();
}

void CMapWnd::SetJangoFromDll(CString sflag, CString strJCod, CString strCodeName, CString strJggb2, CString strPossCnt, CString strPgdg, CString strPprice, 
						CString strPgsi,  CString strPpercent, CString strJggb, CString strSdate)
{
	
CString slog;
slog.Format("[IB100300]notify flagS = 구분=[%s] 종목코드=[%s] 잔고구분=[%s] 가능수량=[%s] 매입평균가=[%s] 평가금액=[%s] 평가손익=[%s] 수익률=[%s] 신용코드=[%s] 대출일=[%s] rowcnt=[%d]\n", 
										sflag , strJCod,    strJggb2,       strPossCnt,     strPgdg,        strPprice,    strPgsi,      strPpercent, strJggb,     strSdate, m_pJangoGrid->GetRowCount());
OutputDebugString(slog);
	if(m_pJangoGrid == NULL)
		return;

	if(!m_bCheckPass)
		return;

	CString stmp, tmpS;
	int iRow = 0;
	if(sflag == "A")    //잔고 조회 그리드 갱신
	{
		iRow = m_pJangoGrid->InsertRow(NULL, -1);
		SetJangoRow(sflag, iRow, strJCod, strCodeName, strJggb2, strPossCnt, strPgdg, strPprice, strPgsi, strPpercent, strJggb, strSdate);
	}
	else if(sflag == "U")  //실시간 업데이트
	{
		strJCod.TrimRight();
		strJggb2.TrimRight();
		strJggb.TrimRight();
		strSdate.TrimRight();

		tmpS.Format("%s%s%s%s", strJCod, strJggb2, strJggb, strSdate);
		for(int ii = 1; ii <= m_pJangoGrid->GetRowCount() ; ii++ )
		{
			//종목코드 , 잔고구분, 신용구분, 대출일
			stmp.Format("%s%s%s%s", m_pJangoGrid->GetItemText(ii, 9), m_pJangoGrid->GetItemText(ii, 10), 
									m_pJangoGrid->GetItemText(ii, 11), m_pJangoGrid->GetItemText(ii, 8));

			if(tmpS == stmp)
				SetJangoRow(sflag, ii, strJCod, strCodeName, strJggb2, strPossCnt, strPgdg, strPprice, strPgsi, strPpercent, strJggb, strSdate);
		}
	}
	else if(sflag == "I")  //신규종목 추가
	{
		iRow = m_pJangoGrid->InsertRow(NULL, -1);
		SetJangoRow(sflag, iRow, strJCod, strCodeName, strJggb2, strPossCnt, strPgdg, strPprice, strPgsi, strPpercent, strJggb, strSdate);
		
	}
	else if(sflag == "D")  //종목삭제(매도후)
	{
		strJCod.TrimRight();
		strJggb2.TrimRight();
		strJggb.TrimRight();
		strSdate.TrimRight();

		CString sJggb2, sJggb, sSdate;
	
		tmpS.Format("%s%s%s%s", strJCod, strJggb2, strJggb, strSdate);  //종목코드 잔고구분 신용구분 신용날자 
		for(int ii = 1; ii <= m_pJangoGrid->GetRowCount() ; ii++ )
		{
			//종목코드 , 잔고구분, 신용구분, 대출일
			stmp.Format("%s%s%s%s", m_pJangoGrid->GetItemText(ii, 9), m_pJangoGrid->GetItemText(ii, 10), 
				m_pJangoGrid->GetItemText(ii, 11), m_pJangoGrid->GetItemText(ii, 8));

slog.Format("[IB100300]notify 잔고커트롤값=[%s] 잔고그리드값=[%s] \n", tmpS, stmp);
OutputDebugString(slog);

			if(tmpS == stmp)
				m_pJangoGrid->DeleteRow(ii);
		}
	}	
		
	m_pJangoGrid->Invalidate();
}

void CMapWnd::DeleteAllJangoRow()
{
	if(m_pJangoGrid == NULL)
		return;

	for(int ii =  m_pJangoGrid->GetRowCount() ; ii > 0 ; ii--)
		m_pJangoGrid->DeleteRow(ii);

	m_iSelectRow = -1;
	m_pJangoGrid->Invalidate();
}	

int CMapWnd::GetGridRowCnt()
{
	if(m_pJangoGrid == NULL)
		return 0;

	int cnt = 0;
	
	CString stmp;
	for(int ii = 0 ; ii < m_pJangoGrid->GetRowCount(); ii++)
	{
		stmp = m_pJangoGrid->GetItemText(ii, 0);
		if(stmp.IsEmpty())
			return cnt;

		cnt = ii;
	}

	return cnt;
}

void CMapWnd::ClearAllSelectRow(int irow)
{
	if(m_pJangoGrid == NULL)
		return;

	if(irow > 0)
	{
		for(int jj = 0 ; jj < CNT_JANGO_COL; jj++)
			m_pJangoGrid->SetItemBkColour(irow, jj, RGB(255,255,255));
	}
	else
	{
		for(int ii = 1; ii <= m_pJangoGrid->GetRowCount() ; ii++ )
		{
			for(int jj = 0 ; jj < CNT_JANGO_COL; jj++)
				m_pJangoGrid->SetItemBkColour(ii, jj, RGB(255,255,255));
		}
	}

	m_pJangoGrid->Invalidate();
	m_iSelectRow = -1;
}

void CMapWnd::SelectJangoRow(int irow)
{
	if(m_pJangoGrid == NULL)
		return;

	for(int jj = 0 ; jj < CNT_JANGO_COL; jj++)
		m_pJangoGrid->SetItemBkColour(irow, jj, m_clrSelectRow);
}

int CMapWnd::GetCondition()
{
	return m_pCtrlWnd->GetCondition();
}

CString CMapWnd::GetCreditType()
{
	return m_pCtrlWnd->GetCrdType();
}