#include "stdafx.h"
#include "DlgEditSubject.h"
#include "ConditionResultWnd.h"
#include "MainFrmWnd.h"



#define BETWEEN_RECT			5
#define BETWEEN_CTRL			5
#define HEIGHT_TOP_RAIDO		30
#define HEIGHT_TREE				200
#define HEIGHT_EXCEPTSTOCK		85
#define HEIGHT_MONTHSETTLEMENT	100

#define TREE_MARKET				0
#define TREE_PORTFOLIO			1
#define TREE_RECOMMENDATION		2

const LPCSTR MONTHS[] = {_T("1월"), _T("2월"), _T("3월"), _T("4월"), _T("5월"), _T("6월"), 
_T("7월"), _T("8월"), _T("9월"), _T("10월"), _T("11월"), _T("12월")};

extern HINSTANCE g_hInstance;

IMPLEMENT_DYNAMIC(CDlgEditSubject, CDialog)

CDlgEditSubject::CDlgEditSubject(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgEditSubject::IDD, pParent)
{
	m_pReWnd = pParent;
	m_gridResult = NULL;
}

CDlgEditSubject::~CDlgEditSubject()
{
}

void CDlgEditSubject::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDlgEditSubject, CDialog)
	ON_WM_CREATE()
	
	ON_BN_CLICKED(IDC_BTN_RADIO_ALL, OnBtnRadioClick)
	ON_BN_CLICKED(IDC_BTN_RADIO_MARCH, OnBtnRadioClick)
	ON_BN_CLICKED(IDC_BTN_RADIO_JUNE, OnBtnRadioClick)
	ON_BN_CLICKED(IDC_BTN_RADIO_SEPTEMBER, OnBtnRadioClick)
	ON_BN_CLICKED(IDC_BTN_RADIO_DECEMBER, OnBtnRadioClick)
	ON_BN_CLICKED(IDC_BTN_RADIO_ESTIVAL, OnBtnRadioClick)

	ON_COMMAND_RANGE(IDC_BTN_CHECK_ISSUESFORADMINISTRATION, IDC_BTN_CHECK_ETF, OnBtnExcept)

	ON_CBN_SELCHANGE(IDC_COMBO_MONTHS, OnChangeCombo)

	ON_BN_CLICKED(IDOK, OnBnClickedOk)
	ON_WM_PAINT()
	ON_WM_CTLCOLOR()
	ON_WM_DESTROY()
	ON_WM_NCDESTROY()
	
END_MESSAGE_MAP()

BOOL CDlgEditSubject::OnInitDialog()
{
	SetInit();
	return TRUE;	
}

void CDlgEditSubject::SetInit()
{
	OnSize();
	m_gridResult->SetHeader();
	m_gridResult->SetMultiSelectMode(TRUE);
	
	SetInitCtrl();

	m_treeCtrlMarket.ShowWindow(SW_SHOW);
	m_treeCtrlMarket.SetRedraw(TRUE);
}

int CDlgEditSubject::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;

	HINSTANCE hInstSave = AfxGetResourceHandle();
	AfxSetResourceHandle(g_hInstance);
	
	LOGFONT logFont;
	GETFONT(logFont, 12, 400, m_fontDefault);

	m_brushBk.CreateSolidBrush(COLOR_DLG_CANVAS);

	CConditionResultWnd* pWnd = (CConditionResultWnd*)m_pReWnd;

	CRect rect = CRect(0, 0, 0, 0);
	DWORD dwStyle = WS_VISIBLE | WS_TABSTOP | WS_CHILD | TVS_HASBUTTONS | TVS_LINESATROOT | TVS_HASLINES | TVS_DISABLEDRAGDROP | TVS_NOTOOLTIPS;

	m_treeCtrlMarket.Create(dwStyle, rect, this, IDC_TREE_MARKET);
	m_treeCtrlMarket.SetFont(&m_fontDefault);

	dwStyle = WS_CHILD | WS_VISIBLE | BS_GROUPBOX;
	m_btnExceptStockGroup.Create(_T("제외종목"), dwStyle, rect, this, IDC_BTN_EXCEPTSTOCK);
	m_btnExceptStockGroup.SetFont(&m_fontDefault);

	dwStyle = WS_VISIBLE | WS_CHILD | WS_TABSTOP | BS_AUTOCHECKBOX;
	m_btnCheckIssuesForAdministration.Create(_T("관리종목"), dwStyle, rect, this, IDC_BTN_CHECK_ISSUESFORADMINISTRATION);
	m_btnCheckIssuesForAdministration.SetFont(&m_fontDefault);

	m_btnCheckDealStop.Create(_T("거래정지"),  dwStyle, rect, this, IDC_BTN_CHECK_DEALSTOP);
	m_btnCheckDealStop.SetFont(&m_fontDefault);

	m_btnCheckTidyDeal.Create(_T("정리매매"), dwStyle, rect, this, IDC_BTN_CHECK_TIDYDEAL);
	m_btnCheckTidyDeal.SetFont(&m_fontDefault);

	m_btnCheckInsincerityNotice.Create(_T("불성실공시"), dwStyle, rect, this, IDC_BTN_CHECK_INSINCERITYNOTICE);
	m_btnCheckInsincerityNotice.SetFont(&m_fontDefault);
		
	m_btnCheckAttentionInvestment.Create(_T("투자주의"), dwStyle, rect, this, IDC_BTN_CHECK_ATTENTIONINVESTMENT);
	m_btnCheckAttentionInvestment.SetFont(&m_fontDefault);

	m_btnCheckNoticeAttentionInvestment.Create(_T("투자위험예고"), dwStyle, rect, this, IDC_BTN_CHECK_NOTICEATTENTIONINVESTMENT);
	m_btnCheckNoticeAttentionInvestment.SetFont(&m_fontDefault);	
	
	m_btnCheckPreferenceStock.Create(_T("우선주"), dwStyle, rect, this, IDC_BTN_CHECK_PREFERENCESTOCK);
	m_btnCheckPreferenceStock.SetFont(&m_fontDefault);	

	m_btnCheckDeposit.Create(_T("증거금100%"),  dwStyle, rect, this, IDC_BTN_CHECK_DEPOSIT);
	m_btnCheckDeposit.SetFont(&m_fontDefault);	

	m_btnCheckDangerInvestment.Create(_T("투자위험"), dwStyle, rect, this, IDC_BTN_CHECK_DANGERINVESTMENT);
	m_btnCheckDangerInvestment.SetFont(&m_fontDefault);

	m_btnCheckWarningInvestment.Create(_T("투자경고"), dwStyle, rect, this, IDC_BTN_CHECK_WARNINGINVESTMENT);
	m_btnCheckWarningInvestment.SetFont(&m_fontDefault);

	m_btnCheckETF.Create(_T("ETF"), dwStyle, rect, this, IDC_BTN_CHECK_ETF);
	m_btnCheckETF.SetFont(&m_fontDefault);
