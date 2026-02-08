// DlgChart.cpp : implementation file
//

#include "stdafx.h"
#include "DlgChart_Stock.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgChart_Stock dialog


CDlgChart_Stock::CDlgChart_Stock(CWnd* pParent /*=NULL*/)
	: CRscDlg(CDlgChart_Stock::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgChart_Stock)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_pBaseDesk		= NULL;
	m_lChartInterface = NULL;
}


void CDlgChart_Stock::DoDataExchange(CDataExchange* pDX)
{
	CRscDlg::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgChart_Stock)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgChart_Stock, CRscDlg)
	//{{AFX_MSG_MAP(CDlgChart_Stock)
	ON_WM_SIZE()
	//}}AFX_MSG_MAP	
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgChart_Stock message handlers

BOOL CDlgChart_Stock::OnInitDialog() 
{
	CRscDlg::OnInitDialog();
	
	// TODO: Add extra initialization here
	CRect rect;
	GetClientRect(rect);
	rect.right = rect.right;

	m_ChartItem.Create(this, rect, 0x764);


	CString strRootPath;
//	GETPATH_APP(strRootPath);
	GetDir_DrfnRoot(strRootPath);

	BOOL bNoToolBar = FALSE;
	
	m_ChartItem.SetInit(this->GetSafeHwnd(), (long)m_pBaseDesk, strRootPath, bNoToolBar);

// 	if(m_lChartInterface)
// 		m_ChartItem.SetOutsideCommonInterface(m_lChartInterface);

	//m_ChartItem.ShowInnerToolBar(FALSE);		-->> // SetAllProperties()에서 "TO=내부\r\n" 만 사용(내부툴바 감춤)
	m_ChartItem.ChangeHorzScalePosition(CDRChartOcx::HORZ_BOTTOM, m_ChartItem.GetRowBlockCount()-1, 0);
	m_ChartItem.ChangeVertScalePosition(CDRChartOcx::VERT_RIGHT, -1, -1,-1);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgChart_Stock::OnSize(UINT nType, int cx, int cy) 
{
	CRscDlg::OnSize(nType, cx, cy);
	
	if (m_ChartItem.GetSafeHwnd())
		if(cx > 70)
			m_ChartItem.SetWindowPos(NULL, 0, 0, cx, cy, SWP_NOZORDER);
}

BOOL CDlgChart_Stock::PreTranslateMessage(MSG* pMsg) 
{
	if(pMsg->message == WM_KEYDOWN)  
	{		
		UINT nChar = (UINT)pMsg->wParam;
		switch(nChar)
		{
		case VK_RETURN :
		case VK_ESCAPE :
			{
				return FALSE;
				break;					
			}
		}
	}
	else if(pMsg->message == WM_SYSKEYDOWN)
	{
		if( ::GetKeyState(VK_MENU) & 0x8000)
			return FALSE;
	}

	if(::IsWindow(m_hWnd))
		return GetParent()->PreTranslateMessage(pMsg);

	return CRscDlg::PreTranslateMessage(pMsg);
}
