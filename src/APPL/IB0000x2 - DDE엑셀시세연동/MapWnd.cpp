// MapWnd.cpp : implementation file
//

#include "stdafx.h"
#include "afxtempl.h"
#include "IB0000X2.h"
#include "MapWnd.h"
#include "excel9.h"

#include "../../h/axisvar.h"
#include "../../h/axisfire.h"
#include "../../h/jmcode.h"
#include "../../h/ledger.h"

#include "../../control/fx_misc/fxStatic.h"
#include "../../control/fx_misc/fxImgButton.h"
#include "../../control/fx_misc/fxEdit.h"
#include "AccountCtrl.h"

struct s_gmid {
    char    func[1];        /* Function 구분    */
							/* 'F' : 시작       */
    char    tmcd[4];        /* 테마코드     */
    char    find[20];       /* 검색         */
}*mid;
#define L_sgmid	sizeof(struct s_gmid)				// Length of stock input

#define KEY_THEMELIST		100
#define KEY_THEMENAME		200
#define KEY_REMAIN			0x01

#define CTRL_TOP_POS	11
#define CTRL_BOTTOM_POS	27
#define COMBO_DATASIZE	200

// 컨트롤 크기
// 캡션과 컨트롤간 간격은 5로 한다. (fxStatic 내부마진 2px)
// 컨트롤 그룹간 간격은 5로 한다.
#define CZ_ALL_GRP1		CRect(125, CTRL_TOP_POS-14, 470, CTRL_BOTTOM_POS+4)
#define CZ_ALL_KOSPI	CRect(121, CTRL_TOP_POS, 181 , CTRL_BOTTOM_POS)
#define CZ_ALL_KOSDAQ	CRect(181, CTRL_TOP_POS, 240 , CTRL_BOTTOM_POS)
#define CZ_ALL_ELW		CRect(240, CTRL_TOP_POS, 285 , CTRL_BOTTOM_POS)
#define CZ_ALL_FUTURE   CRect(285, CTRL_TOP_POS, 330 , CTRL_BOTTOM_POS)
#define CZ_ALL_OPTION	CRect(330, CTRL_TOP_POS, 375 , CTRL_BOTTOM_POS)
#define CZ_ALL_INDEX	CRect(375, CTRL_TOP_POS, 420 , CTRL_BOTTOM_POS)
#define CZ_ALL_PROGRAM	CRect(420, CTRL_TOP_POS, 490 , CTRL_BOTTOM_POS)
#define CZ_ALL_TUJAJA	CRect(490, CTRL_TOP_POS, 545 , CTRL_BOTTOM_POS)

#define CZ_ALL_CODELIST		CRect(7,42,255,230)
#define CZ_ELW_CODELIST		CRect(7,42,255,202)

#define CZ_THEME_LABEL	CRect(131, CTRL_TOP_POS-2, 163 , CTRL_BOTTOM_POS+1)
#define CZ_THEME_SEARCH	CRect(170, CTRL_TOP_POS-2, 250 , CTRL_BOTTOM_POS+1)
#define CZ_THEME_LIST	CRect(260, CTRL_TOP_POS-3, 468 , CTRL_BOTTOM_POS+COMBO_DATASIZE)

#define CZ_REMAINDER_LABEL	  CRect(131, CTRL_TOP_POS, 190 , CTRL_BOTTOM_POS)
#define CZ_REMAINDER_NUMBER	  CRect(190, CTRL_TOP_POS-2, 350 , CTRL_BOTTOM_POS+2)
#define CZ_REMAINDER_NAME	  CRect(355, CTRL_TOP_POS-1, 450 , CTRL_BOTTOM_POS+2)
#define CZ_REMAINDER_PASSWORD CRect(455, CTRL_TOP_POS-2, 515 , CTRL_BOTTOM_POS+2)

#define CZ_INTEREST_LABEL	  CRect(145, CTRL_TOP_POS, 245 , CTRL_BOTTOM_POS)
#define CZ_INTEREST_GROUPLIST CRect(250, CTRL_TOP_POS-3, 360 , CTRL_BOTTOM_POS+COMBO_DATASIZE)

#define CZ_KRX_LABEL	CRect(131, CTRL_TOP_POS, 160 , CTRL_BOTTOM_POS)
#define CZ_KRX_LIST		CRect(180, CTRL_TOP_POS, 240 , CTRL_BOTTOM_POS+COMBO_DATASIZE)
#define CZ_KRX_NAME		CRect(260, CTRL_TOP_POS, 320 , CTRL_BOTTOM_POS)

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define	wMAP	560
#define	hMAP	468

#define MAX_ITEM_COUNT 50
#define MAX_UP 27
#define MAX_K200 29
#define MAX_KOQ 46
#define MAX_K100 4
#define MAX_SECTOR 27
#define MAX_FREE 28
#define MAX_IETC 18

#define	F_POINT		9
#define	F_NAME		"굴림체"
#define	S_TITLE		"[엑셀시세연계]"

#define	IDC_SEL_COMBOLIST	01001

#define	IDC_THEME_LABEL		01011
#define	IDC_THEME_SEARCH	01012
#define	IDC_THEME_LIST		01013

#define	IDC_REMAINDER_LABEL		01021
#define	IDC_REMAINDER_NUMBER	01022
#define	IDC_REMAINDER_NAME		01023
#define	IDC_REMAINDER_PASSWORD	01024

#define	IDC_INTEREST_LABEL		01031
#define	IDC_INTEREST_GROUPLIST	01032

#define	IDC_KRX_LABEL		01041
#define	IDC_KRX_GROUPLIST	01042
#define	IDC_KRX_NAME		01043

#define	IDC_NAME_LABEL		01051
#define	IDC_NAME_SEARCH		01052


// Color Index Define
#define CLR_FORM_BACK		64
#define CLR_PANNEL_BACK		66
#define CLR_PANNEL_LINE		67
#define	CLR_STATIC_BACK		78
#define	CLR_STATIC_FORE		69
#define	CLR_STATIC_LINE		65
#define CLR_TAB_FORE		70
#define CLR_TAB_SELFORE		71
#define CLR_GRID_TEXT		69
#define CLR_GRID_BACK		68
#define CLR_GRID_LINE		75
#define CLR_GRID_HBACK		74
#define CLR_GRID_HTEXT		76
#define CLR_GRID_SBACK		78
#define CLR_GRID_STEXT		69
#define CLR_DATA_UP			94
#define CLR_DATA_DOWN		95
#define CLR_SYMBOL_BACK		160
#define CLR_SYMBOL_FORE		69
#define CLR_SYMBOL_LINE		89
#define CLR_GUIDE_BACK		66
#define CLR_GUIDE_FORE		69
#define CLR_GUIDE_LINE		75

#define	IDC_CODE			10001
#define	IDC_ITEM			10002
#define	IDC_SCODE			10003
#define	IDC_SITEM			10004
#define	IDC_RTYPE			10005
#define	IDC_BSTOCK			10006
#define	IDC_ICORP			10007
#define	IDC_MONTH			10008
#define	IDC_GRP1			10010
#define	IDC_KOSPI			10011
#define	IDC_KOSDAQ			10012
#define	IDC_FUTURE			10013
#define	IDC_OPTION			10014
#define	IDC_INDEX			10015
#define	IDC_ELW				10016
#define IDC_PROGRAM			10017
#define IDC_TUJAJA			10018
#define	IDC_COMM			10021
#define	IDC_CODE_ADD_ALL	10022
#define	IDC_CODE_DEL_ALL	10023
#define	IDC_CODE_ADD		10024
#define	IDC_CODE_DEL		10025
#define	IDC_ITEM_ADD_ALL	10026
#define	IDC_ITEM_DEL_ALL	10027
#define	IDC_ITEM_ADD		10028
#define	IDC_ITEM_DEL		10029
#define	IDC_GRP2		10030
#define	IDC_VERT		10031
#define	IDC_HORZ		10032
#define	IDC_EXCEL		10033
#define	IDC_LABEL		10034
#define	IDC_SEARCH		10035
#define	IDC_NUMBER		10036

#define	IDC_TUJATYPE1	10037
#define	IDC_TUJATYPE2	10038

#define	clFore		69
#define	codeKEY		"SCODE"

CListCtrl*	listCtrl;
int CALLBACK compare(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort);

struct	_listR	{
	char	code[12];
	char	name[50];	//2016.08.24 KSJ 종목명 길이 32 -> 50
};

#define	HANGULMSK	0x80
#define	ChosungS	0xA4A1	// ㄱ
#define	ChosungE	0xA4BE	// ㅎ


/////////////////////////////////////////////////////////////////////////////
// CMapWnd

CMapWnd::CMapWnd(CWnd* parent)
{
	m_parent = parent;

	m_code      = NULL;
	m_item      = NULL;
	m_scode     = NULL;
	m_sitem     = NULL;
	m_rtype     = NULL;
	m_bstock    = NULL;
	m_icorp     = NULL;
	m_month     = NULL;

	m_tujatype1 = NULL;
	m_tujatype2 = NULL;

	m_pGrp1      = NULL;
	m_pKospi     = NULL;
	m_pKosdaq    = NULL;
	m_pFuture    = NULL;
	m_pOption    = NULL;
	m_pIndex     = NULL;
	m_pElw	    = NULL;

	m_pThemeLabel	= NULL;
	m_pThemeSearch	= NULL;		
	m_pThemeList	= NULL;	

	m_pRemainderLabel	= NULL;
	m_pRemainderAcc		= NULL;	
	m_pRemainderName	= NULL;	
	m_pRemainderPassword	= NULL;
	m_pWizard			= NULL;
	m_strAccount        = "";
	m_strPassword       = "";

	m_pInterestLabel		= NULL;
	m_pInterestGroupList	= NULL;	

	m_pKRXLabel	= NULL;
	m_pKRXList	= NULL;	
	m_pKRXName	= NULL;

	m_code_addA = NULL;
	m_code_delA = NULL;
	m_code_add  = NULL; 
	m_code_del  = NULL;

	m_item_addA = NULL;
	m_item_delA = NULL;
	m_item_add  = NULL;
	m_item_del  = NULL;

	m_grp2      = NULL;
	m_horz      = NULL;	
	m_vert      = NULL;
	m_number	= NULL;
	m_bCheck	= false;

	m_excel     = NULL;

	m_type      = cKospi;
	m_clrBG		= GetIndexColor(BACKINDEXCOLOR);

	m_pNameLabel	= NULL;
	m_pNameSearch	= NULL;
	m_bAddAll		= true;
	
	m_listitem.clear();//.RemoveAll();
	m_searchlist.clear();//.RemoveAll();

	inter = std::make_unique<InterMon>(m_parent, this);
}

CMapWnd::~CMapWnd()
{
}


BEGIN_MESSAGE_MAP(CMapWnd, CWnd)
	//{{AFX_MSG_MAP(CMapWnd)
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_WM_PAINT()
	ON_WM_SETFOCUS() 
	ON_CBN_SELCHANGE(IDC_SEL_COMBOLIST,  OnSelChangeSearchCombo)
	ON_CBN_SELCHANGE(IDC_THEME_LIST,	 OnSelChangeThemeList)
	ON_CBN_SELCHANGE(IDC_INTEREST_GROUPLIST, OnSelChangeGroupList)
	ON_CBN_SELCHANGE(IDC_KRX_GROUPLIST,  OnSelChangeKRXTypeList)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_USER, OnMessage)
	ON_MESSAGE(WM_USER+1, OnUser)
	ON_MESSAGE(WM_EDITEX, OnEditExMsg)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMapWnd message handlers

int CMapWnd::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	m_root = (char *)variant(homeCC);
	m_sTabDir.Format("%s\\%s\\", m_root, TABDIR);

	m_font = getFont(F_POINT);
	variant(titleCC, S_TITLE);

	create();
	
	loadItemKey();

	loadCode();
	loadItem();
	loadJCode();

	m_parent->SendMessage(WM_USER, MAKEWPARAM(ddeDLL, 0));
	m_br.CreateSolidBrush(m_clrRoundColor);
	return 0;
}


void CMapWnd::OnDestroy() 
{
	removeAllItems();
/*
	if (m_code)			delete m_code;
	if (m_item)			delete m_item;
	if (m_scode)		delete m_scode;
	if (m_sitem)		delete m_sitem;
	if (m_rtype)		delete m_rtype;
	if (m_bstock)		delete m_bstock;
	if (m_icorp)		delete m_icorp;
	if (m_month)		delete m_month;

	if (m_tujatype1)	delete m_tujatype1;
	if (m_tujatype2)	delete m_tujatype2;

	if (m_pGrp1)		delete m_pGrp1;
	if (m_pKospi)		delete m_pKospi;
	if (m_pKosdaq)		delete m_pKosdaq;
	if (m_pFuture)		delete m_pFuture;
	if (m_pOption)		delete m_pOption;
	if (m_pIndex)		delete m_pIndex;
	if (m_pElw)			delete m_pElw;
	if (m_pProgram)		delete m_pProgram;
	if (m_pTujaja)		delete m_pTujaja;

	if (m_pThemeLabel)	delete m_pThemeLabel;
	if (m_pThemeSearch)	delete m_pThemeSearch;
	if (m_pThemeList)	delete m_pThemeList;

	if (m_pRemainderLabel)		delete m_pRemainderLabel;
	if (m_pRemainderAcc)		delete m_pRemainderAcc;
	if (m_pRemainderName)		delete m_pRemainderName;
	if (m_pRemainderPassword)	delete m_pRemainderPassword;
	if (m_pWizard)				delete m_pWizard;

	if (m_pInterestLabel)		delete m_pInterestLabel;
	if (m_pInterestGroupList)	delete m_pInterestGroupList;

	if (m_pKRXLabel)	delete m_pKRXLabel;
	if (m_pKRXList)		delete m_pKRXList;
	if (m_pKRXName)		delete m_pKRXName;

	if (m_code_addA)	delete m_code_addA;
	if (m_code_delA)	delete m_code_delA;
	if (m_code_add)		delete m_code_add;
	if (m_code_del)		delete m_code_del;

	if (m_item_addA)	delete m_item_addA;
	if (m_item_delA)	delete m_item_delA;
	if (m_item_add)		delete m_item_add;
	if (m_item_del)		delete m_item_del;

	if (m_grp2)			delete m_grp2;
	if (m_horz)			delete m_horz;
	if (m_vert)			delete m_vert;
	if (m_number)		delete m_number;

	if (m_excel)		delete m_excel;

	if (m_pNameLabel)	delete m_pNameLabel;
	if (m_pNameSearch)	delete m_pNameSearch;
*/

	CWnd::OnDestroy();
}

void CMapWnd::ChangeColors()
{
	m_strPal = GetPalette();

	m_clrRoundColor	= GetAxColor(TOOLINDEXCOLOR);
	m_clrBG = GetAxColor(BACKINDEXCOLOR);

	GetDlgItem(IDC_KOSPI)->Invalidate();
	GetDlgItem(IDC_KOSDAQ)->Invalidate();
	GetDlgItem(IDC_ELW)->Invalidate();
	GetDlgItem(IDC_FUTURE)->Invalidate();
	GetDlgItem(IDC_OPTION)->Invalidate();
	GetDlgItem(IDC_INDEX)->Invalidate();
	GetDlgItem(IDC_HORZ)->Invalidate();
	GetDlgItem(IDC_VERT)->Invalidate();
	GetDlgItem(IDC_NUMBER)->Invalidate();
	
	if(m_pThemeLabel)
		m_pThemeLabel->SetBkColor(m_clrRoundColor);
	if(m_pRemainderLabel)
		m_pRemainderLabel->SetBkColor(m_clrRoundColor);
	if(m_pInterestLabel)
		m_pInterestLabel->SetBkColor(m_clrRoundColor);
	if(m_pNameLabel)
		m_pNameLabel->SetBkColor(m_clrRoundColor);

	Invalidate();
}

CString CMapWnd::GetPalette()
{
	char buff[10];
	CString strPal;
	CString file;
	file.Format("%s\\%s\\%s", m_root, "tab", "palette.ini");
	
	GetPrivateProfileString("General", "Palette", "", buff, sizeof(buff), file);
	strPal = buff;
	
	return strPal;
}

void CMapWnd::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	CRect clipRC;
	CRect ClientRect;
	CRect ToolBarRoundRect;
	CRect RoundRect;
	COLORREF clrRoundBG{}; // List Round 바탕 색상
	const int nSizeGap = 1;

	m_clrRoundColor = GetAxColor(TOOLINDEXCOLOR);
	m_hRoundColor = CreateSolidBrush(m_clrRoundColor);

	dc.GetClipBox(&clipRC);
	dc.FillSolidRect(clipRC, m_clrBG);
	clrRoundBG = GetIndexColor(68);

	// 1번째 Round 설정
	GetClientRect(&ToolBarRoundRect);
	ToolBarRoundRect.top += 3;
	ToolBarRoundRect.left += nSizeGap;
	ToolBarRoundRect.right -= nSizeGap;
	ToolBarRoundRect.bottom = ToolBarRoundRect.top + 29;

	dc.FillSolidRect(ToolBarRoundRect, m_clrRoundColor); //toolbar rect
	DrawRoundBitmap(&dc, ToolBarRoundRect, ROUND_PANEL);

	// 2번째 Round 설정
	m_code->GetWindowRect(ClientRect);
	ScreenToClient(ClientRect);
	RoundRect.top = ClientRect.top - CTRL_GAP;
	RoundRect.left = ClientRect.left - CTRL_GAP;
	m_code_addA->GetWindowRect(ClientRect);
	ScreenToClient(ClientRect);
	RoundRect.bottom = ClientRect.bottom + 4;
	m_item->GetWindowRect(ClientRect);
	ScreenToClient(ClientRect);
	RoundRect.right = ClientRect.right + CTRL_GAP;

	dc.FillSolidRect(RoundRect, clrRoundBG); 
	DrawRoundBitmap(&dc, RoundRect, ROUND_CONTENTS);

	// 3번째 Round 설정
	m_scode->GetWindowRect(ClientRect);
	ScreenToClient(ClientRect);
	RoundRect.top = ClientRect.top - CTRL_GAP;
	RoundRect.left = ClientRect.left - CTRL_GAP;
	m_sitem->GetWindowRect(ClientRect);
	ScreenToClient(ClientRect);
	RoundRect.bottom = ClientRect.bottom + CTRL_GAP;
	RoundRect.right = ClientRect.right + CTRL_GAP;
	
	dc.FillSolidRect(RoundRect, clrRoundBG); 
	DrawRoundBitmap(&dc, RoundRect, ROUND_CONTENTS);

	// 4번째 Round 설정
	m_grp2->GetWindowRect(ClientRect);
	ScreenToClient(ClientRect);
	RoundRect.top = ClientRect.top + 3;
	RoundRect.left = ClientRect.left - CTRL_GAP;
	m_excel->GetWindowRect(ClientRect);
	ScreenToClient(ClientRect);
	RoundRect.bottom = ClientRect.bottom + CTRL_GAP;
	RoundRect.right = ClientRect.right + CTRL_GAP;
	
	dc.FillSolidRect(RoundRect, m_clrRoundColor); 
	DrawRoundBitmap(&dc, RoundRect, ROUND_CONTENTS);
}

