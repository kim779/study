// MapWnd.cpp : implementation file
//

#include "stdafx.h"
#include "IB300200.h"
#include "MapWnd.h"
#include "HogaWnd.h"
#include "AccountCtrl.h"
#include "ControlWnd.h"
#include "OptionEx.h"
#include "cx_Notify.h"
#include "../../control/fx_grid/memdc.h"
#include "../../control/fx_misc/fxEdit.h"
#include "../../control/fx_misc/fxImgButton.h"
#include "../../control/fx_misc/fxCodeCtrl.h"
#include "../../control/fx_misc/fxStatic.h"
#include "../../control/fx_misc/misctype.h"
#include "../../h/ledger.h"
#include "../../h/axisfire.h"
#include "../../h/axisvar.h"
#include <math.h>
#include "DialogConfirm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
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
#define ID_CTRL_STATIC		0x0e
#define ID_HOGA_WND		0x0e
#define ID_CTRL_WND		0x0f
#define ID_MAP_WND		0x10

#define	ENCTEST

#define TM_GUIDE		0x01
#define TT_CHECKPSWD	0x02
#define TM_VSIZE		0x03
#define TM_FIRSTQ		0x04
#define	TT_CHKPWD		500
#define TT_SENDFCHG		501

#define UM_PASSWORD	WM_USER+1111

struct SACMT207_mid {
	char in[5];
	char acctNo[20];
	char password[8];
	char zTrxTp[1];
};
#define L_chkPwdMid	sizeof(struct _chkPwdMid)

struct SACMT207_mod {
	struct SACMT207_mid in;
	char out[5];
	char lPwdContErrCnt[9];
	char lAtmPwdContErrCnt[9];
	char lMtrsfPwdContErrCnt[9];
	char zAcntNm[40];
};

struct SACMT238_mid {
	char In[5];
	char zAcntNo[20];
	char zPwd[8];
};

struct SACMT238_mod {
	struct SACMT238_mid mid;
	char Out[5];
	char zAvalYn[1];
};

UINT ParsingRTM(LPVOID lpvoid);

/////////////////////////////////////////////////////////////////////////////
// CMapWnd

CMapWnd::CMapWnd(CWnd *parent)
{
	/* 비밀번호 검증 여부를 저장하는 flag */
	bPasswordChecked = false;
	bChecking = false;

	m_pParent = parent;
	m_bWait	 = FALSE;
	m_pPass = NULL;

	m_output1 = _T("");
	m_output2 = _T("");
	m_output3 = _T("");
	m_output4 = _T("");
	m_output5 = _T("");
	m_output6 = _T("");

	m_pCtrlWnd = NULL;
	m_pHogaWnd = NULL;
	m_pPass = NULL;
	m_pCodeCtrl = NULL;
	m_pBtnSend = NULL;
	m_pBtnVSize = NULL;
	m_pBtnCode = NULL;
	m_pStatic = NULL;
	m_pMapWnd = NULL;
	m_pNotify = NULL;

	m_nWaitTrType = 0;

	m_depth = hgDEPTH10;
	m_size = szNORMAL;
	m_bWarning = true;

	m_ledger.m_pParent = m_pParent;

	m_bThread = false;
	
	m_lSizeRate[0] = 0.16;
	m_lSizeRate[1] = 0.07;
	m_bInit = FALSE;
	m_iOldSize[0] = m_iOldSize[1] = 0;

	m_iWndWidth = MAP_WIDTH;
	m_iContentHeight = m_iWndHeight = MAP_HEIGHT;
	m_iContentHeight -= 20;

	m_iCurFont = 9;
	m_bhogaResizeClick = FALSE;
	m_bFirst = false;

	m_bSendFchgTimer = FALSE;
	m_bCanSend = TRUE;

	m_nFchgCount = 0;
	//initRTM();

	m_bMicheContinue = false;
	m_hiSha256 = NULL;
}

CMapWnd::~CMapWnd()
{
	if (m_pAccount)
	{
		m_pAccount->DestroyWindow();
		m_pAccount = nullptr;
	}
	
	//2013.01.29 KSJ
	if (m_pOptionEx)
	{
		m_pOptionEx->DestroyWindow();
		m_pOptionEx = nullptr;
	}
	//2013.01.29 KSJ End

	if (m_pPass)
		m_pPass = nullptr;
	if (m_pCodeCtrl)
		m_pCodeCtrl = nullptr;
	if (m_pBtnSend)
		m_pBtnSend = nullptr;
	if (m_pBtnConfig)
		m_pBtnConfig = nullptr;
	if (m_pBtnVSize)
		m_pBtnVSize = nullptr;
	if (m_pBtnCode)
		m_pBtnCode = nullptr;
	if (m_pBtnReserve)
		m_pBtnReserve = nullptr;
	if (m_pStatic)
		m_pStatic = nullptr;

	if (m_pHogaWnd)
		m_pHogaWnd = nullptr;
	if (m_pCtrlWnd)
		m_pCtrlWnd = nullptr;
	
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
	ON_WM_SETFOCUS()
	ON_BN_CLICKED(ID_CTRL_CODEBTN, OnButtonCode)
	ON_BN_CLICKED(ID_CTRL_SEND, OnButtonSend)
	ON_BN_CLICKED(ID_CTRL_VSIZE, OnButtonVSize)
	ON_BN_CLICKED(ID_CTRL_HOGA1, OnButtonHoga1)
	ON_BN_CLICKED(ID_CTRL_HOGA3, OnButtonHoga3)
	ON_BN_CLICKED(ID_CTRL_CONFIG, OnButtonConfig)
	ON_BN_CLICKED(ID_CTRL_RESERVE, OnButtonReserve)
	ON_WM_LBUTTONDOWN()
	ON_WM_KILLFOCUS()
	ON_WM_TIMER()
	ON_WM_RBUTTONUP()
	ON_WM_SETCURSOR()
	ON_BN_CLICKED(ID_CTRL_CODE, OnHistoryCode)
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_USER, OnMessage)
	ON_MESSAGE(WM_USER + 1, OnUser)
	ON_MESSAGE(UM_PASSWORD, OnMyPassword)
	ON_CBN_SELENDOK(ID_CTRL_CODE, OnCodeSelEndOk)
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CMapWnd message handlers
void CMapWnd::OnDestroy() 
{
	m_bInit = FALSE;
	

	stopRTM();

	if (m_pMapWnd)
	{
		m_pParent->SendMessage(WM_USER, MAKEWPARAM(detachMAP, m_mapKey), 0);
		m_pMapWnd->DestroyWindow();	
		m_pMapWnd = nullptr;
	}

	if (m_pNotify)
	{
		m_pNotify->DestroyWindow();
		m_pNotify = nullptr;
	}

	CWnd::OnDestroy();
}

void CMapWnd::LoadRoundBitmap()
{
	CString sFile;
	sFile.Format("%s\\tab\\PALETTE.INI", m_home);

	CString sPalette = _T("");
	int readL = 0; 
	char readB[1024]; 
	readL = GetPrivateProfileString(_T("General"), _T("Palette"), _T("Blue"), readB, sizeof(readB), sFile);
	if (readL > 0)	sPalette.Format("%s", readB);

	CString sImage;
	sImage.Format("%s\\Image\\axpanel%s1_",  m_home, sPalette);
	for(int i=0; i<4; i++)
	{
		if(m_hRoundBitmap[i])
			DeleteObject(m_hRoundBitmap[i]);
	}

	CIB300200App* pApp = (CIB300200App*)AfxGetApp(); 
	m_hRoundBitmap[0] = (HBITMAP)::LoadImage( pApp->m_hInstance , sImage+"lt.bmp",
											IMAGE_BITMAP,0,0,LR_LOADFROMFILE );
	m_hRoundBitmap[1] = (HBITMAP)::LoadImage( pApp->m_hInstance, sImage+"rt.bmp",
											IMAGE_BITMAP,0,0,LR_LOADFROMFILE );
	m_hRoundBitmap[2] = (HBITMAP)::LoadImage( pApp->m_hInstance, sImage+"lb.bmp",
											IMAGE_BITMAP,0,0,LR_LOADFROMFILE );
	m_hRoundBitmap[3] = (HBITMAP)::LoadImage(pApp->m_hInstance, sImage+"rb.bmp",
											IMAGE_BITMAP,0,0,LR_LOADFROMFILE );

}

