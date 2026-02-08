// MarketDlg.cpp : implementation file
//

#include "stdafx.h"

#include "ib771100.h"
#include "MarketDlg.h"
#include "MapDlg.h"
#include "MMSystem.h"

#include "../../control/fx_misc/misctype.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

const char* szFilterMapName = _T("IB771103");
#define IDC_HTMLVIEW		1001

/////////////////////////////////////////////////////////////////////////////
// CMarketDlg dialog
CMarketDlg::CMarketDlg(CWnd* pParent) : CDialog(CMarketDlg::IDD, pParent)
{
	m_pAxWnd = pParent;
	m_pFont  = GetAxFont(8,false,"굴림체");;
	m_pParentWnd = pParent;
	
	m_szRootDir = GetAxVariant(homeCC);
	m_szUser = GetAxVariant(nameCC);
	
	m_bMoveKeyCtrl		= FALSE;
	m_bMoveFilterResult = FALSE;
	m_bMoveFilter		= FALSE;
	m_bMoveMarket		= FALSE;
	
	m_bFilter	  = FALSE;
	m_sFilterStr1 = "";
	m_sFilterStr2 = "";
	m_sFilterStr3 = "";
	m_sFilterStr4 = "";
	m_sFilterStr5 = "";
	m_sFilterStr6 = "";
	m_sFilterStr7 = "";
	m_sFilterStr8 = "";
	m_sFilterStr9 = "";
	m_sFilterStr10 = "";
	m_sFilterStr11 = "";
	m_sFilterStr12 = "";
	m_sFilterStr13 = "";
	m_sFilterStr14 = "";
	m_sFilterStr15 = "";
	m_sFilterStr16 = "";
	m_sFilterStr17 = "";
	m_sFilterStr18 = "";
	m_sFilterStr19 = "";
	m_sFilterStr20 = "";
	
	m_nSelIndex = -1;	
}

CMarketDlg::CMarketDlg(CString strRoot, CString strUser, CWnd* pWizard, CFont* pFont, CWnd* pParent/*=NULL*/)
	: CDialog(CMarketDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CMarketDlg)
	//}}AFX_DATA_INIT
	m_pAxWnd = pWizard;
	m_pFont  = pFont;
	m_pParentWnd = pParent;

	m_szRootDir = strRoot;
	m_szUser = strUser;

	m_bMoveKeyCtrl		= FALSE;
	m_bMoveFilterResult = FALSE;
	m_bMoveFilter		= FALSE;
	m_bMoveMarket		= FALSE;

	m_bFilter	  = FALSE;
	m_sFilterStr1 = "";
	m_sFilterStr2 = "";
	m_sFilterStr3 = "";
	m_sFilterStr4 = "";
	m_sFilterStr5 = "";
	m_sFilterStr6 = "";
	m_sFilterStr7 = "";
	m_sFilterStr8 = "";
	m_sFilterStr9 = "";
	m_sFilterStr10 = "";
	m_sFilterStr11 = "";
	m_sFilterStr12 = "";
	m_sFilterStr13 = "";
	m_sFilterStr14 = "";
	m_sFilterStr15 = "";
	m_sFilterStr16 = "";
	m_sFilterStr17 = "";
	m_sFilterStr18 = "";
	m_sFilterStr19 = "";
	m_sFilterStr20 = "";

	m_nSelIndex = -1;
}

void CMarketDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMarketDlg)
	DDX_Control(pDX, IDC_FILTER_EDIT20, m_FilterEdit20);
	DDX_Control(pDX, IDC_FILTER_EDIT19, m_FilterEdit19);
	DDX_Control(pDX, IDC_FILTER_EDIT18, m_FilterEdit18);
	DDX_Control(pDX, IDC_FILTER_EDIT17, m_FilterEdit17);
	DDX_Control(pDX, IDC_FILTER_EDIT16, m_FilterEdit16);
	DDX_Control(pDX, IDC_FILTER_EDIT15, m_FilterEdit15);
	DDX_Control(pDX, IDC_FILTER_EDIT14, m_FilterEdit14);
	DDX_Control(pDX, IDC_FILTER_EDIT13, m_FilterEdit13);
	DDX_Control(pDX, IDC_FILTER_EDIT12, m_FilterEdit12);
	DDX_Control(pDX, IDC_FILTER_EDIT11, m_FilterEdit11);
	DDX_Control(pDX, IDC_FILTER_EDIT10, m_FilterEdit10);
	DDX_Control(pDX, IDC_FILTER_CHECK20, m_FilterCheck20);
	DDX_Control(pDX, IDC_FILTER_CHECK19, m_FilterCheck19);
	DDX_Control(pDX, IDC_FILTER_CHECK18, m_FilterCheck18);
	DDX_Control(pDX, IDC_FILTER_CHECK17, m_FilterCheck17);
	DDX_Control(pDX, IDC_FILTER_CHECK16, m_FilterCheck16);
	DDX_Control(pDX, IDC_FILTER_CHECK15, m_FilterCheck15);
	DDX_Control(pDX, IDC_FILTER_CHECK14, m_FilterCheck14);
	DDX_Control(pDX, IDC_FILTER_CHECK13, m_FilterCheck13);
	DDX_Control(pDX, IDC_FILTER_CHECK12, m_FilterCheck12);
	DDX_Control(pDX, IDC_FILTER_CHECK11, m_FilterCheck11);
	DDX_Control(pDX, IDC_FILTER_CHECK10, m_FilterCheck10);
	DDX_Control(pDX, IDC_FILTERLIST_EXCEL, m_Filterlist_Excel);
	DDX_Control(pDX, IDC_BTN_PLAY, m_btnPlay);
	DDX_Control(pDX, IDC_BTN_PATH, m_btnPath);
	DDX_Control(pDX, IDC_SOUND_FILE, m_editSndFile);
	DDX_Control(pDX, IDC_MARKETCANCEL_BTN, m_MarketCancelBtn);
	DDX_Control(pDX, IDC_MARKET_TREE, m_MarketTreeCtrl);
	DDX_Control(pDX, IDC_MARKET_BTN, m_MarketBtn);
	DDX_Control(pDX, IDC_FILTERRESULT_LIST, m_FilterResultListCtrl);
	DDX_Control(pDX, IDC_FILTER_USECHECK, m_FilterUseCheckBtn);
	DDX_Control(pDX, IDC_FILTER_SOUNDCHECK, m_FilterSoundCkeckBbtn);
	DDX_Control(pDX, IDC_FILTER_SAVEBTN, m_FilterSaveEdit);
	DDX_Control(pDX, IDC_FILTER_EDIT9, m_FilterEdit9);
	DDX_Control(pDX, IDC_FILTER_EDIT8, m_FilterEdit8);
	DDX_Control(pDX, IDC_FILTER_EDIT7, m_FilterEdit7);
	DDX_Control(pDX, IDC_FILTER_EDIT6, m_FilterEdit6);
	DDX_Control(pDX, IDC_FILTER_EDIT5, m_FilterEdit5);
	DDX_Control(pDX, IDC_FILTER_EDIT4, m_FilterEdit4);
	DDX_Control(pDX, IDC_FILTER_EDIT3, m_FilterEdit3);
	DDX_Control(pDX, IDC_FILTER_EDIT2, m_FilterEdit2);
	DDX_Control(pDX, IDC_FILTER_EDIT1, m_FilterEdit1);
	DDX_Control(pDX, IDC_FILTER_DELBTN, m_FilterDelBtn);
	DDX_Control(pDX, IDC_FILTER_CHECK9, m_FilterCheck9);
	DDX_Control(pDX, IDC_FILTER_CHECK8, m_FilterCheck8);
	DDX_Control(pDX, IDC_FILTER_CHECK7, m_FilterCheck7);
	DDX_Control(pDX, IDC_FILTER_CHECK6, m_FilterCheck6);
	DDX_Control(pDX, IDC_FILTER_CHECK5, m_FilterCheck5);
	DDX_Control(pDX, IDC_FILTER_CHECK4, m_FilterCheck4);
	DDX_Control(pDX, IDC_FILTER_CHECK3, m_FilterCheck3);
	DDX_Control(pDX, IDC_FILTER_CHECK2, m_FilterCheck2);
	DDX_Control(pDX, IDC_FILTER_CHECK1, m_FilterCheck1);
	DDX_Control(pDX, IDC_FILTER_STATIC, m_FilterStatic);
	DDX_Control(pDX, IDC_KEY_WORDSTATIC, m_KeyWordStatic);
	DDX_Control(pDX, IDC_KEY_STEPLIST, m_KeyStepListBox);
	DDX_Control(pDX, IDC_KEY_STEPCOMBO, m_KeyStepComboBox);
	DDX_Control(pDX, IDC_KEY_OKBUTTON, m_KeyOKBtn);
	DDX_Control(pDX, IDC_KEY_LIST, m_KeyListBox);
	DDX_Control(pDX, IDC_KEY_LAYERSTATIC, m_KeyLayerStatic);
	DDX_Control(pDX, IDC_KEY_EDIT, m_KeyEdit);
	DDX_Control(pDX, IDC_KEY_COLORSTATIC, m_KeyColorStatic);
	DDX_Control(pDX, IDC_KEY_BUTTON_COLOR, m_KeyBtnColor);
	DDX_Control(pDX, IDC_KEY_BUTTON_CANCEL, m_KeyBtnCancel);
	DDX_Control(pDX, IDC_KEY_BUTTON_EDIT, m_KeyBtnEdit);
	DDX_Control(pDX, IDC_KEY_BUTTON_DEL, m_KeyBtnDel);
	DDX_Control(pDX, IDC_KEY_BUTTON_ADD, m_KeyBtnAdd);
	DDX_Control(pDX, IDC_KEY_ADDCOMBO, m_KeyAddComboBox);
	DDX_Control(pDX, IDC_KEY_GROUP2, m_KeyGroup2);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CMarketDlg, CDialog)
	//{{AFX_MSG_MAP(CMarketDlg)
	ON_LBN_SELCHANGE(IDC_KEY_LIST, OnSelchangeKeyList)
	ON_LBN_SELCHANGE(IDC_KEY_STEPLIST, OnSelchangeKeyStepList)
	ON_BN_CLICKED(IDC_KEY_BUTTON_ADD, OnKeyButtonAdd)
	ON_BN_CLICKED(IDC_KEY_BUTTON_DEL, OnKeyButtonDel)
	ON_BN_CLICKED(IDC_KEY_BUTTON_EDIT, OnKeyButtonEdit)
	ON_CBN_SELCHANGE(IDC_KEY_STEPCOMBO, OnSelchangeKeyStepCombo)
	ON_BN_CLICKED(IDC_KEY_OKBUTTON, OnKeyOkbutton)
	ON_BN_CLICKED(IDC_KEY_BUTTON_CANCEL, OnKeyButtonCancel)
	ON_BN_CLICKED(IDC_FILTER_CHECK1, OnFilterCheck1)
	ON_BN_CLICKED(IDC_FILTER_CHECK2, OnFilterCheck2)
	ON_BN_CLICKED(IDC_FILTER_CHECK3, OnFilterCheck3)
	ON_BN_CLICKED(IDC_FILTER_CHECK4, OnFilterCheck4)
	ON_BN_CLICKED(IDC_FILTER_CHECK5, OnFilterCheck5)
	ON_BN_CLICKED(IDC_FILTER_CHECK6, OnFilterCheck6)
	ON_BN_CLICKED(IDC_FILTER_CHECK7, OnFilterCheck7)
	ON_BN_CLICKED(IDC_FILTER_CHECK8, OnFilterCheck8)
	ON_BN_CLICKED(IDC_FILTER_CHECK9, OnFilterCheck9)
	ON_BN_CLICKED(IDC_FILTER_USECHECK, OnFilterUsecheck)
	ON_BN_CLICKED(IDC_FILTER_DELBTN, OnFilterDelbtn)
	ON_BN_CLICKED(IDC_FILTER_SAVEBTN, OnFilterSavebtn)
	ON_BN_CLICKED(IDC_MARKET_BTN, OnMarketBtn)
	ON_NOTIFY(NM_CLICK, IDC_FILTERRESULT_LIST, OnClickFilterResultList)
	ON_WM_PAINT()
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_MARKETCANCEL_BTN, OnMarketcancelBtn)
	ON_BN_CLICKED(IDC_FILTER_SOUNDCHECK, OnFilterSoundcheck)
	ON_BN_CLICKED(IDC_BTN_PLAY, OnBtnPlay)
	ON_BN_CLICKED(IDC_BTN_PATH, OnBtnPath)
	ON_CBN_EDITCHANGE(IDC_KEY_ADDCOMBO, OnEditchangeKeyAddcombo)
	ON_BN_CLICKED(IDC_FILTERLIST_EXCEL, OnFilterlistExcel)
	ON_BN_CLICKED(IDC_FILTER_CHECK10, OnFilterCheck10)
	ON_BN_CLICKED(IDC_FILTER_CHECK11, OnFilterCheck11)
	ON_BN_CLICKED(IDC_FILTER_CHECK12, OnFilterCheck12)
	ON_BN_CLICKED(IDC_FILTER_CHECK13, OnFilterCheck13)
	ON_BN_CLICKED(IDC_FILTER_CHECK14, OnFilterCheck14)
	ON_BN_CLICKED(IDC_FILTER_CHECK15, OnFilterCheck15)
	ON_BN_CLICKED(IDC_FILTER_CHECK16, OnFilterCheck16)
	ON_BN_CLICKED(IDC_FILTER_CHECK17, OnFilterCheck17)
	ON_BN_CLICKED(IDC_FILTER_CHECK18, OnFilterCheck18)
	ON_BN_CLICKED(IDC_FILTER_CHECK19, OnFilterCheck19)
	ON_BN_CLICKED(IDC_FILTER_CHECK20, OnFilterCheck20)
	//}}AFX_MSG_MAP
	ON_NOTIFY(TCN_SELCHANGE, IDC_MARKET_TAB, OnChangeTab)
	ON_MESSAGE(CPN_SELENDOK, OnChangeColorPicker)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMarketDlg message handlers
