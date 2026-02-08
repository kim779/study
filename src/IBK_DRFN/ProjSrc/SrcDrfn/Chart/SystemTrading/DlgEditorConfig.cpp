// DlgEditorConfig.cpp : implementation file
//

#include "stdafx.h"
#include "systemtrading.h"
#include "DlgEditorConfig.h"

#include "../Include_ST/DeclareDefine.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgEditorConfig dialog


CDlgEditorConfig::CDlgEditorConfig(CFont* pFont, LPCSTR lpszSection, UINT nScriptType, CWnd* pParent /*=NULL*/)
	: CDialog(CDlgEditorConfig::IDD, pParent)
	, m_pFont(pFont)
	, m_nScriptType(nScriptType)
{
	//{{AFX_DATA_INIT(CDlgEditorConfig)
	m_nTextHeight = 9;
	//}}AFX_DATA_INIT

	m_pFontView = NULL;
	::lstrcpy(m_szSection, lpszSection);

	LoadData();
}


void CDlgEditorConfig::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgEditorConfig)
	DDX_Control(pDX, IDC_BTN_USERFUNCTION, m_btnUserFunction);
	DDX_Control(pDX, IDC_BTN_SELTEXT, m_btnSelText);
	DDX_Control(pDX, IDC_BTN_SELBKGRND, m_btnSelBackGrnd);
	DDX_Control(pDX, IDC_BTN_RESERVED, m_btnReserved);
	DDX_Control(pDX, IDC_BTN_OPERATOR, m_btnOperator);
	DDX_Control(pDX, IDC_BTN_NUMBER, m_btnNumber);
	DDX_Control(pDX, IDC_BTN_NORMALTEXT, m_btnNormalText);
	DDX_Control(pDX, IDC_BTN_NOCERTFUNC, m_btnNoCertFunc);
	DDX_Control(pDX, IDC_BTN_COMMENT, m_btnComment);
	DDX_Control(pDX, IDC_BTN_BACKGROUND, m_btnBackGround);
	DDX_Control(pDX, IDC_SPIN_HEIGHT, m_spinHeight);
	DDX_Control(pDX, IDC_CMB_FONT, m_cmbFont);
	DDX_Text(pDX, IDC_EDIT_HEIGHT, m_nTextHeight);
	DDV_MinMaxUInt(pDX, m_nTextHeight, 9, 48);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CDlgEditorConfig, CDialog)
	//{{AFX_MSG_MAP(CDlgEditorConfig)
	ON_WM_DESTROY()
	ON_WM_PAINT()
	ON_CBN_SELCHANGE(IDC_CMB_FONT, OnSelchangeCmbFont)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_HEIGHT, OnDeltaposSpinHeight)
	ON_BN_CLICKED(IDC_BTN_RESET, OnBtnReset)
	ON_CPN_XT_SELENDOK(IDC_BTN_NORMALTEXT, OnSelColorButtonNormalText)
	ON_CPN_XT_SELENDOK(IDC_BTN_BACKGROUND, OnSelColorButtonBackground)
	ON_CPN_XT_SELENDOK(IDC_BTN_SELTEXT, OnSelColorButtonSelText)
	ON_CPN_XT_SELENDOK(IDC_BTN_SELBKGRND, OnSelColorButtonSelBkGrnd)
	ON_CPN_XT_SELENDOK(IDC_BTN_USERFUNCTION, OnSelColorButtonUserFunction)
	ON_CPN_XT_SELENDOK(IDC_BTN_NOCERTFUNC, OnSelColorButtonNoCertFunc)
	ON_CPN_XT_SELENDOK(IDC_BTN_COMMENT, OnSelColorButtonComment)
	ON_CPN_XT_SELENDOK(IDC_BTN_NUMBER, OnSelColorButtonNumber)
	ON_CPN_XT_SELENDOK(IDC_BTN_RESERVED, OnSelColorButtonReserved)
	ON_CPN_XT_SELENDOK(IDC_BTN_OPERATOR, OnSelColorButtonOperator)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgEditorConfig message handlers