void CMapWnd::Init()
{
	CRect rc, cRc;
	GetWindowRect(&rc);

	Variant(titleCC, "[3002] 선물옵션 클릭주문");
	m_home = Variant(homeCC, "");
	m_user = Variant(nameCC, "");
	m_pFont = GetAxFont("굴림체", 9, false, 0);
	m_pBFont = GetAxFont("굴림체", 9, false, FW_BOLD);

	CString strImgDir = "";
	strImgDir.Format("%s\\Image\\", m_home);

	LoadRoundBitmap();
	/*
	m_hRoundBitmap[0] = GetAxHBitmap(strImgDir+_T("axpanel1_lt.bmp"));
	m_hRoundBitmap[1] = GetAxHBitmap(strImgDir+_T("axpanel1_rt.bmp"));
	m_hRoundBitmap[2] = GetAxHBitmap(strImgDir+_T("axpanel1_lb.bmp"));
	m_hRoundBitmap[3] = GetAxHBitmap(strImgDir+_T("axpanel1_rb.bmp"));
	*/
	m_hGuideBitmap = GetAxHBitmap(strImgDir+_T("알림_ic.bmp"));

	m_CmdBgColor = GetIndexColor(CMD_BACK_INDEX);
	m_ContentsBgColor = GetIndexColor(CONTENTS_BACK_INDEX);
	m_GuideBgColor = GetIndexColor(GUIDE_BACK_INDEX);
	m_ContentsBorderColor = GetIndexColor(CONTENTS_BORDER_INDEX);
	m_GuideTxtColor = GetIndexColor(GUIDE_TXT_INDEX);
	m_GridHdrTxtColor = GetIndexColor(COLOR_GRIDHEADTXT);
	m_txtcolor = GetIndexColor(COLOR_TEXT);


// line 1
	const int cx = INNER_FRAME_SPACE + FRAME_SPACE;
	int cy = INNER_FRAME_SPACE;

	const int ibtn2W = 36;
#if 0		// onpaint 부분 때문에 삭제
	cRc.SetRect(0, 0, 500, 22);
	m_pStatic = std::make_unique< CfxStatic();
	m_pStatic->Create("", WS_CHILD|WS_VISIBLE|SS_LEFT|WS_CLIPCHILDREN|WS_CLIPSIBLINGS, cRc, this);
	m_pStatic->Init(ssGRADIENT, 0, "", "", 0x00FFFDFD, 0x00F1E6DE, GetSysColor(COLOR_BTNTEXT), alLEFT);
#endif

	int ileft = INNER_FRAME_SPACE+FRAME_SPACE;//INNER_FRAME_SPACE+OUTER_FRAME_SPACE;
	int itop = INNER_FRAME_SPACE + INNER_FRAME_SPACE;
	cRc.SetRect(ileft, itop, ileft + ACC_WIDTH, itop + CTRL_HEIGHT);
	m_pAccount = std::make_unique<CAccountCtrl>(m_pParent, this, m_home);
	m_pAccount->CreateEx(WS_EX_TOPMOST, NULL, "ACCN", WS_VISIBLE|WS_CHILD|WS_TABSTOP, cRc, this, 0);
	m_pAccount->createAccountCtrl("AN20", KEY_ACCOUNT, GetIndexColor(7));

	m_oRc1.SetRect(cRc.right + HORI_GAP, cRc.top, cRc.right + GAP + 78, cRc.bottom);

	cRc.SetRect(m_oRc1.right + INNER_FRAME_SPACE, cRc.top, m_oRc1.right + GAP + 60, cRc.bottom); 
	m_pPass = std::make_unique<CfxEdit>(m_pFont);
	m_pPass->Create(WS_CHILD | WS_VISIBLE | WS_TABSTOP | ES_PASSWORD, cRc, this, ID_CTRL_PASS);
	((CEdit*)m_pPass.get())->LimitText(8);
	m_pPass->SetWindowText(m_pswd);

	cRc.SetRect(cRc.right + INNER_FRAME_SPACE, cRc.top, cRc.right + INNER_FRAME_SPACE + ibtn2W, cRc.bottom);
	m_pBtnSend = std::make_unique<CfxImgButton>();
	m_pBtnSend->Create("\n조회", cRc, this, ID_CTRL_SEND, FALSE, TRUE);
	m_pBtnSend->SetWindowText("조회");
	m_pBtnSend->SetFont(m_pFont);
	m_pBtnSend->SetDrawFocus();

	cRc.SetRect(cRc.right + BUTTON_SPACE, cRc.top, cRc.right + BUTTON_SPACE + ibtn2W, cRc.bottom);
	m_pBtnReserve = std::make_unique<CfxImgButton>();
	m_pBtnReserve->Create("\nSTOP", cRc, this, ID_CTRL_RESERVE, FALSE, TRUE);
	m_pBtnReserve->SetWindowText("Stop");
	m_pBtnReserve->SetFont(m_pFont);

	cRc.SetRect(cRc.right + BUTTON_SPACE, cRc.top, cRc.right + BUTTON_SPACE + ibtn2W, cRc.bottom);
	m_pBtnConfig = std::make_unique<CfxImgButton>();
	m_pBtnConfig->Create("\n설정", cRc, this, ID_CTRL_CONFIG, FALSE, TRUE);
	m_pBtnConfig->SetWindowText("설정");
	m_pBtnConfig->SetFont(m_pFont);

	cRc.SetRect(cRc.right + BUTTON_SPACE, cRc.top, cRc.right + BUTTON_SPACE + 20, cRc.bottom);
	m_pBtnVSize = std::make_unique<CfxImgButton>();
	m_pBtnVSize->Create("\n5/10호가전환", cRc, this, ID_CTRL_VSIZE, FALSE, TRUE);
	m_pBtnVSize->SetFont(m_pFont);
	const int iright = cRc.right + OUTER_FRAME_SPACE + INNER_FRAME_SPACE;

// line 2
	cy = cRc.bottom + GAP;
	cRc.SetRect(ileft, cy, ileft + 88, cy + CTRL_HEIGHT + 1);
	m_pCodeCtrl = std::make_unique<CfxCodeCtrl>(this, m_home + "\\tab");
	m_pCodeCtrl->Create(this, cRc, ID_CTRL_CODE);
	m_pCodeCtrl->SetUnit(GU_FOCODE);
	m_pCodeCtrl->SetFont(m_pFont);

	cRc.SetRect(cRc.right + BUTTON_SPACE, cRc.top, cRc.right + BUTTON_SPACE + 20, cRc.top + CTRL_HEIGHT);
	m_pBtnCode = std::make_unique<CfxImgButton>();
	m_pBtnCode->Create("\n종목코드", cRc, this, ID_CTRL_CODEBTN, FALSE, TRUE);
	m_pBtnCode->SetFont(m_pFont);

	//2013.01.29 KSJ
	cRc.SetRect(cRc.right + INNER_FRAME_SPACE, cRc.top, cRc.right + INNER_FRAME_SPACE + 38, cRc.bottom);
	m_pOptionEx = std::make_unique<COptionEx>(m_pParent, this, m_home);
	m_pOptionEx->CreateEx(WS_EX_TOPMOST, NULL, "OptionEx", WS_VISIBLE|WS_CHILD|WS_TABSTOP, cRc, this, 1);
	m_pOptionEx->createOptionExCtrl("CX_OptionEx", KEY_OPTIONEX, 192);
	//2013.07.29 KSJ End

	m_oRc6.SetRect(cRc.right + INNER_FRAME_SPACE, cRc.top, cRc.right + INNER_FRAME_SPACE + 150, cRc.bottom);

// line 3
	cy = cRc.bottom + HORI_GAP;
	m_tRc2.SetRect(ileft, cy, ileft + LABEL_WIDTH, cy + CTRL_HEIGHT);
	m_oRc2.SetRect(m_tRc2.right-1, m_tRc2.top, m_tRc2.right+TEXTOUT_WIDTH, m_tRc2.bottom );

	m_tRc3.SetRect(m_oRc2.right-1, m_oRc2.top, m_oRc2.right + LABEL_WIDTH, m_tRc2.bottom);
	m_oRc3.SetRect(m_tRc3.right-1, m_tRc3.top, m_tRc3.right + TEXTOUT_WIDTH, m_tRc3.bottom );

	m_tRc4.SetRect(m_oRc3.right-1, m_oRc3.top, m_oRc3.right + LABEL_WIDTH, m_oRc3.bottom);
	m_oRc4.SetRect(m_tRc4.right-1, m_tRc4.top, m_tRc4.right + TEXTOUT_WIDTH, m_tRc4.bottom );

	m_tRc5.SetRect(m_oRc4.right-1, m_oRc4.top, m_oRc4.right + LABEL_WIDTH, m_oRc4.bottom);
	m_oRc5.SetRect(m_tRc5.right-1, m_tRc5.top, m_tRc5.right + TEXTOUT_WIDTH, m_tRc5.bottom );

// line 4
	m_tRc7.SetRect(m_tRc2.left, m_tRc2.top + CTRL_HEIGHT-1, m_tRc2.right, m_tRc2.bottom + CTRL_HEIGHT - 1);
	m_oRc7.SetRect(m_tRc7.right-1, m_tRc7.top, m_tRc7.right + TEXTOUT_WIDTH, m_tRc7.bottom );
	
	m_tRc8.SetRect(m_oRc7.right-1, m_oRc7.top, m_oRc7.right + LABEL_WIDTH, m_tRc7.bottom);
	m_oRc8.SetRect(m_tRc8.right-1, m_tRc8.top, m_tRc8.right + TEXTOUT_WIDTH, m_tRc8.bottom );
	
	m_tRc9.SetRect(m_oRc8.right-1, m_oRc8.top, m_oRc8.right + LABEL_WIDTH, m_oRc8.bottom);
	m_oRc9.SetRect(m_tRc9.right-1, m_tRc9.top, m_tRc9.right + TEXTOUT_WIDTH, m_tRc9.bottom );
	
	m_tRc10.SetRect(m_oRc9.right-1, m_oRc9.top, m_oRc9.right + LABEL_WIDTH, m_oRc9.bottom);
	m_oRc10.SetRect(m_tRc10.right-1, m_tRc10.top, m_tRc10.right + TEXTOUT_WIDTH, m_tRc10.bottom );

// hoga wnd
	itop = m_oRc10.bottom + INNER_FRAME_SPACE + OUTER_FRAME_SPACE+INNER_FRAME_SPACE;
	cRc.SetRect(INNER_FRAME_SPACE+FRAME_SPACE, itop, HOGA_WIDTH+ileft, itop + HOGA_HEIGHT);
	m_pHogaWnd = std::make_unique<CHogaWnd>(this, m_pParent);
	m_pHogaWnd->Create(NULL, "", WS_CHILD|WS_VISIBLE, cRc, this, ID_HOGA_WND);


// control wnd
	ileft = iright + INNER_FRAME_SPACE +OUTER_FRAME_SPACE;//cRc.right+cx+HORI_GAP;
	cRc.SetRect(ileft, INNER_FRAME_SPACE+1, CTRL_WIDTH+ileft, cRc.bottom);
	m_pCtrlWnd = std::make_unique<CControlWnd>(this, m_pParent);
	m_pCtrlWnd->Create(NULL, "", WS_CHILD | WS_VISIBLE, cRc, this, ID_CTRL_WND);
	m_pCtrlWnd->Init();

//guide
	itop	 = cRc.bottom+INNER_FRAME_SPACE+INNER_FRAME_SPACE;//rc.bottom - GUIDE_HEIGHT - INNER_FRAME_SPACE;
	m_rcGuide.SetRect(FRAME_SPACE, itop, cRc.right+INNER_FRAME_SPACE, itop+GUIDE_HEIGHT);
	m_pHogaWnd->Init(m_pCtrlWnd.get());

	CString slog;
	slog.Format("%d, %d \r\n%d, %d, %d, %d", rc.bottom, rc.right, m_rcGuide.left, m_rcGuide.top, m_rcGuide.right, m_rcGuide.bottom);
//	AfxMessageBox(slog);
// notify d
	m_pNotify = std::make_unique<CNotify>(m_pParent, KEY_NOTIFYCTRL);
	m_pNotify->CreateNotify(this, CRect(0,0,0,0), m_home);
	m_pNotify->SetCtrlWnd(m_pCtrlWnd.get());


// etc
	setButtonBitmaps();

	

	if (m_acc.IsEmpty())
		m_pAccount->SetFocus();
	else
		m_pPass->SetFocus();

	SetHold(false);
	m_bInit = TRUE;

	CString sval = ((CIB300200App*)AfxGetApp())->GetBuffer(_T("VTYPE"));
	if(sval.IsEmpty() || sval == "0") ShowWindow(SW_SHOW);
} 

void CMapWnd::setButtonBitmaps()
{
	CString path = "";
	path.Format("%s\\%s\\", m_home, IMAGEDIR);

	m_hBitmap2    = GetAxBitmap(path + "2btn.bmp")->operator HBITMAP();
	m_hBitmap2_dn = GetAxBitmap(path + "2btn_dn.bmp")->operator HBITMAP();
	m_hBitmap2_hv = GetAxBitmap(path + "2btn_en.bmp")->operator HBITMAP();
	m_pBtnSend->SetImgBitmap(m_hBitmap2, m_hBitmap2_dn, m_hBitmap2_hv);
	m_pBtnReserve->SetImgBitmap(m_hBitmap2, m_hBitmap2_dn, m_hBitmap2_hv);
	m_pBtnConfig->SetImgBitmap(m_hBitmap2, m_hBitmap2_dn, m_hBitmap2_hv);

	m_hBitmapUP    = GetAxBitmap(path + "투자매매종합200.bmp")->operator HBITMAP();
	m_hBitmapUP_dn = GetAxBitmap(path + "투자매매종합200_dn.bmp")->operator HBITMAP();
	m_hBitmapUP_hv = GetAxBitmap(path + "투자매매종합200_en.bmp")->operator HBITMAP();
	m_pBtnVSize->SetImgBitmap(m_hBitmapUP, m_hBitmapUP_dn, m_hBitmapUP_hv);

	m_hBitmapCODE    = GetAxBitmap(path + "S검색.bmp")->operator HBITMAP();
	m_hBitmapCODE_dn = GetAxBitmap(path + "S검색_dn.bmp")->operator HBITMAP();
	m_hBitmapCODE_hv = GetAxBitmap(path + "S검색_en.bmp")->operator HBITMAP();
	m_pBtnCode->SetImgBitmap(m_hBitmapCODE, m_hBitmapCODE_dn, m_hBitmapCODE_hv);
}


void CMapWnd::transaction()
{
	/*
	if (m_code.IsEmpty())
	{
		SetGuide("종목을 선택하십시오");	
		return;
	}
*/
	m_pCtrlWnd->ClearMaip();
	m_pPass->GetWindowText(m_pswd);

	((CIB300200App*)AfxGetApp())->SaveBuffer(_T("PASS"), m_pswd); 
	struct	_smid	mid;
	FillMemory(&mid, sz_SMID, ' ');

//	CString pass = (char*)m_pParent->SendMessage(WM_USER, encPASSx, (LPARAM)m_pswd.operator LPCTSTR());

	CopyMemory(mid.code, m_code, sizeof(mid.code));

	CString data((char*)&mid, sz_SMID);

	SendTR("PIBO3002", 0, data, KEY_TRAN);
	const int pos = 0;
	char code[16]{};
	sprintf(code, "%s", m_code.GetString());
	m_codeName = (char*)m_pParent->SendMessage(WM_USER, MAKEWPARAM(nameDLL, 0), (LPARAM)code);
	
	m_output6 = m_codeName;
	
	CString param;
	param.Format("ed_focod\t%s", m_code);
	
	Variant(codeCC, param);
	Variant(triggerCC, param);
	if (m_mapKey > -1)
		m_pParent->SendMessage(WM_USER, MAKEWPARAM(dominoMAP, m_mapKey), (LPARAM)param.operator LPCTSTR());


	if (!CheckPswd(m_acc, m_pswd)) return;

	SendMicheg();
	if (m_pNotify)
	{
		if (m_pCtrlWnd)
			m_pCtrlWnd->ClearJango();

		m_pNotify->SendJango(m_acc, m_pswd, m_code);
	}
} 

COLORREF CMapWnd::GetIndexColor(int index)
{
	return m_pParent->SendMessage(WM_USER, MAKEWORD(getPALETTE, 0), (LPARAM)index);
}

CFont* CMapWnd::GetAxFont(CString fName, int point, bool bItalic, int nBold)
{
	struct _fontR fontR;
	fontR.name   = (LPSTR)fName.operator LPCTSTR();
	fontR.point  = point;
	fontR.italic = bItalic;
	fontR.bold   = nBold;
	return (CFont*)m_pParent->SendMessage(WM_USER, getFONT, (long)&fontR);
}