BOOL CMarketDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here

	m_clrBk		 = GetAxColor(64);
	m_clrWriteBk = GetAxColor(181);
	m_clrRoundBk = GetAxColor(66);

	m_hBk = CreateSolidBrush(m_clrBk);
	m_hRoundBk = CreateSolidBrush(m_clrRoundBk);
	m_br.CreateSolidBrush(m_clrRoundBk);
	m_strPal = GetPalette();

	if(InitWindows())
	{
		SetFilterSetData();   // 화면이 열릴 때 필터링결과 반영
		SetFilterResultSetData();
		OnChangeTab(0,0);
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CMarketDlg::InitWindows() 
{
	MoveWindow(CRect(0,0,385,360), TRUE);
	const int nGap = 3;

	m_pTab = std::make_unique<CfxTab>();
	m_pTab->Create(CRect(10,10,370,36), this, IDC_MARKET_TAB);
	m_pTab->SetColor(GetIndexColor(CLR_TAB_FORE), GetIndexColor(CLR_FORM_BACK), 
		GetIndexColor(CLR_TAB_SELFORE), GetIndexColor(CLR_FORM_BACK), GetIndexColor(CLR_FORM_BACK));
	m_pTab->SetFont(m_pFont);
	
	m_pTab->LoadTabImage(m_szRootDir, FALSE);
	m_pTab->SetColor(RGB(255, 255, 255), NULL, RGB(0, 0, 0), NULL, RGB(255, 255, 255));
	m_pTab->InsertItem(0, "키워드 설정");

	m_pTab->LoadTabImage(m_szRootDir, FALSE);
	m_pTab->SetColor(RGB(255, 255, 255), NULL, RGB(0, 0, 0), NULL, RGB(255, 255, 255));
	m_pTab->InsertItem(1, "필터링결과");

	m_pTab->LoadTabImage(m_szRootDir, FALSE);
	m_pTab->SetColor(RGB(255, 255, 255), NULL, RGB(0, 0, 0), NULL, RGB(255, 255, 255));
	m_pTab->InsertItem(2, "필터링설정");

	m_pTab->LoadTabImage(m_szRootDir, FALSE);
	m_pTab->SetColor(RGB(255, 255, 255), NULL, RGB(0, 0, 0), NULL, RGB(255, 255, 255));
	m_pTab->InsertItem(3, "제공처설정");

	m_pTab->SetStyle(tsIMAGE, TRUE);

	m_KeyListBox.ModifyStyle(0, WS_BORDER, 0);
	m_KeyStepListBox.ModifyStyle(0, WS_BORDER, 0);
	m_FilterResultListCtrl.ModifyStyle(0, WS_BORDER, 0);
	m_MarketTreeCtrl.ModifyStyle(0, WS_BORDER, 0);
//	m_MarketTreeCtrl.ExpandAll();
//	m_MarketTreeCtrl.CollapseAll();

	m_KeyBtnAdd.SetImgBitmap(getBitmap("1BTN.bmp"), getBitmap("1BTN_DN.bmp"), getBitmap("1BTN_EN.bmp"));
	m_KeyBtnAdd.SetFont(m_pFont, TRUE);

	m_btnPlay.SetImgBitmap(getBitmap("2BTN.bmp"), getBitmap("2BTN_DN.bmp"), getBitmap("2BTN_EN.bmp"));
	m_btnPlay.SetFont(m_pFont, TRUE);

	m_btnPath.SetImgBitmap(getBitmap("2BTN.bmp"), getBitmap("2BTN_DN.bmp"), getBitmap("2BTN_EN.bmp"));
	m_btnPath.SetFont(m_pFont, TRUE);

	m_KeyBtnDel.SetImgBitmap(getBitmap("1BTN.bmp"), getBitmap("1BTN_DN.bmp"), getBitmap("1BTN_EN.bmp"));
	m_KeyBtnDel.SetFont(m_pFont, TRUE);
	m_KeyBtnEdit.SetImgBitmap(getBitmap("3BTN.bmp"), getBitmap("3BTN_DN.bmp"), getBitmap("3BTN_EN.bmp"));
	m_KeyBtnEdit.SetFont(m_pFont, TRUE);
	m_KeyOKBtn.SetImgBitmap(getBitmap("4BTN.bmp"), getBitmap("4BTN_DN.bmp"), getBitmap("4BTN_EN.bmp"));
	m_KeyOKBtn.SetFont(m_pFont, TRUE);
	m_KeyBtnCancel.SetImgBitmap(getBitmap("4BTN.bmp"), getBitmap("4BTN_DN.bmp"), getBitmap("4BTN_EN.bmp"));
	m_KeyBtnCancel.SetFont(m_pFont, TRUE);
	m_KeyBtnCancel.SetImgBitmap(getBitmap("4BTN.bmp"), getBitmap("4BTN_DN.bmp"), getBitmap("4BTN_EN.bmp"));
	m_KeyBtnCancel.SetFont(m_pFont, TRUE);
	m_FilterDelBtn.SetImgBitmap(getBitmap("4BTN.bmp"), getBitmap("4BTN_DN.bmp"), getBitmap("4BTN_EN.bmp"));
	m_FilterDelBtn.SetFont(m_pFont, TRUE);
	m_FilterSaveEdit.SetImgBitmap(getBitmap("4BTN.bmp"), getBitmap("4BTN_DN.bmp"), getBitmap("4BTN_EN.bmp"));
	m_FilterSaveEdit.SetFont(m_pFont, TRUE);
	m_MarketBtn.SetImgBitmap(getBitmap("4BTN.bmp"), getBitmap("4BTN_DN.bmp"), getBitmap("4BTN_EN.bmp"));
	m_MarketBtn.SetFont(m_pFont, TRUE);
	m_MarketCancelBtn.SetImgBitmap(getBitmap("4BTN.bmp"), getBitmap("4BTN_DN.bmp"), getBitmap("4BTN_EN.bmp"));
	m_MarketCancelBtn.SetFont(m_pFont, TRUE);

	m_Filterlist_Excel.SetImgBitmap(getBitmap("TOOL_엑셀.bmp"), getBitmap("TOOL_엑셀_DN.bmp"), getBitmap("TOOL_엑셀.bmp"));
	return TRUE;
}

HBITMAP CMarketDlg::getBitmap(CString bmps)
{
	CString	path;
	
	path.Format("%s\\%s\\%s", m_szRootDir, IMAGEDIR, bmps);
	return ((CBitmap *)m_pAxWnd->SendMessage(WM_USER, MAKEWPARAM(getBITMAP, 0), (LPARAM)(LPCTSTR)path))->operator HBITMAP();
}

void CMarketDlg::ChangeColors(COLORREF clrBKColor, COLORREF clrRoundColor)
{
	m_strPal	 = GetPalette();
	m_clrBk		 = clrBKColor;
	m_clrRoundBk = clrRoundColor;
	m_hBk = CreateSolidBrush(m_clrBk);
	m_hRoundBk = CreateSolidBrush(m_clrRoundBk);

	m_pTab->LoadTabImage(m_szRootDir, FALSE);
	m_pTab->SetColor(GetIndexColor(CLR_TAB_FORE), GetIndexColor(CLR_FORM_BACK), 
		GetIndexColor(CLR_TAB_SELFORE), GetIndexColor(CLR_FORM_BACK), clrRoundColor);
	Invalidate();
}

COLORREF CMarketDlg::GetIndexColor(int index)
{
	if (index & 0x02000000)
		return index;
	return m_pAxWnd->SendMessage(WM_USER, MAKEWORD(getPALETTE, 0), (LPARAM)index);
}

//LRESULT CMarketDlg::OnChangeTab(WPARAM wParam, LPARAM lParam)
void CMarketDlg::OnChangeTab(NMHDR * pHeader, LRESULT * pResult)
{
	m_nSelIndex = m_pTab->GetCurSel();
	WndAllCtrlShow(FALSE);

	Invalidate();

	switch (m_nSelIndex)
	{
	case cKEYWORD_SET:
		CreateKeyWordSetWnd();
		SetKeyWordData();
		break;
	case cFILLER_RESULT:
		CreateFilterResultWnd();
		break;
	case cFILTER_SET:
		CreateFilterSetWnd();
		SetFilterSetData();  // 다른 화면에서 설정된 결과 적용
		break;
	case cMARKET_SET:
		CreateMarketSetWnd();
		break;
	}

	//return FALSE;
}

void CMarketDlg::WndAllCtrlShow(BOOL bShow)
{
	WndAllCtrlShow_Key(bShow);
	WndAllCtrlShow_FilterResult(bShow);
	WndAllCtrlShow_Filter(bShow);
	WndAllCtrlShow_Market(bShow);
}	

void CMarketDlg::WndAllCtrlShow_Key(BOOL bShow)
{
	m_KeyWordStatic.ShowWindow(bShow);
	m_KeyStepListBox.ShowWindow(bShow);
	m_KeyStepComboBox.ShowWindow(bShow);
	m_KeyOKBtn.ShowWindow(bShow);
	m_KeyListBox.ShowWindow(bShow);
	m_KeyLayerStatic.ShowWindow(bShow);
	if (m_KeyGroup1)
		m_KeyGroup1.ShowWindow(bShow);
	m_KeyGroup2.ShowWindow(bShow);
	m_KeyEdit.ShowWindow(bShow);
	m_KeyColorStatic.ShowWindow(bShow);
	m_KeyBtnColor.ShowWindow(bShow);
	m_KeyBtnCancel.ShowWindow(bShow);
	m_KeyBtnEdit.ShowWindow(bShow);
	m_KeyBtnDel.ShowWindow(bShow);
	m_KeyBtnAdd.ShowWindow(bShow);
	if (m_KeyBayStatic) m_KeyBayStatic.ShowWindow(bShow);
	m_KeyAddComboBox.ShowWindow(bShow);
}

void CMarketDlg::WndAllCtrlShow_FilterResult(BOOL bShow)
{
	m_FilterResultListCtrl.ShowWindow(bShow);
	m_Filterlist_Excel.ShowWindow(bShow);
}

CBitmap* CMarketDlg::getAxBitmap(CString path)
{
	return (CBitmap*)m_pAxWnd->SendMessage(WM_USER, getBITMAP, (long)path.operator LPCTSTR());
}

void CMarketDlg::WndAllCtrlShow_Filter(BOOL bShow)
{
	m_FilterUseCheckBtn.ShowWindow(bShow);
	m_FilterSoundCkeckBbtn.ShowWindow(bShow);
	m_FilterSaveEdit.ShowWindow(bShow);
	m_FilterStatic.ShowWindow(bShow);
	m_FilterEdit20.ShowWindow(bShow);
	m_FilterEdit19.ShowWindow(bShow);
	m_FilterEdit18.ShowWindow(bShow);
	m_FilterEdit17.ShowWindow(bShow);
	m_FilterEdit16.ShowWindow(bShow);
	m_FilterEdit15.ShowWindow(bShow);
	m_FilterEdit14.ShowWindow(bShow);
	m_FilterEdit13.ShowWindow(bShow);
	m_FilterEdit12.ShowWindow(bShow);
	m_FilterEdit11.ShowWindow(bShow);
	m_FilterEdit10.ShowWindow(bShow);
	m_FilterEdit9.ShowWindow(bShow);
	m_FilterEdit8.ShowWindow(bShow);
	m_FilterEdit7.ShowWindow(bShow);
	m_FilterEdit6.ShowWindow(bShow);
	m_FilterEdit5.ShowWindow(bShow);
	m_FilterEdit4.ShowWindow(bShow);
	m_FilterEdit3.ShowWindow(bShow);
	m_FilterEdit2.ShowWindow(bShow);
	m_FilterEdit1.ShowWindow(bShow);
	m_FilterDelBtn.ShowWindow(bShow);
	m_FilterCheck20.ShowWindow(bShow);
	m_FilterCheck19.ShowWindow(bShow);
	m_FilterCheck18.ShowWindow(bShow);
	m_FilterCheck17.ShowWindow(bShow);
	m_FilterCheck16.ShowWindow(bShow);
	m_FilterCheck15.ShowWindow(bShow);
	m_FilterCheck14.ShowWindow(bShow);
	m_FilterCheck13.ShowWindow(bShow);
	m_FilterCheck12.ShowWindow(bShow);
	m_FilterCheck11.ShowWindow(bShow);
	m_FilterCheck10.ShowWindow(bShow);
	m_FilterCheck9.ShowWindow(bShow);
	m_FilterCheck8.ShowWindow(bShow);
	m_FilterCheck7.ShowWindow(bShow);
	m_FilterCheck6.ShowWindow(bShow);
	m_FilterCheck5.ShowWindow(bShow);
	m_FilterCheck4.ShowWindow(bShow);
	m_FilterCheck3.ShowWindow(bShow);
	m_FilterCheck2.ShowWindow(bShow);
	m_FilterCheck1.ShowWindow(bShow);
	m_editSndFile.ShowWindow(bShow);
	m_btnPath.ShowWindow( bShow );
	m_btnPlay.ShowWindow( bShow );
}

void CMarketDlg::WndAllCtrlShow_Market(BOOL bShow)
{
	m_MarketTreeCtrl.ShowWindow(bShow);
	m_MarketBtn.ShowWindow(bShow);
	m_MarketCancelBtn.ShowWindow(bShow);
}

////////////////////////// 
// 함수명	    : CMarketDlg::MoveCtrl
// 설 명	    : 다이얼로그에 컨트롤을 움직이는 기능
// 작성일시	    : 2009/06/02
// 작성자       : kbkim
// 파라미터	    : UINT nCtrlID : 움직이고자 하는  컨트롤 ID
//				  int nCx : 움직이고자 하는 X 크기(음수:왼쪽으로)
//				  int nCy : 움직이고자 하는 Y 크기(음수:위쪽으로)
// 리턴 값 		: 
// 관련요건	    : 
////////////////////////// 
void CMarketDlg::MoveCtrl(UINT nCtrlID, int nCx, int nCy)
{
	CRect rCtrl;
	GetDlgItem(nCtrlID)->GetWindowRect(rCtrl);
	
	rCtrl.top = rCtrl.top + nCy;
	rCtrl.bottom = rCtrl.bottom + nCy;
	rCtrl.left = rCtrl.left + nCx;
	rCtrl.right = rCtrl.right + nCx;

	ScreenToClient(&rCtrl);

	GetDlgItem(nCtrlID)->SetWindowPos(NULL, rCtrl.left, rCtrl.top, 0, 0, SWP_NOSIZE);
	GetDlgItem(nCtrlID)->ShowWindow(TRUE);
}

////////// 키워드 설정 관련 함수/////////////////////////////
void CMarketDlg::CreateKeyWordSetWnd()
{
	if(!m_bMoveKeyCtrl)
	{
		MoveCtrl(IDC_KEY_GROUP2,		CTRLMOVEX, CTRLMOVEY); 
		MoveCtrl(IDC_KEY_STEPLIST,		CTRLMOVEX, CTRLMOVEY);     
		MoveCtrl(IDC_KEY_STEPCOMBO,		CTRLMOVEX, CTRLMOVEY);   
		MoveCtrl(IDC_KEY_OKBUTTON,		CTRLMOVEX, CTRLMOVEY);   
		MoveCtrl(IDC_KEY_LIST,			CTRLMOVEX, CTRLMOVEY);   
		MoveCtrl(IDC_KEY_LAYERSTATIC,	CTRLMOVEX, CTRLMOVEY);   
		MoveCtrl(IDC_KEY_WORDSTATIC,	CTRLMOVEX, CTRLMOVEY);   
		MoveCtrl(IDC_KEY_EDIT,			CTRLMOVEX, CTRLMOVEY);   
		MoveCtrl(IDC_KEY_COLORSTATIC,	CTRLMOVEX, CTRLMOVEY);   
		MoveCtrl(IDC_KEY_BUTTON_COLOR,	CTRLMOVEX, CTRLMOVEY);   
		MoveCtrl(IDC_KEY_BUTTON_CANCEL, CTRLMOVEX, CTRLMOVEY);   
		MoveCtrl(IDC_KEY_BUTTON_EDIT,	CTRLMOVEX, CTRLMOVEY);   
		MoveCtrl(IDC_KEY_BUTTON_DEL,	CTRLMOVEX, CTRLMOVEY);   
		MoveCtrl(IDC_KEY_BUTTON_ADD,	CTRLMOVEX, CTRLMOVEY);   
		MoveCtrl(IDC_KEY_ADDCOMBO,		CTRLMOVEX, CTRLMOVEY);   		
		GetDlgItem(IDC_KEY_GROUP2)->ShowWindow(FALSE);
	}
	else
	{
		WndAllCtrlShow_Key(TRUE);
		GetDlgItem(IDC_KEY_GROUP2)->ShowWindow(FALSE);
	}
	
	m_bMoveKeyCtrl = TRUE;
}

void CMarketDlg::SetKeyWordData()
{
	m_KeyStepListBox.ResetContent();
	m_KeyListBox.ResetContent();
	m_KeyAddComboBox.ResetContent();
	m_KeyStepComboBox.ResetContent();

	for(int i=0; i<m_arKeyList.GetSize();i++) 
	{
		if (m_arKeyList.GetAt(i).iVal != 0)
		{
			m_KeyStepListBox.InsertString(i,  CStr(m_arKeyList.GetAt(i).iVal) + "단계");  
			m_KeyListBox.InsertString(i,  m_arKeyList.GetAt(i).sTxt );
		}
	}

	// TODO: Add extra initialization here
	CString stmp;
	for(int i=0; i<m_arStepColor.GetSize();i++) 
	{
		stmp =  CStr(i+1) +"단계";
		m_KeyAddComboBox.InsertString(i, stmp); 
		m_KeyStepComboBox.InsertString(i, stmp); 
	}
	
	if(m_arStepColor.GetSize() > 0)
	{
		m_KeyStepComboBox.SetCurSel(0); 
		m_KeyBtnColor.SetColor(m_arStepColor[0]); 
	}
}

void CMarketDlg::OnSelchangeKeyList() 
{
	// TODO: Add your control notification handler code here
	CString strKey;
	int idx = m_KeyListBox.GetCurSel(); 
	if(idx < 0) 
		return;
	
	m_KeyListBox.GetText(idx, strKey);
	GetDlgItem(IDC_KEY_EDIT)->SetWindowText(strKey);
	
	m_KeyStepListBox.GetText(idx,	strKey);
	idx = atoi(strKey.Left(1) )-1;
	m_KeyAddComboBox.SetCurSel(idx); 
}

void CMarketDlg::OnSelchangeKeyStepList() 
{
	// TODO: Add your control notification handler code here
	CString strKey;
	int idx = m_KeyStepListBox.GetCurSel(); 
	if(idx < 0) return;
	m_KeyListBox.GetText(idx,	strKey);
	GetDlgItem(IDC_KEY_EDIT)->SetWindowText(strKey);
	
	m_KeyStepListBox.GetText(idx,	strKey);
	idx = atoi(strKey.Left(1) )-1;
	m_KeyAddComboBox.SetCurSel(idx); 
}

void CMarketDlg::OnKeyButtonAdd() 
{
	// TODO: Add your control notification handler code here
	CString strKey;
	const int iadd = m_KeyListBox.GetCount();
	const int istep = m_KeyAddComboBox.GetCurSel()+1;
	for(int i=0; i<m_arKeyList.GetSize() ; i++)
	{
		if(m_arKeyList.GetAt(i).iVal ==  istep)
		{
			OnButtonEdit();
			return;
		}
	}
	
	CString strStep = CStr(istep ) + "단계";
	GetDlgItem(IDC_KEY_EDIT)->GetWindowText(strKey);

	m_KeyListBox.InsertString(iadd, strKey );
	m_KeyStepListBox.InsertString(iadd, strStep); 
	m_arKeyList.InsertAt(iadd, AList(strKey, istep));
}

void CMarketDlg::OnButtonEdit() 
{
	CString strKey;
	int iedit = m_KeyListBox.GetCurSel() ;
	if(iedit < 0) 
	{
		iedit = m_KeyStepListBox.GetCurSel() ;
		if(iedit < 0) 
			return;
	}

	m_KeyListBox.DeleteString(iedit); 
	m_KeyStepListBox.DeleteString(iedit);

	const int istep = m_KeyAddComboBox.GetCurSel()+1;
	GetDlgItem(IDC_KEY_EDIT)->GetWindowText(strKey);

	m_KeyListBox.InsertString(iedit, strKey );
	m_KeyStepListBox.InsertString(iedit, CStr(istep ) + "단계"); 
	m_arKeyList.SetAt(iedit, AList(strKey, istep));
}

void CMarketDlg::OnKeyButtonDel() 
{
	// TODO: Add your control notification handler code here
	int idel = m_KeyListBox.GetCurSel() ;
	if(idel < 0) 
	{
		idel = m_KeyStepListBox.GetCurSel() ;
		if(idel < 0) 
			return;
	}
	
	m_KeyListBox.DeleteString(idel); 
	m_KeyStepListBox.DeleteString(idel);
	
	m_arKeyList.RemoveAt(idel);	
}

void CMarketDlg::OnKeyButtonEdit() 
{
	// TODO: Add your control notification handler code here
	CString strKey;
	int iedit = m_KeyListBox.GetCurSel() ;
	if(iedit < 0) 
	{
		iedit = m_KeyStepListBox.GetCurSel() ;
		if(iedit < 0) return;
	}
	m_KeyListBox.DeleteString(iedit); 
	m_KeyStepListBox.DeleteString(iedit);
	
	const int istep = m_KeyAddComboBox.GetCurSel()+1;
	GetDlgItem(IDC_KEY_EDIT)->GetWindowText(strKey);

	m_KeyListBox.InsertString(iedit, strKey );
	m_KeyStepListBox.InsertString(iedit, CStr(istep ) + "단계"); 
	m_arKeyList.SetAt(iedit, AList(strKey, istep));
}

void CMarketDlg::OnSelchangeKeyStepCombo() 
{
	// TODO: Add your control notification handler code here
	const int idx = m_KeyStepComboBox.GetCurSel();
	if(idx > -1 && m_arStepColor.GetSize() > idx) 
		m_KeyBtnColor.SetColor( m_arStepColor.GetAt(idx) ); 
}

LONG CMarketDlg::OnChangeColorPicker(UINT lParam, LONG wParam)
{
	const int iedit = m_KeyStepComboBox.GetCurSel();
	if( iedit < 0) 
		return 0;
	
	const COLORREF SelColor = (DWORD)lParam;
	if(m_arStepColor.GetSize() > iedit) 
		m_arStepColor.SetAt(iedit,  SelColor);
	else 
		m_arStepColor.Add(SelColor);
	
	return 0;
}

CFont* CMarketDlg::GetAxFont(int size ,bool bBold,CString str)
{
	_fontR fontR;
	memset(&fontR,0x00,sizeof(_fontR));
	fontR.name = (char*)(LPCTSTR)str;
	fontR.point =  size;
	fontR.italic = false;
	fontR.bold = bBold?FW_BOLD:FW_NORMAL;
	return (CFont*)m_pAxWnd->SendMessage(WM_USER, MAKEWPARAM(getFONT,0), (long)&fontR);
}

COLORREF CMarketDlg::GetAxColor(int iColor)
{
	if (iColor & 0x02000000)
		return iColor;
	return m_pAxWnd->SendMessage(WM_USER, MAKEWPARAM(getPALETTE, 0), (LPARAM)iColor);
}

////////// 필터링 결과 관련 함수/////////////////////////////
void CMarketDlg::CreateFilterResultWnd()
{
	if(!m_bMoveFilterResult)
	{
		MoveCtrl(IDC_FILTERRESULT_LIST, CTRLMOVE_X, 0);
		MoveCtrl(IDC_FILTERLIST_EXCEL, CTRLMOVE_X, 0);
	}
	else
		WndAllCtrlShow_FilterResult(TRUE);
	
	m_bMoveFilterResult = TRUE;
}

void CMarketDlg::SetFilterResultSetData() 
{
	m_FilterResultListCtrl.SetFont(m_pFont);

	m_FilterResultListCtrl.SetTextColor( GetAxColor(69));

	m_FilterResultListCtrl.InsertColumn(0, _T("시간"), LVCFMT_CENTER,  45, 0);
	m_FilterResultListCtrl.InsertColumn(1, _T("제목"), LVCFMT_LEFT, 300, 1);
	m_FilterResultListCtrl.InsertColumn(2, _T("Key"), LVCFMT_LEFT, 0, 2);
	m_FilterResultListCtrl.InsertColumn(3, _T("종목번호"), LVCFMT_LEFT, 0, 3);
	m_FilterResultListCtrl.InsertColumn(4, _T("종목번호"), LVCFMT_LEFT, 90, 3);
	m_FilterResultListCtrl.InsertColumn(5, _T("종 목 명"), LVCFMT_LEFT, 150, 3);

	m_FilterResultListCtrl.SetExtendedStyle(LVS_EX_FULLROWSELECT);

	m_FilterResultListCtrl.SetFont(GetAxFont(10,false,"굴림체"));
}

////////////////////////// 
// 함수명	    : CMarketDlg::AddFilterResultData
// 설 명	    : Filter 리스트에 하나의 항목을 추가 하는 기능
// 작성일시	    : 2009/06/02
// 작성자       : kbkim
// 파라미터	    : CString strTime  : 시간(XX:XX)
//				  CString strTitle : 제목
//				  CString strKey   : 기사 Key(클릭시 사용하기 위해)
//				  CString strCodeName : 코드 Name(클릭시 사용하기 위해)
// 리턴 값 		: 없음
// 관련요건	    : 
////////////////////////// 
void CMarketDlg::AddFilterResultData(CString strTime, CString strTitle, CString strKey, CString strCodeName, CString hnam) 
{
//	LVITEM * lv = new LVITEM;
	std::shared_ptr<LVITEM> lv = std::make_shared<LVITEM>();


	const int nCount = m_FilterResultListCtrl.GetItemCount();

	lv->iItem = 0;
	lv->iSubItem = 0;
	lv->pszText = (char*)(LPCSTR)strTime;
	lv->mask = LVIF_TEXT;
	m_FilterResultListCtrl.InsertItem(lv.get());

	lv->iSubItem = 1;
	lv->pszText = (char*)(LPCSTR)strTitle;
	lv->mask = LVIF_TEXT;
	m_FilterResultListCtrl.SetItem(lv.get());

	lv->iSubItem = 2;
	lv->pszText = (char*)(LPCSTR)strKey;
	lv->mask = LVIF_TEXT;
	m_FilterResultListCtrl.SetItem(lv.get());

	lv->iSubItem = 3;
	lv->pszText = (char*)(LPCSTR)strCodeName;
	lv->mask = LVIF_TEXT;
	m_FilterResultListCtrl.SetItem(lv.get());

	lv->iSubItem = 4;
	lv->pszText = (char*)(LPCSTR)(strCodeName.Mid(1,6));
	lv->mask = LVIF_TEXT;	
	m_FilterResultListCtrl.SetItem(lv.get());

	lv->iSubItem = 5;
	lv->pszText = (char*)(LPCSTR)hnam;
	lv->mask = LVIF_TEXT;
	m_FilterResultListCtrl.SetItem(lv.get());
}

////////////////////////// 
// 함수명	    : CMarketDlg::MoveCtrl
// 설 명	    : 설정된 뉴스에 대한 정보를 표시 여부를 확인하고 리스트에 나타내 주는 기능
// 작성일시	    : 2009/06/02
// 작성자       : kbkim
// 파라미터	    : news_info* : 뉴스 관련 구조체 포인트
// 리턴 값 		: 
// 관련요건	    : 
////////////////////////// 
bool CMarketDlg::SetMarketNewsData(news_info* pNewsData)
{
	bool result = false;

 	if(m_bFilter && FilterNewsData(pNewsData->titl))
	{
 		if(FilterMarketNewsData(pNewsData->gubn, pNewsData->subg))
		{	
			if( m_FilterSoundCkeckBbtn.GetCheck() == BST_CHECKED )
			{
				char	readB[256] = _T("");
				int readL = 0;

				CString strINI;
				strINI.Format("%s\\User\\%s\\userconf.ini", m_szRootDir, m_szUser);
				readL = GetPrivateProfileString(szFilterMapName, "SOUNDFILE", "0", readB, sizeof(readB), strINI);

				CString strPlay = CString(readB, readL);

				sndPlaySound( strPlay, SND_ASYNC );
			}

//			AddFilterResultData(pNewsData->time, pNewsData->titl, pNewsData->keyv, pNewsData->code, pNewsData->hnam);
			result =  true;
		}
	}

	return result;
}

////////////////////////// 
// 함수명	    : CMarketDlg::FilterNewsData
// 설 명	    : 뉴스에 설정된 필터에 해당되는 것인지를 판단하는 기능
// 작성일시	    : 2009/06/02
// 작성자       : kbkim
// 파라미터	    : CString strNewsTitle : 뉴스 제목
// 리턴 값 		: TRUE : 필터에 해당하는 기사임
// 관련요건	    : 
////////////////////////// 
BOOL CMarketDlg::FilterNewsData(CString strNewsTitle)
{
	//2016.10.06 KSJ 체크되어 있을 때만 단어를 검사한다.
	if( (m_FilterCheck1.GetCheck() && m_sFilterStr1 != "" && strNewsTitle.Find(m_sFilterStr1) > -1)||
		(m_FilterCheck2.GetCheck() && m_sFilterStr2 != "" && strNewsTitle.Find(m_sFilterStr2) > -1)||
		(m_FilterCheck3.GetCheck() && m_sFilterStr3 != "" && strNewsTitle.Find(m_sFilterStr3) > -1)||
		(m_FilterCheck4.GetCheck() && m_sFilterStr4 != "" && strNewsTitle.Find(m_sFilterStr4) > -1)||
		(m_FilterCheck5.GetCheck() && m_sFilterStr5 != "" && strNewsTitle.Find(m_sFilterStr5) > -1)||
		(m_FilterCheck6.GetCheck() && m_sFilterStr6 != "" && strNewsTitle.Find(m_sFilterStr6) > -1)||
		(m_FilterCheck7.GetCheck() && m_sFilterStr7 != "" && strNewsTitle.Find(m_sFilterStr7) > -1)||
		(m_FilterCheck8.GetCheck() && m_sFilterStr8 != "" && strNewsTitle.Find(m_sFilterStr8) > -1)||
		(m_FilterCheck9.GetCheck() && m_sFilterStr9 != "" && strNewsTitle.Find(m_sFilterStr9) > -1)||
		(m_FilterCheck10.GetCheck() && m_sFilterStr10 != "" && strNewsTitle.Find(m_sFilterStr10) > -1)||
		(m_FilterCheck11.GetCheck() && m_sFilterStr11 != "" && strNewsTitle.Find(m_sFilterStr11) > -1)||
		(m_FilterCheck12.GetCheck() && m_sFilterStr12 != "" && strNewsTitle.Find(m_sFilterStr12) > -1)||
		(m_FilterCheck13.GetCheck() && m_sFilterStr13 != "" && strNewsTitle.Find(m_sFilterStr13) > -1)||
		(m_FilterCheck14.GetCheck() && m_sFilterStr14 != "" && strNewsTitle.Find(m_sFilterStr14) > -1)||
		(m_FilterCheck15.GetCheck() && m_sFilterStr15 != "" && strNewsTitle.Find(m_sFilterStr15) > -1)||
		(m_FilterCheck16.GetCheck() && m_sFilterStr16 != "" && strNewsTitle.Find(m_sFilterStr16) > -1)||
		(m_FilterCheck17.GetCheck() && m_sFilterStr17 != "" && strNewsTitle.Find(m_sFilterStr17) > -1)||
		(m_FilterCheck18.GetCheck() && m_sFilterStr18 != "" && strNewsTitle.Find(m_sFilterStr18) > -1)||
		(m_FilterCheck19.GetCheck() && m_sFilterStr19 != "" && strNewsTitle.Find(m_sFilterStr19) > -1)||
	    (m_FilterCheck20.GetCheck() && m_sFilterStr20 != "" && strNewsTitle.Find(m_sFilterStr20) > -1))
		return TRUE;
	return FALSE;
}

////////////////////////// 
// 함수명	    : CMarketDlg::FilterMarketNewsData
// 설 명	    : 설정된 뉴스에 대한 정보를 표시 여부를 확인하고 리스트에 나타내 주는 기능
// 작성일시	    : 2009/06/02
// 작성자       : kbkim
// 파라미터	    : CString sRootName : 제공처 루트 Name(공시, 이투데이, ...)
//				  CString sSubName  : 제공처 서브 Name(거래소공시, 증권, 금융, ...)
// 리턴 값 		: TRUE : 체크되어 있음, FALSE : 미 체크
// 관련요건	    : 000006(조원형:2009/05/12)
////////////////////////// 
BOOL CMarketDlg::FilterMarketNewsData(CString sRootName, CString sSubName)
{
	const int nInitCount = m_arNewsList.GetSize()-4;
	int iSubcnt = 0;
	CString sBulruNameList;

	//전체(0) 을 빼고 공시(1) 부터, 마지막(조회수상위),마지막-1(스크랩) 빼고 
	NewsList NewsList;
	for(int nIndex = 1;nIndex<nInitCount;nIndex++)
	{
		NewsList = m_arNewsList.GetAt(nIndex);
		if(NewsList.sGubnName != sRootName)
			continue;

		iSubcnt = m_arNewsList.GetAt(nIndex).ibulruCnt;
		for(int j=0; j<iSubcnt-1; j++)
		{
			sBulruNameList = NewsList.sBulruNameList[j];
			if(sBulruNameList == sSubName)
			{
				if(NewsList.iCheck[j] == 1)
					return TRUE;
				else
					return FALSE;
			}
		}
	}

	return TRUE;
}

////////// 필터링 설정 관련 함수/////////////////////////////
void CMarketDlg::CreateFilterSetWnd()
{
	if(!m_bMoveFilter)
	{
		MoveCtrl(IDC_FILTER_USECHECK,	0, CTRLMOVE_Y);   
		MoveCtrl(IDC_FILTER_SOUNDCHECK, 0, CTRLMOVE_Y);   
		MoveCtrl(IDC_FILTER_SAVEBTN,	0, CTRLMOVE_Y);   
		MoveCtrl(IDC_FILTER_CHECK1,		0, CTRLMOVE_Y);
		MoveCtrl(IDC_FILTER_CHECK2,		0, CTRLMOVE_Y);
		MoveCtrl(IDC_FILTER_CHECK3,		0, CTRLMOVE_Y);
		MoveCtrl(IDC_FILTER_CHECK4,		0, CTRLMOVE_Y);
		MoveCtrl(IDC_FILTER_CHECK5,		0, CTRLMOVE_Y);
		MoveCtrl(IDC_FILTER_CHECK6,		0, CTRLMOVE_Y);
		MoveCtrl(IDC_FILTER_CHECK7,		0, CTRLMOVE_Y);
		MoveCtrl(IDC_FILTER_CHECK8,		0, CTRLMOVE_Y);
		MoveCtrl(IDC_FILTER_CHECK9,		0, CTRLMOVE_Y);
		MoveCtrl(IDC_FILTER_CHECK10,	0, CTRLMOVE_Y);
		MoveCtrl(IDC_FILTER_CHECK11,	0, CTRLMOVE_Y);
		MoveCtrl(IDC_FILTER_CHECK12,	0, CTRLMOVE_Y);
		MoveCtrl(IDC_FILTER_CHECK13,	0, CTRLMOVE_Y);
		MoveCtrl(IDC_FILTER_CHECK14,	0, CTRLMOVE_Y);
		MoveCtrl(IDC_FILTER_CHECK15,	0, CTRLMOVE_Y);
		MoveCtrl(IDC_FILTER_CHECK16,	0, CTRLMOVE_Y);
		MoveCtrl(IDC_FILTER_CHECK17,	0, CTRLMOVE_Y);
		MoveCtrl(IDC_FILTER_CHECK18,	0, CTRLMOVE_Y);
		MoveCtrl(IDC_FILTER_CHECK19,	0, CTRLMOVE_Y);
		MoveCtrl(IDC_FILTER_CHECK20,	0, CTRLMOVE_Y);
		MoveCtrl(IDC_FILTER_EDIT1,		0, CTRLMOVE_Y);   
		MoveCtrl(IDC_FILTER_EDIT2,		0, CTRLMOVE_Y);   
		MoveCtrl(IDC_FILTER_EDIT3,		0, CTRLMOVE_Y);   
		MoveCtrl(IDC_FILTER_EDIT4,		0, CTRLMOVE_Y);   
		MoveCtrl(IDC_FILTER_EDIT5,		0, CTRLMOVE_Y);   
		MoveCtrl(IDC_FILTER_EDIT6,		0, CTRLMOVE_Y);   
		MoveCtrl(IDC_FILTER_EDIT7,		0, CTRLMOVE_Y);   
		MoveCtrl(IDC_FILTER_EDIT8,		0, CTRLMOVE_Y);   
		MoveCtrl(IDC_FILTER_EDIT9,		0, CTRLMOVE_Y); 
		MoveCtrl(IDC_FILTER_EDIT10,		0, CTRLMOVE_Y);   
		MoveCtrl(IDC_FILTER_EDIT11,		0, CTRLMOVE_Y);   
		MoveCtrl(IDC_FILTER_EDIT12,		0, CTRLMOVE_Y);   
		MoveCtrl(IDC_FILTER_EDIT13,		0, CTRLMOVE_Y);   
		MoveCtrl(IDC_FILTER_EDIT14,		0, CTRLMOVE_Y); 
		MoveCtrl(IDC_FILTER_EDIT15,		0, CTRLMOVE_Y);   
		MoveCtrl(IDC_FILTER_EDIT16,		0, CTRLMOVE_Y);   
		MoveCtrl(IDC_FILTER_EDIT17,		0, CTRLMOVE_Y);   
		MoveCtrl(IDC_FILTER_EDIT18,		0, CTRLMOVE_Y);
		MoveCtrl(IDC_FILTER_EDIT19,		0, CTRLMOVE_Y);
		MoveCtrl(IDC_FILTER_EDIT20,		0, CTRLMOVE_Y);
		MoveCtrl(IDC_FILTER_DELBTN,		0, CTRLMOVE_Y);
		MoveCtrl(IDC_FILTER_STATIC,		0, CTRLMOVE_Y);
		MoveCtrl(IDC_FILTER_STATIC2,		0, CTRLMOVE_Y);
		MoveCtrl(IDC_SOUND_FILE,		0, CTRLMOVE_Y);
		MoveCtrl(IDC_BTN_PLAY,		0, CTRLMOVE_Y);
		MoveCtrl(IDC_BTN_PATH,		0, CTRLMOVE_Y);
		GetDlgItem(IDC_FILTER_STATIC)->ShowWindow(FALSE);
		GetDlgItem(IDC_FILTER_STATIC2)->ShowWindow(FALSE);
	}
	else
	{
		WndAllCtrlShow_Filter(TRUE);
		GetDlgItem(IDC_FILTER_STATIC)->ShowWindow(FALSE);
		GetDlgItem(IDC_FILTER_STATIC2)->ShowWindow(FALSE);

	}
	m_bMoveFilter = TRUE;
}

////////////////////////// 
// 함수명	    : CMarketDlg::SetFilterSetData
// 설 명	    : 필터링설정 탭에 설정된 내용을 userconf.ini에 기록하는 기능
// 작성일시	    : 2009/06/02
// 작성자       : kbkim
// 파라미터	    : CString sRootName : 제공처 루트 Name(공시, 이투데이, ...)
//				  CString sSubName  : 제공처 서브 Name(거래소공시, 증권, 금융, ...)
// 리턴 값 		: TRUE : 체크되어 있음, FALSE : 미 체크
// 관련요건	    : 000006(조원형:2009/05/12)
////////////////////////// 
void CMarketDlg::SetFilterSetData() 
{
	CString strINI(_T(""));
	CString strFilter;
	CString strSound;
	CString strData;
	char	readB[256] = _T("");
	int readL = 0;

	strINI.Format("%s\\User\\%s\\userconf.ini", m_szRootDir, m_szUser);

	readL = GetPrivateProfileString(szFilterMapName, "FILTERING", "0", readB, sizeof(readB), strINI);
	strFilter = CString(readB, readL);
	if (strFilter == "1")
	{
		m_FilterUseCheckBtn.SetCheck(BST_CHECKED);
		m_bFilter = TRUE;
	}
	else
	{
		m_FilterUseCheckBtn.SetCheck(BST_UNCHECKED);
		m_bFilter = FALSE;
	}

	m_FilterCheck1.EnableWindow(m_bFilter);
	m_FilterCheck2.EnableWindow(m_bFilter);
	m_FilterCheck3.EnableWindow(m_bFilter);
	m_FilterCheck4.EnableWindow(m_bFilter);
	m_FilterCheck5.EnableWindow(m_bFilter);
	m_FilterCheck6.EnableWindow(m_bFilter);
	m_FilterCheck7.EnableWindow(m_bFilter);
	m_FilterCheck8.EnableWindow(m_bFilter);
	m_FilterCheck9.EnableWindow(m_bFilter);
	m_FilterCheck10.EnableWindow(m_bFilter);
	m_FilterCheck11.EnableWindow(m_bFilter);
	m_FilterCheck12.EnableWindow(m_bFilter);
	m_FilterCheck13.EnableWindow(m_bFilter);
	m_FilterCheck14.EnableWindow(m_bFilter);
	m_FilterCheck15.EnableWindow(m_bFilter);
	m_FilterCheck16.EnableWindow(m_bFilter);
	m_FilterCheck17.EnableWindow(m_bFilter);
	m_FilterCheck18.EnableWindow(m_bFilter);
	m_FilterCheck19.EnableWindow(m_bFilter);
	m_FilterCheck20.EnableWindow(m_bFilter);

	readL = GetPrivateProfileString(szFilterMapName, "SOUND", "0", readB, sizeof(readB), strINI);
	strSound = CString(readB, readL);
	if (strSound == "1")
		m_FilterSoundCkeckBbtn.SetCheck(BST_CHECKED);
	else
		m_FilterSoundCkeckBbtn.SetCheck(BST_UNCHECKED);

	CString	path;	
	path.Format("%s\\%s\\%s", m_szRootDir, IMAGEDIR, "뉴스포착01.wav");

	readL = GetPrivateProfileString( szFilterMapName, "SOUNDFILE", path, readB, sizeof(readB), strINI );
	CString strFile(readB,readL);

	if( strFile == path )
	{
		WritePrivateProfileString(szFilterMapName, "SOUNDFILE", (LPTSTR)(LPCTSTR)strFile, strINI);
	}

	const int idx = strFile.ReverseFind( '\\' );
	const int iLen = strFile.GetLength();
	m_editSndFile.SetWindowText( strFile.Right( iLen-idx-1 ) );

	readL = GetPrivateProfileString(szFilterMapName, "CON1", "0", readB, sizeof(readB), strINI);
	strData = CString(readB, readL);
	if(strData == "1")
		m_FilterCheck1.SetCheck(BST_CHECKED);
	else
		m_FilterCheck1.SetCheck(BST_UNCHECKED);
	readL = GetPrivateProfileString(szFilterMapName, "ECON1", "0", readB, sizeof(readB), strINI);
	m_sFilterStr1 = CString(readB, readL);
	if (strData != "1")
		m_sFilterStr1.Replace("0", "");
	GetDlgItem(IDC_FILTER_EDIT1)->SetWindowText(m_sFilterStr1);

	readL = GetPrivateProfileString(szFilterMapName, "CON2", "0", readB, sizeof(readB), strINI);
	strData = CString(readB, readL);
	if(strData == "1")
		m_FilterCheck2.SetCheck(BST_CHECKED);
	else
		m_FilterCheck2.SetCheck(BST_UNCHECKED);	
	readL = GetPrivateProfileString(szFilterMapName, "ECON2", "0", readB, sizeof(readB), strINI);
	m_sFilterStr2 = CString(readB, readL);
	if (strData != "1")
		m_sFilterStr2.Replace("0", "");
	GetDlgItem(IDC_FILTER_EDIT2)->SetWindowText(m_sFilterStr2);
	
	readL = GetPrivateProfileString(szFilterMapName, "CON3", "0", readB, sizeof(readB), strINI);
	strData = CString(readB, readL);
	if(strData == "1")
		m_FilterCheck3.SetCheck(BST_CHECKED);
	else
		m_FilterCheck3.SetCheck(BST_UNCHECKED);	
	readL = GetPrivateProfileString(szFilterMapName, "ECON3", "0", readB, sizeof(readB), strINI);
	m_sFilterStr3 = CString(readB, readL);
	if (strData != "1")
		m_sFilterStr3.Replace("0", "");
	GetDlgItem(IDC_FILTER_EDIT3)->SetWindowText(m_sFilterStr3);
	
	readL = GetPrivateProfileString(szFilterMapName, "CON4", "0", readB, sizeof(readB), strINI);
	strData = CString(readB, readL);
	if(strData == "1")
		m_FilterCheck4.SetCheck(BST_CHECKED);
	else
		m_FilterCheck4.SetCheck(BST_UNCHECKED);	
	readL = GetPrivateProfileString(szFilterMapName, "ECON4", "0", readB, sizeof(readB), strINI);
	m_sFilterStr4 = CString(readB, readL);
	if (strData != "1")
		m_sFilterStr4.Replace("0", "");
	GetDlgItem(IDC_FILTER_EDIT4)->SetWindowText(m_sFilterStr4);
	
	readL = GetPrivateProfileString(szFilterMapName, "CON5", "0", readB, sizeof(readB), strINI);
	strData = CString(readB, readL);
	if(strData == "1")
		m_FilterCheck5.SetCheck(BST_CHECKED);
	else
		m_FilterCheck5.SetCheck(BST_UNCHECKED);	
	readL = GetPrivateProfileString(szFilterMapName, "ECON5", "0", readB, sizeof(readB), strINI);
	m_sFilterStr5 = CString(readB, readL);
	if (strData != "1")
		m_sFilterStr5.Replace("0", "");
	GetDlgItem(IDC_FILTER_EDIT5)->SetWindowText(m_sFilterStr5);
	
	readL = GetPrivateProfileString(szFilterMapName, "CON6", "0", readB, sizeof(readB), strINI);
	strData = CString(readB, readL);
	if(strData == "1")
		m_FilterCheck6.SetCheck(BST_CHECKED);
	else
		m_FilterCheck6.SetCheck(BST_UNCHECKED);	
	readL = GetPrivateProfileString(szFilterMapName, "ECON6", "0", readB, sizeof(readB), strINI);
	m_sFilterStr6 = CString(readB, readL);
	if (strData != "1")
		m_sFilterStr6.Replace("0", "");
	GetDlgItem(IDC_FILTER_EDIT6)->SetWindowText(m_sFilterStr6);
	
	readL = GetPrivateProfileString(szFilterMapName, "CON7", "0", readB, sizeof(readB), strINI);
	strData = CString(readB, readL);
	if(strData == "1")
		m_FilterCheck7.SetCheck(BST_CHECKED);
	else
		m_FilterCheck7.SetCheck(BST_UNCHECKED);	
	readL = GetPrivateProfileString(szFilterMapName, "ECON7", "0", readB, sizeof(readB), strINI);
	m_sFilterStr7 = CString(readB, readL);
	if (strData != "1")
		m_sFilterStr7.Replace("0", "");
	GetDlgItem(IDC_FILTER_EDIT7)->SetWindowText(m_sFilterStr7);
	
	readL = GetPrivateProfileString(szFilterMapName, "CON8", "0", readB, sizeof(readB), strINI);
	strData = CString(readB, readL);
	if(strData == "1")
		m_FilterCheck8.SetCheck(BST_CHECKED);
	else
		m_FilterCheck8.SetCheck(BST_UNCHECKED);	
	readL = GetPrivateProfileString(szFilterMapName, "ECON8", "0", readB, sizeof(readB), strINI);
	m_sFilterStr8 = CString(readB, readL);
	if (strData != "1")
		m_sFilterStr8.Replace("0", "");
	GetDlgItem(IDC_FILTER_EDIT8)->SetWindowText(m_sFilterStr8);

	readL = GetPrivateProfileString(szFilterMapName, "CON9", "0", readB, sizeof(readB), strINI);
	strData = CString(readB, readL);
	if(strData == "1")
		m_FilterCheck9.SetCheck(BST_CHECKED);
	else
		m_FilterCheck9.SetCheck(BST_UNCHECKED);	
	readL = GetPrivateProfileString(szFilterMapName, "ECON9", "0", readB, sizeof(readB), strINI);
	m_sFilterStr9 = CString(readB, readL);
	if (strData != "1")
		m_sFilterStr9.Replace("0", "");
	GetDlgItem(IDC_FILTER_EDIT9)->SetWindowText(m_sFilterStr9);

	readL = GetPrivateProfileString(szFilterMapName, "CON10", "0", readB, sizeof(readB), strINI);
	strData = CString(readB, readL);
	if(strData == "1")
		m_FilterCheck10.SetCheck(BST_CHECKED);
	else
		m_FilterCheck10.SetCheck(BST_UNCHECKED);	
	readL = GetPrivateProfileString(szFilterMapName, "ECON10", "0", readB, sizeof(readB), strINI);
	m_sFilterStr10 = CString(readB, readL);
	if (strData != "1")
		m_sFilterStr10.Replace("0", "");
	GetDlgItem(IDC_FILTER_EDIT10)->SetWindowText(m_sFilterStr10);

	readL = GetPrivateProfileString(szFilterMapName, "CON11", "0", readB, sizeof(readB), strINI);
	strData = CString(readB, readL);
	if(strData == "1")
		m_FilterCheck11.SetCheck(BST_CHECKED);
	else
		m_FilterCheck11.SetCheck(BST_UNCHECKED);
	readL = GetPrivateProfileString(szFilterMapName, "ECON11", "0", readB, sizeof(readB), strINI);
	m_sFilterStr11 = CString(readB, readL);
	if (strData != "1")
		m_sFilterStr11.Replace("0", "");
	GetDlgItem(IDC_FILTER_EDIT11)->SetWindowText(m_sFilterStr11);

	readL = GetPrivateProfileString(szFilterMapName, "CON12", "0", readB, sizeof(readB), strINI);
	strData = CString(readB, readL);
	if(strData == "1")
		m_FilterCheck12.SetCheck(BST_CHECKED);
	else
		m_FilterCheck12.SetCheck(BST_UNCHECKED);	
	readL = GetPrivateProfileString(szFilterMapName, "ECON12", "0", readB, sizeof(readB), strINI);
	m_sFilterStr12 = CString(readB, readL);
	if (strData != "1")
		m_sFilterStr12.Replace("0", "");
	GetDlgItem(IDC_FILTER_EDIT12)->SetWindowText(m_sFilterStr12);
	
	readL = GetPrivateProfileString(szFilterMapName, "CON13", "0", readB, sizeof(readB), strINI);
	strData = CString(readB, readL);
	if(strData == "1")
		m_FilterCheck13.SetCheck(BST_CHECKED);
	else
		m_FilterCheck13.SetCheck(BST_UNCHECKED);	
	readL = GetPrivateProfileString(szFilterMapName, "ECON13", "0", readB, sizeof(readB), strINI);
	m_sFilterStr13 = CString(readB, readL);
	if (strData != "1")
		m_sFilterStr13.Replace("0", "");
	GetDlgItem(IDC_FILTER_EDIT13)->SetWindowText(m_sFilterStr13);
	
	readL = GetPrivateProfileString(szFilterMapName, "CON14", "0", readB, sizeof(readB), strINI);
	strData = CString(readB, readL);
	if(strData == "1")
		m_FilterCheck14.SetCheck(BST_CHECKED);
	else
		m_FilterCheck14.SetCheck(BST_UNCHECKED);	
	readL = GetPrivateProfileString(szFilterMapName, "ECON14", "0", readB, sizeof(readB), strINI);
	m_sFilterStr14 = CString(readB, readL);
	if (strData != "1")
		m_sFilterStr14.Replace("0", "");
	GetDlgItem(IDC_FILTER_EDIT14)->SetWindowText(m_sFilterStr14);
	
	readL = GetPrivateProfileString(szFilterMapName, "CON15", "0", readB, sizeof(readB), strINI);
	strData = CString(readB, readL);
	if(strData == "1")
		m_FilterCheck15.SetCheck(BST_CHECKED);
	else
		m_FilterCheck15.SetCheck(BST_UNCHECKED);	
	readL = GetPrivateProfileString(szFilterMapName, "ECON15", "0", readB, sizeof(readB), strINI);
	m_sFilterStr15 = CString(readB, readL);
	if (strData != "1")
		m_sFilterStr15.Replace("0", "");
	GetDlgItem(IDC_FILTER_EDIT15)->SetWindowText(m_sFilterStr15);
	
	readL = GetPrivateProfileString(szFilterMapName, "CON16", "0", readB, sizeof(readB), strINI);
	strData = CString(readB, readL);
	if(strData == "1")
		m_FilterCheck16.SetCheck(BST_CHECKED);
	else
		m_FilterCheck16.SetCheck(BST_UNCHECKED);	
	readL = GetPrivateProfileString(szFilterMapName, "ECON16", "0", readB, sizeof(readB), strINI);
	m_sFilterStr16 = CString(readB, readL);
	if (strData != "1")
		m_sFilterStr16.Replace("0", "");
	GetDlgItem(IDC_FILTER_EDIT16)->SetWindowText(m_sFilterStr16);
	
	readL = GetPrivateProfileString(szFilterMapName, "CON17", "0", readB, sizeof(readB), strINI);
	strData = CString(readB, readL);
	if(strData == "1")
		m_FilterCheck17.SetCheck(BST_CHECKED);
	else
		m_FilterCheck17.SetCheck(BST_UNCHECKED);	
	readL = GetPrivateProfileString(szFilterMapName, "ECON17", "0", readB, sizeof(readB), strINI);
	m_sFilterStr17 = CString(readB, readL);
	if (strData != "1")
		m_sFilterStr17.Replace("0", "");
	GetDlgItem(IDC_FILTER_EDIT17)->SetWindowText(m_sFilterStr17);
	
	readL = GetPrivateProfileString(szFilterMapName, "CON18", "0", readB, sizeof(readB), strINI);
	strData = CString(readB, readL);
	if(strData == "1")
		m_FilterCheck18.SetCheck(BST_CHECKED);
	else
		m_FilterCheck18.SetCheck(BST_UNCHECKED);	
	readL = GetPrivateProfileString(szFilterMapName, "ECON18", "0", readB, sizeof(readB), strINI);
	m_sFilterStr18 = CString(readB, readL);
	if (strData != "1")
		m_sFilterStr18.Replace("0", "");
	GetDlgItem(IDC_FILTER_EDIT18)->SetWindowText(m_sFilterStr18);

	readL = GetPrivateProfileString(szFilterMapName, "CON19", "0", readB, sizeof(readB), strINI);
	strData = CString(readB, readL);
	if(strData == "1")
		m_FilterCheck19.SetCheck(BST_CHECKED);
	else
		m_FilterCheck19.SetCheck(BST_UNCHECKED);	
	readL = GetPrivateProfileString(szFilterMapName, "ECON19", "0", readB, sizeof(readB), strINI);
	m_sFilterStr19 = CString(readB, readL);
	if (strData != "1")
		m_sFilterStr19.Replace("0", "");
	GetDlgItem(IDC_FILTER_EDIT19)->SetWindowText(m_sFilterStr19);

	readL = GetPrivateProfileString(szFilterMapName, "CON20", "0", readB, sizeof(readB), strINI);
	strData = CString(readB, readL);
	if(strData == "1")
		m_FilterCheck20.SetCheck(BST_CHECKED);
	else
		m_FilterCheck20.SetCheck(BST_UNCHECKED);	
	readL = GetPrivateProfileString(szFilterMapName, "ECON20", "0", readB, sizeof(readB), strINI);
	m_sFilterStr20 = CString(readB, readL);
	if (strData != "1")
		m_sFilterStr20.Replace("0", "");
	GetDlgItem(IDC_FILTER_EDIT20)->SetWindowText(m_sFilterStr20);

	OnFilterCheck(&m_FilterCheck1, &m_FilterEdit1);
	OnFilterCheck(&m_FilterCheck2, &m_FilterEdit2);
	OnFilterCheck(&m_FilterCheck3, &m_FilterEdit3);
	OnFilterCheck(&m_FilterCheck4, &m_FilterEdit4);
	OnFilterCheck(&m_FilterCheck5, &m_FilterEdit5);
	OnFilterCheck(&m_FilterCheck6, &m_FilterEdit6);
	OnFilterCheck(&m_FilterCheck7, &m_FilterEdit7);
	OnFilterCheck(&m_FilterCheck8, &m_FilterEdit8);
	OnFilterCheck(&m_FilterCheck9, &m_FilterEdit9);
	OnFilterCheck(&m_FilterCheck10, &m_FilterEdit10);
	OnFilterCheck(&m_FilterCheck11, &m_FilterEdit11);
	OnFilterCheck(&m_FilterCheck12, &m_FilterEdit12);
	OnFilterCheck(&m_FilterCheck13, &m_FilterEdit13);
	OnFilterCheck(&m_FilterCheck14, &m_FilterEdit14);
	OnFilterCheck(&m_FilterCheck15, &m_FilterEdit15);
	OnFilterCheck(&m_FilterCheck16, &m_FilterEdit16);
	OnFilterCheck(&m_FilterCheck17, &m_FilterEdit17);
	OnFilterCheck(&m_FilterCheck18, &m_FilterEdit18);
	OnFilterCheck(&m_FilterCheck19, &m_FilterEdit19);
	OnFilterCheck(&m_FilterCheck20, &m_FilterEdit20);
}

////////////////////////// 
// 함수명	    : CMarketDlg::OnFilterUsecheck
// 설 명	    : 필터링사용 체크시 호출
// 작성일시	    : 2009/06/02
// 작성자       : kbkim
// 파라미터	    : 
// 리턴 값 		: 
// 관련요건	    : 000006(조원형:2009/05/12)
////////////////////////// 
void CMarketDlg::OnFilterUsecheck() 
{
	// TODO: Add your control notification handler code here
	int bEnableCtrl{};
	if(m_FilterUseCheckBtn.GetCheck() == BST_CHECKED )
		bEnableCtrl = TRUE;
	else
		bEnableCtrl = FALSE;

	m_FilterCheck1.EnableWindow(bEnableCtrl);
	m_FilterCheck2.EnableWindow(bEnableCtrl);
	m_FilterCheck3.EnableWindow(bEnableCtrl);
	m_FilterCheck4.EnableWindow(bEnableCtrl);
	m_FilterCheck5.EnableWindow(bEnableCtrl);
	m_FilterCheck6.EnableWindow(bEnableCtrl);
	m_FilterCheck7.EnableWindow(bEnableCtrl);
	m_FilterCheck8.EnableWindow(bEnableCtrl);
	m_FilterCheck9.EnableWindow(bEnableCtrl);
	m_FilterCheck10.EnableWindow(bEnableCtrl);
	m_FilterCheck11.EnableWindow(bEnableCtrl);
	m_FilterCheck12.EnableWindow(bEnableCtrl);
	m_FilterCheck13.EnableWindow(bEnableCtrl);
	m_FilterCheck14.EnableWindow(bEnableCtrl);
	m_FilterCheck15.EnableWindow(bEnableCtrl);
	m_FilterCheck16.EnableWindow(bEnableCtrl);
	m_FilterCheck17.EnableWindow(bEnableCtrl);
	m_FilterCheck18.EnableWindow(bEnableCtrl);
	m_FilterCheck19.EnableWindow(bEnableCtrl);
	m_FilterCheck20.EnableWindow(bEnableCtrl);

	if(bEnableCtrl == FALSE)
	{
		m_FilterEdit1.EnableWindow(bEnableCtrl);
		m_FilterEdit2.EnableWindow(bEnableCtrl);
		m_FilterEdit3.EnableWindow(bEnableCtrl);
		m_FilterEdit4.EnableWindow(bEnableCtrl);
		m_FilterEdit5.EnableWindow(bEnableCtrl);
		m_FilterEdit6.EnableWindow(bEnableCtrl);
		m_FilterEdit7.EnableWindow(bEnableCtrl);
		m_FilterEdit8.EnableWindow(bEnableCtrl);
		m_FilterEdit9.EnableWindow(bEnableCtrl);
		m_FilterEdit10.EnableWindow(bEnableCtrl);
		m_FilterEdit11.EnableWindow(bEnableCtrl);
		m_FilterEdit12.EnableWindow(bEnableCtrl);
		m_FilterEdit13.EnableWindow(bEnableCtrl);
		m_FilterEdit14.EnableWindow(bEnableCtrl);
		m_FilterEdit15.EnableWindow(bEnableCtrl);
		m_FilterEdit16.EnableWindow(bEnableCtrl);
		m_FilterEdit17.EnableWindow(bEnableCtrl);
		m_FilterEdit18.EnableWindow(bEnableCtrl);
		m_FilterEdit19.EnableWindow(bEnableCtrl);
		m_FilterEdit20.EnableWindow(bEnableCtrl);
	}
	else
	{
		if(m_FilterCheck1.GetCheck() == BST_CHECKED)
			m_FilterEdit1.EnableWindow(bEnableCtrl);

		if(m_FilterCheck2.GetCheck() == BST_CHECKED)
			m_FilterEdit2.EnableWindow(bEnableCtrl);

		if(m_FilterCheck3.GetCheck() == BST_CHECKED)
			m_FilterEdit3.EnableWindow(bEnableCtrl);
		
		if(m_FilterCheck4.GetCheck() == BST_CHECKED)
			m_FilterEdit4.EnableWindow(bEnableCtrl);

		if(m_FilterCheck5.GetCheck() == BST_CHECKED)
			m_FilterEdit5.EnableWindow(bEnableCtrl);
		
		if(m_FilterCheck6.GetCheck() == BST_CHECKED)
			m_FilterEdit6.EnableWindow(bEnableCtrl);

		if(m_FilterCheck7.GetCheck() == BST_CHECKED)
			m_FilterEdit7.EnableWindow(bEnableCtrl);

		if(m_FilterCheck8.GetCheck() == BST_CHECKED)
			m_FilterEdit8.EnableWindow(bEnableCtrl);

		if(m_FilterCheck9.GetCheck() == BST_CHECKED)
			m_FilterEdit9.EnableWindow(bEnableCtrl);

		if(m_FilterCheck10.GetCheck() == BST_CHECKED)
			m_FilterEdit10.EnableWindow(bEnableCtrl);

		if(m_FilterCheck11.GetCheck() == BST_CHECKED)
			m_FilterEdit11.EnableWindow(bEnableCtrl);

		if(m_FilterCheck12.GetCheck() == BST_CHECKED)
			m_FilterEdit12.EnableWindow(bEnableCtrl);

		if(m_FilterCheck13.GetCheck() == BST_CHECKED)
			m_FilterEdit13.EnableWindow(bEnableCtrl);

		if(m_FilterCheck14.GetCheck() == BST_CHECKED)
			m_FilterEdit14.EnableWindow(bEnableCtrl);

		if(m_FilterCheck15.GetCheck() == BST_CHECKED)
			m_FilterEdit15.EnableWindow(bEnableCtrl);

		if(m_FilterCheck16.GetCheck() == BST_CHECKED)
			m_FilterEdit16.EnableWindow(bEnableCtrl);

		if(m_FilterCheck17.GetCheck() == BST_CHECKED)
			m_FilterEdit17.EnableWindow(bEnableCtrl);

		if(m_FilterCheck18.GetCheck() == BST_CHECKED)
			m_FilterEdit18.EnableWindow(bEnableCtrl);

		if(m_FilterCheck19.GetCheck() == BST_CHECKED)
			m_FilterEdit19.EnableWindow(bEnableCtrl);

		if(m_FilterCheck20.GetCheck() == BST_CHECKED)
			m_FilterEdit20.EnableWindow(bEnableCtrl);
	}

	m_FilterDelBtn.EnableWindow(bEnableCtrl);
	m_FilterSaveEdit.EnableWindow(bEnableCtrl);

	CString strINI(_T(""));
	
	strINI.Format("%s\\User\\%s\\userconf.ini", m_szRootDir, m_szUser);

	if(m_FilterUseCheckBtn.GetCheck() == BST_CHECKED )
	{
		WriteConfigFileInt(strINI, szFilterMapName, "FILTERING", 1);
		m_bFilter = TRUE;
	}
	else
	{
		WriteConfigFileInt(strINI, szFilterMapName, "FILTERING", 0);
		m_bFilter = FALSE;
	}
}

void CMarketDlg::OnFilterCheck1() 
{
	// TODO: Add your control notification handler code here
	OnFilterCheck(&m_FilterCheck1, &m_FilterEdit1);
}

void CMarketDlg::OnFilterCheck2() 
{
	// TODO: Add your control notification handler code here
	OnFilterCheck(&m_FilterCheck2, &m_FilterEdit2);
}
void CMarketDlg::OnFilterCheck3() 
{
	// TODO: Add your control notification handler code here
	OnFilterCheck(&m_FilterCheck3, &m_FilterEdit3);
}

void CMarketDlg::OnFilterCheck4() 
{
	// TODO: Add your control notification handler code here
	OnFilterCheck(&m_FilterCheck4, &m_FilterEdit4);
}

void CMarketDlg::OnFilterCheck5() 
{
	// TODO: Add your control notification handler code here
	OnFilterCheck(&m_FilterCheck5, &m_FilterEdit5);
}

void CMarketDlg::OnFilterCheck6() 
{
	// TODO: Add your control notification handler code here
	OnFilterCheck(&m_FilterCheck6, &m_FilterEdit6);
}

void CMarketDlg::OnFilterCheck7() 
{
	// TODO: Add your control notification handler code here
	OnFilterCheck(&m_FilterCheck7, &m_FilterEdit7);
}

void CMarketDlg::OnFilterCheck8() 
{
	// TODO: Add your control notification handler code here
	OnFilterCheck(&m_FilterCheck8, &m_FilterEdit8);
}

void CMarketDlg::OnFilterCheck9() 
{
	// TODO: Add your control notification handler code here
	OnFilterCheck(&m_FilterCheck9, &m_FilterEdit9);
}

void CMarketDlg::OnFilterCheck(CButton* pCheckButton, CEdit* pEdit) 
{
	// TODO: Add your control notification handler code here
	if(pCheckButton->GetCheck() == BST_CHECKED )
		pEdit->EnableWindow(TRUE);
	else
		pEdit->EnableWindow(FALSE);
}

void CMarketDlg::OnFilterDelbtn() 
{
	// TODO: Add your control notification handler code here
	FilterCtrlClear(&m_FilterCheck1, &m_FilterEdit1);
	FilterCtrlClear(&m_FilterCheck2, &m_FilterEdit2);
	FilterCtrlClear(&m_FilterCheck3, &m_FilterEdit3);
	FilterCtrlClear(&m_FilterCheck4, &m_FilterEdit4);
	FilterCtrlClear(&m_FilterCheck5, &m_FilterEdit5);
	FilterCtrlClear(&m_FilterCheck6, &m_FilterEdit6);
	FilterCtrlClear(&m_FilterCheck7, &m_FilterEdit7);
	FilterCtrlClear(&m_FilterCheck8, &m_FilterEdit8);
	FilterCtrlClear(&m_FilterCheck9, &m_FilterEdit9);
}

void CMarketDlg::FilterCtrlClear(CButton* pCheckButton, CEdit* pEdit) 
{
	pCheckButton->SetCheck(BST_UNCHECKED);
	pCheckButton->EnableWindow(TRUE);
	pEdit->SetWindowText("");
	pEdit->EnableWindow(FALSE);
}

void CMarketDlg::OnFilterSavebtn() 
{
	// TODO: Add your control notification handler code here
	CString strINI(_T(""));
	CString sData(_T(""));

	strINI.Format("%s\\User\\%s\\userconf.ini", m_szRootDir, m_szUser);
	if(m_FilterUseCheckBtn.GetCheck() == BST_CHECKED )
	{
		WriteConfigFileInt(strINI, szFilterMapName, "FILTERING", 1);
		m_bFilter = TRUE;
	}
	else
	{
		WriteConfigFileInt(strINI, szFilterMapName, "FILTERING", 0);
		m_bFilter = FALSE;
	}

	if(m_FilterSoundCkeckBbtn.GetCheck() == BST_CHECKED )
		WriteConfigFileInt(strINI, szFilterMapName, "SOUND", 1);
	else
		WriteConfigFileInt(strINI, szFilterMapName, "SOUND", 0);

	m_editSndFile.GetWindowText( sData );
	WritePrivateProfileString(szFilterMapName, "SOUNDFILE", (LPTSTR)(LPCTSTR)sData, strINI);

	GetDlgItem(IDC_FILTER_EDIT1)->GetWindowText(sData);
	if(m_FilterCheck1.GetCheck() == BST_CHECKED )
	{
		WriteConfigFileInt(strINI, szFilterMapName, "CON1", 1);
		m_sFilterStr1 = sData;
	}
	else
	{
		WriteConfigFileInt(strINI, szFilterMapName, "CON1", 0);
		m_sFilterStr1 = "";
	}
	WritePrivateProfileString(szFilterMapName, "ECON1", (LPTSTR)(LPCTSTR)sData, strINI);

	GetDlgItem(IDC_FILTER_EDIT2)->GetWindowText(sData);
	if(m_FilterCheck2.GetCheck() == BST_CHECKED )
	{
		WriteConfigFileInt(strINI, szFilterMapName, "CON2", 1);
		m_sFilterStr2 = sData;
	}
	else
	{
		WriteConfigFileInt(strINI, szFilterMapName, "CON2", 0);
		m_sFilterStr2 = "";
	}
	WritePrivateProfileString(szFilterMapName, "ECON2", (LPTSTR)(LPCTSTR)sData, strINI);

	GetDlgItem(IDC_FILTER_EDIT3)->GetWindowText(sData);
	if(m_FilterCheck3.GetCheck() == BST_CHECKED )
	{
		WriteConfigFileInt(strINI, szFilterMapName, "CON3", 1);
		m_sFilterStr3 = sData;
	}
	else
	{
		WriteConfigFileInt(strINI, szFilterMapName, "CON3", 0);
		m_sFilterStr3 = "";
	}
	WritePrivateProfileString(szFilterMapName, "ECON3", (LPTSTR)(LPCTSTR)sData, strINI);

	GetDlgItem(IDC_FILTER_EDIT4)->GetWindowText(sData);
	if(m_FilterCheck4.GetCheck() == BST_CHECKED )
	{
		WriteConfigFileInt(strINI, szFilterMapName, "CON4", 1);
		m_sFilterStr4 = sData;
	}
	else
	{
		WriteConfigFileInt(strINI, szFilterMapName, "CON4", 0);
		m_sFilterStr4 = "";
	}

	WritePrivateProfileString(szFilterMapName, "ECON4", (LPTSTR)(LPCTSTR)sData, strINI);

	GetDlgItem(IDC_FILTER_EDIT5)->GetWindowText(sData);
	if(m_FilterCheck5.GetCheck() == BST_CHECKED )
	{
		WriteConfigFileInt(strINI, szFilterMapName, "CON5", 1);
		m_sFilterStr5 = sData;
	}
	else
	{
		WriteConfigFileInt(strINI, szFilterMapName, "CON5", 0);
		m_sFilterStr5 = "";
	}

	WritePrivateProfileString(szFilterMapName, "ECON5", (LPTSTR)(LPCTSTR)sData, strINI);

	GetDlgItem(IDC_FILTER_EDIT6)->GetWindowText(sData);
	if(m_FilterCheck6.GetCheck() == BST_CHECKED )
	{
		WriteConfigFileInt(strINI, szFilterMapName, "CON6", 1);
		m_sFilterStr6 = sData;
	}
	else
	{
		WriteConfigFileInt(strINI, szFilterMapName, "CON6", 0);
		m_sFilterStr6 = "";
	}

	WritePrivateProfileString(szFilterMapName, "ECON6", (LPTSTR)(LPCTSTR)sData, strINI);

	GetDlgItem(IDC_FILTER_EDIT7)->GetWindowText(sData);
	if(m_FilterCheck7.GetCheck() == BST_CHECKED )
	{
		WriteConfigFileInt(strINI, szFilterMapName, "CON7", 1);
		m_sFilterStr7 = sData;
	}
	else
	{
		WriteConfigFileInt(strINI, szFilterMapName, "CON7", 0);
		m_sFilterStr7 = "";
	}

	WritePrivateProfileString(szFilterMapName, "ECON7", (LPTSTR)(LPCTSTR)sData, strINI);

	GetDlgItem(IDC_FILTER_EDIT8)->GetWindowText(sData);
	if(m_FilterCheck8.GetCheck() == BST_CHECKED )
	{
		WriteConfigFileInt(strINI, szFilterMapName, "CON8", 1);
		m_sFilterStr8 = sData;
	}
	else
	{
		WriteConfigFileInt(strINI, szFilterMapName, "CON8", 0);
		m_sFilterStr8 = "";
	}

	WritePrivateProfileString(szFilterMapName, "ECON8", (LPTSTR)(LPCTSTR)sData, strINI);

	GetDlgItem(IDC_FILTER_EDIT9)->GetWindowText(sData);
	if(m_FilterCheck9.GetCheck() == BST_CHECKED )
	{
		WriteConfigFileInt(strINI, szFilterMapName, "CON9", 1);
		m_sFilterStr9 = sData;
	}
	else
	{
		WriteConfigFileInt(strINI, szFilterMapName, "CON9", 0);
		m_sFilterStr9 = "";
	}
	WritePrivateProfileString(szFilterMapName, "ECON9", (LPTSTR)(LPCTSTR)sData, strINI);

	GetDlgItem(IDC_FILTER_EDIT10)->GetWindowText(sData);
	if(m_FilterCheck10.GetCheck() == BST_CHECKED )
	{
		WriteConfigFileInt(strINI, szFilterMapName, "CON10", 1);
		m_sFilterStr10 = sData;
	}
	else
	{
		WriteConfigFileInt(strINI, szFilterMapName, "CON10", 0);
		m_sFilterStr10 = "";
	}
	WritePrivateProfileString(szFilterMapName, "ECON10", (LPTSTR)(LPCTSTR)sData, strINI);

	GetDlgItem(IDC_FILTER_EDIT11)->GetWindowText(sData);
	if(m_FilterCheck11.GetCheck() == BST_CHECKED )
	{
		WriteConfigFileInt(strINI, szFilterMapName, "CON11", 1);
		m_sFilterStr11 = sData;
	}
	else
	{
		WriteConfigFileInt(strINI, szFilterMapName, "CON11", 0);
		m_sFilterStr11 = "";
	}
	WritePrivateProfileString(szFilterMapName, "ECON11", (LPTSTR)(LPCTSTR)sData, strINI);

	GetDlgItem(IDC_FILTER_EDIT12)->GetWindowText(sData);
	if(m_FilterCheck12.GetCheck() == BST_CHECKED )
	{
		WriteConfigFileInt(strINI, szFilterMapName, "CON12", 1);
		m_sFilterStr12 = sData;
	}
	else
	{
		WriteConfigFileInt(strINI, szFilterMapName, "CON12", 0);
		m_sFilterStr12 = "";
	}
	WritePrivateProfileString(szFilterMapName, "ECON12", (LPTSTR)(LPCTSTR)sData, strINI);

	GetDlgItem(IDC_FILTER_EDIT13)->GetWindowText(sData);
	if(m_FilterCheck13.GetCheck() == BST_CHECKED )
	{
		WriteConfigFileInt(strINI, szFilterMapName, "CON13", 1);
		m_sFilterStr13 = sData;
	}
	else
	{
		WriteConfigFileInt(strINI, szFilterMapName, "CON13", 0);
		m_sFilterStr13 = "";
	}
	WritePrivateProfileString(szFilterMapName, "ECON13", (LPTSTR)(LPCTSTR)sData, strINI);

	GetDlgItem(IDC_FILTER_EDIT14)->GetWindowText(sData);
	if(m_FilterCheck14.GetCheck() == BST_CHECKED )
	{
		WriteConfigFileInt(strINI, szFilterMapName, "CON14", 1);
		m_sFilterStr14 = sData;
	}
	else
	{
		WriteConfigFileInt(strINI, szFilterMapName, "CON14", 0);
		m_sFilterStr14 = "";
	}
	WritePrivateProfileString(szFilterMapName, "ECON14", (LPTSTR)(LPCTSTR)sData, strINI);

	GetDlgItem(IDC_FILTER_EDIT15)->GetWindowText(sData);
	if(m_FilterCheck15.GetCheck() == BST_CHECKED )
	{
		WriteConfigFileInt(strINI, szFilterMapName, "CON15", 1);
		m_sFilterStr15 = sData;
	}
	else
	{
		WriteConfigFileInt(strINI, szFilterMapName, "CON15", 0);
		m_sFilterStr15 = "";
	}
	WritePrivateProfileString(szFilterMapName, "ECON15", (LPTSTR)(LPCTSTR)sData, strINI);

	GetDlgItem(IDC_FILTER_EDIT16)->GetWindowText(sData);
	if(m_FilterCheck16.GetCheck() == BST_CHECKED )
	{
		WriteConfigFileInt(strINI, szFilterMapName, "CON16", 1);
		m_sFilterStr16 = sData;
	}
	else
	{
		WriteConfigFileInt(strINI, szFilterMapName, "CON16", 0);
		m_sFilterStr16 = "";
	}
	WritePrivateProfileString(szFilterMapName, "ECON16", (LPTSTR)(LPCTSTR)sData, strINI);

	GetDlgItem(IDC_FILTER_EDIT17)->GetWindowText(sData);
	if(m_FilterCheck17.GetCheck() == BST_CHECKED )
	{
		WriteConfigFileInt(strINI, szFilterMapName, "CON17", 1);
		m_sFilterStr17 = sData;
	}
	else
	{
		WriteConfigFileInt(strINI, szFilterMapName, "CON17", 0);
		m_sFilterStr17 = "";
	}
	WritePrivateProfileString(szFilterMapName, "ECON17", (LPTSTR)(LPCTSTR)sData, strINI);

	GetDlgItem(IDC_FILTER_EDIT18)->GetWindowText(sData);
	if(m_FilterCheck18.GetCheck() == BST_CHECKED )
	{
		WriteConfigFileInt(strINI, szFilterMapName, "CON18", 1);
		m_sFilterStr18 = sData;
	}
	else
	{
		WriteConfigFileInt(strINI, szFilterMapName, "CON18", 0);
		m_sFilterStr18 = "";
	}
	WritePrivateProfileString(szFilterMapName, "ECON18", (LPTSTR)(LPCTSTR)sData, strINI);

	GetDlgItem(IDC_FILTER_EDIT19)->GetWindowText(sData);
	if(m_FilterCheck19.GetCheck() == BST_CHECKED )
	{
		WriteConfigFileInt(strINI, szFilterMapName, "CON19", 1);
		m_sFilterStr19 = sData;
	}
	else
	{
		WriteConfigFileInt(strINI, szFilterMapName, "CON19", 0);
		m_sFilterStr19 = "";
	}
	WritePrivateProfileString(szFilterMapName, "ECON19", (LPTSTR)(LPCTSTR)sData, strINI);

	GetDlgItem(IDC_FILTER_EDIT20)->GetWindowText(sData);
	if(m_FilterCheck20.GetCheck() == BST_CHECKED )
	{
		WriteConfigFileInt(strINI, szFilterMapName, "CON20", 1);
		m_sFilterStr20 = sData;
	}
	else
	{
		WriteConfigFileInt(strINI, szFilterMapName, "CON20", 0);
		m_sFilterStr20 = "";
	}
	WritePrivateProfileString(szFilterMapName, "ECON20", (LPTSTR)(LPCTSTR)sData, strINI);
}

////////// 제공처 설정 관련 함수/////////////////////////////
void CMarketDlg::CreateMarketSetWnd()
{
	if(!m_bMoveMarket)
	{
		MoveCtrl(IDC_MARKETCANCEL_BTN,	CTRLMOVE_X, CTRLMOVE_Y);
		MoveCtrl(IDC_MARKET_TREE,		CTRLMOVE_X, CTRLMOVE_Y);
		MoveCtrl(IDC_MARKET_BTN,		CTRLMOVE_X, CTRLMOVE_Y);
		InitMarketTreeData();
	}
	else
		WndAllCtrlShow_Market(TRUE);
	
	m_bMoveMarket = TRUE;

}

////////////////////////// 
// 함수명	    : CMarketDlg::InitMarketTreeData
// 설 명	    : 제공처설정에 Tree 항목을 설정하는 기능
// 작성일시	    : 2009/06/02
// 작성자       : kbkim
// 파라미터	    : 
// 리턴 값 		: 없음
// 관련요건	    : 000006(조원형:2009/05/12)
////////////////////////// 
void CMarketDlg::InitMarketTreeData()
{
	// TODO: Add your specialized creation code here
	HTREEITEM hTreeItem{};
	HTREEITEM hItem{};
	
	NewsList NewsList;
	const int nInitCount = m_arNewsList.GetSize()-4;
	int iSubcnt = 0;
	//전체(0) 을 빼고 공시(1) 부터, 마지막(조회수상위),마지막-1(스크랩) 빼고 

	m_MarketTreeCtrl.SetSmartCheckBox(TRUE);
	for(int nIndex = 1;nIndex<nInitCount;nIndex++)
	{
		NewsList = m_arNewsList.GetAt(nIndex);

		if(NewsList.ikey != type_category && NewsList.ikey != type_subtotal)
		{
			hTreeItem = FillTreeControl(NULL, NewsList.sGubnName);
			iSubcnt = m_arNewsList.GetAt(nIndex).ibulruCnt;
			for(int j=0; j<iSubcnt-1; j++)
			{
				hItem = FillTreeControl(hTreeItem, m_arNewsList.GetAt(nIndex).sBulruNameList[j]);
				if(m_arNewsList.GetAt(nIndex).iCheck[j] == 1)
					m_MarketTreeCtrl.SetCheck(hItem, TRUE);
				else
					m_MarketTreeCtrl.SetCheck(hItem, FALSE);
			}
		}
	}

	//  외곽선을 나타나게 하기 위해서...
	m_MarketTreeCtrl.ExpandAll();
	m_MarketTreeCtrl.CollapseAll();
}

////////////////////////// 
// 함수명	    : CMarketDlg::FillTreeControl
// 설 명	    : TreeCtrl에 한개의 item을 추가하는 기능
// 작성일시	    : 2009/06/02
// 작성자       : kbkim
// 파라미터	    : HTREEITEM hParent : 추가하고자 하는 부모 TREEITEM
//				  CString strDate
// 리턴 값 		: HTREEITEM : 추가된 item의 TREEITEM, 실패시 NULL
// 관련요건	    : 000006(조원형:2009/05/12)
////////////////////////// 
HTREEITEM CMarketDlg::FillTreeControl(HTREEITEM hParent, CString strDate)
{
	// fill items
	TV_INSERTSTRUCT	AddTreeItem;
	memset (&AddTreeItem,0,sizeof(AddTreeItem));
	
	AddTreeItem.hParent = hParent;
	AddTreeItem.hInsertAfter = NULL;
	AddTreeItem.item.mask = TVIF_STATE | TVIF_TEXT | TVIF_IMAGE | TVIF_SELECTEDIMAGE;
	// !!! DO NOT USE Index 0 for Statusimage 
	AddTreeItem.item.state = INDEXTOSTATEIMAGEMASK (1);
	AddTreeItem.item.stateMask = 0;
	// !!! keep care when copying this string
	AddTreeItem.item.pszText = (char*)(LPCSTR)strDate;
	AddTreeItem.item.cchTextMax = 255;

	AddTreeItem.item.iImage = 0;
	AddTreeItem.item.iSelectedImage = 1;
	
	if(m_MarketTreeCtrl.GetSafeHwnd())
		return m_MarketTreeCtrl.InsertItem(&AddTreeItem);
	
	return NULL;
}

void CMarketDlg::OnKeyOkbutton() 
{
	// TODO: Add your control notification handler code here
	((CMapDlg*)m_pParentWnd)->m_arSpecialKeyList.Copy(m_arKeyList);
	((CMapDlg*)m_pParentWnd)->m_arStepColor.Copy(m_arStepColor);
	((CMapDlg*)m_pParentWnd)->SaveKeyConfig();
	((CMapDlg*)m_pParentWnd)->m_List.Invalidate(FALSE); 
}

void CMarketDlg::OnKeyButtonCancel() 
{
	// TODO: Add your control notification handler code here
	CDialog::OnCancel();
}

////////////////////////// 
// 함수명	    : CMarketDlg::OnMarketBtn
// 설 명	    : 제공처설정 화면에서 설정 버튼 클릭시 설정된 내용을 저장하는 기능
// 작성일시	    : 2009/06/02
// 작성자       : kbkim
// 파라미터	    : 
// 리턴 값 		: 
// 관련요건	    : 000006(조원형:2009/05/12)
////////////////////////// 
void CMarketDlg::OnMarketBtn() 
{
	// TODO: Add your control notification handler code here
	HTREEITEM hParent{};
	int nInitCount=0;
	int iSubcnt=0;
	NewsList NewsList;

	hParent = m_MarketTreeCtrl.GetRootItem();
	nInitCount = m_arNewsList.GetSize()-4;

	for(int nIndex = 1;nIndex<nInitCount;nIndex++)
	{
		NewsList = m_arNewsList.GetAt(nIndex);
		HTREEITEM hItem = m_MarketTreeCtrl.GetChildItem(hParent);
		
		if(m_MarketTreeCtrl.GetItemText(hParent) == "")
			break;
		
		iSubcnt = m_arNewsList.GetAt(nIndex).ibulruCnt;
		for(int j=0; j<iSubcnt-1; j++)
		{
			if(m_MarketTreeCtrl.IsChecked(hItem))
			{
				NewsList.iCheck[j] = 1;
				m_arNewsList.RemoveAt(nIndex);
				m_arNewsList.InsertAt(nIndex, NewsList);
			}
			else
			{
				NewsList.iCheck[j] = 0;
				m_arNewsList.RemoveAt(nIndex);
				m_arNewsList.InsertAt(nIndex, NewsList);
			}
			hItem = m_MarketTreeCtrl.GetNextSiblingItem(hItem);
		}
		
		hParent = m_MarketTreeCtrl.GetNextSiblingItem(hParent);
	}
	
	((CMapDlg*)m_pParentWnd)->m_arNewsList.Copy(m_arNewsList);
}

void CMarketDlg::OnClickFilterResultList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	const NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	if(pNMListView->iItem < 0) // 빈 공간 클릭시 return
		return;

	CString strTitle, strKey, strCode;
	strTitle = m_FilterResultListCtrl.GetItemText(pNMListView->iItem, 1);
	strKey	 = m_FilterResultListCtrl.GetItemText(pNMListView->iItem, 2);
	strCode	 = m_FilterResultListCtrl.GetItemText(pNMListView->iItem, 3);
	
	CString str;
	str.Format("IBNEWSXX /t /p5 /d 015\t%s\n016\t%s\n1301\t%s", strTitle, strKey, strCode);

	if(m_pAxWnd)
		m_pAxWnd->SendMessage(WM_USER, MAKEWPARAM(viewDLL, typePOPUPX), (LPARAM)(LPCTSTR)str);
	
	*pResult = 0;	
}