BOOL CDlgEditorConfig::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
//	m_cmbFont.SubclassDlgItem(IDC_CMB_FONT, this);
	m_cmbFont.Initialize();
	m_cmbFont.SetFont(m_pFont);
	m_cmbFont.SetCurSel(m_cmbFont.FindString(0, m_szSavedFont));

	m_spinHeight.SetRange(9, 48);
	
	m_btnUserFunction.SetColor(m_clrUserFunction);
	m_btnSelText.SetColor(m_clrSelText);
	m_btnSelBackGrnd.SetColor(m_clrSelBkGrnd);
	m_btnReserved.SetColor(m_clrReservedText);
	m_btnOperator.SetColor(m_clrOperator);
	m_btnNumber.SetColor(m_clrNumber);
	m_btnNormalText.SetColor(m_clrNormalText);
	m_btnNoCertFunc.SetColor(m_clrNoCertFunc);
	m_btnComment.SetColor(m_clrComment);
	m_btnBackGround.SetColor(m_clrBackGround);

	m_btnUserFunction.SetDefaultColor(RGB(0, 128, 192));
	m_btnSelText.SetDefaultColor(RGB(255, 255, 255));
	m_btnSelBackGrnd.SetDefaultColor(RGB(0, 0, 0));
	m_btnReserved.SetDefaultColor(RGB(0, 0, 255));
	m_btnOperator.SetDefaultColor(RGB(0, 0, 0));
	m_btnNumber.SetDefaultColor(RGB(128, 0, 0));
	m_btnNormalText.SetDefaultColor(RGB(0, 0, 0));
	m_btnNoCertFunc.SetDefaultColor(RGB(255, 0, 0));
	m_btnComment.SetDefaultColor(RGB(0, 192, 0));
	if(m_nScriptType == ST_YESTRADER)
		m_btnBackGround.SetDefaultColor(RGB(255, 247, 238));
	else
		m_btnBackGround.SetDefaultColor(RGB(255, 255, 255));

	CreateFont();
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgEditorConfig::OnDestroy() 
{
	CDialog::OnDestroy();
	
	m_cmbFont.DestroyWindow();
	if(m_pFontView)
	{
		m_pFontView->DeleteObject();
		delete m_pFontView;
		m_pFontView = NULL;
	}
}

