// DialogConfirm.cpp : implementation file
//

#include "stdafx.h"
#include "DialogConfirm.h"
#include "TextWnd.h"
#include "../../control/fx_misc/fxImgButton.h"
#include "../../h/axisfire.h"
#include "../../h/axisvar.h"
#include "MapWnd.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define	C_ACCOUNT		0	// 계좌번호
#define	C_NAME			1	// 계좌명
#define	C_JM			2	// 종목명
#define	C_TRADE_KIND	3	// 매매유형
#define	C_ONE			4	// 원주문번호
#define	C_JJ_KIND		5	// 정정구분
#define	C_COUNT			6	// 주문수량
#define	C_PRICE			7	// 주문단가

#define	D_ACCOUNT		8	// 계좌번호
#define	D_NAME			9	// 계좌명
#define	D_JM			10	// 종목명
#define	D_TRADE_KIND		11	// 매매유형
#define	D_ONE			12	// 원주문번호
#define	D_JJ_KIND		13	// 정정구분
#define	D_COUNT			14	// 주문수량
#define	D_PRICE			15	// 주문단가

#define IDC_HEADBASE	1000
#define IDC_DATABASE	1100
#define IDC_CONFIRM		1200
#define	BTN2WIDTH		36

#define	INDEX_COUNT		16
#define	ROW_HEIGHT		20
#define GAP				2

#define CLR_SELL_TITLE		GetIndexColor(95)
#define CLR_SELL_BG			GetIndexColor(164)
#define CLR_SELL_LINE		GetIndexColor(165)
#define CLR_SELL_DATA_BG	GetIndexColor(171)
#define CLR_SELL_HEAD_BG	GetIndexColor(175)

#define CLR_BUY_TITLE		GetIndexColor(94)
#define CLR_BUY_BG			GetIndexColor(162)
#define CLR_BUY_LINE		GetIndexColor(163)
#define CLR_BUY_DATA_BG		GetIndexColor(170)
#define CLR_BUY_HEAD_BG		GetIndexColor(174)

#define CLR_MODIFY_TITLE	GetIndexColor(69)
#define CLR_MODIFY_BG		GetIndexColor(166)
#define CLR_MODIFY_LINE		GetIndexColor(167)
#define CLR_MODIFY_DATA_BG	GetIndexColor(172)
#define CLR_MODIFY_HEAD_BG	GetIndexColor(176)

#define CLR_CANCEL_TITLE	GetIndexColor(69)
#define CLR_CANCEL_BG		GetIndexColor(168)
#define CLR_CANCEL_LINE		GetIndexColor(169)
#define CLR_CANCEL_DATA_BG	GetIndexColor(173)
#define CLR_CANCEL_HEAD_BG	GetIndexColor(177)

#define CLR_FG				GetIndexColor(69)
#define CLR_BG				GetIndexColor(64)
#define CLR_BODYBG			GetIndexColor(181)

// Start 사용안함
#define	COLOR_STATIC_SELL_BG	GetIndexColor(164)
#define	COLOR_STATIC_BUY_BG		GetIndexColor(162)
#define	COLOR_STATIC_FIX_BG		GetIndexColor(166)
#define	COLOR_STATIC_CANCEL_BG	GetIndexColor(168)
#define	COLOR_STATIC_FG			GetIndexColor(69)
#define	COLOR_DATA_FG		GetIndexColor(69)
#define	COLOR_SELL_BORDER	GetIndexColor(165)
#define	COLOR_BUY_BORDER	GetIndexColor(163)
#define	COLOR_FIX_BORDER	GetIndexColor(167)
#define	COLOR_CANCEL_BORDER	GetIndexColor(169)
// End

#define	ID_IMAGE_BG		30
#define	ID_IMAGE_BAR		31

#define	ID_TEXT_1		40
#define	ID_TEXT_2		41
#define	ID_TEXT_3		42
#define ID_BACK			43

const char* arrHeadTop[8] = { "원주문번호", "계좌번호", "계 좌 명", "종    목", "구    분", "구    분", "수    량", "단    가" };

/////////////////////////////////////////////////////////////////////////////
// CDialogConfirm dialog


CDialogConfirm::CDialogConfirm(CRect rc, CWnd* pWizard, CWnd* pParent /*=NULL*/)
	: CDialog(CDialogConfirm::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDialogConfirm)
	//}}AFX_DATA_INIT
	m_parent = pParent;
	m_rc		= rc;
	m_pWizard	= pWizard;

	m_pTitle = NULL;
	m_pHead = NULL;
	m_pData = NULL;
	m_pConfirm = NULL;
	m_pBtnOK	= NULL;
	m_pBtnCancel	= NULL;
	
	m_bInit		= true;

	m_nOrderKind = -1;
}