void FillSafeArray(OLECHAR FAR* sz, int iRow, int iCol,
				   COleSafeArray* sa)
{
	
	VARIANT v{};
	long index[2];
	
	index[0] = iRow;
	index[1] = iCol;
	
	VariantInit(&v);
	v.vt = VT_BSTR;
	v.bstrVal = SysAllocString(sz);
	sa->PutElement(index, v.bstrVal);
	SysFreeString(v.bstrVal);
	VariantClear(&v);
	
}

void CMarketDlg::OnFilterlistExcel() 
{
	_Application app;
	Workbooks books;
	_Workbook book;
	
	Worksheets sheets;
	_Worksheet sheet;
	Range range;
	Font font;
	Range cols;
	Interior interior;
	Borders borders;

	// 2010.07.21 화면캡쳐시 메뉴잔상이 남는 현상이 있어 Invalidate -> 메세지펌핑 후에 캡쳐한다. by LKM
	{
		MSG msg;
		Invalidate(TRUE);
		while(PeekMessage(&msg, m_hWnd, 0, 0, PM_REMOVE)==TRUE)
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	int ii{}, size{};
	char r_end[4]{};
	memset (r_end, 0x00, sizeof(r_end));
	CString time, title, code, hname;
	// Commonly used OLE variants.
	COleVariant
		covTrue((short)TRUE),
		covFalse((short)FALSE),
		covOptional((long)DISP_E_PARAMNOTFOUND, VT_ERROR);
	
	if(!app.CreateDispatch("Excel.Application"))
	{
		AfxMessageBox("엑셀 파일을 열수 없습니다.");
		return;
	}

	//Get a new workbook.
	books = app.GetWorkbooks();
	book = books.Add (covOptional);
	
	//Get the first sheet.
	sheets =book.GetSheets();
	sheet = sheets.GetItem(COleVariant((short)1));
	
	//Fill cells A1, B1, C1, and D1 one cell at a time with "headers".
	range = sheet.GetRange(COleVariant("A1"),COleVariant("A1"));
	range.SetValue2(COleVariant("시  간"));
	range = sheet.GetRange(COleVariant("B1"),COleVariant("B1"));
	range.SetValue2(COleVariant("제    목"));
	range = sheet.GetRange(COleVariant("C1"),COleVariant("C1"));
	range.SetValue2(COleVariant("종목코드"));
	range = sheet.GetRange(COleVariant("D1"),COleVariant("D1"));
	range.SetValue2(COleVariant("종 목 명"));
	
	
	//Format A1:D1 as bold, vertical alignment = center.
	range = sheet.GetRange(COleVariant("A1"), COleVariant("D1"));
	font = range.GetFont();
	font.SetBold(covTrue);
	interior = range.GetInterior();
    interior.SetColorIndex(COleVariant((short)36));
	range.SetVerticalAlignment(COleVariant((short)-4108));   //xlVAlignCenter = -4108
	
	//Fill A2:D% with an array of values (First & Last Names).
	size = m_FilterResultListCtrl.GetItemCount();
	if (size > 0)
	{
		CString idx;
		for (ii =0; ii < size; ii++)
		{
			time = m_FilterResultListCtrl.GetItemText(ii, 0);
			title = m_FilterResultListCtrl.GetItemText(ii, 1);
			code = m_FilterResultListCtrl.GetItemText(ii, 3);
			hname =m_FilterResultListCtrl.GetItemText(ii, 5);
			
			idx.Format("%d", ii+2);
			range = sheet.GetRange(COleVariant("A"+idx),COleVariant("A"+idx)); range.SetValue2(COleVariant(time));
			range = sheet.GetRange(COleVariant("B"+idx),COleVariant("B"+idx)); range.SetValue2(COleVariant(title));
			range = sheet.GetRange(COleVariant("C"+idx),COleVariant("C"+idx)); range.SetValue2(COleVariant(code));
			range = sheet.GetRange(COleVariant("D"+idx),COleVariant("D"+idx)); range.SetValue2(COleVariant(hname));

		}      
		range = sheet.GetRange(COleVariant("A1"), COleVariant("D"+idx));
		cols = range.GetEntireColumn();
		cols.AutoFit();
	}
	borders = range.GetBorders();
    borders.SetWeight(COleVariant((short)2));   //xlThin = 2

	Border bottomborder;
	bottomborder = borders.GetItem((long)9);
	bottomborder.SetLineStyle(COleVariant((short)-4119));   //xlDouble = -4119
	bottomborder.SetWeight(COleVariant((short)4));       //xlThick = 4

	app.SetVisible(TRUE);
    app.SetUserControl(TRUE);
}

void CMarketDlg::Excute(CString fname)
{
	::ShellExecute(NULL, "open", (LPCTSTR)fname, NULL, NULL, SW_SHOWNORMAL); 
}

void CMarketDlg::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
	const int nGep = 7;
	CRect rc;
	CRect rcRount;
	GetWindowRect(rc);
	ScreenToClient(rc);
	dc.FillSolidRect(&rc, m_clrBk);

	rc.top = 4;
	rc.bottom -= nGep;
	rc.left += nGep;
	rc.right -= nGep;
	dc.FillSolidRect(&rc, m_clrWriteBk);
	DrawRoundBitmap(&dc, rc, ROUND_PANEL);

	rcRount = rc;
	rcRount.top = 23;
	rcRount.left = 10;
	rcRount.right = rc.right - nGep;
	if(m_nSelIndex == cFILLER_RESULT)
		rcRount.bottom -= nGep;
	else
		rcRount.bottom -= 30;

	dc.FillSolidRect(&rcRount, m_clrRoundBk);
	// Do not call CDialog::OnPaint() for painting messages

	if(m_pTab->GetCurSel() == cKEYWORD_SET)
		draw_groupBox(&dc, IDC_KEY_GROUP2);
	else if(m_pTab->GetCurSel() == cFILTER_SET)
	{
		draw_groupBox(&dc, IDC_FILTER_STATIC);
		draw_groupBox(&dc, IDC_FILTER_STATIC2);
	}

	dc.DeleteDC();
}

