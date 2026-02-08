// MapWnd.cpp : implementation file
//

#include "stdafx.h"
#include <math.h>
#include "IB401500.h"
#include "MapWnd.h"
#include "Cfg.h"
#include "OpSensCal.h"
#include "../../h/memdc.h"
#include "../../h/axisfire.h"
#include "../../h/axisvar.h"
#include "../../control/fx_misc/misctype.h"
#include "../../control/fx_misc/fxImgButton.h"
#include "../../control/fx_grid/fxgrid.h"
#include "../../control/fx_grid/griditem.h"

#include "../../control/fx_misc/fxTab.h"
#include "../../control/fx_misc/fxCodeCtrl.h"

#define SETUP_FILE	"axisensetup.ini"
#define INI_FILE	"userconf.ini"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define	clBackIn	66
#define	clBack		64

const COLORREF	clrRED	= PALETTERGB(255, 0, 0);
const COLORREF	clrBLUE	= PALETTERGB(0, 0, 255);
const COLORREF	clrBLACK= PALETTERGB(0, 0, 0);

#define KEY_DATA	1

#define THREAD_TIMER_KEY	1000
#define THREAD_INTERVAL		1000

const int gap1		= 1;
const int gap2		= 3;
const int comboH	= 20;
const int comboW	= 60;
const int staticW	= 60;
const int static2W	= 65;
const int btn2W		= 36;
const int btn3W		= 48;
const int borderH	= 50;
const int gridH		= 121;
const int gridTitleW	= 20;
const int gridHeaderH	= 20;
const int rowH		= 20;

#define	mapWidth	630
#define	mapHeight	312
#define VERT_GAP		3
#define HORI_GAP	3
#define BOT_GAP		6

#define BOT_GAP		6
#define FRAME_SPACE		1

#define TOPANDBODY_SPACE	5

const int gap3		= VERT_GAP+VERT_GAP;


struct	_staticX {
	int	sx;
	int	sy;
	int	width;
	int	order;
	int	align;
	DWORD	attr;
	COLORREF	clrBk;  // Color Index로 변경필요
	COLORREF	clrBox; // Color Index로 변경필요
	char*	title;
	int	dataIdx;
	//CfxStatic*	statW;
} staticX[CNT_STATIC] = {
	{ gap2*1+staticW*0, gap3, staticW,   -1, alCENTER,	0, 			0x00F6EAE2, 0x00BBCBBC,	"기초자산",	-1,		}, 
	{ gap2*1+staticW*1, gap3, staticW,   -1, alRIGHT,	0,			0x00FFFFFF, 0x00BBCBBC,	"",		datBasicPrice	},
	{ gap2*1+staticW*2, gap3, staticW*2, -1, alCENTER,	0,			0x00FFFFFF, 0x00BBCBBC,	"",		datBasicRate	},
	{ gap2*2+staticW*4, gap3, staticW,   -1, alCENTER,	0,			0x00F6EAE2, 0x00BBCBBC,	"잔존일수",	-1		}, 
	{ gap2*2+staticW*5, gap3, staticW,   -1, alRIGHT,	0,			0x00FFFFFF, 0x00BBCBBC,	"",		datJjil		},

	{ gap2*1+static2W*0, gap2*2+((comboH+VERT_GAP)*1), staticW, -1, alCENTER,	0,			0x00F5F2EE, 0x00BBCBBC,	"F",		datFuCode	},
	{ gap2*1+static2W*1, gap2*2+((comboH+VERT_GAP)*1), staticW, -1, alRIGHT,	0,			0x00FFFFFF, 0x00BBCBBC,	"",		datFuCur	},
	{ gap2*1+static2W*2, gap2*2+((comboH+VERT_GAP)*1), staticW, -1, alRIGHT,	attrNUMERIC|attrCONTRAST,0x00FFFFFF, 0x00BBCBBC,	"",	datFuDif	},
	{ gap2*1+static2W*3, gap2*2+((comboH+VERT_GAP)*1), staticW, -1, alRIGHT,	0,			0x00FFFFFF, 0x00BBCBBC,	"",		datFuRate	},
	{ gap2*1+static2W*4, gap2*2+((comboH+VERT_GAP)*1), staticW, -1, alRIGHT,	attrNUMERIC|attrCORGBx,	0x00FFFFFF, 0x00BBCBBC,	"",		datFuSell	},
	{ gap2*1+static2W*5, gap2*2+((comboH+VERT_GAP)*1), staticW, -1, alRIGHT,	attrNUMERIC|attrCORGBx,	0x00FFFFFF, 0x00BBCBBC,	"",		datFuBuy	},
	{ gap2*1+static2W*6, gap2*2+((comboH+VERT_GAP)*1), staticW, -1, alRIGHT,	attrNUMERIC|attrCOMMA,	0x00FFFFFF, 0x00BBCBBC,	"",		datFuGVol	},
	{ gap2*1+static2W*7, gap2*2+((comboH+VERT_GAP)*1), staticW, -1, alRIGHT,	attrNUMERIC|attrCOMMA,	0x00FFFFFF, 0x00BBCBBC,	"",		datFuMigeul	},
	{ gap2*1+static2W*8, gap2*2+((comboH+VERT_GAP)*1), staticW, -1, alRIGHT,	attrNUMERIC|attrCORGB|attrCOMMA,	0x00FFFFFF, 0x00BBCBBC,	"",	datFuMDaebi	} 
};

struct	_gridHdr {
	char*	text;		// fixed header
	UINT	width;		// fixed width
	UINT	attr;		// fixde attr

	char	symbol[8];	// nonfixed symbol
	UINT	format;		// nonfixed format
	UINT	attrx;		// nonfixed attr

} gridHdr[] = {
	{ "행사가",	50,	0,	"",		GVFM_CENTER,	GVAT_ONLYROW},			// 0	gfPrice
	{ "지수환산",	60,	0,	"",		GVFM_RIGHT,	GVAT_ONLYROW|GVAT_COMMA},	// 1	gfJisuConv
	{ "현재가",	55,	0,	"023",		GVFM_RIGHT,	GVAT_ONLYROW|GVAT_CONDITIONx},	// 2	gfCur
	{ "전일비",	55,	0,	"024",		GVFM_RIGHT,	GVAT_ONLYROW|GVAT_CONTRAST},	// 3	gfDif
	{ "등락율",	58,	0,	"033",		GVFM_RIGHT,	GVAT_ONLYROW|GVAT_CONDITIONx},	// 4	gfRate---278
	{ "매도호가",	55,	0,	"025",		GVFM_RIGHT,	GVAT_ONLYROW|GVAT_CONDITIONx},	// 5	gfMsga
	{ "매수호가",	55,	0,	"026",		GVFM_RIGHT,	GVAT_ONLYROW|GVAT_CONDITIONx},	// 6	gfMdga

	{ "시가",	55,	0,	"029",		GVFM_RIGHT,	GVAT_ONLYROW|GVAT_CONDITIONx},	// 7	gfSiga
	{ "고가",	55,	0,	"030",		GVFM_RIGHT,	GVAT_ONLYROW|GVAT_CONDITIONx},	// 8	gfKoga
	{ "저가",	55,	0,	"031",		GVFM_RIGHT,	GVAT_ONLYROW|GVAT_CONDITIONx},	// 9	gfJega

	{ "거래량",	65,	0,	"027",		GVFM_RIGHT,	GVAT_ONLYROW|GVAT_COMMA},	// 10	gfGvol
	{ "미결제",	56,	0,	"201",		GVFM_RIGHT,	GVAT_ONLYROW|GVAT_COMMA},	// 11	gfMigeul
	{ "증감",	30,	0,	"206",		GVFM_RIGHT,	GVAT_ONLYROW|GVAT_COMMA},	// 12	gfMDif---593

	{ "IV",		55,	0,	"",		GVFM_RIGHT,	GVAT_ONLYROW|GVAT_COMMA},	// 13	gfIV
	{ "델타",	55,	0,	"",		GVFM_RIGHT,	GVAT_ONLYROW|GVAT_COMMA},	// 14	gfDelta
	{ "감마",	55,	0,	"",		GVFM_RIGHT,	GVAT_ONLYROW|GVAT_COMMA},	// 15	gfGamma
	{ "쎄타",	55,	0,	"",		GVFM_RIGHT,	GVAT_ONLYROW|GVAT_COMMA},	// 16	gfTheta
	{ "베가",	55,	0,	"",		GVFM_RIGHT,	GVAT_ONLYROW|GVAT_COMMA},	// 17	gfVegga---593
};

const double g_lrate[] = 
{0.084, 0.101, 0.092,  0.097, 0.092, 0.132, 0.092, 0.080, 0.080, 0.080, 0.10, 0.097, 0.094, 0.070, 0.088, 0.088, 0.088, 0.088};
const int gfPrice = 0, gfJisuConv = 1, gfCur = 2, gfDif = 3, gfRate = 4, gfMdga = 5, gfMsga = 6, gfSiga = 7, gfKoga = 8, gfJega = 9, gfGvol = 10, gfMigeul = 11, gfMDif = 12,
	gfIV = 13, gfDelta = 14, gfGamma = 15, gfTheta = 16, gfVegga = 17, gfEndCnt = 18;	
//2014.02.27 KSJ gfMdga gfMsga 순서바꿈 실제 매도, 매수호가가 반대로 나왔었음.

#define			GRID_TXT_COLOR		69
/////////////////////////////////////////////////////////////////////////////
// CMapWnd

CMapWnd::CMapWnd()
{
	m_bmp2BTN	= NULL;
	m_bmp2BTN_DN	= NULL;
	m_bmp2BTN_EN	= NULL;
	m_bmp3BTN	= NULL;
	m_bmp3BTN_DN	= NULL;
	m_bmp3BTN_EN	= NULL;

	m_pMonthCbo	= NULL;
	m_pGridCbo = NULL;
	m_pGreekBtn	= NULL;
	m_pMingamBtn	= NULL;
	m_pCallTitle	= NULL;
	m_pPutTitle	= NULL;
	m_pCallGrid	= NULL;
	m_pPutGrid	= NULL;
	m_pUpBtn	= NULL;
	m_pDownBtn	= NULL;
	m_pBorder	= NULL;
	m_opsensCal	= NULL;

	m_DataCount	= 0;
	m_pStrike	= NULL;

	//m_pCodeCtrl	= NULL; // test

	m_bShowMingam	= FALSE;
	m_bShowSiKoJe	= FALSE;	//2013.10.16 KSJ 시고저 보기

	m_bResult1 = m_bResult2 = m_bResult3 = false;

	m_bThreadRTM	= false;		// Thread
	m_pThreadRTM	= NULL;
	m_hEventRTM	= NULL;

	m_bRTMReady	= FALSE;
		
	m_iCurFont = 9;
	m_iOldSize[0] = m_iOldSize[1] = 0;
	m_lSizeRate[0] = 0.16;
	m_lSizeRate[1] = 0.07;

	m_bChangeFont = m_bCreated = FALSE;
	m_pContent = NULL;
	m_iScrollw = 14;


	m_Bfont = NULL;
	m_blinkColor = NULL;
	m_bmpDown = NULL;
	m_bmpDown_DN = NULL;
	m_bmpDown_EN = NULL;

	m_bmpL = NULL;
	m_bmpM = NULL;
	m_bmpR = NULL;
	m_bmpSelL = NULL;
	m_bmpSelM = NULL;
	m_bmpSelR = NULL;

	m_bmpUp = NULL;
	m_bmpUp_DN = NULL;
	m_bmpUp_EN = NULL;

	m_clrBack = NULL;
	m_clrBackIn = NULL;
	m_font = NULL;
	m_iGridColwidths = NULL;
	m_pParent = NULL;

// 	initRTM();
}

CMapWnd::~CMapWnd()
{
}

