// DlgSearchDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "DlgSearchRng.h"
#include "ConditionResultWnd.h"

#define BACK_GROUND_COLOR		RGB(167, 178, 198)

extern HINSTANCE g_hInstance;

// CDlgSearchDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(CDlgSearchRng, CDialog)

CDlgSearchRng::CDlgSearchRng(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgSearchRng::IDD, pParent)
{
	m_pParentWnd = pParent;
}

CDlgSearchRng::~CDlgSearchRng()
{
	m_pFontBold->DeleteObject();
	m_pFont->DeleteObject();
	m_brush.DeleteObject();
}

void CDlgSearchRng::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDlgSearchRng, CDialog)
	ON_WM_PAINT()

	ON_WM_CREATE()
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
	ON_WM_DESTROY()

	ON_BN_CLICKED(IDC_EQUITY_CAPITAL, OnCheckEquityCapital)
	ON_BN_CLICKED(IDC_PRICE_RANGE, OnCheckPriceRange)
	ON_BN_CLICKED(IDC_AGGREGATE_VALUE_OF_LISTED_STOCKS, OnCheckAggregateValueOfListedStocks)
	ON_BN_CLICKED(IDC_TRADING_VOLUME, OnCheckTradingVolume)
	ON_BN_CLICKED(IDC_PAR_VALUE, OnCheckParValue)

	ON_EN_CHANGE(IDC_EQUITY_CAPITAL_ABOVE, OnChangeEquityCapitalAbove)
	ON_EN_CHANGE(IDC_EQUITY_CAPITAL_BELOW, OnChangeEquityCapitalBelow)
	ON_EN_CHANGE(IDC_PRICE_RANGE_ABOVE, OnChangePriceRangeAbove)
	ON_EN_CHANGE(IDC_PRICE_RANGE_BELOW, OnChangePriceRangeBelow)
	ON_EN_CHANGE(IDC_AGGREGATE_VALUE_OF_LISTED_STOCKS_ABOVE, OnChangeOnCheckAggregateValueOfListedStocksAbove)
	ON_EN_CHANGE(IDC_AGGREGATE_VALUE_OF_LISTED_STOCKS_BELOW, OnChangeOnCheckAggregateValueOfListedStocksBelow)
	ON_EN_CHANGE(IDC_TRADING_VOLUME_ABOVE, OnChangeTradingVolumeAbove)
	ON_EN_CHANGE(IDC_TRADING_VOLUME_BELOW, OnChangeTradingVolumeBelow)
	ON_EN_CHANGE(IDC_PAR_VALUE_ABOVE, OnChangeParValueAbove)
	ON_EN_CHANGE(IDC_PAR_VALUE_BELOW, OnChangeParValueBelow)

END_MESSAGE_MAP()


// CDlgSearchDlg 메시지 처리기입니다.
void CDlgSearchRng::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	// 그리기 메시지에 대해서는 CDialog::OnPaint()을(를) 호출하지 마십시오.
	CRect reClient;
	GetClientRect(&reClient);

	CConditionResultWnd* pWnd = (CConditionResultWnd*)m_pParentWnd;
	dc.FillSolidRect(reClient, pWnd->GetAxColor(66));

	CPen pen;
	pen.CreatePen(PS_SOLID, 1, RGB(0, 0, 255));
	
	CRect reTemp;
	reTemp.top = reClient.bottom - 46;
	reTemp.left = reClient.left + 3;
	reTemp.right = 96;
	reTemp.bottom = reTemp.top + 20;

	CFont* pOldFont = dc.SelectObject(m_pFont);
	CPen* pOldpen = dc.SelectObject(&pen);
	dc.SetBkColor(pWnd->GetAxColor(66));
	dc.SetTextColor(RGB(0, 0, 0));
	dc.DrawText(_T("검색범위 설정은 "), -1, &reTemp, DT_SINGLELINE | DT_VCENTER);
	
	reTemp.left = reTemp.right;
	reTemp.right = reTemp.left + 85;
	dc.SelectObject(m_pFontBold);
	dc.DrawText(_T("검색 기준일자"), -1, &reTemp, DT_SINGLELINE | DT_VCENTER);

	reTemp.left = reTemp.right;
	reTemp.right = reTemp.left + 48;
	dc.SelectObject(m_pFont);
	dc.DrawText(_T("가 아닌 "), -1, &reTemp, DT_SINGLELINE | DT_VCENTER);

	reTemp.left = reTemp.right;
	reTemp.right = reTemp.left + 52;
	dc.SelectObject(m_pFontBold);
	dc.DrawText(_T("당일시세"), -1, &reTemp, DT_SINGLELINE | DT_VCENTER);

	reTemp.left = reTemp.right;
	reTemp.right = reTemp.left + 150;
	dc.SelectObject(m_pFont);
	dc.DrawText(_T("에 대해 설정이 됩니다."), -1, &reTemp, DT_SINGLELINE | DT_VCENTER);

	dc.SelectObject(pOldFont);
	dc.SelectObject(pOldpen);
}