CDialogConfirm::~CDialogConfirm()
{
	if (m_pTitle)
	{
		m_pTitle = nullptr;
	}

	if (m_pHead)
	{
		//delete [] m_pHead;
		m_pHead = nullptr;
	}

	if (m_pData)
	{
		delete [] m_pData;
		m_pData = NULL;
	}

	if (m_pConfirm)
	{
		m_pConfirm = nullptr;
	}	
}

void CDialogConfirm::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDialogConfirm)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CDialogConfirm, CDialog)
	//{{AFX_MSG_MAP(CDialogConfirm)
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDialogConfirm message handlers

BOOL CDialogConfirm::OnInitDialog() 
{
	CDialog::OnInitDialog();

	return Initialize();
}


BOOL CDialogConfirm::Initialize()
{
	CreateTextWnd();

	CRect rcClient;
	GetClientRect(&rcClient);

	int nX = 0;
	int nY = 0;

	nY = GetBackHeight() + 25;
	nX = rcClient.Width() - GAP - BTN2WIDTH * 2 - 6;

	CFont* pFont = RequestFont("굴림체", 9, FALSE, NULL);

	m_pBtnOK = std::make_unique<CfxImgButton>();
	m_pBtnOK->Create("\n확인", CRect(nX, nY, nX + BTN2WIDTH, nY + ROW_HEIGHT), this, IDOK);
	m_pBtnOK->SetWindowText("확인");
	m_pBtnOK->SetFont(pFont);
	m_pBtnOK->SetDrawFocus();

	nX = nX + BTN2WIDTH + 1;

	m_pBtnCancel = std::make_unique < CfxImgButton>();
	m_pBtnCancel->Create("\n취소", CRect(nX, nY, nX + BTN2WIDTH, nY + ROW_HEIGHT), this, IDCANCEL);
	m_pBtnCancel->SetWindowText("취소");
	m_pBtnCancel->SetFont(pFont);
	m_pBtnCancel->SetDrawFocus();

	CString path = "";
	CString home = GetHome();
	HBITMAP hBitmap2{}, hBitmap2_dn{}, hBitmap2_hv{};
	path.Format("%s\\%s\\", home, IMAGEDIR);

	hBitmap2    = GetAxBitmap(path + "2btn.bmp")->operator HBITMAP();
	hBitmap2_dn = GetAxBitmap(path + "2btn_dn.bmp")->operator HBITMAP();
	hBitmap2_hv = GetAxBitmap(path + "2btn_en.bmp")->operator HBITMAP();

	m_pBtnOK->SetImgBitmap(hBitmap2, hBitmap2_dn, hBitmap2_hv);
	m_pBtnCancel->SetImgBitmap(hBitmap2, hBitmap2_dn, hBitmap2_hv);

	switch (m_nOrderKind)
	{
	case 2:
		m_pData[0].SetText(m_strOne);
		m_pData[1].SetText(m_strAccount);
		m_pData[2].SetText(m_strName);
		m_pData[3].SetText(m_strJM);
		m_pData[4].SetText(_T("전부취소"));
		m_pData[5].SetText(m_strOrderCount);
		break;
	case 3:
		m_pData[0].SetText(m_strOne);
		m_pData[1].SetText(m_strAccount);
		m_pData[2].SetText(m_strName);
		m_pData[3].SetText(m_strJM);
		m_pData[4].SetText(m_strTradeKind);
		m_pData[5].SetText(_T("전부정정"));
		m_pData[6].SetText(m_strOrderCount);
		m_pData[7].SetText(m_strOrderPrice);
		break;
	default:
		m_pData[0].SetText(m_strAccount);
		m_pData[1].SetText(m_strName);
		m_pData[2].SetText(m_strJM);
		m_pData[3].SetText(m_strTradeKind);
		m_pData[4].SetText(m_strOrderCount);
		m_pData[5].SetText(m_strOrderPrice);
		break;
	}

	CRect rcWindow;
	GetWindowRect(&rcWindow);
	rcWindow.bottom = rcWindow.top + GetBackHeight() + 85 + 6;
	MoveWindow(&rcWindow);

	SetWindowText(_T("[3002] 주문::선물옵션클릭주문"));
/*	
	// m_patw[D_ORDER_KIND]->SetText(m_strOrderKind);
	m_patw[D_ACCOUNT]->SetText(m_strAccount);
	m_patw[D_NAME]->SetText(m_strName);
	m_patw[D_JM]->SetText(m_strJM);
	m_patw[D_COUNT]->SetText(m_strOrderCount);
	m_patw[D_PRICE]->SetText(m_strOrderPrice);
	m_patw[D_TRADE_KIND]->SetText(m_strTradeKind);
	m_patw[D_ONE]->SetText(m_strOne);
	m_patw[D_JJ_KIND]->SetText("전부");
	
	m_patw[C_COUNT]->SetColorFG(GetIndexColor(225));
	m_patw[C_PRICE]->SetColorFG(GetIndexColor(225));
	
	
	CFont* pfont = RequestFont("굴림체", 9, FALSE, NULL);
	m_pBtnOK = new CfxImgButton();
	m_pBtnOK->Create("\n확인", CRect(65, 257, 65 + 55, 257 + 18), this, IDOK);
	m_pBtnOK->SetWindowText("확인");
	m_pBtnOK->SetFont(pfont);
	m_pBtnOK->SetDrawFocus();
	
	m_pBtnCancel = new CfxImgButton();
	m_pBtnCancel->Create("\n취소", CRect(129, 257, 129 + 55, 257 + 18), this, IDCANCEL);
	m_pBtnCancel->SetWindowText("취소");
	m_pBtnCancel->SetFont(pfont);
	m_pBtnCancel->SetDrawFocus();
	
	CString strPath = "";
	strPath.Format("%s\\image\\", GetHome());
	HBITMAP hbmp = RequestBitmap(strPath + "3btn.bmp")->operator HBITMAP();
	HBITMAP hbmp_dn = RequestBitmap(strPath + "3btn_dn.bmp")->operator HBITMAP();
	HBITMAP hbmp_hv = RequestBitmap(strPath + "3btn_en.bmp")->operator HBITMAP();
	m_pBtnOK->SetImgBitmap(hbmp, hbmp_dn, hbmp_hv);
	m_pBtnCancel->SetImgBitmap(hbmp, hbmp_dn, hbmp_hv);
	
	CRect rectW;
	GetWindowRect(rectW);
	if (m_strOrderKind.Compare("매도") == 0 || m_strOrderKind.Compare("매수") == 0)
	{
		m_patw[C_ONE]->ShowWindow(FALSE);
		m_patw[D_ONE]->ShowWindow(FALSE);
		m_patw[C_JJ_KIND]->ShowWindow(FALSE);
		m_patw[D_JJ_KIND]->ShowWindow(FALSE);
		
		SetWindowPos(NULL, 0, 0, rectW.Width(), rectW.Height() - ROW_HEIGHT * 2, SWP_NOZORDER | SWP_NOMOVE);
		
		CWnd* awnd[] = {m_patw[C_COUNT], m_patw[D_COUNT], m_patw[C_PRICE], m_patw[D_PRICE], m_ptwStatic1, m_ptwStatic2, m_ptwStatic3, m_pBtnOK, m_pBtnCancel};
		
		for (int ii = 0; ii < 9; ii++)
		{
			CWnd* pwnd = awnd[ii];
			CRect rect;
			pwnd->GetWindowRect(rect);
			ScreenToClient(rect);
			pwnd->SetWindowPos(NULL, rect.left, rect.top - ROW_HEIGHT * 2, NULL, NULL, SWP_NOZORDER | SWP_NOSIZE);
		}
		
	}
	else if (m_strOrderKind.Compare("취소") == 0)
	{
		m_patw[C_TRADE_KIND]->ShowWindow(FALSE);
		m_patw[D_TRADE_KIND]->ShowWindow(FALSE);
		m_patw[C_PRICE]->ShowWindow(FALSE);
		m_patw[D_PRICE]->ShowWindow(FALSE);
		
		SetWindowPos(NULL, 0, 0, rectW.Width(), rectW.Height() - ROW_HEIGHT * 2, SWP_NOZORDER | SWP_NOMOVE);
		
		CWnd* awnd[] = {m_patw[C_ONE], m_patw[D_ONE], m_patw[C_COUNT], m_patw[D_COUNT], m_ptwStatic1, m_ptwStatic2, m_ptwStatic3, m_pBtnOK, m_pBtnCancel};
		
		for (int ii = 0; ii < 9; ii++)
		{
			CWnd* pwnd = awnd[ii];
			CRect rect;
			pwnd->GetWindowRect(rect);
			ScreenToClient(rect);
			pwnd->SetWindowPos(NULL, rect.left, rect.top - ROW_HEIGHT * (ii <= 1 ? 1 : 2), NULL, NULL, SWP_NOZORDER | SWP_NOSIZE);
		}
	}
	
	
	SetWindowText(m_strWindowText);
	
	CRect rectC;
	GetClientRect(&rectC);
	SetWindowPos(NULL, m_rc.left + m_rc.Width() / 2 - rectC.Width() / 2, m_rc.top + m_rc.Height() / 2 - rectC.Height() / 2,
		0, 0, SWP_NOSIZE | SWP_NOZORDER);
	
	m_pBtnOK->SetFocus();
*/	
	return TRUE;
}