CBitmap* CMapWnd::GetAxBitmap(CString path)
{
	return (CBitmap*)m_pParent->SendMessage(WM_USER, getBITMAP, (long)path.operator LPCTSTR());
}
HBITMAP CMapWnd::GetAxHBitmap(CString stPath)
{
	CBitmap* pBitmap = GetAxBitmap(stPath);
	if(pBitmap)
		return pBitmap->operator HBITMAP();

	return NULL;
}

CPen* CMapWnd::GetAxPen(COLORREF clr, int width, int style)
{
	struct	_penR penR;
	penR.clr   = clr;
	penR.width = width;
	penR.style = style;
	return (CPen*)m_pParent->SendMessage(WM_USER, getPEN, (long)&penR);
}

CBrush* CMapWnd::GetAxBrush(COLORREF clr)
{
	return (CBrush*)m_pParent->SendMessage(WM_USER, getBRUSH, (long)clr);
}

CString CMapWnd::Variant(int cmd, CString data)
{
	CString retvalue;
	char* dta = (char*)m_pParent->SendMessage(WM_USER, MAKEWPARAM(variantDLL, cmd), (LPARAM)(LPCTSTR)data);

	if ((long)dta > 1)
		retvalue = dta;
	else
		return "";

	return retvalue;
}

int CMapWnd::Variant(int cmd, int data)
{
	return m_pParent->SendMessage(WM_USER, MAKEWPARAM(variantDLL, cmd), (LPARAM)data);
}

void CMapWnd::SetView(int type, CString data)
{
	m_pParent->SendMessage(WM_USER, MAKEWPARAM(viewDLL, type), (LPARAM)(LPCTSTR)data);
}

CString CMapWnd::SetComma(CString src)
{
	CString temp, floatStr;
	int	ii{}, jj = -1;

	src.TrimLeft();
	if (src.Find('.') >= 0)
	{
		floatStr = src.Mid(src.Find('.'));
		src = src.Left(src.Find('.'));
	}
	else
		floatStr.Empty();
	
	temp = src;
	src.Empty();
	
	for (ii = temp.GetLength() - 1; ii >= 0; ii--)
	{
		jj++;
		if (jj == 3)
		{
			if (temp.GetAt(0) != '-' && temp.GetAt(0) != '+')
				src = ',' + src;
			else if (ii != 0)
				src = ',' + src;
			
			jj = 0;
		}
		src = temp.GetAt(ii) + src;
	}
	src += floatStr;
	
	return src;
}

BOOL CMapWnd::SendTR2(LPCSTR name, char type, LPCSTR data, int dlen, int key)
{
	if (strlen(name) != 8) return FALSE;

	std::unique_ptr<char[]>buff = std::make_unique<char[]>(L_userTH + dlen + 1);

	// Header setting
	struct _userTH* udat = (struct _userTH*)buff.get();
	CopyMemory(udat->trc, name, 8);
	udat->key = key;
	udat->stat = type;

	// Data setting
	CopyMemory(&buff.get()[L_userTH], data, dlen);

	return m_pParent->SendMessage(WM_USER, MAKEWPARAM(invokeTRx, dlen), (LPARAM)buff.get());
}


BOOL CMapWnd::SendTR(CString name, BYTE type, CString data, int key)
{
	const int iLength = data.GetAllocLength();
	std::unique_ptr<char[]> pcDataBuffer = std::make_unique<char[]>(L_userTH + iLength);
	ZeroMemory(pcDataBuffer.get(), sizeof(pcDataBuffer));
	struct	_userTH* puserTH;
	puserTH = (struct _userTH*)pcDataBuffer.get();

	CopyMemory(puserTH->trc, name, sizeof(puserTH->trc));
	puserTH->key = key;
	puserTH->stat = type;

	CopyMemory(&pcDataBuffer.get()[L_userTH], data.GetString(), iLength);

	const LRESULT lResult = m_pParent->SendMessage(WM_USER, MAKEWPARAM(invokeTRx, iLength), (LPARAM)pcDataBuffer.get());

	pcDataBuffer = nullptr;
	return lResult;
}


CString CMapWnd::Parser(CString &srcstr, CString substr)
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

void CMapWnd::DrawBitmap(CDC* pDC, CRect rc, HBITMAP hBitmap)
{
	HDC hDC = ::CreateCompatibleDC (NULL);
	SelectObject (hDC, hBitmap); //icon
	//::BitBlt( pDC->m_hDC, rc.left, rc.top, rc.Width(), rc.Height(),	hDC, 0, 0, SRCCOPY);
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
	if(!m_pHogaWnd) return;
	CPaintDC dc(this); // device context for painting
	CMemDC memDC(&dc);

	CRect rc, cRc, wrc, rc2, rc3;
	GetClientRect(&rc);

	m_pHogaWnd->GetWindowRect(&wrc);
	ScreenToClient(wrc);

	const int bkMode = memDC.SetBkMode(TRANSPARENT);

	const COLORREF toolbgcolor = GetIndexColor(COLOR_PANELBACK);
	const COLORREF bodybgcolor = GetIndexColor(COLOR_BODYBACK);
	memDC.FillSolidRect(rc, GetIndexColor(COLOR_BK));			
//	memDC.FillSolidRect(rc, 0x880000);			

	CPen *pPen = GetAxPen(GetIndexColor(COLOR_LINE), 1, PS_SOLID);
	CPen *gPen = GetAxPen(GetIndexColor(89), 1, PS_SOLID);
	CPen *pOldPen = memDC.SelectObject(pPen);
	memDC.SelectStockObject(NULL_BRUSH);

	rc2 = CRect(FRAME_SPACE, VERT_GAP, wrc.right + INNER_FRAME_SPACE, wrc.bottom  + VERT_GAP);
	rc3 = rc2;

	rc2.bottom = m_oRc10.bottom + INNER_FRAME_SPACE;
	memDC.FillSolidRect(rc2, m_CmdBgColor);	
	DrawCornerRoundBitmap(&memDC, rc2);

	rc3.left = rc2.right + OUTER_FRAME_SPACE;
	rc3.right = rc.right - FRAME_SPACE;
	memDC.FillSolidRect(rc3, m_ContentsBgColor);	
	DrawCornerRoundBitmap(&memDC, rc3);

	CRect rc4 = rc3;
	rc4.left = rc2.left;
	rc4.right = rc2.right;
	rc4.top = rc2.bottom + OUTER_FRAME_SPACE;
	memDC.FillSolidRect(rc4, m_ContentsBgColor);	
	DrawCornerRoundBitmap(&memDC, rc4);


// line 1
	cRc = m_oRc1;
	cRc.DeflateRect(2, 0);
	memDC.FillSolidRect(&m_oRc1, GetIndexColor(160));
	memDC.Rectangle(&m_oRc1);
	DrawSiseText(&memDC, m_output1, cRc, DT_LEFT, 0,m_txtcolor);

	gPen = GetAxPen(GetIndexColor(87), 1, PS_SOLID);

// line 3
	memDC.FillSolidRect(&m_oRc6, GetIndexColor(COLOR_TABLEHEAD));
	memDC.Rectangle(&m_oRc6);
	DrawSiseText(&memDC, m_output6, m_oRc6, DT_LEFT, 0,m_txtcolor);

// sise table
	memDC.FillSolidRect(&m_tRc2, GetIndexColor(COLOR_GRIDHEAD));	memDC.Rectangle(&m_tRc2);
	memDC.FillSolidRect(&m_tRc3, GetIndexColor(COLOR_GRIDHEAD));	memDC.Rectangle(&m_tRc3);
	memDC.FillSolidRect(&m_tRc4, GetIndexColor(COLOR_GRIDHEAD));	memDC.Rectangle(&m_tRc4);
	memDC.FillSolidRect(&m_tRc5, GetIndexColor(COLOR_GRIDHEAD));	memDC.Rectangle(&m_tRc5);
	memDC.FillSolidRect(&m_tRc7, GetIndexColor(COLOR_GRIDHEAD));	memDC.Rectangle(&m_tRc7);
	memDC.FillSolidRect(&m_tRc8, GetIndexColor(COLOR_GRIDHEAD));	memDC.Rectangle(&m_tRc8);
	memDC.FillSolidRect(&m_tRc9, GetIndexColor(COLOR_GRIDHEAD));	memDC.Rectangle(&m_tRc9);
	memDC.FillSolidRect(&m_tRc10, GetIndexColor(COLOR_GRIDHEAD));	memDC.Rectangle(&m_tRc10);

	DrawSiseText(&memDC, "현재가", m_tRc2, DT_CENTER, 0, m_GridHdrTxtColor);
	DrawSiseText(&memDC, "대  비", m_tRc3, DT_CENTER, 0, m_GridHdrTxtColor);
	DrawSiseText(&memDC, "등락률", m_tRc4, DT_CENTER, 0, m_GridHdrTxtColor);
	DrawSiseText(&memDC, "거래량", m_tRc5, DT_CENTER, 0, m_GridHdrTxtColor);
	DrawSiseText(&memDC, "시  가", m_tRc7, DT_CENTER, 0, m_GridHdrTxtColor);
	DrawSiseText(&memDC, "고  가", m_tRc8, DT_CENTER, 0, m_GridHdrTxtColor);
	DrawSiseText(&memDC, "저  가", m_tRc9, DT_CENTER, 0, m_GridHdrTxtColor);
	DrawSiseText(&memDC, "미결제", m_tRc10, DT_CENTER, 0, m_GridHdrTxtColor);

	memDC.FillSolidRect(&m_oRc2, GetIndexColor(COLOR_DATA));	memDC.Rectangle(&m_oRc2);
	memDC.FillSolidRect(&m_oRc3, GetIndexColor(COLOR_DATA));	memDC.Rectangle(&m_oRc3);
	memDC.FillSolidRect(&m_oRc4, GetIndexColor(COLOR_DATA));	memDC.Rectangle(&m_oRc4);
	memDC.FillSolidRect(&m_oRc5, GetIndexColor(COLOR_DATA));	memDC.Rectangle(&m_oRc5);
	memDC.FillSolidRect(&m_oRc7, GetIndexColor(COLOR_DATA));	memDC.Rectangle(&m_oRc7);
	memDC.FillSolidRect(&m_oRc8, GetIndexColor(COLOR_DATA));	memDC.Rectangle(&m_oRc8);
	memDC.FillSolidRect(&m_oRc9, GetIndexColor(COLOR_DATA));	memDC.Rectangle(&m_oRc9);
	memDC.FillSolidRect(&m_oRc10, GetIndexColor(COLOR_DATA));	memDC.Rectangle(&m_oRc10);

	DrawSiseText(&memDC, m_output2, m_oRc2, DT_RIGHT, ST_COLOR | ST_COMMA | ST_REMOVE | ST_NORMAL | ST_BOLD, m_txtcolor);
	DrawSiseText(&memDC, m_output3, m_oRc3, DT_RIGHT, ST_COMMA | ST_REMOVE | ST_NORMAL | ST_BOLD | ST_SHGA, m_txtcolor);
	DrawSiseText(&memDC, m_output4, m_oRc4, DT_RIGHT, ST_COLOR | ST_PLUS_REMOVE | ST_COMMA | ST_NORMAL | ST_BOLD, m_txtcolor);
	DrawSiseText(&memDC, m_output5, m_oRc5, DT_RIGHT, ST_COLOR | ST_COMMA | ST_REMOVE | ST_NORMAL | ST_BOLD, m_txtcolor);
	DrawSiseText(&memDC, m_output7, m_oRc7, DT_RIGHT, ST_COLOR | ST_COMMA | ST_REMOVE | ST_NORMAL | ST_BOLD, m_txtcolor);
	DrawSiseText(&memDC, m_output8, m_oRc8, DT_RIGHT, ST_COLOR | ST_COMMA | ST_REMOVE | ST_NORMAL | ST_BOLD, m_txtcolor);
	DrawSiseText(&memDC, m_output9, m_oRc9, DT_RIGHT, ST_COLOR | ST_COMMA | ST_REMOVE | ST_NORMAL | ST_BOLD, m_txtcolor);
	DrawSiseText(&memDC, m_output10, m_oRc10, DT_RIGHT, ST_COLOR | ST_COMMA | ST_REMOVE | ST_NORMAL | ST_BOLD, m_txtcolor);

// guide
	memDC.FillSolidRect(&m_rcGuide, m_GuideBgColor);	
	memDC.Rectangle(&m_rcGuide);
	DrawCornerRoundBitmap(&memDC, m_rcGuide);

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

	memDC.SetTextColor(m_GuideTxtColor);
	DrawSiseText(&memDC, m_guide, guideTxtrc, DT_LEFT, 0, m_GuideTxtColor);

	memDC.SelectObject(pOldPen);
	memDC.SetBkMode(bkMode);
	//Sleep(50);
}