void CDlgSearchRng::SetInit()
{
	CRect reClient;
	CRect reTopRect;
	CRect reBottomRect;
	CRect reTemp;

	m_pFontBold = &(((CConditionResultWnd*)m_pParentWnd)->m_fontBold);
	m_pFont = &(((CConditionResultWnd*)m_pParentWnd)->m_fontDefault);

	GetClientRect(&reClient);

	reTopRect.left		= reClient.left		+ 3;
	reTopRect.top		= reClient.top		+ 3;
	reTopRect.right		= reClient.right	- 3;
	reTopRect.bottom	= (reClient.bottom	- 20) / 3;//m_reBackGround.bottom	/ 3;
	//m_ctrlTopRect.MoveWindow(reTopRect);

	reBottomRect.left	= reTopRect.left;
	reBottomRect.top	= reTopRect.bottom + 5;
	reBottomRect.right	= reTopRect.right;
	reBottomRect.bottom	= reClient.bottom	- 42;
	//m_ctrlBottomRect.MoveWindow(reBottomRect);
	
	reTemp.left		= reTopRect.left	+ 1;
	reTemp.top		= reTopRect.top		+ 1;
	reTemp.right	= reTopRect.right	- 1;
	reTemp.bottom	= reTopRect.top		+ 85;
	m_btnTopGroup.MoveWindow(reTemp);
	m_btnTopGroup.SetFont(m_pFontBold);
	
	reTemp.left		= reBottomRect.left		+ 1;
	reTemp.top		= reBottomRect.top		+ 1;
	reTemp.right	= reBottomRect.right	- 1;
	reTemp.bottom	= reBottomRect.top		+ 155;
	m_btnBottonGroup.MoveWindow(reTemp);
	m_btnBottonGroup.SetFont(m_pFontBold);

	// 대상 제외 >>
	reTopRect.top += 15;
	m_ctrlIssuesForAdministration.MoveWindow(reTopRect.left + 10,
											 reTopRect.top + 2,
											 reTopRect.Width()  / 4 - 20,
											 reTopRect.Height() / 2 - 20);
	m_ctrlIssuesForAdministration.SetFont(m_pFont);
	
	m_ctrlInvestAttention.MoveWindow(reTopRect.Width()  / 4 + 5,
									 reTopRect.top + 2,
									 reTopRect.Width()  / 4 - 20,
									 reTopRect.Height() / 2 - 20);
	m_ctrlInvestAttention.SetFont(m_pFont);
		
	m_ctrlInvestWarning.MoveWindow(reTopRect.Width()  / 2 - 5,
								   reTopRect.top + 2,
								   reTopRect.Width()  / 4 - 20,
								   reTopRect.Height() / 2 - 20);
	m_ctrlInvestWarning.SetFont(m_pFont);

	m_ctrlPreInvestRisk.MoveWindow(reTopRect.Width()  / 4 * 3 - 15,
								   reTopRect.top + 2,
								   reTopRect.Width()  / 4 - 10,
								   reTopRect.Height() / 2 - 20);
	m_ctrlPreInvestRisk.SetFont(m_pFont);
	
	// ------------------------------------------------------------------
	
	m_ctrlInvestRisk.MoveWindow(reTopRect.left + 10,
						    	reTopRect.top + reTopRect.Height() / 3 - 3,
						        reTopRect.Width()  / 4 - 20,
							    reTopRect.Height() / 2 - 20);
	m_ctrlInvestRisk.SetFont(m_pFont);
	
	m_ctrlDealStop.MoveWindow(reTopRect.Width()  / 4 + 5,
							  reTopRect.top + reTopRect.Height() / 3 - 3,
							  reTopRect.Width()  / 4 - 20,
							  reTopRect.Height() / 2 - 20);
	m_ctrlDealStop.SetFont(m_pFont);
	
	m_ctrlTidyDeal.MoveWindow(reTopRect.Width()  / 2 - 5,
							  reTopRect.top + reTopRect.Height() / 3 - 3,
							  reTopRect.Width()  / 4 - 20,
							  reTopRect.Height() / 2 - 20);
	m_ctrlTidyDeal.SetFont(m_pFont);

	m_ctrlAttentionInvestment.MoveWindow(reTopRect.Width()  / 4 * 3 - 15,
										 reTopRect.top + reTopRect.Height() / 3 - 3,
										 reTopRect.Width()  / 4 - 10,
										 reTopRect.Height() / 2 - 20);
	m_ctrlAttentionInvestment.SetFont(m_pFont);

	// ------------------------------------------------------------------

	m_ctrlInsincerityNotice.MoveWindow(reTopRect.left + 10,
									   reTopRect.top + reTopRect.Height() / 4 * 3 - 15,
									   reTopRect.Width()  / 4 - 20,
									   reTopRect.Height() / 2 - 20);
	m_ctrlInsincerityNotice.SetFont(m_pFont);

	m_ctrlPreferenceStock.MoveWindow(reTopRect.Width()  / 4 + 5,
									 reTopRect.top + reTopRect.Height() / 4 * 3 - 15,
 									 reTopRect.Width()  / 4 - 20,
									 reTopRect.Height() / 2 - 20);
	m_ctrlPreferenceStock.SetFont(m_pFont);

	// 대상 제외 <<

	// 적용 범위 >>	
	reBottomRect.top += 12;

	// 자본금 >>
	reTemp			= reBottomRect;
	reTemp.left		+= 10;
	reTemp.right	= reTemp.left + 70;
	reTemp.bottom	= reBottomRect.top + reBottomRect.Height() / 5 - 2;
	m_ctrlEquityCapital.MoveWindow(&reTemp);
	m_ctrlEquityCapital.SetFont(m_pFont);

	reTemp.left  = reTemp.right + 10;
	reTemp.right = reTemp.left + 80;
	m_ctrlEquityCapitalAbove.MoveWindow(reTemp.left, reTemp.top + 5, reTemp.Width(), reTemp.Height() - 9);
	m_ctrlEquityCapitalAbove.SetFont(m_pFont);
	
	reTemp.left  = reTemp.right + 10;
	reTemp.right = reTemp.left + 50;
	m_ctrlEquityCapitalAboveText.MoveWindow(&reTemp);
	m_ctrlEquityCapitalAboveText.SetFont(m_pFont);

	reTemp.left  = reTemp.right + 10;
	reTemp.right = reTemp.left + 80;
	m_ctrlEquityCapitalBelow.MoveWindow(reTemp.left, reTemp.top + 5, reTemp.Width(), reTemp.Height() - 9);
	m_ctrlEquityCapitalBelow.SetFont(m_pFont);
	
	reTemp.left  = reTemp.right + 10;
	reTemp.right = reTemp.left + 50;
	m_ctrlEquityCapitalBelowText.MoveWindow(&reTemp);
	m_ctrlEquityCapitalBelowText.SetFont(m_pFont);
	// 자본금 <<
	
	// 가격대 >>
	reTemp.left		= reBottomRect.left + 10;
	reTemp.right	= reTemp.left + 70;
	reTemp.top		= reTemp.bottom;
	reTemp.bottom	+= reBottomRect.Height() / 5 - 2;
	m_ctrlPriceRange.MoveWindow(&reTemp);
	m_ctrlPriceRange.SetFont(m_pFont);

	reTemp.left  = reTemp.right + 10;
	reTemp.right = reTemp.left + 80;
	m_ctrlPriceRangeAbove.MoveWindow(reTemp.left, reTemp.top + 5, reTemp.Width(), reTemp.Height() - 9);
	m_ctrlPriceRangeAbove.SetFont(m_pFont);

	reTemp.left  = reTemp.right + 10;
	reTemp.right = reTemp.left + 50;
	m_ctrlPriceRangeAboveText.MoveWindow(&reTemp);
	m_ctrlPriceRangeAboveText.SetFont(m_pFont);

	reTemp.left  = reTemp.right + 10;
	reTemp.right = reTemp.left + 80;
	m_ctrlPriceRangeBelow.MoveWindow(reTemp.left, reTemp.top + 5, reTemp.Width(), reTemp.Height() - 9);
	m_ctrlPriceRangeBelow.SetFont(m_pFont);

	reTemp.left  = reTemp.right + 10;
	reTemp.right = reTemp.left + 50;
	m_ctrlPriceRangeBelowText.MoveWindow(&reTemp);
	m_ctrlPriceRangeBelowText.SetFont(m_pFont);
	// 가격대 <<

	// 시가총액 >>
	reTemp.left		= reBottomRect.left + 10;
	reTemp.right	= reTemp.left + 70;
	reTemp.top		= reTemp.bottom;
	reTemp.bottom	+= reBottomRect.Height() / 5 - 2;
	m_ctrlAggregateValueOfListedStocks.MoveWindow(&reTemp);
	m_ctrlAggregateValueOfListedStocks.SetFont(m_pFont);

	reTemp.left  = reTemp.right + 10;
	reTemp.right = reTemp.left + 80;
	m_ctrlAggregateValueOfListedStocksAbove.MoveWindow(reTemp.left, reTemp.top + 5, reTemp.Width(), reTemp.Height() - 9);
	m_ctrlAggregateValueOfListedStocksAbove.SetFont(m_pFont);

	reTemp.left  = reTemp.right + 10;
	reTemp.right = reTemp.left + 50;
	m_ctrlAggregateValueOfListedStocksAboveText.MoveWindow(&reTemp);
	m_ctrlAggregateValueOfListedStocksAboveText.SetFont(m_pFont);

	reTemp.left  = reTemp.right + 10;
	reTemp.right = reTemp.left + 80;
	m_ctrlAggregateValueOfListedStocksBelow.MoveWindow(reTemp.left, reTemp.top + 5, reTemp.Width(), reTemp.Height() - 9);
	m_ctrlAggregateValueOfListedStocksBelow.SetFont(m_pFont);

	reTemp.left  = reTemp.right + 10;
	reTemp.right = reTemp.left + 50;
	m_ctrlAggregateValueOfListedStocksBelowText.MoveWindow(&reTemp);
	m_ctrlAggregateValueOfListedStocksBelowText.SetFont(m_pFont);
	// 시가총액 <<

	// 거래량 >>
	reTemp.left		= reBottomRect.left + 10;
	reTemp.right	= reTemp.left + 70;
	reTemp.top		= reTemp.bottom;
	reTemp.bottom	+= reBottomRect.Height() / 5 - 2;
	m_ctrlTradingVolume.MoveWindow(&reTemp);
	m_ctrlTradingVolume.SetFont(m_pFont);

	reTemp.left  = reTemp.right + 10;
	reTemp.right = reTemp.left + 80;
	m_ctrlTradingVolumeAbove.MoveWindow(reTemp.left, reTemp.top + 5, reTemp.Width(), reTemp.Height() - 9);
	m_ctrlTradingVolumeAbove.SetFont(m_pFont);

	reTemp.left  = reTemp.right + 10;
	reTemp.right = reTemp.left + 50;
	m_ctrlTradingVolumeAboveText.MoveWindow(&reTemp);
	m_ctrlTradingVolumeAboveText.SetFont(m_pFont);

	reTemp.left  = reTemp.right + 10;
	reTemp.right = reTemp.left + 80;
	m_ctrlTradingVolumeBelow.MoveWindow(reTemp.left, reTemp.top + 5, reTemp.Width(), reTemp.Height() - 9);
	m_ctrlTradingVolumeBelow.SetFont(m_pFont);

	reTemp.left  = reTemp.right + 10;
	reTemp.right = reTemp.left + 50;
	m_ctrlTradingVolumeBelowText.MoveWindow(&reTemp);
	m_ctrlTradingVolumeBelowText.SetFont(m_pFont);
	// 거래량 <<

	// 액면가 >>
	reTemp.left		= reBottomRect.left + 10;
	reTemp.right	= reTemp.left + 70;
	reTemp.top		= reTemp.bottom;
	reTemp.bottom	+= reBottomRect.Height() / 5 - 2;
	m_ctrlParValue.MoveWindow(&reTemp);
	m_ctrlParValue.SetFont(m_pFont);

	reTemp.left  = reTemp.right + 10;
	reTemp.right = reTemp.left + 80;
	m_ctrlParValueAbove.MoveWindow(reTemp.left, reTemp.top + 5, reTemp.Width(), reTemp.Height() - 9);
	m_ctrlParValueAbove.SetFont(m_pFont);

	reTemp.left  = reTemp.right + 10;
	reTemp.right = reTemp.left + 50;
	m_ctrlParValueAboveText.MoveWindow(&reTemp);
	m_ctrlParValueAboveText.SetFont(m_pFont);

	reTemp.left  = reTemp.right + 10;
	reTemp.right = reTemp.left + 80;
	m_ctrlParValueBelow.MoveWindow(reTemp.left, reTemp.top + 5, reTemp.Width(), reTemp.Height() - 9);
	m_ctrlParValueBelow.SetFont(m_pFont);

	reTemp.left  = reTemp.right + 10;
	reTemp.right = reTemp.left + 50;
	m_ctrlParValueBelowText.MoveWindow(&reTemp);
	m_ctrlParValueBelowText.SetFont(m_pFont);
	// 액면가 <<
	// 적용 범위 <<

	// 버튼
	reTemp = reClient;
	reTemp.top = reTemp.bottom - 25;
	reTemp.bottom = reTemp.top + 20;

	reTemp.right -= 3;
	reTemp.left = reTemp.right - 60;
	m_btnCancel.MoveWindow(&reTemp);

	reTemp.right = reTemp.left - 2;
	reTemp.left = reTemp.right - 60;
	m_btnOk.MoveWindow(&reTemp);

	SetInit_Data();
}