void CMapWnd::DrawRoundBitmap(CDC *pDC, CRect rc, int nKind)
{
	CRect bitRc;
	
	switch(nKind)
	{
	case ROUND_PANEL:
		//left + top
		bitRc.SetRect(rc.left, rc.top, rc.left + 5, rc.top + 5);
		DrawBitmap(pDC, bitRc, "axpanel" + m_strPal + "0_lt.bmp");
		
		//right + top
		bitRc.SetRect(rc.right - 5, rc.top, rc.right, rc.top + 5);
		DrawBitmap(pDC, bitRc, "axpanel" + m_strPal + "0_rt.bmp");
		
		//left + bottom
		bitRc.SetRect(rc.left, rc.bottom - 5, rc.left + 5, rc.bottom);
		DrawBitmap(pDC, bitRc, "axpanel" + m_strPal + "0_lb.bmp");
		
		//right + bottom
		bitRc.SetRect(rc.right - 5, rc.bottom - 5, rc.right, rc.bottom);
		DrawBitmap(pDC, bitRc, "axpanel" + m_strPal + "0_rb.bmp");
		break;
	case ROUND_CONTENTS:
		//left + top
		bitRc.SetRect(rc.left, rc.top, rc.left + 5, rc.top + 5);
		DrawBitmap(pDC, bitRc, "axpanel" + m_strPal + "1_lt.bmp");
		
		//right + top
		bitRc.SetRect(rc.right - 5, rc.top, rc.right, rc.top + 5);
		DrawBitmap(pDC, bitRc, "axpanel" + m_strPal + "1_rt.bmp");
		
		//left + bottom
		bitRc.SetRect(rc.left, rc.bottom - 5, rc.left + 5, rc.bottom);
		DrawBitmap(pDC, bitRc, "axpanel" + m_strPal + "1_lb.bmp");
		
		//right + bottom
		bitRc.SetRect(rc.right - 5, rc.bottom - 5, rc.right, rc.bottom);
		DrawBitmap(pDC, bitRc, "axpanel" + m_strPal + "1_rb.bmp");
		break;
	}
}

void CMapWnd::DrawBitmap(CDC *pDC, CRect rc, CString strImage)
{
	CString strPath = Variant(homeCC, "");
	CString sImageDir = strPath + "\\image\\" + strImage;
	
	HBITMAP BitMap = (HBITMAP)::LoadImage(AfxGetInstanceHandle(), sImageDir, IMAGE_BITMAP,0,0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
	CDC		memDC;
	
	memDC.CreateCompatibleDC(pDC);
	
	if (memDC.m_hDC != NULL) {
		memDC.SelectObject(BitMap);
		::TransparentBlt(pDC->m_hDC, rc.left, rc.top, rc.Width(), rc.Height(), memDC.m_hDC, 0, 0, rc.Width(), rc.Height(), 0xFF00FF);
	}
	
	DeleteObject(BitMap);
	memDC.DeleteDC();
}

void CMapWnd::OnSetFocus(CWnd* pOldWnd) 
{
	CWnd::OnSetFocus(pOldWnd);
	m_code->SetFocus();	
}

BOOL CMapWnd::OnCommand(WPARAM wParam, LPARAM lParam) 
{
	int		flag_sw_stock=0, flag_sw_elw=0, flag_sw_tuja=0;
	switch (LOWORD(wParam))
	{
	case IDC_KOSPI:
	case IDC_KOSDAQ:
	case IDC_FUTURE:
	case IDC_OPTION:
	case IDC_INDEX:
	case IDC_ELW:
	case IDC_PROGRAM:
	case IDC_TUJAJA:
		if (HIWORD(wParam) != BN_CLICKED)
			break;

		m_type = (ctype)(LOWORD(wParam) - IDC_KOSPI);
		switch (m_type)
		{
		case cFutures:
		case cOption:
		case cIndex:
		case cProgram:
			m_code->MoveWindow(CZ_ALL_CODELIST);
			flag_sw_stock = SW_HIDE;
			flag_sw_elw   = SW_HIDE;
			flag_sw_tuja  = SW_HIDE;
			break;
		case cTujaja:
			m_code->MoveWindow(CZ_ELW_CODELIST);
			flag_sw_stock = SW_HIDE;
			flag_sw_elw   = SW_HIDE;
			flag_sw_tuja  = SW_SHOW;
			settujaCombo();
			break;
		case cKospi:
		case cKosdaq:
			m_code->MoveWindow(CZ_ELW_CODELIST);
			flag_sw_stock = SW_SHOW;
			flag_sw_elw   = SW_HIDE;
			flag_sw_tuja  = SW_HIDE;
			break;
		case cElw:
		default:
			m_code->MoveWindow(CZ_ELW_CODELIST);
			if (m_type == cElw)
			{
				setElwCombo();
				flag_sw_stock = SW_HIDE;
				flag_sw_elw   = SW_SHOW;
				flag_sw_tuja  = SW_HIDE;
			}
			else
			{
				flag_sw_stock = SW_SHOW;
				flag_sw_elw   = SW_HIDE;
				flag_sw_tuja  = SW_HIDE;
			}
			break;
		}
		if (m_rtype)
		{
			m_rtype->ShowWindow(flag_sw_elw);
			m_bstock->ShowWindow(flag_sw_elw);
			m_icorp->ShowWindow(flag_sw_elw);
			m_month->ShowWindow(flag_sw_elw);
		}
		if (m_tujatype1)
		{
			m_tujatype1->ShowWindow(flag_sw_tuja);
			removeAllItems();
		}
		if (m_tujatype2)
		{
			m_tujatype2->ShowWindow(SW_HIDE);
		}

		m_pNameLabel->ShowWindow(flag_sw_stock);
		m_pNameSearch->ShowWindow(flag_sw_stock);
		m_pNameSearch->SetWindowText("");	//기존 검색이름 삭제
		
		loadCode();
		loadItem();
		break;

	case IDC_TUJATYPE1:
		if (HIWORD(wParam) == CBN_SELCHANGE)
			loadTUJA();
		break;
	case IDC_TUJATYPE2:
		if (HIWORD(wParam) == CBN_SELCHANGE)
			loadCode();
		break;
	case IDC_RTYPE:
	case IDC_BSTOCK:
	case IDC_ICORP:
	case IDC_MONTH:
		if (HIWORD(wParam) == CBN_SELCHANGE)
			loadCode();
		break;

	case IDC_CODE_ADD_ALL:
		if( checkAddItem( m_code.get(), m_scode.get(), TRUE ) )
			addItem(m_code.get(), m_scode.get(), true, true);
		break;

	case IDC_CODE_DEL_ALL:
		deleteItem(m_scode.get(), true);
		break;

	case IDC_CODE_ADD:
		if( checkAddItem( m_code.get(), m_scode.get(), FALSE ) )
			addItem(m_code.get(), m_scode.get(), true);
		break;

	case IDC_CODE_DEL:
		deleteItem(m_scode.get());
		break;

	case IDC_ITEM_ADD_ALL:
		addItem(m_item.get(), m_sitem.get(), false, true);
		break;

	case IDC_ITEM_DEL_ALL:
		deleteItem(m_sitem.get(), true);
		break;

	case IDC_ITEM_ADD:
		addItem(m_item.get(), m_sitem.get(), false);
		break;

	case IDC_ITEM_DEL:
		deleteItem(m_sitem.get());
		break;

	case IDC_EXCEL:
		runExcel();
		break;

	case IDC_SEARCH:
		if (HIWORD(wParam) == EN_CHANGE)
			search();
		break;

	case  IDC_NUMBER:	//2013.01.18 KSJ 셀서식 숫자변환
		{
			m_bCheck = !m_bCheck;
			((CButton *)m_number.get())->SetCheck(m_bCheck);
			
			CString strFile, strCheck;
			strFile.Format("%s/%s/dde_item.ini", m_root, TABDIR);
			
			strCheck.Format("%d", m_bCheck ? 1:0);
			WritePrivateProfileString("Check", "Check", strCheck, strFile);
		}
		break;
	default:
		break;
	}
	
	return CWnd::OnCommand(wParam, lParam);
}

BOOL CMapWnd::OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult) 
{
	NMHDR* pnmh = (NMHDR *)lParam;
	if (pnmh->code == NM_DBLCLK)
	{
		switch (pnmh->idFrom)
		{
		case IDC_CODE:	addItem(m_code.get(), m_scode.get(), true);		break;
		case IDC_ITEM:	addItem(m_item.get(), m_sitem.get(), false);	break;
		case IDC_SCODE:	deleteItem(m_scode.get());			break;
		case IDC_SITEM:	deleteItem(m_sitem.get());			break;
		}
	}
	else if (m_code && pnmh->hwndFrom == m_code->GetSafeHwnd() && pnmh->code == LVN_COLUMNCLICK)
	{
		CRect	rc;
		CSize	size;

		NMLISTVIEW* pnmv = (NMLISTVIEW *)lParam;
		m_code->SortItems(compare, pnmv->iSubItem);

		m_code->SetItemState(0, LVIS_SELECTED|LVIS_FOCUSED, LVIS_SELECTED|LVIS_FOCUSED);
		m_code->GetItemRect(0, rc, LVIR_LABEL);
		size.cx = 0;
		size.cy = rc.Height() * (0 - m_code->GetTopIndex());
		
		m_code->Scroll(size);
	}

	if (wParam == IDC_REMAINDER_PASSWORD)
		OnRemain();

	return CWnd::OnNotify(wParam, lParam, pResult);
}

BOOL CMapWnd::CreateMap()
{
	return CWnd::Create(NULL, NULL, WS_CHILD|WS_VISIBLE|WS_CLIPCHILDREN, CRect(0, 0, wMAP, hMAP), m_parent, 0);
}

void CMapWnd::create()
{
	m_clrRoundColor = GetAxColor(TOOLINDEXCOLOR);
	m_hRoundColor = CreateSolidBrush(m_clrRoundColor);
	m_strPal      = GetPalette();

	// list control
	m_code =std::make_unique<CListCtrl>();
	m_code->Create(WS_VISIBLE|WS_BORDER|WS_CHILD|WS_VSCROLL|LVS_ALIGNTOP|LVS_REPORT|LVS_SHOWSELALWAYS,
				   CZ_ELW_CODELIST, this, IDC_CODE);
	m_code->SetExtendedStyle(LVS_EX_FULLROWSELECT);
	m_code->SetFont(m_font, TRUE);
	addColumn(m_code.get(), _T("종목"), 0, 40);
	addColumn(m_code.get(), _T("종목명"), 1, 110);
	listCtrl = m_code.get();

	m_item = std::make_unique<CListCtrl>();
	m_item->Create(WS_VISIBLE|WS_BORDER|WS_CHILD|LVS_ALIGNTOP|LVS_REPORT|LVS_SHOWSELALWAYS, 
				   CRect(260,42,553,228), this, IDC_ITEM);
	m_item->SetExtendedStyle(LVS_EX_FULLROWSELECT);
	m_item->SetFont(m_font, TRUE);
	addColumn(m_item.get(), _T("항목"), 0, 100);
	addColumn(m_item.get(), _T("Excel 설정"), 1, 100);

	m_scode = std::make_unique<CListCtrl>();
	m_scode->Create(WS_VISIBLE|WS_BORDER|WS_CHILD|LVS_ALIGNTOP|LVS_REPORT,
				    CRect(7,266,255,416), this, IDC_SCODE);
	m_scode->SetExtendedStyle(LVS_EX_FULLROWSELECT);
	m_scode->SetFont(m_font, TRUE);
	addColumn(m_scode.get(), _T("종목"), 0, 40);
	addColumn(m_scode.get(), _T("종목명"), 1, 110);

	m_sitem = std::make_unique<CListCtrl>();
	m_sitem->Create(WS_VISIBLE|WS_BORDER|WS_CHILD|LVS_ALIGNTOP|LVS_REPORT, 
					CRect(260,266,553,416), this, IDC_SITEM);
	m_sitem->SetExtendedStyle(LVS_EX_FULLROWSELECT);
	m_sitem->SetFont(m_font, TRUE);
	addColumn(m_sitem.get(), _T("항목"), 0, 100);
	addColumn(m_sitem.get(), _T("Excel 설정"), 1, 100);

	m_pSelList = std::make_unique<CComboBox>();
	m_pSelList->Create(WS_VISIBLE|WS_CHILD|WS_GROUP| WS_CLIPSIBLINGS | WS_VSCROLL | CBS_DROPDOWNLIST | BS_GROUPBOX, 
					   CRect(8, 8, 120, 8+COMBO_DATASIZE), this, IDC_SEL_COMBOLIST);
	m_pSelList->SetFont(m_font, TRUE);
	m_pSelList->AddString(STR_COMBODATA_ALL);
	m_pSelList->AddString(STR_COMBODATA_THEME);
	m_pSelList->AddString(STR_COMBODATA_REMAINDER);
	m_pSelList->AddString(STR_COMBODATA_INTEREST);	
	SetWindowTheme(m_pSelList->GetSafeHwnd(), L"", L"");
	

//	m_pSelList->AddString("KRX섹터");
	m_pSelList->SetCurSel(0);

 	CreateSearchCtrl();
	setSearchName();
	OnSelChangeSearchCombo();

	const int nButtonTop= 232;
	const int nButtonBottom = 252;
	const int nBButtonLen = 72;
	const int nSButtonLen = 48;
	// code add/del button
	// Gap = 2
	m_code_addA = std::make_unique<CfxImgButton>();
	m_code_addA->Create(_T("전체추가"), CRect(9,nButtonTop,9+nBButtonLen,nButtonBottom), this, IDC_CODE_ADD_ALL);
	m_code_addA->SetImgBitmap(getBitmap("5BTN.bmp"), getBitmap("5BTN_DN.bmp"), getBitmap("5BTN_EN.bmp"));
//	m_code_addA->SetTextColor(getColor(clFore));
	m_code_addA->SetFont(m_font, TRUE);

	m_code_delA = std::make_unique<CfxImgButton>();
	m_code_delA->Create(_T("전체삭제"), CRect(83,nButtonTop,83+nBButtonLen,nButtonBottom), this, IDC_CODE_DEL_ALL);
	m_code_delA->SetImgBitmap(getBitmap("5BTN.bmp"), getBitmap("5BTN_DN.bmp"), getBitmap("5BTN_EN.bmp"));
//	m_code_delA->SetTextColor(getColor(clFore));
	m_code_delA->SetFont(m_font, TRUE);

	m_code_add = std::make_unique<CfxImgButton>();
	m_code_add->Create(_T("추가"), CRect(157,nButtonTop,157+nSButtonLen,nButtonBottom), this, IDC_CODE_ADD);
	m_code_add->SetImgBitmap(getBitmap("3BTN.bmp"), getBitmap("3BTN_DN.bmp"), getBitmap("3BTN_EN.bmp"));
//	m_code_add->SetTextColor(getColor(clFore));
	m_code_add->SetFont(m_font, TRUE);

	m_code_del = std::make_unique<CfxImgButton>();
	m_code_del->Create(_T("삭제"), CRect(207,nButtonTop,207+nSButtonLen,nButtonBottom), this, IDC_CODE_DEL);
	m_code_del->SetImgBitmap(getBitmap("3BTN.bmp"), getBitmap("3BTN_DN.bmp"), getBitmap("3BTN_EN.bmp"));
//	m_code_del->SetTextColor(getColor(clFore));
	m_code_del->SetFont(m_font, TRUE);

	// item add/del button
	m_item_addA = std::make_unique<CfxImgButton>();
	m_item_addA->Create(_T("전체추가"), CRect(283,nButtonTop,283+nBButtonLen,nButtonBottom), this, IDC_ITEM_ADD_ALL);
	m_item_addA->SetImgBitmap(getBitmap("5BTN.bmp"), getBitmap("5BTN_DN.bmp"), getBitmap("5BTN_EN.bmp"));
//	m_item_addA->SetTextColor(getColor(clFore));
	m_item_addA->SetFont(m_font, TRUE);

	m_item_delA = std::make_unique<CfxImgButton>();
	m_item_delA->Create(_T("전체삭제"), CRect(357,nButtonTop,357+nBButtonLen,nButtonBottom), this, IDC_ITEM_DEL_ALL);
	m_item_delA->SetImgBitmap(getBitmap("5BTN.bmp"), getBitmap("5BTN_DN.bmp"), getBitmap("5BTN_EN.bmp"));
//	m_item_delA->SetTextColor(getColor(clFore));
	m_item_delA->SetFont(m_font, TRUE);

	m_item_add = std::make_unique<CfxImgButton>();
	m_item_add->Create(_T("추가"), CRect(431,nButtonTop,431+nSButtonLen,nButtonBottom), this, IDC_ITEM_ADD);
	m_item_add->SetImgBitmap(getBitmap("3BTN.bmp"), getBitmap("3BTN_DN.bmp"), getBitmap("3BTN_EN.bmp"));
//	m_item_add->SetTextColor(getColor(clFore));
	m_item_add->SetFont(m_font, TRUE);

	m_item_del = std::make_unique<CfxImgButton>();
	m_item_del->Create(_T("삭제"), CRect(481,nButtonTop,481+nSButtonLen,nButtonBottom), this, IDC_ITEM_DEL);
	m_item_del->SetImgBitmap(getBitmap("3BTN.bmp"), getBitmap("3BTN_DN.bmp"), getBitmap("3BTN_EN.bmp"));
//	m_item_del->SetTextColor(getColor(clFore));
	m_item_del->SetFont(m_font, TRUE);

	// radio button for display
	m_grp2 = std::make_unique<CButton>();
	m_grp2->Create("", WS_VISIBLE|WS_CHILD|WS_GROUP|BS_GROUPBOX, 
				   CRect(7,423,188,458), this, IDC_GRP2);
	m_grp2->ModifyStyleEx(0, WS_EX_TRANSPARENT);
	m_grp2->ShowWindow(SW_HIDE);

	m_horz = std::make_unique<CButton>();
	m_horz->Create(_T("가로 항목"), WS_VISIBLE|WS_CHILD|BS_AUTORADIOBUTTON, 
				   CRect(5,435,80,452), this, IDC_HORZ);
	m_horz->SetFont(m_font, TRUE);

	m_vert = std::make_unique<CButton>();
	m_vert->Create(_T("세로 항목"), WS_VISIBLE|WS_CHILD|BS_AUTORADIOBUTTON, 
				   CRect(85,435,160,452), this, IDC_VERT);
	m_vert->SetFont(m_font, TRUE);
	((CButton *)m_vert.get())->SetCheck(1);

	CString strFile;
	strFile.Format("%s/%s/dde_item.ini", m_root, TABDIR);

	char szBuf[255];
	GetPrivateProfileString("Check", "Check", "0", szBuf, sizeof(szBuf), strFile);
	m_bCheck = atoi(szBuf) == 1 ? true:false;

	m_number = std::make_unique<CButton>();
	m_number->Create(_T("1000단위 구분기호(,) 사용 (예: 1234 ⇔ 1,234)"), WS_VISIBLE|WS_CHILD|BS_CHECKBOX, 
		CRect(165,435,452,452), this, IDC_NUMBER);
	m_number->SetFont(m_font, TRUE);
	((CButton *)m_number.get())->SetCheck(m_bCheck);

	// excel
	m_excel = std::make_unique<CfxImgButton>();
	m_excel->Create(_T("Excel 전송"), CRect(456,433,552,453), this, IDC_EXCEL);
	m_excel->SetImgBitmap(getBitmap("7BTN.bmp"), getBitmap("7BTN_DN.bmp"), getBitmap("7BTN_EN.bmp"));
//	m_excel->SetTextColor(getColor(clFore));
	m_excel->SetFont(m_font, TRUE);

	m_code->SetFocus();
}