long CMapWnd::OnMessage(WPARAM wParam, LPARAM lParam)
{
	switch(LOBYTE(LOWORD(wParam)))
	{
	case DLL_OUB:
		{
			CString strTemp;
			CString strLedger(_T(""));
			CString strErrCode(_T(""));
			CString strErrText(_T(""));
			CString strMsg(_T(""));
			struct _ledgerH ledger;
			const int key = HIBYTE(LOWORD(wParam));
			int len = HIWORD(wParam);

			switch (key)
			{
			case KEY_CHKPASS:
				ParsingCheckPswd( CString((char*)lParam,HIWORD(wParam)) );
				break;
				
			case KEY_ACCOUNT:
			case KEY_ACCNAME:
			case 250:
				m_pAccount->m_pAccountCtrl->SendMessage(WM_USER, wParam, lParam);
				break;
			case KEY_TRAN:
				dispatchData((char *)lParam, HIWORD(wParam));
				break;
			case KEY_JUMUN:
				{
					len = HIWORD(wParam);
					
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
							SetGuide(emsg);
						}
						
					}

					{
						CString strData((char*) (lParam + L_ledgerH), len - L_ledgerH);
						
						if (strData.GetLength() < 21)
							return 0;
						
						strData.Delete(0, 21);
						SetGuide(strData);
					}
				}
//				TRACE("%s\n", (char *)lParam);
				break;
			case KEY_CANALLS:	// 매도주문가능수량+청산가능수량
			case KEY_CANALLB:	// 매수주문가능수량+청산가능수량
				{
					strTemp = (char*)lParam;
					CopyMemory(&ledger, (void*)lParam, L_ledgerH);
					strLedger = CString((char*)&ledger, L_ledgerH);
					strErrCode = CString((char*)&ledger.emsg, 4);
					strErrText = CString((char*)&ledger.emsg, 98);
						
					strMsg = "ERR\t" + strErrText;

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

					lParam += L_ledgerH;

					if (key == KEY_CANALLS)
						m_pCtrlWnd->SetCanClearS((char*)lParam);
					else
						m_pCtrlWnd->SetCanClearB((char*)lParam);

				}
				break;
			case KEY_MICHEG:
				if (m_bMicheContinue)
				{
					m_pHogaWnd->DispatchMicheg((char *)(lParam + m_lenLedger), HIWORD(wParam), false);
					m_pCtrlWnd->DispatchMicheg((char *)(lParam + m_lenLedger), HIWORD(wParam), false);
				}
				else
				{
					m_pHogaWnd->DispatchMicheg((char *)(lParam + m_lenLedger), HIWORD(wParam));
					m_pCtrlWnd->DispatchMicheg((char *)(lParam + m_lenLedger), HIWORD(wParam));
				}

				struct _ledgerH ledger;
				CopyMemory(&ledger, (void*)lParam, L_ledgerH);
			
				if (ledger.next[0] == _T('Y'))
					SendMicheg(CString((char*)&ledger, L_ledgerH));
				break;
			}
		}	
		break;
	//case DLL_ALERT:
	//	DoParse((char*)lParam);
	case DLL_ALERTx:
		DoParsex((struct _alertR *)lParam);
		break;
	case DLL_NOTICE:
		m_Section.Lock();
		dispatchNotice((char*)lParam);
		m_pHogaWnd->DispatchNotice((char*)lParam);
		m_pCtrlWnd->SetMicheg(m_code, m_pHogaWnd->getMichegCount());
		m_Section.Unlock();
		break;
	case DLL_TRIGGER:
		{
			CString szTrigger((char*)lParam);
			
			while (!szTrigger.IsEmpty())
			{
				CString str = Parser(szTrigger, "\n"), sym = Parser(str, "\t");
				if (!sym.CompareNoCase("ed_focod"))
				{
#if 0
					char code[16];
					sprintf(code, "%s", str);
					CString name = (char*)m_pParent->SendMessage(WM_USER, MAKEWPARAM(nameDLL, 0), (LPARAM)code);
#endif
					str.TrimRight();

					if (str.GetLength() >= 0 && (str.Left(3) == "103" || str.Left(1) == "4"))
						return 0;
					SetCode(str);
				}
				else if (!sym.CompareNoCase("AN21"))
				{
					m_pAccount->TriggerAccount(str);
				}
			}
		}
		break;
	case DLL_SETPAL:
		ChangePallete();
		break;
	case DLL_DOMINO:
		{
			CString szDomino((char*)lParam);
			
			while (!szDomino.IsEmpty())
			{
				CString str = Parser(szDomino, "\n"), sym = Parser(str, "\t");
				if (!sym.CompareNoCase("ed_focod"))
				{
					if (str.GetLength() >= 0 && (str.Left(3) == "103" || str.Left(3) == "403"))
						return 0;

					CString sval = ((CIB300200App*)AfxGetApp())->GetBuffer(_T("PASS")); 
					if(!sval.IsEmpty()) 
						m_pPass->SetWindowText(sval);
					SetCode(str);
				}
				else if (!sym.CompareNoCase("AN21"))
				{
					m_pAccount->TriggerAccount(str);
				}
			}
		}
		break;
	case DLL_SETFONT:break;
	case DLL_SETFONTx:
		ChangeFont( HIWORD(wParam), (char*)lParam);

	case DLL_GUIDE:
		SetGuide((char *)lParam);
		return true;
		break;
	}
	return 0;
}

void CMapWnd::ChangePallete()
{
	LoadRoundBitmap();
	m_CmdBgColor = GetIndexColor(CMD_BACK_INDEX);
	m_ContentsBgColor = GetIndexColor(CONTENTS_BACK_INDEX);
	m_GuideBgColor = GetIndexColor(GUIDE_BACK_INDEX);
	m_ContentsBorderColor = GetIndexColor(CONTENTS_BORDER_INDEX);
	m_GuideTxtColor = GetIndexColor(GUIDE_TXT_INDEX);
	m_GridHdrTxtColor = GetIndexColor(COLOR_GRIDHEADTXT);
	m_txtcolor =  GetIndexColor(COLOR_TEXT);


	Invalidate();
	m_pHogaWnd->Invalidate();
	m_pCtrlWnd->Invalidate();
}

long CMapWnd::OnUser(WPARAM wParam, LPARAM lParam)
{
	switch(LOWORD(wParam))
	{
	case 100:	// CAccountCtrl 계좌변경시 수신 Message
		{
			/* 비밀번호 검증여부 flag 초기화 */
			bPasswordChecked = FALSE;
			m_pPass->EnableWindow(TRUE);
			m_pPass->SetWindowText("");
			m_pHogaWnd->ClearJumun();

			CString data = (char*)lParam;

			CString acc = Parser(data, "\t");
			m_output1 = Parser(data, "\t");
			m_pswd = Parser(data, "\t");

			if (m_acc != acc)
			{
				if (m_pCtrlWnd)
					m_pCtrlWnd->ClearJango();
				if (m_pHogaWnd)
					m_pHogaWnd->ClearJumun();
			}

			BOOL bDiff = FALSE;
			
			acc.TrimLeft();
			acc.TrimRight();

			if(m_acc == acc)
			{
				bDiff = TRUE;
			}

			m_acc = acc;

			if (m_pPass)
			{
				m_pPass->SetWindowText(m_pswd);
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

			if (!m_pswd.IsEmpty())
			{
				if(bDiff == FALSE)
					transaction();
			}

			InvalidateRect(&m_oRc1);
		}
		break;

	case 200: //CX_OptionEx 코드 체인지
		{
			SetCode((char*)lParam);
		}
		break;
	}
	return 0;
}

void CMapWnd::OnSetFocus(CWnd* pOldWnd)
{
	CWnd::OnSetFocus(pOldWnd);
	
}

BOOL CMapWnd::OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult) 
{
	if (wParam == ID_CTRL_PASS)
	{
/*
		CString pswd;
		m_pPass->GetWindowText(pswd);

		NMHDR* pNMHDR = (NMHDR*)lParam;
		if (pNMHDR->code == ENM_VKRETURN || pNMHDR->code == VK_TAB)
		{	
			bool ret = m_pAccount->ConfirmPswd(pswd);
			if (ret)
			{
				m_pswd = pswd;
				transaction();
			}
		}
*/
		transaction();
	}
	
	return CWnd::OnNotify(wParam, lParam, pResult);
}

void CMapWnd::OnHistoryCode()
{
	CString history = Variant(historyCC, "ed_focod");
	m_pCodeCtrl->SetHistory(Variant(historyCC, "ed_focod"));
}

void CMapWnd::OnCodeSelEndOk()
{
	CString code, name;
	code = m_pCodeCtrl->GetEditData();

	char code2[16];
	sprintf(code2, "%s", code.GetString());
	name = (char*)m_pParent->SendMessage(WM_USER, MAKEWPARAM(nameDLL, 0), (LPARAM)code2);

	if (name.IsEmpty() || code.GetLength() != 8)	return;

	m_code = code;
	m_code.TrimLeft();
	m_code.TrimRight();

	if (m_pOptionEx)
		m_pOptionEx->SetCode(m_code);

	transaction();
}

void CMapWnd::OnButtonCode()
{
	CRect rect;
	GetDlgItem(ID_CTRL_CODEBTN)->GetWindowRect(rect);
	CString strCODE = (char*) m_pParent->SendMessage(WM_USER, MAKEWPARAM(codeDLL, MAKEWORD(foptionCODE, NULL)), MAKELPARAM(rect.left, rect.bottom + 2));

	const int iPos = strCODE.Find('\t');
	if (iPos > 0)
	{
		strCODE = strCODE.Left(iPos);
		if (strCODE.GetLength() == 5)
			strCODE = strCODE + "000";

		m_code = strCODE;
		m_code.TrimLeft();
		m_code.TrimRight();
		m_pCodeCtrl->SetEditData(m_code);

		if (m_pOptionEx)
			m_pOptionEx->SetCode(m_code);

		m_pCodeCtrl->SetFocus();

		transaction();
	}

}

void CMapWnd::OnButtonSend()
{
	if(m_acc.IsEmpty()) 
		return;
	if (m_code.IsEmpty())
	{
		SetGuide("종목을 선택하십시오");	
		return ;
	}

	transaction();
}

void CMapWnd::OnButtonVSize()
{
	CRect cRc, hRc, rc;
	GetClientRect(&rc);
	m_pHogaWnd->GetClientRect(&hRc);
	m_pCtrlWnd->GetClientRect(&cRc);

	m_bhogaResizeClick = TRUE;
	const int ihogaRow = m_pHogaWnd->GetRowheight();
	const int ichangeH = ihogaRow * 10;

	if (m_depth == hgDEPTH10)
	{
		m_depth = hgDEPTH5;
		const int itmp = m_iWndHeight - ichangeH;
		SetWindowPos(NULL, 0, 0, rc.Width(), itmp, SWP_NOZORDER | SWP_NOMOVE);
		m_pParent->SendMessage(WM_USER, MAKEWPARAM(sizeDLL, NULL), MAKELPARAM(m_iWndWidth, itmp));
		m_rcGuide.OffsetRect(0, -ichangeH);
		m_pHogaWnd->SetWindowPos(NULL, 0, 0, hRc.Width(), hRc.Height() - ichangeH, SWP_NOZORDER | SWP_NOMOVE);
		m_pCtrlWnd->SetWindowPos(NULL, 0, 0, cRc.Width(), cRc.Height() - ichangeH, SWP_NOZORDER | SWP_NOMOVE);

		
//		SetWindowPos(NULL, 0, 0, rc.Width(), MAP_HEIGHT2, SWP_NOZORDER | SWP_NOMOVE);
//		m_pParent->SendMessage(WM_USER, MAKEWPARAM(sizeDLL, NULL), MAKELPARAM(rc.Width(), MAP_HEIGHT2));

//		m_rcGuide.OffsetRect(0, -180);
//		m_pHogaWnd->SetWindowPos(NULL, 0, 0, hRc.Width(), hRc.Height() - 180, SWP_NOZORDER | SWP_NOMOVE);
//		m_pCtrlWnd->SetWindowPos(NULL, 0, 0, cRc.Width(), cRc.Height() - 180, SWP_NOZORDER | SWP_NOMOVE);
	}
	else
	{
		m_depth = hgDEPTH10;

		SetWindowPos(NULL, 0, 0, rc.Width(), m_iWndHeight, SWP_NOZORDER | SWP_NOMOVE);
		m_pParent->SendMessage(WM_USER, MAKEWPARAM(sizeDLL, NULL), MAKELPARAM(m_iWndWidth, m_iWndHeight));

		m_rcGuide.OffsetRect(0, ichangeH);
		m_pHogaWnd->SetWindowPos(NULL, 0, 0, hRc.Width(), hRc.Height() + ichangeH, SWP_NOZORDER | SWP_NOMOVE);
		m_pCtrlWnd->SetWindowPos(NULL, 0, 0, cRc.Width(), cRc.Height() + ichangeH, SWP_NOZORDER | SWP_NOMOVE);

//		SetWindowPos(NULL, 0, 0, rc.Width(), MAP_HEIGHT, SWP_NOZORDER | SWP_NOMOVE);
//		m_pParent->SendMessage(WM_USER, MAKEWPARAM(sizeDLL, NULL), MAKELPARAM(rc.Width(), MAP_HEIGHT));

//		m_rcGuide.OffsetRect(0, 180);
//		m_pHogaWnd->SetWindowPos(NULL, 0, 0, hRc.Width(), hRc.Height() + 180, SWP_NOZORDER | SWP_NOMOVE);
//		m_pCtrlWnd->SetWindowPos(NULL, 0, 0, cRc.Width(), cRc.Height() + 180, SWP_NOZORDER | SWP_NOMOVE);
	}

	CString sval;
	sval.Format("%d", m_depth);
	((CIB300200App*)AfxGetApp())->SaveBuffer(_T("VTYPE"), sval); 

	m_pHogaWnd->SetHogaDepth(m_depth);
	m_pCtrlWnd->SetSize(m_depth);
	if( !IsWindowVisible()) ShowWindow(SW_SHOW);
}