int CDlgSearchRng::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;

	HINSTANCE hInstSave = AfxGetResourceHandle();
	AfxSetResourceHandle(g_hInstance);

	CConditionResultWnd* pWnd = (CConditionResultWnd*)m_pParentWnd;

	m_btnOk.Create(_T("확인"), CRect(0, 0, 0, 0), this, IDOK);
	pWnd->SetBtnImg(&m_btnOk, 4);
	
	m_btnCancel.Create(_T("취소"), CRect(0, 0, 0, 0), this, IDCANCEL);
	pWnd->SetBtnImg(&m_btnCancel, 4);
	
	m_btnTopGroup.Create(_T("대상제외"), WS_CHILD | WS_VISIBLE | BS_GROUPBOX, 
		CRect(0, 0, 0, 0), this, IDC_TOP_BTN);
	m_btnBottonGroup.Create(_T("적용범위"), WS_CHILD | WS_VISIBLE | BS_GROUPBOX, 
		CRect(0, 0, 0, 0), this, IDC_BOTTOM_BTN);

	// 대상 제외 >>
	m_ctrlIssuesForAdministration.Create(_T("관리종목"), WS_CHILD | WS_VISIBLE | WS_TABSTOP | BS_AUTOCHECKBOX,
		CRect(0, 0, 0, 0), this, IDC_ISSUES_FOR_ADMINISTRATION);
	m_ctrlInvestAttention.Create(_T("투자주의"), WS_CHILD | WS_VISIBLE | WS_TABSTOP | BS_AUTOCHECKBOX,
		CRect(0, 0, 0, 0), this, IDC_INVEST_ATTENTION);
	m_ctrlInvestWarning.Create(_T("투자경고"), WS_CHILD | WS_VISIBLE | WS_TABSTOP | BS_AUTOCHECKBOX,
		CRect(0, 0, 0, 0), this, IDC_INVEST_ATTENTION);
	m_ctrlPreInvestRisk.Create(_T("투자위험예고"), WS_CHILD | WS_VISIBLE | WS_TABSTOP | BS_AUTOCHECKBOX,
		CRect(0, 0, 0, 0), this, IDC_PRE_INVEST_RISK);
	m_ctrlInvestRisk.Create(_T("투자위험"), WS_CHILD | WS_VISIBLE | WS_TABSTOP | BS_AUTOCHECKBOX,
		CRect(0, 0, 0, 0), this, IDC_INVEST_RISK);
	m_ctrlDealStop.Create(_T("거래정지"), WS_CHILD | WS_VISIBLE | WS_TABSTOP | BS_AUTOCHECKBOX,
		CRect(0, 0, 0, 0), this, IDC_DEAL_STOP);
	m_ctrlAttentionInvestment.Create(_T("투자주의환기"), WS_CHILD | WS_VISIBLE | WS_TABSTOP | BS_AUTOCHECKBOX,
		CRect(0, 0, 0, 0), this, IDC_INVESTMENT_ATTENTION);
	m_ctrlPreferenceStock.Create(_T("우선주"), WS_CHILD | WS_VISIBLE | WS_TABSTOP | BS_AUTOCHECKBOX,
		CRect(0, 0, 0, 0), this, IDC_PREFERENCE_STOCK);
