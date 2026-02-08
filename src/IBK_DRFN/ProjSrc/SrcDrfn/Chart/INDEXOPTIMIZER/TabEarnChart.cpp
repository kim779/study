// TabEarnChart.cpp : implementation file
//

#include "stdafx.h"
#include "TabEarnChart.h"

#include "../Common_ST/IStrategyItem.h"		// for EXITSHORT
#include ".\tabearnchart.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTabSoonikChart dialog


CTabSoonikChart::CTabSoonikChart(CWnd* pParent /*=NULL*/)
	: CDlgInterface(CTabSoonikChart::IDD, pParent)
	{
	//{{AFX_DATA_INIT(CTabSoonikChart)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_brBackground.CreateSolidBrush( CLR_BACKGROUND);
}

CTabSoonikChart::~CTabSoonikChart()						// standard destructor
{
	m_brBackground.DeleteObject();
}

void CTabSoonikChart::DoDataExchange(CDataExchange* pDX)
{
	CDlgInterface::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTabSoonikChart)
	DDX_Control(pDX, IDC_MSCHART1, m_ctlLineMsChart);		// accumulation earning rate
	DDX_Control(pDX, IDC_TCHART1, m_ctlBarTChart);			//Gunbyul
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CTabSoonikChart, CDlgInterface)
	//{{AFX_MSG_MAP(CTabSoonikChart)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_RADIO1, OnBnClickedRadio1)
	ON_BN_CLICKED(IDC_RADIO2, OnBnClickedRadio2)	
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTabSoonikChart message handlers
//-----------------------------------------------------------------------------
HBRUSH CTabSoonikChart::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CDlgInterface::OnCtlColor(pDC, pWnd, nCtlColor);
	
	pDC->SetBkColor( RGB(255,255,255));//CLR_BACKGROUND;
	return m_brBackground;
}

void CTabSoonikChart::OnOK() 
{
//	CDlgInterface::OnOK();
}

void CTabSoonikChart::OnCancel() 
{
//	CDlgInterface::OnCancel();
}

BOOL CTabSoonikChart::OnInitDialog() 
{
	CDlgInterface::OnInitDialog();

	m_ctlLineMsChart.put_ColumnCount(1);
	m_ctlLineMsChart.put_RowCount(0);

	CheckRadioButton(IDC_RADIO1, IDC_RADIO2, IDC_RADIO1);
	OnBnClickedRadio1();

	return TRUE;  // return TRUE unless you set the focus to a control									
	              // EXCEPTION: OCX Property Pages should return FALSE
}



//-----------------------------------------------------------------------------
void CTabSoonikChart::OnBnClickedRadio1()
{
	m_ctlLineMsChart.ShowWindow(SW_HIDE);
	m_ctlBarTChart.ShowWindow(SW_SHOW);	

}

void CTabSoonikChart::OnBnClickedRadio2()
{
	m_ctlLineMsChart.ShowWindow(SW_SHOW);
	m_ctlBarTChart.ShowWindow(SW_HIDE);
}



/* ---------------------------------------------------------------------------------------
void CTabSoonikChart::ShowData2Chart(CStringArray &stNujukArray, int nShowRow)
작성 :	서정현 06.09.30 휴일근무
설명 :	조회 후 처음바차트를 보여주는 처리를 한다.
		그리고 데이터가 1개 일경우엔 바차트만 보이도록 처리한다.
--------------------------------------------------------------------------------------- */
void CTabSoonikChart::PrepareShow(int nShowCount)
{
	GetDlgItem(IDC_RADIO1)->EnableWindow(TRUE);
	GetDlgItem(IDC_RADIO2)->EnableWindow(TRUE);	

	if(nShowCount<=1)
	{	
		CheckRadioButton(IDC_RADIO1, IDC_RADIO2, IDC_RADIO1);
		m_ctlBarTChart.ShowWindow(SW_SHOW);
		m_ctlLineMsChart.ShowWindow(SW_HIDE);

		GetDlgItem(IDC_RADIO1)->EnableWindow(FALSE);
		GetDlgItem(IDC_RADIO2)->EnableWindow(FALSE);
	}
}
void CTabSoonikChart::ShowData1Chart(long dwValue)
{
	m_ctlBarTChart.Series(0).Clear();

	CArray<double , double> * dValue = (CArray<double,double>*)dwValue;
	COLORREF nRed, nBlue, clr;
	nRed = RGB(255, 000, 000);
	nBlue = RGB(000, 000, 255);
	
	double dwValue1;
    for (int AX = 0; AX < dValue->GetSize(); AX++){
		dwValue1 = dValue->GetAt(AX);
		clr = (dwValue1 < 0) ? nBlue : nRed;
		m_ctlBarTChart.Series(0).AddXY(AX, dwValue1, "", clr);
	}
}

/* ---------------------------------------------------------------------------------------
void CTabSoonikChart::ShowData2Chart(CStringArray &stNujukArray, int nShowRow)
작성 :	서정현 06.09.30 휴일근무
설명 :	데이터의 갯수가 한개일경우에는 아무처리도 하지 않는다.
		데이터가 한개일 경우에는 선차트를 그릴 수 없기때문임.
--------------------------------------------------------------------------------------- */
void CTabSoonikChart::ShowData2Chart(CStringArray &stNujukArray, int nShowRow)
 {
	if(stNujukArray.GetSize()==1)		return;

	//m_ctlLineMsChart.VtChChartType2dLine;			// Resource Editer에서 기본적으로 (VtChChartType2dLine으로) 세팅되어 있음.
	m_ctlLineMsChart.put_AllowSelections(FALSE);
	
	int nShowCol = 1;
	m_ctlLineMsChart.put_ColumnCount(nShowCol);
	m_ctlLineMsChart.put_RowCount(nShowRow);

	CString szTmp, szData;
	for (int i = 0; i< nShowRow; i++)	
	{
		m_ctlLineMsChart.put_Row(i+1);  
		
		szTmp.Format("%d",i+1);		
		m_ctlLineMsChart.put_RowLabel((LPCSTR)szTmp);

		//데이타 값을 넣기
		szData = stNujukArray.GetAt(i);
		szData.Remove('%');
		m_ctlLineMsChart.put_Data((LPCSTR)szData);
	}
	m_ctlLineMsChart.Refresh();		    
 }