/*
	m_btnCheckETN.Create(_T("ETN"), dwStyle, rect, this, IDC_BTN_CHECK_ETN);
	m_btnCheckETN.SetFont(&m_fontDefault);
*/
	dwStyle = WS_CHILD | WS_VISIBLE | BS_GROUPBOX;
	m_btnMonthSettlementGroup.Create(_T("결산월"), dwStyle, rect, this, IDC_BTN_MONTHSETTLEMENT);
	m_btnMonthSettlementGroup.SetFont(&m_fontDefault);

	dwStyle = WS_VISIBLE | WS_CHILD | WS_TABSTOP | WS_GROUP | BS_AUTORADIOBUTTON;
	m_btnRadioAll.Create(_T("전체"), dwStyle, rect, this, IDC_BTN_RADIO_ALL);
	m_btnRadioAll.SetFont(&m_fontDefault);
	
	dwStyle = WS_VISIBLE | WS_CHILD | WS_TABSTOP | BS_AUTORADIOBUTTON;
	m_btnRadioMarch.Create(_T("3월"), dwStyle, rect, this, IDC_BTN_RADIO_MARCH);
	m_btnRadioMarch.SetFont(&m_fontDefault);

	m_btnRadioJune.Create(_T("6월"), dwStyle, rect, this, IDC_BTN_RADIO_JUNE);
	m_btnRadioJune.SetFont(&m_fontDefault);

	m_btnRadioSeptember.Create(_T("9월"), dwStyle, rect, this, IDC_BTN_RADIO_SEPTEMBER);
	m_btnRadioSeptember.SetFont(&m_fontDefault);

	m_btnRadioDecember.Create(_T("12월"), dwStyle, rect, this, IDC_BTN_RADIO_DECEMBER);
	m_btnRadioDecember.SetFont(&m_fontDefault);

	m_btnRadioEstival.Create(_T("기타"), dwStyle, rect, this, IDC_BTN_RADIO_ESTIVAL);
	m_btnRadioEstival.SetFont(&m_fontDefault);

	dwStyle = WS_CHILD | WS_VISIBLE | WS_VSCROLL | CBS_DROPDOWNLIST;
	m_comboMonths.Create(dwStyle, rect, this, IDC_COMBO_MONTHS);
	m_comboMonths.SetFont(&m_fontDefault);

	dwStyle = WS_VISIBLE | WS_CHILD | SS_CENTERIMAGE;
	m_staticGuide.Create(_T(" ※설정사항은 [조건검색] 화면에만 적용됩니다."), dwStyle, rect, this, IDC_STATIC_GUIDE);
	m_staticGuide.SetFont(&m_fontDefault);

	for(int i = 0; i < _countof(MONTHS); i++)
	{
		m_comboMonths.AddString(MONTHS[i]);
	}
	m_comboMonths.SetCurSel(0);
	m_comboMonths.EnableWindow(m_btnRadioEstival.GetCheck());

	m_gridResult = new CGrid_Result;
	m_gridResult->Create(NULL, _T(""), WS_CHILD | WS_BORDER | WS_VISIBLE, rect, this, NULL);
	m_gridResult->SetFont(&m_fontDefault);
	m_gridResult->OnSetup(FALSE, "대상변경");

	ST_GRIDCOLOR stGridColor;
	stGridColor.clrGridHeader = pWnd->GetAxColor(74);
 	stGridColor.clrGridHilight = pWnd->GetAxColor(78);
	m_gridResult->ChangeGridColor(stGridColor);

	dwStyle = WS_VISIBLE | WS_CHILD | SS_LEFT | SS_CENTERIMAGE;
	m_stcTarget.Create(_T(""), dwStyle, rect, this);
	m_stcTarget.SetFont(&m_fontDefault);

	m_stcTotCntTargetItem.Create(_T(""), dwStyle, rect, this);
	m_stcTotCntTargetItem.SetFont(&m_fontDefault);

	m_btnOk.Create(_T("확인"), rect, this, IDOK);
	pWnd->SetBtnImg(&m_btnOk, 4);
	
	m_btnCancel.Create(_T("취소"), rect, this, IDCANCEL);
	pWnd->SetBtnImg(&m_btnCancel, 4);

	AfxSetResourceHandle(hInstSave);
	
	return 0;
}

