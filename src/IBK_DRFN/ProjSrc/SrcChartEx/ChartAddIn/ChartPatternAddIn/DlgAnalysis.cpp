// DlgAnalysis.cpp : implementation file
//

#include "stdafx.h"
#include "DlgAnalysis.h"

#include "PatternDefine.h"
#include "PatternMaker.h"				// for CPatternMaker
#include "DlgSetup.h"			// for Pattern Environment Setup Dlg
#include "Control_Grid/GridCtrl.h"		// for CGridCtrl
#include <MATH.H>						// for fabs
// 2008.02.20 by LYH >>
#include "./Include_Chart/Dll_Load/IMetaTable.h"		// for g_iMetaTable
// 2008.02.20 by LYH <<

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgAnalysis dialog


extern HINSTANCE g_hInstance;

CDlgAnalysis::CDlgAnalysis(CWnd* pParent, CPatternMaker* pPatternMaker)
	: CDialog(CDlgAnalysis::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgAnalysis)
	m_rdo_nRptType = 0;
	//}}AFX_DATA_INIT

	m_pPatternMaker = pPatternMaker;


	m_pGridWaveRptType1		= NULL;
	m_pGridCandleRtpType1	= NULL;


	m_brBack.CreateSolidBrush( COLOR_BG_ANLYS_DLG);		// Dialog BG Color
// 2008.02.20 by LYH >>
	m_hOcxWnd = NULL;
// 2008.02.20 by LYH <<
}


void CDlgAnalysis::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgAnalysis)
	DDX_Control(pDX, IDC_STATIC_GRIDBASE, m_staticGridBase);
	DDX_Radio(pDX, IDC_RDO_WAVE, m_rdo_nRptType);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgAnalysis, CDialog)
	//{{AFX_MSG_MAP(CDlgAnalysis)
	ON_WM_DESTROY()
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_RDO_WAVE, OnRdoWave)
	ON_BN_CLICKED(IDC_RDO_CANDLE, OnRdoCandle)
	ON_BN_CLICKED(IDC_BTN_ENVSETUP, OnBtnEnvSetup)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgAnalysis message handlers

BOOL CDlgAnalysis::OnInitDialog() 
{
	CDialog::OnInitDialog();

	ML_SET_LANGUAGE_RES();
	
	// TODO: Add extra initialization here
	// 폰트 초기화
	InitFont();

	// 파동패턴 그리드 초기화
	InitGrid_WaveRptType1();

	// 캔들패턴 그리드 초기화
	InitGrid_CandleRptType1();

	
	// **************************************************************************************
	//			다이아로그가 화면 영역을 벗어날 경우 위치 조정 - SeoJeong - 
	// -------------------------------------------------------------------------------------->
	CRect dlgRect;
	GetWindowRect(&dlgRect); 
	
	HDC hDC = CreateDC("DISPLAY", NULL, NULL, NULL);
    int width = GetDeviceCaps(hDC, HORZRES);
    int height = GetDeviceCaps(hDC, VERTRES);
	
	BOOL bReposWin = FALSE;
	if(dlgRect.bottom > height)
	{
		bReposWin = TRUE;
		dlgRect.top = height - dlgRect.Height();
	}
	if(dlgRect.right  > width )
	{
		bReposWin = TRUE;
		dlgRect.left = width - dlgRect.Width();
	}
	
	if(bReposWin)
		SetWindowPos(NULL, dlgRect.left, dlgRect.top, dlgRect.Width(), dlgRect.Height(), SW_SHOW);
	// <------------------------------------------------------------------------------------------
	DeleteDC(hDC);

// 2008.02.20 by LYH >>
//	AfxSetResourceHandle(hInstResourceClient);	
// 2008.02.20 by LYH <<
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}



void CDlgAnalysis::InitFont()
{
	CClientDC dc(this);
	int PointSize					= 9;
	m_logFont.lfHeight				= -MulDiv(PointSize, GetDeviceCaps(dc.GetSafeHdc(), LOGPIXELSY), 72);
	m_logFont.lfWidth				= 0;
	m_logFont.lfEscapement			= 0;
	m_logFont.lfOrientation			= 0;
	m_logFont.lfWeight				= FW_THIN;
	m_logFont.lfItalic				= FALSE;
	m_logFont.lfUnderline			= FALSE;
	m_logFont.lfStrikeOut			= FALSE;
	m_logFont.lfCharSet				= DEFAULT_CHARSET;
	m_logFont.lfOutPrecision		= OUT_CHARACTER_PRECIS;
	m_logFont.lfClipPrecision		= CLIP_CHARACTER_PRECIS;
	m_logFont.lfQuality				= DEFAULT_QUALITY;
	m_logFont.lfPitchAndFamily		= DEFAULT_PITCH | FF_ROMAN;
// 2008.02.20 by LYH >>
	//strcpy(m_logFont.lfFaceName, "굴림체");
	CString strFont;
	strFont.LoadString(IDS_STRING_FONT);
	strcpy(m_logFont.lfFaceName, strFont);
// 2008.02.20 by LYH <<	

	if( m_Font.GetSafeHandle() )
		m_Font.DeleteObject();

	m_Font.CreateFontIndirect(&m_logFont);
}