void CMapWnd::CreateSearchCtrl()
{
	CreateSearchCtrl_All();
	CreateSearchCtrl_Theme();
	CreateSearchCtrl_Remainder();
	CreateSearchCtrl_Interest();
	CreateSearchCtrl_KRX();
}

void CMapWnd::CreateSearchCtrl_All()
{
	// radio button for type
// 	m_pGrp1 = std::make_unique<CButton>();
// 	m_pGrp1->Create("", WS_VISIBLE|WS_CHILD|WS_GROUP|BS_GROUPBOX, 
// 					CZ_ALL_GRP1, this, IDC_GRP1);
// 	m_pGrp1->ModifyStyleEx(0, WS_EX_TRANSPARENT);
	
	m_pKospi = std::make_unique<CButton>();
	m_pKospi->Create(_T("거래소"), WS_VISIBLE|WS_CHILD|BS_AUTORADIOBUTTON, 
					 CZ_ALL_KOSPI, this, IDC_KOSPI);
	m_pKospi->SetFont(m_font, TRUE);
	m_pKospi->SetCheck(1);
	
	m_pKosdaq = std::make_unique<CButton>();
	m_pKosdaq->Create(_T("코스닥"), WS_VISIBLE|WS_CHILD|BS_AUTORADIOBUTTON, 
					  CZ_ALL_KOSDAQ, this, IDC_KOSDAQ);
	m_pKosdaq->SetFont(m_font, TRUE);
	
	m_pElw = std::make_unique<CButton>();
	m_pElw->Create(_T("ELW"), WS_VISIBLE|WS_CHILD|BS_AUTORADIOBUTTON, 
				   CZ_ALL_ELW, this, IDC_ELW);
	m_pElw->SetFont(m_font, TRUE);
	
	m_pFuture = std::make_unique<CButton>();
	m_pFuture->Create(_T("선물"), WS_VISIBLE|WS_CHILD|BS_AUTORADIOBUTTON, 
					  CZ_ALL_FUTURE, this, IDC_FUTURE);
	m_pFuture->SetFont(m_font, TRUE);
	
	m_pOption = std::make_unique<CButton>();
	m_pOption->Create(_T("옵션"), WS_VISIBLE|WS_CHILD|BS_AUTORADIOBUTTON, 
					  CZ_ALL_OPTION, this, IDC_OPTION);
	m_pOption->SetFont(m_font, TRUE);
	
	m_pIndex = std::make_unique<CButton>();
	m_pIndex->Create(_T("업종"), WS_VISIBLE|WS_CHILD|BS_AUTORADIOBUTTON, 
					 CZ_ALL_INDEX, this, IDC_INDEX);
	m_pIndex->SetFont(m_font, TRUE);

	m_pProgram = std::make_unique<CButton>();
	m_pProgram->Create(_T("프로그램"), WS_VISIBLE|WS_CHILD|BS_AUTORADIOBUTTON, 
		CZ_ALL_PROGRAM, this, IDC_PROGRAM);
	m_pProgram->SetFont(m_font, TRUE);

	m_pTujaja = std::make_unique<CButton>();
	m_pTujaja->Create(_T("투자자"), WS_VISIBLE|WS_CHILD|BS_AUTORADIOBUTTON, 
		CZ_ALL_TUJAJA, this, IDC_TUJAJA);
	m_pTujaja->SetFont(m_font, TRUE);
}

void CMapWnd::CreateSearchCtrl_Theme()
{
	m_pThemeLabel = std::make_unique<CfxStatic>();
	m_pThemeLabel->Create(_T("검색"), WS_VISIBLE|WS_CHILD|BS_AUTORADIOBUTTON, 
		                  CZ_THEME_LABEL, this, IDC_THEME_LABEL);
	m_pThemeLabel->SetFont(m_font, TRUE);
	m_pThemeLabel->SetBkColor(m_clrRoundColor);

	m_pThemeSearch = std::make_unique<CEditEx>();
	m_pThemeSearch->Create(WS_BORDER|WS_CHILD|WS_VISIBLE|ES_AUTOHSCROLL, 
		                   CZ_THEME_SEARCH, this,  IDC_THEME_SEARCH);
	m_pThemeSearch->SetFont(m_font, TRUE);

	m_pThemeList = std::make_unique<CComboBox>();
	m_pThemeList->Create(WS_VISIBLE|WS_CHILD|WS_GROUP|BS_GROUPBOX|WS_VSCROLL, 
		                 CZ_THEME_LIST, this, IDC_THEME_LIST);
	m_pThemeList->SetFont(m_font, TRUE);
	SetWindowTheme(m_pThemeList->GetSafeHwnd(), L"", L"");
}

void CMapWnd::CreateSearchCtrl_Remainder()
{
	m_pRemainderLabel = std::make_unique<CfxStatic>();
	m_pRemainderLabel->Create(_T("계좌번호"), WS_VISIBLE|WS_CHILD|BS_AUTORADIOBUTTON, 
		                      CZ_REMAINDER_LABEL, this, IDC_REMAINDER_LABEL);
	m_pRemainderLabel->SetFont(m_font, TRUE);
	m_pRemainderLabel->SetBkColor(m_clrRoundColor);

	m_pRemainderAcc = std::make_unique<CAccountCtrl>(m_parent, this, m_root);
	m_pRemainderAcc->CreateEx(WS_EX_TOPMOST, NULL, "ACCN", WS_VISIBLE | WS_CHILD | WS_TABSTOP, 
		                         CZ_REMAINDER_NUMBER, this, IDC_REMAINDER_NUMBER);
	m_pRemainderAcc->createAccountCtrl("AN10", 255, GetIndexColor(CLR_PANNEL_BACK));
	m_pRemainderAcc->SetFont(m_font, TRUE);

	m_pRemainderName = std::make_unique<CStatic>();
	m_pRemainderName->Create(_T(""), WS_CHILD | WS_VISIBLE | SS_LEFT | SS_CENTERIMAGE | SS_SUNKEN, 
							  CZ_REMAINDER_NAME, this, IDC_REMAINDER_NAME);
	m_pRemainderName->SetFont(m_font, TRUE);

	m_pRemainderPassword = std::make_unique<CfxEdit>();
	m_pRemainderPassword->Create(WS_CHILD | WS_VISIBLE | WS_TABSTOP | ES_PASSWORD, 
								  CZ_REMAINDER_PASSWORD, this, IDC_REMAINDER_PASSWORD);
	m_pRemainderPassword->LimitText(8);
	m_pRemainderPassword->SetFont(m_font, TRUE);
}

void CMapWnd::CreateSearchCtrl_Interest()
{
	m_pInterestLabel = std::make_unique<CfxStatic>();
	m_pInterestLabel->Create(_T("관심종목그룹명"), WS_VISIBLE|WS_CHILD|BS_AUTORADIOBUTTON, 
		                     CZ_INTEREST_LABEL, this, IDC_INTEREST_LABEL);
	m_pInterestLabel->SetFont(m_font, TRUE);
	m_pInterestLabel->SetBkColor(m_clrRoundColor);
	
	m_pInterestGroupList = std::make_unique<CComboBox>();
	m_pInterestGroupList->Create(WS_VISIBLE|WS_CHILD|WS_GROUP|BS_GROUPBOX|BS_GROUPBOX, 
		                         CZ_INTEREST_GROUPLIST, this, IDC_INTEREST_GROUPLIST);
					 
	m_pInterestGroupList->SetFont(m_font, TRUE);
	SetWindowTheme(m_pInterestGroupList->GetSafeHwnd(), L"", L"");
	
}

void CMapWnd::CreateSearchCtrl_KRX()
{
// 	m_pKRXLabel = std::make_unique<CfxStatic>();
// 	m_pKRXLabel->Create(_T("업종"), WS_VISIBLE|WS_CHILD|BS_AUTORADIOBUTTON, 
// 		                CZ_KRX_LABEL, this, IDC_KRX_LABEL);
// 	m_pKRXLabel->SetFont(m_font, TRUE);
// 	
// 	m_pKRXList = std::make_unique<CComboBox>();
// 	m_pKRXList->Create(WS_VISIBLE|WS_CHILD|WS_GROUP|BS_GROUPBOX|BS_GROUPBOX, 
// 		               CZ_KRX_LIST, this, IDC_KRX_GROUPLIST);
// 	m_pKRXList->SetFont(m_font, TRUE);
// 
// 	m_pKRXName = std::make_unique<CfxStatic>();
// 	m_pKRXName->Create(_T(""), WS_VISIBLE|WS_CHILD|BS_AUTORADIOBUTTON|SS_SUNKEN, 
// 		               CZ_KRX_NAME, this, IDC_KRX_NAME);
// 	m_pKRXName->SetFont(m_font, TRUE);
}

void CMapWnd::CtrlAllHidden()
{
	if(m_pGrp1)		m_pGrp1->ShowWindow(SW_HIDE);
	if(m_pKospi)	m_pKospi->ShowWindow(SW_HIDE);
	if(m_pKosdaq)	m_pKosdaq->ShowWindow(SW_HIDE);
	if(m_pElw)		m_pElw->ShowWindow(SW_HIDE);
	if(m_pFuture)	m_pFuture->ShowWindow(SW_HIDE);
	if(m_pOption)	m_pOption->ShowWindow(SW_HIDE);
	if(m_pIndex)	m_pIndex->ShowWindow(SW_HIDE);
	if(m_pProgram)	m_pProgram->ShowWindow(SW_HIDE);
	if(m_pTujaja)	m_pTujaja->ShowWindow(SW_HIDE);

	if(m_pThemeLabel)	m_pThemeLabel->ShowWindow(SW_HIDE);
	if(m_pThemeSearch)	m_pThemeSearch->ShowWindow(SW_HIDE);
	if(m_pThemeList)	m_pThemeList->ShowWindow(SW_HIDE);

	if(m_pRemainderLabel)		m_pRemainderLabel->ShowWindow(SW_HIDE);
	if(m_pRemainderAcc)			m_pRemainderAcc->ShowWindow(SW_HIDE);
	if(m_pRemainderName)		m_pRemainderName->ShowWindow(SW_HIDE);
	if(m_pRemainderPassword)	m_pRemainderPassword->ShowWindow(SW_HIDE);

	if(m_pInterestLabel)		m_pInterestLabel->ShowWindow(SW_HIDE);
	if(m_pInterestGroupList)	m_pInterestGroupList->ShowWindow(SW_HIDE);

	if(m_pNameLabel)	m_pNameLabel->ShowWindow(SW_HIDE);
	if(m_pNameSearch)	m_pNameSearch->ShowWindow(SW_HIDE);

// 	if(m_pKRXLabel)		m_pKRXLabel->ShowWindow(SW_HIDE);
// 	if(m_pKRXList)		m_pKRXList->ShowWindow(SW_HIDE);
// 	if(m_pKRXName)		m_pKRXName->ShowWindow(SW_HIDE);
}

void CMapWnd::CtrlAllShow()
{
	if(m_pGrp1)		m_pGrp1->ShowWindow(SW_SHOW);
	if(m_pKospi)	m_pKospi->ShowWindow(SW_SHOW);
	if(m_pKosdaq)	m_pKosdaq->ShowWindow(SW_SHOW);
	if(m_pElw)		m_pElw->ShowWindow(SW_SHOW);
	if(m_pElw && m_pElw->GetCheck() == 1)
	{
		m_code->MoveWindow(CZ_ELW_CODELIST);
		if(m_rtype->GetSafeHwnd())
		{
			m_rtype->ShowWindow(SW_SHOW);
			m_bstock->ShowWindow(SW_SHOW);
			m_icorp->ShowWindow(SW_SHOW);
			m_month->ShowWindow(SW_SHOW);
		}
	}
	else if((m_pKosdaq && m_pKosdaq->GetCheck() == 1 )|| (m_pKospi && m_pKospi->GetCheck() == 1))
	{
		m_code->MoveWindow(CZ_ELW_CODELIST);
		if(m_pNameLabel)	m_pNameLabel->ShowWindow(SW_SHOW);
		if(m_pNameSearch)	m_pNameSearch->ShowWindow(SW_SHOW);
	}
	else
	{
		m_code->MoveWindow(CZ_ALL_CODELIST);
		if(m_pNameLabel)	m_pNameLabel->ShowWindow(SW_HIDE);
		if(m_pNameSearch)	m_pNameSearch->ShowWindow(SW_HIDE);
	}

	if(m_pFuture)	m_pFuture->ShowWindow(SW_SHOW);
	if(m_pOption)	m_pOption->ShowWindow(SW_SHOW);
	if(m_pIndex)	m_pIndex->ShowWindow(SW_SHOW);
	if(m_pProgram)	m_pProgram->ShowWindow(SW_SHOW);
	if(m_pTujaja)	m_pTujaja->ShowWindow(SW_SHOW);

	//2016.08.24 KSJ 콤보박스를 선택하면 항목을 선택된것으로 변경

	//cKospi=0, cKosdaq, cFutures, cOption, cIndex, cElw, cProgram, cTujaja
	if(m_pKospi && m_pKospi->GetCheck())		m_type = cKospi;
	else if(m_pKosdaq && m_pKosdaq->GetCheck())	m_type = cKosdaq;
	else if(m_pElw && m_pElw->GetCheck())		m_type = cElw;
	else if(m_pFuture && m_pFuture->GetCheck())	m_type = cFutures;
	else if(m_pOption && m_pOption->GetCheck())	m_type = cOption;
	else if(m_pIndex && m_pIndex->GetCheck())	m_type = cIndex;
	else if(m_pProgram && m_pProgram->GetCheck())	m_type = cProgram;
	else if(m_pTujaja && m_pTujaja->GetCheck())	m_type = cTujaja;

	loadItem();
}

void CMapWnd::CtrlThemeShow()
{
	CtrlAllHidden();
	if(m_pThemeLabel)	m_pThemeLabel->ShowWindow(SW_SHOW);
	if(m_pThemeSearch)	m_pThemeSearch->ShowWindow(SW_SHOW);
	if(m_pThemeList)	m_pThemeList->ShowWindow(SW_SHOW);

	if(m_pThemeList)
		SelThemeList();
}

