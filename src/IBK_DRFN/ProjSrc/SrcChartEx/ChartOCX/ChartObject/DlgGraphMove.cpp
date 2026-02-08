// DlgGraphMove.cpp : implementation file
//

#include "stdafx.h"
#include "chartobject.h"
#include "DlgGraphMove.h"

#include "../Include_Chart/Dll_Load/IMetaTable.h"		// for g_iMetaTable
#include "../Include_Chart/Dll_Load/IndicatorInfo.h"	// for CIndicatorInfo
#include "../Include_Chart/ScaleDataList.h"
#include "../Include_Chart/DataMath.h"
#include "../Include_Chart/IChartCtrl.h"				// for IChartCtrl
#include "../Include_Chart/IStyleManager.h"				// for IStyleManager
#include "GraphImp.h"									// for CGraphImp

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgGraphMove dialog

CDlgGraphMove::CDlgGraphMove( CWnd* pParent, CGraphImp* pSourceGraph, const CVerticalScaleGroupDataList& vertScaleGroupDataList, IChartCtrl *p_pIChartCtrl)
	: CDialog(CDlgGraphMove::IDD, pParent),
	m_pVertScaleGroupDataList(&vertScaleGroupDataList)
{
	//{{AFX_DATA_INIT(CDlgGraphMove)
	m_nScaleGroup = 0;
	m_nScalePosition = 1;
	m_nScaleGrid = 0;
	//}}AFX_DATA_INIT

	// (2009/9/21 - Seung-Won, Bae) Show Style Type on Price, Volume and MAs.
	m_pSourceGraph = pSourceGraph;
	m_pIChartCtrl = p_pIChartCtrl;
	m_pTitleFont = NULL;
}


void CDlgGraphMove::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgGraphMove)
	DDX_Control(pDX, IDC_STYLECOMBO, m_cbxStyleList);
	DDX_Control(pDX, IDC_SCALEGROUP, m_ctlScaleGroup);
	DDX_CBIndex(pDX, IDC_SCALEGROUP, m_nScaleGroup);
	DDX_Radio(pDX, IDC_SCALEPOSITION, m_nScalePosition);
	DDX_Radio(pDX, IDC_SCALEGRID, m_nScaleGrid);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgGraphMove, CDialog)
	//{{AFX_MSG_MAP(CDlgGraphMove)
	ON_CBN_SELCHANGE(IDC_SCALEGROUP, OnSelchangeScalegroup)
	ON_WM_PAINT()
	ON_WM_DESTROY()
	ON_WM_CREATE()
	ON_CBN_SELCHANGE(IDC_STYLECOMBO, OnSelchangeStylecombo)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgGraphMove message handlers