BEGIN_MESSAGE_MAP(CMapWnd, CWnd)
	//{{AFX_MSG_MAP(CMapWnd)
	ON_WM_PAINT()
	ON_CBN_SELCHANGE(ID_MONTHCOMBO, OnSelChangeMonth)
	ON_CBN_SELCHANGE(ID_GRIDCOMBO, OnSelChangeGrid)
	ON_WM_DESTROY()
	ON_WM_RBUTTONDOWN()
	ON_WM_TIMER()
	ON_BN_CLICKED(ID_GREEKBTN, OnClickGreek)
	ON_BN_CLICKED(ID_MINGAMBTN, OnClickMingam)
	ON_BN_CLICKED(ID_UPBTN, OnClickUp)
	ON_BN_CLICKED(ID_DOWNBTN, OnClickDown)
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_USER, OnMessage)
	ON_NOTIFY(GVNM_RMOUSEDOWN, ID_CALLGRID, OnCallGridRMouseDown)
	ON_NOTIFY(GVNM_RMOUSEDOWN, ID_PUTGRID, OnPutGridRMouseDown)
	ON_NOTIFY(GVNM_LMOUSEUP, ID_CALLGRID, OnGridLMouseUp)
	ON_NOTIFY(GVNM_LMOUSEUP, ID_PUTGRID, OnGridLMouseUp)
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CMapWnd message handlers

void CMapWnd::PostNcDestroy() 
{
	//CWnd::PostNcDestroy();
}

CString CMapWnd::Variant(int comm, CString data)
{
	CString retvalue;
	if (comm == guideCC)
	{
		//m_outResult.SetWindowText(data);
		KillTimer(1);
		SetTimer(1, 2000, NULL);
		return "";
	}
	const char* dta = (char*)m_pParent->SendMessage(WM_USER, MAKEWPARAM(variantDLL, comm), (LPARAM)(LPCTSTR)data);

	if ((long)dta > 1)
		retvalue = dta;
	else
		return "";

	return retvalue; 
}

CBitmap* CMapWnd::getBitmap(CString path)
{
	return (CBitmap*)m_pParent->SendMessage(WM_USER, getBITMAP, (long)path.operator LPCTSTR());
}

HBITMAP CMapWnd::GetAxHBitmap(CString stPath)
{
	const CBitmap* pBitmap = getBitmap(stPath);
	if(pBitmap)
		return pBitmap->operator HBITMAP();
	 
	return NULL;
}
COLORREF CMapWnd::GetIndexColor(int index)
{
	if (index & 0x02000000)
		return index;
	return m_pParent->SendMessage(WM_USER, MAKEWORD(getPALETTE, 0), (LPARAM)index);
}

CBrush* CMapWnd::getAxBrush(COLORREF clr)
{
	return (CBrush*)m_pParent->SendMessage(WM_USER, getBRUSH, (long)clr);
}

CFont* CMapWnd::getAxFont(CString fName, int point, int style)
{
	struct _fontR fontR;

	fontR.name = (LPSTR)fName.operator LPCTSTR();
	fontR.point = point; fontR.italic = false; fontR.bold = 0;

	switch (style)
	{
	case 0: case 1: // none
		break;
	case 2: // italic
		fontR.italic = true;
		break;
	case 3: // bold
		fontR.bold = FW_BOLD;
		break;
	case 4: // both
		fontR.italic = true;
		fontR.bold = FW_BOLD;
		break;
	}
	return (CFont*)m_pParent->SendMessage(WM_USER, getFONT, (long)&fontR);
}

void CMapWnd::sendTR(CString trCode, char* datB, int datL, int nKey, int nStat)
{
	std::unique_ptr<char[]> sendB = std::make_unique<char[]>(L_userTH + datL + 1);
	struct	_userTH* uTH{};
	uTH = (struct _userTH *) sendB.get();

	strcpy(uTH->trc, trCode);
	uTH->key = nKey;
	uTH->stat = nStat;

	CopyMemory(&sendB.get()[L_userTH], datB, datL);
	
	m_pParent->SendMessage(WM_USER, MAKEWPARAM(invokeTRx, datL), (LPARAM) sendB.get());
}

void CMapWnd::LoadRoundBitmap()
{
	CString sFile;
	sFile.Format("%s\\tab\\PALETTE.INI", m_sRoot);

	CString sPalette = _T("");
	int readL = 0; 
	char readB[1024]; 
	readL = GetPrivateProfileString(_T("General"), _T("Palette"), _T("Blue"), readB, sizeof(readB), sFile);
	if (readL > 0)	sPalette.Format("%s", readB);

	CString sImage;
	sImage.Format("%s\\Image\\axpanel%s1_",  m_sRoot, sPalette);
	for(int i=0; i<4; i++)
	{
		if(m_hRoundBitmap[i])
			DeleteObject(m_hRoundBitmap[i]);
	}

	CIB401500App* pApp = (CIB401500App*)AfxGetApp(); 
	m_hRoundBitmap[0] = m_hRoundBitmap[4] = (HBITMAP)::LoadImage( pApp->m_hInstance , sImage+"lt.bmp",
											IMAGE_BITMAP,0,0,LR_LOADFROMFILE );
	m_hRoundBitmap[1] = m_hRoundBitmap[5] = (HBITMAP)::LoadImage( pApp->m_hInstance, sImage+"rt.bmp",
											IMAGE_BITMAP,0,0,LR_LOADFROMFILE );
	m_hRoundBitmap[2] = m_hRoundBitmap[6] = (HBITMAP)::LoadImage( pApp->m_hInstance, sImage+"lb.bmp",
											IMAGE_BITMAP,0,0,LR_LOADFROMFILE );
	m_hRoundBitmap[3] = m_hRoundBitmap[7] = (HBITMAP)::LoadImage(pApp->m_hInstance, sImage+"rb.bmp",
											IMAGE_BITMAP,0,0,LR_LOADFROMFILE );

}