// 	m_ctrlOver.Create(_T("이상급등"), WS_CHILD | WS_VISIBLE | WS_TABSTOP | BS_AUTOCHECKBOX,
// 		CRect(0, 0, 0, 0), this, IDC_OVER);
	m_ctrlTidyDeal.Create(_T("정리매매"), WS_CHILD | WS_VISIBLE | WS_TABSTOP | BS_AUTOCHECKBOX,
		CRect(0, 0, 0, 0), this, IDC_TIDY_DEAL);
	m_ctrlInsincerityNotice.Create(_T("불성실공시"), WS_CHILD | WS_VISIBLE | WS_TABSTOP | BS_AUTOCHECKBOX,
		CRect(0, 0, 0, 0), this, IDC_INSINCERITY_NOTICE);
	// 대상 제외 <<
	
	// 적용 범위 >>
	m_ctrlEquityCapital.Create(_T("자본금"), WS_CHILD | WS_VISIBLE | WS_TABSTOP | BS_AUTOCHECKBOX,
		CRect(0, 0, 0, 0), this, IDC_EQUITY_CAPITAL);
	m_ctrlEquityCapitalAbove.Create(WS_CHILD | WS_VISIBLE | WS_TABSTOP | WS_BORDER | SS_RIGHT,
		CRect(0, 0, 0, 0), this, IDC_EQUITY_CAPITAL_ABOVE);
	m_ctrlEquityCapitalBelow.Create(WS_CHILD | WS_VISIBLE | WS_TABSTOP | WS_BORDER | SS_RIGHT,
		CRect(0, 0, 0, 0), this, IDC_EQUITY_CAPITAL_BELOW);
	m_ctrlEquityCapitalAboveText.Create(_T("억원이상"), WS_CHILD | WS_VISIBLE | SS_CENTERIMAGE,
		CRect(0, 0, 0, 0), this, IDC_EQUITY_CAPITAL_ABOVE_TEXT);
	m_ctrlEquityCapitalBelowText.Create(_T("억원이하"), WS_CHILD | WS_VISIBLE | SS_CENTERIMAGE,
		CRect(0, 0, 0, 0), this, IDC_EQUITY_CAPITAL_BELOW_TEXT);

	m_ctrlPriceRange.Create(_T("가격대"), WS_CHILD | WS_VISIBLE | WS_TABSTOP | BS_AUTOCHECKBOX,
		CRect(0, 0, 0, 0), this, IDC_PRICE_RANGE);
	m_ctrlPriceRangeAbove.Create(WS_CHILD | WS_VISIBLE | WS_TABSTOP | WS_BORDER | SS_RIGHT,
		CRect(0, 0, 0, 0), this, IDC_PRICE_RANGE_ABOVE);
	m_ctrlPriceRangeBelow.Create(WS_CHILD | WS_VISIBLE | WS_TABSTOP | WS_BORDER | SS_RIGHT,
		CRect(0, 0, 0, 0), this, IDC_PRICE_RANGE_BELOW);
	m_ctrlPriceRangeAboveText.Create(_T("원이상"), WS_CHILD | WS_VISIBLE | SS_CENTERIMAGE,
		CRect(0, 0, 0, 0), this, IDC_PRICE_RANGE_ABOVE_TEXT);
	m_ctrlPriceRangeBelowText.Create(_T("원이하"), WS_CHILD | WS_VISIBLE | SS_CENTERIMAGE,
		CRect(0, 0, 0, 0), this, IDC_PRICE_RANGE_BELOW_TEXT);

	m_ctrlAggregateValueOfListedStocks.Create(_T("시가총액"), WS_CHILD | WS_VISIBLE | WS_TABSTOP | BS_AUTOCHECKBOX,
		CRect(0, 0, 0, 0), this, IDC_AGGREGATE_VALUE_OF_LISTED_STOCKS);
	m_ctrlAggregateValueOfListedStocksAbove.Create(WS_CHILD | WS_VISIBLE | WS_TABSTOP | WS_BORDER | SS_RIGHT,
		CRect(0, 0, 0, 0), this, IDC_AGGREGATE_VALUE_OF_LISTED_STOCKS_ABOVE);
	m_ctrlAggregateValueOfListedStocksBelow.Create(WS_CHILD | WS_VISIBLE | WS_TABSTOP | WS_BORDER | SS_RIGHT,
		CRect(0, 0, 0, 0), this, IDC_AGGREGATE_VALUE_OF_LISTED_STOCKS_BELOW);
	m_ctrlAggregateValueOfListedStocksAboveText.Create(_T("억원이상"), WS_CHILD | WS_VISIBLE | SS_CENTERIMAGE,
		CRect(0, 0, 0, 0), this, IDC_AGGREGATE_VALUE_OF_LISTED_STOCKS_ABOVE_TEXT);
	m_ctrlAggregateValueOfListedStocksBelowText.Create(_T("억원이하"), WS_CHILD | WS_VISIBLE | SS_CENTERIMAGE,
		CRect(0, 0, 0, 0), this, IDC_AGGREGATE_VALUE_OF_LISTED_STOCKS_BELOW_TEXT);

	m_ctrlTradingVolume.Create(_T("거래량"), WS_CHILD | WS_VISIBLE | WS_TABSTOP | BS_AUTOCHECKBOX,
		CRect(0, 0, 0, 0), this, IDC_TRADING_VOLUME);
	m_ctrlTradingVolumeAbove.Create(WS_CHILD | WS_VISIBLE | WS_TABSTOP | WS_BORDER | SS_RIGHT,
		CRect(0, 0, 0, 0), this, IDC_TRADING_VOLUME_ABOVE);
	m_ctrlTradingVolumeBelow.Create(WS_CHILD | WS_VISIBLE | WS_TABSTOP | WS_BORDER | SS_RIGHT,
		CRect(0, 0, 0, 0), this, IDC_TRADING_VOLUME_BELOW);
	m_ctrlTradingVolumeAboveText.Create(_T("만주이상"), WS_CHILD | WS_VISIBLE | SS_CENTERIMAGE,
		CRect(0, 0, 0, 0), this, IDC_TRADING_VOLUME_ABOVE_TEXT);
	m_ctrlTradingVolumeBelowText.Create(_T("만주이하"), WS_CHILD | WS_VISIBLE | SS_CENTERIMAGE,
		CRect(0, 0, 0, 0), this, IDC_TRADING_VOLUME_BELOW_TEXT);

	m_ctrlParValue.Create(_T("액면가"), WS_CHILD | WS_VISIBLE | WS_TABSTOP | BS_AUTOCHECKBOX,
		CRect(0, 0, 0, 0), this, IDC_PAR_VALUE);
	m_ctrlParValueAbove.Create(WS_CHILD | WS_VISIBLE | WS_TABSTOP | WS_BORDER | SS_RIGHT,
		CRect(0, 0, 0, 0), this, IDC_PAR_VALUE_ABOVE);
	m_ctrlParValueBelow.Create(WS_CHILD | WS_VISIBLE | WS_TABSTOP | WS_BORDER | SS_RIGHT,
		CRect(0, 0, 0, 0), this, IDC_PAR_VALUE_BELOW);
	m_ctrlParValueAboveText.Create(_T("원이상"), WS_CHILD | WS_VISIBLE | SS_CENTERIMAGE,
		CRect(0, 0, 0, 0), this, IDC_PAR_VALUE_ABOVE_TEXT);
	m_ctrlParValueBelowText.Create(_T("원이하"), WS_CHILD | WS_VISIBLE | SS_CENTERIMAGE,
		CRect(0, 0, 0, 0), this, IDC_PAR_VALUE_BELOW_TEXT);
	// 적용 범위
	
	AfxSetResourceHandle(hInstSave);

	SetInit();
	return 0;
}