void CDialogConfirm::CreateTextWnd()
{
	CString strTitle(_T(""));
	COLORREF clrTitle{};
	COLORREF clrBack{};
	COLORREF clrDataBack{};
	COLORREF clrHeadBack{};
	COLORREF clrLine{};
	int nX = GAP;
	int nY = GAP;
	CRect rcClient;
	GetClientRect(&rcClient);

	const int nWidth = rcClient.Width();
	const int nHeight = rcClient.Height();
	const int nCount = GetFieldCount();
	int nTop = 0;

//	m_pHead = std::make_unique<CTextWnd[]>(nCount);
	m_pHead = new CTextWnd[nCount];
	m_pData = new CTextWnd[nCount];

	CStringArray arrHead;

	switch (m_nOrderKind)
	{
	case -1:		// Not Setting OrderKind
		return;

	case 1:			// 매수
		strTitle.Format("※%s주문확인창", m_strOrderKind);
		clrTitle = CLR_BUY_TITLE;
		clrDataBack = CLR_BUY_DATA_BG;
		clrHeadBack = CLR_BUY_HEAD_BG;
		clrLine = CLR_BUY_LINE;
		clrBack = CLR_BUY_BG;
		nTop = 4;
		arrHead.Add(arrHeadTop[1]);
		arrHead.Add(arrHeadTop[2]);
		arrHead.Add(arrHeadTop[3]);
		arrHead.Add(arrHeadTop[4]);
		arrHead.Add(arrHeadTop[6]);
		arrHead.Add(arrHeadTop[7]);
		break;

	case 2:			// 취소
		strTitle.Format("※%s확인창", m_strOrderKind);
		clrTitle = CLR_CANCEL_TITLE;
		clrDataBack = CLR_CANCEL_DATA_BG;
		clrHeadBack = CLR_CANCEL_HEAD_BG;
		clrLine = CLR_CANCEL_LINE;
		clrBack = CLR_CANCEL_BG;
		nTop = 5;
		arrHead.Add(arrHeadTop[0]);
		arrHead.Add(arrHeadTop[1]);
		arrHead.Add(arrHeadTop[2]);
		arrHead.Add(arrHeadTop[3]);
		arrHead.Add(arrHeadTop[4]);
		arrHead.Add(arrHeadTop[6]);
		break;

	case 3:			// 정정
		strTitle.Format("※%s확인창", m_strOrderKind);
		clrTitle = CLR_MODIFY_TITLE;
		clrDataBack = CLR_MODIFY_DATA_BG;
		clrHeadBack = CLR_MODIFY_HEAD_BG;
		clrLine = CLR_MODIFY_LINE;
		clrBack = CLR_MODIFY_BG;
		nTop = 6;
		arrHead.Add(arrHeadTop[0]);
		arrHead.Add(arrHeadTop[1]);
		arrHead.Add(arrHeadTop[2]);
		arrHead.Add(arrHeadTop[3]);
		arrHead.Add(arrHeadTop[4]);
		arrHead.Add(arrHeadTop[5]);
		arrHead.Add(arrHeadTop[6]);
		arrHead.Add(arrHeadTop[7]);
		break;

	default:	// 매도
		strTitle.Format("※%s주문확인창", m_strOrderKind);
		clrTitle = CLR_SELL_TITLE;
		clrDataBack = CLR_SELL_DATA_BG;
		clrHeadBack = CLR_SELL_HEAD_BG;
		clrLine = CLR_SELL_LINE;
		clrBack = CLR_SELL_BG;
		nTop = 4;
		arrHead.Add(arrHeadTop[1]);
		arrHead.Add(arrHeadTop[2]);
		arrHead.Add(arrHeadTop[3]);
		arrHead.Add(arrHeadTop[4]);
		arrHead.Add(arrHeadTop[6]);
		arrHead.Add(arrHeadTop[7]);
		break;
	}

	const int iright = nWidth - 5;
	nX = 5;
	nY = 6;
	CFont* pfontTitle = RequestFont("굴림체", 9, FALSE, FW_BOLD);
	m_pTitle = std::make_unique<CTextWnd>();
	m_pTitle->Create(strTitle, NULL, CRect(nX, nY, iright, nY + ROW_HEIGHT), 
						this, ID_IMAGE_BG, pfontTitle, clrBack, clrTitle, clrBack);

	m_pTitle->SetAlign(DT_LEFT);

	nY += ROW_HEIGHT;

	CFont* pFont = RequestFont("굴림체", 9, FALSE, NULL);

//	nX += GAP;
	nY = nY + GAP;
	nX = 5;

	const int ix = nX + 3;
	for (int i = 0; i < nCount; i++)
	{
		if (i == nTop)
			nY += 3;

		m_pHead[i].Create(arrHead[i], NULL, CRect(ix, nY, ix + 84, nY + ROW_HEIGHT),
							this, IDC_HEADBASE + i, NULL, clrHeadBack, CLR_FG, clrLine);
		m_pHead[i].SetFontFromParent(pFont);

		m_pData[i].Create(_T(""), NULL, CRect(ix + 83, nY, iright-3, nY + ROW_HEIGHT),
							this, IDC_DATABASE + i, NULL, clrDataBack, CLR_FG, clrLine);
		m_pData[i].SetFontFromParent(pFont);

		if (i < nTop)
			m_pData[i].SetAlign(DT_LEFT);
		else
			m_pData[i].SetAlign(DT_RIGHT);

		nY = nY + ROW_HEIGHT - 1;
	}

	nY += 11;

	m_pConfirm = std::make_unique <CTextWnd>();
	m_pConfirm->Create(_T("주문처리하시겠습니까?"), NULL, CRect(nX, nY, iright, nY + 18),
						this, IDC_CONFIRM, pfontTitle, clrBack, CLR_FG, clrBack);
	
	m_pConfirm->SetAlign(DT_LEFT);

/*
	CString strPathTitle;
	CString strPathBar;
	CString strTitle;
	CString strCountC;
	CString strPriceC;
	CString strGubn;
	COLORREF rgbFG = RGB(0, 0, 0);
	if (m_strOrderKind.Compare("매도") == 0)
	{
		strTitle = "매도주문";
		strPathTitle.Format("%s\\image\\매도확인_bg.bmp", GetHome());
		strPathBar.Format("%s\\image\\매도확인_bar.bmp", GetHome());
		m_rgbBG = GetIndexColor(162);
		strCountC = "매도수량";
		strPriceC = "매도단가";
		rgbFG = GetIndexColor(214);
	}
	else if (m_strOrderKind.Compare("매수") == 0)
	{
		strTitle = "매수주문";
		strPathTitle.Format("%s\\image\\매수확인_bg.bmp", GetHome());
		strPathBar.Format("%s\\image\\매수확인_bar.bmp", GetHome());
		m_rgbBG = GetIndexColor(161);
		strCountC = "매수수량";
		strPriceC = "매수단가";
		rgbFG = GetIndexColor(209);
	}
	else if (m_strOrderKind.Compare("정정") == 0)
	{
		strTitle = "정정주문";
		strPathTitle.Format("%s\\image\\정정확인_bg.bmp", GetHome());
		strPathBar.Format("%s\\image\\정정확인_bar.bmp", GetHome());
		m_rgbBG = GetIndexColor(163);
		strCountC = "정정수량";
		strPriceC = "정정단가";
		rgbFG = GetIndexColor(229);
	}
	else if (m_strOrderKind.Compare("취소") == 0)
	{
		strTitle = "취소주문";
		strPathTitle.Format("%s\\image\\취소확인_bg.bmp", GetHome());
		strPathBar.Format("%s\\image\\취소확인_bar.bmp", GetHome());
		m_rgbBG = GetIndexColor(164);
		strCountC = "취소수량";
		rgbFG = GetIndexColor(210);
		strGubn = "취소구분";
	}
	
	CFont* pfontTitle = RequestFont("굴림체", 11, FALSE, FW_BOLD);
	m_ptwTitle = new CTextWnd;
	m_ptwTitle->Create(strTitle, NULL, CRect(10, 10, 10 + 232, 10 + 35), this, ID_IMAGE_BG);
	m_ptwTitle->SetImagePath(strPathTitle, CRect(0, 0, 232, 35));
	m_ptwTitle->SetColorFG(GetIndexColor(192));
	m_ptwTitle->SetFontFromParent(pfontTitle);
	
	m_ptwBar = new CTextWnd;
	m_ptwBar->Create("", NULL, CRect(10, 45, 10 + 232, 45 + 8), this, ID_IMAGE_BAR);
	m_ptwBar->SetImagePath(strPathBar, CRect(0, 0, 232, 8));
	m_ptwBar->SetColorFG(GetIndexColor(192));
	m_ptwBar->SetFontFromParent(pfontTitle);
	
	
	
	CString astrText[8] = { "계좌번호", "계좌명", "종목명", "매매유형", "원주문번호", "정정구분", "주문수량", "주문단가" };
	if (!strCountC.IsEmpty())
		astrText[C_COUNT] = strCountC;
	
	if (!strPriceC.IsEmpty())
		astrText[C_PRICE] = strPriceC;
	
	if (!strGubn.IsEmpty())
		astrText[C_JJ_KIND] = strGubn;
	
	
	CFont* pfont = RequestFont("굴림체", 9, FALSE, NULL);
	CFont* pfontOrder = RequestFont("굴림", 9, FALSE, FW_BOLD);
	
	
#define WIDTH_C  100
#define WIDTH_D  132
	for (int ii = 0; ii < INDEX_COUNT / 2; ii++)
	{
		m_patw[ii] = (CTextWnd*) new CTextWnd;
		m_patw[ii]->Create(astrText[ii], NULL, CRect(10, 55 + ROW_HEIGHT * ii, 10 + WIDTH_C, 55 + ROW_HEIGHT * (ii + 1) + 1), this,
			C_ACCOUNT + ii, NULL, m_rgbBG, COLOR_STATIC_FG, m_rgbBorder);
		m_patw[ii]->SetAlign(DT_LEFT);
		m_patw[ii]->SetFontFromParent(pfont);
		
		m_patw[ii + INDEX_COUNT / 2] = (CTextWnd*) new CTextWnd;
		m_patw[ii + INDEX_COUNT / 2]->Create("", NULL, CRect(10 + WIDTH_C - 1, 55 + ROW_HEIGHT * ii, 10 + WIDTH_C + WIDTH_D, 55 + ROW_HEIGHT * (ii + 1) + 1), this,
			D_ACCOUNT + ii, NULL, COLOR_DATA_BG, COLOR_DATA_FG, m_rgbBorder);
		
		if (ii + INDEX_COUNT / 2 == D_ONE || ii + INDEX_COUNT / 2 == D_COUNT || ii + INDEX_COUNT / 2 == D_PRICE)
		{
			m_patw[ii + INDEX_COUNT / 2]->SetAlign(DT_RIGHT);
			
		}
		else
		{
			m_patw[ii + INDEX_COUNT / 2]->SetAlign(DT_LEFT);
		}
		m_patw[ii + INDEX_COUNT / 2]->SetFontFromParent(pfont);
	}
	
	m_patw[C_PRICE]->SetFontFromParent(pfontOrder);
	m_patw[D_PRICE]->SetFontFromParent(pfontOrder);
	m_patw[C_COUNT]->SetFontFromParent(pfontOrder);
	m_patw[D_COUNT]->SetFontFromParent(pfontOrder);
	
	m_ptwStatic1 = new CTextWnd;
	m_ptwStatic1->Create("위의", NULL, CRect(32, 228, 32 + 35, 228 + 16), this, ID_TEXT_1);
	m_ptwStatic1->SetColor(m_rgbBG, RGB(0, 0, 0), m_rgbBG);
	m_ptwStatic1->SetFontFromParent(pfont);
	
	m_ptwStatic2 = new CTextWnd;
	m_ptwStatic2->Create(strTitle, NULL, CRect(67, 228, 67 + 60, 228 + 16), this, ID_TEXT_2);
	m_ptwStatic2->SetColor(m_rgbBG, rgbFG, m_rgbBG);
	m_ptwStatic2->SetFontFromParent(pfontOrder);
	
	m_ptwStatic3 = new CTextWnd;
	m_ptwStatic3->Create("을 하시겠습니까?", NULL, CRect(127, 228, 127 + 105, 228 + 16), this, ID_TEXT_3);
	m_ptwStatic3->SetColor(m_rgbBG, RGB(0, 0, 0), m_rgbBG);
	m_ptwStatic3->SetFontFromParent(pfont);
*/
}