CString CMarketDlg::GetPalette()
{
	char buff[10];
	CString strPal;
	CString file;
	file.Format("%s\\%s\\%s", m_szRootDir, "tab", "palette.ini");
	
	GetPrivateProfileString("General", "Palette", "", buff, sizeof(buff), file);
	strPal = buff;
	
	return strPal;
}

void CMarketDlg::DrawRoundBitmap(CDC *pDC, CRect rc, int nKind)
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

void CMarketDlg::DrawBitmap(CDC *pDC, CRect rc, CString strImage)
{
	CString strPath = Variant(homeCC, "");
	CString sImageDir = strPath + "\\image\\" + strImage;
	
	HBITMAP BitMap = (HBITMAP)::LoadImage(AfxGetInstanceHandle(), sImageDir, IMAGE_BITMAP,0,0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
	CDC		memDC;
	
	memDC.CreateCompatibleDC(pDC);
	
	if (memDC.m_hDC != NULL) 
		memDC.SelectObject(BitMap);
	
	if (memDC.m_hDC != NULL)
		::TransparentBlt(pDC->m_hDC, rc.left, rc.top, rc.Width(), rc.Height(), memDC.m_hDC, 0, 0, rc.Width(), rc.Height(), 0xFF00FF);
	
	DeleteObject(BitMap);
	memDC.DeleteDC();
}

CString CMarketDlg::Variant(int comm, CString data)
{
	CString retvalue;
	const char* dta = (char*)m_pAxWnd->SendMessage(WM_USER, MAKEWPARAM(variantDLL, comm), (LPARAM)(LPCTSTR)data);
	
	if ((long)dta > 1)
		retvalue = dta;
	else
		return "";
	
	return retvalue;
}

HBRUSH CMarketDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CWnd::OnCtlColor(pDC, pWnd, nCtlColor);
	pDC->SetBkMode(TRANSPARENT);
	
	if( pWnd->GetDlgCtrlID() == IDC_KEY_LIST		||
		pWnd->GetDlgCtrlID() == IDC_KEY_STEPLIST	||
		pWnd->GetDlgCtrlID() == IDC_KEY_EDIT		||
		pWnd->GetDlgCtrlID() == IDC_KEY_ADDCOMBO	||
		pWnd->GetDlgCtrlID() == IDC_KEY_STEPCOMBO	||
		pWnd->GetDlgCtrlID() == IDC_FILTER_EDIT1	||
		pWnd->GetDlgCtrlID() == IDC_FILTER_EDIT2	||
		pWnd->GetDlgCtrlID() == IDC_FILTER_EDIT3	||
		pWnd->GetDlgCtrlID() == IDC_FILTER_EDIT4	||
		pWnd->GetDlgCtrlID() == IDC_FILTER_EDIT5	||
		pWnd->GetDlgCtrlID() == IDC_FILTER_EDIT6	||
		pWnd->GetDlgCtrlID() == IDC_FILTER_EDIT7	||
		pWnd->GetDlgCtrlID() == IDC_FILTER_EDIT8	||
		pWnd->GetDlgCtrlID() == IDC_FILTER_EDIT9	||
		pWnd->GetDlgCtrlID() == IDC_FILTER_EDIT10	||
		pWnd->GetDlgCtrlID() == IDC_FILTER_EDIT11	||
		pWnd->GetDlgCtrlID() == IDC_FILTER_EDIT12	||
		pWnd->GetDlgCtrlID() == IDC_FILTER_EDIT13	||
		pWnd->GetDlgCtrlID() == IDC_FILTER_EDIT14	||
		pWnd->GetDlgCtrlID() == IDC_FILTER_EDIT15	||
		pWnd->GetDlgCtrlID() == IDC_FILTER_EDIT16	||
		pWnd->GetDlgCtrlID() == IDC_FILTER_EDIT17	||
		pWnd->GetDlgCtrlID() == IDC_FILTER_EDIT18	||
		pWnd->GetDlgCtrlID() == IDC_FILTER_EDIT19	||
		pWnd->GetDlgCtrlID() == IDC_FILTER_EDIT20	||
		pWnd->GetDlgCtrlID() == IDC_MARKET_TREE	)
		return hbr;
	
	if( pWnd->GetDlgCtrlID() == IDC_KEY_WORDSTATIC		||
		pWnd->GetDlgCtrlID() == IDC_KEY_LAYERSTATIC		||
		pWnd->GetDlgCtrlID() == IDC_KEY_COLORSTATIC		||
		pWnd->GetDlgCtrlID() == IDC_FILTER_USECHECK		||
		pWnd->GetDlgCtrlID() == IDC_FILTER_SOUNDCHECK	||
		pWnd->GetDlgCtrlID() == IDC_FILTER_STATIC		||
		pWnd->GetDlgCtrlID() == IDC_FILTER_STATIC2		||
		pWnd->GetDlgCtrlID() == IDC_FILTER_CHECK1		||
		pWnd->GetDlgCtrlID() == IDC_FILTER_CHECK2		||
		pWnd->GetDlgCtrlID() == IDC_FILTER_CHECK3		||
		pWnd->GetDlgCtrlID() == IDC_FILTER_CHECK4		||
		pWnd->GetDlgCtrlID() == IDC_FILTER_CHECK5		||
		pWnd->GetDlgCtrlID() == IDC_FILTER_CHECK6		||
		pWnd->GetDlgCtrlID() == IDC_FILTER_CHECK7		||
		pWnd->GetDlgCtrlID() == IDC_FILTER_CHECK8		||
		pWnd->GetDlgCtrlID() == IDC_FILTER_CHECK9		||
		pWnd->GetDlgCtrlID() == IDC_FILTER_CHECK10		||
		pWnd->GetDlgCtrlID() == IDC_FILTER_CHECK11		||
		pWnd->GetDlgCtrlID() == IDC_FILTER_CHECK12		||
		pWnd->GetDlgCtrlID() == IDC_FILTER_CHECK13		||
		pWnd->GetDlgCtrlID() == IDC_FILTER_CHECK14		||
		pWnd->GetDlgCtrlID() == IDC_FILTER_CHECK15		||
		pWnd->GetDlgCtrlID() == IDC_FILTER_CHECK16		||
		pWnd->GetDlgCtrlID() == IDC_FILTER_CHECK17		||
		pWnd->GetDlgCtrlID() == IDC_FILTER_CHECK18		||
		pWnd->GetDlgCtrlID() == IDC_FILTER_CHECK19		||
		pWnd->GetDlgCtrlID() == IDC_FILTER_CHECK20
		)
		return m_hRoundBk;

	// TODO: Return a different brush if the default is not desired
	return hbr;
}

