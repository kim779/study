// PSConfigDlg.cpp : implementation file
//

#include "stdafx.h"
#include "chartperiodsummaryaddin.h"
#include "PSConfigDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define DEFAULT_LINE_CLR	RGB(242,159,36) //주황색
/////////////////////////////////////////////////////////////////////////////
// CPSConfigDlg dialog


CPSConfigDlg::CPSConfigDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CPSConfigDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CPSConfigDlg)
	m_SDate = 0;
	m_EDate = 0;
	m_SPrice = 0.0;
	m_EPrice = 0.0;
	m_bSEPrice = FALSE;
	m_bEEPrice = FALSE;
	m_bHPrice = FALSE;
	m_bLPrice = FALSE;
	m_bMPrice = FALSE;
	m_bHAmount = FALSE;
	m_bLAmount = FALSE;
	m_bMAmount = FALSE;
	m_bPCandle = FALSE;
	m_bMCandle = FALSE;
	m_bPMRatio = FALSE;
	m_bAllChange = FALSE;
	//}}AFX_DATA_INIT

	m_nLineStyle = 0;
	m_nLineThickness = 0;
	m_nTextAlignment = 0;
	m_nCnt = 0;

}


void CPSConfigDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPSConfigDlg)
	DDX_Control(pDX, IDC_DATETIMEPICKER2, m_ctlEDate);
	DDX_Control(pDX, IDC_DATETIMEPICKER1, m_ctlSDate);
	DDX_Control(pDX, IDC_COMBO2, m_LineThickness);
	DDX_Control(pDX, IDC_COMBO1, m_LineStyle);
	DDX_Control(pDX, IDC_BUTTON1, m_ColorPicker);
	DDX_DateTimeCtrl(pDX, IDC_DATETIMEPICKER1, m_SDate);
	DDX_DateTimeCtrl(pDX, IDC_DATETIMEPICKER2, m_EDate);
	DDX_Text(pDX, IDC_EDIT1, m_SPrice);
	DDX_Text(pDX, IDC_EDIT2, m_EPrice);
	DDX_Check(pDX, IDC_CHECK1, m_bSEPrice);
	DDX_Check(pDX, IDC_CHECK2, m_bEEPrice);
	DDX_Check(pDX, IDC_CHECK3, m_bHPrice);
	DDX_Check(pDX, IDC_CHECK4, m_bLPrice);
	DDX_Check(pDX, IDC_CHECK5, m_bMPrice);
	DDX_Check(pDX, IDC_CHECK6, m_bHAmount);
	DDX_Check(pDX, IDC_CHECK7, m_bLAmount);
	DDX_Check(pDX, IDC_CHECK8, m_bMAmount);
	DDX_Check(pDX, IDC_CHECK9, m_bPCandle);
	DDX_Check(pDX, IDC_CHECK10, m_bMCandle);
	DDX_Check(pDX, IDC_CHECK11, m_bPMRatio);
	DDX_Check(pDX, IDC_CHECK13, m_bAllChange);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPSConfigDlg, CDialog)
	//{{AFX_MSG_MAP(CPSConfigDlg)
	ON_BN_CLICKED(IDC_RADIO1, OnTextAlignmentLeft)
	ON_BN_CLICKED(IDC_RADIO2, OnTextAlignmentRight)
	ON_BN_CLICKED(IDC_BUTTON1, OnColorPick)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPSConfigDlg message handlers