BOOL CDlgGraphMove::OnInitDialog() 
{
	CDialog::OnInitDialog();

	// Dialog에 포커스를 주기 위해 사용
	mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);
	mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);

	InitializeData();

	// (2009/9/21 - Seung-Won, Bae) Show Style Type on Price, Volume and MAs.
	if( IsStyledGraph())
	{
		m_pIChartCtrl->LoadAddIn( "ChartEnvSetupDlgAddIn.dll:STYLE_DATA");
		IStyleManager *pIStyleManager = ( IStyleManager *)m_pIChartCtrl->GetAddInCustomInterface( "ChartEnvSetupDlgAddIn.dll:STYLE_DATA");
		if( pIStyleManager)
		{
			pIStyleManager->LoadCurrentStyle();
			int nStyleCount = pIStyleManager->GetStyleCount();
			for( int i = 0; i < nStyleCount; i++)
				m_cbxStyleList.AddString( pIStyleManager->GetStyleName( i));
			pIStyleManager->Release();

			m_cbxStyleList.SetCurSel( nStyleCount - 1);
		}
	}
	else
	{
		CRect rctStyle;
		GetDlgItem( IDC_DRAW_STATIC)->GetWindowRect( &rctStyle);
		CRect rctWnd;
		GetWindowRect( &rctWnd);
		rctWnd.right = rctStyle.left;
		MoveWindow( &rctWnd);
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

/////////////////////////////////////////////////////////////////////////////
void CDlgGraphMove::OnSelchangeScalegroup() 
{
	UpdateData(TRUE);
	SetScalePositionAndGrid(GetVerticalScale(m_nScaleGroup));
}

void CDlgGraphMove::OnOK() 
{
	// (2009/9/22 - Seung-Won, Bae) Update new style.
	int nStyleIndex = m_cbxStyleList.GetCurSel();
	if( nStyleIndex != m_cbxStyleList.GetCount() - 1)
	{
		// (2009/9/21 - Seung-Won, Bae) Show Style Type on Price, Volume and MAs.
		if( IsStyledGraph())
		{
			IStyleManager *pIStyleManager = ( IStyleManager *)m_pIChartCtrl->GetAddInCustomInterface( "ChartEnvSetupDlgAddIn.dll:STYLE_DATA");
			if( pIStyleManager)
			{
				pIStyleManager->UpdateIndexStyle( m_pSourceGraph->GetGraphData().GetGraphName(), nStyleIndex);
				pIStyleManager->Release();
			}
		}
	}

	CDialog::OnOK();
}

void CDlgGraphMove::OnCancel() 
{	
	CDialog::OnCancel();
}

/////////////////////////////////////////////////////////////////////////////
// public ==================================================================
// 이동할 graph의 scale data 가져오기.
CMovingGraphVertScaleData CDlgGraphMove::GetMovingGraphVertScaleData() const
{
	return CMovingGraphVertScaleData(m_nScaleGroup, GetCurrentVerticalScale());
}


// private ==================================================================
// data 초기화
void CDlgGraphMove::InitializeData()
{
	for(int nIndex = 0; nIndex < m_pVertScaleGroupDataList->GetCount(); nIndex++){
		CVerticalScaleGroupData vertScaleGroupData = m_pVertScaleGroupDataList->GetAt(nIndex);
		m_ctlScaleGroup.AddString(vertScaleGroupData.GetVertScaleTitle());
		m_nScaleGroup = nIndex;
		SetScalePositionAndGrid(vertScaleGroupData.GetVertScale());
	}
	UpdateData(TRUE);
}

void CDlgGraphMove::SetScalePositionAndGrid(const CVerticalScale& verticalScale)
{
	m_nScalePosition = (int) verticalScale.GetVertScalePosition();
	m_nScaleGrid = (int) verticalScale.GetGridType();
	UpdateData(FALSE);
}

// ----------------------------------------------------------------------------
CVerticalScale CDlgGraphMove::GetVerticalScale(const int nIndex) const
{
	return m_pVertScaleGroupDataList->GetAt(nIndex).GetVertScale();
}

CVerticalScale CDlgGraphMove::GetCurrentVerticalScale() const
{
	CVerticalScale vertScale = GetVerticalScale(m_nScaleGroup);
	vertScale.SetGridType((CScaleBaseData::SCALEGRIDTYPE) m_nScaleGrid);
	vertScale.SetVertScalePosition((CScaleBaseData::VERTSCALEPOSITION) m_nScalePosition);
	return vertScale;
}

// (2009/9/21 - Seung-Won, Bae) Show Style Image.
void CDlgGraphMove::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
	
	// Do not call CDialog::OnPaint() for painting messages

	CRect rect( 10, 11, 176, 269 );
	CButton *pButton = ( CButton * )GetDlgItem( IDC_DRAW_STATIC);
	if( !pButton) return;

	// ( 2006 / 10 / 31 - Sang-Woo, Cho ) - IDC_DRAW_STATIC을 따라 움직이게 수정.
	pButton->GetWindowRect(rect);
	ScreenToClient(rect);

	rect.OffsetRect( 5, 5 );
	rect.DeflateRect( 0, 7, 10, 12 );

	dc.SetMapMode( MM_TEXT);
	IStyleManager *pIStyleManager = ( IStyleManager *)m_pIChartCtrl->GetAddInCustomInterface( "ChartEnvSetupDlgAddIn.dll:STYLE_DATA");
	if( pIStyleManager)
	{
		int nStyleIndex = m_cbxStyleList.GetCurSel();

		pIStyleManager->DrawSampleImage( dc.GetSafeHdc(), rect.left, rect.top, rect.right, rect.bottom, nStyleIndex, m_pTitleFont->GetSafeHandle());
		pIStyleManager->Release();
	}
}

void CDlgGraphMove::OnDestroy() 
{
	CDialog::OnDestroy();
	
	// TODO: Add your message handler code here
	if(m_pTitleFont != NULL)
	{
		m_pTitleFont->DeleteObject();
		delete m_pTitleFont;
		m_pTitleFont = NULL;
	}
}

int CDlgGraphMove::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// TODO: Add your specialized creation code here
	// 3. Init Font
	if( !m_pTitleFont)
	{
		m_pTitleFont = new CFont();

		LOGFONT	logFont;
		memset( &logFont, 0, sizeof(LOGFONT));       // zero out structure
		logFont.lfHeight = 12;
		logFont.lfCharSet = DEFAULT_CHARSET;

		_MHWND( m_pIChartCtrl->GetSafeHwnd());
		ML_SET_LANGUAGE_RES();
		CString strFont;
		strFont.LoadString( IDS_STRING_FONT);
		strcpy(logFont.lfFaceName, strFont);

		m_pTitleFont->CreateFontIndirect( &logFont );
	}
	
	return 0;
}

void CDlgGraphMove::OnSelchangeStylecombo() 
{
	// TODO: Add your control notification handler code here
	InvalidateRect( NULL);	
}

// (2009/9/21 - Seung-Won, Bae) Show Style Type on Price, Volume and MAs.
BOOL CDlgGraphMove::IsStyledGraph( void)
{
	if( m_pSourceGraph)
	{
		CIndicatorInfo* pInfo = m_pSourceGraph->GetIndicatorInfo();
		if(pInfo)
		{
			switch( g_iMetaTable.GetIndicatorID( pInfo->GetIndicatorName()))
			{
			case IMetaTable::EDDC_VOLUMECHART:			
// vntsorl:[0003431] [7111]차트화면에서의 조회속도가 구차트에 비해 늦어 개선 요구
// 			case IMetaTable::EDDC_ST_VOLUMEMACHART:		if( CChartInfo::FX == m_pIChartCtrl->GetChartMode())
			case IMetaTable::EDDC_OCX_VOLUMEMACHART:	if( CChartInfo::FX == m_pIChartCtrl->GetChartMode())
															return FALSE;
			case IMetaTable::EDDC_PRICECHART:
// 			case IMetaTable::EDDC_ST_PRICEMACHART:		return TRUE;
			case IMetaTable::EDDC_OCX_PRICEMACHART:		return TRUE;
			}
		}
	}
	return FALSE;
}