void CMarketDlg::OnMarketcancelBtn() 
{
	// TODO: Add your control notification handler code here
	CDialog::OnCancel();
}

void CMarketDlg::draw_groupBox(CPaintDC *pdc, int ids)
{
	pdc->SelectObject(m_pFont);
	CRect rc, out;
	GetDlgItem(ids)->GetClientRect(rc);
	GetDlgItem(ids)->GetWindowRect(out);
	ScreenToClient(out);

	rc.top+=6;
	CPen pn_in, pn_out;
	pn_in.CreatePen(PS_SOLID, 1, GetAxColor(38));
	pn_out.CreatePen(PS_SOLID, 1, GetAxColor(76));
	
	rc.OffsetRect(out.left, out.top); 
	rc.OffsetRect(1,1);
	
	pdc->SelectObject(pn_out);
	pdc->MoveTo(rc.left, rc.top); pdc->LineTo(rc.right, rc.top); pdc->LineTo(rc.right, rc.bottom);
	pdc->LineTo(rc.left, rc.bottom); pdc->LineTo(rc.left, rc.top);
	rc.OffsetRect(-1,-1);
	pdc->SelectObject(pn_in);
	pdc->MoveTo(rc.left, rc.top); pdc->LineTo(rc.right, rc.top); pdc->LineTo(rc.right, rc.bottom);
	pdc->LineTo(rc.left, rc.bottom); pdc->LineTo(rc.left, rc.top);

	CString data;
	GetDlgItem(ids)->GetWindowText(data);
	pdc->TextOut(rc.left+7, rc.top-5, data);
} 