void CDialogConfirm::SetData(char cOrderKind, CString strAccount, CString strName, CString strJM, int iOrderCount, CString strOrderPrice, char cTradeKind, CString strOne, CString jmgbS, BOOL bMarket)
{
	CString jmgbEx;
	if (!jmgbS.IsEmpty())
	{
		if (jmgbS == "0")	// 없음
			jmgbEx = "";
		else if (jmgbS == "1")	// IOC
			jmgbEx = "IOC";
		else if (jmgbS == "2")	// FOK
			jmgbEx = "FOK";
	}

	switch (cOrderKind)
	{
	case 'S':
		m_strOrderKind = "매도";
		m_strWindowText = "매도주문확인";
		m_rgbBG = COLOR_STATIC_SELL_BG;
		m_rgbBorder = COLOR_SELL_BORDER;
		m_nOrderKind = 0;

		if (atof(strOrderPrice) > 0)
			m_strOrderPrice.Format("%s", strOrderPrice);
		break;

	case 'B':
		m_strOrderKind = "매수";
		m_strWindowText = "매수주문확인";
		m_rgbBG = COLOR_STATIC_BUY_BG;
		m_rgbBorder = COLOR_BUY_BORDER;
		m_nOrderKind = 1;

		if (atof(strOrderPrice) > 0)
			m_strOrderPrice.Format("%s", strOrderPrice);
		break;

	case 'C':
		m_strOrderKind = "취소";
		m_strWindowText = "취소주문확인";
		m_rgbBG = COLOR_STATIC_CANCEL_BG;
		m_rgbBorder = COLOR_CANCEL_BORDER;
		m_nOrderKind = 2
			;
		if (atof(strOrderPrice) > 0)
			m_strOrderPrice.Format("%s", strOrderPrice);
		m_strOne = strOne;
		break;

	case 'F':
		m_strOrderKind = "정정";
		m_strWindowText = "정정주문확인";
		m_rgbBG = COLOR_STATIC_FIX_BG;
		m_rgbBorder = COLOR_FIX_BORDER;
		m_nOrderKind = 3;

		m_strOrderPrice.Format("%s", strOrderPrice);
		m_strOne = strOne;
		break;
	
	case 'M':
		m_strOrderKind = "매도";
		m_strWindowText = "자동매도주문(이익실현)";
		m_rgbBG = COLOR_STATIC_SELL_BG;
		m_rgbBorder = COLOR_SELL_BORDER;
		m_nOrderKind = 4;

		if (atof(strOrderPrice) > 0)
			m_strOrderPrice.Format("%s", strOrderPrice);
		break;

	case 'L':
		m_strOrderKind = "매도";
		m_strWindowText = "자동매도주문(손실제한)";
		m_rgbBG = COLOR_STATIC_SELL_BG;
		m_rgbBorder = COLOR_SELL_BORDER;
		m_nOrderKind = 5;

		if (atof(strOrderPrice) > 0)
			m_strOrderPrice.Format("%s", strOrderPrice);
		break;
	}

	if (strAccount.Find('-') < 0)
		m_strAccount	= strAccount.Left(3) + "-" + strAccount.Mid(3, 2) + "-" + strAccount.Right(6);
	else
		m_strAccount	= strAccount;
	
	m_strName	= strName;
	m_strJM		= strJM;
	m_strJM.Replace("&", "&&");
	CString strTemp;
	strTemp.Format("%d", iOrderCount);
	strTemp = AddComma(strTemp);
	m_strOrderCount.Format("%s  계약", strTemp);

	switch (cTradeKind)
	{

	case 'M':
		m_strTradeKind = "시장가";
		break;

	case 'F':	
	default:
		if (jmgbEx.IsEmpty())
			m_strTradeKind = "지정가";
		else
		{
			m_strTradeKind = "지정가(" + jmgbEx + ")";
		}

		break;
	}

	//if(((CMapWnd*)m_pParentWnd)->GetMarketEnable())
	if(bMarket)
	{
		m_strTradeKind = "시장가";

		m_strOrderPrice = "";
	}
}