// **************************************************
// Grid of Wave Pattern Analysis
// **************************************************
void CDlgAnalysis::InitGrid_WaveRptType1()
{
	if (m_pGridWaveRptType1 == NULL)
	{
		// Create the Gridctrl object
		m_pGridWaveRptType1 = new CGridCtrl;
		if (!m_pGridWaveRptType1) return;

		// Create the Gridctrl window
		m_staticGridBase.GetWindowRect(m_rectGrid);		
		ScreenToClient(&m_rectGrid);
		m_pGridWaveRptType1->Create(m_rectGrid, this, 100);
		
		// fill it up with stuff
		m_pGridWaveRptType1->SetEditable(TRUE);
		m_pGridWaveRptType1->EnableDragAndDrop(FALSE);
		m_pGridWaveRptType1->SetFont(&m_Font, FALSE);
		m_pGridWaveRptType1->SetGridColor(COLOR_WHITE);
		m_pGridWaveRptType1->SetGridLineColor(COLOR_GRIDLINE);
		m_pGridWaveRptType1->SetListMode(TRUE);
		m_pGridWaveRptType1->EnableColumnHide();
		m_pGridWaveRptType1->EnableRowHide();
		m_pGridWaveRptType1->SetPatternReportType( WAVE_REPORTTYPE1);	// Report Grid Type


		m_pGridWaveRptType1->ShowWindow( SW_SHOW);

		
		// 그리드 모양을 Default로 한다
		InitGridRowCol_WaveRptType1();
	
		// 그리드의 모양을 바꾼다
		ResizeGridRowCol_WaveRptType1();

		// 그리드의 셀 포맷
		InitGridFormat_WaveRptType1();
	}
}

void CDlgAnalysis::InitGridRowCol_WaveRptType1()
{	
	try {
		m_pGridWaveRptType1->SetRowCount(DEFAULT_ROW_CNT_WAV1);
		m_pGridWaveRptType1->SetColumnCount(DEFAULT_COL_CNT_WAV1);
		m_pGridWaveRptType1->SetFixedRowCount(2);
		m_pGridWaveRptType1->SetFixedColumnCount(0);

		m_pGridWaveRptType1->SetBkColor(COLOR_WHITE);
	}
	catch (CMemoryException* e)
	{
		e->ReportError();
		e->Delete();
		return;
	}
}


void CDlgAnalysis::ResizeGridRowCol_WaveRptType1()
{
	int nCol = m_pGridWaveRptType1->GetColumnCount();
	int nRow = m_pGridWaveRptType1->GetRowCount();

	// fill rows/cols with text
	for (int row = 0; row < nRow; row++)
	{
		// Set Row Height
		m_pGridWaveRptType1->SetRowHeight(row, ROW_HEIGHT);

	}

	
	int nColWidth[] = { COL_WIDTH_WAV1_NUM,				COL_WIDTH_WAV1_TIME,			COL_WIDTH_WAV1_TYPE,			COL_WIDTH_WAV1_NAME,	
						COL_WIDTH_WAV1_CANDLECNT,		COL_WIDTH_WAV1_EXPUPDOWN_PER,	COL_WIDTH_WAV1_ACTUPDOWN_PER,	COL_WIDTH_WAV1_ACHIEVE_PER
					};

	for (int col = 0; col < nCol; col++)
	{
		// Set Column Width
		m_pGridWaveRptType1->SetColumnWidth(col, nColWidth[col]);
	}

}


void CDlgAnalysis::InitGridFormat_WaveRptType1()
{
	CString strTemp;

	int nCol = m_pGridWaveRptType1->GetColumnCount();
	int nRow = m_pGridWaveRptType1->GetRowCount();

// 2008.02.20 by LYH >>
	char *szTitle_K[]		= {	"No.", "날짜", "유형", "추세패턴명", "구성봉수", "발생별도달률",	"발생별도달률",	"발생별도달률"	};
	char *szTitle_E[]		= {	"No.", "Date", "Type", "Trend Name", "Bong Num", "Hitting Rate by Occurrence",	"Hitting Rate by Occurrence",	"Hitting Rate by Occurrence"	};
	char *szTitleSub_K[]	= {	"No.", "날짜", "유형", "추세패턴명", "구성봉수", "예상등락률",		"실제등락률",	"도달률"		};
	char *szTitleSub_E[]	= {	"No.", "Date", "Type", "Trend Name", "Bong Num", "Estimated %",		"Real %",	"Hitting %"		};

	IMetaTable::E_LANGUAGEID eLanguageID = g_iMetaTable.GetCurrentLanguageID( m_hOcxWnd);
// 2008.02.20 by LYH <<
	
	// fill rows/cols with text
	int row, col;	
	for (row = 0; row < nRow; row++)
	{
		for (col = 0; col < nCol; col++)		// JSJ_Modify
		{ 
			
			GV_ITEM Item;
			Item.mask = GVIF_TEXT|GVIF_FORMAT|GVIF_FGCLR|GVIF_BKCLR|GVIF_STATE;
			Item.nState = GVIS_READONLY | GVIS_MODIFIED ;//| GVIS_DROPHILITED;
			Item.row = row;
			Item.col = col;
			
			Item.nFormat = DT_VCENTER|DT_SINGLELINE|DT_NOPREFIX;//|DT_END_ELLIPSIS;
			
			// Text
			Item.strText = "";

			if( row == ROW_FIRST )
			{
// 2008.02.20 by LYH >>
				// Text
				//Item.strText = szTitle[col];
				if(eLanguageID == IMetaTable::ENGLISH)
					Item.strText = szTitle_E[col];
				else
					Item.strText = szTitle_K[col];
// 2008.02.20 by LYH <<
				
				// Format
				Item.nFormat |= DT_CENTER;

				// Color
				Item.crBkClr = COLOR_GRIDHEADER;
				Item.crFgClr = COLOR_BLACK;
			}
			else if( row == ROW_FIRST + 1 )
			{
// 2008.02.20 by LYH >>
				// Text
				//Item.strText = szTitleSub[col];
				if(eLanguageID == IMetaTable::ENGLISH)
					Item.strText = szTitleSub_E[col];
				else
					Item.strText = szTitleSub_K[col];
// 2008.02.20 by LYH <<
				
				// Format
				Item.nFormat |= DT_CENTER;

				// Color
				Item.crBkClr = COLOR_GRIDHEADER;
				Item.crFgClr = COLOR_BLACK;
			}
			else
			{
				if ( col == COL_WAV1_NAME)
					Item.nFormat |= DT_LEFT;
				else
					Item.nFormat |= DT_CENTER;

				// Color
				Item.crBkClr = COLOR_WHITE;
				Item.crFgClr = COLOR_BLACK;
			}


			// SetItem
			m_pGridWaveRptType1->SetItem(&Item);
		}
	}
}