void CMapWnd::CreateMap(CWnd* pParent)
{
	m_pParent = pParent;
	Create(NULL, NULL, WS_CHILD | WS_VISIBLE , CRect(0, 0, mapWidth, mapHeight), pParent, 100, NULL);

	Variant(titleCC, "[4015] 옵션 월물별 시세");

	m_sRoot = Variant(homeCC, "");
	m_sUser = Variant(nameCC, "");
	m_font = getAxFont("굴림체", 9);
	m_Bfont = getAxFont("굴림체", 9, 3);

	CString strImgDir = "";
	strImgDir.Format("%s\\Image\\", m_sRoot);
	LoadRoundBitmap();
/*
	m_hRoundBitmap[0] = GetAxHBitmap(strImgDir+_T("axpanel1_lt.bmp"));
	m_hRoundBitmap[1] = GetAxHBitmap(strImgDir+_T("axpanel1_rt.bmp"));
	m_hRoundBitmap[2] = GetAxHBitmap(strImgDir+_T("axpanel1_lb.bmp"));
	m_hRoundBitmap[3] = GetAxHBitmap(strImgDir+_T("axpanel1_rb.bmp"));

	HBITMAP hbodybit[4];
	hbodybit[0] = m_hRoundBitmap[4] = GetAxHBitmap(strImgDir+_T("axpanel1_lt.bmp"));
	hbodybit[1] = m_hRoundBitmap[5] = GetAxHBitmap(strImgDir+_T("axpanel1_rt.bmp"));
	hbodybit[2] = m_hRoundBitmap[6] = GetAxHBitmap(strImgDir+_T("axpanel1_lb.bmp"));
	hbodybit[3] = m_hRoundBitmap[7] = GetAxHBitmap(strImgDir+_T("axpanel1_rb.bmp"));
*/
	m_clrBackIn = GetIndexColor(clBackIn);
	m_clrBack = GetIndexColor(clBack);
	const int ixgap = HORI_GAP + HORI_GAP+1;
	const int iygap = VERT_GAP + VERT_GAP;


	int	sx=0, sy=0, ex=0, ey=0;
	CRect	clientRC, itemRC;
	GetClientRect(&clientRC);

	const int ipanelbot = 50+VERT_GAP;
	m_pBorder = std::make_unique< CfxStatic >();
	m_pBorder->Create("", WS_CHILD|WS_VISIBLE|SS_LEFT|WS_CLIPCHILDREN|WS_CLIPSIBLINGS, 
							CRect(FRAME_SPACE, VERT_GAP, clientRC.right - FRAME_SPACE, ipanelbot), this);
	m_pBorder->SetBkColor(GetIndexColor(66));
	m_pBorder->SetCornerRound(m_hRoundBitmap);

	m_pContent = std::make_unique< CfxStatic >();
	m_pContent->Create("", WS_CHILD|WS_VISIBLE|SS_LEFT|WS_CLIPCHILDREN|WS_CLIPSIBLINGS, 
							CRect(FRAME_SPACE, ipanelbot+TOPANDBODY_SPACE, clientRC.right - FRAME_SPACE, clientRC.bottom - VERT_GAP), this);
	m_pContent->SetBkColor(GetIndexColor(181));
	m_pContent->SetCornerRound(m_hRoundBitmap);

	const int iright = clientRC.right - HORI_GAP-HORI_GAP;
	sx = ixgap; sy = iygap; ex = sx + comboW; ey = sy + comboH * 5; 

	itemRC.SetRect(sx, sy, ex, ey);
	m_pMonthCbo = std::make_unique<CComboBox>();
	m_pMonthCbo->Create(WS_CHILD|WS_VISIBLE|WS_VSCROLL|CBS_DROPDOWNLIST, itemRC, this, ID_MONTHCOMBO);
	m_pMonthCbo->SetFont(m_font);
	SetWindowTheme(m_pMonthCbo->GetSafeHwnd(), L"", L"");
	m_static = std::make_unique< CfxStatic[] >(CNT_STATIC);

	int idx = 0;
	const int istaticw[] = {127, 55,56,55,54,78, 66, 57, 67};
	sx = gridTitleW + ixgap;
	sx = ixgap;
	int ii = 0;
	for (ii = 0; ii < CNT_STATIC; ii++)
	{
		if (ii < 5)
			itemRC.SetRect(ex + staticX[ii].sx, staticX[ii].sy, ex + staticX[ii].sx + staticX[ii].width + 1, staticX[ii].sy + comboH);
		else
		{
			if (ii == 5)
				ex = sx + gridHdr[ii -5].width + gridHdr[ii -5 +1].width;
			else
				ex = sx + gridHdr[ii -5 +1].width + 1;


			ex = sx + istaticw[idx++] + 1;
			itemRC.SetRect(sx, staticX[ii].sy, ex, staticX[ii].sy + comboH);
			sx = ex -1;
		}

//		m_static[ii] = std::make_unique< CfxStatic >();

		m_static[ii].Create(staticX[ii].title, WS_CHILD|WS_VISIBLE|SS_LEFT, itemRC, this);
		m_static[ii].Init(ssCOLOR | ssBORDER, staticX[ii].attr, "", "", staticX[ii].clrBk, staticX[ii].clrBk,
			GetSysColor(COLOR_BTNTEXT), staticX[ii].align);
		
		m_static[ii].SetFont(m_font);
		m_static[ii].SetBorderColor(staticX[ii].clrBox);
	}
	m_static[1].SetFont(m_Bfont); // 현재가

	CString sImg(m_sRoot + "\\Image\\");
	m_bmp2BTN	= *getBitmap(sImg + "2BTN.BMP");
	m_bmp2BTN_DN	= *getBitmap(sImg + "2BTN_DN.BMP");
	m_bmp2BTN_EN	= *getBitmap(sImg + "2BTN_EN.BMP");
	m_bmp3BTN	= *getBitmap(sImg + "3BTN.BMP");
	m_bmp3BTN_DN	= *getBitmap(sImg + "3BTN_DN.BMP");
	m_bmp3BTN_EN	= *getBitmap(sImg + "3BTN_EN.BMP");
	m_bmpUp		= *getBitmap(sImg + "UPMOVE.BMP");
	m_bmpUp_DN	= *getBitmap(sImg + "UPMOVE_DN.BMP");
	m_bmpUp_EN	= *getBitmap(sImg + "UPMOVE_EN.BMP");
	m_bmpDown	= *getBitmap(sImg + "DOWNMOVE.BMP");
	m_bmpDown_DN	= *getBitmap(sImg + "DOWNMOVE_DN.BMP");
	m_bmpDown_EN	= *getBitmap(sImg + "DOWNMOVE_EN.BMP");

	//2013.10.16 KSJ 호가/시고저 선택
	sx = clientRC.right - (btn3W + btn2W + gap1 + btn2W + gap2); 
	ex = sx + btn3W + gap2*5; 
	ey = sy + comboH * 5; itemRC.SetRect(sx, sy, ex, ey);

	m_pGridCbo = std::make_unique<CComboBox>();
	m_pGridCbo->Create(WS_CHILD|WS_VISIBLE|WS_VSCROLL|CBS_DROPDOWNLIST, itemRC, this, ID_GRIDCOMBO);
	m_pGridCbo->SetFont(m_font);
	SetWindowTheme(m_pGridCbo->GetSafeHwnd(), L"", L"");

	m_pGridCbo->AddString("호가");
	m_pGridCbo->AddString("시고저");
	m_pGridCbo->AddString("Greeks");

	CString path;
	int nSelect = 0;
	
	path.Format("%s\\%s\\%s\\%s", m_sRoot, USRDIR, Variant(nameCC, ""), INI_FILE);
	nSelect = GetPrivateProfileInt("IB401500", "select", 0, path);
	m_pGridCbo->SetCurSel(nSelect);
	//KSJ END

	sx = clientRC.right - (btn3W + gap1 + btn2W + gap2); 
	sx = iright - (btn3W + gap2 + btn2W+gap2+gap2);
	ex = sx + btn3W; 
	ey = sy + comboH; itemRC.SetRect(sx, sy, ex, ey);
// 	m_pGreekBtn = new CfxImgButton(m_font);
// 	m_pGreekBtn->Create("GREEKS", itemRC, this, ID_GREEKBTN);
// 	m_pGreekBtn->SetImgBitmap(m_bmp3BTN, m_bmp3BTN_DN, m_bmp3BTN_EN);
	
//	sx = clientRC.right - (btn2W + gap2); 
	sx = ex + gap2;
	ex = sx + btn2W; ey = sy + comboH; itemRC.SetRect(sx, sy, ex, ey);
	m_pMingamBtn = std::make_unique<CfxImgButton>(m_font);
	m_pMingamBtn->Create("설정", itemRC, this, ID_MINGAMBTN);
	m_pMingamBtn->SetImgBitmap(m_bmp2BTN, m_bmp2BTN_DN, m_bmp2BTN_EN);
	
	// CallGrid 영역
	sx = ixgap; 
	ex = sx + gridTitleW; 
	sy = ipanelbot + iygap + gridHeaderH -1;//borderH + gridHeaderH -1; 
	ey = sy + gridH; 

	itemRC.SetRect(sx, sy, ex, ey);
	m_pCallTitle = std::make_unique<CfxStatic>();
	m_pCallTitle->Create("콜", WS_CHILD|WS_VISIBLE|SS_LEFT, itemRC, this);
	m_pCallTitle->Init(ssCOLOR | ssBORDER, 0, "", "", 0x00F6EAE2, 0x00F6EAE2, GetSysColor(COLOR_BTNTEXT), 
							staticX[ii].align);
	m_pCallTitle->SetFont(m_font);
	m_pCallTitle->SetBorderColor(0x00BBCBBC);

	sy = ey - comboH; itemRC.SetRect(sx, sy, ex, ey);
	m_pUpBtn = std::make_unique<CfxImgButton>(m_font);
	m_pUpBtn->Create("", itemRC, this, ID_UPBTN);
	m_pUpBtn->SetImgBitmap(m_bmpUp, m_bmpUp_DN, m_bmpUp_EN);

	sx = ex; ex = clientRC.Width(); 
	sy = borderH; 
//	sy = ipanelbot + iygap+TOPANDBODY_SPACE;
	sy = ipanelbot + TOPANDBODY_SPACE + VERT_GAP;
	itemRC.SetRect(sx, sy, ex, ey);
//VERT_GAP
	m_pCallGrid = std::make_unique <CfxGrid>();

	m_pCallGrid->Create(itemRC, this, ID_CALLGRID, GVSC_VERT, GVDD_FULL, WS_CHILD|WS_TABSTOP|WS_VISIBLE);
	m_pCallGrid->SetFont(m_font);
	initGrid(m_pCallGrid.get(), true);

	// PutGrid 영역
	sx = ixgap; 
	ex = sx + gridTitleW; 
	sy = ey+1; 
	ey = sy + gridH -1; 
	itemRC.SetRect(sx, sy, ex, ey);
	m_pPutTitle = std::make_unique<CfxStatic>();
	m_pPutTitle->Create("풋", WS_CHILD|WS_VISIBLE|SS_LEFT, itemRC, this);
	m_pPutTitle->Init(ssCOLOR | ssBORDER, 0, "", "", 0x00F6EAE2, 0x00F6EAE2, GetSysColor(COLOR_BTNTEXT), staticX[ii].align);
	m_pPutTitle->SetFont(m_font);
	m_pPutTitle->SetBorderColor(0x00BBCBBC);

	ey = sy + comboH; itemRC.SetRect(sx, sy, ex, ey);
	m_pDownBtn = std::make_unique<CfxImgButton>(m_font);
	m_pDownBtn->Create("", itemRC, this, ID_DOWNBTN);
	m_pDownBtn->SetImgBitmap(m_bmpDown, m_bmpDown_DN, m_bmpDown_EN);

	sx = ex; 
	ex = clientRC.Width(); ey = sy + gridH-1; itemRC.SetRect(sx, sy, ex, ey);
	m_pPutGrid = std::make_unique <CfxGrid>();
	m_pPutGrid->Create(itemRC, this, ID_PUTGRID, GVSC_VERT, GVDD_FULL, WS_CHILD|WS_TABSTOP|WS_VISIBLE);
	m_pPutGrid->SetFont(m_font);
	initGrid(m_pPutGrid.get(), false);
	
// 	m_bShowMingam = FALSE;
// 	CString sval = ((CIB401500App*)AfxGetApp())->GetBuffer(_T("GREEKS")); 
// 	if(!sval.IsEmpty()) 
// 		m_bShowMingam = atoi(sval);

//	showMingamField(m_bShowMingam);

	initSymbol();

//	m_cfg = new CCfg();

	m_cfg = std::make_unique<CCfg>(); 
	m_cfg->CreateCfg(m_pParent, m_sRoot);

	m_opsensCal = std::make_unique< COpSensCal >();
	m_opsensCal->CreateOpSensCal(this, m_sRoot);

	/*
	CString sTab;
	sy = ey + 1; ey = sy + 20; ex = sx + 100; itemRC.SetRect(sx, sy, ex, ey);
	sTab.Format("%s\\%s", m_sRoot, "tab");
	m_pCodeCtrl = new CfxCodeCtrl(m_pParent, sTab);
	m_pCodeCtrl->Create(this, itemRC, ID_CODEEDIT);
	m_pCodeCtrl->SetUnit(GU_FUTURE);

	CString history = _T("000010 가나다1\t000020 가나다2\t"); //	historyCC
	m_pCodeCtrl->SetHistory(history);
	*/

	OnSelChangeMonth();
	OnSelChangeGrid();

	m_iOldSize[0] = mapWidth;
	m_iOldSize[1] = mapHeight;
	m_bBlink = false;//GetBlinkInfo();
	SetTimer(THREAD_TIMER_KEY, THREAD_INTERVAL, NULL);
	m_bCreated = TRUE;
}

long CMapWnd::OnMessage(WPARAM wParam, LPARAM lParam)
{
	if(!m_bCreated) return 0;

	switch(LOBYTE(LOWORD(wParam)))
	{
	case DLL_OUB:
		switch(HIBYTE(LOWORD(wParam)))
		{
		case KEY_DATA:
			{
				parseData((char*)lParam, HIWORD(wParam));
			}
			break;
		case 10: // Kospi
			{
				m_bResult1 = true;
				int pos = 0;
				CString sCode, sVal, sData((char*)lParam);

				pos = sData.Find('\t');
				if (pos == -1) return 0;
				sCode = sData.Left(pos++);
				sData = sData.Mid(pos);

				pos = sData.Find('\t');	if (pos == -1)  return 0;
				sVal = sData.Left(pos++); sData = sData.Mid(pos);
				m_pdataX[datKpCode].val = sCode;
				m_pdataX[datKpCur].val = sVal;

				if (m_bResult1 && m_bResult2 && m_bResult3)
					sendDataTr();
			}
			break;
		case 11: // K200
			{
				m_bResult2 = true;

				//int pos = 0;
				CString sCode, sVal, sData((char*)lParam);

				sCode = Parser(sData); 
				if (sCode.IsEmpty() || sData.IsEmpty()) break;

				m_pdataX[datK200Code].val = sCode;
				m_pdataX[datK200Cur].val = Parser(sData);
				if (atof(m_pdataX[datK200Cur].val) == 0)
					m_pdataX[datK200Cur].val = Parser(sData);
				else
					Parser(sData);
				m_pdataX[datK200Time].val = Parser(sData);
				
				m_opsensCal->SetK200Cur(m_pdataX[datK200Cur].val);
				m_opsensCal->SetTime(m_pdataX[datK200Time].val);

				if (m_bResult1 && m_bResult2 && m_bResult3)
					sendDataTr();
			}
			break;
		case 12: // 선물
			{
				m_bResult3 = true;

				int pos = 0;
				CString sCode, sVal, sData((char*)lParam);

// 				OutputDebugString("[KSJ] Message sData[" + sData + "]");

				pos = sData.Find('\t');
				if (pos == -1) return 0;
				sCode = sData.Left(pos++);
				sData = sData.Mid(pos);

				pos = sData.Find('\t');	if (pos == -1)  return 0;
				sVal = sData.Left(pos++); sData = sData.Mid(pos);
				m_pdataX[datFuCode].val = sCode;
				m_pdataX[datFuCur].val = sVal;

// 				OutputDebugString("[KSJ] Message curr[" + sVal + "]");

				pos = sData.Find('\t');	if (pos == -1)  return 0;
				sVal = sData.Left(pos++); sData = sData.Mid(pos);
				m_pdataX[datFuDif].val = sVal;

				pos = sData.Find('\t');	if (pos == -1)  return 0;
				sVal = sData.Left(pos++); sData = sData.Mid(pos);
				m_pdataX[datFuRate].val = sVal;

				pos = sData.Find('\t');	if (pos == -1)  return 0;
				sVal = sData.Left(pos++); sData = sData.Mid(pos);
				m_pdataX[datFuSell].val = sVal;

				pos = sData.Find('\t');	if (pos == -1)  return 0;
				sVal = sData.Left(pos++); sData = sData.Mid(pos);
				m_pdataX[datFuBuy].val = sVal;

				pos = sData.Find('\t');	if (pos == -1)  return 0;
				sVal = sData.Left(pos++); sData = sData.Mid(pos);
				m_pdataX[datFuGVol].val = sVal;

				pos = sData.Find('\t');	if (pos == -1)  return 0;
				sVal = sData.Left(pos++); sData = sData.Mid(pos);
				m_pdataX[datFuMigeul].val = sVal;

				pos = sData.Find('\t');	if (pos == -1)  return 0;
				sVal = sData.Left(pos++); sData = sData.Mid(pos); 
				sVal.TrimRight(); if (sVal.IsEmpty()) sVal = "0";
				m_pdataX[datFuMDaebi].val = sVal;

				m_opsensCal->SetFutureCur(m_pdataX[datFuCur].val);
				if (m_bResult1 && m_bResult2 && m_bResult3)
					sendDataTr();
			}
			break;
		default:
			break;
		}
		m_bRTMReady = TRUE;	// To start RTM Parsing
		break;
	case DLL_ALERT:
		break;
	
	//2012.12.07 KSJ DLL_ALERTx로 변경
	case DLL_ALERTx:
		parseAlert(lParam);
		break;
	case DLL_SETPAL:
		SetPalette();
		break;

	case DLL_SETFONT:break;
	case DLL_SETFONTx:
		ChangeFont(HIWORD(wParam), (char*)lParam );
		break;

	default:
		break;
	}
	return 0;
}