CString CDialogConfirm::AddComma(CString strData)
{
	CString strBuffer = strData;
	strBuffer.TrimLeft();
	strBuffer.TrimRight();

	if (strBuffer.IsEmpty())
		return "";

	CString strSign;
	if (strBuffer[0] == '-')
	{
		strSign = "-";
		strBuffer = strBuffer.Mid(1);
	}

	const int iLength = strBuffer.GetLength();
	const int iPosPoint = strBuffer.Find('.');

	int iNaturalLength{};

	if (iPosPoint < 0)
		iNaturalLength = iLength;
	else
		iNaturalLength = iLength - iPosPoint - 1;

	if (iNaturalLength < 4)
		return strData.GetBuffer(24);

	int iCommaCount = (iNaturalLength - 1) / 3;

	int iDest = 0;
	char acData[24]{};
	for (int ii = 0; ii < iNaturalLength; ii++)
	{
		if ((iNaturalLength - ii) % 3 == 0 && ii != 0) 
			acData[iDest++] = ',';
		acData[iDest++] = strBuffer[ii];
	}

	acData[iNaturalLength + iCommaCount] = 0;
	return strSign + CString(acData);
}

void CDialogConfirm::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
	CRect rectC;
	GetClientRect(rectC);
	dc.FillSolidRect(rectC, CLR_BG);
	rectC.DeflateRect(2,2,2,2); 
	

	if(m_parent)
	{
		rectC.DeflateRect(2,2,2,2); 
		dc.FillSolidRect(rectC, CLR_BODYBG);
		((CMapWnd*)m_parent)->DrawCornerRoundBitmap(&dc, rectC);
	}
	
	CRect rcBack;
	const int nBackHeight = GetBackHeight();
	COLORREF clrBack{};

	switch (m_nOrderKind)
	{
	case -1:		// Not Setting OrderKind
		return;
	case 1:			// 매수
		clrBack = CLR_BUY_BG;
		break;
	case 2:			// 취소
		clrBack = CLR_CANCEL_BG;
		break;
	case 3:			// 정정
		clrBack = CLR_MODIFY_BG;
		break;
	default:	// 매도
		clrBack = CLR_SELL_BG;
		break;
	}

	rcBack.SetRect(5, 8, rectC.Width()+3, 22 + nBackHeight);