void CDlgEditSubject::OnSize()
{
	CRect reClient;
	GetClientRect(reClient);

	CRect reTopRadio;
	CRect reTreeCtrl;
	CRect reGridCtrl;
	CRect reExceptStock;
	CRect reMonthSettlement;
	CRect reBottom;
	CRect reStatic;

	reTopRadio.left		= reClient.left;
	reTopRadio.top		= reClient.top;
	reTopRadio.right	= reClient.right;
	reTopRadio.bottom	= reTopRadio.top + HEIGHT_TOP_RAIDO;
	OnSizeTopRadio(reTopRadio);

	reStatic.left		= reClient.left + 5;
	reStatic.top		= reClient.top + 5;
	reStatic.right		= reStatic.left + 300;
	reStatic.bottom		= reStatic.top + 20;
	m_stcTarget.MoveWindow(reStatic);

	reStatic.right		= reClient.right - 5;
	reStatic.left		= reStatic.right - 120;
	m_stcTotCntTargetItem.MoveWindow(reStatic);	

	reTreeCtrl.left		= reClient.left + BETWEEN_RECT;
	reTreeCtrl.top		= reStatic.bottom + 1;//reTopRadio.bottom + BETWEEN_RECT;
	reTreeCtrl.right	= (reClient.right - BETWEEN_RECT) / 2;
	reTreeCtrl.bottom	= reTreeCtrl.top + HEIGHT_TREE + 40;
	OnSizeTreeCtrl(reTreeCtrl);

	reGridCtrl.left		= reTreeCtrl.right + 5;
	reGridCtrl.top		= reTreeCtrl.top;
	reGridCtrl.right	= reClient.right - 5;
	reGridCtrl.bottom	= reTreeCtrl.bottom;
	m_gridResult->MoveWindow(reGridCtrl);

	reExceptStock.left		= reClient.left + BETWEEN_RECT;
	reExceptStock.top		= reTreeCtrl.bottom + BETWEEN_RECT;
	reExceptStock.right		= reClient.right - BETWEEN_RECT;
	reExceptStock.bottom	= reExceptStock.top + HEIGHT_EXCEPTSTOCK;
	OnSizeExceptStock(reExceptStock);

	reMonthSettlement.left		= reClient.left + BETWEEN_RECT;
	reMonthSettlement.top		= reExceptStock.bottom + BETWEEN_RECT;
	reMonthSettlement.right		= reClient.right - BETWEEN_RECT;
	reMonthSettlement.bottom	= reMonthSettlement.top + HEIGHT_MONTHSETTLEMENT;
	OnSizeMonthSettlement(reMonthSettlement);

	reBottom = reClient;
	reBottom.top = reBottom.bottom - 25;
	reBottom.bottom = reBottom.top + 20;
	reBottom.right -= 5;
	reBottom.left = reBottom.right - 60;
	m_btnCancel.MoveWindow(reBottom);

	reBottom.right = reBottom.left - 2;
	reBottom.left = reBottom.right - 60;
	m_btnOk.MoveWindow(reBottom);
}

void CDlgEditSubject::OnSizeTopRadio(CRect rect)
{
	rect.left += 50;
	rect.right -= 50;
	rect.top += 10;

	int nWidth = rect.Width() / 3;
}

void CDlgEditSubject::OnSizeTreeCtrl(CRect rect)
{
	m_treeCtrlMarket.ModifyStyleEx(1, WS_EX_CLIENTEDGE);
	m_treeCtrlMarket.MoveWindow(rect);
}