BOOL CPSConfigDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	//{{{콤보 박스들 초기화
	Initial_StyleComboBox();
	Initial_ThicknessComboBox();
	//{{{색 지정 버튼 초기화
	Initial_ColorBtn();

	
	// 데이터 표시 플래그
	if(m_bSEPrice) m_nCnt++;
	if(m_bEEPrice) m_nCnt++;
	if(m_bHPrice) m_nCnt++;
	if(m_bLPrice) m_nCnt++;
	if(m_bMPrice) m_nCnt++;
	if(m_bHAmount) m_nCnt++;
	if(m_bLAmount) m_nCnt++;
	if(m_bMAmount) m_nCnt++;
	if(m_bPCandle) m_nCnt++;
	if(m_bMCandle) m_nCnt++;
	if(m_bPMRatio) m_nCnt++;

	// 텍스트 정렬
	CButton* RadioBtn;
	switch(m_nTextAlignment)
	{
	case 0:	// 좌측
		RadioBtn = (CButton*) GetDlgItem(IDC_RADIO1);
		RadioBtn->SetCheck(1);
		break;
	case 1: // 우측
		RadioBtn = (CButton*) GetDlgItem(IDC_RADIO2);
		RadioBtn->SetCheck(1);
		break;
	}
	// 일자
	long lDate;
	int a, b, c, d, e, f;

	lDate = m_lSDate;
	a = lDate/10000;
	lDate = lDate%10000;
	b = lDate/100;
	lDate = lDate%100;
	c = lDate;

	lDate = m_lEDate;
	d = lDate/10000;
	lDate = lDate%10000;
	e = lDate/100;
	lDate = lDate%100;
	f = lDate;	
	switch(m_nDateType)
	{
	case 0:	// date
		m_ctlSDate.ModifyStyleEx(DTS_TIMEFORMAT, DTS_SHORTDATEFORMAT);
		m_ctlEDate.ModifyStyleEx(DTS_TIMEFORMAT, DTS_SHORTDATEFORMAT);
  		m_SDate = CTime(a,b,c, 0, 0, 0);
  		m_EDate = CTime(d,e,f, 0, 0, 0);
		break;
	case 1:	// time
		m_ctlSDate.ModifyStyleEx(DTS_SHORTDATEFORMAT, DTS_TIMEFORMAT);
		m_ctlEDate.ModifyStyleEx(DTS_TIMEFORMAT, DTS_SHORTDATEFORMAT);
  		m_SDate = CTime(0, 0, 0,a,b,c);
  		m_EDate = CTime(0, 0, 0,d,e,f);
		break;
	}

	m_LineStyle.SetCurSel(m_nLineStyle);
	m_LineThickness.SetCurSel(m_nLineThickness);

	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CPSConfigDlg::OnOK() 
{
	UpdateData(TRUE);

	// 데이터 표시 플래그
	m_nCnt = 0;
	if(m_bSEPrice) m_nCnt++;
	if(m_bEEPrice) m_nCnt++;
	if(m_bHPrice) m_nCnt++;
	if(m_bLPrice) m_nCnt++;
	if(m_bMPrice) m_nCnt++;
	if(m_bHAmount) m_nCnt++;
	if(m_bLAmount) m_nCnt++;
	if(m_bMAmount) m_nCnt++;
	if(m_bPCandle) m_nCnt++;
	if(m_bMCandle) m_nCnt++;
	if(m_bPMRatio) m_nCnt++;

	// 일자
	int a, b, c;
	a = m_SDate.GetYear();
	b = m_SDate.GetMonth();
	c = m_SDate.GetDay();
	m_lSDate = a*10000 + b*100 + c;
	a = m_EDate.GetYear();
	b = m_EDate.GetMonth();
	c = m_EDate.GetDay();
	m_lEDate = a*10000 + b*100 + c;

	m_nLineStyle = m_LineStyle.GetCurSel();
	m_nLineThickness = m_LineThickness.GetCurSel();

	m_ColorPicker.GetColor();
	CDialog::OnOK();
}

void CPSConfigDlg::OnCancel() 
{
	UpdateData(FALSE);
	
	CDialog::OnCancel();
}

void CPSConfigDlg::OnTextAlignmentLeft() 
{
	m_nTextAlignment = 0;
	
}

void CPSConfigDlg::OnTextAlignmentRight() 
{
	m_nTextAlignment = 1;
	
}

void CPSConfigDlg::Initial_ThicknessComboBox()
{

	CWnd* pWnd=NULL;                         
	
	pWnd = GetDlgItem(IDC_COMBO2);     
	if(pWnd)    
	{
		CBitmapItemComboBox* pBmpCombo = ((CBitmapItemComboBox*)pWnd);
		pBmpCombo->m_bitmap.LoadBitmap(IDB_LINE_THICKNESS);
		AddLineType(pBmpCombo, 5);
		
		for (int i= -1 ;i < pBmpCombo->GetCount();i++) 
			pBmpCombo->SetItemHeight(i, 14);

		pBmpCombo->SetCurSel(0);			
	}
}

void CPSConfigDlg::Initial_StyleComboBox()
{

	CWnd* pWnd=NULL;                         
	
	pWnd = GetDlgItem(IDC_COMBO1);     
	if(pWnd)    
	{
		CBitmapItemComboBox* pBmpCombo = ((CBitmapItemComboBox*)pWnd);
		pBmpCombo->m_bitmap.LoadBitmap(IDB_LINE_STYLE);
		AddLineType(pBmpCombo, 5);
		
		for (int i= -1 ;i < pBmpCombo->GetCount();i++) 
			pBmpCombo->SetItemHeight(i, 14);

		pBmpCombo->SetCurSel(0);			
	}
}
void CPSConfigDlg::AddLineType(CComboBox* pCombo, int Type)
{
	ASSERT(pCombo != NULL);
	pCombo->ResetContent();
	
	for (int i = 0; i < Type; i++)
		pCombo->AddString(szType[i]);
}

void CPSConfigDlg::Initial_ColorBtn()
{
	m_ColorPicker.SetColor(m_clrLineColor);
	
	COLORREF clr = DEFAULT_LINE_CLR;
}

void CPSConfigDlg::OnColorPick() 
{
	COLORREF crColor = m_ColorPicker.GetColor();
	CColorDialog color(crColor, CC_FULLOPEN);
	if(color.DoModal() == IDOK) 
	{
		m_ColorPicker.SetColor(color.GetColor());				
		m_ColorPicker.Invalidate();
	}
	m_clrLineColor = m_ColorPicker.GetColor();
}

HBRUSH CPSConfigDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	
	// TODO: Change any attributes of the DC here
	
	// TODO: Return a different brush if the default is not desired
	return hbr;
}