//	dc.Rectangle(rcBack); 
	dc.FillSolidRect(rcBack, clrBack);

/*	if (m_bInit)
	{
		m_pBtnOK->SetFocus();
		m_bInit = false;
	}
*/	// Do not call CDialog::OnPaint() for painting messages
}

char* CDialogConfirm::GetHome()
{
	return (char*)m_pWizard->SendMessage(WM_USER, MAKEWPARAM(variantDLL, homeCC), NULL);
}

CFont* CDialogConfirm::RequestFont(CString strName, int iPoint, BOOL bItalic, int iBold)
{
	struct	_fontR fontR;

	fontR.name	= (char*)(strName.operator LPCTSTR());
	fontR.point	= iPoint;
	fontR.italic	= (bItalic == TRUE ? true : false);
	fontR.bold	= iBold;

	return (CFont*)m_pWizard->SendMessage(WM_USER, (WPARAM)MAKEWORD(getFONT, NULL), (LPARAM)&fontR);
}

CBitmap* CDialogConfirm::RequestBitmap(CString stPath)
{
	return (CBitmap*)m_pWizard->SendMessage(WM_USER, getBITMAP, (LPARAM)stPath.operator LPCTSTR());
}

COLORREF CDialogConfirm::GetIndexColor(int iIndex)
{
	return m_pWizard->SendMessage(WM_USER, MAKEWORD(getPALETTE, 0), (LPARAM)iIndex);
}