/* 현재 사용하지 않는 코드 escdream 2022.07.08
BOOL CMapWnd::IsHogaData(char *data)
{
	CString	sData(data);
	CString	sDataCode;		// 종목 Code
	int	posData = -1;
//	char	*pTemp = NULL;
	std::unique_ptr<char[]> pTemp;
	BOOL	bRet = FALSE;
	const char	chegurlSym[] = "\t023\t"; 
	
	posData = sData.Find("\t");
	sDataCode = sData.Left(posData++);

	pTemp = std::make_unique<char[]>(strlen(data) + 1);
//	pTemp = new char [strlen(data)+1];


	memcpy(pTemp.get(), data, strlen(data));
//	strcpy(pTemp, data);
		
	// 현재가심볼("023")로 Data가 체결Data인지 호가Data인지를 구분한다.
	if (strstr(data, chegurlSym) == NULL)
		bRet = TRUE;
	else
		bRet = FALSE;

//	delete [] pTemp;

	return bRet;
}
*/
// 유일한 코드만 처리
/* 현재 사용하지 않는 코드 escdream 2022.07.08
void CMapWnd::CheckDupCodeData(char * dat)
{
	CString	sData(dat), sTemp;
	CString	sDataCode, sTempCode;		// 종목 Code
	int	posData = -1, posTemp = -1;
	char	*pTemp = NULL;
	BOOL	bExist = FALSE;			// code 존재 유무 check
	BOOL	bNow_HogaData = FALSE;

	posData = sData.Find("\t");
	sDataCode = sData.Left(posData++);

	bNow_HogaData = IsHogaData(dat);	// 현재 데이터 종류 판별(호가/체결)
	int ii = 0;
	for (ii = 0; ii < m_arBytesRTM.GetSize(); ii++)
	{
		if(!m_bCreated) return;
		pTemp = m_arBytesRTM.GetAt(ii);
		sTemp = CString(pTemp);
		posTemp = sTemp.Find("\t");
		if (posTemp < 0) return;
		sTempCode = sTemp.Left(posTemp++);
		
		if (sDataCode == sTempCode)
		{
			if (bNow_HogaData != IsHogaData(pTemp))	// 호가/체결 데이터인지 구분
				bExist = FALSE;		
			else	
				bExist = TRUE;	
			break;
		}
	}

	if (bExist == FALSE)
		m_arBytesRTM.Add(dat);
	else
	{
		if (ii < m_arBytesRTM.GetSize())
		{
			pTemp = m_arBytesRTM.GetAt(ii);
			delete [] pTemp;
			pTemp = NULL;
			m_arBytesRTM.SetAt(ii, dat);
		}
	}
}
*/

void CMapWnd::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
//	CMemDC	mdc(&dc);

	CRect rcClient{};
	GetClientRect(&rcClient);
	dc.FillSolidRect(rcClient, m_clrBack);

}

void CMapWnd::initGrid(CfxGrid* grid, bool header)
{
	const int count = sizeof(gridHdr) / sizeof(_gridHdr);
	if (header)
		grid->Initial(10, count, 1, 0);
	else
		grid->Initial(10, count, 0, 0);

	LOGFONT lf; m_font->GetLogFont(&lf); lf.lfHeight = 90;
	GVITEM	gvitem;

 	// column header setting
	m_iGridColwidths = 0;
	if (header)
	{
		for (int ii = 0; ii < count; ii++)
		{
			gvitem.row   = 0;
			gvitem.col   = ii;
			gvitem.state = 0;
			gvitem.mask  = GVMK_TEXT|GVMK_FORMAT|GVMK_FONT|GVMK_ATTR|GVMK_FGCOLOR|GVMK_BKCOLOR|GVMK_MISC;
			gvitem.format= GVFM_HEADER;

			gvitem.fgcol = GetIndexColor(76);
			gvitem.bkcol = GetIndexColor(74);

			CopyMemory(&gvitem.font, &lf, sizeof(LOGFONT));

			gvitem.text  = gridHdr[ii].text;
			
// 			CString strTemp;
// 			strTemp.Format("[KSJ] Head[%s]", gridHdr[ii].text);
// 			OutputDebugString(strTemp);

			gvitem.attr  = gridHdr[ii].attr;
			grid->SetItem(&gvitem);
		};
	};

	int ii = 0;
	for (ii = 0; ii < count; ii++)
		grid->SetColumnWidth(ii, gridHdr[ii].width);

	const COLORREF clrGridFgColor = GetIndexColor(GRID_TXT_COLOR);
	for ( ii = 0; ii < count; ii++)
	{
		gvitem.state = 0;
		gvitem.mask  = GVMK_FORMAT|GVMK_FONT|GVMK_ATTR|GVMK_FGCOLOR|GVMK_BKCOLOR|GVMK_SYMBOL;

		gvitem.format= gridHdr[ii].format;
		gvitem.symbol= gridHdr[ii].symbol; 
		gvitem.attr  = gridHdr[ii].attrx;

		gvitem.fgcol = clrGridFgColor;
		gvitem.bkcol = GetIndexColor(68);
		CopyMemory(&gvitem.font, &lf, sizeof(LOGFONT));

		grid->SetColumnItems(ii, &gvitem);
	}

	for (ii = 0; ii < grid->GetRowCount(); ii++)
		grid->SetRowHeight(ii, rowH);

	//grid->SetGridDIScroll(false);
	//grid->SetGridScroll(false);
	//grid->SetFocusCellSensitive(false);
	if (header)
		grid->SetStepColor(1, GetIndexColor(77), GetIndexColor(68));
	else
		grid->SetStepColor(1, GetIndexColor(68), GetIndexColor(77));

	grid->SetGridLine(GVLN_VERT);
	grid->SetGridScroll(GVSC_VERT);
	grid->SetRowColResize(FALSE, FALSE);
	grid->SetGridFocusLine(FALSE, TRUE);
	grid->SetGridColor(GetIndexColor(75));
	grid->SetBKSelColor(GetIndexColor(78));
	grid->Adjust();

}

void CMapWnd::OnSelChangeMonth()
{
	sendSubTr();
}

void CMapWnd::OnSelChangeGrid()
{
	CString path;
	CString strSelect;
	
	path.Format("%s\\%s\\%s\\%s", m_sRoot, USRDIR, Variant(nameCC, ""), INI_FILE);
	strSelect.Format("%d", m_pGridCbo->GetCurSel());
	WritePrivateProfileString("IB401500", "select", strSelect, path);

	showSiKoJeField(m_pGridCbo->GetCurSel());
	SetGridColumnWidth();
}

void CMapWnd::sendDataTr() 
{
	int idx = m_pMonthCbo->GetCurSel();

	CString opt;
	struct _mid mid {};

	if (idx > 0)
		sprintf(mid.mymd, "%6s", m_pCode[idx]);
	else
		sprintf(mid.mymd, "      ");	// 만기월 

	opt = m_cfg->ReadCfg("HVDay");
	if (atoi(opt) <= 0) opt = "90";
	sprintf(mid.hvil, "%s", opt.GetString());			// 역사적 변동일

	opt = m_cfg->ReadCfg("RDType");
	if (opt.GetLength() != 1) opt = "0";

	opt = (opt == "0") ? "1" : "0";
	//sprintf(mid.jjil, "%1d", atoi(opt));	// 잔존일수 0:달력, 1:영업(거래)
	mid.jjil[0] = opt[0]; // 잔존일수 0:달력, 1:영업(거래)

	sendTR("PIBO4015", (char*)&mid, L_MID, KEY_DATA, 0);	//2012.12.05 KSJ PIBO4015 --> PIBF4015
}

void CMapWnd::sendSubTr()
{
	CString sSend;
	m_bResult1 = m_bResult2 = m_bResult3 = false;

	// KOSPI 지수
	sSend.Format("20301%c%s\t20021\t20023\t", 0x7f, "001"); 
	sendTR("POOPPOOP", (LPSTR)(LPCTSTR)sSend, sSend.GetLength(), 10, US_OOP);

	// K200 지수
	sSend.Format("20301%c%s\t20021\t20023\t20306\t20034\t", 0x7f, "101"); // rts코드, 현재가, 전일종가, 체결시간 
	sendTR("POOPPOOP", (LPSTR)(LPCTSTR)sSend, sSend.GetLength(), 11, US_OOP);

	// 선물
	CString sFCode = (char*)m_pParent->SendMessage(WM_USER, MAKEWPARAM(codeDLL, MAKEWORD(getCODEfuture, 0)), 0);
	sSend.Format("30301%c%s\t30021\t31023\t31024\t31033\t31025\t31026\t31027\t31201\t31206\t", 0x7f, sFCode);
	sendTR("POOPPOOP", (LPSTR)(LPCTSTR)sSend, sSend.GetLength(), 12, US_OOP);
}