void CDlgEditSubject::OnSizeExceptStock(CRect rect)
{
	CRect reTemp, reGroup;

	reGroup.left		= rect.left;
	reGroup.top		= rect.top + BETWEEN_CTRL;
	reGroup.right	= rect.right;
//	reGroup.bottom	= reGroup.top + 60;
	reGroup.bottom	= reGroup.top + HEIGHT_EXCEPTSTOCK;
	m_btnExceptStockGroup.MoveWindow(reGroup);
	
	reTemp.left		= reGroup.left + BETWEEN_CTRL;
	reTemp.top		= reGroup.top + 15;
	reTemp.right	= rect.right;
	reTemp.bottom	= reTemp.top + 20;
	
	reTemp.right = reTemp.left + 95;
	m_btnCheckIssuesForAdministration.MoveWindow(reTemp);

	reTemp.left		= reTemp.right + BETWEEN_CTRL;
	reTemp.right	= reTemp.left + 80;
	m_btnCheckDealStop.MoveWindow(reTemp);

	reTemp.left		= reTemp.right;
	reTemp.right	= reTemp.left + 90;
	m_btnCheckTidyDeal.MoveWindow(reTemp);	
	
	reTemp.left		= reTemp.right;
	reTemp.right	= reTemp.left + 90;
	m_btnCheckInsincerityNotice.MoveWindow(reTemp);

	reTemp.left		= reTemp.right;
	reTemp.right	= reGroup.right - 2;
	m_btnCheckAttentionInvestment.MoveWindow(reTemp);

	reTemp.left		= rect.left + 5; //BETWEEN_CTRL * 5;
	reTemp.top		= reTemp.bottom + 2;
	reTemp.bottom	= reTemp.top + 20;
	reTemp.right	= reTemp.left + 95;
	m_btnCheckNoticeAttentionInvestment.MoveWindow(reTemp);
	
	reTemp.left		= reTemp.right + BETWEEN_CTRL;
	reTemp.right	= reTemp.left + 80;
	m_btnCheckPreferenceStock.MoveWindow(reTemp);

	reTemp.left		= reTemp.right;
	reTemp.right	= reTemp.left + 90;
	m_btnCheckDeposit.MoveWindow(reTemp);

	reTemp.left		= reTemp.right;
	reTemp.right	= reTemp.left + 90;
	m_btnCheckDangerInvestment.MoveWindow(reTemp);

	reTemp.left		= reTemp.right;
	reTemp.right	= reGroup.right - 2;
	m_btnCheckWarningInvestment.MoveWindow(reTemp);

	reTemp.left		= rect.left + 5; //BETWEEN_CTRL * 5;
	reTemp.top		= reTemp.bottom + 2;
	reTemp.bottom	= reTemp.top + 20;
	reTemp.right	= reTemp.left + 95;
	m_btnCheckETF.MoveWindow(reTemp);
	/*
	reTemp.left		= reTemp.right + BETWEEN_CTRL;
	reTemp.right	= reTemp.left + 80;
	m_btnCheckETN.MoveWindow(reTemp);
	*/
}

void CDlgEditSubject::OnSizeMonthSettlement(CRect rect)
{
	CRect reTemp, reGroup;
		
	reGroup.left		= rect.left;
	reGroup.top		= rect.top + BETWEEN_CTRL;
	reGroup.right	= rect.right;
	reGroup.bottom	= reGroup.top + 40;
	m_btnMonthSettlementGroup.MoveWindow(reGroup);
	
	int nWidth		= (rect.Width() - 10) / 7;
	reTemp.left		= reGroup.left + BETWEEN_CTRL;
	reTemp.top		= reGroup.top + 15;
	reTemp.bottom	= reTemp.top + 20;
	reTemp.right	= reTemp.left + nWidth;
	m_btnRadioAll.MoveWindow(reTemp);

	reTemp.left		= reTemp.right;
	reTemp.right	= reTemp.left + nWidth;
	m_btnRadioMarch.MoveWindow(reTemp);

	reTemp.left		= reTemp.right;
	reTemp.right	= reTemp.left + nWidth;
	m_btnRadioJune.MoveWindow(reTemp);
	
	reTemp.left		= reTemp.right;
	reTemp.right	= reTemp.left + nWidth;
	m_btnRadioSeptember.MoveWindow(reTemp);

	reTemp.left		= reTemp.right;
	reTemp.right	= reTemp.left + nWidth;
	m_btnRadioDecember.MoveWindow(reTemp);
	
	reTemp.left		= reTemp.right;
	reTemp.right	= reTemp.left + nWidth;
	m_btnRadioEstival.MoveWindow(reTemp);

	reTemp.left		= reTemp.right;
	reTemp.right	= reGroup.right - BETWEEN_CTRL;
	reTemp.bottom	+= 100;
	m_comboMonths.MoveWindow(reTemp);

	reTemp.left		= rect.left;
	reTemp.top		= reTemp.bottom -91 + BETWEEN_CTRL;	//위의 콤보에서 +100한건 뺀다.
	reTemp.bottom	= reTemp.top + 12;
	reTemp.right	= reTemp.left + rect.Width();
	m_staticGuide.MoveWindow(reTemp);
}

void CDlgEditSubject::OnBtnRadioClick()
{
	m_comboMonths.EnableWindow(m_btnRadioEstival.GetCheck());
	
	if(m_btnRadioAll.GetCheck())
		m_nRadioMonth = MONTH_ALL;
	else if(m_btnRadioMarch.GetCheck())
		m_nRadioMonth = MONTH_MARCH;
	else if(m_btnRadioJune.GetCheck())
		m_nRadioMonth = MONTH_JUNE;
	else if(m_btnRadioSeptember.GetCheck())
		m_nRadioMonth = MONTH_SEPTEMBER;
	else if(m_btnRadioDecember.GetCheck())
		m_nRadioMonth = MONTH_DECEMBER;
	else
		m_nRadioMonth = MONTH_ETC;

	OnBtnExcept(0);
}