void CDlgEditorConfig::OnPaint() 
{
	CPaintDC dc(this); // device context for painting

	CRect rc;
	GetDlgItem(IDC_STC_VIEW)->GetWindowRect(rc);
	ScreenToClient(rc);

	dc.FillSolidRect(rc, m_clrBackGround);

	dc.SelectObject(m_pFontView);

	dc.SetTextColor(m_clrReservedText);
	dc.SetBkColor(m_clrBackGround);

	rc.top = 10;
	int nLeft = rc.left;

	// input : nValue(10);
	CString strText = _T("input");
	dc.DrawText(strText, rc, DT_SINGLELINE|DT_WORDBREAK);

	CSize sizeText = dc.GetTextExtent(strText);

	dc.SetTextColor(m_clrNormalText);
	strText = _T(" : nValue(");
	rc.left += sizeText.cx;
	dc.DrawText(strText, rc, DT_SINGLELINE|DT_WORDBREAK);
	
	sizeText = dc.GetTextExtent(strText);
	dc.SetTextColor(m_clrNumber);
	strText = _T("10");
	rc.left += sizeText.cx;
	dc.DrawText(strText, rc, DT_SINGLELINE|DT_WORDBREAK);

	sizeText = dc.GetTextExtent(strText);
	dc.SetTextColor(m_clrNormalText);
	strText = _T(");");
	rc.left += sizeText.cx;
	dc.DrawText(strText, rc, DT_SINGLELINE|DT_WORDBREAK);

	// var : bValue(False);
	rc.left = nLeft;
	rc.top += sizeText.cy;

	dc.SetTextColor(m_clrReservedText);
	strText = _T("var");
	dc.DrawText(strText, rc, DT_SINGLELINE|DT_WORDBREAK);

	sizeText = dc.GetTextExtent(strText);
	dc.SetTextColor(m_clrNormalText);
	strText = _T(" : bValue(");
	rc.left += sizeText.cx;
	dc.DrawText(strText, rc, DT_SINGLELINE|DT_WORDBREAK);

	sizeText = dc.GetTextExtent(strText);
	dc.SetTextColor(m_clrReservedText);
	strText = _T("False");
	rc.left += sizeText.cx;
	dc.DrawText(strText, rc, DT_SINGLELINE|DT_WORDBREAK);

	sizeText = dc.GetTextExtent(strText);
	dc.SetTextColor(m_clrNormalText);
	strText = _T(");");
	rc.left += sizeText.cx;
	dc.DrawText(strText, rc, DT_SINGLELINE|DT_WORDBREAK);

	// // 미리보기 입니다.
	rc.left = nLeft;
	rc.top += sizeText.cy * 2;

	dc.SetTextColor(m_clrComment);
	strText = _T("// 미리보기 입니다.");
	dc.DrawText(strText, rc, DT_SINGLELINE|DT_WORDBREAK);

	// bValue = 미검증함수(nValue);
	rc.left = nLeft;
	rc.top += sizeText.cy * 2;

	dc.SetTextColor(m_clrNormalText);
	strText = _T("bValue");
	dc.DrawText(strText, rc, DT_SINGLELINE|DT_WORDBREAK);

	sizeText = dc.GetTextExtent(strText);
	dc.SetTextColor(m_clrOperator);
	strText = _T(" = ");
	rc.left += sizeText.cx;
	dc.DrawText(strText, rc, DT_SINGLELINE|DT_WORDBREAK);

	sizeText = dc.GetTextExtent(strText);
	dc.SetTextColor(m_clrNoCertFunc);
	strText = _T("미검증함수");
	rc.left += sizeText.cx;
	dc.DrawText(strText, rc, DT_SINGLELINE|DT_WORDBREAK);

	sizeText = dc.GetTextExtent(strText);
	dc.SetTextColor(m_clrNormalText);
	strText = _T("(nValue);");
	rc.left += sizeText.cx;
	dc.DrawText(strText, rc, DT_SINGLELINE|DT_WORDBREAK);

	// if(bValue) then begin
	rc.left = nLeft;
	rc.top += sizeText.cy;

	dc.SetTextColor(m_clrReservedText);
	strText = _T("if");
	dc.DrawText(strText, rc, DT_SINGLELINE|DT_WORDBREAK);

	sizeText = dc.GetTextExtent(strText);
	dc.SetTextColor(m_clrNormalText);
	strText = _T("(bValue)");
	rc.left += sizeText.cx;
	dc.DrawText(strText, rc, DT_SINGLELINE|DT_WORDBREAK);

	sizeText = dc.GetTextExtent(strText);
	dc.SetTextColor(m_clrReservedText);
	strText = _T(" then begin");
	rc.left += sizeText.cx;
	dc.DrawText(strText, rc, DT_SINGLELINE|DT_WORDBREAK);

	//     검증된함수(nValue);
	rc.left = nLeft;
	rc.top += sizeText.cy;

	dc.SetTextColor(m_clrUserFunction);
	strText = _T("    검증된함수");
	dc.DrawText(strText, rc, DT_SINGLELINE|DT_WORDBREAK);

	sizeText = dc.GetTextExtent(strText);
	dc.SetTextColor(m_clrNormalText);
	strText = _T("(nValue);");
	rc.left += sizeText.cx;
	dc.DrawText(strText, rc, DT_SINGLELINE|DT_WORDBREAK);

	// end;
	rc.left = nLeft;
	rc.top += sizeText.cy;

	dc.SetTextColor(m_clrReservedText);
	strText = _T("end");
	dc.DrawText(strText, rc, DT_SINGLELINE|DT_WORDBREAK);

	sizeText = dc.GetTextExtent(strText);
	dc.SetTextColor(m_clrNormalText);
	strText = _T(";");
	rc.left += sizeText.cx;
	dc.DrawText(strText, rc, DT_SINGLELINE|DT_WORDBREAK);

	// 선택된 문자열 보기
	rc.left = nLeft;
	rc.top += sizeText.cy * 2;

	dc.SetTextColor(m_clrSelText);
	dc.SetBkColor(m_clrSelBkGrnd);
	strText = _T("선택된 문자열 보기");
	dc.DrawText(strText, rc, DT_SINGLELINE|DT_WORDBREAK);
}