void CMapWnd::parseData(char* pData, int datL)
{
	const COLORREF clrGridFgColor = GetIndexColor(GRID_TXT_COLOR);
	struct  _mod	*pMod;
	pMod = (struct _mod*)pData;

	initData();

	CString str;
	if (m_pMonthCbo->GetCount() == 0)
	{
		for (int ii = 0; ii < 11; ii++)
		{
			sprintf(m_pCode[ii], "%.6s", pMod->mgym[ii]);
			str.Format("%.2s%.2s", &m_pCode[ii][2], &m_pCode[ii][4]);
			m_pMonthCbo->AddString(str);
		}
		m_pMonthCbo->SetCurSel(0);
	}

	m_date.Format("%.8s", pMod->date); str.TrimRight();
	str.Format("%.5s", pMod->frts);	str.TrimRight();	m_pdataX[datFuCode].val = str + "000";
	str.Format("%.8s", pMod->fcur);	str.TrimRight();	
	/*str.Replace("+", ""); str.Replace("-", "");*/		m_pdataX[datFuCur].val = str; 
	str.Format("%.8s", pMod->fdif);	str.TrimRight();	m_pdataX[datFuDif].val = str; 
	str.Format("%.8s", pMod->frat);	str.TrimRight();	m_pdataX[datFuRate].val = str;

// 	str.Format("%.8s", pMod->krts);	str.TrimRight();	m_pdataX[datK200Code].val = str;
// 	str.Format("%.8s", pMod->kcur);	str.TrimRight();	m_pdataX[datK200Cur].val = str;
	str.Format("%.8s", pMod->kdif);	str.TrimRight();	m_pdataX[datK200Dif].val = str;
	str.Format("%.8s", pMod->krat);	str.TrimRight();	m_pdataX[datK200Rate].val = str;
	str.Format("%.3s", pMod->jjil);	str.TrimRight();	m_jjilOrigin.Format("%.2f", atof(str));
	str.Format("%.8s", pMod->hval);	str.TrimRight();	m_hv = str;
	str.Format("%.8s", pMod->cdgl);	str.TrimRight();	m_cdgl = str;
	str.Format("%.8s", pMod->bdjs);	str.TrimRight();	m_bdgs.Format("%.6f", atof(str) / 1000000);
	str.Format("%.8s", pMod->atmg);	str.TrimRight();	m_atmg = str;
	str.Format("%.4s", pMod->grec);	str.TrimRight();	m_DataCount = atoi(str);

	if (m_DataCount <= 0)
		return;

	//m_pStrike  = new _strike[m_DataCount];
	m_pStrike = std::make_unique< STRIKE[]>(m_DataCount);
	m_pCallGrid->SetRowCount(1);
	m_pPutGrid->SetRowCount(1);
//	CString ss;
//	ss.Format("%d",  m_DataCount);
//AfxMessageBox(ss);

	int nRow = 1;
	struct  _gmod* gmod{};
	for (int ii = 0; ii < m_DataCount; ii++)
	{
		insertRow();
		gmod = &pMod->gmod[ii];

		sprintf(m_pStrike[ii].ccod, "%.8s", gmod->ccod);
		sprintf(m_pStrike[ii].pcod, "%.8s", gmod->pcod);

		str.Format("%.8s", gmod->price); str.TrimRight();	
		m_pCallGrid->SetItemText(nRow, gfPrice, str);		
		m_pPutGrid->SetItemText(nRow-1, gfPrice, str);

		str.Format("%.8s", gmod->ccur); str.TrimRight();	
		m_pCallGrid->SetItemText(nRow, gfCur, str);
		
		str.Format("%.8s", gmod->cdif); str.TrimRight();	
		
		if (str.GetAt(0) == _T('+'))
			str.Delete(0);
		
		m_pCallGrid->SetItemText(nRow, gfDif, str);	
		

		str.Format("%.8s", gmod->cmdga); str.TrimRight();	m_pCallGrid->SetItemText(nRow, gfMdga, str);
		str.Format("%.8s", gmod->cmsga); str.TrimRight();	m_pCallGrid->SetItemText(nRow, gfMsga, str);
		str.Format("%.8s", gmod->cgvol); str.TrimRight();	m_pCallGrid->SetItemText(nRow, gfGvol, str);
		str.Format("%.8s", gmod->cmgjv); str.TrimRight();	m_pCallGrid->SetItemText(nRow, gfMigeul, str);
		
		str.Format("%.8s", gmod->cmgjg); str.TrimLeft();	str.TrimRight();

		if (str.GetAt(0) == _T('+'))
		{
			m_pCallGrid->SetItemFgColor(nRow, gfMDif, clrRED);
			str.Delete(0);
		}
		else if (str.GetAt(0) == _T('-'))
			m_pCallGrid->SetItemFgColor(nRow, gfMDif, clrBLUE);
		else
			m_pCallGrid->SetItemFgColor(nRow, gfMDif, clrGridFgColor);
		
		m_pCallGrid->SetItemText(nRow, gfMDif, str);

		//2013.10.17 KSJ  시고저추가
		str.Format("%.8s", gmod->csiga); str.TrimRight();	m_pCallGrid->SetItemText(nRow, gfSiga, str);
		str.Format("%.8s", gmod->ckoga); str.TrimRight();	m_pCallGrid->SetItemText(nRow, gfKoga, str);
		str.Format("%.8s", gmod->cjega); str.TrimRight();	m_pCallGrid->SetItemText(nRow, gfJega, str);
		
		str.Format("%.8s", gmod->pcur); str.TrimRight();	m_pPutGrid->SetItemText(nRow-1, gfCur, str);
		
		str.Format("%.8s", gmod->pdif); str.TrimRight();	
		
		if (str.GetAt(0) == _T('+'))
			str.Delete(0);
				
		m_pPutGrid->SetItemText(nRow-1, gfDif, str);

		str.Format("%.8s", gmod->pmdga); str.TrimRight();	m_pPutGrid->SetItemText(nRow-1, gfMdga, str);
		str.Format("%.8s", gmod->pmsga); str.TrimRight();	m_pPutGrid->SetItemText(nRow-1, gfMsga, str);
		str.Format("%.8s", gmod->pgvol); str.TrimRight();	m_pPutGrid->SetItemText(nRow-1, gfGvol, str);
		str.Format("%.8s", gmod->pmgjv); str.TrimRight();	m_pPutGrid->SetItemText(nRow-1, gfMigeul, str);
		str.Format("%.8s", gmod->pmgjg); str.TrimLeft();	str.TrimRight();
		
		if (str.GetAt(0) == _T('+'))
		{
			m_pPutGrid->SetItemFgColor(nRow-1, gfMDif, clrRED);
			str.Delete(0);
		}
		else if (str.GetAt(0) == _T('-'))
			m_pPutGrid->SetItemFgColor(nRow-1, gfMDif, clrBLUE);
		else
			m_pPutGrid->SetItemFgColor(nRow-1, gfMDif, clrGridFgColor);
		
		m_pPutGrid->SetItemText(nRow-1, gfMDif, str);

		//2013.10.17 KSJ  시고저추가
		str.Format("%.8s", gmod->psiga); str.TrimRight();	m_pPutGrid->SetItemText(nRow-1, gfSiga, str);	
		str.Format("%.8s", gmod->pkoga); str.TrimRight();	m_pPutGrid->SetItemText(nRow-1, gfKoga, str);
		str.Format("%.8s", gmod->pjega); str.TrimRight();	m_pPutGrid->SetItemText(nRow-1, gfJega, str);
		
		nRow++;
	}

	m_pPutGrid->SetRowCount(m_pPutGrid->GetRowCount() -1);

	m_opsensCal->SetUserName(Variant(nameCC, ""));
	m_opsensCal->ReadCfg();
	m_opsensCal->SetCd(m_cdgl);
	m_opsensCal->SetBedangJisu(m_bdgs);
	
	generateFData();
	generateODatas();

	m_pCallGrid->Invalidate();
	m_pPutGrid->Invalidate();
}

//2012.12.07 KSJ AlertX로 변경
void CMapWnd::parseAlert(LPARAM lParam)
{
	//int pos = 0, 
	int nRow = -1, nDisplayRow = 0;
	enum alertType {ctK200, ctKospi, ctFuture, ctOption} codeType = ctK200;
	CString	sCode, sym, val, sData;
	CfxGrid	*grid = NULL;

	if (m_pStrike == NULL) return;

	struct _alertR* alertR;
	alertR = (struct _alertR*)lParam;
	DWORD* data = nullptr;

	sCode = alertR->code;

	if (sCode[0] == 'A') 
		return;

	if (m_pdataX[datK200Code].val.CompareNoCase(sCode) == 0)
	{
		codeType = ctK200;
	}	
	else if (m_pdataX[datKpCode].val.CompareNoCase(sCode) == 0)
	{
		codeType = ctKospi;
	}
	else if (m_pdataX[datFuCode].val.CompareNoCase(sCode) == 0)
	{
		codeType = ctFuture;
	}
	else
	{
		if (sCode.GetLength() == 0 || (sCode[0] != '2' && sCode[0] != '3')) return;

		for (int ii = 0; ii < m_DataCount; ii++)
		{
			if (sCode.Compare(m_pStrike[ii].ccod) == 0)
			{
				nRow = ii;
				grid = m_pCallGrid.get();
				break;
			} 
			else if (sCode.Compare(m_pStrike[ii].pcod) == 0)
			{
				nRow = ii;
				grid = m_pPutGrid.get();
				break;
			}
		}
		if (nRow == -1) return;

		codeType = ctOption;
	}

	//for(int nIndex = 0; nIndex < alertR->size; nIndex++)
	{
		data = (DWORD*)alertR->ptr[0];

		if (codeType == ctK200)
		{
			if (sCode == m_pdataX[datK200Code].val)
			{
				if(!m_pdataX[datK200Cur].sym.IsEmpty() && data[atoi(m_pdataX[datK200Cur].sym)])
				{
					m_pdataX[datK200Cur].val = (char*)data[atoi(m_pdataX[datK200Cur].sym)];

					if(!m_pdataX[datK200Dif].sym.IsEmpty() && data[atoi(m_pdataX[datK200Dif].sym)])
						m_pdataX[datK200Dif].val = (char*)data[atoi(m_pdataX[datK200Dif].sym)];
				}
			}
			m_opsensCal->SetK200Cur(m_pdataX[datK200Cur].val);
			generateFData();
			generateODatas();

		}
		else if(codeType == ctKospi)
		{
			if(!m_pdataX[datKpCur].sym.IsEmpty() && data[atoi(m_pdataX[datKpCur].sym)])
				m_pdataX[datKpCur].val = (char*)data[atoi(m_pdataX[datKpCur].sym)];
		}
		else if (codeType == ctFuture)
		{
			for (int ii = datFuCur; ii <= datFuMDaebi; ii++)
			{
				if (!m_pdataX[ii].sym.IsEmpty() && (char*)data[atoi(m_pdataX[ii].sym)])
				{
					m_pdataX[ii].val = (char*)data[atoi(m_pdataX[ii].sym)];
				}
			}
			m_opsensCal->SetFutureCur(m_pdataX[datFuCur].val);
			generateFData();
			generateODatas();
		}
		else
		{
			if (grid == m_pCallGrid.get())
				nDisplayRow = nRow + 1;
			else
				nDisplayRow = nRow;

			CString sPrv, strTemp;
			for (int ii = 0; ii < gfEndCnt; ii++)
			{
				if (gridHdr[ii].symbol != "" && atoi(gridHdr[ii].symbol) != 0)
				{
					val = (char*)data[atoi(gridHdr[ii].symbol)];
					sPrv = grid->GetItemText(nDisplayRow, ii);
					
					if (sPrv.CompareNoCase(val) != 0 && !val.IsEmpty()) 
					{
						if((atoi(gridHdr[ii].symbol) == 25 || atoi(gridHdr[ii].symbol) == 26) && data[23])
						{
							double dVal;

							dVal = atof((char*)data[23]) - atof((char*)data[24]) - atof(val);

// 							CString strTemp;
// 							strTemp.Format("[KSJ] 000[%s] val[%s] symbol[%s] dVal[%.2f]", (char*)data[0], val, gridHdr[ii].symbol, dVal);
// 							OutputDebugString(strTemp);

							if(dVal != 0)
							{
								CString sBuho;
								sBuho = (dVal < 0) ? "-":"+";

								if(val.GetAt(0) != '+' && val.GetAt(0) != '-')
									val = sBuho + val;
								//grid->SetItemFgColor(nDisplayRow, ii, (dVal < 0) ? clrRED:clrBLUE);
							}
						}

						grid->SetItemText(nDisplayRow, ii, val);

						if (m_bBlink) grid->Blink(nDisplayRow, ii, m_blinkColor);
					}
				}
			}
			generateOData(nRow, grid == m_pCallGrid.get());
		}
	}
}

COLORREF CMapWnd::ParseSymbol(char *pData, CString& symbolS, CString& signS)
{
	COLORREF color{};
	switch (pData[0])
	{
	case 1:
	case '1':
		color = clrRED;
		symbolS = "↑";
		signS = "+";
		break;
	case 2:
	case '2':
	case '+':
		color = clrRED;
		symbolS = "▲";
		signS = "+";
		break;
	case 4:
	case '4':
		color = clrBLUE;
		symbolS = "↓";
		signS = "-";
		break;
	case 5:
	case '5':
	case '-':
		color = clrBLUE;
		symbolS = "▼";
		signS = "-";
		break;
	default:
		color = clrBLACK;
		symbolS = " ";
		signS = " ";
		break;
	}

	return color;
}

void CMapWnd::OnDestroy() 
{
	m_bCreated = FALSE;
	initData();

	CWnd::OnDestroy();

	if (m_pMonthCbo)	{ m_pMonthCbo->DestroyWindow();		}
	if (m_pGridCbo)		{ m_pGridCbo->DestroyWindow();		}
	if (m_pGreekBtn)	{ m_pGreekBtn->DestroyWindow();		}
	if (m_pMingamBtn)	{ m_pMingamBtn->DestroyWindow();	}
	if (m_pCallTitle)	{ m_pCallTitle->DestroyWindow();	}
	if (m_pPutTitle)	{ m_pPutTitle->DestroyWindow();	}
	if (m_pCallGrid)	{ m_pCallGrid->DestroyWindow(); }
	if (m_pPutGrid)		{ m_pPutGrid->DestroyWindow(); }
	if (m_pUpBtn)       { m_pUpBtn->DestroyWindow();		}
	if (m_pDownBtn)		{ m_pDownBtn->DestroyWindow();		}
	if (m_pBorder)		{ m_pBorder->DestroyWindow();			}
	if(m_pContent)		{ m_pContent->DestroyWindow();			}
// escdream check need
	if (m_cfg) { m_cfg->DestroyWindow(); } 

	for (int ii = 0; ii < CNT_STATIC; ii++)
	{
		if (m_static[ii])
		{
			m_static[ii].DestroyWindow();
			//delete staticX[ii].statW;
		}
	}
	
	/* 현재 사용하지 않는 코드 escdream 2022.07.08
	stopRTM();

	if (m_arBytesRTM.GetSize() > 0)
	{
		char * pTemp = NULL;
		for (int jj = 0; jj < m_arBytesRTM.GetSize(); jj++)
		{
			pTemp = m_arBytesRTM.GetAt(jj);
			delete [] pTemp;
			pTemp = NULL;
		}
	}
	*/
}