void CDlgAnalysis::OnDestroy() 
{	
	// Wave Pattern Grid
	if( m_pGridWaveRptType1)	delete m_pGridWaveRptType1;
	m_pGridWaveRptType1 = NULL;

	// Candle Pattern Grid
	if( m_pGridCandleRtpType1)	delete m_pGridCandleRtpType1;
	m_pGridCandleRtpType1 = NULL;


	// Delete Brush
	if ( m_brBack.GetSafeHandle())	m_brBack.DeleteObject();


	CDialog::OnDestroy();
}


void CDlgAnalysis::SetFoundWavePtnToGrid_RptType1( CObject *pObjWavePatternData, double *p_dTime, double *p_dHigh, double *p_dLow, int nTotalDataCnt)
{
	if ( !m_pGridWaveRptType1)	return;


	CString strText, strTime;
	int		nDataCnt, nRow, nPatternTrend;
	double	dExpUpDownPer, dTemp, dActUpDownPer;
	COLORREF clrBG;

	CArrayObjPt(CWavePatternData) paryWavePatternData = NULL;
	paryWavePatternData = (CArrayObjPt(CWavePatternData))pObjWavePatternData;

	CWavePattern wpWavePattern;
	CWavePatternData* pWavePatternData = NULL;

	nDataCnt = paryWavePatternData->GetSize();
	InsertGridRow( m_pGridWaveRptType1, nDataCnt);

	for ( int i = 0; i < nDataCnt; i++)
	{
		nRow = i+ROW_FIRSTDATA;
		//pWavePatternData = paryWavePatternData->GetAt(i);
		pWavePatternData = paryWavePatternData->GetAt(nDataCnt-1-i);

		if ( pWavePatternData)
		{
			// Number
			strText.Format("%d", i+1);
			m_pGridWaveRptType1->SetItemText( nRow, COL_WAV1_NUM,		strText);

			// Time
			strTime.Format("%.f", p_dTime[pWavePatternData->m_nEndSeq] );		// TEMPORARY - SeoJeong - 2005/05/23
			if(strTime.GetLength() == 8)
			{
				// 일간/주간
				if(strTime.GetAt(0) == '1' && ((strTime.GetAt(1) - '1') < 3))
					strText = strTime.Left(2) + "/" + strTime.Mid(2, 2) + "/" + strTime.Mid(4, 2) + ":" + strTime.Mid(6);
				else
					strText = strTime.Left(4) + "-" + strTime.Mid(4, 2) + "-" + strTime.Mid(6);
			}
			else if(strTime.GetLength() == 7)
			{
				strText = "0" + strTime.Left(1) + "/" + strTime.Mid(1, 2) + "/" + strTime.Mid(3, 2) + ":" + strTime.Mid(5);
			}
			else if(strTime.GetLength() == 6)
			{
				// 월간
				//strText = strTime.Left(2) + ":" + strTime.Mid(2, 2) + ":" + strTime.Mid(4);
				if(strTime.GetAt(0) == '1' && ((strTime.GetAt(1) - '1') < 9))
					strText = strTime.Left(2) + ":" + strTime.Mid(2, 2) + ":" + strTime.Mid(4);
				else
					strText = strTime.Left(4) + "-" + strTime.Mid(4) + "-01";
			}
			else if(strTime.GetLength() == 5)
			{
				strText = "0" + strTime.Left(1) + ":" + strTime.Mid(1, 2) + ":" + strTime.Mid(3);
			}
			// 2011.01.06 by SYS >> 분차트 YYYY(년) 추가
			else if(strTime.GetLength()==14)
			{
				// 분간/틱 주기 경우..
				// YY/MM/DD,hh:mm 로 변경
				strText = strTime.Mid(2,2) + "/" + strTime.Mid(4,2) + "/" + strTime.Mid(6, 2) + "," + strTime.Mid(8, 2) + ":" + strTime.Mid(10,2);
			}
			// 2011.01.06 by SYS <<
			
			m_pGridWaveRptType1->SetItemText( nRow, COL_WAV1_TIME,		strText);

			// Wave Pattern Type
			strText = wpWavePattern.GetPatternTrendName( pWavePatternData->m_nPatternType);
			m_pGridWaveRptType1->SetItemText( nRow, COL_WAV1_TYPE,		strText);
			
			// Wave Pattern Name
			strText = wpWavePattern.GetPatternName( pWavePatternData->m_nPatternType);
			m_pGridWaveRptType1->SetItemText( nRow, COL_WAV1_NAME,		strText);

			// Consist Candle Count
			strText.Format("%d", pWavePatternData->m_nEndSeq - pWavePatternData->m_nStartSeq + 1);
			m_pGridWaveRptType1->SetItemText( nRow, COL_WAV1_CANDLECNT, strText);

			// Expect UpDown Percent Rate
			dTemp = fabs(pWavePatternData->m_dNeckBaseYPt1 - pWavePatternData->m_dNeckBaseYPt2);
			if ( pWavePatternData->m_dNeckYPt2 != ZERO)
			{
				dExpUpDownPer = dTemp / pWavePatternData->m_dNeckYPt2 * 100;
				if (wpWavePattern.GetPatternTrend( pWavePatternData->m_nPatternType) == DNWARD_REVERSE	|| 
					wpWavePattern.GetPatternTrend( pWavePatternData->m_nPatternType) == DNWARD_CONTINUE	)
					dExpUpDownPer *= -1;

				SetPercentNColor( m_pGridWaveRptType1, nRow, COL_WAV1_EXPUPDOWN_PER, dExpUpDownPer);

				// Real UpDown Percent Rate
				if(dExpUpDownPer>0)
				{
					dActUpDownPer = (p_dHigh[pWavePatternData->m_nEndSeq] - pWavePatternData->m_dNeckYPt2) / pWavePatternData->m_dNeckYPt2 * 100;
					SetPercentNColor( m_pGridWaveRptType1, nRow, COL_WAV1_ACTUPDOWN_PER, dActUpDownPer);
				}
				else 
				{
					dActUpDownPer = (p_dLow[pWavePatternData->m_nEndSeq] - pWavePatternData->m_dNeckYPt2) / pWavePatternData->m_dNeckYPt2 * 100;
					SetPercentNColor( m_pGridWaveRptType1, nRow, COL_WAV1_ACTUPDOWN_PER, dActUpDownPer);
				}

				int rate = (int)(dActUpDownPer/dExpUpDownPer)*100;	// 도달율 = 실제등락율/예상등락률 * 100
				SetPercentNColor( m_pGridWaveRptType1, nRow, COL_WAV1_ACHIEVE_PER, rate);
			}

//			// Real UpDown Percent Rate
//			if ( pWavePatternData->m_dNeckYPt2 != ZERO)
//			{
//				double dOffset = pWavePatternData->m_dNeckSeq2 + (pWavePatternData->m_dNeckSeq2 - pWavePatternData->m_dNeckSeq1);
//				
//				int nCloseOffset;
//				nCloseOffset = floor(dOffset+0.5);
//				if(nCloseOffset < nTotalDataCnt)
//				{
//					dExpUpDownPer = (p_dClose[nCloseOffset] - pWavePatternData->m_dNeckYPt2) / pWavePatternData->m_dNeckYPt2 * 100;
//					//if (wpWavePattern.GetPatternTrend( pWavePatternData->m_nPatternType) == DNWARD_REVERSE	|| 
//					//	wpWavePattern.GetPatternTrend( pWavePatternData->m_nPatternType) == DNWARD_CONTINUE	)
//					//	dExpUpDownPer *= -1;
//
//					SetPercentNColor( m_pGridWaveRptType1, nRow, COL_WAV1_ACTUPDOWN_PER, dExpUpDownPer);
//				}
//			}


			// Grid Color Set
			nPatternTrend = wpWavePattern.GetPatternTrend( pWavePatternData->m_nPatternType);
			if ( nPatternTrend == UPWARD_REVERSE)
				clrBG = COLOR_BG_UPWARD_REVERSE;
			else if ( nPatternTrend == DNWARD_REVERSE)
				clrBG = COLOR_BG_DNWARD_REVERSE;
			else if ( nPatternTrend == UPWARD_CONTINUE)
				clrBG = COLOR_BG_UPWARD_CONTINUE;
			else if ( nPatternTrend == DNWARD_CONTINUE)
				clrBG = COLOR_BG_DNWARD_CONTINUE;
				
			m_pGridWaveRptType1->SetItemBkColour( nRow, COL_WAV1_TYPE,		clrBG);
			m_pGridWaveRptType1->SetItemBkColour( nRow, COL_WAV1_NAME,		clrBG);
			m_pGridWaveRptType1->SetItemBkColour( nRow, COL_WAV1_CANDLECNT, clrBG);
		}
	}

	m_pGridCandleRtpType1->Refresh();
}