void CMarketDlg::OnFilterSoundcheck() 
{
	// TODO: Add your control notification handler code here	
	CString strINI(_T(""));
	
	strINI.Format("%s\\User\\%s\\userconf.ini", m_szRootDir, m_szUser);
	if(m_FilterSoundCkeckBbtn.GetCheck() == BST_CHECKED )
		WriteConfigFileInt(strINI, szFilterMapName, "SOUND", 1);
	else
		WriteConfigFileInt(strINI, szFilterMapName, "SOUND", 0);
}

void CMarketDlg::OnBtnPlay() 
{
	// TODO: Add your control notification handler code here		
	char	readB[256] = _T("");
	int readL = 0;

	CString strINI;
	strINI.Format("%s\\User\\%s\\userconf.ini", m_szRootDir, m_szUser);
	readL = GetPrivateProfileString(szFilterMapName, "SOUNDFILE", "0", readB, sizeof(readB), strINI);

	CString strPlay;
	if( readL == 0 || readL < 15 ) 
	{
		CString strWrite;
		strWrite.Format( "%s\\image\\뉴스포착01.WAV", m_szRootDir );
		WritePrivateProfileString( szFilterMapName, "SOUNDFILE", (LPTSTR)(LPCTSTR)strWrite, strINI);

		strPlay = strWrite;
	} 
	else
		strPlay = CString(readB, readL);		
	
	sndPlaySound( strPlay, SND_ASYNC );
}
 