void CDlgEditSubject::OnBnClickedOk()
{
	BOOL bCheck = FALSE;
	CString stTmp, strText;

	CConditionResultWnd* pWnd = (CConditionResultWnd*)m_pReWnd;

	pWnd->m_arTargetResult.RemoveAll();

	pWnd->m_strEditSubjectSelectItemText = m_treeCtrlMarket.m_strSelectItemText;
	pWnd->m_strEditSubjectSelectParentItemText = m_treeCtrlMarket.m_strParentItemText;

	CString strCode;
	CStringArray saItemCode;
	for (int i=0; i < m_gridResult->GetNumberRows(); i++)
	{
		strCode = m_gridResult->GetCellData(i, 0);
		saItemCode.Add(strCode);
	}

	for (int j=0; j < saItemCode.GetSize(); j++)
		pWnd->m_arTargetResult.Add(saItemCode.GetAt(j));

	pWnd->SetTargetBit();

	pWnd->m_arTargetMText.RemoveAll();
	pWnd->m_arTargetMText.Add(m_strTreeName);
	pWnd->m_bIssuesForAdministration = m_btnCheckIssuesForAdministration.GetCheck();
	pWnd->m_bDealStop = m_btnCheckDealStop.GetCheck();
	pWnd->m_bTidyDeal = m_btnCheckTidyDeal.GetCheck();
	pWnd->m_bInsincerityNotice = m_btnCheckInsincerityNotice.GetCheck();
	pWnd->m_bAttentionInvestment = m_btnCheckAttentionInvestment.GetCheck();
	pWnd->m_bNoticeAttentionInvestment = m_btnCheckNoticeAttentionInvestment.GetCheck();
	pWnd->m_bPreferenceStock = m_btnCheckPreferenceStock.GetCheck();
	pWnd->m_bDeposit = m_btnCheckDeposit.GetCheck();
	pWnd->m_bETF = m_btnCheckETF.GetCheck();
	pWnd->m_bDangerInvestment = m_btnCheckDangerInvestment.GetCheck();
	pWnd->m_bWarningInvestment = m_btnCheckWarningInvestment.GetCheck();
	pWnd->m_nSelRadioMonth = m_nRadioMonth;
	if (m_nRadioMonth == MONTH_ETC)
		pWnd->m_nIndexMonthCombo = m_comboMonths.GetCurSel();	

	pWnd->m_strJangGubun = m_strJangGubun;

	CDialog::OnOK();
}

void CDlgEditSubject::SetInitCtrl()
{
	CConditionResultWnd* pWnd = (CConditionResultWnd*)m_pReWnd;

	m_btnCheckIssuesForAdministration.SetCheck(pWnd->m_bIssuesForAdministration);
	m_btnCheckDealStop.SetCheck(pWnd->m_bDealStop);
	m_btnCheckTidyDeal.SetCheck(pWnd->m_bTidyDeal);
	m_btnCheckInsincerityNotice.SetCheck(pWnd->m_bInsincerityNotice);
	m_btnCheckAttentionInvestment.SetCheck(pWnd->m_bAttentionInvestment);
	m_btnCheckNoticeAttentionInvestment.SetCheck(pWnd->m_bNoticeAttentionInvestment);
	m_btnCheckPreferenceStock.SetCheck(pWnd->m_bPreferenceStock);
	m_btnCheckDeposit.SetCheck(pWnd->m_bDeposit);
	m_btnCheckETF.SetCheck(pWnd->m_bETF);
	m_btnCheckDangerInvestment.SetCheck(pWnd->m_bDangerInvestment);
	m_btnCheckWarningInvestment.SetCheck(pWnd->m_bWarningInvestment);

	switch(pWnd->m_nSelRadioMonth)
	{
		case MONTH_ALL			: m_btnRadioAll.SetCheck(TRUE);							break;
		case MONTH_MARCH		: m_btnRadioMarch.SetCheck(TRUE);						break;
		case MONTH_JUNE			: m_btnRadioJune.SetCheck(TRUE);						break;
		case MONTH_SEPTEMBER	: m_btnRadioSeptember.SetCheck(TRUE);					break;
		case MONTH_DECEMBER		: m_btnRadioDecember.SetCheck(TRUE);					break;
		case MONTH_ETC			: m_btnRadioEstival.SetCheck(TRUE); 
								  m_comboMonths.SetCurSel(pWnd->m_nIndexMonthCombo);	break;
	}

//	m_treeCtrlMarket.SetInit(TREE_MARKET, &m_saMarket, &pWnd->m_listHaveItem, m_pReWnd);	
	m_treeCtrlMarket.SetInit(TREE_MARKET, &pWnd->m_listHaveItem, m_pReWnd);	
	
	CString strUserPath;
	strUserPath.Format("%s%s%s", pWnd->Variant(homeCC), USRDIR, pWnd->Variant(nameCC));

	m_stSavePath =  strUserPath + (CString)FOLDER_FINDDATA + "\\7127_ConditionInfo.dat";

	OnBtnRadioClick();
}