// **************************************************
// Grid of Candle Pattern Analysis
// **************************************************
void CDlgAnalysis::InitGrid_CandleRptType1()
{
	if (m_pGridCandleRtpType1 == NULL)
	{
		// Create the Gridctrl object
		m_pGridCandleRtpType1 = new CGridCtrl;
		if (!m_pGridCandleRtpType1) return;

		// Create the Gridctrl window
		m_staticGridBase.GetWindowRect(m_rectGrid);		
		ScreenToClient(&m_rectGrid);
		m_pGridCandleRtpType1->Create(m_rectGrid, this, 100);
		
		// fill it up with stuff
		m_pGridCandleRtpType1->SetEditable(TRUE);
		m_pGridCandleRtpType1->EnableDragAndDrop(FALSE);
		m_pGridCandleRtpType1->SetFont(&m_Font, FALSE);
		m_pGridCandleRtpType1->SetGridColor(COLOR_WHITE);
		m_pGridCandleRtpType1->SetGridLineColor(COLOR_GRIDLINE);
		m_pGridCandleRtpType1->SetListMode(TRUE);
		m_pGridCandleRtpType1->EnableColumnHide();
		m_pGridCandleRtpType1->EnableRowHide();
		m_pGridCandleRtpType1->SetPatternReportType( CANDLE_REPORTTYPE1);	// Report Grid Type


		m_pGridCandleRtpType1->ShowWindow( SW_HIDE);


		// 그리드 모양을 Default로 한다
		InitGridRowCol_CandleRptType1();
	
		// 그리드의 모양을 바꾼다
		ResizeGridRowCol_CandleRptType1();

		// 그리드의 셀 포맷
		InitGridFormat_CandleRptType1();
	}
}