void CMarketDlg::OnBtnPath() 
{
	// TODO: Add your control notification handler code here
	char	readB[256] = _T("");
	int readL = 0;

	CString strINI;
	strINI.Format("%s\\User\\%s\\userconf.ini", m_szRootDir, m_szUser);
	readL = GetPrivateProfileString(szFilterMapName, "SOUNDFILE", "0", readB, sizeof(readB), strINI);

	CString strPlay;
	if( readL == 0 || readL < 15 )
	{
		CString strWrite;
		strWrite.Format( "%s\\image\\뉴스포착01.WAV", m_szRootDir );
		WritePrivateProfileString( szFilterMapName, "SOUNDFILE", (LPTSTR)(LPCTSTR)strWrite, strINI);

		strPlay = strWrite;
	}
	else
		strPlay = CString(readB, readL);	

	CString strFilter = _T("Wave Files (*.wav)|*.wav|");   

	CFileDialog file( TRUE, NULL, strPlay, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, strFilter );
	if( file.DoModal() == IDOK )
	{
		m_editSndFile.SetWindowText( file.GetFileName() );

		CString strINI;
		strINI.Format("%s\\User\\%s\\userconf.ini", m_szRootDir, m_szUser);

		const CString sData = file.GetPathName();
		WritePrivateProfileString( szFilterMapName, "SOUNDFILE", (LPTSTR)(LPCTSTR)sData, strINI);
	}
	
}