void CDlgEditorConfig::OnSelchangeCmbFont() 
{
	int nSel = m_cmbFont.GetCurSel();
	m_cmbFont.GetLBText(nSel, m_szSavedFont);

	CreateFont();
}

void CDlgEditorConfig::OnDeltaposSpinHeight(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
	
	if(m_nTextHeight == 9 && pNMUpDown->iDelta < 0)
		return;
	else if(m_nTextHeight == 48 && pNMUpDown->iDelta > 0)
		return;
	
	m_nTextHeight += pNMUpDown->iDelta;

	CreateFont();

	*pResult = 0;
}

void CDlgEditorConfig::CreateFont()
{
	int nSel = m_cmbFont.GetCurSel();
	if(nSel == CB_ERR)
	{
		m_cmbFont.SetCurSel(m_cmbFont.FindString(0, m_szSavedFont));
		return;
	}

	if(m_pFontView)
		m_pFontView->DeleteObject();
	else
		m_pFontView = new CFont;

	int x = 0;
	m_pFontView->CreateFont(m_nTextHeight, x, 0, 0, FW_NORMAL, FALSE, FALSE, 0,
			ANSI_CHARSET, OUT_DEFAULT_PRECIS, 
			CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
			DEFAULT_PITCH | FF_DONTCARE, m_szSavedFont);

	CRect rc;
	GetDlgItem(IDC_STC_VIEW)->GetWindowRect(rc);
	ScreenToClient(rc);

	InvalidateRect(rc);
}

void CDlgEditorConfig::LoadData()
{
	if(m_nScriptType == ST_TRADESTATION)
		SetDefaultData();
	else if(m_nScriptType == ST_YESTRADER)
		SetDefaultData_YES();
	
	m_clrBackGround = ::GetProfileInt(m_szSection, _T("BackGround"), m_clrBackGround);
	m_clrComment = ::GetProfileInt(m_szSection, _T("Comment"), m_clrComment);
	m_clrNoCertFunc = ::GetProfileInt(m_szSection, _T("NoCertFunc"), m_clrNoCertFunc);
	m_clrNormalText = ::GetProfileInt(m_szSection, _T("NormalText"), m_clrNormalText);
	m_clrNumber = ::GetProfileInt(m_szSection, _T("Number"), m_clrNumber);
	m_clrOperator = ::GetProfileInt(m_szSection, _T("Operator"), m_clrOperator);
	m_clrReservedText = ::GetProfileInt(m_szSection, _T("ReservedText"), m_clrReservedText);
	m_clrSelBkGrnd = ::GetProfileInt(m_szSection, _T("SelBkGrnd"), m_clrSelBkGrnd);
	m_clrSelText = ::GetProfileInt(m_szSection, _T("SelText"), m_clrSelText);
	m_clrUserFunction = ::GetProfileInt(m_szSection, _T("UserFunction"), m_clrUserFunction);
	m_nTextHeight = ::GetProfileInt(m_szSection, _T("TextHeight"), m_nTextHeight);
	::GetProfileString(m_szSection, _T("SavedFont"), m_szSavedFont, m_szSavedFont, sizeof(m_szSavedFont));
}