void CDlgAnalysis::InitGridRowCol_CandleRptType1()
{	
	try {
		m_pGridCandleRtpType1->SetRowCount(DEFAULT_ROW_CNT_CND1);
		m_pGridCandleRtpType1->SetColumnCount(DEFAULT_COL_CNT_CND1);
		m_pGridCandleRtpType1->SetFixedRowCount(2);
		m_pGridCandleRtpType1->SetFixedColumnCount(0);

		m_pGridCandleRtpType1->SetBkColor(COLOR_WHITE);
	}
	catch (CMemoryException* e)
	{
		e->ReportError();
		e->Delete();
		return;
	}
}


void CDlgAnalysis::ResizeGridRowCol_CandleRptType1()
{
	int nCol = m_pGridCandleRtpType1->GetColumnCount();
	int nRow = m_pGridCandleRtpType1->GetRowCount();

	// fill rows/cols with text
	for (int row = 0; row < nRow; row++)
	{
		// Set Row Height
		m_pGridCandleRtpType1->SetRowHeight(row, ROW_HEIGHT);

	}

	
	int nColWidth[] = { COL_WIDTH_CND1_NUM,		COL_WIDTH_CND1_TIME,		COL_WIDTH_CND1_TYPE,	COL_WIDTH_CND1_NAME,	COL_WIDTH_CND1_DAY1,
						COL_WIDTH_CND1_DAY2,	COL_WIDTH_CND1_DAY3,		COL_WIDTH_CND1_DAY4,	COL_WIDTH_CND1_DAY5
					};

	for (int col = 0; col < nCol; col++)
	{
		// Set Column Width
		m_pGridCandleRtpType1->SetColumnWidth(col, nColWidth[col]);
	}

}


void CDlgAnalysis::InitGridFormat_CandleRptType1()
{
	CString strTemp;

	int nCol = m_pGridCandleRtpType1->GetColumnCount();
	int nRow = m_pGridCandleRtpType1->GetRowCount();

// 2008.02.20 by LYH >>
	char *szTitle_K[]		= {	"No.", "날짜", "유형", "캔들패턴명", "누적등락률",	"누적등락률",	"누적등락률",	"누적등락률",	"누적등락률"};
	char *szTitle_E[]		= {	"No.", "Date", "Type", "Candle Name", "Accumulated % Change",	"Accumulated % Change",	"Accumulated % Change",	"Accumulated % Change",	"Accumulated % Change"};
	char *szTitleSub_K[]	= {	"No.", "날짜", "유형", "캔들패턴명", "1일",			"2일",			"3일",			"4일",			"5일"		};
	char *szTitleSub_E[]	= {	"No.", "Date", "Type", "Candle Name", "1d",			"2d",			"3d",			"4d",			"5d"		};

	IMetaTable::E_LANGUAGEID eLanguageID = g_iMetaTable.GetCurrentLanguageID( m_hOcxWnd);				
// 2008.02.20 by LYH <<

	// fill rows/cols with text
	int row, col;	
	for (row = 0; row < nRow; row++)
	{
		for (col = 0; col < nCol; col++)		// JSJ_Modify
		{ 
			
			GV_ITEM Item;
			Item.mask = GVIF_TEXT|GVIF_FORMAT|GVIF_FGCLR|GVIF_BKCLR|GVIF_STATE;
			Item.nState = GVIS_READONLY | GVIS_MODIFIED ;//| GVIS_DROPHILITED;
			Item.row = row;
			Item.col = col;
			
			Item.nFormat = DT_VCENTER|DT_SINGLELINE|DT_NOPREFIX;//|DT_END_ELLIPSIS;
			
			// Text
			Item.strText = "";

			if( row == ROW_FIRST )
			{
				// Text
// 2008.02.20 by LYH >>
				//Item.strText = szTitle[col];
				if(eLanguageID == IMetaTable::ENGLISH)
					Item.strText = szTitle_E[col];
				else
					Item.strText = szTitle_K[col];
// 2008.02.20 by LYH <<
				
				// Format
				Item.nFormat |= DT_CENTER;

				// Color
				Item.crBkClr = COLOR_GRIDHEADER;
				Item.crFgClr = COLOR_BLACK;
			}
			else if( row == ROW_FIRST + 1 )
			{
// 2008.02.20 by LYH >>
				// Text
				//Item.strText = szTitleSub[col];
				if(eLanguageID == IMetaTable::ENGLISH)
					Item.strText = szTitleSub_E[col];
				else
					Item.strText = szTitleSub_K[col];
// 2008.02.20 by LYH <<
				
				// Format
				Item.nFormat |= DT_CENTER;

				// Color
				Item.crBkClr = COLOR_GRIDHEADER;
				Item.crFgClr = COLOR_BLACK;
			}
			else
			{
				if ( col == COL_CND1_NAME)
					Item.nFormat |= DT_LEFT;
				else
					Item.nFormat |= DT_CENTER;

				// Color
				Item.crBkClr = COLOR_WHITE;
				Item.crFgClr = COLOR_BLACK;
			}


			// SetItem
			m_pGridCandleRtpType1->SetItem(&Item);
		}
	}
}