void CDlgEditSubject::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	// 그리기 메시지에 대해서는 CDialog::OnPaint()을(를) 호출하지 마십시오.

	CConditionResultWnd* pWnd = (CConditionResultWnd*)m_pReWnd;
	CRect reClient;
	GetClientRect(reClient);
	dc.FillSolidRect(reClient, pWnd->GetAxColor(66));
}

HBRUSH CDlgEditSubject::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  여기서 DC의 특성을 변경합니다.
	m_brushCtrl.DeleteObject();

	UINT id = pWnd->GetDlgCtrlID();
	
	switch(id)
	{
		case IDC_TREE_MARKET :
		case IDC_TREE_PORTFOLIO :
		case IDC_TREE_RECOMMENDATION:
		case IDC_COMBO_MONTHS :
			pDC->SetBkMode(TRANSPARENT);
			return hbr;
		default :
			CConditionResultWnd* pReWnd = (CConditionResultWnd*)m_pReWnd;
			pDC->SetBkMode(TRANSPARENT);
			m_brushCtrl.CreateSolidBrush(pReWnd->GetAxColor(66));
			hbr = (HBRUSH)m_brushCtrl.m_hObject;
	}
	
	// TODO:  기본값이 적당하지 않으면 다른 브러시를 반환합니다.
	return hbr;
}
void CDlgEditSubject::OnDestroy()
{
	CDialog::OnDestroy();

	m_fontDefault.DeleteObject();
	m_brushBk.DeleteObject();
	m_brushCtrl.DeleteObject();

	if (m_gridResult != NULL)
	{
		m_gridResult->DestroyWindow();
		delete m_gridResult;
		m_gridResult = NULL;
	}
}


void CDlgEditSubject::OnNcDestroy()
{
	CDialog::OnNcDestroy();

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
}

//>>[0000094]alzioyes:대상변경에서 포트폴리오 -> 관심그룹 선택시 검색결과가 나오지 않는 현상.
/*
int CDlgEditSubject::getPortCodeList(LPCSTR stSavePath, CStringArray *pArrTargetResult)
{
	char aBuff[1000+1]={0,};
	if(::GetPrivateProfileString("Tree_Upjong", "Port_Index", "", aBuff, 1000, stSavePath)==0) return 0;

	CString szBuffer(aBuff);
	int nPos = szBuffer.Find(',');
	CString szCode, strTmpCode;
	CStringArray saItemCode;
	CStringArray saItemName;
	CCheckTreeCtrl selectedTree;

	while(nPos>0)
	{
		szCode = szBuffer.Left(nPos);
		szBuffer = szBuffer.Mid(nPos);
		nPos = szBuffer.Find(',');

		saItemCode.RemoveAll();
		saItemName.RemoveAll();
		if(selectedTree.GetItemCodeFromGwanGroup(szCode, &saItemCode, &saItemName))
		{
			for(int i = 0; i < saItemCode.GetSize(); i++)
			{
				strTmpCode = saItemCode.GetAt(i);
				if(strTmpCode.GetLength()==6)
					pArrTargetResult->Add(strTmpCode);
			}
		}
	}
	return pArrTargetResult->GetSize();
}
*/
//<<[0000094]

BOOL CDlgEditSubject::GetUpMasterData(CStringArray *&psaCode, CStringArray *&psaName, int jgub)
{
	CConditionResultWnd* pReWnd = (CConditionResultWnd*)m_pReWnd;
	return pReWnd->GetUpMasterData(psaCode, psaName, jgub);
}

void CDlgEditSubject::GetStockMasterData(CString strJangGubun, CString strSelectItem, CString strSelectItemParent)
{
	UINT Jgub, Upgub;
	
	Jgub = _ttoi(strJangGubun.Mid(0, 1)) * 100;
	Upgub = _ttoi(strJangGubun);

	m_saTargetItemCode.RemoveAll();
	m_saTargetItemName.RemoveAll();
	
	CString strMsg;
	if(strSelectItemParent != "")
		strMsg.Format("%s - %s", strSelectItemParent, strSelectItem);
	else
		strMsg.Format("%s", strSelectItem);

	m_strTreeName = strMsg;
	strMsg.Format("선택대상: %s", strMsg);
	m_stcTarget.SetWindowText(strMsg);

	if(m_strTreeName.CompareNoCase("전체") == 0)
		Jgub = 1000;
	else if(m_strTreeName.CompareNoCase("거래소") == 0)
	{
		Jgub = GETMARKET_KOSPI;
		Upgub = 0;
	}
	else if(m_strTreeName.CompareNoCase("코스닥") == 0)
	{
		Jgub = GETMARKET_KOSDAQ;
		Upgub = 0;
	}
	else if(m_strTreeName.CompareNoCase("KOSPI200") == 0)
	{
		Jgub = GETMARKET_KOSPI200;
		Upgub = 0;
	}
	else if(m_strTreeName.CompareNoCase("KRX100") == 0)
	{
		Jgub = GETMARKET_KRX;
		Upgub = 0;
	}

	CConditionResultWnd* pReWnd = (CConditionResultWnd*)m_pReWnd;
	if(strSelectItemParent == "관심종목")
		pReWnd->GetData_PortItem(m_saTargetItemCode, m_saTargetItemName, _ttoi(strJangGubun));
	else
		pReWnd->GetStockMasterData(m_saTargetItemCode, m_saTargetItemName, Jgub, Upgub);

	m_strJangGubun = strJangGubun;

	OnBtnExcept(0);
}