void CMapWnd::initData()
{
	if (m_pStrike)
	{
		//delete[] m_pStrike;
		m_pStrike.reset();
//		m_pStrike = NULL;
	}
}

void CMapWnd::initSymbol()
{
	//m_pdataX[datFuName].sym	= "";
	m_pdataX[datFuCur].sym		= "023";
	m_pdataX[datFuDif].sym		= "024";
	m_pdataX[datFuRate].sym		= "033";
	m_pdataX[datFuSell].sym		= "025";
	m_pdataX[datFuBuy].sym		= "026";
	m_pdataX[datFuGVol].sym		= "027";
	m_pdataX[datFuMigeul].sym	= "201";
	m_pdataX[datFuMDaebi].sym	= "206";
	//m_pdataX[datKpName].sym	= "";
	m_pdataX[datK200Cur].sym	= "023";
	m_pdataX[datK200Dif].sym	= "024";
	m_pdataX[datK200Time].sym	= "034";
	//m_pdataX[datKpRate].sym	= "";
	m_pdataX[datKpCur].sym		= "023";
}

void CMapWnd::generateFData()
{
	CString str;
	double	d1 = 0.0, kfRate = 0.0;
	int bpType = 0;

	CString opt;
	opt = m_cfg->ReadCfg("TimeValue");
	str = m_jjilOrigin;
	if (atoi(opt) == 1)
	{
		double stime=0.0, etime=0.0, ctime=0.0;
		double timeloss = 0;
		CString sCtime;

		stime = GetMinTime(m_cfg->ReadCfg("TimeStart"));	// 시작시간
		etime = GetMinTime(m_cfg->ReadCfg("TimeEnd"));		// 종료시간
		sCtime= m_pdataX[datK200Time].val;			// 현재시간

		sCtime.Replace(":", "");
		if (sCtime.CompareNoCase("장마감") == 0)
			sCtime = "1500";
		else if (sCtime.CompareNoCase("장종료") == 0)
			sCtime = "1600";		
		ctime = GetMinTime(sCtime);
		
		if (ctime > stime)
		{
			if (ctime > etime)  ctime = etime;
			if (stime < etime)  timeloss = (ctime-stime) / (etime - stime);
		}
		str.Format("%.2f", atof(str) - timeloss);

	}
	else
		str.Format("%.2f", atof(str));
	m_pdataX[datJjil].val = str;

	bpType = atoi(m_cfg->ReadCfg("BPType"));
	switch (bpType)
	{
	case 1:
		kfRate = 0;
		d1 = fabs(atof(m_pdataX[datFuCur].val));
		m_pdataX[datBasicPrice].val.Format("%.2f", d1);
		break;
	case 2:
		kfRate = atof(m_cfg->ReadCfg("K200Rate"));
		d1 = (fabs(atof(m_pdataX[datK200Cur].val) - atof(m_bdgs)) * kfRate + fabs(atof(m_pdataX[datFuCur].val)) * (100 - kfRate)) / 100;
		m_pdataX[datBasicPrice].val.Format("%.2f", d1);
		m_pdataX[datBasicRate].val.Format("%.2f", kfRate);
		break;
	case 3:
		if (atoi(m_cfg->ReadCfg("RDType")))
			d1 = fabs(atof(m_pdataX[datFuCur].val)) * exp(-1 * atof(m_cdgl) / 100 * atof(m_pdataX[datJjil].val) / 365);
		else
			d1 = fabs(atof(m_pdataX[datFuCur].val)) * exp(-1 * atof(m_cdgl) / 100 * atof(m_pdataX[datJjil].val) / 250);
		m_pdataX[datBasicPrice].val.Format("%.2f", d1);
		m_pdataX[datBasicRate].val.Format("%s", "자동계산");
		kfRate = -1;
		break;
	default:
		kfRate = 100;
		d1 = fabs(atof(m_pdataX[datK200Cur].val)) - atof(m_bdgs);
		m_pdataX[datBasicPrice].val.Format("%.2f", d1);
		break;
	}

	CString symbol, sign;
	COLORREF clr{};
	for (int ii = 0; ii < sizeof(staticX) / sizeof(staticX[0]); ii++)
	{
		switch (staticX[ii].dataIdx)
		{
		case datFuCur:
			{
				clr = ParseSymbol( (LPSTR)(LPCTSTR)m_pdataX[staticX[ii].dataIdx].val, symbol, sign );
				m_static[ii].SetText( m_pdataX[staticX[ii].dataIdx].val.Mid(1) );
				m_static[ii].SetFgColor(clr);
			}
			break;
		case datFuMDaebi:
			{
				clr = ParseSymbol( (LPSTR)(LPCTSTR)m_pdataX[staticX[ii].dataIdx].val, symbol, sign );
				m_static[ii].SetText( m_pdataX[staticX[ii].dataIdx].val.Mid(sign == "+" ? 1 : 0) );
				m_static[ii].SetFgColor(clr);
			}
			break;
		case datFuRate:
			{
				clr = ParseSymbol( (LPSTR)(LPCTSTR)m_pdataX[staticX[ii].dataIdx].val, symbol, sign );
				m_static[ii].SetText( m_pdataX[staticX[ii].dataIdx].val.Mid(1)  + "%");
				m_static[ii].SetFgColor(clr);
			}
			break;
		case datBasicRate:
			{
				if (kfRate == -1)
					str.Format("%s", " 자동계산 ");
				else
					str.Format("%.2f%% / %.2f%%", kfRate, (100 - kfRate));
				m_static[ii].SetText(str);
			}
		case -1:
			break;
		default:
			m_static[ii].SetText(m_pdataX[staticX[ii].dataIdx].val);
			break;
		}
	}
}

void CMapWnd::generateODatas()
{
	for (int ii = 0; ii < m_DataCount; ii++)
	{
		generateOData(ii, true);
		generateOData(ii, false);
	}
}

void CMapWnd::generateOData(int nRow, bool callput)
{
	int nDisplayRow = 0;
	CString sCode, str, sPrv, opt, sIV;
	CfxGrid	*grid = NULL;
	if (callput)
	{
		sCode = m_pStrike[nRow].ccod;
		grid  = m_pCallGrid.get();
		nDisplayRow = nRow + 1;
	}
	else
	{
		sCode = m_pStrike[nRow].pcod;
		grid  = m_pPutGrid.get();
		nDisplayRow = nRow;
	}

	// 지수환산가
	double strike{}, jisuConv{}, k200{};
	strike = getOptionStrike(sCode); if (strike == 0) strike = 1;
	k200 = fabs(atof(m_pdataX[datK200Cur].val));
	jisuConv = fabs(atof(m_pdataX[datKpCur].val)) * strike / k200;

	str.Format("%.2f", floor(jisuConv * 100) / 100);
	sPrv = grid->GetItemText(nDisplayRow, gfJisuConv);
	if (sPrv.CompareNoCase(str) != 0) 
	{
		grid->SetItemText(nDisplayRow, gfJisuConv, str);
		if (m_bBlink) grid->Blink(nDisplayRow, gfJisuConv, m_blinkColor);
	}

	// 등락율
	CString sign(" "), symbol(" "), val;
	double cur{}, dif{};
	val = grid->GetItemText(nDisplayRow, gfCur);
	if (!val.IsEmpty()) val = val.Mid(1);
	cur = atof(val);

	val = grid->GetItemText(nDisplayRow, gfDif);
	if (!val.IsEmpty()) 
	{
		ParseSymbol((LPSTR)(LPCTSTR)val, symbol, sign);
		val = val.Mid(1);
	}
	dif = atof(sign + val);

	if ((cur - dif == 0) || (dif / (cur - dif) * 100 == 0))
	{
		str = " 0";
	}
	else
	{
		str.Format("%.2f%%", dif / (cur - dif) * 100);
		if (str[0] != '-') str = "+" + str;
	}
	sPrv = grid->GetItemText(nDisplayRow, gfRate);
	if (sPrv.CompareNoCase(str) != 0) 
	{
		grid->SetItemText(nDisplayRow, gfRate, str);
		if (m_bBlink) grid->Blink(nDisplayRow, gfRate, m_blinkColor);
	}

	// 2012.12.14 KSJ 매도호가, 매수호가 색상지정
	// (현재가 - 전일비) = 기준가, 매도, 매수 호가가 기준가보다 작으면   -(파랑)이고, 크면 +(빨강), 같으면 검정
	double standard{}, mdga{}, msga{}, dVal{}; //기준가, 매도, 매수
	standard = cur - dif;

	msga = atof(grid->GetItemText(nDisplayRow, gfMsga));
	mdga = atof(grid->GetItemText(nDisplayRow, gfMdga));

	dVal = standard - msga;

// 	CString strTemp;
// 	strTemp.Format("[KSJ] msga[%.2f] mdga[%.2f] dVal[%.2f]", msga, mdga, dVal);
// 	OutputDebugString(strTemp);


// 	if(dVal != 0)
// 	{
// 		grid->SetItemFgColor(nDisplayRow, gfMsga, (dVal < 0) ? clrRED:clrBLUE);
// 	}
// 
// 	dVal = standard - mdga;
// 	
// 	if(dVal != 0)
// 	{
// 		grid->SetItemFgColor(nDisplayRow, gfMdga, (dVal < 0) ? clrRED:clrBLUE);
// 	}
	
// 	CString strTemp;
// 	strTemp.Format("[KSJ] oData[%d] standard[%lf] msga[%lf] mdga[%lf] st-ms[%lf][%s] st-md[%lf][%s]", nDisplayRow, standard, msga, mdga, standard-msga, (standard-msga)>0 ? "PLUS":"MINUS", standard-mdga, (standard-mdga)>0 ? "PLUS":"MINUS");
// 	OutputDebugString(strTemp);
	//2012.12.14 KSJ 끝

	// 민감도 계산(iv, delta, gamma, theta, vegga, rho)
	m_opsensCal->SetJCode(sCode);
	val = grid->GetItemText(nDisplayRow, gfCur);
	if (!val.IsEmpty()) val = val.Mid(1);
	m_opsensCal->SetCurrValue(val);
	str.Format("%.1f", strike);
	m_opsensCal->SetStrike(str);
	m_opsensCal->SetBzRemainDays(m_jjilOrigin);
	m_opsensCal->SetCalRemainDays(m_jjilOrigin);
	m_opsensCal->SetHv(m_hv, m_hv, m_hv, m_hv);
	m_opsensCal->Calc(0);
	sIV.Format("%s", m_opsensCal->GetIv());

//CString s;
//s.Format("IC row:%d, sCode:%s cur:%s strike:%.1f underlying:%srd:%s bdjs:%s cdgl:%s iv:%s\n", 
//	 nDisplayRow, sCode, grid->GetItemText(nDisplayRow, gfCur).Mid(1), strike, m_pdataX[datBasicPrice].val, 
//	 m_pdataX[datJjil].val, m_opsensCal->GetBedangJisu(), m_opsensCal->GetCd(), sIV);
//OutputDebugString(s);

	sPrv = grid->GetItemText(nDisplayRow, gfIV); 
	str = sIV;
	if (sPrv.CompareNoCase(str) != 0) 
	{
		grid->SetItemText(nDisplayRow, gfIV, str);
		if (m_bBlink) grid->Blink(nDisplayRow, gfIV, m_blinkColor);
	}
	
	sPrv = grid->GetItemText(nDisplayRow, gfDelta); str = m_opsensCal->GetDelta();
	if (sPrv.CompareNoCase(str) != 0) 
	{
		grid->SetItemText(nDisplayRow, gfDelta, str);
		if (m_bBlink) grid->Blink(nDisplayRow, gfDelta, m_blinkColor);
	}
	
	sPrv = grid->GetItemText(nDisplayRow, gfGamma); str = m_opsensCal->GetGamma();
	if (sPrv.CompareNoCase(str) != 0) 
	{
		grid->SetItemText(nDisplayRow, gfGamma, str);
		if (m_bBlink) grid->Blink(nDisplayRow, gfGamma, m_blinkColor);
	}
	
	sPrv = grid->GetItemText(nDisplayRow, gfTheta); str = m_opsensCal->GetTheta();
	if (sPrv.CompareNoCase(str) != 0) 
	{
		grid->SetItemText(nDisplayRow, gfTheta, str);
		if (m_bBlink) grid->Blink(nDisplayRow, gfTheta, m_blinkColor);
	}

	sPrv = grid->GetItemText(nDisplayRow, gfVegga); str = m_opsensCal->GetVegga();
	if (sPrv.CompareNoCase(str) != 0) 
	{
		grid->SetItemText(nDisplayRow, gfVegga, str);
		if (m_bBlink) grid->Blink(nDisplayRow, gfVegga, m_blinkColor);
	}
}