void CDlgAnalysis::SetFoundCandlePtnToGrid_RptType1(	CObject *pObjCandlePatternData)
{
	if ( !m_pGridCandleRtpType1)	return;


	CString strText, strTime;
	int		nDataCnt, nRow, nPatternTrend;
	double	dBaseClose, dTemp;
	COLORREF clrBG;

	CArrayObjPt(CCandlePatternData) paryCandlePatternData = NULL;
	paryCandlePatternData = (CArrayObjPt(CCandlePatternData))pObjCandlePatternData;

	CCandlePattern cpCandlePattern;
	CCandlePatternData* pCandlePatternData = NULL;

	nDataCnt = paryCandlePatternData->GetSize();
	InsertGridRow( m_pGridCandleRtpType1, nDataCnt);

	for ( int i = 0; i < nDataCnt; i++)
	{
		nRow = i+ROW_FIRSTDATA;
		//pCandlePatternData = paryCandlePatternData->GetAt(i);
		pCandlePatternData = paryCandlePatternData->GetAt(nDataCnt-1-i);

		if ( pCandlePatternData)
		{
			// Number
			strText.Format("%d", i+1);
			m_pGridCandleRtpType1->SetItemText( nRow, COL_CND1_NUM, strText);

			// Time
			strTime.Format("%.f", pCandlePatternData->m_dTime);
//			strText = strTime.Left(4) + "-" + strTime.Mid(4, 2) + "-" + strTime.Mid(6);
			if(strTime.GetLength() == 8)
			{
				// 일간/주간
				if(strTime.GetAt(0) == '1' && ((strTime.GetAt(1) - '1') < 3))
					strText = strTime.Left(2) + "/" + strTime.Mid(2, 2) + "/" + strTime.Mid(4, 2) + ":" + strTime.Mid(6);
				else
					strText = strTime.Left(4) + "-" + strTime.Mid(4, 2) + "-" + strTime.Mid(6);
			}
			else if(strTime.GetLength() == 7)
			{
				strText = "0" + strTime.Left(1) + "/" + strTime.Mid(1, 2) + "/" + strTime.Mid(3, 2) + ":" + strTime.Mid(5);
			}
			else if(strTime.GetLength() == 6)
			{
				// 월간
				if(strTime.GetAt(0) == '1' && ((strTime.GetAt(1) - '1') < 9))
					strText = strTime.Left(2) + ":" + strTime.Mid(2, 2) + ":" + strTime.Mid(4);
				else
					strText = strTime.Left(4) + "-" + strTime.Mid(4) + "-01";
			}
			else if(strTime.GetLength() == 5)
			{
				strText = "0" + strTime.Left(1) + ":" + strTime.Mid(1, 2) + ":" + strTime.Mid(3);
			}
			// 2011.01.06 by SYS >> 분차트 YYYY(년) 추가
			else if(strTime.GetLength()==14)
			{
				// 분간/틱 주기 경우
				// YY/MM/DD,hh:mm 로 변경
				strText = strTime.Mid(2,2) + "/" + strTime.Mid(4,2) + "/" + strTime.Mid(6, 2) + "," + strTime.Mid(8, 2) + ":" + strTime.Mid(10,2);
			}
			// 2011.01.06 by SYS <<


			m_pGridCandleRtpType1->SetItemText( nRow, COL_CND1_TIME, strText);

			// Candle Pattern Type
			strText = cpCandlePattern.GetCandlePatternTrendName( pCandlePatternData->m_nCandlePtnType);
			m_pGridCandleRtpType1->SetItemText( nRow, COL_CND1_TYPE, strText);
			
			// Candle Pattern Name
			strText = cpCandlePattern.GetCandlePatternName( pCandlePatternData->m_nCandlePtnType);
			m_pGridCandleRtpType1->SetItemText( nRow, COL_CND1_NAME, strText);

			// 1Day : Accumulation rise and fall rate
			dBaseClose = pCandlePatternData->m_dClose[0];
			dTemp = pCandlePatternData->m_dClose[1]/dBaseClose * 100 - 100.;
			SetPercentNColor( m_pGridCandleRtpType1, nRow, COL_CND1_DAY1, dTemp);

			// 2Day : Accumulation rise and fall rate
			dTemp = pCandlePatternData->m_dClose[2]/dBaseClose * 100 - 100.;
			SetPercentNColor( m_pGridCandleRtpType1, nRow, COL_CND1_DAY2, dTemp);

			// 3Day : Accumulation rise and fall rate
			dTemp = pCandlePatternData->m_dClose[3]/dBaseClose * 100 - 100.;
			SetPercentNColor( m_pGridCandleRtpType1, nRow, COL_CND1_DAY3, dTemp);

			// 4Day : Accumulation rise and fall rate
			dTemp = pCandlePatternData->m_dClose[4]/dBaseClose * 100 - 100.;
			SetPercentNColor( m_pGridCandleRtpType1, nRow, COL_CND1_DAY4, dTemp);

			// 5Day : Accumulation rise and fall rate
			dTemp = pCandlePatternData->m_dClose[5]/dBaseClose * 100 - 100.;
			SetPercentNColor( m_pGridCandleRtpType1, nRow, COL_CND1_DAY5, dTemp);


			// Grid Color Set
			nPatternTrend = cpCandlePattern.GetCandlePatternTrend( pCandlePatternData->m_nCandlePtnType);
			if ( nPatternTrend == UPWARD_REVERSE)
				clrBG = COLOR_BG_UPWARD_REVERSE;
			else if ( nPatternTrend == DNWARD_REVERSE)
				clrBG = COLOR_BG_DNWARD_REVERSE;
			else if ( nPatternTrend == UPWARD_CONTINUE)
				clrBG = COLOR_BG_UPWARD_CONTINUE;
			else if ( nPatternTrend == DNWARD_CONTINUE)
				clrBG = COLOR_BG_DNWARD_CONTINUE;
				
			m_pGridCandleRtpType1->SetItemBkColour( nRow, COL_CND1_TYPE, clrBG);
			m_pGridCandleRtpType1->SetItemBkColour( nRow, COL_CND1_NAME, clrBG);
		}
	}

	m_pGridCandleRtpType1->Refresh();
}