BOOL CDialogConfirm::PreTranslateMessage(MSG* pMsg) 
{
	if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_RETURN)
	{
		CWnd *pFocusWnd = GetFocus();
		if (pFocusWnd->m_hWnd == m_pBtnOK->m_hWnd)
		{
			EndDialog(IDOK);
			return TRUE;
		}
		else
		{
			EndDialog(IDCANCEL);
			return TRUE;
		}
	}
	else if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_TAB)
	{
		CWnd *pFocusWnd = GetFocus();
		if (pFocusWnd->m_hWnd == m_pBtnOK->m_hWnd)
			m_pBtnCancel->SetFocus();
		else
			m_pBtnOK->SetFocus();

		return TRUE;
	}
	
	return CDialog::PreTranslateMessage(pMsg);
}

int CDialogConfirm::GetFieldCount()
{
	switch (m_nOrderKind)
	{
	case -1:	return -1;	// Err
	case 2:		return 6;	// 취소
	case 3:		return 8;	// 정정
	default:	return 6;	// 매도 or 매수		
	}
}

CBitmap* CDialogConfirm::GetAxBitmap(CString path)
{
	return (CBitmap*)m_pWizard->SendMessage(WM_USER, getBITMAP, (long)path.operator LPCTSTR());
}

int CDialogConfirm::GetBackHeight()
{
	switch (m_nOrderKind)
	{
	case -1: return -1;
	case 1:	return 158;
	case 2:	return 158;
	case 3:	return 196;
	default: return 158;
	}
}