void CMapWnd::CtrlRemainderShow()
{
	CtrlAllHidden();
	if(m_pRemainderLabel)	m_pRemainderLabel->ShowWindow(SW_SHOW);
	if(m_pRemainderAcc)		m_pRemainderAcc->ShowWindow(SW_SHOW);
	if(m_pRemainderName)	m_pRemainderName->ShowWindow(SW_SHOW);
	if(m_pRemainderPassword)	
	{
		m_pRemainderPassword->ShowWindow(SW_SHOW);
		m_pRemainderPassword->SetWindowText("");
	}
}

void CMapWnd::CtrlInterestShow()
{
	CtrlAllHidden();
	if(m_pInterestLabel)		m_pInterestLabel->ShowWindow(SW_SHOW);
	if(m_pInterestGroupList)	m_pInterestGroupList->ShowWindow(SW_SHOW);
}

void CMapWnd::SetInterest()
{

	inter->Request_GroupList();


	int	idx=0;	int	readL = 0;
	char	readB[256] = _T("");
	CString	string, gno, gname, filePath;
	CString sName;
	sName = Variant(nameCC, "");
	m_pInterestGroupList->ResetContent();
	
	filePath.Format("%s/%s/%s/%s", m_root, USRDIR, sName, tempFILE);
	
	CFileFind finder;
	if (!finder.FindFile(filePath))	
		filePath.Format("%s/%s/%s/%s", m_root, USRDIR, sName, saveFILE);
	
	readL = GetPrivateProfileString(_T("GROUPORDER"), "00", "", readB, sizeof(readB), filePath);

	if (readL <= 0)	return;
	string = CString(readB, readL);

	while (!string.IsEmpty())
	{
		gno = parseX(string, ";");
		if (gno.IsEmpty() || gno.GetLength() != 2)
			break;
		
		readL = GetPrivateProfileString(_T("GROUPNAME"), gno, "", readB, sizeof(readB), filePath);
		gname = CString(readB, readL);
		
		idx = m_pInterestGroupList->AddString(gname); 
		m_pInterestGroupList->SetItemData(idx, atoi(gno));
	}
}

CString CMapWnd::parseX(CString &srcstr, CString substr)
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

void CMapWnd::CtrlKRXShow()
{
// 	if(m_pKRXLabel)	m_pKRXLabel->ShowWindow(SW_SHOW);
// 	if(m_pKRXList)	m_pKRXList->ShowWindow(SW_SHOW);
// 	if(m_pKRXName)	m_pKRXName->ShowWindow(SW_SHOW);
}

void CMapWnd::settujaCombo()
{

	if (!m_tujatype1)
	{
		m_tujatype1 = std::make_unique<CComboBox>();
		m_tujatype1->Create(WS_BORDER|WS_VISIBLE|CBS_DROPDOWNLIST|WS_VSCROLL|WS_TABSTOP|LVS_ALIGNTOP|LVS_REPORT,
			CRect(8,207,80,460), this, IDC_TUJATYPE1);
		m_tujatype1->SetFont(m_font, TRUE);
		SetWindowTheme(m_tujatype1->GetSafeHwnd(), L"", L"");
		m_tujatype1->AddString("_지수");
		m_tujatype1->AddString("KOSPI");
		m_tujatype1->AddString("KOSDAQ");
		m_tujatype1->AddString("선물");
		m_tujatype1->AddString("콜옵션");
		m_tujatype1->AddString("풋옵션");
		m_tujatype1->SetCurSel(0);
	}
	m_tujatype1->SetCurSel(0);
}

void CMapWnd::loadTUJA()
{
	std::unique_ptr<char[]> wb;
	CFile	file;
	int		size{};
	CString	path;
	struct	upcode* upc{};
	std::shared_ptr<struct upcode> pUpc{};
	CString	string;
	CString strdata;
	
	path.Format("%s/%s/%s", m_root, TABDIR, UPCODE);
	if (!file.Open(path, CFile::modeRead|CFile::typeBinary))
		return;
	
	size = (int)file.GetLength();
	wb = std::make_unique<char[]>(size);
	
	file.Read(wb.get(), size);
	file.Close();
	

	for (unsigned ii = 0; ii < m_arUPCODE.size(); ii++)
	{
		pUpc = m_arUPCODE[ii];
		pUpc.reset();
	}
	m_arUPCODE.clear();

	size /= sizeof(struct upcode);
	upc = (struct upcode *)wb.get();

	for (int ii = 0; ii < size; ii++, upc++)
	{
		pUpc = std::make_unique<struct upcode>();
		::CopyMemory(pUpc.get(), upc, sizeof(struct upcode));
		m_arUPCODE.push_back(pUpc);
	}
	//delete[] wb;
	
	if (!m_tujatype2)
	{
		m_tujatype2 = std::make_unique<CComboBox>();
		m_tujatype2->Create(WS_BORDER|WS_VISIBLE|CBS_DROPDOWNLIST|CBS_SORT|WS_VSCROLL|WS_TABSTOP|LVS_ALIGNTOP|LVS_REPORT, 
			CRect(83,207,255,460), this, IDC_TUJATYPE2);
		m_tujatype2->SetFont(m_font, TRUE);
		m_tujatype2->SetDroppedWidth(110);
		SetWindowTheme(m_tujatype2->GetSafeHwnd(), L"", L"");
	}

	m_tujatype1->GetLBText(m_tujatype1->GetCurSel(),strdata);
	strdata.TrimLeft();
	strdata.TrimRight();
	if(!strdata.Compare("KOSPI"))	
	{
		if (m_tujatype1)
		{
			m_tujatype2->ShowWindow(SW_SHOW);
		}
		m_tujatype2->ResetContent();
		m_tujatype2->AddString(" _업종");

		for (unsigned ii = 0; ii < m_arUPCODE.size(); ii++)
		{
			upc = (struct upcode*)m_arUPCODE[ii].get();
			if (upc->jgub == 0 || upc->jgub == 1 || upc->jgub == 4)
			{
				string.Format("%d%02d", upc->jgub, upc->ucod);
				m_tujatype2->AddString(string + " " + CString(upc->hnam, sizeof(upc->hnam)));
			}
		}
		removeAllItems();
		m_tujatype2->SetCurSel(0);
	}
	else if(!strdata.Compare("KOSDAQ"))
	{
		if (m_tujatype1)
		{
			m_tujatype2->ShowWindow(SW_SHOW);
		}
		m_tujatype2->ResetContent();
		m_tujatype2->AddString(" _업종");

		//for (int ii = 0; ii < m_arUPCODE.GetSize(); ii++)
		//{
		//	upc = (struct upcode*)m_arUPCODE.GetAt(ii);
		//	if (upc->jgub == 2 || upc->jgub == 3)
		//	{
		//		string.Format("%d%02d", upc->jgub, upc->ucod);
		//		m_tujatype2->AddString(string+" "+CString(upc->hnam,sizeof(upc->hnam)));
		//	}
		//}
		for (unsigned ii = 0; ii < m_arUPCODE.size(); ii++)
		{
			upc = (struct upcode*)m_arUPCODE[ii].get();
			if (upc->jgub == 2 || upc->jgub == 3)
			{
				string.Format("%d%02d", upc->jgub, upc->ucod);
				m_tujatype2->AddString(string + " " + CString(upc->hnam, sizeof(upc->hnam)));
			}
		}

		removeAllItems();
		m_tujatype2->SetCurSel(0);
	}
	else
	{
		m_tujatype2->ShowWindow(SW_HIDE);
		loadCode();
	}
}
void CMapWnd::setElwCombo()
{
	std::unique_ptr<char[]>wb;
	int		size{};
	int		ii{}, jj{}, retc{};
	CString	gcjs, hbge, mjmt;
	CString	code, name;


	if (!m_rtype)
	{
		m_rtype = std::make_unique<CComboBox>();
		m_rtype->Create(WS_BORDER|WS_VISIBLE|CBS_DROPDOWNLIST|WS_VSCROLL|WS_TABSTOP|LVS_ALIGNTOP|LVS_REPORT,
			            CRect(8,207,59,460), this, IDC_RTYPE);
		m_rtype->SetFont(m_font, TRUE);
		SetWindowTheme(m_rtype->GetSafeHwnd(), L"", L"");
		m_rtype->AddString("권리");
		m_rtype->AddString("콜");
		m_rtype->AddString("풋");
		m_rtype->AddString("기타");
		m_rtype->SetCurSel(0);
	}

	if (!m_bstock)
	{
		m_bstock = std::make_unique<CComboBox>();
		m_bstock->Create(WS_BORDER|WS_VISIBLE|CBS_DROPDOWNLIST|CBS_SORT|WS_VSCROLL|WS_TABSTOP|LVS_ALIGNTOP|LVS_REPORT, 
			             CRect(63,207,131,460), this, IDC_BSTOCK);
		m_bstock->SetFont(m_font, TRUE);
		m_bstock->SetDroppedWidth(110);
		SetWindowTheme(m_bstock->GetSafeHwnd(), L"", L"");
		m_bstock->AddString(" _기초자산");
		m_bstock->SetCurSel(0);
	}
	if (!m_icorp)
	{
		m_icorp = std::make_unique<CComboBox>();
		m_icorp->Create(WS_BORDER|WS_VISIBLE|CBS_DROPDOWNLIST|WS_VSCROLL|WS_TABSTOP|LVS_ALIGNTOP|LVS_REPORT, 
						CRect(135,207,192,460), this, IDC_ICORP);
		m_icorp->SetFont(m_font, TRUE);
		SetWindowTheme(m_icorp->GetSafeHwnd(), L"", L"");
		m_icorp->SetDroppedWidth(120);
		m_icorp->AddString("발행사");
		m_icorp->SetCurSel(0);
	}
	if (!m_month)
	{
		m_month = std::make_unique<CComboBox>();
		m_month->Create(WS_BORDER|WS_VISIBLE|CBS_DROPDOWNLIST|WS_VSCROLL|WS_TABSTOP|LVS_ALIGNTOP|LVS_REPORT, 
			            CRect(196,207,252,460), this, IDC_MONTH);
		m_month->SetFont(m_font, TRUE);
		SetWindowTheme(m_month->GetSafeHwnd(), L"", L"");
		m_month->SetDroppedWidth(50);
		m_month->AddString("만기");
		m_month->SetCurSel(0);
	}

	if (m_month->GetCount() > 2)
		return;

	// Set 기초자산, 발행사, 만기월 for Combo
	wb = readCodeFile(&size, ELWCODE);
	if (size <= 0)
		return;

	struct	elwcode*	wcode;

	wcode = (struct elwcode *)wb.get();
	size /= sizeof(struct elwcode);
	for (ii = 0; ii < size; ii++, wcode++)
	{
		hbge = CString(wcode->hbge, ELWENameLen);
		mjmt = CString(wcode->mjmt, 8);
		hbge.Replace('\0', ' '); hbge.TrimRight();
		mjmt.Replace('\0', ' '); mjmt.TrimRight();

		// 기초자산별
		for (jj = 0; jj < 5; jj++)
		{
			gcjs = CString(&wcode->gcjs[jj][1], HCodeLen-1);
			gcjs.Replace('\0', ' '); gcjs.TrimRight();
			if (gcjs.IsEmpty())
				break;
			name = GetAxName(gcjs);
			if (name.IsEmpty() == false && m_bstock->FindString(0, name) == CB_ERR)	// 발행사별
			{
				retc = m_bstock->AddString(name.Left(HNameLen));
				m_bstock->SetItemData(retc, atoi(gcjs.Mid(1,6)));
			}
		}
		if (m_icorp->FindString(0, hbge) == CB_ERR)	// 발행사별
			m_icorp->AddString(hbge);
		if (m_month->FindString(0, mjmt.Mid(2, 4)) == CB_ERR)	// 만기월별
			m_month->AddString(mjmt.Mid(2, 4));
	}
	//delete[] wb;
}

//2013.02.28 KSJ 코스피, 코스닥 종목검색
void CMapWnd::setSearchName()
{
	if (!m_pNameLabel)
	{
		m_pNameLabel = std::make_unique<CfxStatic>();
		m_pNameLabel->Create(_T("검색"), WS_VISIBLE|WS_CHILD|BS_AUTORADIOBUTTON, 
			CRect(8,207,59,227), this, IDC_NAME_LABEL);
		m_pNameLabel->SetFont(m_font, TRUE);
		m_pNameLabel->SetBkColor(m_clrRoundColor);
	}

	if(!m_pNameSearch)
	{
		m_pNameSearch = std::make_unique<CEditEx>();
		m_pNameSearch->Create(WS_BORDER|WS_CHILD|WS_VISIBLE|ES_AUTOHSCROLL|ES_UPPERCASE, 
			CRect(63,207,255,227), this,  IDC_NAME_SEARCH);
		m_pNameSearch->SetFont(m_font, TRUE);
	}
	m_pNameLabel->ShowWindow(SW_SHOW);
	m_pNameSearch->ShowWindow(SW_SHOW);
}

// KSJ
CString CMapWnd::GetAxName(CString code)
{
	CString	ret;
	if (code.Compare("OSPI200") == 0)
		return (" _KOSPI200");
	if ( !m_parent )
		return "";

	char* dta = (char*)m_parent->SendMessage(WM_USER, MAKEWPARAM(nameDLL, 0), (LPARAM)(char*)code.operator LPCTSTR());
	ret.Format("%s", dta);
	return ret;
}

int CMapWnd::l_dudcode(int code)
{
    int upgb=0;
    int upcd=0;
    int dcod=0;
	
    upgb = code / 100;
    upcd = code % 100;
	#if 0
		if (code == 501) {
			return code;
		}
	#endif

	switch(upgb)
	{
	case 0: /* KOSKPI   */
		if (upcd > MAX_UP || upcd <= 0)
		{
			if (upcd < 41 || upcd >= 42)		/* 20160912 지배구조 지수 산출 중단 */
				return (-1);
		}
		if (upcd == 23)
			return (-1);
		dcod = upcd;
		break;
	case 1: /* KP200    */
		if (upcd == 98)
		{
			dcod = 8029;
			return dcod;	
		}
		else if(upcd == 99)
		{
			dcod = 9999;
			return dcod;	
		}
		/* -> ETN */
		else if (upcd == 97)  
		{    
			dcod = 197; 
			return dcod;
		}    
		/* <- */

		if (upcd <= 0 || upcd > MAX_K200)
			return (-1);
		if (upcd >= 2 && upcd < 7)  /* 기존 102 ~ 106 업종은 산출중단 */
			return (-1);
		dcod = 1000 + upcd;
		break;
	case 2: /* KOSDAQ   */
		if (upcd <= 0 || upcd > MAX_KOQ)
			return (-1);
		dcod = 9000 + upcd;
		break;
	case 3: /* KOSDAQ50 */
		if (upcd == 1)      /* KOSDAQ50		*/
			return (-1);
		else if (upcd == 2) /* KOSDAQIT		*/
			return (-1);
		#if 0
				/* KOSTAR		*/
				/* -> 20160523 KOSTAR 산출 중단
				else if (upcd == 3) 
					dcod = 9102;
				<- */
		#endif
		else if (upcd == 5) /* KOSDAQ150	*/
			dcod = 9103;
		/* -> 20150831 */
		else if (upcd == 6) /* KOSDAQ150 동일가중 */
			dcod = 9104;
		else if (upcd == 7) /* KOSDAQ150 레버리지 */
			dcod = 9105;
		/* <- */
		/* -> 20160129 */
		else if (upcd == 8) /* KOSDAQ150 레버리지 0.5 */
			dcod = 9106;
		/* <- */
		else if (upcd >= 9 && upcd <= 17) /* KOSDAQ150 섹터지수*/
			dcod = 9098 + upcd;

		else
			return (-1);
		break;
	case 4: /* KOSPI100 */
		if (upcd <= 0 || upcd > MAX_K100)
			return (-1);
		dcod = 2000 + upcd;
		break;      
		/*switch(upcd)
		{
			case 1: /-* KOSPI100    *-/
			case 2: /-* KOSPI50 *-/
				dcod = 2000 + upcd;
				break;
			case 3: /-* KOSPI IT    *-/
			default: return (-1);
		}
		break;*/
	case 5: /* 코넥스 */
		if (upcd == 1)
			dcod = 501;
		else 
			return (-1);
		break;
	#if 0
		case 5: /* KOSDI    */
			if (upcd == 1)
				dcod = 41;
			else
				return (-1);
			break;
	#endif
		case 6: /* SECTOR   */
			if (upcd <= 0 || upcd > MAX_SECTOR)
				return (-1);
			dcod = 6000 + upcd;
			break;
		case 7: /* FREE   */
			if (upcd <= 0 || upcd > MAX_FREE)
				return (-1);
			dcod = 7000 + upcd;
			break;
		/* 20161128 미니 코스피 200선물 추가 */
		case 8: /* 기타지수 */
			if (upcd <= 0 ||  upcd ==6 || upcd > MAX_IETC)    
				return (-1);
			dcod = 8000 + upcd;
			break;

		case 9: /*  통합지수(코드:999)  */
			switch(upcd)
			{
				case 99:
					dcod = 5001;
					break;
				case 98:
					dcod = 1001;
					break;

				default:
					return -1;
			}
			/*
			if (upcd != 99)
				return (-1);
			dcod = 5001;
			*/
			break;
		default:
			return (-1);
		}
		return (dcod);
}