HBRUSH CDlgSearchRng::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  여기서 DC의 특성을 변경합니다.
	
	CConditionResultWnd* pReWnd = (CConditionResultWnd*)m_pParentWnd;
	UINT id = pWnd->GetDlgCtrlID();
	m_brush.DeleteObject();
	// TODO:  기본값이 적당하지 않으면 다른 브러시를 반환합니다.
	switch(id){
		case IDC_BACK_GROUND :
			pDC->SetBkMode(TRANSPARENT);
			m_brush.CreateSolidBrush(pReWnd->GetAxColor(66));
			hbr = (HBRUSH)m_brush.m_hObject;
			break;
		
		case IDC_TOP_RECT :
		case IDC_BOTTOM_RECT :
			pDC->SetBkMode(TRANSPARENT);
			m_brush.CreateSolidBrush(pReWnd->GetAxColor(66));
			hbr = (HBRUSH)m_brush.m_hObject;
			break;

		case IDC_TOP_BTN :
		case IDC_BOTTOM_BTN :
			pDC->SetBkMode(TRANSPARENT);
			m_brush.CreateSolidBrush(pReWnd->GetAxColor(66));
			hbr = (HBRUSH)m_brush.m_hObject;
			break;

		case IDC_ISSUES_FOR_ADMINISTRATION :
		case IDC_DEAL_STOP :
		case IDC_INVESTMENT_ATTENTION :
		case IDC_PREFERENCE_STOCK :
//		case IDC_OVER :
		case IDC_TIDY_DEAL :
		case IDC_INSINCERITY_NOTICE :
		case IDC_EQUITY_CAPITAL :
		case IDC_EQUITY_CAPITAL_ABOVE_TEXT :
		case IDC_EQUITY_CAPITAL_BELOW_TEXT :
		case IDC_PRICE_RANGE :
		case IDC_PRICE_RANGE_ABOVE_TEXT :
		case IDC_PRICE_RANGE_BELOW_TEXT :
		case IDC_AGGREGATE_VALUE_OF_LISTED_STOCKS :
		case IDC_AGGREGATE_VALUE_OF_LISTED_STOCKS_ABOVE_TEXT :
		case IDC_AGGREGATE_VALUE_OF_LISTED_STOCKS_BELOW_TEXT :
		case IDC_TRADING_VOLUME :
		case IDC_TRADING_VOLUME_ABOVE_TEXT :
		case IDC_TRADING_VOLUME_BELOW_TEXT:
		case IDC_PAR_VALUE :
		case IDC_PAR_VALUE_ABOVE_TEXT :
		case IDC_PAR_VALUE_BELOW_TEXT :
		case IDC_INVEST_ATTENTION :
		case IDC_INVEST_WARNING :
		case IDC_INVEST_RISK :
		case IDC_PRE_INVEST_RISK :
			pDC->SetBkMode(TRANSPARENT);
			m_brush.CreateSolidBrush(pReWnd->GetAxColor(66));
			hbr = (HBRUSH)m_brush.m_hObject;
			break;
	}

	return hbr;
}