void CDlgEditSubject::GetItemCodeFromThemeGroup(CString strThemeIndex)
{
	m_saTargetItemCode.RemoveAll();
	m_saTargetItemName.RemoveAll();
	
	CConditionResultWnd* pReWnd = (CConditionResultWnd*)m_pReWnd;
	
	CString sFile;
	sFile.Format("%s\\%s\\%s", pReWnd->Variant(homeCC), TABDIR, FILE_THEME);	
	
	for (int i = 0; ; i++)
	{
		char szBuf[BUFSIZ] = { 0x00, };
		CString sKey;
		
		if (i < 100)
			sKey.Format("%02d", i);
		else
			sKey.Format("%03d", i);
		
		GetPrivateProfileString("FOLDER", sKey, "", szBuf, sizeof(szBuf), sFile);
		
		if (strlen(szBuf) <= 0)
			break;
		
		CString sData = szBuf;
		
		if (sData.GetLength() < 33)
			continue;
		
		CString sName = sData.Left(30);
		CString sTheCode = sData.Right(3);
		
		sName.TrimLeft(); sName.TrimRight();
		sTheCode.TrimLeft(); sTheCode.TrimRight();
		
		if(strThemeIndex != sTheCode)
			continue;

		char szCList[BUFSIZ] = { 0x00, };
		GetPrivateProfileString("TMCODE", sTheCode, "", szCList, sizeof(szCList), sFile);
		
		CString sCList = szCList;
		char szBufname[16];
		CString strName;
		while (1)
		{
			CString sCode = CUtils::Parser(sCList, ";");
			sCode.TrimLeft(); sCode.TrimRight();
			
			if (sCode.GetLength() <= 0)
				break;
			
			sprintf(szBufname, "%s ", sCode);
			strName = (char*)pReWnd->GetModuleMainWnd()->SendMessage(WM_USER, MAKEWPARAM(nameDLL, 0), (LPARAM)szBufname);
			strName.TrimLeft();
			strName.TrimRight();

			m_saTargetItemCode.Add(sCode);
			m_saTargetItemName.Add(strName);
		}
		
		if (m_saTargetItemCode.GetSize() <= 0)
		{
			return ;
		}
		
		CString strTitle;
		strTitle.Format("[%04d] %s", atol(sTheCode), sName);
		m_strTreeName = strTitle;
		strTitle.Format("대상선택: %s", strTitle);
		m_stcTarget.SetWindowText(strTitle);

		break;
	}

	m_strJangGubun = strThemeIndex;

	OnBtnExcept(0);
}
void CDlgEditSubject::OnBtnExcept(UINT nID)
{
	CStringArray saTargetItemCode;
	CStringArray saTargetItemName;

	CConditionResultWnd* pReWnd = (CConditionResultWnd*)m_pReWnd;
	int nIndex = 0;
	for( nIndex = 0; nIndex < m_saTargetItemCode.GetSize(); nIndex++)
	{
		CString strExceptDetail;
		if(pReWnd->m_mapExeptItem.Lookup(m_saTargetItemCode.GetAt(nIndex), strExceptDetail))
		{
			if(strExceptDetail.GetLength() != 8)
				continue;
			
			if(m_btnCheckIssuesForAdministration.GetCheck() && strExceptDetail.GetAt(0) == '1')		// 관리종목
				continue;
			if(m_btnCheckDealStop.GetCheck() && strExceptDetail.GetAt(1) == '1')					// 거래정지
				continue;
			if(m_btnCheckTidyDeal.GetCheck() && strExceptDetail.GetAt(2) == '1')					// 정리매매
				continue;
			if(m_btnCheckInsincerityNotice.GetCheck() && strExceptDetail.GetAt(3) == '1')			// 불성실공시
				continue;
			if(m_btnCheckAttentionInvestment.GetCheck() && strExceptDetail.GetAt(4) == '1')			// 투자주의
				continue;
			if(m_btnCheckWarningInvestment.GetCheck() && strExceptDetail.GetAt(4) == '2')			// 투자경고
				continue;
			if(m_btnCheckDangerInvestment.GetCheck() && strExceptDetail.GetAt(4) == '3')			// 투자위험
				continue;
			if(m_btnCheckNoticeAttentionInvestment.GetCheck() && strExceptDetail.GetAt(4) == '4')	// 투자위험예고
				continue;
			if(m_btnCheckPreferenceStock.GetCheck() && strExceptDetail.GetAt(5) == '1')				// 우선주
				continue;
			if(m_btnCheckDeposit.GetCheck() && strExceptDetail.GetAt(6) == '1')						// 증거금
				continue;
			if(m_btnCheckETF.GetCheck() && strExceptDetail.GetAt(7) == '1')							// ETF
				continue;
		}
		
		saTargetItemCode.Add(m_saTargetItemCode.GetAt(nIndex));
		saTargetItemName.Add(m_saTargetItemName.GetAt(nIndex));	
	}

	CStringArray saFilterMonthItemCode;
	CStringArray saFiltermonthItemName;
	
	if(!m_btnRadioAll.GetCheck())
	{
		CStringArray saMonthSettlement;
		GetMonthSettlement(&saMonthSettlement);

		for( nIndex = 0; nIndex < saMonthSettlement.GetSize(); nIndex++)
		{
			CString strItemCodeMonth;
			strItemCodeMonth = saMonthSettlement.GetAt(nIndex);
			for(int nTargetIndex = 0; nTargetIndex < saTargetItemCode.GetSize(); nTargetIndex++)
			{
				if(strItemCodeMonth == saTargetItemCode.GetAt(nTargetIndex))
				{
					saFilterMonthItemCode.Add(saTargetItemCode.GetAt(nTargetIndex));
					saFiltermonthItemName.Add(saTargetItemName.GetAt(nTargetIndex));
					break;
				}
			}
		}
	}
	else
	{
		saFilterMonthItemCode.Copy(saTargetItemCode);
		saFiltermonthItemName.Copy(saTargetItemName);
	}
	
	m_gridResult->ClearAll();
	m_gridResult->SetNumberRows(saFilterMonthItemCode.GetSize());
	for(nIndex = 0; nIndex < saFilterMonthItemCode.GetSize(); nIndex++)
	{
		m_gridResult->SetAddRow(0, nIndex, saFilterMonthItemCode.GetAt(nIndex), saFiltermonthItemName.GetAt(nIndex), FALSE);
	}

	m_gridResult->RedrawAll();

	CString strMsg;
	strMsg.Format("%s%d개", "조회개수 : ", saFilterMonthItemCode.GetSize());
	m_stcTotCntTargetItem.SetWindowText(strMsg);
}