// Set Percent Text and Color
void CDlgAnalysis::SetPercentNColor(CGridCtrl* pGridCtrl, int nRow, int nCol, double dValue)
{
	if ( dValue == -100)
		return;

	CString strText;
	COLORREF clrBG;

	strText.Format("%.2f", dValue);
	pGridCtrl->SetItemText( nRow, nCol, strText);

	if ( dValue > 0)		// 상승
		clrBG = COLOR_BG_UPWARD_REVERSE;
	else if ( dValue < 0)	// 하락
		clrBG = COLOR_BG_DNWARD_REVERSE;
	else					// 보합
		clrBG = COLOR_WHITE;

	pGridCtrl->SetItemBkColour( nRow, nCol, clrBG);
}


// 그리드 열 추가
void CDlgAnalysis::InsertGridRow(CGridCtrl* pGridCtrl, int nAddRowCnt)
{
	int nFixedRowCnt = pGridCtrl->GetFixedRowCount();

	pGridCtrl->SetRowCount( nFixedRowCnt + nAddRowCnt);

	InitInsertedGirdRow( pGridCtrl);
}


// 추가된 그리드 열의 초기화
void CDlgAnalysis::InitInsertedGirdRow(CGridCtrl* pGridCtrl)
{
	int row, col;
	int nRowCnt = pGridCtrl->GetRowCount();
	int nColCnt = pGridCtrl->GetColumnCount();

	// Reset Row Height and Set Column Format
	for (row = ROW_FIRSTDATA; row < nRowCnt; row++)
	{
		// Row Height
		pGridCtrl->SetRowHeight_NoReset(row, ROW_HEIGHT);
		
		// Column Format
		for (col = 0; col < nColCnt; col++)
		{ 
			GV_ITEM Item;
			Item.mask = GVIF_TEXT|GVIF_FORMAT|GVIF_FGCLR|GVIF_BKCLR|GVIF_STATE;
			Item.nState = GVIS_READONLY | GVIS_MODIFIED ;//| GVIS_DROPHILITED;
			Item.row = row;
			Item.col = col;
			
			Item.nFormat = DT_VCENTER|DT_SINGLELINE|DT_NOPREFIX;//|DT_END_ELLIPSIS;
			
			if ( pGridCtrl->GetPatternReportType() == WAVE_REPORTTYPE1)
			{
				if ( col == COL_WAV1_NAME)
					Item.nFormat |= DT_LEFT;
				else
					Item.nFormat |= DT_CENTER;
			}
			else if ( pGridCtrl->GetPatternReportType() == CANDLE_REPORTTYPE1)
			{
				if ( col == COL_CND1_NAME)
					Item.nFormat |= DT_LEFT;
				else
					Item.nFormat |= DT_CENTER;
			}
			else
			{
				Item.nFormat |= DT_CENTER;
			}
			
			Item.strText = "";
			
			Item.crBkClr = COLOR_WHITE;
			Item.crFgClr = COLOR_BLACK;
			
			pGridCtrl->SetItem(&Item);
		}
	}
	pGridCtrl->Call_ResetScrollBars();

	pGridCtrl->Refresh();	
}

// Set Static Title
void CDlgAnalysis::SetReportTitleBasicInfo( double dStartDate, double dEndDate, int nDataCnt)
{
	CString strTitle, strTemp;
	
	strTemp.Format( "%.f", dStartDate);
	// 2011.01.06 by SYS >> 분차트 YYYY(년) 추가
	//if(strTemp.GetLength()>=8)
	//	strTitle = strTemp.Left(4) + "-" + strTemp.Mid(4, 2) + "-" + strTemp.Mid(6);
	if(strTemp.GetLength()==8)
	{
		// 일간/주간/월간 경우
		if(strTemp.GetAt(0) == '1' && ((strTemp.GetAt(1) - '1') < 3))
			strTitle = strTemp.Left(2) + "/" + strTemp.Mid(2, 2) + "," + strTemp.Mid(4, 2) + ":" + strTemp.Mid(6);
		else
			strTitle = strTemp.Left(4) + "-" + strTemp.Mid(4, 2) + "-" + strTemp.Mid(6);
	}
	else if(strTemp.GetLength() == 7)
	{
		strTitle = "0" + strTemp.Left(1) + "/" + strTemp.Mid(1, 2) + "," + strTemp.Mid(3, 2) + ":" + strTemp.Mid(5);
	}
	else if(strTemp.GetLength()==14)
	{
		// 분간/틱 주기 경우
		// MM/DD,hh:mm
		strTitle = strTemp.Mid(4,2) + "/" + strTemp.Mid(6, 2) + "," + strTemp.Mid(8, 2) + ":" + strTemp.Mid(10,2);
	}
	// 2011.01.06 by SYS <<
	else
	{
		// 월간 경우...
		strTitle = strTemp.Left(4) + "-" + strTemp.Mid(4);
	}
	SetDlgItemText( IDC_STATIC_STARTDATE, strTitle);

	strTemp.Format( "%.f", dEndDate);
	// 2011.01.06 by SYS >> 분차트 YYYY(년) 추가
	//if(strTemp.GetLength()>=8)
	//	strTitle = strTemp.Left(4) + "-" + strTemp.Mid(4, 2) + "-" + strTemp.Mid(6);
	if(strTemp.GetLength()==8)
	{
		if(strTemp.GetAt(0) == '1' && ((strTemp.GetAt(1) - '1') < 3))
			strTitle = strTemp.Left(2) + "/" + strTemp.Mid(2, 2) + "," + strTemp.Mid(4, 2) + ":" + strTemp.Mid(6);
		else
			strTitle = strTemp.Left(4) + "-" + strTemp.Mid(4, 2) + "-" + strTemp.Mid(6);
	}
	else if(strTemp.GetLength() == 7)
	{
		strTitle = "0" + strTemp.Left(1) + "/" + strTemp.Mid(1, 2) + "," + strTemp.Mid(3, 2) + ":" + strTemp.Mid(5);
	}
	else if(strTemp.GetLength()==14)
	{
		// MM/DD,hh:mm
		strTitle = strTemp.Mid(4,2) + "/" + strTemp.Mid(6, 2) + "," + strTemp.Mid(8, 2) + ":" + strTemp.Mid(10,2);
	}
	// 2011.01.06 by SYS <<
	else
		strTitle = strTemp.Left(4) + "-" + strTemp.Mid(4);
	SetDlgItemText( IDC_STATIC_ENDDATE, strTitle);

	strTitle.Format( "%d", nDataCnt);
	SetDlgItemText( IDC_STATIC_DATACNT, strTitle);
}