void CDlgSearchRng::OnBnClickedOk()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	
	if(!VerificationValues())
		return;

	SaveValue();
	CDialog::OnOK();
}

void CDlgSearchRng::OnDestroy()
{
	CDialog::OnDestroy();

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
// 	m_pFont->DeleteObject();
// 	m_pFont = NULL;
// 	m_pFontBold->DeleteObject();
// 	m_pFontBold = NULL;
}

void CDlgSearchRng::SetInit_Data()
{
	CConditionResultWnd* pWnd = (CConditionResultWnd*)m_pParentWnd;
	
	m_ctrlIssuesForAdministration.SetCheck(pWnd->m_bIssuesForAdministration);
	m_ctrlInvestAttention.SetCheck(pWnd->m_bAttentionInvestment);
	m_ctrlInvestWarning.SetCheck(pWnd->m_bWarningInvestment);
	m_ctrlPreInvestRisk.SetCheck(pWnd->m_bNoticeAttentionInvestment);
	
	m_ctrlInvestRisk.SetCheck(pWnd->m_bDangerInvestment);
	m_ctrlDealStop.SetCheck(pWnd->m_bDealStop);
	m_ctrlTidyDeal.SetCheck(pWnd->m_bTidyDeal);
	m_ctrlAttentionInvestment.SetCheck(pWnd->m_bInvestAttention);

	m_ctrlInsincerityNotice.SetCheck(pWnd->m_bInsincerityNotice);
	m_ctrlPreferenceStock.SetCheck(pWnd->m_bPreferenceStock);
	
	m_ctrlOver.SetCheck(FALSE);
	
	m_ctrlEquityCapital.SetCheck(pWnd->m_bEquityCapital);
	m_ctrlPriceRange.SetCheck(pWnd->m_bPriceRange);
	m_ctrlAggregateValueOfListedStocks.SetCheck(pWnd->m_bAggregateValueOfListedStocks);
	m_ctrlTradingVolume.SetCheck(pWnd->m_bTradingVolume);
	m_ctrlParValue.SetCheck(pWnd->m_bParValue);

	CString szDate;
	szDate.Format("%d", pWnd->m_nEquityCapitalAbove);
	m_ctrlEquityCapitalAbove.SetWindowText(szDate);
	szDate.Format("%d", pWnd->m_nEquityCapitalBelow);
	m_ctrlEquityCapitalBelow.SetWindowText(szDate);

	szDate.Format("%d", pWnd->m_nPriceRangeAbove);
	m_ctrlPriceRangeAbove.SetWindowText(szDate);
	szDate.Format("%d", pWnd->m_nPriceRangeBelow);
	m_ctrlPriceRangeBelow.SetWindowText(szDate);

	szDate.Format("%d", pWnd->m_nAggregateValueOfListedStocksAbove);
	m_ctrlAggregateValueOfListedStocksAbove.SetWindowText(szDate);
	szDate.Format("%d", pWnd->m_nAggregateValueOfListedStocksBelow);
	m_ctrlAggregateValueOfListedStocksBelow.SetWindowText(szDate);

	szDate.Format("%d", pWnd->m_nTradingVolumeAbove);
	m_ctrlTradingVolumeAbove.SetWindowText(szDate);
	szDate.Format("%d", pWnd->m_nTradingVolumeBelow);
	m_ctrlTradingVolumeBelow.SetWindowText(szDate);

	szDate.Format("%d", pWnd->m_nParValueAbove);
	m_ctrlParValueAbove.SetWindowText(szDate);
	szDate.Format("%d", pWnd->m_nParValueBelow);
	m_ctrlParValueBelow.SetWindowText(szDate);

	m_ctrlEquityCapitalAbove.EnableWindow(m_ctrlEquityCapital.GetCheck());
	m_ctrlEquityCapitalBelow.EnableWindow(m_ctrlEquityCapital.GetCheck());
	m_ctrlPriceRangeAbove.EnableWindow(m_ctrlPriceRange.GetCheck());
	m_ctrlPriceRangeBelow.EnableWindow(m_ctrlPriceRange.GetCheck());
	m_ctrlAggregateValueOfListedStocksAbove.EnableWindow(m_ctrlAggregateValueOfListedStocks.GetCheck());
	m_ctrlAggregateValueOfListedStocksBelow.EnableWindow(m_ctrlAggregateValueOfListedStocks.GetCheck());
	m_ctrlTradingVolumeAbove.EnableWindow(m_ctrlTradingVolume.GetCheck());
	m_ctrlTradingVolumeBelow.EnableWindow(m_ctrlTradingVolume.GetCheck());
	m_ctrlParValueAbove.EnableWindow(m_ctrlParValue.GetCheck());
	m_ctrlParValueBelow.EnableWindow(m_ctrlParValue.GetCheck());
}
void CDlgSearchRng::OnCheckEquityCapital()
{
	m_ctrlEquityCapitalAbove.EnableWindow(m_ctrlEquityCapital.GetCheck());
	m_ctrlEquityCapitalBelow.EnableWindow(m_ctrlEquityCapital.GetCheck());
}

void CDlgSearchRng::OnCheckPriceRange()
{
	m_ctrlPriceRangeAbove.EnableWindow(m_ctrlPriceRange.GetCheck());
	m_ctrlPriceRangeBelow.EnableWindow(m_ctrlPriceRange.GetCheck());
}