void CMapWnd::loadCode()
{
	char tmpb[ELWENameLen + 4]{};
	std::unique_ptr<char[]>wb;
	int		ii=0, jj=0, size=0;
	int		retc=0, scode=0;
	CString	code, name, gcjs;
	bool	hskip=0;

	//_listitem* listitem{};
	std::shared_ptr<_listitem> listitem;

	removeAllItems();
	//m_searchlist.RemoveAll();
	//m_listitem.RemoveAll();
	m_listitem.clear();
	m_searchlist.clear();

	if (m_scode->GetItemCount() > 0)
	{
		switch (m_type)
		{
		case cFutures:	case cOption:
			//선물,옵션간 전환시 선택한 종목 초기화 안함
			code = CString(m_scode->GetItemText(0, 0));
			if (code.GetLength() != 8)
				m_scode->DeleteAllItems();
			break;
		case cKospi:	case cKosdaq:	case cElw:
			//주식종목간 전환시 선택한 종목 초기화 안함
			code = CString(m_scode->GetItemText(0, 0));
			if (code.GetLength() != 6)
				m_scode->DeleteAllItems();
			break;
		case cTujaja:
			code = CString(m_scode->GetItemText(0, 0));
			if (code.GetLength() != 10)
				m_scode->DeleteAllItems();
			break;
		case cIndex:
		default:
			m_scode->DeleteAllItems();
			break;
		}
	}

	switch (m_type)
	{
	case cFutures:
		wb = readCodeFile(&size, FJCODE);
		if (size <= 0)
			return;

		struct	fjcode*	fcode;
		fcode = (struct fjcode *)wb.get();
		size /= sizeof(struct fjcode);
		for (ii = 0; ii < size ; ii++, fcode++)
		{
			code = CString(fcode->cod2, FCodeLen);
			name = CString(fcode->hnam, FNameLen);
			code.Replace('\0', ' '); code.TrimRight();
			name.Replace('\0', ' '); name.TrimRight();

			addCode(code, name);
		}
		break;

	case cOption:
		wb = readCodeFile(&size, OJCODE);
		if (size <= 0)
			return;

		struct	ojcode*	ocode;
		ocode = (struct ojcode *)(wb.get() + sizeof(struct ojcodh));
		size -= sizeof(struct ojcodh);
		size /= sizeof(struct ojcode);
		for (ii = 0; ii < size; ii++, ocode++)
		{
			for (int jj = 0; jj < 11; jj++)
			{
				if (ocode->call[jj].yorn == '1')
				{
					code = CString(ocode->call[jj].cod2, OCodeLen);
					name = CString(ocode->call[jj].hnam, ONameLen);
					code.Replace('\0', ' '); code.TrimRight();
					name.Replace('\0', ' '); name.TrimRight();

					addCode(code, name);
				}

				if (ocode->put[jj].yorn == '1')
				{
					code = CString(ocode->put[jj].cod2, OCodeLen);
					name = CString(ocode->put[jj].hnam, ONameLen);
					code.Replace('\0', ' '); code.TrimRight();
					name.Replace('\0', ' '); name.TrimRight();

					addCode(code, name);
				}
			}
		}
		break;

	case cIndex:
		wb = readCodeFile(&size, UPCODE);
		if (size <= 0)
			return;

		struct	upcode*	ucode;

		ucode = (struct upcode *)wb.get();
		size /= sizeof(struct upcode);
		for (ii = 0; ii < size; ii++, ucode++)
		{
			name = CString(ucode->hnam, UNameLen);
			switch (ucode->jgub)
			{
			case upKOSPI:
				switch (ucode->ucod)
				{
				case 41:	/* 배당지수(KBD40) */
					code.Format("BD40");	break;
				case 42:	/* 지배구조(KGI50) */
					code.Format("GI50");	break;
					break;
				default:
					code.Format("00%02d", ucode->ucod);
					break;
				}
				break;
			case upKOSDAQ:
				code.Format("Q0%02d", ucode->ucod);
				if (name.Find("KOSDAQ") == -1)
					name.Insert(0, "KOSDAQ ");
				break;
			case upKOSPI200:
				code.Format("20%02d", ucode->ucod);
				break;
			case upKOSPIx:
				switch (ucode->ucod)
				{
				case 1:	/* KOSPI100 */
				default:
					code.Format("10%02d", ucode->ucod);
					break;
				case 2:	/* KOSPI 50 */
					code.Format("0501");	break;
					break;
				case 3:	/* KOSPI IT */
					code.Format("IT01");	break;
				}
				break;
			default:
				continue;
			}
			name.Replace('\0', ' '); name.TrimRight();

			addCode(code, name);
		}
		break;

	case cElw:
		wb = readCodeFile(&size, ELWCODE);
		if (size <= 0)
			return;
		struct	elwcode*	wcode;

		wcode = (struct elwcode *)wb.get();
		size /= sizeof(struct elwcode);
		for (ii = 0; ii < size; ii++, wcode++)
		{
			// 권리유형
			if (m_rtype->GetCurSel() != 0 
				&& (wcode->krye[1]&0x0f) != m_rtype->GetCurSel())
				continue;
			if (m_month->GetCurSel() != 0)
			{	// 만기월별
				retc = m_month->GetLBText(m_month->GetCurSel(), tmpb);
				if (retc != CB_ERR && strncmp(&wcode->mjmt[2], tmpb, 4) != 0)
					continue;
			}
			if (m_bstock->GetCurSel() != 0)
			{	// 기초자산별
				scode = m_bstock->GetItemData(m_bstock->GetCurSel());
				hskip = true;
				for (jj = 0; jj < 5; jj++)
				{
					gcjs = CString(&wcode->gcjs[jj][1], HCodeLen-1);
					gcjs.Replace('\0', ' '); gcjs.TrimRight();
					if (gcjs.IsEmpty())
						break;
					if (atoi(gcjs.Left(HCodeLen)) == scode)
					{
						hskip = false;
						break;
					}
				}
				if (hskip == true)
					continue;
			}

			code = CString(wcode->codx, ELWCodeLen);
			name = CString(wcode->hnam, ELWHNameLen);
			code.Replace('\0', ' '); code.TrimRight();
			name.Replace('\0', ' '); name.TrimRight();

			if (m_icorp->GetCurSel() != 0)
			{	// 발행사별
				retc = m_icorp->GetLBText(m_icorp->GetCurSel(), tmpb);
				if (retc != CB_ERR && strncmp(name.Mid(1, 4), tmpb, 4) != 0)
					continue;
			}

			addCode(code.Mid(0), name.Mid(1));
		}
		break;

	case cKosdaq:
		wb = readCodeFile(&size, HJCODEx);
		if (size <= 0)
			return;
		wb[size] = '\0';

		struct	hjcodex*	hcode;

		hcode = (struct hjcodex *)wb.get();
		size /= sizeof(struct hjcodex);
		for (ii = 0; ii < size; ii++, hcode++)
		{
			if (hcode->kosd != jmKOSDAQ)
				continue;

			//listitem = new _listitem();
			listitem = std::make_shared<_listitem>();

			code = CString(hcode->code, HCodeLen);
			name = CString(hcode->hnam, HNameLen);
			code.Replace('\0', ' '); code.TrimRight();
			name.Replace('\0', ' '); name.TrimRight();
			name.MakeUpper();

			::memcpy(listitem->code, code.Mid(1), code.Mid(1).GetLength());
			::memcpy(listitem->name, name.Mid(1), name.Mid(1).GetLength());


			m_listitem.push_back(listitem);
			m_searchlist.push_back(listitem);

			addCode(code.Mid(1), name.Mid(1));
		}
		break;

	case cProgram:
		addCode("PM0001", "코스피");
		addCode("PM9001", "코스닥");
		break;

	case cTujaja:
		{
			CString strdata1,strdata2;
			CString codedata1,codedata2,codenm;
			int dcode=0;
			
			m_tujatype1->GetLBText(m_tujatype1->GetCurSel(),strdata1);
			strdata1.TrimLeft();
			strdata1.TrimRight();
			

			if(m_tujatype2)
			{
				m_tujatype2->GetLBText(m_tujatype2->GetCurSel(),strdata2);
				strdata2.TrimLeft();
				strdata2.TrimRight();
			}

		
			if(!strdata1.Compare("KOSPI"))
			{
				codedata1 ="01";
				if(!strdata2.Compare("_업종"))
					return;
				else
				{
					dcode = atoi(strdata2.Mid(0,3));
					dcode = l_dudcode(dcode);
					codedata2.Format("%04d",dcode);
					//codedata2 = strdata2.Mid(0,3);
				    codenm = "-KOSPI("+strdata2.Mid(4)+")";
				}
			}
			else if(!strdata1.Compare("KOSDAQ"))
			{
				codedata1 ="01";
				if(!strdata2.Compare("_업종"))
					return;
				else
				{
					dcode = atoi(strdata2.Mid(0,3));
					dcode = l_dudcode(dcode);
					codedata2.Format("%04d",dcode);
					//codedata2 = strdata2.Mid(0,3);
					codenm = "-KOSDAQ("+strdata2.Mid(4)+")";
				}
			}
			else if(!strdata1.Compare("선물"))
			{
				codedata1 ="02";
				codedata2 ="0000";
				codenm = "-선물";
			}
			else if(!strdata1.Compare("콜옵션"))
			{
				codedata1 ="05";
				codedata2 ="0000";
				codenm = "-콜옵션";
			}
			else if(!strdata1.Compare("풋옵션"))
			{
				codedata1 ="06";
				codedata2 ="0000";
				codenm = "-풋옵션";
			}
			else return;

			codenm.Replace(" ","");
			addCode("TJ"+codedata1+"00"+codedata2,"기관계"+codenm);
			addCode("TJ"+codedata1+"01"+codedata2,"금융투자"+codenm);
			addCode("TJ"+codedata1+"02"+codedata2,"보험"+codenm);
			addCode("TJ"+codedata1+"03"+codedata2,"투신"+codenm);
			addCode("TJ"+codedata1+"04"+codedata2,"은행"+codenm);
			addCode("TJ"+codedata1+"05"+codedata2,"기타금융"+codenm);
			addCode("TJ"+codedata1+"06"+codedata2,"연기금"+codenm);
			addCode("TJ"+codedata1+"07"+codedata2,"기타법인"+codenm);
			addCode("TJ"+codedata1+"08"+codedata2,"개인"+codenm);
			addCode("TJ"+codedata1+"09"+codedata2,"외국인"+codenm);
			addCode("TJ"+codedata1+"12"+codedata2,"사모"+codenm);
			addCode("TJ"+codedata1+"13"+codedata2,"국가"+codenm);

			break;
		}

	case cKospi:
	default:
		wb = readCodeFile(&size, HJCODEx);
		if (size <= 0)
			return;

		struct	hjcodex*	hcodx;

		hcodx = (struct hjcodex *)wb.get();
		size /= sizeof(struct hjcodex);
		for (ii = 0; ii < size; ii++, hcodx++)
		{
			if (hcodx->kosd != jmKOSPI)
				continue;

			//listitem = new _listitem();
			listitem = std::make_shared<_listitem>();

			switch (hcodx->ssgb)
			{
			case jmSINJU: case jmSINJS:	case jmHYFND: case jmELW:
				continue;
			default: break;// Keep going
			}

			code = CString(hcodx->code, HCodeLen);
			name = CString(hcodx->hnam, HNameLen);
			code.Replace('\0', ' '); code.TrimRight();
			name.Replace('\0', ' '); name.TrimRight();
			name.MakeUpper();

			::memcpy(listitem->code, code.Mid(1), code.Mid(1).GetLength());
			::memcpy(listitem->name, name.Mid(1), name.Mid(1).GetLength());
			
			m_listitem.push_back(listitem);
			m_searchlist.push_back(listitem);
			
			addCode(code.Mid(1), name.Mid(1));
		}
		break;
	}
}

std::unique_ptr<char[]> CMapWnd::readCodeFile(int* size, char* codefile)
{
	CFile	file;
	std::unique_ptr<char[]>	wb{};
	CString	text;

	text.Format("%s/%s/%s", m_root, TABDIR, codefile);
	if (!file.Open(text, CFile::modeRead | CFile::typeBinary))
		return NULL;

	*size = (int)file.GetLength();
	wb = std::make_unique<char[]>(*size + 1);

	if (*size != (int)file.Read(wb.get(), *size))
	{
		file.Close();
		return NULL;
	}
	file.Close();

	wb.get()[*size] = '\0';

	return wb;
}


char* CMapWnd::readCodeFile0(int* size, char* codefile)
{
	CFile	file;
	std::unique_ptr<char[]>	wb{};
	CString	text;
	
	text.Format("%s/%s/%s", m_root, TABDIR, codefile);
	if (!file.Open(text, CFile::modeRead|CFile::typeBinary))
		return NULL;

	*size = (int)file.GetLength();
	wb = std::make_unique<char[]>(*size + 1);

	if (*size != (int)file.Read(wb.get(), *size))
	{
		file.Close();
		return NULL;
	}
	file.Close();

	return wb.release();
}

void CMapWnd::loadItem()
{
	int	size{};
	std::unique_ptr<char[]> wb = std::make_unique<char[]>(1024 * 6);
	CString	text, str, forms, tmpcs;

	if (m_sitem->GetItemCount() > 0)
	{
		CString code;
		switch (m_type)
		{
		case cKospi:	case cKosdaq:	case cElw:
			// 주식종목간 전환시 선택된 종목(아래쪽 리스트) 초기화 안함
			code = CString(m_scode->GetItemText(0, 0));
			if (code.GetLength() != 6)
				m_sitem->DeleteAllItems();
			break;
		case cFutures:	case cOption:
			// 선물/옵션간 전환시 선택된 종목(아래쪽 리스트) 초기화 안함
			code = CString(m_scode->GetItemText(0, 0));
			if (code.GetLength() != 8)
				m_sitem->DeleteAllItems();
			break;
		case cIndex:
		default:
			m_sitem->DeleteAllItems();
			break;
		}
	}

	switch (m_type)
	{
	case cFutures:	text = _T("Futures");	break;
	case cOption:	text = _T("Option");	break;
	case cIndex:	text = _T("Index");		break;
	case cProgram:	text = _T("Program");	break;
	case cTujaja:	text = _T("Tujaja");	break;
	case cKospi:
	case cKosdaq:
	case cElw:
	default:	text = _T("Stock");	break;
	}
	m_item->DeleteAllItems();

	str.Format("%s/%s/dde_item.ini", m_root, TABDIR);

	size = GetPrivateProfileSection(text, wb.get(), 1024 * 6, str);// sizeof(wb.get()), str);
	text = CString(wb.get(), size);
	for (int idx = 0 ; !text.IsEmpty(); idx++)
	{
		size = text.Find('\0');
		if (size != -1)
		{
			str  = text.Left(size++);
			text = text.Mid(size);
		}
		else
		{
			str = text;
			text.Empty();
		}

		size = str.Find('=');
		if (size != -1)
		{
			forms.Format("%s\'%s.%s\'", m_itemKey, codeKEY, str.Mid(size+1));
			addItem(m_item.get(), str.Left(size), forms, idx);
		}
	}
}

void CMapWnd::loadItemKey()
{
	char	wb[64];
	CString	text, str;

	text.Format("%s\\%s\\axisDDE.ini", m_root, TABDIR);
	GetPrivateProfileString("Informations", "Service", "AX", wb, sizeof(wb), text);
	str = wb;
	GetPrivateProfileString("Informations", "Topic", "KS", wb, sizeof(wb), text);
	m_itemKey.Format("=%s|%s!", str, wb);
}

void CMapWnd::addCode(CString code, CString name)
{
	if (m_code == NULL)
		return;

	const int	idx = m_code->GetItemCount();
	addItem(m_code.get(), code, name, idx);
}

BOOL CMapWnd::addColumn(CListCtrl* list, CString columns, int col, int width)
{
	LV_COLUMN lvc;

	lvc.mask     = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT;
	lvc.fmt      = LVCFMT_LEFT;
	lvc.pszText  = (LPTSTR)columns.operator LPCTSTR();
	lvc.cchTextMax = columns.GetLength();
	lvc.cx       = list->GetStringWidth(lvc.pszText) + width;
	lvc.iSubItem = -1;

	return list->InsertColumn(col, &lvc);
}

[[gsl::suppress(26400)]]
[[gsl::suppress(26409)]]
BOOL CMapWnd::addItem(CListCtrl* list, CString items, CString subs, int item)
{
	LV_ITEM lvi;

	lvi.mask     = LVIF_TEXT;
	lvi.iItem    = item;
	lvi.iSubItem = 0;
	lvi.pszText  = (LPTSTR)items.operator LPCTSTR();

	if (list == m_code.get())
	{
		struct	_listR*	listR;
		listR = new _listR;

		::memcpy(listR->code, items, items.GetLength());
		::memcpy(listR->name, subs, subs.GetLength());		//2016.08.24 KSJ 종목명 사이즈가 늘어나면서 에러 발생했다. 이유 없이 죽어서 찾는데 힘들었음....

		lvi.mask  |= LVIF_PARAM;
		lvi.lParam = (LPARAM)listR;
	}
	list->InsertItem(&lvi);
	return list->SetItemText(item, 1, subs);
}

[[gsl::suppress(26401)]]
[[gsl::suppress(26409)]]
void CMapWnd::removeAllItems()
{
	for (int ii = 0; ii < m_code->GetItemCount(); ii++)
	{
		if (m_code->GetItemData(ii))
		{
			delete (struct _listR *)m_code->GetItemData(ii);
		}
	}
	
	m_code->DeleteAllItems();
}

BOOL CMapWnd::checkAddItem( CListCtrl* pSrc, CListCtrl* pDest, BOOL bAll )
{
	BOOL bRet = TRUE;

	if( bAll )
		if( pSrc->GetItemCount() > MAX_ITEM_COUNT )
			bRet = FALSE;

	int iSrcCount = 0;
	for (POSITION pos = pSrc->GetFirstSelectedItemPosition(); pos; )
	{
		const int	item = pSrc->GetNextSelectedItem(pos);
		CString items = pSrc->GetItemText(item, 0);
		if (!checkDuplication(pDest, items))
		{
			iSrcCount++;
		}
	}

	const int iDestCount = pDest->GetItemCount();

	if( iSrcCount > MAX_ITEM_COUNT )
		bRet = FALSE;

	if( iSrcCount + iDestCount > MAX_ITEM_COUNT )
		bRet = FALSE;			

	if( !bRet )
	{
		CString str;
		str.Format("%d 개 이상의 종목을 시세연결 할 수 없습니다.", MAX_ITEM_COUNT);
		AfxMessageBox(str,MB_ICONWARNING);
	}

	return bRet;
}