void CMapWnd::insertRow(int row)
{
	if (row >= m_pCallGrid->GetRowCount())
		row = m_pCallGrid->GetRowCount();

	m_pCallGrid->InsertRow("", row);
	m_pPutGrid->InsertRow("", row);

	LOGFONT lf; m_font->GetLogFont(&lf); lf.lfHeight = 90;
	GVITEM	gvitem;

	const COLORREF clrGridFgColor = GetIndexColor(GRID_TXT_COLOR);
	const int hdr_count = sizeof(gridHdr) / sizeof(struct _gridHdr);
	for (int ii = 0; ii < hdr_count; ii++)
	{
		gvitem.row   = row;
		gvitem.col   = ii;
		gvitem.state = 0;
		gvitem.mask  = GVMK_FORMAT|GVMK_FONT|GVMK_ATTR|GVMK_FGCOLOR|GVMK_BKCOLOR|GVMK_SYMBOL;

		gvitem.format= gridHdr[ii].format;
		gvitem.symbol= gridHdr[ii].symbol; 
		gvitem.attr  = gridHdr[ii].attrx;
		gvitem.fgcol = clrGridFgColor;
		gvitem.bkcol = GetIndexColor(32);

		CopyMemory(&gvitem.font, &lf, sizeof(LOGFONT));
		m_pCallGrid->SetItem(&gvitem);
		m_pPutGrid->SetItem(&gvitem);
	}

	m_pCallGrid->SetRowHeight(row, rowH);
	m_pPutGrid->SetRowHeight(row, rowH);
}

void CMapWnd::moveGrid(int step)
{
	int nRow, nCol;
	m_pCallGrid->GetTopLeftNonFixedCell(nRow, nCol);

	
	if (step < 0)
	{
		if (nRow <= m_pCallGrid->GetFixedRowCount())
		{
			syncGridTopRow();
			return;
		}
		m_pCallGrid->MessageToGrid(WM_VSCROLL, SB_LINEUP);
		syncGridTopRow();
	}
	else if (step > 0)
	{
		if (nRow >= m_pCallGrid->GetRowCount() - m_pCallGrid->GetFixedRowCount())
		{
			syncGridTopRow();
			return;
		}
		m_pCallGrid->MessageToGrid(WM_VSCROLL, SB_LINEDOWN);
		syncGridTopRow();		
	}
}

void CMapWnd::syncGridTopRow()
{
	int nCol, nCallRow, nPutRow;
	m_pCallGrid->GetTopLeftNonFixedCell(nCallRow, nCol);
	m_pPutGrid->GetTopLeftNonFixedCell(nPutRow, nCol); nPutRow++;

	int nMove{}, nDirection{};
	nMove = abs(nCallRow - nPutRow);
	nDirection = (nCallRow - nPutRow  > 0) ? SB_LINEDOWN : SB_LINEUP;
	
	while (nMove > 0)
	{
		m_pPutGrid->MessageToGrid(WM_VSCROLL, nDirection);
		nMove--;
	}
}

void CMapWnd::showMingamField(BOOL showMingam)
{	
	int ii = 0;
	for (ii = gfMsga; ii <= gfMDif; ii++)
	{
		if (showMingam)
		{
			m_pCallGrid->SetColumnWidth(ii, 0);
			m_pPutGrid->SetColumnWidth(ii, 0);
		}
		else
		{
			m_pCallGrid->SetColumnWidth(ii, gridHdr[ii].width);
			m_pPutGrid->SetColumnWidth(ii, gridHdr[ii].width);
		}
	}

	for (ii = gfIV; ii <= gfVegga; ii++)
	{
		if (showMingam)
		{
			m_pCallGrid->SetColumnWidth(ii, gridHdr[ii].width);
			m_pPutGrid->SetColumnWidth(ii, gridHdr[ii].width);
		}
		else
		{
			m_pCallGrid->SetColumnWidth(ii, 0);
			m_pPutGrid->SetColumnWidth(ii, 0);
		}
	}

	m_pCallGrid->Invalidate();
	m_pPutGrid->Invalidate();
}

void CMapWnd::showSiKoJeField(int showSiKoJe)	//2013.10.16 KSJ 시고저 보여주기
{
	int ii = 0;
	for (ii = gfRate; ii <= gfMdga; ii++)
	{
		if (showSiKoJe == 0) //호가
		{
			m_pCallGrid->SetColumnWidth(ii, gridHdr[ii].width);
			m_pPutGrid->SetColumnWidth(ii, gridHdr[ii].width);
		}
		else //시고저, Greeks
		{
			m_pCallGrid->SetColumnWidth(ii, 0);
			m_pPutGrid->SetColumnWidth(ii, 0);
		}
	}
	
	for (ii = gfSiga; ii <= gfJega; ii++)
	{
		if (showSiKoJe == 0 || showSiKoJe == 2) //호가, Greeks
		{
			m_pCallGrid->SetColumnWidth(ii, 0);
			m_pPutGrid->SetColumnWidth(ii, 0);
		}
		else if(showSiKoJe == 1)	//시고저
		{
			m_pCallGrid->SetColumnWidth(ii, gridHdr[ii].width);
			m_pPutGrid->SetColumnWidth(ii, gridHdr[ii].width);
		}
	}

	for (ii = gfGvol; ii <= gfMDif; ii++)
	{
		if (showSiKoJe == 2)
		{
			m_pCallGrid->SetColumnWidth(ii, 0);
			m_pPutGrid->SetColumnWidth(ii, 0);
		}
		else
		{
			m_pCallGrid->SetColumnWidth(ii, gridHdr[ii].width);
			m_pPutGrid->SetColumnWidth(ii, gridHdr[ii].width);
		}
	}

	for (ii = gfIV; ii <= gfVegga; ii++)
	{
		if (showSiKoJe == 2)
		{
			m_pCallGrid->SetColumnWidth(ii, gridHdr[ii].width);
			m_pPutGrid->SetColumnWidth(ii, gridHdr[ii].width);
		}
		else
		{
			m_pCallGrid->SetColumnWidth(ii, 0);
			m_pPutGrid->SetColumnWidth(ii, 0);
		}
	}

	if(showSiKoJe == 2 || showSiKoJe == 0)
	{
		m_pCallGrid->SetColumnWidth(gfRate, gridHdr[gfRate].width);
		m_pPutGrid->SetColumnWidth(gfRate, gridHdr[gfRate].width);
	}
	else
	{
		m_pCallGrid->SetColumnWidth(gfRate, 0);
		m_pPutGrid->SetColumnWidth(gfRate, 0);
	}

	m_pCallGrid->Invalidate();
	m_pPutGrid->Invalidate();
}

double CMapWnd::getOptionStrike(CString opCode)
{
	double strike;

	strike = atof(opCode.Mid(5, 3));
	if (opCode[7] == '2' || opCode[7] == '7')
		strike = strike + 0.5;

	return strike;
}

double CMapWnd::getAtmStrike(double k200)
{
	int k;
	k = int(k200 * 100);
	if (k % 250 > 124) 
		k = k - k % 250 + 250;
	else
		k = k - k % 250;

	return k / 100;
}

int CMapWnd::GetMinTime(CString time)
{
	if (time.GetLength() != 4)
		return 0;
	return atoi(time.Left(2)) * 60 + atoi(time.Mid(2, 2));
}

void CMapWnd::OnClickUp()
{
	moveGrid(-1);
}

void CMapWnd::OnClickDown()
{
	moveGrid(+1);
	m_pPutGrid->Invalidate();
}

void CMapWnd::OnClickGreek()
{
	m_bShowMingam = m_bShowMingam ? FALSE : TRUE;
	showMingamField(m_bShowMingam);
//	SetGridColumnWidth();
}

void CMapWnd::OnClickMingam()
{
	if (m_cfg->ShowDlg())
		sendDataTr();
}


void CMapWnd::SetPalette()
{
	LoadRoundBitmap();
	m_clrBack = GetIndexColor(clBack);
	m_clrBack = GetIndexColor(64);
//	m_pBorder->Init(ssGRADIENT, 0, "", "", GetIndexColor(66), GetIndexColor(66),
//		GetSysColor(COLOR_BTNTEXT), alLEFT);

	m_pBorder->SetBkColor(GetIndexColor(66));
	m_pBorder->SetCornerRound(m_hRoundBitmap);
	m_pContent->SetBkColor(GetIndexColor(181));
	m_pContent->SetCornerRound(m_hRoundBitmap);

	initGrid(m_pCallGrid.get(), true);
	initGrid(m_pPutGrid.get(), false);
	Invalidate();
}

void CMapWnd::OnRButtonDown(UINT nFlags, CPoint point) 
{	
	CPoint pointx; GetCursorPos(&pointx);

	m_mouseDat.Format("IB401500\n%d\t%d\n", pointx.x, pointx.y);
	m_pParent->PostMessage(WM_USER, MAKEWPARAM(mouseDLL, 0), (long)m_mouseDat.operator LPCTSTR());

	CWnd::OnRButtonDown(nFlags, point);
}

void CMapWnd::OnCallGridRMouseDown(NMHDR* pNMHDR, LRESULT* pResult)
{
	const NM_GVITEM *vitem = (NM_GVITEM*)pNMHDR;

	CString code("");
	if (pNMHDR->idFrom == ID_CALLGRID)
	{
		if (vitem->row > 0)
			code = m_pStrike[vitem->row-1].ccod;
	}
	else
		code = m_pStrike[vitem->row].pcod;

	CPoint pointx; GetCursorPos(&pointx);
	if (code.IsEmpty())
		m_mouseDat.Format("IB401500\n%d\t%d\n", pointx.x, pointx.y);
	else
		m_mouseDat.Format("IB401500\n%d\t%d\n%s\t%s\n", pointx.x, pointx.y, "40301", code);
	m_pParent->PostMessage(WM_USER, MAKEWPARAM(mouseDLL, 0), (long)m_mouseDat.operator LPCTSTR());

	*pResult = 0;
}

void CMapWnd::OnPutGridRMouseDown(NMHDR* pNMHDR, LRESULT* pResult)
{
	const NM_GVITEM *vitem = (NM_GVITEM*)pNMHDR;

	CString code = m_pStrike[vitem->row].pcod;
	CPoint pointx; GetCursorPos(&pointx);

	m_mouseDat.Format("IB401500\n%d\t%d\n%s\t%s\n", pointx.x, pointx.y, "40301", code);
	m_pParent->PostMessage(WM_USER, MAKEWPARAM(mouseDLL, 0), (long)m_mouseDat.operator LPCTSTR());

	*pResult = 0;
}