void CDlgSearchRng::OnCheckAggregateValueOfListedStocks()
{
	m_ctrlAggregateValueOfListedStocksAbove.EnableWindow(m_ctrlAggregateValueOfListedStocks.GetCheck());
	m_ctrlAggregateValueOfListedStocksBelow.EnableWindow(m_ctrlAggregateValueOfListedStocks.GetCheck());
}

void CDlgSearchRng::OnCheckTradingVolume()
{
	m_ctrlTradingVolumeAbove.EnableWindow(m_ctrlTradingVolume.GetCheck());
	m_ctrlTradingVolumeBelow.EnableWindow(m_ctrlTradingVolume.GetCheck());
}

void CDlgSearchRng::OnCheckParValue()
{
	m_ctrlParValueAbove.EnableWindow(m_ctrlParValue.GetCheck());
	m_ctrlParValueBelow.EnableWindow(m_ctrlParValue.GetCheck());
}

void CDlgSearchRng::SaveValue()
{
	CConditionResultWnd* pWnd = (CConditionResultWnd*)m_pParentWnd;
	CString szValue;
	
//KHD: 제외종목 저장 
	pWnd->m_bIssuesForAdministration	= m_ctrlIssuesForAdministration.GetCheck();
	szValue.Format("%d",pWnd->m_bIssuesForAdministration);
	::WritePrivateProfileString("Except_JM", "Issue", szValue, m_stSavePath);

	pWnd->m_bAttentionInvestment		= m_ctrlInvestAttention.GetCheck();
	szValue.Format("%d",pWnd->m_bAttentionInvestment);
	::WritePrivateProfileString("Except_JM", "AttentionInvestment", szValue, m_stSavePath);

	pWnd->m_bWarningInvestment			= m_ctrlInvestWarning.GetCheck();
	szValue.Format("%d",pWnd->m_bWarningInvestment);
	::WritePrivateProfileString("Except_JM", "WarningInvestment", szValue, m_stSavePath);

	pWnd->m_bNoticeAttentionInvestment	= m_ctrlPreInvestRisk.GetCheck();
	szValue.Format("%d",pWnd->m_bNoticeAttentionInvestment);
	::WritePrivateProfileString("Except_JM", "NoticeAttentionInvestment", szValue, m_stSavePath);


	pWnd->m_bDangerInvestment	= m_ctrlInvestRisk.GetCheck();
	szValue.Format("%d",pWnd->m_bDangerInvestment);
	::WritePrivateProfileString("Except_JM", "DangerInvestment", szValue, m_stSavePath);

	pWnd->m_bDealStop					= m_ctrlDealStop.GetCheck();
	szValue.Format("%d",pWnd->m_bDealStop);
	::WritePrivateProfileString("Except_JM", "CheckDealStop", szValue, m_stSavePath);

	pWnd->m_bTidyDeal					= m_ctrlTidyDeal.GetCheck();
	szValue.Format("%d",pWnd->m_bTidyDeal);
	::WritePrivateProfileString("Except_JM", "TidyDeal", szValue, m_stSavePath);

	pWnd->m_bInvestAttention			= m_ctrlAttentionInvestment.GetCheck();
	szValue.Format("%d",pWnd->m_bInvestAttention);
	::WritePrivateProfileString("Except_JM", "InvestAttention", szValue, m_stSavePath);


	pWnd->m_bInsincerityNotice			= m_ctrlInsincerityNotice.GetCheck();
	szValue.Format("%d",pWnd->m_bInsincerityNotice);
	::WritePrivateProfileString("Except_JM", "InsincerityNotice", szValue, m_stSavePath);

	pWnd->m_bPreferenceStock			= m_ctrlPreferenceStock.GetCheck();
	szValue.Format("%d",pWnd->m_bPreferenceStock);
	::WritePrivateProfileString("Except_JM", "CheckPreferenceStock", szValue, m_stSavePath);

	pWnd->m_bOver						= FALSE;
	szValue.Format("%d",pWnd->m_bOver);
	::WritePrivateProfileString("Except_JM", "Over", szValue, m_stSavePath);
//<<
 
// 적용범위 저장 
	pWnd->m_bEquityCapital					= m_ctrlEquityCapital.GetCheck();
	szValue.Format("%d",pWnd->m_bEquityCapital);
	::WritePrivateProfileString("Apply_EquityCapital", "EquityCapital", szValue, m_stSavePath);

	pWnd->m_bPriceRange						= m_ctrlPriceRange.GetCheck();
	szValue.Format("%d",pWnd->m_bPriceRange);
	::WritePrivateProfileString("Apply_PriceRange", "PriceRange", szValue, m_stSavePath);

	pWnd->m_bAggregateValueOfListedStocks	= m_ctrlAggregateValueOfListedStocks.GetCheck();
	szValue.Format("%d",pWnd->m_bAggregateValueOfListedStocks);
	::WritePrivateProfileString("Apply_AggregateValueOfListedStocks", "AggregateValueOfListedStocks", szValue, m_stSavePath);

	pWnd->m_bTradingVolume					= m_ctrlTradingVolume.GetCheck();
	szValue.Format("%d",pWnd->m_bTradingVolume);
	::WritePrivateProfileString("Apply_TradingVolume", "TradingVolume", szValue, m_stSavePath);

	pWnd->m_bParValue						= m_ctrlParValue.GetCheck();
	szValue.Format("%d",pWnd->m_bParValue);
	::WritePrivateProfileString("Apply_ParValue", "ParValue", szValue, m_stSavePath);

//	<<

	if(pWnd->m_bEquityCapital)
	{
		m_ctrlEquityCapitalAbove.GetWindowText(szValue);
		::WritePrivateProfileString("Apply_EquityCapital", "Above", szValue, m_stSavePath);
		pWnd->m_nEquityCapitalAbove = _ttoi(szValue);
		
		m_ctrlEquityCapitalBelow.GetWindowText(szValue);
		::WritePrivateProfileString("Apply_EquityCapital", "Below", szValue, m_stSavePath);
		pWnd->m_nEquityCapitalBelow = _ttoi(szValue);
	}

	if(pWnd->m_bPriceRange)
	{
		m_ctrlPriceRangeAbove.GetWindowText(szValue);
		::WritePrivateProfileString("Apply_PriceRange", "Above", szValue, m_stSavePath);
		pWnd->m_nPriceRangeAbove = _ttoi(szValue);

		m_ctrlPriceRangeBelow.GetWindowText(szValue);
		::WritePrivateProfileString("Apply_PriceRange", "Below", szValue, m_stSavePath);
		pWnd->m_nPriceRangeBelow  = _ttoi(szValue);
	}

	if(pWnd->m_bAggregateValueOfListedStocks)
	{
		m_ctrlAggregateValueOfListedStocksAbove.GetWindowText(szValue);
		::WritePrivateProfileString("Apply_AggregateValueOfListedStocks", "Above", szValue, m_stSavePath);
		pWnd->m_nAggregateValueOfListedStocksAbove = _ttoi(szValue);

		m_ctrlAggregateValueOfListedStocksBelow.GetWindowText(szValue);
		::WritePrivateProfileString("Apply_AggregateValueOfListedStocks", "Below", szValue, m_stSavePath);
		pWnd->m_nAggregateValueOfListedStocksBelow = _ttoi(szValue);
	}

	if(pWnd->m_bTradingVolume)
	{
		m_ctrlTradingVolumeAbove.GetWindowText(szValue);
		::WritePrivateProfileString("Apply_TradingVolume", "Above", szValue, m_stSavePath);
		pWnd->m_nTradingVolumeAbove = _ttoi(szValue);

		m_ctrlTradingVolumeBelow.GetWindowText(szValue);
		::WritePrivateProfileString("Apply_TradingVolume", "Below", szValue, m_stSavePath);
		pWnd->m_nTradingVolumeBelow = _ttoi(szValue);
	}

	if(pWnd->m_bParValue)
	{
		m_ctrlParValueAbove.GetWindowText(szValue);
		::WritePrivateProfileString("Apply_ParValue", "Above", szValue, m_stSavePath);
		pWnd->m_nParValueAbove = _ttoi(szValue);

		m_ctrlParValueBelow.GetWindowText(szValue);
		::WritePrivateProfileString("Apply_ParValue", "Below", szValue, m_stSavePath);
		pWnd->m_nParValueBelow = _ttoi(szValue);
	}

}