void CDlgAnalysis::OnRdoWave() 
{
	if ( m_pGridWaveRptType1)
		m_pGridWaveRptType1->ShowWindow( SW_SHOW);

	if ( m_pGridCandleRtpType1)
		m_pGridCandleRtpType1->ShowWindow( SW_HIDE);
}

void CDlgAnalysis::OnRdoCandle() 
{
	if ( m_pGridWaveRptType1)
		m_pGridWaveRptType1->ShowWindow( SW_HIDE);

	if ( m_pGridCandleRtpType1)
		m_pGridCandleRtpType1->ShowWindow( SW_SHOW);
}




LRESULT CDlgAnalysis::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	if(message == WM_NOTIFY)
	{
		NM_GRIDVIEW *pNmgv = (NM_GRIDVIEW *)lParam;
		int nCode = pNmgv->hdr.code;
		
		if( nCode == NM_CLICK)				// LButton Click	(GVN_SELCHANGING -> 사용하지 않음)
		{
			if(pNmgv->hdr.hwndFrom == m_pGridWaveRptType1->m_hWnd)
			{
				if ( pNmgv->iRow >= ROW_FIRSTDATA)
					SetShareData(m_pGridWaveRptType1, pNmgv->iRow);
			}
			else
			{
				if ( m_pGridCandleRtpType1->GetPatternReportType() == CANDLE_REPORTTYPE1)
				{
					if ( pNmgv->iRow >= ROW_FIRSTDATA)
					{
						SetShareData(m_pGridCandleRtpType1, pNmgv->iRow);
						GetSelectedCandlePatternData( pNmgv->iRow);
					}
				}
			}
		}
	}
	
	return CDialog::WindowProc(message, wParam, lParam);
}


void CDlgAnalysis::GetSelectedCandlePatternData(int nRow)
{
	double dTime;
	CString strPatternName, strText;

	strText = m_pGridCandleRtpType1->GetItemText( nRow, COL_CND1_TIME);
	strText.Remove('-');
	dTime = atof(strText);

	strPatternName = m_pGridCandleRtpType1->GetItemText( nRow, COL_CND1_NAME);
	m_pPatternMaker->SetSelectedCandlePatternData( dTime, strPatternName);
}

void CDlgAnalysis::SetShareData(CGridCtrl* pGrid, int nRow)
{
	CString strText;

	strText = pGrid->GetItemText( nRow, COL_CND1_TIME);

	// 2011.01.06 by SYS >> 분차트 YYYY(년) 추가
	if(strText.Find(',') > 0)
	{
		strText.Remove('/');
		strText.Remove(':');
		strText.Remove(',');
		CTime currentTime = CTime::GetCurrentTime(); 	
		CString strYear = currentTime.Format("%Y");
		strText = strYear.Mid(0,2) + strText + "00";
	}
	// 2011.01.06 by SYS <<
	else if(strText.Find('/') > 0)
	{
		strText.Remove('/');
		strText.Remove(':');
		CTime currentTime = CTime::GetCurrentTime(); 	
		CString strYear = currentTime.Format("%Y");
		strText = strYear + strText + "00";
	}
	else if(strText.Find(':') > 0)
	{
		strText.Remove(':');
		CTime currentTime = CTime::GetCurrentTime(); 	
		CString strDate = currentTime.Format("%Y%m%d");
		strText = strDate + strText;
	}
	else
		strText.Remove('-');

	m_pPatternMaker->SetGSharedData("시간-차트",strText);
}

HBRUSH CDlgAnalysis::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	
	pDC->SetBkMode( TRANSPARENT);

	if ( nCtlColor == CTLCOLOR_STATIC)
	{
	}

	hbr = m_brBack;
	
	// TODO: Return a different brush if the default is not desired
	return hbr;
}

void CDlgAnalysis::OnBtnEnvSetup() 
{
	// (2004.10.19, 배승원) Resource의 접근이 잘되도록 Module State를 적용한다.
	//AFX_MANAGE_STATE( AfxGetStaticModuleState());

	ML_SET_LANGUAGE_RES();

	// 패턴 환경설정 Dlg
	CDlgSetup dlgSetup;
	dlgSetup.SetPatternEnvData(&(m_pPatternMaker->m_PatternEnvData));

	if( dlgSetup.DoModal() == IDOK )
	{
		m_pPatternMaker->DeleteAllData();				// 패턴관련 데이터 모두 삭제
		m_pPatternMaker->InitSelectedPatternData();		// Init Selected Pattern Data

		// Get All WavePattern Data
		m_pPatternMaker->GetAllData_AboutPattern();
		// Get All CandlePattern Data
		m_pPatternMaker->GetAllData_AboutCandlePattern();
	}	

	m_pPatternMaker->RefreshChart();
}