void CMapWnd::OnGridLMouseUp(NMHDR* pNMHDR, LRESULT* pResult)
{
	const NM_GVITEM *vitem = (NM_GVITEM*)pNMHDR;

	CString code("");
	
	if (pNMHDR->idFrom == ID_CALLGRID)
	{
		if (vitem->row > 0)
			code = m_pStrike[vitem->row-1].ccod;
	}
	else
	{
		code = m_pStrike[vitem->row].pcod;
	}

	if (!code.IsEmpty())
	{
		CString param;
		param.Format("40301\t%s", code);	Variant(triggerCC, param);
		param.Format("ed_focod\t%s", code);	Variant(triggerCC, param);
	}

	*pResult = 0;
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

bool CMapWnd::GetBlinkInfo()
{
	char	wb[128];
	CString path;
	path.Format("%s\\%s\\%s", m_sRoot, TABDIR, PALETTE);
	GetPrivateProfileString(GENERALSN, FLASHSN, "", wb, sizeof(wb), path);
	m_blinkColor = GetIndexColor(atoi(wb));
	
	path.Format("%s\\%s\\%s\\%s", m_sRoot, USRDIR, Variant(nameCC, ""), SETUP_FILE);
	GetPrivateProfileString("SCREEN", "UNFLESH", "", wb, sizeof(wb), path);

	return atoi(wb) == 0;
}

/* 현재 사용하지 않는 코드 escdream 2022.07.08
UINT ParsingRTM(LPVOID lpvoid)
{
	CMapWnd* pMain = (CMapWnd *)lpvoid;
	if (pMain == NULL)
		return FALSE;

	char* pBytes = NULL;

	while (pMain->m_bThreadRTM)
	{
		pBytes = NULL;

		if (pMain->m_arBytesRTM.GetUpperBound() < 0)	
		{
			pMain->WaitRTM();
			continue;
		}

		pMain->m_syncRTM.Lock();		// Lock

		pBytes = pMain->m_arBytesRTM.GetAt(0);
		pMain->m_arBytesRTM.RemoveAt(0);

		pMain->m_syncRTM.Unlock();		// Unlock

		pMain->doRTM(pBytes);

		if (pBytes != NULL)
		{	
			delete [] pBytes;
			pBytes = NULL;
		}
	}

	return 0;
}

bool CMapWnd::WaitRTM()
{
	if (!m_pThreadRTM)
		return false;
	const DWORD res = WaitForSingleObject(m_hEventRTM, 3000);
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
*/

/* 현재 사용하지 않는 코드 escdream 2022.07.08
void CMapWnd::doRTM(char *pBytes)
{
	if (m_bRTMReady)
	{
// 		parseAlert(pBytes);
	}
}
*/

/* 현재 사용하지 않는 코드 escdream 2022.07.08
void CMapWnd::initRTM()
{
	if (m_bThreadRTM)
		return;

	m_bThreadRTM = true;
	m_pThreadRTM = AfxBeginThread((AFX_THREADPROC) ParsingRTM, (LPVOID) this, THREAD_PRIORITY_NORMAL);
	m_hEventRTM = ::CreateEvent(NULL, FALSE, FALSE, NULL);	
}
*/
/* 현재 사용하지 않는 코드 escdream 2022.07.08
void CMapWnd::stopRTM()
{
	if (!m_bThreadRTM)
		return;
	
	m_bThreadRTM = false;
	SetEvent(m_hEventRTM);
	m_pThreadRTM->PostThreadMessage(WM_QUIT, 0, 0);
	switch (WaitForSingleObject(m_pThreadRTM->m_hThread, 1000))
	{
	case WAIT_OBJECT_0:
	case WAIT_FAILED:
	case WAIT_ABANDONED:
		break;
	case WAIT_TIMEOUT:
		TerminateThread(m_pThreadRTM->m_hThread, 0);
		break;
	}
	removeRTM();
}
*/

/* 현재 사용하지 않는 코드 escdream 2022.07.08
void CMapWnd::removeRTM()
{
	m_syncRTM.Lock();

	char* pBytes = NULL;

	for (int ii = m_arBytesRTM.GetUpperBound(); ii >= 0; ii--)
	{
		pBytes = m_arBytesRTM.GetAt(ii);
		delete [] pBytes;	
		pBytes = NULL;
	}
	m_arBytesRTM.RemoveAll();

	m_syncRTM.Unlock();
}
*/

void CMapWnd::OnTimer(UINT nIDEvent) 
{
	if (nIDEvent == THREAD_TIMER_KEY)
		SetEvent(m_hEventRTM);	

	CWnd::OnTimer(nIDEvent);
}

void CMapWnd::ChangeFont(int ifontsize, LPCTSTR lszfont)
{
	if(!m_bCreated) return;
	if(m_iCurFont == ifontsize) return;

	
	const double ldif = float(ifontsize - 9);
	const int iWndWidth = mapWidth + (int)(m_lSizeRate[0] * (float)mapWidth * ldif);
	const int iWndHeight = mapHeight + (int)(m_lSizeRate[1] * (float)mapHeight * ldif);

	m_iCurFont = ifontsize;
	m_bChangeFont = TRUE;
	SetWindowPos(NULL, 0, 0, iWndWidth, iWndHeight, SWP_NOZORDER | SWP_NOMOVE);
	m_pParent->SendMessage(WM_USER, MAKEWPARAM(sizeDLL, 0), MAKELPARAM(iWndWidth, iWndHeight) );
	
}

void CMapWnd::OnSize(UINT nType, int cx, int cy) 
{
	CWnd::OnSize(nType, cx, cy);
	// TODO: Add your message handler code here
	if(!m_bCreated) return;
	m_pClientRc = CRect(0, 0, cx, cy);
	if( m_iOldSize[0] == 0 && m_iOldSize[1] == 0)
	{
		m_iOldSize[0] = cx;
		m_iOldSize[1] = cy;
		return;
	}
//	if(m_iOldSize[0] == cx && m_iOldSize[1] == cy) return;
	ResizeControl(cx, cy);
}


void CMapWnd::ResizeControl(int cx, int cy)
{
	const double lcalcrate[2] = {( (float)cx - (float)m_iOldSize[0] ) / (float)m_iOldSize[0], 
							( (float)cy - (float)m_iOldSize[1] ) /(float)m_iOldSize[1]};
	CRect rc, rc2, mvrc1, mvrc2;
	GetClientRect(rc2);
	m_pCallGrid->GetWindowRect(&rc);
	ScreenToClient(&rc);

	const int ixgap = HORI_GAP + HORI_GAP;
	const int iygap = VERT_GAP + VERT_GAP;

//	int itop = borderH;// + gridHeaderH;
	const int itop = 50+TOPANDBODY_SPACE + VERT_GAP+VERT_GAP;
	const int ibotgap = BOT_GAP + VERT_GAP;
	const int ih = cy - itop - VERT_GAP;
	const int idiv = ih / 2;

	const int igridh = (m_DataCount+1) * rowH;
	mvrc1 = CRect(rc.left, rc.top, cx - ixgap, rc.top + idiv);

	
	int ilastw = 0;
	if(m_DataCount > 10 && mvrc1.Height() >  igridh) 
	{
		m_iScrollw = 14;
//		mvrc1.bottom = mvrc1.top + igridh;

	//	m_pCallGrid->SetColumnWidth(icol, gridHdr[icol].width  + iscrollw);
	//	m_pPutGrid->SetColumnWidth(icol, gridHdr[icol].width + iscrollw);
	}
	else
	{
		m_iScrollw = 0;
	//	m_pCallGrid->SetColumnWidth(icol, ilastw);
	//	m_pPutGrid->SetColumnWidth(icol, ilastw );
	}

	mvrc2 = CRect(rc.left, mvrc1.bottom+1, cx - ixgap, cy-TOPANDBODY_SPACE-FRAME_SPACE);

	m_pCallGrid->MoveWindow(mvrc1); 
	m_pPutGrid->MoveWindow(mvrc2);

	int iw = 0, ilastcol = 0;
	ilastw = 0;
	int iwidth = 0;

	m_pBorder->GetWindowRect(&rc);
	ScreenToClient(&rc);
	rc.right = cx - FRAME_SPACE ;
	m_pBorder->MoveWindow(rc);
	rc.top = rc.bottom +TOPANDBODY_SPACE;
	rc.bottom = cy - VERT_GAP;
	m_pContent->MoveWindow(rc);

	CRect btnrc1{}, btnrc2{};
	const int igap = 1;
// 	m_pGreekBtn->GetWindowRect(&btnrc1);
// 	ScreenToClient(&btnrc1);
	iwidth = btnrc1.Width() + igap; 
	m_pMingamBtn->GetWindowRect(&btnrc2);
	ScreenToClient(&btnrc2);
	iwidth += (btnrc2.Width() + igap);
	const int ileft = mvrc1.right - iwidth;

	iw = btnrc1.Width(); 
	btnrc1.left = ileft;
	btnrc1.right = btnrc1.left + iw;
// 	m_pGreekBtn->MoveWindow(&btnrc1);

	iw = btnrc2.Width(); 
	btnrc2.left = btnrc1.right + igap;
	btnrc2.right = btnrc2.left + iw;
	m_pMingamBtn->MoveWindow(&btnrc2);

	iwidth = 0;
	ilastcol = -1, ilastw = 0;

	m_pUpBtn->GetWindowRect(&rc);
	ScreenToClient(&rc);

	mvrc1.right = mvrc1.left -1;
	mvrc2.right = mvrc2.left -1;

	mvrc2.left = mvrc1.left = ixgap;
	mvrc1.top = mvrc1.top + gridHeaderH;
	m_pCallTitle->MoveWindow(mvrc1);
	m_pPutTitle->MoveWindow(mvrc2);

	
	mvrc1.top = mvrc1.bottom - rc.Height(); 
	mvrc2.bottom = mvrc2.top + rc.Height(); 

// 	CString strTemp;
// 	strTemp.Format("[KSJ] rc[%d] mvrc[%d] mvrc2[%d, %d, %d, %d]", rc.Height(), mvrc1.Height(), mvrc2.left, mvrc2.top, mvrc2.right, mvrc2.bottom);
// 	OutputDebugString(strTemp);

	m_pUpBtn->MoveWindow(mvrc1);
	m_pDownBtn->MoveWindow(mvrc2);

	m_bChangeFont = TRUE;
	SetGridColumnWidth();
}

void CMapWnd::SetGridColumnWidth()
{
	CRect rc;
	m_pCallGrid->GetWindowRect(&rc);
	ScreenToClient(&rc);

	const double lgridw = (float)(rc.Width()-m_iScrollw);
	int iw = 0;
	for(int i=0; i<m_pCallGrid->GetColumnCount(); i++)
	{
		iw = m_pCallGrid->GetColumnWidth(i); 
		if(iw <= 0) continue;

		iw = (int)(g_lrate[i] * lgridw);

		m_pCallGrid->SetColumnWidth(i, iw); 
		m_pPutGrid->SetColumnWidth(i, iw);
	}
}

void CMapWnd::DrawBitmap(CDC* pDC, CRect rc, HBITMAP hBitmap)
{
	HDC hDC = ::CreateCompatibleDC (NULL);
	SelectObject (hDC, hBitmap); //icon
	::BitBlt( pDC->m_hDC, rc.left, rc.top, rc.Width(), rc.Height(),	hDC, 0, 0, SRCCOPY);
	::DeleteDC (hDC);
}

void CMapWnd::DrawCornerRoundBitmap(CDC* pDC, CRect rc, int iType)
{
	const int iPoint = 5;
	int idx = 0;
	if(iType) idx = 4;

	DrawBitmap(pDC, CRect(rc.left, rc.top, rc.left+iPoint, rc.top +iPoint), m_hRoundBitmap[idx++]);
	DrawBitmap(pDC, CRect(rc.right-iPoint, rc.top, rc.right, rc.top +iPoint), m_hRoundBitmap[idx++]);
	DrawBitmap(pDC, CRect(rc.left, rc.bottom-iPoint, rc.left+iPoint, rc.bottom), m_hRoundBitmap[idx++]);
	DrawBitmap(pDC, CRect(rc.right-iPoint, rc.bottom-iPoint, rc.right, rc.bottom), m_hRoundBitmap[idx++]);
}