void CMapWnd::addItem(CListCtrl* src, CListCtrl* dest, bool code, bool all)
{
	CString	items, subs;

	if (all)
	{
		dest->DeleteAllItems();
		for (int ii = 0; ii < src->GetItemCount(); ii++)
		{
			items = src->GetItemText(ii, 0);
			subs  = src->GetItemText(ii, 1);
			addItem(dest, items, subs, ii);
		}
	}
	else
	{
		for (POSITION pos = src->GetFirstSelectedItemPosition(); pos; )
		{
			const int	item = src->GetNextSelectedItem(pos);
			items = src->GetItemText(item, 0);
			if (!checkDuplication(dest, items))
			{
				const int	index = dest->GetItemCount();
				subs = src->GetItemText(item, 1);
				addItem(dest, items, subs, index);
			}
		}
	}
}

void CMapWnd::deleteItem(CListCtrl* dest, bool all)
{
	if (all)
		dest->DeleteAllItems();
	else
	{
		for (POSITION pos = dest->GetFirstSelectedItemPosition(); pos; )
		{
			const int	item = dest->GetNextSelectedItem(pos);
			dest->DeleteItem(item);
		}
	}
}

bool CMapWnd::checkDuplication(CListCtrl* dest, CString text)
{
	for (int ii = 0; ii < dest->GetItemCount(); ii++)
	{
		if (!text.CompareNoCase(dest->GetItemText(ii, 0)))
			return true;
	}

	return false;
}

void CMapWnd::search()
{
	int	len=0;
	CString	str, text;

	if (text.IsEmpty())
		return;

	len = text.GetLength();
	m_code->SortItems(compare, 1);
	for (int ii = 0; ii < m_code->GetItemCount(); ii++)
	{
		str = m_code->GetItemText(ii, 1);
		str.TrimLeft();
		str = str.Left(len);
		if (text.CompareNoCase(str) == 0)
		{
			CRect	rc;
			CSize	size;

			for( int j = 0; j < m_code->GetItemCount(); j++ ) // jjk 2009.02.11 키워드 발견시 선택되었던 값 언셀렉트
				m_code->SetItemState(j, 0, LVIS_SELECTED|LVIS_FOCUSED);

			m_code->SetItemState(ii, LVIS_SELECTED|LVIS_FOCUSED, LVIS_SELECTED|LVIS_FOCUSED);
			m_code->GetItemRect(0, rc, LVIR_LABEL);
			size.cx = 0;
			size.cy = rc.Height() * (ii - m_code->GetTopIndex());
			
			m_code->Scroll(size);
			break;
		}
	}
}

// jjk 엑셀 매크로 코드 추가 2009.2.11
CString _make_time_string( CString item )
{
	item.Delete(0);
	CString strFormula = "=IF(LEN(TEXT("
		                 +item+
						 ",\"0\"))=6,MID(TEXT("
		                 +item+
						 ",\"0\"),1,2)&\":\"&MID(TEXT("
						 +item+
						 ",\"0\"),3,2)&\":\"&MID(TEXT("
						 +item+
						 ",\"0\"),5,2),MID(TEXT("
						 +item+
						 ",\"0\"),1,1)&\":\"&MID(TEXT("
						 +item+
						 ",\"0\"),2,2)&\":\"&MID(TEXT("
						 +item+
						 ",\"0\"),4,2))"
						 
						 ;
	return strFormula;
}

CString _make_money_string( CString item )
{
	item.Delete(0);
	CString strFormula = "=TEXT(" 
		                 +item+
						 ",\"#,##0\")";
	return strFormula;
}
////////////////////////////////////////

// jjk 엑셀 표시형식 추가 
CString _make_float2_string( CString item )
{
	item.Delete(0);
	CString strFormula = "=TEXT(" 
		                 +item+
						 ",\"0.00\")";
	return strFormula;
}

//2013.01.18 KSJ 셀서식 숫자변환 체크를 위해 bool bCheck를 추가함.
CString _find_type(CList<CString,CString>& list, CString strItem, bool bCheck)
{
	CString str = strItem.Right(4);
	str = str.Left(3);

	POSITION pos = list.Find( str );
	if( pos == NULL )
		return strItem;

	CString strFormat = list.GetNext( pos );
	strFormat = list.GetAt( pos );
	const int type = atoi( strFormat );

	CString strRet;
	if( type == 1 )
		strRet = _make_time_string( strItem );
	else if( type == 2 && bCheck)	//2013.01.18 KSJ 셀서식 숫자변환 체크가 되어있을때만 
		strRet = _make_money_string( strItem );
	else if( type == 3 )
		strRet = _make_float2_string( strItem);
	else
		strRet = strItem;

	return strRet;
}

int _make_format_array( int type, CList<CString,CString>& list, CString strRoot )
{
	CString strSection="Format_";
	switch( type )
	{
	case 2: strSection += "Futures"; break;
    case 3: strSection += "Option"; break;
	case 4: strSection += "Index"; break;
	case 0: 
	case 1: 
	case 5: 
		strSection += "Stock";
		break;
	}

	char wb[1024*10];
	CString str;
	int size;
	
	str.Format("%s/%s/dde_item.ini", strRoot, TABDIR);
	size = GetPrivateProfileSection(strSection, wb, sizeof(wb),str);
	CString text = CString(wb, size);
	for (int idx = 0 ; !text.IsEmpty(); idx++)
	{
		size = text.Find('\0');
		if (size != -1)
		{
			str  = text.Left(size++);
			text = text.Mid(size);
		}
		else
		{
			str = text;
			text.Empty();
		}

		size = str.Find('=');
		if (size != -1)
		{
			//forms.Format("%s\'%s.%s\'", m_itemKey, codeKEY, str.Mid(size+1));
			//addItem(m_item, str.Left(size), forms, idx);	
			list.AddTail(str.Left(size));
			list.AddTail(str.Mid(size+1));		
		}
	}

	return 1;
}
////////////////////////////////////////////////////

void CMapWnd::runExcel()
{
	CString	text;
	CStringArray	codes, codeName, items, heads;

	CList<CString,CString> formatList;
	_make_format_array( m_type, formatList, m_root );
	
	codes.RemoveAll();
	codeName.RemoveAll();
	items.RemoveAll();
	heads.RemoveAll();
	for (int ii = 0; ii < m_scode->GetItemCount(); ii++)
	{
		switch (m_type)
		{
		case cIndex:
			codes.Add('K'+m_scode->GetItemText(ii, 0));
			codeName.Add(m_scode->GetItemText(ii, 1));
			break;
		case cKospi: case cKosdaq:
		case cElw:
			text = m_scode->GetItemText(ii, 0);
			if ((text.GetLength() == 6))		//2016.08.24 KSJ 관심종목일 경우는 주식과 선옵이 같이 있을수도 있음.
			{
				if((text.GetAt(0) < '5' || text.GetAt(0) > '8'))
					codes.Add('A'+m_scode->GetItemText(ii, 0));
				else if(getCodeType(text) == etnType)
					codes.Add('Q'+m_scode->GetItemText(ii, 0));
				else
					codes.Add('J'+m_scode->GetItemText(ii, 0));
			}
			else if(text.GetLength() == 8)
				codes.Add(m_scode->GetItemText(ii, 0));

			codeName.Add(m_scode->GetItemText(ii, 1));
			break;
		default:
			codes.Add(m_scode->GetItemText(ii, 0));
			codeName.Add(m_scode->GetItemText(ii, 1));
			break;
		}
	}
	if (codes.GetSize() <= 0)
	{
		AfxMessageBox("선택된 종목코드가 없습니다.");
		return;
	}

	for (int ii = 0; ii < m_sitem->GetItemCount(); ii++)
	{
		heads.Add(m_sitem->GetItemText(ii, 0));
		text = m_sitem->GetItemText(ii, 1) + m_sitem->GetItemText(ii, 2);
		items.Add(text);
	}
	if (items.GetSize() <= 0)
	{
		AfxMessageBox("선택된 DDE 항목이 없습니다.");
		return;
	}

	::CoInitialize(0);
	_Application	excel;

	if (!excel.CreateDispatch("Excel.Application"))
	{
	//	AfxMessageBox("Can't run Excel Application.");
		CString slog;
		slog.Format("[%d] ", GetLastError());
		AfxMessageBox(slog);
		return;
	}

	bool	vertical = false;
	if (m_vert->GetCheck() == 1)
		vertical = true;
	COleVariant covOptional((long)DISP_E_PARAMNOTFOUND, VT_ERROR);
	TRY
	{
		Workbooks	wbs;
		_Workbook	wb;
		Worksheets	sheets;
		_Worksheet	sheet;
		Range		range;
		LPDISPATCH	lpDisp{};
		CString		code, item;

		const	int	alphas = 26;		// alphabet character #

		lpDisp = excel.GetWorkbooks();		//OutputDebugString("[KSJ] excel.GetWorkbooks()");
		wbs.AttachDispatch(lpDisp);			//OutputDebugString("[KSJ] wbs.AttachDispatch(lpDisp)");

		lpDisp = wbs.Add(covOptional);		//OutputDebugString("[KSJ] wbs.Add(covOptional)");
		wb.AttachDispatch(lpDisp);			//OutputDebugString("[KSJ] wb.AttachDispatch(lpDisp)");

		lpDisp = wb.GetSheets();			//OutputDebugString("[KSJ] wb.GetSheets()");
		sheets.AttachDispatch(lpDisp);		//OutputDebugString("[KSJ] sheets.AttachDispatch(lpDisp)");

		if (sheets.GetCount() > 0)
		{
			lpDisp = sheets.GetItem(COleVariant((long)1, VT_I4));	//OutputDebugString("[KSJ] sheets.GetItem(COleVariant((long)1, VT_I4))");
		}
		else
		{
			lpDisp = sheets.Add(covOptional, covOptional, covOptional, covOptional);	//OutputDebugString("[KSJ] sheets.Add(covOptional, covOptional, covOptional, covOptional)");
		}
		sheet.AttachDispatch(lpDisp);		//OutputDebugString("[KSJ] sheet.AttachDispatch(lpDisp)");

		if (vertical)
		{
			CString strTemp;
// 			OutputDebugString("[KSJ] if (vertical)");
			for (int ii = 0, idx = -1; idx < codes.GetSize(); ii++, idx++)
			{
				for (int jj = -1; jj < items.GetSize(); jj++)
				{
					if(jj == -1 && ii < alphas)		//2016.08.24 KSJ 세로일때는 종목이 행단위로 추가되기때문에 Z -> AA로 갈때 
					{
						if(ii ==0)
							continue;

						item.Format("%c%d", (char)('A'+ii), jj+2);
					}
					else
					{
						if (ii < alphas)
							item.Format("%c%d", (char)('A'+ii), jj+2);
						else
							item.Format("%c%c%d", (char)('@'+ii/alphas), (char)('A'+ii%alphas), jj+2);
					}

					lpDisp = sheet.GetRange(COleVariant((LPCTSTR)item, VT_BSTR), COleVariant((LPCTSTR)item, VT_BSTR));
// 					strTemp.Format("[KSJ] [%d][%d] sheet.GetRange(%s)", ii, jj, item);
// 					OutputDebugString(strTemp);

					range.AttachDispatch(lpDisp);

// 					strTemp.Format("[KSJ] [%d][%d] range.AttachDispatch(lpDisp)", ii, jj);
// 					OutputDebugString(strTemp);


					if (idx < 0)
					{
						if(jj>-1)
						{
							range.SetValue(COleVariant((LPCTSTR)heads.GetAt(jj), VT_BSTR));
// 							strTemp.Format("[KSJ] [%d][%d] range.SetValue(COleVariant((LPCTSTR)heads(%s)", ii, jj, heads);
// 							OutputDebugString(strTemp);
						}
						else
						{
							range.SetValue(COleVariant("temp", VT_BSTR));
// 							strTemp.Format("[KSJ] [%d][%d] range.SetValue(COleVariant((LPCTSTR)temp", ii, jj);
// 							OutputDebugString(strTemp);
						}
					}
					else
					{
						code = codes.GetAt(idx);
						if(jj>-1)
							item = items.GetAt(jj);      // Excel 설정 값
						else
							item = codeName.GetAt(ii-1); // 종목명, 위에서 ii ==0일 경우에  continue했기 때문에 ii-1 이 됨

						item.Replace(codeKEY, code);	
						
						//2013.01.18 셀서식 숫자변환 체크되어있으면 ,를 찍어준다.
						item = _find_type( formatList, item, m_bCheck );
						
// 						strTemp.Format("[KSJ] [%d][%d] _find_type(%s)", ii, jj, item);
// 						OutputDebugString(strTemp);

						range.SetFormula(COleVariant((LPCTSTR)item, VT_BSTR));  //test

// 						strTemp.Format("[KSJ] [%d][%d] range.SetFormula(%s)", ii, jj, item);
// 						OutputDebugString(strTemp);

						range.SetHorizontalAlignment(COleVariant((short)-4152));  //test

// 						OutputDebugString("[KSJ] range.SetHorizontalAlignment");
					}
				}
			}			
		}
		else
		{
// 			OutputDebugString("[KSJ] if (vertical) else");
			for (int ii = 0; ii < items.GetSize()+1; ii++)
			{
				for (int jj = 1, idx = -1; idx < codes.GetSize(); jj++, idx++)		// item desc (idx = -1)
				{
					if(ii == 0)
					{
						if(jj == 1)
							continue;
						item.Format("%c%d", (char)('A'+ii), jj);
					}
					else
					{
						if (ii < alphas)
							item.Format("%c%d", (char)('A'+ii), jj);
						else
							item.Format("%c%c%d", (char)('@'+ii/alphas), (char)('A'+ii%alphas), jj);
					}


					lpDisp = sheet.GetRange(COleVariant((LPCTSTR)item, VT_BSTR), COleVariant((LPCTSTR)item, VT_BSTR));
					range.AttachDispatch(lpDisp);

					if (idx < 0)
					{
						range.SetValue(COleVariant((LPCTSTR)heads.GetAt(ii-1), VT_BSTR));
					}
					else
					{
						code = codes.GetAt(idx);

						if(ii != 0)
						{
							item = items.GetAt(ii-1);      // Excel 설정 값
						}
						else
						{
							item = codeName.GetAt(jj-2); // 종목명, 위에서 ii ==0일 경우에  continue했기 때문에 ii-1 이 됨
 						}

						item.Replace(codeKEY, code);

						//2013.01.18 셀서식 숫자변환 체크되어있으면 ,를 찍어준다.
						item = _find_type( formatList, item, m_bCheck );

						range.SetFormula(COleVariant((LPCTSTR)item, VT_BSTR));
						range.SetHorizontalAlignment(COleVariant((short)-4152));						
					}
				}
			} 
		}		
		 
		// jjk 2009.2.11 EntireColumnSelect 가 잘못 동작됨 AutoFit 함수가 제대로 작동 안됨 (삭제)
		range.ReleaseDispatch();		//OutputDebugString("[KSJ] range.ReleaseDispatch()");

		// jjk 2009.2.11 첫 번째 열 자동 맞춤. 다른 열은 동적으로 데이터가 변하므로 불가 
		Range fitRange = sheet.GetRange(COleVariant("A1"),covOptional);		//OutputDebugString("[KSJ] sheet.GetRange(COleVariant(A),covOptional)");
		Range cols = fitRange.GetEntireColumn();							//OutputDebugString("[KSJ] fitRange.GetEntireColumn()");
		cols.AutoFit();														//OutputDebugString("[KSJ] cols.AutoFit()");
	}
	CATCH(COleDispatchException, e)
	{
		::MessageBox(NULL, e->m_strDescription, e->m_strSource, MB_OK|MB_ICONERROR|MB_SYSTEMMODAL);
	}
	END_CATCH;

	excel.SetVisible(TRUE);				//OutputDebugString("[KSJ] excel.SetVisible(TRUE)");
	excel.ReleaseDispatch();			//OutputDebugString("[KSJ] excel.ReleaseDispatch()");
	
}

COLORREF CMapWnd::getColor(int color)
{
	if (color & 0x02000000)
		return color;

	return m_parent->SendMessage(WM_USER, MAKEWPARAM(getPALETTE, 0), (LPARAM)color);
}

CFont* CMapWnd::getFont(int point, CString name, int bold, bool italic)
{
	struct	_fontR	fontR;

	if (name.IsEmpty())
		name = F_NAME;
	fontR.name   = (char *)name.operator LPCTSTR();
	fontR.point  = point;
	fontR.italic = italic;
	fontR.bold   = bold;

	return (CFont *)m_parent->SendMessage(WM_USER, MAKEWPARAM(getFONT, 0), (LPARAM)&fontR);
}

CBrush* CMapWnd::getAxBrush(COLORREF clr)
{
	return (CBrush*)m_parent->SendMessage(WM_USER, getBRUSH, (long)clr);
}

HBITMAP CMapWnd::getBitmap(CString bmps)
{
	CString	path;

	path.Format("%s\\%s\\%s", m_root, IMAGEDIR, bmps);
	return ((CBitmap *)m_parent->SendMessage(WM_USER, MAKEWPARAM(getBITMAP, 0), (LPARAM)(LPCTSTR)path))->operator HBITMAP();
}

long CMapWnd::variant(int key, CString data)
{
	return m_parent->SendMessage(WM_USER, MAKEWPARAM(variantDLL, key), (LPARAM)(LPCTSTR)data);
}