void CMapWnd::OnButtonHoga1()
{

	return;
}

void CMapWnd::OnButtonHoga3()
{

	return;
}

void CMapWnd::OnButtonConfig()
{
	m_pHogaWnd->DoModalConfigDlg();

	return ;
}

void CMapWnd::OnButtonReserve()
{
	const int ret = m_pHogaWnd->SetHogaReserve();

	if (ret)
		m_pBtnReserve->SetWindowText("호가"); 
	else
		m_pBtnReserve->SetWindowText("STOP");

	return;
}

void CMapWnd::dispatchData(char* pData, int len)
{
	removeRTM();

	struct _smod* pMod = (struct _smod *)pData;
//	int nrec = atoi(CString(pMod->nrec, sizeof(pMod->nrec)));

//	if (len != (int)(sz_SMOD + nrec * sz_JINFO))
//		return;

	m_output2 = CString(pMod->curr, sizeof(pMod->curr));
	m_output3 = CString(pMod->diff, sizeof(pMod->diff));	m_output3.TrimLeft();
	m_output4 = CString(pMod->rate, sizeof(pMod->rate));	m_output4.TrimLeft();
	m_output5 = CString(pMod->gvol, sizeof(pMod->gvol));
	m_output7 = CString(pMod->siga, sizeof(pMod->siga));	m_output7.TrimLeft();
	m_output8 = CString(pMod->koga, sizeof(pMod->koga));	m_output8.TrimLeft();
	m_output9 = CString(pMod->jega, sizeof(pMod->jega));	m_output9.TrimLeft();
	m_output10 = CString(pMod->mgjv, sizeof(pMod->mgjv));	m_output10.TrimLeft();
	m_pHogaWnd->DispatchData(pData, len);
	m_pCtrlWnd->DispatchData(pData, len);

	Invalidate();
}



void CMapWnd::dispatchAlert(CString alert)
{
	int index = alert.Find('\t');
	if (index <= 0)
		return;

	CString val, code = alert.Left(index++);

	if (m_code != code)
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
			if (m_output2 != val)
			{
				m_output2 = val;
				InvalidateRect(m_oRc2);
			}
		}
		else if (symbol == "024")		// 전일대비
		{
			if (m_output3 != val)
			{
				m_output3 = val;
				InvalidateRect(m_oRc3);
			}
		}
		else if (symbol == "033")		// 등락율
		{
			if (m_output4 != val)
			{
				m_output4 = val;
				InvalidateRect(m_oRc4);
			}
		}
		else if (symbol == "027")		// 거래량
		{
			if (m_output5 != val)
			{
				m_output5 = val;
				InvalidateRect(m_oRc5);
			}
		}
		else if (symbol == "029")		// 시가
		{
			if (m_output7 != val)
			{
				m_output7 = val;
				InvalidateRect(m_oRc7);
			}
		}
		else if (symbol == "030")		// 고가
		{
			if (m_output8 != val)
			{
				m_output8 = val;
				InvalidateRect(m_oRc8);
			}
		}
		else if (symbol == "031")		// 저가
		{
			if (m_output9 != val)
			{
				m_output9 = val;
				InvalidateRect(m_oRc9);
			}
		}
		else if (symbol == "201")		// 미결약정
		{
			if (m_output10 != val)
			{
				m_output10 = val;
				InvalidateRect(m_oRc10);
			}
		}
	}
}


void CMapWnd::dispatchAlertX(struct _alertR* alertR)
{
	DWORD* data = nullptr;

	if (alertR->size <= 0)
		return;

	CString val, code = alertR->code;

	if (m_code != code)
		return;

	data = (DWORD*)alertR->ptr[0];

	//for (POSITION pos = rtmStore.GetStartPosition(); pos;)
	{
		//rtmStore.GetNextAssoc(pos, symbol, val);

		if (data[23])			// 현재가
		{
			CString val = (char*)data[23];
			if (m_output2 != val)
			{
				m_output2 = val;
				InvalidateRect(m_oRc2);
			}
		}
		if (data[24])		// 전일대비
		{
			CString val = (char*)data[24];
			if (m_output3 != val)
			{
				m_output3 = val;
				InvalidateRect(m_oRc3);
			}
		}
		if (data[33])		// 등락율
		{
			CString val = (char*)data[33];
			if (m_output4 != val)
			{
				m_output4 = val;
				InvalidateRect(m_oRc4);
			}
		}
		if (data[27])		// 거래량
		{
			CString val = (char*)data[27];
			if (m_output5 != val)
			{
				m_output5 = val;
				InvalidateRect(m_oRc5);
			}
		}
		if (data[29])		// 시가
		{
			CString val = (char*)data[29];
			if (m_output7 != val)
			{
				m_output7 = val;
				InvalidateRect(m_oRc7);
			}
		}
		if (data[30])		// 고가
		{
			CString val = (char*)data[30];
			if (m_output8 != val)
			{
				m_output8 = val;
				InvalidateRect(m_oRc8);
			}
		}
		if (data[31])		// 저가
		{
			CString val = (char*)data[31];
			if (m_output9 != val)
			{
				m_output9 = val;
				InvalidateRect(m_oRc9);
			}
		}
		if (data[201])		// 미결약정
		{
			CString val = (char*)data[201];
			if (m_output10 != val)
			{
				m_output10 = val;
				InvalidateRect(m_oRc10);
			}
		}
	}
}

void CMapWnd::dispatchNotice(CString notice)
{

}
//m_GridHdrTxtColor
void CMapWnd::DrawSiseText(CDC* pDC, CString data, CRect rc, UINT nFormat, UINT nType, COLORREF txtcolor)
{
	rc.DeflateRect(5, 0);
	nFormat = DT_SINGLELINE | DT_VCENTER | nFormat;
	COLORREF color{}, clrText = txtcolor;//GetIndexColor(COLOR_TEXT);
	int	pos = -1;
	CString value, giho = "", percent = "";

	if (data.IsEmpty())	return;

	CFont* pOldFont{};
	if (nType & ST_BOLD)
		pOldFont = pDC->SelectObject(m_pBFont);
	else
		pOldFont = pDC->SelectObject(m_pFont);

	if (nType & ST_TEXT)
	{
		pDC->DrawText(data, rc, nFormat);
		return;
	}

	if (nType & ST_SHGA)
	{
		if (data[0] == '+')
		{
			giho = "▲ ";
			clrText = GetIndexColor(94);
		}
		else if (data[0] == '-')
		{
			giho = "▼ ";
			clrText = GetIndexColor(95);
		}
		else
		{
			giho = "";
			clrText = GetIndexColor(COLOR_TEXT);
		}

		data = data.Mid(1);
	}

	if (nType & ST_COLOR)
	{
		pos = data.Find('+');
		if (pos >= 0)
			clrText = GetIndexColor(94);

		pos = data.Find('-');
		if (pos >= 0)
			clrText = GetIndexColor(95);
	}

	if (nType & ST_PERCENT)
		percent = " %";

	if (nType & ST_REMOVE)
	{
		data.Remove('+');
		data.Remove('-');
	}

	if (nType & ST_PLUS_REMOVE)
	{
		data.Remove('+');
	}

	if (nType & ST_NORMAL)
	{
		CString pre = _T("");
		pos = data.Find('+');
		if (pos >= 0)
		{
			pre = "+";
			data.Remove('+');
		}

		pos = data.Find('-');
		if (pos >= 0)
		{
			pre = "-";
			data.Remove('-');
		}

		data.Remove(' ');
/*		while (true)
		{
			if (data.IsEmpty())
				break;

			if (data.GetAt(0) == '0')
				data = data.Mid(1);
			else
				break;
		}
*/
		pos = data.Find('.');
		if (pos >= 0)
			value.Format("%.2f", atof(data));
		else
			value.Format("%d", atoi(data));

		if (atof(data) == 0.0)
			data = "";
		else
			data = pre + value;
	}

	if (nType & ST_COMMA)
	{
		data = SetComma(data);	
	}

	color = pDC->SetTextColor(clrText);
	pDC->DrawText(giho + data + percent, rc, nFormat);

	pDC->SelectObject(pOldFont);
	pDC->SetTextColor(color);
}

void CMapWnd::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	
	CWnd::OnLButtonDown(nFlags, point);
}

void CMapWnd::SendJumun(CString jflg, double price, int juno, int vol, bool bmarket)
{
	//본사직원 주문 안나가도록 예외 처리 [ Start ]
	if (!(long)m_pParent->SendMessage(WM_USER, MAKEWPARAM(variantDLL, orderCC), 0L))
	{
		if (!(long)m_pParent->SendMessage(WM_USER, MAKEWPARAM(variantDLL, orderCCx), 0L))
		{
			MessageBox("본사직원은 주문이 불가합니다.","IBK투자증권");
			return;
		}	
	}
	//본사직원 주문 안나가도록 예외 처리 [ End ]
	
	CString	pswd;
	m_pPass->GetWindowText(pswd);
	
	CString jprc, jqty, ogno, ledger;
	jprc.Format("%.2f", price);
	jprc.Remove('.');

	if (vol == 0 && (atoi(jflg) != 3 && atoi(jflg) != 4))
		vol = m_pCtrlWnd->GetJumunCount();

	jqty.Format("%d", vol);
	ogno.Format("%d", juno);

	ledger = m_ledger.LedgerTR("1141", "2", "", "", GetEncPassword(pswd));	//2013.12.23 KSJ 일방향암호화 추가
	m_lenLedger = ledger.GetLength();

	struct _jmid mid;
	FillMemory(&mid, sz_JMID, ' ');
	CopyMemory(mid.nrec, "0001", sizeof(mid.nrec));
	mid.odgb = '2';
	mid.mmgb = jflg[0];
	CopyMemory(mid.acno, m_acc, sizeof(mid.acno));

	CopyMemory(mid.pswd, "HEAD", 4);	//2013.12.23 KSJ 일방향암호화

/*
#ifdef	ENCTEST
	CString passX;
	passX.Format("%s\t%s", pswd, m_acc.Left(8));
	passX = (char*)m_pParent->SendMessage(WM_USER, MAKEWPARAM(encPASSx, modeACN), (long)passX.operator LPCTSTR());
	CopyMemory(mid.pswd, passX, passX.GetLength());
#else
	CopyMemory(mid.pswd, pswd, pswd.GetLength());
	CopyMemory(mid.pswd, "HEAD", 4);	//2013.12.23 KSJ 일방향암호화
#endif
*/
	
	CopyMemory(mid.ogno, ogno, ogno.GetLength());
	CopyMemory(mid.code, m_code, m_code.GetLength());
	CopyMemory(mid.jqty, jqty, jqty.GetLength());
	CopyMemory(mid.jprc, jprc, jprc.GetLength());
	
	if(bmarket)
		CopyMemory(mid.hogb, "03", sizeof(mid.hogb));
	else
		CopyMemory(mid.hogb, "00", sizeof(mid.hogb));

	mid.mdgb = '2';
	mid.prgb = 'X';

	if (atoi(jflg) == 1 || atoi(jflg) == 2)// || atoi(jflg) == 3)	정정은??
		mid.jmgb[0] = m_pCtrlWnd->GetJmgb().GetAt(0);
//		CopyMemory(mid.jmgb, m_pCtrlWnd->GetJmgb(), sizeof(mid.jmgb));	// 주문조건추가
	
	CString jmid((char *)&mid, sz_JMID);
	
	ledger += jmid;
	SendTR("PIBOFODR", US_CA|US_ENC, ledger, KEY_JUMUN);
}