void CDlgEditSubject::RequestHaveItem(CString strAccNo, CString strText)
{
	m_gridResult->ClearAll();
	m_saTargetItemCode.RemoveAll();
	m_saTargetItemName.RemoveAll();

	CString strMsg;
	strMsg.Format("계좌번호 : %s", strText);
	m_strTreeName = strMsg;
	strMsg.Format("대상선택: %s", strMsg);
	m_stcTarget.SetWindowText(strMsg);

	CConditionResultWnd* pReWnd = (CConditionResultWnd*)m_pReWnd;
	pReWnd->RequestHaveItem(strAccNo);
}

void CDlgEditSubject::SendHaveItem(CStringArray* psaItemCode)
{
	CConditionResultWnd* pReWnd = (CConditionResultWnd*)m_pReWnd;
	char szBufname[16] = { 0 };
	for(int nIndex = 0; nIndex < psaItemCode->GetSize(); nIndex++)
	{
		CString strItemCode = psaItemCode->GetAt(nIndex);
		
		sprintf(szBufname, "%s ", strItemCode);

		CString strItemName = (char*)pReWnd->GetModuleMainWnd()->SendMessage(WM_USER, MAKEWPARAM(nameDLL, 0), (LPARAM)szBufname);

		if(strItemName == "")
			continue;
		
		strItemName.TrimLeft();
		strItemName.TrimRight();
		
		m_saTargetItemCode.Add(strItemCode);
		m_saTargetItemName.Add(strItemName);
	}

	CString strMsg;
	strMsg.Format("%s%d개", "조회개수 : ", m_saTargetItemCode.GetSize());
	m_stcTotCntTargetItem.SetWindowText(strMsg);
	
	OnBtnExcept(0);
}

void CDlgEditSubject::AllClear()
{
	m_stcTarget.SetWindowText("");
	m_stcTotCntTargetItem.SetWindowText("");
	m_gridResult->ClearAll();
}

void CDlgEditSubject::GetMonthSettlement(CStringArray* psaItemCode)
{
	int nMonth;

	if(m_btnRadioAll.GetCheck())
	{
		
	}
	else if(m_btnRadioMarch.GetCheck())
	{
		nMonth = 3;
	}
	else if(m_btnRadioJune.GetCheck())
	{
		nMonth = 6;
	}
	else if(m_btnRadioSeptember.GetCheck())
	{
		nMonth = 9;
	}
	else if(m_btnRadioDecember.GetCheck())
	{
		nMonth = 12;
	}
	else if(m_btnRadioEstival.GetCheck())
	{
		nMonth = m_comboMonths.GetCurSel() + 1;
	}
	
	
	CConditionResultWnd* pReWnd = (CConditionResultWnd*)m_pReWnd;
	pReWnd->GetMonthSettlement(psaItemCode, nMonth);
}

void CDlgEditSubject::OnChangeCombo()
{
	OnBtnExcept(0);
}