void CDlgEditorConfig::SaveData()
{
	char szData[10];

//	::memset(szData, 0x00, sizeof(szData));
	::wsprintf(szData, _T("%d"), m_clrBackGround);
	::WriteProfileString(m_szSection, _T("BackGround"), szData);
	::wsprintf(szData, _T("%d"), m_clrComment);
	::WriteProfileString(m_szSection, _T("Comment"), szData);
	::wsprintf(szData, _T("%d"), m_clrNoCertFunc);
	::WriteProfileString(m_szSection, _T("NoCertFunc"), szData);
	::wsprintf(szData, _T("%d"), m_clrNormalText);
	::WriteProfileString(m_szSection, _T("NormalText"), szData);
	::wsprintf(szData, _T("%d"), m_clrNumber);
	::WriteProfileString(m_szSection, _T("Number"), szData);
	::wsprintf(szData, _T("%d"), m_clrOperator);
	::WriteProfileString(m_szSection, _T("Operator"), szData);
	::wsprintf(szData, _T("%d"), m_clrReservedText);
	::WriteProfileString(m_szSection, _T("ReservedText"), szData);
	::wsprintf(szData, _T("%d"), m_clrSelBkGrnd);
	::WriteProfileString(m_szSection, _T("SelBkGrnd"), szData);
	::wsprintf(szData, _T("%d"), m_clrSelText);
	::WriteProfileString(m_szSection, _T("SelText"), szData);
	::wsprintf(szData, _T("%d"), m_clrUserFunction);
	::WriteProfileString(m_szSection, _T("UserFunction"), szData);
	::wsprintf(szData, _T("%d"), m_nTextHeight);
	::WriteProfileString(m_szSection, _T("TextHeight"), szData);
	::WriteProfileString(m_szSection, _T("SavedFont"), m_szSavedFont);
}

void CDlgEditorConfig::SetDefaultData()
{
	m_clrBackGround = RGB(255, 255, 255);
	m_clrComment = RGB(0, 192, 0);
	m_clrNoCertFunc = RGB(255, 0, 0);
	m_clrNormalText = RGB(0, 0, 0);
	m_clrNumber = RGB(128, 0, 0);
	m_clrOperator = RGB(0, 0, 0);
	m_clrReservedText = RGB(0, 0, 255);
	m_clrSelBkGrnd = RGB(0, 0, 0);
	m_clrSelText = RGB(255, 255, 255);
	m_clrUserFunction = RGB(0, 128, 192);
	m_nTextHeight = 14;

	::lstrcpy(m_szSavedFont, _T("굴림체"));
}

void CDlgEditorConfig::SetDefaultData_YES()
{
	m_clrBackGround = RGB(255, 247, 238);
	m_clrComment = RGB(0, 192, 0);
	m_clrNoCertFunc = RGB(255, 0, 0);
	m_clrNormalText = RGB(0, 0, 0);
	m_clrNumber = RGB(128, 0, 0);
	m_clrOperator = RGB(0, 0, 0);
	m_clrReservedText = RGB(0, 0, 255);
	m_clrSelBkGrnd = RGB(0, 0, 0);
	m_clrSelText = RGB(255, 255, 255);
	m_clrUserFunction = RGB(0, 128, 192);
	m_nTextHeight = 14;

	::lstrcpy(m_szSavedFont, _T("굴림체"));

}

void CDlgEditorConfig::OnSelColorButtonNormalText()
{
	m_clrNormalText = m_btnNormalText.GetColor();

	CRect rc;
	GetDlgItem(IDC_STC_VIEW)->GetWindowRect(rc);
	ScreenToClient(rc);

	InvalidateRect(rc);
}

void CDlgEditorConfig::OnSelColorButtonBackground()
{
	m_clrBackGround = m_btnBackGround.GetColor();

	CRect rc;
	GetDlgItem(IDC_STC_VIEW)->GetWindowRect(rc);
	ScreenToClient(rc);

	InvalidateRect(rc);
}