void CMarketDlg::OnEditchangeKeyAddcombo() 
{
	// TODO: Add your control notification handler code here
	
}
/*
void CMarketDlg::OnRclickFilterresultList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	CPoint	point; 
	int index = 0;
	const int userBASE=100;

	GetCursorPos(&point);
	
	CMenu* popM = new CMenu(); 
	popM->CreatePopupMenu();
	popM->AppendMenu(MF_STRING, userBASE+0,  "엑셀로 보내기");
	index = popM->TrackPopupMenu(TPM_LEFTALIGN|TPM_RETURNCMD, point.x, point.y, this);
	
	switch (index)
	{
		case userBASE+0:
			
	}
	delete popM;
	*pResult = 0;
}
*/


void CMarketDlg::OnFilterCheck10() 
{
	// TODO: Add your control notification handler code here
	OnFilterCheck(&m_FilterCheck10, &m_FilterEdit10);
}

void CMarketDlg::OnFilterCheck11() 
{
	// TODO: Add your control notification handler code here
	OnFilterCheck(&m_FilterCheck11, &m_FilterEdit11);
}

void CMarketDlg::OnFilterCheck12() 
{
	// TODO: Add your control notification handler code here
	OnFilterCheck(&m_FilterCheck12, &m_FilterEdit12);
}

void CMarketDlg::OnFilterCheck13() 
{
	// TODO: Add your control notification handler code here
	OnFilterCheck(&m_FilterCheck13, &m_FilterEdit13);
}

void CMarketDlg::OnFilterCheck14() 
{
	// TODO: Add your control notification handler code here
	OnFilterCheck(&m_FilterCheck14, &m_FilterEdit14);
}

void CMarketDlg::OnFilterCheck15() 
{
	// TODO: Add your control notification handler code here
	OnFilterCheck(&m_FilterCheck15, &m_FilterEdit15);
}

void CMarketDlg::OnFilterCheck16() 
{
	// TODO: Add your control notification handler code here
	OnFilterCheck(&m_FilterCheck16, &m_FilterEdit16);
}

void CMarketDlg::OnFilterCheck17() 
{
	// TODO: Add your control notification handler code here
	OnFilterCheck(&m_FilterCheck17, &m_FilterEdit17);
}

void CMarketDlg::OnFilterCheck18() 
{
	// TODO: Add your control notification handler code here
	OnFilterCheck(&m_FilterCheck18, &m_FilterEdit18);
}

void CMarketDlg::OnFilterCheck19() 
{
	// TODO: Add your control notification handler code here
	OnFilterCheck(&m_FilterCheck19, &m_FilterEdit19);
}

void CMarketDlg::OnFilterCheck20() 
{
	// TODO: Add your control notification handler code here
	OnFilterCheck(&m_FilterCheck20, &m_FilterEdit20);
}

//===========================================================================================================
CString CMarketDlg::GetAxVariant(int cmd, CString data)
{
	if( !m_pAxWnd )
		return "";
	
	const char* dta = (char*)m_pAxWnd->SendMessage(WM_USER, 
		MAKEWPARAM(variantDLL, cmd), (LPARAM)(LPCTSTR)data);
	
	if ((long)dta > 1)	return dta;
	else			return "";
}

CString CMarketDlg::GetAxCode(int cmd,LPARAM lParam)
{
	if( !m_pAxWnd )
		return "";
	
	const char* dta = (char*)m_pAxWnd->SendMessage(WM_USER, MAKEWPARAM(codeDLL, MAKEWORD(cmd, NULL)), lParam);
	if ((long)dta > 1)	return dta;
	else			return "";
}