void CMapWnd::SetCode(CString code)
{
	if (m_pCodeCtrl)
		m_pCodeCtrl->SetEditData(code);

	if (m_pOptionEx)
		m_pOptionEx->SetCode(code);

	BOOL bDiff = FALSE;
	
	code.TrimLeft();
	code.TrimRight();
	
	if(m_code == code)
	{
		bDiff = TRUE;
	}

	m_code = code;

	if(bDiff == FALSE)
		transaction();
}

CString CMapWnd::getEncPass(CString sPass, CString sAcc)
{
	CString tmpS;

	tmpS.Format("%s\t%s", sPass, CString(sAcc, 8));
	sPass = (char*)m_pParent->SendMessage(WM_USER, MAKEWPARAM(encPASSx, 0), (long)tmpS.operator LPCTSTR());
	return sPass;
}

void CMapWnd::SendMaxJmcn(CString jmdg)
{
	if (m_acc.IsEmpty())
	{
		SetGuide("계좌번호를 입력하세요.");
		return;
	}

	m_pPass->GetWindowText(m_pswd);
	if (m_pswd.IsEmpty())
	{
		SetGuide("비밀번호를 입력하세요.");
		return;
	}

	CString strUser = Variant(userCC, "");
	CString strEncPass(_T(""));
	strEncPass.Format("%s\t%s", m_pswd, m_acc.Left(8));
	CString strPassword = (char *)m_pParent->SendMessage(WM_USER, MAKEWPARAM(encPASSx, modeACN), (LPARAM)(LPCTSTR)strEncPass);

	struct _ledgerH ledger;

	FillMemory(&ledger, L_ledgerH, ' ');
	m_pParent->SendMessage(WM_USER, ledgerDLL, (LPARAM)&ledger);

	CopyMemory(&ledger.svcd, _T("SONBQ101"), sizeof(ledger.svcd));
	CopyMemory(&ledger.usid, (LPCTSTR)strUser, strUser.GetLength());
	CopyMemory(&ledger.brno, m_acc.Left(3), 3);
	CopyMemory(&ledger.rcnt, _T("0000"), sizeof(ledger.rcnt));

	ledger.fkey[0] = 'C';
	ledger.mkty[0] = '2';
	ledger.odrf[0] = '2';

	CString strLedger = CString((char*)&ledger, L_ledgerH);
//	strLedger = _T(" ..\"...9999piboPBxQ.....000551B00262SONBQ101068890358          HT109000000092@HQ1410561525678            172017004133000000001     K1056350910136 0C   0000                         780022A101130101210005172016205020                                                  00136 ...... ...............                                                ");
	m_lenLedger = strLedger.GetLength();

	struct max_mid mid;
	FillMemory(&mid, L_max_mid, _T(' '));

	CopyMemory(&mid.In, _T("00001"), sizeof(mid.In));
	CopyMemory(&mid.zAcntNo, (LPCTSTR)m_acc, m_acc.GetLength());
	CopyMemory(&mid.zPwd, (LPCTSTR)strPassword, strPassword.GetLength());
	mid.zQryTp[0] = _T('1');
	CopyMemory(&mid.lOrdAmt, _T("+000000000000000"), sizeof(mid.lOrdAmt));
	CopyMemory(&mid.dRatVal, _T("+000000000.00000000"), sizeof(mid.dRatVal));
	CopyMemory(&mid.zFnoIsuNo, (LPCTSTR)m_code, m_code.GetLength());
	mid.zBnsTp[0] = _T('1');
	CopyMemory(&mid.dOrdPrc, _T("+000000000.00"), sizeof(mid.dOrdPrc));
	CopyMemory(&mid.zFnoOrdprcPtnCode, _T("00"), sizeof(mid.zFnoOrdprcPtnCode));

	CString strData = strLedger + CString((char*)&mid, L_max_mid);
	SendTR(_T("PIBOPBXQ"), US_XRTM, strData, KEY_CANALLS);

	mid.zBnsTp[0] = _T('2');
	strData = strLedger + CString((char*)&mid, L_max_mid);
	SendTR(_T("PIBOPBXQ"), US_XRTM, strData, KEY_CANALLB);

/*
//	CString strData;
//	jmdg.Remove('.');

	// 주문가능수량 조회
//	ledger = m_ledger.LedgerTR("1141", "1", "", "SC270419_Q");	
	strData = m_ledger.LedgerTR("3002", m_acc.Left(3), "", "SOFSQ220");	
	m_lenLedger = strData.GetLength();

	struct _maxjmct_mid mid;
	FillMemory(&mid, L_maxjmct_mid, ' ');

	CString strVal(_T(""));

	CopyMemory(&mid.in, _T("00001"), sizeof(mid.in));
	CopyMemory(&mid.zAcntNo, (LPCTSTR)m_acc, m_acc.GetLength());
	CopyMemory(&mid.zPwd, (LPCTSTR)m_pswd, m_pswd.GetLength());

	strData += CString((char *)&mid, L_maxjmct_mid);

	SendTR("pibopbxq", US_XRTM, strData, KEY_CANALL);
*/
/*
	CString passX;
	passX.Format("%s\t%s", m_pswd, m_acc.Left(8));
	passX = (char*)m_pParent->SendMessage(WM_USER, MAKEWPARAM(encPASSx, modeACN), (long)passX.operator LPCTSTR());
	mid.Format("%s\t%s\t%s\t%s\t%s\t", m_acc.Left(8), passX, m_code, "1", jmdg);
*/
//	SendTR("pibopbxq", US_XRTM, ledger+mid, KEY_CANALL);
	/*
	// 청산가능수량 조회
	ledger = m_ledger.LedgerTR("1141", "1", "", "SC270011_Q");	

#ifdef	ENCTEST
	CString passX;
	passX.Format("%s\t%s", m_pswd, m_acc.Left(8));
	passX = (char*)m_pParent->SendMessage(WM_USER, MAKEWPARAM(encPASSx, modeACN), (long)passX.operator LPCTSTR());
	mid.Format("%s\t%s\t%s\t%s\t", m_acc.Left(8), "21", passX, m_code);

#else
	mid.Format("%s\t%s\t%s\t%s\t", m_acc.Left(8), "21", m_pswd, m_code);

#endif
	SendTR("PIBOTUXQ", US_XRTM, ledger+mid, KEY_COVERCAN);

	// 주문가능수량 조회
	m_lenLedger = ledger.GetLength();
//#if 0
//	ledger = m_ledger.LedgerTR("1141", "1", "", "SC270001_Q");	// 주문가능수량 조회
//	mid.Format("%s\t%s\t%s\t%s\t%s\t%s\t%s\t", m_acc.Left(8), "21", m_pswd,
//		m_code, sMmgb, "1", jmdg);
//	SendTR("PIBOTUXQ", US_XRTM, ledger+mid, KEY_JUMUNCAN);
//#else
	ledger = m_ledger.LedgerTR("1141", "1", "");

	struct _fcan_mid fmid;
	FillMemory(&fmid, sz_FCAN_MID, ' ');
	CopyMemory(fmid.accn, m_acc, sizeof(fmid.accn));

#ifdef	ENCTEST
	CopyMemory(fmid.pswd, passX, passX.GetLength());
#else
	CopyMemory(fmid.pswd, m_pswd, m_pswd.GetLength());
#endif
	
	CopyMemory(fmid.cod2, m_code, m_code.GetLength());
	fmid.gubn[0] = '0';
	fmid.jmyh[0] = '1';
	CopyMemory(fmid.oprc, jmdg, jmdg.GetLength());
	fmid.msgb[0] = '1';	// 매도
	CString cmid = CString((char *)&fmid, sz_FCAN_MID);
	
	SendTR("PIBOFCAN", 0, ledger+cmid, KEY_JUMUNMDCAN);	

	fmid.msgb[0] = '2';	// 매수
	cmid = CString((char *)&fmid, sz_FCAN_MID);
	SendTR("PIBOFCAN", 0, ledger+cmid, KEY_JUMUNMSCAN);	
//#endif
*/
}

void CMapWnd::SendMicheg(CString strLedger)
{
	if (m_acc.IsEmpty())
	{
		SetGuide("계좌번호를 입력하세요.");
		return;
	}

	m_pPass->GetWindowText(m_pswd);
	if (m_pswd.IsEmpty())
	{
		SetGuide("비밀번호를 입력하세요.");
		return;
	}

	
	struct _ledgerH ledger;

	if (strLedger.IsEmpty())
	{
		m_bMicheContinue = false;
		CopyMemory(&ledger, m_ledger.LedgerTR("1141", m_acc.Left(3), ""), L_ledgerH);
	}
	else
	{
		m_bMicheContinue = true;
		CopyMemory(&ledger, (LPCTSTR)strLedger, L_ledgerH);
		CString strNext = CString((char*)&ledger.nkey, sizeof(ledger.nkey));
		CopyMemory(&ledger, m_ledger.LedgerTR("1141", m_acc.Left(3), ""), L_ledgerH);

		CopyMemory(&ledger.nkey, (LPCTSTR)strNext, sizeof(ledger.nkey));
		ledger.fkey[0] = '7';
	}

	CString strData = CString((char*)&ledger, L_ledgerH);
	m_lenLedger = strData.GetLength();

	struct _cmid mid;
	FillMemory(&mid, sz_CMID, ' ');
	CopyMemory(mid.accn, m_acc, sizeof(mid.accn));

#ifdef	ENCTEST
	CString passX;
	passX.Format("%s\t%s", m_pswd, m_acc.Left(8));
	passX = (char*)m_pParent->SendMessage(WM_USER, MAKEWPARAM(encPASSx, modeACN), (long)passX.operator LPCTSTR());
	CopyMemory(mid.pswd, passX, passX.GetLength());
#else
	CopyMemory(mid.pswd, m_pswd, m_pswd.GetLength());
#endif

	
	mid.gubn[0] = '2';
	mid.sort[0] = '1';

	// 전부 요청하는 것으로 변경 2009.08.19
	// CopyMemory(mid.cod2, m_code, m_code.GetLength());

	CString cmid((char *)&mid, sz_CMID);
	
	strData += cmid;

	if(m_bCanSend)
		SendTR("PIBOFCHG", US_ENC, strData, KEY_MICHEG);

	m_nFchgCount++;

	if(m_bSendFchgTimer == FALSE)
	{
		m_bSendFchgTimer = TRUE;

		SetTimer(TT_SENDFCHG,1000,FALSE);
	}
}

void CMapWnd::SendJangoData()
{
}

void CMapWnd::AttatchMap()
{
	m_mapKey = m_pParent->SendMessage(WM_USER, MAKEWPARAM(attachMAP, NULL), (LPARAM)m_pMapWnd.get());
}

void CMapWnd::DetachMap()
{
	if (m_mapKey > -1)
	{
		m_pParent->SendMessage(WM_USER, MAKEWPARAM(detachMAP, m_mapKey), NULL);
		m_mapKey = -1;
	}
}

void CMapWnd::LoadMap(CString param)
{
	m_pParent->SendMessage(WM_USER, MAKEWPARAM(loadMAP, m_mapKey), (LPARAM)param.operator LPCTSTR());
}

void CMapWnd::CreateMap(int iLeft, int iTop, int iRight, int iBottom)
{
	if (m_pMapWnd == NULL)
	{
		m_pMapWnd = std::make_unique<CWnd>();
		m_pMapWnd->Create(NULL, NULL, WS_CHILD|WS_VISIBLE, CRect(iLeft, iTop, iRight, iBottom), this, ID_MAP_WND);
	}
}

void CMapWnd::DeleteMap()
{
	if (m_pMapWnd)
	{
		m_pMapWnd->DestroyWindow();
		m_pMapWnd = nullptr;
	}
}

CString CMapWnd::GetPswd()
{
	m_pPass->GetWindowText(m_pswd);
	return m_pswd;
}

void CMapWnd::OnKillFocus(CWnd* pNewWnd) 
{
	CWnd::OnKillFocus(pNewWnd);
	
	
}