void CDlgEditorConfig::OnSelColorButtonSelText()
{
	m_clrSelText = m_btnSelText.GetColor();

	CRect rc;
	GetDlgItem(IDC_STC_VIEW)->GetWindowRect(rc);
	ScreenToClient(rc);

	InvalidateRect(rc);
}

void CDlgEditorConfig::OnSelColorButtonSelBkGrnd()
{
	m_clrSelBkGrnd = m_btnSelBackGrnd.GetColor();

	CRect rc;
	GetDlgItem(IDC_STC_VIEW)->GetWindowRect(rc);
	ScreenToClient(rc);

	InvalidateRect(rc);
}

void CDlgEditorConfig::OnSelColorButtonUserFunction()
{
	m_clrUserFunction = m_btnUserFunction.GetColor();

	CRect rc;
	GetDlgItem(IDC_STC_VIEW)->GetWindowRect(rc);
	ScreenToClient(rc);

	InvalidateRect(rc);
}

void CDlgEditorConfig::OnSelColorButtonNoCertFunc()
{
	m_clrNoCertFunc = m_btnNoCertFunc.GetColor();

	CRect rc;
	GetDlgItem(IDC_STC_VIEW)->GetWindowRect(rc);
	ScreenToClient(rc);

	InvalidateRect(rc);
}

void CDlgEditorConfig::OnSelColorButtonComment()
{
	m_clrComment = m_btnComment.GetColor();

	CRect rc;
	GetDlgItem(IDC_STC_VIEW)->GetWindowRect(rc);
	ScreenToClient(rc);

	InvalidateRect(rc);
}

void CDlgEditorConfig::OnSelColorButtonNumber()
{
	m_clrNumber = m_btnNumber.GetColor();

	CRect rc;
	GetDlgItem(IDC_STC_VIEW)->GetWindowRect(rc);
	ScreenToClient(rc);

	InvalidateRect(rc);
}

void CDlgEditorConfig::OnSelColorButtonReserved()
{
	m_clrReservedText = m_btnReserved.GetColor();

	CRect rc;
	GetDlgItem(IDC_STC_VIEW)->GetWindowRect(rc);
	ScreenToClient(rc);

	InvalidateRect(rc);
}

void CDlgEditorConfig::OnSelColorButtonOperator()
{
	m_clrOperator = m_btnOperator.GetColor();

	CRect rc;
	GetDlgItem(IDC_STC_VIEW)->GetWindowRect(rc);
	ScreenToClient(rc);

	InvalidateRect(rc);
}

void CDlgEditorConfig::OnBtnReset() 
{
	if(m_nScriptType == ST_TRADESTATION)
		SetDefaultData();
	else if(m_nScriptType == ST_YESTRADER)
		SetDefaultData_YES();

	UpdateData(FALSE);
	m_cmbFont.SetDefalutFont();

	m_btnUserFunction.SetColor(m_clrUserFunction);
	m_btnSelText.SetColor(m_clrSelText);
	m_btnSelBackGrnd.SetColor(m_clrSelBkGrnd);
	m_btnReserved.SetColor(m_clrReservedText);
	m_btnOperator.SetColor(m_clrOperator);
	m_btnNumber.SetColor(m_clrNumber);
	m_btnNormalText.SetColor(m_clrNormalText);
	m_btnNoCertFunc.SetColor(m_clrNoCertFunc);
	m_btnComment.SetColor(m_clrComment);
	m_btnBackGround.SetColor(m_clrBackGround);

/*
	m_btnUserFunction.Invalidate(FALSE);
	m_btnSelText.Invalidate(FALSE);
	m_btnSelBackGrnd.Invalidate(FALSE);
	m_btnReserved.Invalidate(FALSE);
	m_btnOperator.Invalidate(FALSE);
	m_btnNumber.Invalidate(FALSE);
	m_btnNormalText.Invalidate(FALSE);
	m_btnNoCertFunc.Invalidate(FALSE);
	m_btnComment.Invalidate(FALSE);
	m_btnBackGround.Invalidate(FALSE);
*/

	CreateFont();
}