BOOL CDlgSearchRng::VerificationValues()
{
	CString szValueAbove;
	CString szValueBelow;
	CString szMsg;
	BOOL	bCheck = FALSE;
	
	if(m_ctrlEquityCapital.GetCheck())
	{
		m_ctrlEquityCapitalAbove.GetWindowText(szValueAbove);
		m_ctrlEquityCapitalBelow.GetWindowText(szValueBelow);
		szMsg  = _T("자본금");
		bCheck = TRUE;
	}

	if(m_ctrlPriceRange.GetCheck())
	{
		m_ctrlPriceRangeAbove.GetWindowText(szValueAbove);
		m_ctrlPriceRangeBelow.GetWindowText(szValueBelow);
		szMsg  = _T("가격대");
		bCheck = TRUE;
	}

	if(m_ctrlAggregateValueOfListedStocks.GetCheck())
	{
		m_ctrlAggregateValueOfListedStocksAbove.GetWindowText(szValueAbove);
		m_ctrlAggregateValueOfListedStocksBelow.GetWindowText(szValueBelow);
		szMsg  = _T("시가총액");
		bCheck = TRUE;
	}

	if(m_ctrlTradingVolume.GetCheck())
	{
		m_ctrlTradingVolumeAbove.GetWindowText(szValueAbove);
		m_ctrlTradingVolumeBelow.GetWindowText(szValueBelow);
		szMsg  = _T("거래량");
		bCheck = TRUE;
	}

	if(m_ctrlParValue.GetCheck())
	{
		m_ctrlParValueAbove.GetWindowText(szValueAbove);
		m_ctrlParValueBelow.GetWindowText(szValueBelow);
		szMsg  = _T("액면가");
		bCheck = TRUE;
	}	
	
	if(_ttoi(szValueAbove) >= _ttoi(szValueBelow) && bCheck)
	{
		szMsg += _T("에 잘못된 조건이 있습니다");
		::MessageBox(m_hWnd, szMsg, PROGRAM_TITLE_BRIGHT, MB_OK);
		return FALSE;
	}

	return TRUE;
}


void CDlgSearchRng::OnChangeEquityCapitalAbove()
{
	VerificationInput(IDC_EQUITY_CAPITAL_ABOVE);
}

void CDlgSearchRng::OnChangeEquityCapitalBelow()
{
	VerificationInput(IDC_EQUITY_CAPITAL_BELOW);
}

void CDlgSearchRng::OnChangePriceRangeAbove()
{
	VerificationInput(IDC_PRICE_RANGE_ABOVE);
}

void CDlgSearchRng::OnChangePriceRangeBelow()
{
	VerificationInput(IDC_PRICE_RANGE_BELOW);
}

void CDlgSearchRng::OnChangeOnCheckAggregateValueOfListedStocksAbove()
{
	VerificationInput(IDC_AGGREGATE_VALUE_OF_LISTED_STOCKS_ABOVE);
}

void CDlgSearchRng::OnChangeOnCheckAggregateValueOfListedStocksBelow()
{
	VerificationInput(IDC_AGGREGATE_VALUE_OF_LISTED_STOCKS_BELOW);
}

void CDlgSearchRng::OnChangeTradingVolumeAbove()
{
	VerificationInput(IDC_TRADING_VOLUME_ABOVE);
}

void CDlgSearchRng::OnChangeTradingVolumeBelow()
{
	VerificationInput(IDC_TRADING_VOLUME_BELOW);
}

void CDlgSearchRng::OnChangeParValueAbove()
{
	VerificationInput(IDC_PAR_VALUE_ABOVE);
}

void CDlgSearchRng::OnChangeParValueBelow()
{
	VerificationInput(IDC_PAR_VALUE_BELOW);
}

BOOL CDlgSearchRng::VerificationInput(UINT ID)
{
	CEdit* pCurrEditWnd = (CEdit*)GetDlgItem(ID);
	
	CString szCurrWndText;
	pCurrEditWnd->GetWindowText(szCurrWndText);

	for(int i = 0; i < szCurrWndText.GetLength(); i++)
	{
		char ch = szCurrWndText.GetAt(i);
		if(ch < '0' || ch > '9')
		{
			char* szMsg = "숫자만 입력 가능합니다.";
			::MessageBox(m_hWnd, szMsg, PROGRAM_TITLE_BRIGHT, MB_OK);

			szCurrWndText.Remove(ch);
			pCurrEditWnd->SetWindowText(szCurrWndText);
			UpdateData(FALSE);
			
			return FALSE;
		}
	}
	return TRUE;
}