void CMapWnd::setTabFocus()
{
	if (m_acc.IsEmpty())
	{
		m_pAccount->SetFocus();
		return;
	}

	if (m_pswd.IsEmpty())
	{
		m_pPass->SetFocus();
		return;
	}

	if (m_code.IsEmpty())
	{
		m_pCodeCtrl->SetFocus();
		return;
	}

	if (m_pHogaWnd)
		m_pHogaWnd->SetFocus();
}

void CMapWnd::SetGuide(CString guide)
{
	KillTimer(TM_GUIDE);
	if( guide.Find("그룹계좌") > -1 && guide.Find("존재") > -1 )
		return;

	m_guide = guide;

	InvalidateRect(&m_rcGuide);
	SetTimer(TM_GUIDE, 3000, NULL);
}

void CMapWnd::OnTimer(UINT nIDEvent) 
{
	if (nIDEvent == TT_CHKPWD)
	{
		KillTimer(TT_CHKPWD);
		transaction();
	}	
	else if (nIDEvent == TM_GUIDE)
	{
		KillTimer(TM_GUIDE);
		
		m_guide = "";
		InvalidateRect(&m_rcGuide);
	}
	else if (nIDEvent == TM_VSIZE)
	{
		KillTimer(TM_VSIZE);
		CString sval = ((CIB300200App*)AfxGetApp())->GetBuffer(_T("VTYPE"));
		if(!sval.IsEmpty())
		{
			if(sval == "1")
			{
				m_depth = hgDEPTH10;
				OnButtonVSize();
			}
		}

	}
	else if (nIDEvent == TM_FIRSTQ)
	{
		KillTimer(TM_FIRSTQ);
		CString sval = ((CIB300200App*)AfxGetApp())->GetBuffer(_T("PASS")); 
		if(!sval.IsEmpty()) 
		{
			m_pswd = sval;
			m_pPass->SetWindowText(sval);
			transaction();
		}
	}
	else if(nIDEvent == TT_SENDFCHG)
	{
		KillTimer(TT_SENDFCHG);

		if(m_nFchgCount == 100)
		{
			m_bCanSend = FALSE;
		}

		m_nFchgCount = 0;

		m_bSendFchgTimer = FALSE;
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

void CMapWnd::DrawColorBox(CDC* pDC, CRect rc, BOOL l, BOOL t, BOOL r, BOOL b)
{
	CPen *pHorz = GetAxPen(GetIndexColor(COLOR_LINE), 1, PS_SOLID);
	CPen *pVert = GetAxPen(GetIndexColor(COLOR_LINE), 1, PS_SOLID);

	CPen *pOldPen = pDC->SelectObject(pHorz);
	if (t)
	{
		pDC->MoveTo(rc.left, rc.top);	
		pDC->LineTo(rc.right - 1, rc.top);
	}
	if (b)
	{
		pDC->MoveTo(rc.left, rc.bottom - 1);
		pDC->LineTo(rc.right - 1, rc.bottom - 1);
	}

	pDC->SelectObject(pVert);
	if (l)
	{
		pDC->MoveTo(rc.left, rc.top);	
		pDC->LineTo(rc.left, rc.bottom - 1);
	}
	if (r)
	{
		pDC->MoveTo(rc.right - 1, rc.top);
		pDC->LineTo(rc.right - 1, rc.bottom);
	}

	pDC->SelectObject(pOldPen);
}

void CMapWnd::OnRButtonUp(UINT nFlags, CPoint point) 
{
	CPoint pt = point;
	ClientToScreen(&pt);
	if (m_code.IsEmpty())
		m_mouseDLL.Format("IB300200\n%d\t%d\n", pt.x, pt.y);
	else
		m_mouseDLL.Format("IB300200\n%d\t%d\ned_focod\t%s\n", pt.x, pt.y, m_code);
	m_pParent->PostMessage(WM_USER, MAKEWPARAM(mouseDLL, FALSE), (long)m_mouseDLL.operator LPCTSTR());
	
	CWnd::OnRButtonUp(nFlags, point);
}


void CMapWnd::alert(char* buf)
{
	CString tmpS = CString(buf);
	const int len = strlen(buf);
	CString codeS = Parser(tmpS, "\t");

	if (m_code != codeS)
		return;

	gsl::owner<char*> pBytes = new char[len + 1]{};
	memset(pBytes, 0x00, len+1);
	memcpy(pBytes, buf, len);

	m_Section.Lock();
	m_arBytes.Add(pBytes);
	m_Section.Unlock();
	SetEvent(m_hEvent);
}

UINT ParsingRTM(LPVOID lpvoid)
{
	CMapWnd* pMap = (CMapWnd *)lpvoid;
	if (pMap == NULL)
		return FALSE;

	gsl::owner<char*> pBytes = NULL;
	while (pMap->m_bThread)
	{
		if (pMap->m_arBytes.GetUpperBound() < 0)
		{
			pMap->WaitRTM();
			continue;
		}
		pMap->m_Section.Lock();
		pBytes = pMap->m_arBytes.GetAt(0);
		pMap->m_arBytes.RemoveAt(0);
		pMap->m_Section.Unlock();
		pMap->DoParse(pBytes);
		[[gsl::suppress(26409)]]
		delete[] pBytes;
		pBytes = NULL;
		
	}
	return 0;
}

void CMapWnd::initRTM()
{
	if (m_bThread)
		return;

	m_bThread = true;
	m_pThread = AfxBeginThread((AFX_THREADPROC) ParsingRTM, (LPVOID) this, THREAD_PRIORITY_NORMAL);
	m_hEvent = ::CreateEvent(NULL, FALSE, FALSE, NULL);	
}

void CMapWnd::stopRTM()
{
	if (!m_bThread)
		return;
	
	m_bThread = false;
	SetEvent(m_hEvent);
	m_pThread->PostThreadMessage(WM_QUIT, 0, 0);
	switch (WaitForSingleObject(m_pThread->m_hThread, 1000))
	{
	case WAIT_OBJECT_0:
	case WAIT_FAILED:
	case WAIT_ABANDONED:
		break;
	case WAIT_TIMEOUT:
		TerminateThread(m_pThread->m_hThread, 0);
		break;
	}
	removeRTM();
	
}

bool CMapWnd::WaitRTM()
{
	if (!m_pThread)
		return false;
	const DWORD res = WaitForSingleObject(m_hEvent, 3000);
	switch (res)
	{
	case WAIT_OBJECT_0:
		return true;
	case WAIT_FAILED:
	case WAIT_ABANDONED:
	case WAIT_TIMEOUT:
		return false;
	}
	return true;
}

void CMapWnd::removeRTM()
{
	m_Section.Lock();
	gsl::owner<char*> pBytes = NULL;
	for (int ii = m_arBytes.GetUpperBound(); ii >= 0; ii--)
	{
		pBytes = m_arBytes.GetAt(ii);
		[[gsl::suppress(26409)]]
		delete[] pBytes;
		pBytes = NULL;
	}
	m_arBytes.RemoveAll();
	m_Section.Unlock();
}

void CMapWnd::DoParse(CString dataS)
{
}

void CMapWnd::DoParsex(struct _alertR * alertR)
{
	dispatchAlertX(alertR);
	m_pHogaWnd->DispatchAlertX(alertR);
}

void CMapWnd::SetHold(bool bHold)
{
	//TRACE("bHold[%d]\n", bHold ? 1 : 0);
	if (m_pHogaWnd)
	{
		m_pHogaWnd->SetHold(bHold);
	}

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

BOOL CMapWnd::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message) 
{
	if (m_bWait)
	{
		SetCursor(LoadCursor(NULL, IDC_WAIT));
		return TRUE;
	}
	
	return CWnd::OnSetCursor(pWnd, nHitTest, message);
}

struct _ledgerH* CMapWnd::GetLedger(struct _ledgerH* ledger)
{
	m_pParent->SendMessage(WM_USER, ledgerDLL, (LPARAM)ledger);

	LPCSTR usid = Variant(userCC, (LPCSTR)NULL);
	CopyMemory(ledger->usid, usid, strlen(usid));
	CopyMemory(ledger->fkey, "C", 1);
	CopyMemory(ledger->mkty, "1", 1);

	return ledger;
}

/*
bool CMapWnd::CheckPswd(CString acno, CString pswd)
{
	vector<char> buff(L_ledgerH + sizeof(SACMT238_mid), ' ');
	struct _ledgerH* ledger = (struct _ledgerH*)&buff[0];
	SACMT238_mid* mid = (SACMT238_mid*)&buff[L_ledgerH];

	GetLedger(ledger);
	CopyMemory(ledger->svcd, "SACMT238", 8);
	CopyMemory(ledger->brno, "000", 3);
	CopyMemory(ledger->odrf, "1", 1);
	CopyMemory(ledger->mkty, "1", 1);

	//2013.12.23 KSJ 일방향암호화 추가
	if (strlen(pswd) > 0)
	{
		CString sPswd;
		sPswd = GetEncPassword(pswd.GetString());
		CopyMemory(ledger->hsiz, "44", sizeof(ledger->hsiz));
		CopyMemory(ledger->epwd, sPswd, sPswd.GetLength());
	}

	CopyMemory(mid->In, "00001", 5);
	CopyMemory(mid->zAcntNo, acno, strlen(acno.GetString()));

	// 2011.07.06 로그인계좌소유주 점검 결과 개별계좌 송신으로 변경
	//mid->zAcntNo[3] = mid->zAcntNo[4] = '0';
	//CopyMemory(mid->zPwd, pswd, strlen(pswd));
	CopyMemory(mid->zPwd, "HEAD", 4);	//2013.12.23 KSJ 일방향암호화 추가

	SendTR("piboPBxQ", US_ENC, &buff[0], KEY_CHKPASS);

	return false;
}
*/

bool CMapWnd::CheckPswd(CString strAccount, CString strPassword)
{
	if (strAccount.IsEmpty()) return false;
	if (strPassword.IsEmpty()) {
		SetGuide("비밀번호를 확인하십시오.");
		return false;
	}
	if (bPasswordChecked) return true;

	if (bChecking) return false;
	bChecking = true;
	//strAccount.SetAt(3, '0');
	//strAccount.SetAt(4, '0');

	CString usid = (char*)m_pParent->SendMessage(WM_USER, MAKEWPARAM(variantDLL, userCC), NULL);
#ifdef USE_SACMT207
	struct	_ledgerH ledger;
	memset(&ledger, ' ', L_ledgerH);
	m_pParent->SendMessage(WM_USER, ledgerDLL, (LPARAM)&ledger);
	memcpy(ledger.svcd, "SACMT207", 8);
	memcpy(ledger.usid, usid, usid.GetLength());
	memcpy(ledger.brno, "000", 3);
	memcpy(ledger.rcnt, "0000", 4);
	ledger.fkey[0] = 'C';
	ledger.mkty[0] = '1';
	ledger.odrf[0] = '1';

	SACMT207_mid mid;
	memset(&mid, ' ', sizeof(mid));
	memcpy(mid.in, "00001", 5);
	memcpy(mid.acctNo, strAccount, strAccount.GetLength());
	memcpy(mid.password, strPassword, strPassword.GetLength());
	memcpy(mid.zTrxTp, "1", 1);

	CString data;
	data += CString((char*)&ledger, sizeof(ledger));
	data += CString((char*)&mid, sizeof(mid));

	SendTR("piboPBxQ", 0, data, KEY_CHKPASS);
#else
	struct	_ledgerH ledger;
	memset(&ledger, ' ', L_ledgerH);
	m_pParent->SendMessage(WM_USER, ledgerDLL, (LPARAM)&ledger);
	memcpy(ledger.svcd, "SACMT238", 8);
	memcpy(ledger.usid, usid, usid.GetLength());
	memcpy(ledger.brno, "000", 3);
	memcpy(ledger.rcnt, "0000", 4);

	//2013.12.23 KSJ 일방향암호화 추가
	CString sPswd;
	sPswd = GetEncPassword(strPassword);
	CopyMemory(&ledger.hsiz, "44", sizeof(ledger.hsiz));
	CopyMemory(&ledger.epwd, sPswd, sPswd.GetLength());

	ledger.fkey[0] = 'C';
	ledger.mkty[0] = '1';
	ledger.odrf[0] = '1';
	
	SACMT238_mid mid;
	memset(&mid, ' ', sizeof(mid));
	memcpy(mid.In, "00001", 5);
	memcpy(mid.zAcntNo, strAccount, strAccount.GetLength());
//	memcpy(mid.zPwd, strPassword, strPassword.GetLength());
	memcpy(mid.zPwd, "HEAD", 4);	//2013.12.23 KSJ 일방향암호화 추가
	
	CString data;
	data += CString((char*)&ledger, sizeof(ledger));
	data += CString((char*)&mid, sizeof(mid));
	
	const int len = sizeof(ledger) + sizeof(mid);
//	SendTR("piboPBxQ", US_ENC, data, KEY_CHKPASS);
	SendTR2("piboPBxQ", US_ENC, data.GetString(), len, KEY_CHKPASS);
#endif
	return false;
}


void CMapWnd::ParsingCheckPswd(CString strData)
{
	if (strData.IsEmpty())
		return;
	bChecking = false;

	struct _ledgerH *ledger = (struct _ledgerH *)(LPCSTR)strData;
	CString strErrCode(ledger->ecod, 4);
	CString strErrText(ledger->emsg + 4, sizeof(ledger->emsg) - 4);
	strErrText.TrimRight(" "); 
#ifdef USE_SACMT207
	if(strErrCode.Left(1) != "0")
	{
		SetGuide(strErrText);
		MessageBox(strErrCode + ":" + strErrText, "IBK투자증권", MB_OK|MB_ICONERROR);
		m_pPass->SetWindowText("");
		m_pPass->SetFocus();
		return;
	}
#else
	struct SACMT238_mod *mod = (struct SACMT238_mod*)(((LPCSTR)strData)+L_ledgerH);
	if (mod->zAvalYn[0]!='Y')
	{
		SetGuide(strErrText);
		MessageBox(strErrCode + ":" + strErrText, "IBK투자증권", MB_OK|MB_ICONERROR);
		m_pPass->SetWindowText("");
		m_pPass->SetFocus();
		return;
	}
#endif
	bPasswordChecked = true;
	transaction();
	m_pPass->EnableWindow(FALSE);
}

LRESULT CMapWnd::OnMyPassword(WPARAM wParam, LPARAM lParam)
{
	//CString rcb = (char*)lParam;
	//MessageBox((char*)lParam);
	m_pPass->SetWindowText((char*)lParam);
	//MessageBox("Received: "+rcb);
	return 1;
}

bool CMapWnd::IsNumber(CString str)
{
    bool bRet = TRUE;
	const int nLen = str.GetLength();
    for( int i=0 ; i < nLen ; ++i)
    {
        if( isdigit(str.GetAt(i)) == FALSE)
        {
            bRet = FALSE;
            break;
        }
    }
    return bRet;
}

void CMapWnd::ChangeFont(int ifontsixze, LPCTSTR lszFont)
{
	
	if(!m_bInit) return;
//	if(m_iCurFont == ifontsixze) return;

	m_pFont = GetAxFont(lszFont, ifontsixze, false, 0);;
	m_pBFont = GetAxFont(lszFont, ifontsixze, false, FW_BOLD);
//	m_pfontB = m_pTool->RequestFont(sFont, ifontsixze, false, FW_BOLD);
//	if(!m_pfont) return;

	const double ldif = float(ifontsixze - 9);

	m_iWndWidth = MAP_WIDTH + (int)(m_lSizeRate[0] * (float)MAP_WIDTH * ldif);
	m_iWndHeight = MAP_HEIGHT +(int) (m_lSizeRate[1] * (float)MAP_HEIGHT * ldif);
	m_iCurFont = ifontsixze;

	SetWindowPos(NULL, 0, 0, m_iWndWidth, m_iWndHeight, SWP_NOZORDER | SWP_NOMOVE);
	m_pParent->SendMessage(WM_USER, MAKEWPARAM(sizeDLL, 0), MAKELPARAM(m_iWndWidth, m_iWndHeight) );
}

void CMapWnd::OnSize(UINT nType, int cx, int cy) 
{
	CWnd::OnSize(nType, cx, cy);
	// TODO: Add your message handler code here
	if(m_bhogaResizeClick) 
	{
		m_bhogaResizeClick = FALSE;
		return;
	}

	if(m_iOldSize[0] == 0 && m_iOldSize[1] == 0)
	{
		m_iOldSize[0] = cx ;
		m_iOldSize[1] = cy ;
		return;
	}

	ResizeControl(cx, cy);
	SetTimer(TM_VSIZE, 10, NULL);

}

void CMapWnd::ResizeControl(int cx, int cy)
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
	int itop1 = itop = wrc.top;
	wrc.right = wrc.left + iw;
	wrc.bottom = wrc.top + ih;
	m_pAccount->MoveWindow(wrc); 
	m_pAccount->ChangeFont(m_iCurFont, _T("굴림체") ); 
	

	//계좌명
	int ileft = wrc.right + 1;
	iw = m_oRc1.Width();
	iw = iw + (int)(lcalcrate[0] * (float)iw);
	m_oRc1.SetRect(ileft, itop, ileft + iw, itop+ih);
	ileft = ileft + iw + GAP;

	//패스워드
	m_pPass->GetWindowRect( &wrc);
	ScreenToClient(&wrc);
	iw = wrc.Width() , ih = wrc.Height() ;

	iw = iw + (int)(lcalcrate[0] * (float)iw);
	ih = ih + (int)(lcalcrate[1] * (float)ih);
	wrc.SetRect(ileft, itop, ileft + iw, itop+ih);
	m_pPass->SetFont(m_pFont);
	m_pPass->MoveWindow(wrc); 


	ileft = ileft + iw + GAP;
	m_pBtnSend->GetWindowRect( &wrc);
	ScreenToClient(&wrc);
	iw = wrc.Width();
	iw = iw + (int)(lcalcrate[0] * (float)iw);
	wrc.SetRect(ileft, itop, ileft + iw, itop+ih);
	m_pBtnSend->SetFont(m_pFont);
	m_pBtnSend->MoveWindow(wrc); 

	ileft = ileft + iw + BUTTON_SPACE;
	m_pBtnReserve->GetWindowRect( &wrc);
	ScreenToClient(&wrc);
	iw = wrc.Width();
	iw = iw + (int)(lcalcrate[0] * (float)iw);
	wrc.SetRect(ileft, itop, ileft + iw, itop+ih);
	m_pBtnReserve->SetFont(m_pFont);
	m_pBtnReserve->MoveWindow(wrc); 

	ileft = ileft + iw + BUTTON_SPACE;
	m_pBtnConfig->GetWindowRect( &wrc);
	ScreenToClient(&wrc);
	iw = wrc.Width();
	iw = iw + (int)(lcalcrate[0] * (float)iw);
	wrc.SetRect(ileft, itop, ileft + iw, itop + ih);
	m_pBtnConfig->SetFont(m_pFont);
	m_pBtnConfig->MoveWindow(wrc); 


	ileft = ileft + iw + BUTTON_SPACE;
	m_pBtnVSize->GetWindowRect( &wrc);
	ScreenToClient(&wrc);
	iw = wrc.Width();
	iw = iw + (int)(lcalcrate[0] * (float)iw);
	wrc.SetRect(ileft, itop, ileft + iw, itop + ih);
	m_pBtnVSize->SetFont(m_pFont);
	m_pBtnVSize->MoveWindow(wrc);
	
///////////////////////////2 LINE
	itop += (ih + GAP);
	m_pCodeCtrl->GetWindowRect( &wrc);
	ScreenToClient(&wrc);
	ileft = wrc.left; 
	iw = wrc.Width();
	ih = wrc.Height(); 
	iw = iw + (int)(lcalcrate[0] * (float)iw);
	ih = ih + (int)(lcalcrate[1] * (float)ih);
	wrc.SetRect(ileft, itop, ileft + iw, itop + ih);
	m_pCodeCtrl->SetFont(m_pFont);
	m_pCodeCtrl->MoveWindow(wrc); 

	ileft = wrc.right + 1;
	m_pBtnCode->GetWindowRect( &wrc);
	ScreenToClient(&wrc);
	iw = wrc.Width();
	iw = iw + (int)(lcalcrate[0] * (float)iw);
	wrc.SetRect(ileft, itop, ileft + iw, itop + ih);
	m_pBtnCode->SetFont(m_pFont);
	m_pBtnCode->MoveWindow(wrc); 

	//OptionEx
	m_pOptionEx->GetWindowRect( &wrc);
	ScreenToClient(&wrc);
	iw = wrc.Width() , ih = wrc.Height() ;
	iw = iw + (int)(lcalcrate[0] * (float)iw );
	ih = ih + (int)(lcalcrate[1] * (float)ih);
	
	//OptionEx
	itop1 = itop = wrc.top;
	wrc.right = wrc.left + iw;
	wrc.bottom = wrc.top + ih;
	m_pOptionEx->MoveWindow(wrc); 
	m_pOptionEx->ChangeFont(m_iCurFont, _T("굴림체") ); 


	//현재가~
	ileft = ileft + iw + GAP;
	iw = m_tRc2.Width();
	iw = iw + (int)(lcalcrate[0] * (float)iw);
	m_tRc2.SetRect(ileft, itop, ileft + iw, itop + ih);

	ileft += iw;
	iw = m_tRc3.Width();
	iw = iw + (int)(lcalcrate[0] * (float)iw);
	m_tRc3.SetRect(ileft, itop, ileft + iw, itop + ih);

	ileft += iw;
	iw = m_tRc4.Width();
	iw = iw + (int)(lcalcrate[0] * (float)iw);
	m_tRc4.SetRect(ileft, itop, ileft + iw, itop + ih);

	ileft += iw;
	iw = m_tRc5.Width();
	iw = iw + (int)(lcalcrate[0] * (float)iw);
	m_tRc5.SetRect(ileft, itop, ileft + iw, itop + ih);

/////////////////////////3 LINE
	itop += (ih);
	iw = m_oRc6.Width();
	ileft = m_oRc6.left;
	iw = iw + (int)(lcalcrate[0] * (float)iw);
	m_oRc6.SetRect(ileft, itop, ileft + iw, itop + ih);

	ileft += iw;
	m_oRc2 = m_tRc2;
	m_oRc2.top = itop;
	m_oRc2.bottom = itop + ih;

	m_oRc3 = m_tRc3;
	m_oRc3.top = itop;
	m_oRc3.bottom = itop + ih;

	m_oRc4 = m_tRc4;
	m_oRc4.top = itop;
	m_oRc4.bottom = itop + ih;

	m_oRc5 = m_tRc5;
	m_oRc5.top = itop;
	m_oRc5.bottom = itop + ih;

	itop += (ih + GAP);
	m_pHogaWnd->GetWindowRect( &wrc);
	ScreenToClient(&wrc);
	ileft = wrc.left;
	iw = wrc.Width();
	ih = wrc.Height();
	
	iw = iw + (int)(lcalcrate[0] * (float)iw);
	ih = ih + (int)(lcalcrate[1] * (float)ih);

	itop = m_oRc5.bottom + INNER_FRAME_SPACE + OUTER_FRAME_SPACE+INNER_FRAME_SPACE;
	int iguideh = m_rcGuide.Height();
	iguideh = iguideh + (int)(lcalcrate[1] * (float)iguideh);
	const int ibottom = cy - iguideh - OUTER_FRAME_SPACE - INNER_FRAME_SPACE-INNER_FRAME_SPACE;


//	wrc.SetRect(ileft, itop, ileft + iw, itop + ih);
	if(m_iCurFont == 9) iw = HOGA_WIDTH;
	wrc.SetRect(ileft, itop, ileft + iw, ibottom);
	m_pHogaWnd->SetFont(m_pFont);
	m_pHogaWnd->MoveWindow(wrc);
	m_pHogaWnd->ChangeFont(m_pFont, m_pBFont);
	

	const int ileft1 = wrc.left;
	const int ibottom1 = wrc.bottom;

	ileft = wrc.right + 1;

	m_rcGuide.SetRect(FRAME_SPACE, ibottom1+INNER_FRAME_SPACE+INNER_FRAME_SPACE, cx-FRAME_SPACE, cy-INNER_FRAME_SPACE);
	m_iContentHeight = m_iWndHeight - m_rcGuide.Height();


	ileft = wrc.right + HORI_GAP+HORI_GAP+HORI_GAP;
	const int ixgap = INNER_FRAME_SPACE+1;
	const int iygap = VERT_GAP+VERT_GAP;
	wrc.SetRect(ileft, VERT_GAP+VERT_GAP, cx-ixgap, m_rcGuide.top-iygap);
	
	m_pCtrlWnd->MoveWindow(wrc); 
	m_pCtrlWnd->ChangeFont(lcalcrate, m_pFont, m_pBFont);

}

bool CMapWnd::GetMarketEnable()
{
	return m_pCtrlWnd->GetMarket();
}

CString CMapWnd::GetEncPassword(CString sPswd)
{
	CString dllPath;
	dllPath.Format(_T("%s%s"), m_home, _T("\\dev\\CX_SHA256.DLL"));
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