int CMapWnd::getCodeType(CString strcode)
{
	return m_parent->SendMessage(WM_USER, MAKEWPARAM(codeTYPE, 0), (LPARAM)(LPCTSTR)strcode);
}

////////////////////////// 
// 함수명	    : CMapWnd::OnSelChangeSearchCombo
// 설 명	    : 메인 콤보박스의 항목을 변경했을 때 호출되는 함수
// 작성일시	    : 2009/05/26
// 작성자       : kbkim
// 리턴 값	    : void 
// 관련요건	    : 000010(조원형:2009/05/12)
////////////////////////// 
void CMapWnd::OnSelChangeSearchCombo() 
{
	int nSelIndex = 0;
	Invalidate();

	CtrlAllHidden();
	m_code->DeleteAllItems();
	m_code->MoveWindow(CZ_ALL_CODELIST);
	if(m_rtype->GetSafeHwnd())
	{
		m_rtype->ShowWindow(SW_HIDE);
		m_bstock->ShowWindow(SW_HIDE);
		m_icorp->ShowWindow(SW_HIDE);
		m_month->ShowWindow(SW_HIDE);
	}

	m_scode->DeleteAllItems();
	m_sitem->DeleteAllItems();

	m_pNameSearch->SetWindowText("");	//기존 검색이름 삭제

	nSelIndex = m_pSelList->GetCurSel();
	switch(nSelIndex)
	{
	case SEL_ALL:
		CtrlAllShow();
		loadCode();
		break;
	case SEL_THEME:
		CtrlThemeShow();
		m_type = cKospi;		//2016.08.24 KSJ 콤보박스를 선택하면 항목을 코스피로 변경
		loadItem();
		break;
	case SEL_REMAINDER:
		CtrlRemainderShow();
		m_type = cKospi;		//2016.08.24 KSJ 콤보박스를 선택하면 항목을 코스피로 변경
		loadItem();
		break;
	case SEL_INTEREST:
		CtrlInterestShow();
		if(m_pInterestGroupList->GetCount() == 0)
			SetInterest();
		OnSelChangeGroupList();
		m_type = cKospi;		//2016.08.24 KSJ 콤보박스를 선택하면 항목을 코스피로 변경
		loadItem();
		break;
// 	case SEL_KRX:
// 		CtrlKRXShow();
// 		break;
	}
}

void CMapWnd::OnRemain() 
{
	CString strEncPass(_T(""));
	CString strUser(_T(""));
	CString strSendData(_T(""));
	struct _ledgerH ledger {};
	struct s_mid mid {};
	
	m_pRemainderPassword->GetWindowText(m_strPassword);

	if (m_strPassword.IsEmpty())
	{
		AfxMessageBox("비밀번호를 입력하세요");
		return;
	}
	if (m_strPassword.GetLength()<4)
	{
		MessageBox("비밀번호를 확인하십시오.","IBK투자증권");
		return;
	}
	m_strAccount = m_pRemainderAcc->GetAccNo();
	strUser = Variant(userCC, "");
	strEncPass.Format("%s\t%s", m_strPassword, m_strAccount.Left(8));
	m_strPassword = (char *)m_parent->SendMessage(WM_USER, MAKEWPARAM(encPASSx, modeACN), (LPARAM)(LPCTSTR)strEncPass);
	
	::FillMemory(&ledger, L_ledgerH, ' ');
	
	m_parent->SendMessage(WM_USER, ledgerDLL, (LPARAM)&ledger);
	::CopyMemory(&ledger.svcd, _T("SONAQ052"), sizeof(ledger.svcd));
	::CopyMemory(&ledger.usid, (LPCTSTR)strUser, strUser.GetLength());
	::CopyMemory(&ledger.brno, m_strAccount.Left(3), 3);
	::CopyMemory(&ledger.rcnt, _T("0000"), sizeof(ledger.rcnt));
	
	ledger.fkey[0] = 'C';
	ledger.mkty[0] = '2';
	ledger.odrf[0] = '1';
	
	strSendData = CString((char*)&ledger, L_ledgerH);
	
	::FillMemory(&mid, L_smid, ' ');
	
//	strAccount = "002-01-000010";
	::CopyMemory(&mid.accn, m_strAccount, sizeof(mid.accn));
	::CopyMemory(&mid.pswd, m_strPassword, m_strPassword.GetLength());
	mid.allf[0] = '1';
	
	strSendData += CString((char*)&mid, L_smid);

	SendTR("PIBOSJGO", strSendData, KEY_REMAIN, 0);
}

BOOL CMapWnd::SendTR(CString strTR, CString strData, int nKey, int nStat)
{
	CString strTRData = "";

	struct _userTH user;
	strncpy(&user.trc[0], strTR, 8);
	user.key  = nKey;
	user.stat = nStat;
	
	strTRData = CString((char*)&user, sizeof(_userTH));
	strTRData += strData;
	
	return m_parent->SendMessage(WM_USER, MAKEWPARAM(invokeTRx, strTRData.GetLength()), (LPARAM)(LPCTSTR)strTRData);
}

CString CMapWnd::Variant(int nComm, CString strData)
{
	CString strRet(_T(""));
	
	char* pData = (char*)m_parent->SendMessage(WM_USER, MAKEWPARAM(variantDLL, nComm), (LPARAM)(LPCTSTR)strData);
	
	if ((long)pData > 1)
		strRet = pData;
	
	return strRet;
}

BOOL CMapWnd::loadJCode()
{
	if (m_arCode.GetSize() > 0) return false;
	
	CFile	fileH;
	CString	filePath;
	struct  hjcodex  hjcode {};
	struct	_code	code;
	
	filePath.Format("%s\\%s", m_sTabDir, HJCODEx);
	if (!fileH.Open(filePath, CFile::modeRead|CFile::typeBinary))
	{
		AfxMessageBox("종목코드가 없습니다.");
		return false;
	}
	
	int idx = 0;
	const int cnt = (int)(fileH.GetLength() / sizeof(struct hjcodex));
	for (int ii = 0; ii < cnt; ii++)
	{
		fileH.Read(&hjcode, sizeof(struct hjcodex));
		
		if (hjcode.ssgb != 0)
		{
			switch (hjcode.ssgb)
			{
			case jmETF:	case jmELW:
				break;
			default:
				continue;
			}
		}
		
//		code.codeS = CString(hjcode.code, HCodeLen + 2).Mid(1);
		code.codeS = CString(hjcode.code, HCodeLen).Mid(1);

		code.nameS = CString(hjcode.hnam, HNameLen);
		idx = code.nameS.Find('\0');
		if (idx != -1) 	code.nameS = code.nameS.Left(idx);
		
		if (hjcode.ssgb != jmELW)
			code.nameS = code.nameS.Mid(1);
		code.nameS.MakeUpper(); 
		code.nameS.TrimRight();
		
		code.kosd = hjcode.kosd;
		code.ssgb = hjcode.ssgb;
		
		m_arCode.Add(code);
	}
	fileH.Close();
	return true;
}

void CMapWnd::OnSelChangeThemeList() 
{
	CString strComboData;
	struct s_gmid gmid {};
	
	m_pThemeList->GetLBText(m_pThemeList->GetCurSel(), strComboData);
	::ZeroMemory(&gmid, sizeof(gmid));
	
	if(strComboData.GetLength()>4)
	{
		::CopyMemory(gmid.tmcd, (LPCTSTR) strComboData.Mid(1,4), sizeof(gmid.tmcd));
		::CopyMemory(gmid.find, (LPCTSTR) "", sizeof(gmid.find));
		

		inter->sendTR ("pibo2701", (char*)&gmid, sizeof(gmid), KEY_THEMENAME);

//		SendTR("pibo2701", (char *)&gmid, KEY_THEMENAME, 0);
	}
}

CString CMapWnd::getCodeName(CString codeS, int& gubn)
{
	gubn = 0/*CG_NOT*/;
	const int codeL = codeS.GetLength();
	int ii = 0;
	if (codeL == 6)					// kospi code
	{
		_code   code; 
		for (ii = 0; ii < m_arCode.GetSize(); ii++)
		{
			code = m_arCode.GetAt(ii);
			if (codeS != code.codeS)
				continue;
			if (code.kosd == jmKOSPI)
				gubn = CG_KOSPI;
			else if (code.kosd == jmKOSDAQ)
				gubn = CG_KOSDAQ;
			return code.nameS;
		}
	}

	return _T("");
}

void CMapWnd::SelThemeList() 
{
	struct mid_aa gmid;
	CString strSearchData;

	m_pThemeList->ResetContent();

	::ZeroMemory(&gmid, sizeof(gmid));
	GetDlgItem(IDC_THEME_SEARCH)->GetWindowText(strSearchData);

	::CopyMemory(gmid.find, (LPCTSTR)strSearchData, sizeof(gmid.find)); // 테마 항목의 검색어 데이터

	SendTR("ho270102", (char *)&gmid, KEY_THEMELIST, 0); // 테마 콤보 박스에 대한 항목을 받기 위해 Tr을 생성 
}

CString CMapWnd::ParserThemeList(CString &srcstr, CString substr)
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

////////////////////////// 
// 함수명	    : CMapWnd::OnSelChangeGroupList
// 설 명	    : 관심종목의 항목이 변경되었을 때 호출
// 작성일시	    : 2009/05/26
// 작성자       : kbkim
// 리턴 값	    : void 
// 관련요건	    : 000010(조원형:2009/05/12)
////////////////////////// 
void CMapWnd::OnSelChangeGroupList() 
{
	CString	gname;
	m_code->DeleteAllItems();
	m_inters.RemoveAll();


	const int nSelIndex = m_pInterestGroupList->GetItemData(m_pInterestGroupList->GetCurSel());
	inter->Request_GroupCode(nSelIndex, 0);
	return;
	
/*
	const int count = loadingInterest(nSelIndex);
	
	CString strCode, strName;
	_inters* pinter{};

	for (int ii = 0; ii < count; ii++)
	{
		pinter = m_inters.GetAt(ii);

		strCode = CString(pinter->code, 12);	//2016.08.24 KSJ 6 -> 12로 변경
		strName = CString(pinter->name, 32);

		strCode.Replace('\0', ' '); 
		strCode.TrimLeft();
		strCode.TrimRight();

		if(strCode.GetLength() <= 4) // 업종 관련 종목은 표시하지 않음
			continue;

		addCode(strCode, strName);
	}
*/
}

int CMapWnd::loadingInterest(int gno)
{
/*
	CString	filePath;
	CString sName;
	sName = Variant(nameCC, "");

	if (ExistFile(gno, true))
		filePath.Format("%s/%s/%s/portfolio.i%02d.tmp", m_root, USRDIR, sName, gno);
	else if (ExistFile(gno, FALSE))
		filePath.Format("%s/%s/%s/portfolio.i%02d", m_root, USRDIR, sName, gno);
	else
		return 0;
	
	UINT	readL=0;
	struct	_inters* pinter{};
	
	CFile	fileH(filePath, CFile::modeRead);
	
	for (int ii = 0; ii < 100; ii++)
	{
		pinter = (_inters *) new char[sz_inters];
		ZeroMemory(pinter, sz_inters);
		
		readL = fileH.Read(pinter, sz_inters);

		if (readL < sz_inters)
		{
			delete pinter;
			break;
		}

		m_inters.Add(pinter);
	}
	
	fileH.Close();
	return m_inters.GetSize();
*/
	return 0;
}

BOOL CMapWnd::ExistFile(int gno, bool temp /*=false*/)
{
	CString	filePath;
	CString sName;
	sName = Variant(nameCC, "");

	if (temp)
		filePath.Format("%s/%s/%s/portfolio.i%02d.tmp", m_root, USRDIR, sName, gno);
	else
		filePath.Format("%s/%s/%s/portfolio.i%02d", m_root, USRDIR, sName, gno);
	
	CFileFind finder;
	if (!finder.FindFile(filePath))
		return false;
	
	return true;
}

// 차후에 필요하면 기능 추가
void CMapWnd::OnSelChangeKRXTypeList() 
{

}

LRESULT CMapWnd::OnMessage(WPARAM wParam, LPARAM lParam)
{
	mod* lsmod{};
	
	switch(LOBYTE(LOWORD(wParam))) 
	{
	case invokeTRx:
		if (HIWORD(wParam))
		{
			switch (HIBYTE(LOWORD(wParam)))
			{
			case 255:		// CX_Account(입력 계좌 설정)
			case 254:
				m_pRemainderAcc->m_pAccountCtrl->SendMessage(WM_USER, wParam, lParam);
				m_pRemainderPassword->SetWindowText("");
				break;
			case KEY_THEMELIST:
				SetThemeComboList((char*)lParam);
				break;
			case KEY_THEMENAME:
				lsmod = (mod*)(char*)lParam;
				SetThemeGroupName(lsmod);
				break;
			case KEY_REMAIN:
				SetRemainList((char*)lParam);
				break;
			case TRKEY_GROUP:
			{
				CString data = (char*)lParam;
				inter->receiveOub(HIBYTE(LOWORD(wParam)), data, 0);

				m_pInterestGroupList->ResetContent();
				for (int i = 0; i < inter->m_group_count; i++)
				{
					const int index = m_pInterestGroupList->AddString(inter->m_groupname[i].first);
					m_pInterestGroupList->SetItemData(index, atoi(inter->m_groupname[i].second));
				}
			}
			break;
			case TRKEY_INTER:
			{
				CString data = (char*)lParam;
				inter->receiveOub(TRKEY_INTER, data, 0);

				CString	gname;
				m_code->DeleteAllItems();
				m_inters.RemoveAll();

				const int nSelIndex = m_pInterestGroupList->GetItemData(m_pInterestGroupList->GetCurSel());
				const int count = inter->m_inters.GetCount();

				CString strCode, strName;
				_inters* pinter{};

				for (int ii = 0; ii < count; ii++)
				{
					pinter = inter->m_inters.GetAt(ii).get();

					strCode = CString(pinter->code, 12);	//2016.08.24 KSJ 6 -> 12로 변경
					strName = CString(pinter->name, 32);

					strCode.Replace('\0', ' ');
					strCode.TrimLeft();
					strCode.TrimRight();

					//					if (strCode.GetLength() <= 4) // 업종 관련 종목은 표시하지 않음
					//						continue;

					addCode(strCode, strName);
				}
				break;
			}
			}
		}
		break;
	case DLL_SETPAL:
		ChangeColors();
		break;
	case DLL_GUIDE:
		return TRUE;
	}
	return FALSE;
}

CString CMapWnd::Parser(CString &strSrc, char cDel)
{
	CString strReturn;
	strReturn.Empty();
	if (strSrc.Find(cDel) < 0)
	{
		strReturn = strSrc;
		strSrc.Empty();
	}
	else
	{
		strReturn = strSrc.Left(strSrc.Find(cDel));
		strSrc = strSrc.Mid(strSrc.Find(cDel) + 1);
	}
	return strReturn;
}

LONG CMapWnd::OnUser(WPARAM wParam, LPARAM lParam)
{
	CStringArray arArray;
	CString strToken = "\t";// tabCHAR;
	CString strData = CString((char*)lParam);
	switch( LOBYTE(LOWORD(wParam)) )
	{
	case CHANGEACCOUNT:
		SplitString(strData, strToken, arArray, 3);
		m_pRemainderName->SetWindowText(arArray[1]);
		m_pRemainderPassword->SetWindowText("");
		m_pRemainderPassword->SetFocus();

		break;
	}
	
	return 0;
}

int CMapWnd::SplitString(CString strData, CString strToken, CStringArray &astrResult, int imaxcnt)
{
	if(strToken == "") return 0;
	
	astrResult.RemoveAll();
	int iFind = 0;
	while(1)
	{
		iFind = strData.Find(strToken); 
		if(iFind < 0 ) 
		{
			if(!strData.IsEmpty() )astrResult.Add(strData);
			break;
		}
		
		astrResult.Add( strData.Left(iFind) ); 
		strData = strData.Right(strData.GetLength() - iFind - strToken.GetLength() );  
		if(strData == "")break;
		if(imaxcnt < astrResult.GetSize() ) break;
	}
	
	return astrResult.GetSize(); 
}

LRESULT CMapWnd::OnEditExMsg(WPARAM wParam, LPARAM lParam)
{
	CString	string;
	m_pNameSearch->GetWindowText(string);
	
	switch ((int) wParam)
	{	
	case wpBLANK:
		//m_search.GetWindowText(string);
		
		searchAction(0);	// blank name search by kwon
		//selcMark(&m_list1, 1);
		break;
	case wpENTER:		
		if(m_pThemeSearch->IsWindowVisible() && m_pThemeSearch->GetWindowTextLength() > 0)
		{
			m_code->DeleteAllItems();
			SelThemeList();
		}
		else if(m_pNameSearch->IsWindowVisible() && m_pNameSearch->GetWindowTextLength() > 0)
		{
			
		}
		break;
	case wpSEARCH1:
		searchAction(0);	// number search by kwon
		break;
	case wpSEARCH2:
		CString temp = string.GetAt(0);
		if(atoi(temp) >= 5 && atoi(temp) < 9)
		{
			searchAction(0);	// char search by kwon
		}
		else
		{
			searchAction(1);	// char search by kwon
		}
		
		break;
	}
	return TRUE;
}

