// JongStatusInTab.cpp : implementation file
//

#include "stdafx.h"
#include "JongStatusInTab.h"
//#include "./define/IntDef.h"
#include "./define/ChartMsgDef.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CJongStatusInTab dialog
CJongStatusInTab::CJongStatusInTab(CWnd* pParent /*=NULL*/)
	: CRscDlg(CJongStatusInTab::IDD, pParent)
{
	//{{AFX_DATA_INIT(CJongStatusInTab)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_lUpLimit = 0;
	m_lUp = 0;
	m_lSteady = 0;
	m_lDown = 0;
	m_lDownLimit = 0;
	m_lCnt = 0;
	m_dDaebiRate = 0;
	m_crUpLimit = RGB(233,0,0);			// 상한 
	m_crUp = RGB(237,146,149);			// 상승
	m_crSteady = RGB(191,195,200);		// 보합
	m_crDown = RGB(164,185,206);		// 하락
	m_crDownLimit = RGB(155,155,253);		// 하한
	m_crBk = CRBACK;
}


void CJongStatusInTab::DoDataExchange(CDataExchange* pDX)
{
	CRscDlg::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CJongStatusInTab)
	DDX_Control(pDX, IDC_RATE, m_stRate);
	DDX_Control(pDX, IDC_UPLIMIT, m_stUpLimit);
	DDX_Control(pDX, IDC_UP, m_stUp);
	DDX_Control(pDX, IDC_STEADY, m_stSteady);
	DDX_Control(pDX, IDC_DOWNLIMIT, m_stDownLimit);
	DDX_Control(pDX, IDC_DOWN, m_stDown);
	DDX_Control(pDX, IDC_CNT, m_stCnt);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CJongStatusInTab, CRscDlg)
	//{{AFX_MSG_MAP(CJongStatusInTab)
	ON_WM_CTLCOLOR()
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CJongStatusInTab message handlers

BOOL CJongStatusInTab::OnInitDialog() 
{
	CRscDlg::OnInitDialog();
	
	// TODO: Add extra initialization here	

	m_brBkColor.CreateSolidBrush(m_crBk);

	COLORREF clrText =	CLR_FONT_DEFAULT;		// RGB(255,255,255);
	COLORREF clrOutLine =	RGB(128,128,128);	// RGB(255,255,255);
	m_stUpLimit.SetStaticColor(clrOutLine, m_crUpLimit, clrText);
	m_stUp.SetStaticColor(clrOutLine, m_crUp, clrText);
	m_stSteady.SetStaticColor(clrOutLine, m_crSteady, clrText);
	m_stDown.SetStaticColor(clrOutLine, m_crDown, clrText);
	m_stDownLimit.SetStaticColor(clrOutLine, m_crDownLimit, clrText);


	InitStatus();

	//{{ToolTip Add by LYH 2007.01.03
	// create tooltip
	if(!m_ToolTip.Create(this, TTS_ALWAYSTIP))
		return FALSE;

	m_ToolTip.Activate(TRUE);
	m_ToolTip.AddTool(&m_stUpLimit, "상한");
	m_ToolTip.AddTool(&m_stUp, "상승");
	m_ToolTip.AddTool(&m_stSteady, "보합");
	m_ToolTip.AddTool(&m_stDown, "하락");
	m_ToolTip.AddTool(&m_stDownLimit, "하한");
	//}}
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CJongStatusInTab::SetStatus(long lUpLimit,long lUp, long lSteady, long lDown, long lDownLimit, long lCnt, double dDaebiRateTotal)
{
	m_lUpLimit = lUpLimit;
	m_lUp = lUp;
	m_lSteady = lSteady;
	m_lDown = lDown;
	m_lDownLimit = lDownLimit;
	m_lCnt = lCnt;
	m_dDaebiRate = dDaebiRateTotal;

	UpdateInCtrl();
}

void CJongStatusInTab::UpdateStatus(char chSignNew, char chSignOld, double dDaebiRateChg)
{
	switch(chSignNew)
	{
		case UP_LIMIT	:	m_lUpLimit++;		break;	// UpLimit
		case UP			:	m_lUp++;			break;	// Up		
		case EQUAL		:	m_lSteady++;		break;	// Steady
		case DOWN		:	m_lDown++;			break;	// Down
		case DOWN_LIMIT	:	m_lDownLimit++;		break;	// DownLimit		
	}
	switch(chSignOld)
	{
		case UP_LIMIT	:	m_lUpLimit--;		break;	// UpLimit
		case UP			:	m_lUp--;			break;	// Up		
		case EQUAL		:	m_lSteady--;		break;	// Steady
		case DOWN		:	m_lDown--;			break;	// Down
		case DOWN_LIMIT	:	m_lDownLimit--;		break;	// DownLimit		
	}


	m_dDaebiRate += dDaebiRateChg;
	UpdateInCtrl();
}

void CJongStatusInTab::InitStatus()
{
	m_lUpLimit = 0;
	m_lUp = 0;
	m_lSteady = 0;
	m_lDown = 0;
	m_lDownLimit = 0;
	m_lCnt = 0;
	m_dDaebiRate = 0;
	m_stUpLimit.SetText("0");
	m_stUp.SetText("0");
	m_stSteady.SetText("0");
	m_stDown.SetText("0");
	m_stDownLimit.SetText("0");
	m_stCnt.SetText("0");
	m_stRate.SetText("0%");
}


void CJongStatusInTab::UpdateInCtrl()
{
	CString strUpLimit;
	CString strUp;
	CString strSteady;
	CString strDown;
	CString strDownLimit;
	CString strCnt;
	CString strDaebiRate;
	strUpLimit.Format("%d",m_lUpLimit);
	strUp.Format("%d",m_lUp);
	strSteady.Format("%d",m_lSteady);
	strDown.Format("%d",m_lDown);
	strDownLimit.Format("%d",m_lDownLimit);
	strCnt.Format("%d",m_lCnt);
	if(m_lCnt)
	{
		strDaebiRate.Format("%+2.02f%%",m_dDaebiRate/(double)m_lCnt);
	}
	else
	{
		strDaebiRate.Format("");	
	}
	
	m_stUpLimit.SetText(strUpLimit);
	m_stUp.SetText(strUp);
	m_stSteady.SetText(strSteady);
	m_stDown.SetText(strDown);
	m_stDownLimit.SetText(strDownLimit);
	m_stCnt.SetText(strCnt);
	m_stRate.SetText(strDaebiRate);
}

HBRUSH CJongStatusInTab::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CRscDlg::OnCtlColor(pDC, pWnd, nCtlColor);
	
	// TODO: Change any attributes of the DC here
	switch(nCtlColor) {

	case CTLCOLOR_DLG:
		return m_brBkColor;
		break;
	}

	// TODO: Return a different brush if the default is not desired
	return hbr;
}

void CJongStatusInTab::OnDestroy() 
{
	CRscDlg::OnDestroy();
	
	// TODO: Add your message handler code here
	m_brBkColor.DeleteObject();	

	if (m_ToolTip.GetSafeHwnd())
	{
		m_ToolTip.DestroyWindow();
	}
}

BOOL CJongStatusInTab::PreTranslateMessage(MSG* pMsg) 
{
	if(m_ToolTip.GetSafeHwnd())
		m_ToolTip.RelayEvent(pMsg);

	return CDialog::PreTranslateMessage(pMsg);
}

