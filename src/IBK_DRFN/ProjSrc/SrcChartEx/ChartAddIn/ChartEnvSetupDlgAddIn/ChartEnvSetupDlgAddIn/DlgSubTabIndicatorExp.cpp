// DlgSubTabIndicatorExp.cpp : implementation file
//

#include "stdafx.h"
#include "chartenvsetupdlgaddin.h"
#include "IndicatorExpHtmlView.h"
#include "DlgSubTabIndicatorExp.h"
#include "DlgMainTabIndicatorSetUp.h"

// 2008.01.25 by LYH >>
#include "../Include_Chart/Dll_Load/IMetaTable.h"	// for _MTOL0()
// 2008.01.25 by LYH <<

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgSubTabIndicatorExp dialog


CDlgSubTabIndicatorExp::CDlgSubTabIndicatorExp( IChartOCX *p_pIChartOCX, CWnd* pParent /* = NULL */)
	: CDialog( CDlgSubTabIndicatorExp::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgSubTabIndicatorExp)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_strBaseFilePath = "";

	m_pIChartOCX = p_pIChartOCX;
	if( m_pIChartOCX) m_pIChartOCX->AddRef();

// 2008.01.25 by LYH >>
	m_hOcxWnd = NULL;
// 2008.01.25 by LYH <<
}

CDlgSubTabIndicatorExp::~CDlgSubTabIndicatorExp()							// standard destructor
{
	if( m_pIChartOCX)
	{
		m_pIChartOCX->Release();
		m_pIChartOCX = NULL;
	}
}

void CDlgSubTabIndicatorExp::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgSubTabIndicatorExp)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgSubTabIndicatorExp, CDialog)
	//{{AFX_MSG_MAP(CDlgSubTabIndicatorExp)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgSubTabIndicatorExp message handlers

BOOL CDlgSubTabIndicatorExp::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here

	// 1. 현재 실행되는 Module의 전체경로를 얻어온다.
	ILPCSTR szBaseFilePath = m_pIChartOCX->GetDefaultPathString();
	m_strBaseFilePath = szBaseFilePath;

	// 2. 얻어온 경로에 지표설명 Html이 있는 폴더의 경로로 변경해준다.
	m_strBaseFilePath = m_strBaseFilePath + "Exp\\";

	// 3. HtmlView를 생성한다.
	CWnd *pFrameWnd = this;
	CCreateContext pContext;
	pContext.m_pCurrentDoc = new CMyDocument;
    pContext.m_pNewViewClass = RUNTIME_CLASS(CIndicatorExpHtmlView);

    m_pIndicatorExpHtmlView = (CIndicatorExpHtmlView *)((CFrameWnd*)pFrameWnd)->CreateView(&pContext);
    ASSERT(m_pIndicatorExpHtmlView);

	CRect rect;
 	GetClientRect(rect);
	rect.DeflateRect(5,5);

	m_pIndicatorExpHtmlView->ShowWindow(SW_SHOW);
	m_pIndicatorExpHtmlView->MoveWindow(rect);
  
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


//-----------------------------------------------------------------------------
// Author		: Sang-Woo, Cho		Date : 2006 / 10 / 31
// Return void	: 
// Param  CString strIndicatorName : 
// Comments		: 
//-----------------------------------------------------------------------------
void CDlgSubTabIndicatorExp::ReadIndicatorExpFile( CString strIndicatorName )
{
	static CMapStringToString mapIndicatorHelpName;
	if( mapIndicatorHelpName.GetCount() <= 0)
	{
		mapIndicatorHelpName.SetAt( _MTEXT( C2_VOLUME_AMOUNT),				"VolumePrice");
		mapIndicatorHelpName.SetAt( _MTEXT( C2_RAINBOW_CHART),				"Rainbow");
		mapIndicatorHelpName.SetAt( _MTEXT( C2_OVERHANGING_SUPPLY),			"VolumeForSale");
		mapIndicatorHelpName.SetAt( _MTEXT( C2_DISPERSION),					"Dispersion");
		mapIndicatorHelpName.SetAt( _MTEXT( C2_THREE_LINE_BREAK),			"ThreeLineBreak");
		mapIndicatorHelpName.SetAt( _MTEXT( C2_PSYCHOLOGICAL_LINE),			"Psychology");
		mapIndicatorHelpName.SetAt( _MTEXT( C2_NEW_PSYCHOLOGY_LINE),		"신심리도");
		mapIndicatorHelpName.SetAt( _MTEXT( C2_INVERSE_TIME_LINE),			"InverseLine");
		mapIndicatorHelpName.SetAt( _MTEXT( C2_DISPARITY_SMA),				"Disparity_SMA");
		mapIndicatorHelpName.SetAt( _MTEXT( C2_DISPARITY_EMA),				"Disparity_EMA");
		mapIndicatorHelpName.SetAt( _MTEXT( C2_OBVIOUSLY_BALANCE_CHART),	"GlanceBalance");
		// (2008/10/16 - Seung-Won, Bae) Make short for standard ~.
		mapIndicatorHelpName.SetAt( _MTEXT( C2_STANDARD_DEVIATION),			"Std_Dev");
		mapIndicatorHelpName.SetAt( _MTEXT( C2_STANDARD_ERROR),				"Std_Err");
	}
	CString strNewIndicatorName;
	if( mapIndicatorHelpName.Lookup( strIndicatorName, strNewIndicatorName)) strIndicatorName = strNewIndicatorName;
	else	strIndicatorName = _MTOL2( strIndicatorName);
	switch( g_iMetaTable.GetCurrentLanguageID( m_hOcxWnd))
	{
		case IMetaTable::ENGLISH:	strIndicatorName = "_e_" + strIndicatorName;
									break;
	}

	// ( 2006 / 11 / 18 - Sang-Woo, Cho ) - 삼성에서 지표 설명파일에 공백이 있으면
	// 버전처리가 안된다고 해서 지표명에 공백이 있으면 "_"로 바꾼다.
	strIndicatorName.Replace( ' ', '_' );
	strIndicatorName.Replace( '%', '_');
	strIndicatorName.Replace( '&', '_');
	strIndicatorName.Replace( '\'', '_');

	CString strFilePath = m_strBaseFilePath + strIndicatorName + ".html";
	
	CFileFind finder;
	if( finder.FindFile(strFilePath) )
		m_pIndicatorExpHtmlView->Navigate(strFilePath);

	else
	{
		strFilePath = m_strBaseFilePath + "empty.html";
		m_pIndicatorExpHtmlView->Navigate(strFilePath);
	}
}

BOOL CDlgSubTabIndicatorExp::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	return CDialog::PreTranslateMessage(pMsg);
}