void CMapWnd::searchAction(int column)
{
	CString	string; 
	m_pNameSearch->GetWindowText(string);
	m_code->UpdateData(false);
	m_codedata.RemoveAll();
	_listitem* lim{};
	if (column == 0)	// number search
	{
		if (!m_bAddAll)
		{
			CString	code, name;
			_listitem* listitem{};
			int	position = 0;
			m_code->DeleteAllItems();

			for_each(m_listitem.begin(), m_listitem.end(), [&](auto item) {
				listitem = (_listitem *)item.get();

				name = CString(listitem->name, sizeof(listitem->name));
				code = CString(listitem->code, sizeof(listitem->code));

				if (!m_codedata.Lookup(code, (void*&)lim))
				{
					AppendItemToList1(position, code, name, listitem);
					position += 1;
				}

			});

			m_bAddAll = true;
		}

		if (m_code->GetItemCount() > 0)
		{
			int	idx = -1;
			if (string.GetLength() > 0)
			{
// 				m_listsort = false;
// 				ListView_SortItems(m_list1, ListViewCompareFunc, column);

				deselcMark(m_code.get());

				idx = findMatchIndex(string, column);
				if (idx >= 0) selcMark(m_code.get(), idx);
			}
			else
			{
				deselcMark(m_code.get());
				selcMark(m_code.get(), 0);
			}
		}
	}
	else			// name search
	{
		m_bAddAll = SearchData(string, m_bAddAll);
		if (m_code->GetItemCount() > 0)
		{
// 			m_listsort = false;
//			ListView_SortItems(m_code, ListViewCompareFunc, column);

 			deselcMark(m_code.get());
 			selcMark(m_code.get(), 0);
		}
	}
	m_code->UpdateData(true);
}

bool CMapWnd::SearchData(CString sName, bool bAddAll)
{
	CString	code, name, rt06, rate, idtp, sdat, rknm, shgb;
	_listitem* listitem{};
	CArray <_listitem*, _listitem*> searchlist;
	
	int	position = 0;

	for_each(m_listitem.begin(), m_listitem.end(), [&](auto item) {
		searchlist.Add(item.get());
	});

	if (sName.IsEmpty())
	{
		if (!bAddAll)
		{
			m_code->DeleteAllItems();
			for (int ii = 0; ii < searchlist.GetSize(); ii++)
			{
				listitem = searchlist.GetAt(ii);

				name = CString(listitem->name, sizeof(listitem->name));
				code = CString(listitem->code, sizeof(listitem->code));
				
				AppendItemToList1(position, code, name, listitem);
				
				position += 1;
			}
		}
		return true;
	}

	m_code->DeleteAllItems();

	char	cName{};
	const int	sLen = sName.GetLength();
	if (sLen == 1)
	{	// 일반검색적용(길이만큼 맞는것만 적용)
		const int	sLen = sName.GetLength();
		for (int ii = 0; ii < searchlist.GetSize(); ii++)
		{
			listitem = searchlist.GetAt(ii);

			name = CString(listitem->name, sizeof(listitem->name));
			if (strncmp(sName, name, sLen) != 0)
				continue;

			code = CString(listitem->code, sizeof(listitem->code));
			AppendItemToList1(position, code, name, listitem);
			position += 1;
		}
	}
	else
	{	// 한글 검색
		WORD	wHangul{}, wHangul2{}, wStart{}, wEnd{};
		CPtrArray	arSearch;
		arSearch.RemoveAll();
		int ii = 0;

		cName = sName.GetAt(ii);
		if (cName & HANGULMSK)	// 한글일땐 2Byte
		{
			wHangul = MakeHangul(sName.GetAt(ii), sName.GetAt(ii+1));
			if (IsChosung(wHangul, wStart, wEnd))
			{	// 초성 비교
				for (int jj = 0; jj < searchlist.GetSize(); jj++)
				{
					listitem = searchlist.GetAt(jj);
					name = CString(listitem->name, sizeof(listitem->name));
					//앞공백제거
					name.TrimLeft();

					if (name.GetLength() < 2)	continue;
					wHangul2 = MakeHangul(name.GetAt(ii), name.GetAt(ii+1));
					if (wStart <= wHangul2 && wHangul2 <= wEnd)	arSearch.Add(listitem);
				}
			}
			else
			{	// 한글 비교

				for (int jj = 0; jj < searchlist.GetSize(); jj++)
				{
					listitem = searchlist.GetAt(jj);
					name = CString(listitem->name, sizeof(listitem->name));
					//앞공백제거
					name.TrimLeft();

					if (name.GetLength() < 2)	continue;
					wHangul2 = MakeHangul(name.GetAt(ii), name.GetAt(ii+1));
					if (wHangul == wHangul2)	arSearch.Add(listitem);
				}
			}
			ii += 2;
		}
		else			// 그외는 1Byte
		{
			for (int jj = 0; jj < searchlist.GetSize(); jj++)
			{
				listitem = searchlist.GetAt(jj);
				name = CString(listitem->name, sizeof(listitem->name));

				if (name.GetLength() < 1)	continue;
				if (cName == name.GetAt(ii))	arSearch.Add(listitem);
			}
			ii += 1;
		}

		for (; ii < sLen && arSearch.GetSize();)
		{
			cName = sName.GetAt(ii);
			if (cName & HANGULMSK)	// 한글일땐 2Byte
			{
				wHangul = MakeHangul(sName.GetAt(ii), sName.GetAt(ii+1));
				if (IsChosung(wHangul, wStart, wEnd))
				{	// 초성 비교
					for (int jj = arSearch.GetUpperBound(); jj >= 0; jj--)
					{
						listitem = (_listitem*)arSearch.GetAt(jj);
						name = CString(listitem->name, sizeof(listitem->name));
						//앞공백제거
						name.TrimLeft();

						if (name.GetLength() < ii+2)	arSearch.RemoveAt(jj);;
						wHangul2 = MakeHangul(name.GetAt(ii), name.GetAt(ii+1));
						if (wStart > wHangul2 || wHangul2 > wEnd)	arSearch.RemoveAt(jj);
					}
				}
				else
				{	// 한글 비교

					for (int jj = arSearch.GetUpperBound(); jj >= 0; jj--)
					{
						listitem = (_listitem*)arSearch.GetAt(jj);
						name = CString(listitem->name, sizeof(listitem->name));
						//앞공백제거
						name.TrimLeft();

						if (name.GetLength() < ii+2)	
							arSearch.RemoveAt(jj);
						wHangul2 = MakeHangul(name.GetAt(ii), name.GetAt(ii+1));
						if (wHangul != wHangul2)
							arSearch.RemoveAt(jj);
					}
				}
				ii += 2;
			}
			else			// 그외는 1Byte
			{
				for (int jj = arSearch.GetUpperBound(); jj >= 0; jj--)
				{
					listitem = (_listitem*)arSearch.GetAt(jj);
					name = CString(listitem->name, sizeof(listitem->name));

					if (name.GetLength() < ii+1)	arSearch.RemoveAt(jj);
					if (cName != name.GetAt(ii))	arSearch.RemoveAt(jj);
				}
				ii += 1;
			}
		}

		for (ii = 0; ii < arSearch.GetSize(); ii++)
		{
			listitem = (_listitem*)arSearch.GetAt(ii);

			name = CString(listitem->name, sizeof(listitem->name));
			code = CString(listitem->code, sizeof(listitem->code));

			AppendItemToList1(position, code, name, listitem);
			position += 1;
		}
	}

	return false;
}

WORD CMapWnd::MakeHangul(char cChar1, char cChar2)
{	// 2Byte문자로부터 한글을 맹글어주라~~
	const WORD	wHangul = (cChar1<<8) | (0x00FF & cChar2);
	return wHangul;
}

bool CMapWnd::IsChosung(WORD wHangul, WORD &wStart, WORD &wEnd)
{	// 초성만 가진 한글인지~~
	if (ChosungS > wHangul || wHangul > ChosungE)
		return false;
	
	const WORD	pChosung[19] = 
	{
		0xA4A1, 0xA4A2, 0xA4A4, 0xA4A7, 0xA4A8, 0xA4A9, 0xA4B1,	// ㄱ,ㄲ,ㄴ,ㄷ,ㄸ,ㄹ,ㅁ
			0xA4B2, 0xA4B3, 0xA4B5, 0xA4B6, 0xA4B7, 0xA4B8, 0xA4B9,	// ㅂ,ㅃ,ㅅ,ㅆ,ㅇ,ㅈ,ㅉ
			0xA4BA, 0xA4BB, 0xA4BC, 0xA4BD, 0xA4BE			// ㅊ,ㅋ,ㅌ,ㅍ,ㅎ
	};
	
	const WORD	pHangulStart[19] = 
	{
		0xB0A1, 0xB1EE, 0xB3AA, 0xB4D9, 0xB5FB, 0xB6F3, 0xB8B6,	// 가,까,나,다,따,라,마
			0xB9D9, 0xBAFC, 0xBBE7, 0xBDCE, 0xBEC6, 0xC0DA, 0xC2A5,	// 바,빠,사,싸,아,자,짜
			0xC2F7, 0xC4AB, 0xC5B8, 0xC6C4, 0xC7CF			// 차,카,타,파,하
	};
	const WORD	pHangulEnd[19] = 
	{
		0xB1ED, 0xB3A9, 0xB4D8, 0xB5FA, 0xB6F2, 0xB8B5,		
			0xB9D8, 0xBAFB, 0xBBE6, 0xBDCD, 0xBEC5, 0xC0D9, 0xC2A4,	
			0xC2F6, 0xC4AA, 0xC5B7, 0xC6C3, 0xC7CE, 0xC8FE			
	};
	
	int ii = 0;
	for (ii = 0; ii < 19; ii++)
	{
		if (wHangul == pChosung[ii])
		{
			wStart = pHangulStart[ii];
			wEnd = pHangulEnd[ii];
			return true;
		}			
	}
	
	wStart = pHangulStart[ii] - 1;
	wEnd = pHangulEnd[ii] + 1;
	
	return true;
}

void CMapWnd::AppendItemToList1(int ipos, CString code, CString name,  _listitem *pListItem)
{
	int nItem = 0;
	std::shared_ptr<_listitem> listitem{};
	
	name.TrimRight();
	
	if (pListItem)
	{
		nItem = m_code->GetItemCount();
		m_code->SetItemData(nItem, (LPARAM) pListItem);
		addItem(m_code.get(), code, name, nItem);
	}
	else	// 처음 종류를 선택할때만 추가된다.
	{
		listitem = std::make_shared<_listitem>();
		
		::CopyMemory(listitem->code, code, strlen(code));
		::CopyMemory(listitem->name, name, strlen(name));
		
		m_listitem.push_back (listitem);
		m_code->SetItemData(nItem, (LPARAM) listitem.get());
	}
}

int CMapWnd::findMatchIndex(CString text)
{
	const int nameCOL = 1;

	CString	string;
	const int	textL = text.GetLength();
	for (int ii = 0; ii < m_code->GetItemCount(); ii++)
	{
		string = m_code->GetItemText(ii, nameCOL);
		string = string.Mid(1);

		if (strncmp(text, string, textL) <= 0)
			return ii;
	}
	return -1;
}

int CMapWnd::findMatchIndex(CString text, int column)
{
	CString	string;
	const int	textL = text.GetLength();
	for (int ii = 0; ii < m_code->GetItemCount(); ii++)
	{
		string = m_code->GetItemText(ii, column);
		if (column == 1) string = string.Mid(1);

		if (!strncmp(text, string, textL))
			return ii;
	}
	return -1;
}

void CMapWnd::deselcMark(CListCtrl* listctrl)
{
	int nItem{};
	POSITION pos = listctrl->GetFirstSelectedItemPosition();
	if (pos != NULL)
	{
		while (pos)
		{
			nItem = listctrl->GetNextSelectedItem(pos);
			listctrl->SetItemState(nItem, NULL, LVIS_SELECTED|LVIS_FOCUSED);
		}
	}
}

void CMapWnd::selcMark(CListCtrl* listctrl, int nItem, bool scroll /*=true*/)
{
	CRect	rc; CSize size;
	
	listctrl->SetItemState(nItem, LVIS_SELECTED|LVIS_FOCUSED, LVIS_SELECTED|LVIS_FOCUSED);
	
	if (scroll)
	{
		listctrl->GetItemRect(0, rc, LVIR_LABEL);
		size.cx = 0;
		size.cy = rc.Height() * (nItem - listctrl->GetTopIndex());
		listctrl->Scroll(size);
	}
}

void CMapWnd::SetRemainList(char* szReceive)
{
	CString sLedger = CString(szReceive, L_ledgerH);
	CString sMessage;
	BOOL bnext = FALSE;
	BOOL berrcode;
	berrcode = GetLedgerMessage(sLedger, sMessage, bnext);
	
	m_code->DeleteAllItems();
	m_scode->DeleteAllItems();
	m_sitem->DeleteAllItems();
	if(berrcode)
	{
		s_mod* mo = (s_mod*)(&szReceive[L_ledgerH]);
		int iRowCnt = atoi(CString(mo->nrec, sizeof(mo->nrec)));
		CString sReceive = (char*)mo->rec;
		CString sCode;
		CString sName;
		CString sRow;

		m_CodeList.RemoveAll();
		for(int i=0; i<iRowCnt; i++)
		{
			sRow = sReceive.Left(186/*SJGOROWLEN*/); 
			if( SetJangoFormat(sRow, sCode, sName) < 0) 
				continue;
			addCode(sCode, sName);
			sReceive = sReceive.Right(sReceive.GetLength() -  194);
		}
	}
}

int CMapWnd::SetJangoFormat(CString sRows, CString& strCode, CString& strName)
{
	const int iLes[] = {12, 40, 2, 10, 10, 10, 15, 10, 10, 10, 15, 15, 10, 2, 8, 15};//struct s_rec
	
	for(int i=0; i<3; i++)
	{
		if(sRows.GetLength() <  iLes[i]) break;
		
		if(i==0)
		{	
			strCode = sRows.Mid(1, 6);
			if(m_CodeList.Find(strCode) != NULL) // 중복 항목 스킵
				return -1;

			m_CodeList.AddTail(strCode);
		}
		else if(i==1)
			strName = CString(sRows.Left( iLes[i]), iLes[1]-iLes[0]);

		sRows = sRows.Right( sRows.GetLength() -   iLes[i] );
	}
	
	return 0; 
}

BOOL CMapWnd::GetLedgerMessage(CString sLedger, CString& sMessage, BOOL& bNext)
{
	struct	_ledgerH ledger;
	::CopyMemory(&ledger, sLedger, L_ledgerH);
	
	//	char	emsg[99];	// 241	오류메세지	코드(4)+메시지(94)(9999 오류입니다 !)
	sMessage = CString(ledger.emsg, 99) ;
	int iErrCode = atoi(sMessage.Left(4)); 
//	sMessage = Trim( sMessage.Right(sMessage.GetLength() -4) );
	
	bNext = false;
	if( CString(ledger.next, 1) == "Y" ) 
	{
		bNext = true;
//		m_strNextKey = ledger.nkey ;
	}
	
	if(iErrCode > 999) return false;
	return true;
}

void CMapWnd::SetThemeGroupName(mod *lsmod) 
{
	CString strCodeNum;
	CString strCodeName;
	int nGridCount;

	nGridCount = atoi(CString(lsmod->nrec, sizeof(lsmod->nrec)));

	m_code->DeleteAllItems();
	for(int n =0;n<nGridCount;n++)
	{
		strCodeNum = CString(lsmod->grid[n].code, sizeof(lsmod->grid[n].code));
		strCodeName = CString(lsmod->grid[n].hnam, sizeof(lsmod->grid[n].hnam));
		strCodeNum.TrimRight();
		strCodeName.TrimRight();
		addCode(strCodeNum,strCodeName);
	}
}

void CMapWnd::SetThemeComboList(CString strGetData) 
{
	int nFindIndex = -1;
	CString strData = strGetData;
	CString strtoken = "|";
	CString str;
	CString strComboData;

	m_pThemeList->ResetContent();
	while(1)
	{
		nFindIndex = strData.Find(strtoken);
		if(nFindIndex == -1)
			break;
		strComboData.Format("[%s] %s", strData.Mid(0,4), strData.Mid(5, nFindIndex-5));
		m_pThemeList->AddString(strComboData);
		str = strData.Mid(0, nFindIndex);
		strData = strData.Mid(nFindIndex+1);
	}

	if(m_pThemeList->GetCount() == 0)
	{
		m_code->DeleteAllItems();
		return;
	}
		
	if(m_pThemeList->GetCurSel() == -1)
	{
		m_pThemeList->SetCurSel(0);
		OnSelChangeThemeList();
	}
}

COLORREF CMapWnd::GetAxColor(int iColor)
{
	if (iColor & 0x02000000)
		return iColor;
	return m_parent->SendMessage(WM_USER, MAKEWPARAM(getPALETTE, 0), (LPARAM)iColor);
}

int CALLBACK compare(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort) 
{
	CString	item1, item2;
	struct	_listR* list1 = (struct _listR *)lParam1;
	struct	_listR* list2 = (struct _listR *)lParam2;

	if ((int)lParamSort == 0)
	{
		item1 = CString(list1->code);
		item2 = CString(list2->code);
	}
	else
	{
		item1 = CString(list1->name);	item1.TrimLeft();
		item2 = CString(list2->name);	item2.TrimLeft();
	}

	return strcmp(item1, item2); 
}

HBRUSH CMapWnd::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CWnd::OnCtlColor(pDC, pWnd, nCtlColor);
	pDC->SetBkMode(TRANSPARENT);
	// TODO: Return a different brush if the default is not desired

	const int nID = pWnd->GetDlgCtrlID();
	if( nID == IDC_KOSPI	||	nID == IDC_KOSDAQ	||
		nID == IDC_ELW		||	nID == IDC_FUTURE	||
		nID == IDC_OPTION	||	nID == IDC_INDEX	||
		nID == IDC_HORZ		||	nID == IDC_VERT		||
		nID == IDC_PROGRAM	||	nID == IDC_TUJAJA	||
		nID == IDC_NUMBER)
		return m_hRoundColor;

	return hbr;
}
