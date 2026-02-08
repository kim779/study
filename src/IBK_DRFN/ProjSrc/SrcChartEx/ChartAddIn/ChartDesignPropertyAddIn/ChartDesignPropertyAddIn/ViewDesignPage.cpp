// ViewDesignPage.cpp : implementation file
//

#include "stdafx.h"
#include "ChartDesignPropertyAddIn.h"
#include "ViewDesignPage.h"

#include "../Include_Chart/xUtil_Chart.h"		// for CHART_CONVERT_BEGIN
#include "../Include_Chart/IPropertyMap.h"		// for IPropertyMap
#include "ChartPropertySheet.h"
#include "PropertiesData.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CViewDesignPage property page

IMPLEMENT_DYNCREATE(CViewDesignPage, CPropertyPage)

CViewDesignPage::CViewDesignPage() : CPropertyPage(CViewDesignPage::IDD)
{
	//{{AFX_DATA_INIT(CViewDesignPage)
	m_nBorderShape = -1;
	m_bDisplayTotalData = FALSE;
	m_nDataCountPerOnePage = 0;
	m_nLeftSpace = 0;
	m_nRightSpace = 0;
	m_nTopSpace = 0;
	m_nBottomSpace = 0;
	m_nMarginBorderType = -1;
	m_bUseConditiontoTitle = FALSE;
	m_bUseDatatoTitle = FALSE;
	m_nVertGap = 0;
	m_nHorzGap = 0;
	m_nBongCountMargin = 0;
	m_bDisplayTRdata = FALSE; //sy 2005.12.08.
	//}}AFX_DATA_INIT
	m_nMinDataCountOnOnePage = 5;

	// (2006/7/5 - Seung-Won, Bae) Manage Property Map for Update
	m_pIPropertyMap = NULL;

	// (2008/1/14 - Seung-Won, Bae) for MultiLanguage.
	m_hOcxWnd = NULL;
}

CViewDesignPage::~CViewDesignPage()
{
}

void CViewDesignPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CViewDesignPage)
	DDX_Control(pDX, IDC_DATACOUNTPERONEPAGE, m_CtrlDataCountPerOnePage);
	DDX_Radio(pDX, IDC_BORDERSHAPE, m_nBorderShape);
	DDX_Check(pDX, IDC_DISPLAYTOTALDATA, m_bDisplayTotalData);
	DDX_Text(pDX, IDC_DATACOUNTPERONEPAGE, m_nDataCountPerOnePage);
	DDX_Text(pDX, IDC_LEFTSPACE, m_nLeftSpace);
	DDX_Text(pDX, IDC_RIGHTSPACE, m_nRightSpace);
	DDX_Text(pDX, IDC_TOPSPACE, m_nTopSpace);
	DDX_Text(pDX, IDC_BOTTOMSPACE, m_nBottomSpace);
	DDX_Radio(pDX, IDC_MARGIN_BORDERSHAPE, m_nMarginBorderType);
	DDX_Check(pDX, IDC_USECONDITIONTOTITLE, m_bUseConditiontoTitle);
	DDX_Check(pDX, IDC_USEDATATOTITLE, m_bUseDatatoTitle);
	DDX_Text(pDX, IDC_VERTGAP, m_nVertGap);
	DDX_Text(pDX, IDC_HORZGAP, m_nHorzGap);
	DDX_Text(pDX, IDC_MINDATACOUNT, m_nMinDataCountOnOnePage);
	DDX_Text(pDX, IDC_BONGCOUNT_MARGIN, m_nBongCountMargin);
	DDV_MinMaxInt(pDX, m_nBongCountMargin, 0, 500);
	DDX_Check(pDX, IDC_DISPLAY_TRDATA, m_bDisplayTRdata);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CViewDesignPage, CPropertyPage)
	//{{AFX_MSG_MAP(CViewDesignPage)
	ON_BN_CLICKED(IDC_DISPLAYTOTALDATA, OnDisplaytotaldata)
	ON_EN_CHANGE(IDC_MINDATACOUNT, OnChangeMindatacount)
	ON_BN_CLICKED(IDC_BTN_GRIDSHAPE_LINE, OnBtnGridshapeLine)
	ON_BN_CLICKED(IDC_BTN_GRIDSHAPE_CUTTEDLINE, OnBtnGridshapeCuttedline)
	ON_BN_CLICKED(IDC_BTN_GRIDSHAPE_DOTLINE, OnBtnGridshapeDotline)
	ON_BN_CLICKED(IDC_BTN_GRIDSHAPE_NONE, OnBtnGridshapeNone)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CViewDesignPage message handlers

BOOL CViewDesignPage::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();

	//sy 2005.12.08.
	OnDisplaytotaldata();

	CreateToolTip();
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

//Create tooltip object and Add text to StringArray 
void CViewDesignPage::CreateToolTip()
{
	ML_SET_LANGUAGE_RES();
	m_toolTip.Create( this );
	CDWordArray dwaInfo;

	dwaInfo.RemoveAll();
	dwaInfo.Add( IDS_VIEWDESIGN_STATIC31_TOOLTIP);
	m_toolTip.AddControlInfo( IDC_STATIC31, dwaInfo );

	dwaInfo.RemoveAll();
	dwaInfo.Add( IDS_VIEWDESIGN_STATIC32_TOOLTIP);
	m_toolTip.AddControlInfo( IDC_STATIC32, dwaInfo );

	dwaInfo.RemoveAll();
	dwaInfo.Add( IDS_DISPLAYTOTALDATA_TOOLTIP);
	m_toolTip.AddControlInfo( IDC_DISPLAYTOTALDATA, dwaInfo );

	dwaInfo.RemoveAll();
	dwaInfo.Add( IDS_DATACOUNTPERONEPAGE_TOOLTIP);
	m_toolTip.AddControlInfo( IDC_STATIC33, dwaInfo );
	m_toolTip.AddControlInfo( IDC_DATACOUNTPERONEPAGE, dwaInfo );

	dwaInfo.RemoveAll();
	dwaInfo.Add( IDS_VIEWDESIGN_STATIC34_TOOLTIP);
	m_toolTip.AddControlInfo( IDC_STATIC34, dwaInfo );
}

BOOL CViewDesignPage::PreTranslateMessage(MSG* pMsg) 
{
	POINT pt = pMsg->pt;	
	ScreenToClient( &pt );

	if( pMsg->message == WM_RBUTTONDOWN )	{
		m_toolTip.ShowToolTip( (CPoint)pt );
	}
	if(pMsg->message == WM_MOUSEMOVE){
		m_toolTip.HideToolTip( (CPoint)pt );
	}
	return CPropertyPage::PreTranslateMessage(pMsg);
}

void CViewDesignPage::OnDisplaytotaldata() 
{
	//sy 2005.12.08.
	UpdateData(TRUE);
	if(m_bDisplayTotalData == TRUE)
	{
		m_CtrlDataCountPerOnePage.EnableWindow(FALSE);
		EnableWindow(IDC_DISPLAY_TRDATA, false);
	}
	else
	{
		m_CtrlDataCountPerOnePage.EnableWindow(TRUE);
		EnableWindow(IDC_DISPLAY_TRDATA, true);	
	}
}

void CViewDesignPage::OnChangeMindatacount() 
{
	CString str;
	CEdit* pmindatacount = (CEdit* )GetDlgItem(IDC_MINDATACOUNT);
	pmindatacount->GetWindowText(str);
	m_nMinDataCountOnOnePage = atoi(str);

	if(m_nDataCountPerOnePage < m_nMinDataCountOnOnePage) 
	{
		ML_SET_LANGUAGE_RES();
		CString strMsg;
		strMsg.LoadString( IDS_ENTER_LESS_THAN_VIEW_COUNT);
		AfxMessageBox( strMsg);
		m_nMinDataCountOnOnePage = 0;
		UpdateData(FALSE);
	}
}

//sy 2005.12.08.
void CViewDesignPage::EnableWindow(const int nID, const bool bEnable)
{
	CWnd* pWnd = (CWnd*) GetDlgItem(nID);
	if(pWnd != NULL)
		pWnd->EnableWindow(bEnable);
}
//sy end


// (2007/7/2 - Seung-Won, Bae) Do not Support Main Grid Type.
void CViewDesignPage::OnBtnGridshapeLine() 
{
	// TODO: Add your control notification handler code here
	SetAllGridType( CScaleBaseData::SOLIDLINE);
}
void CViewDesignPage::OnBtnGridshapeCuttedline() 
{
	// TODO: Add your control notification handler code here
	SetAllGridType( CScaleBaseData::LONGDOTTEDLINE);
}
void CViewDesignPage::OnBtnGridshapeDotline() 
{
	// TODO: Add your control notification handler code here
	SetAllGridType( CScaleBaseData::SHORTDOTTEDLINE);
}
void CViewDesignPage::OnBtnGridshapeNone() 
{
	// TODO: Add your control notification handler code here
	SetAllGridType( CScaleBaseData::SLNONE);
}
void CViewDesignPage::SetAllGridType( const int p_nScaleGridType) 
{
	UpdateData(TRUE);

	ML_SET_LANGUAGE_RES();
	CString text, strTitle;
	text.LoadString( IDS_MSG_APPLY_ALL_SCALE_LINE);
	strTitle.LoadString( IDS_BLOCK_SETTING);
	int nOk = MessageBox(text, strTitle, MB_OKCANCEL | MB_ICONQUESTION);
	if(nOk == IDOK)
		((CChartPropertySheet *)GetParent())->m_ChartListPage.m_TreeChart.SetScaleCompartTypeToAllBlocks( p_nScaleGridType);
}

//Set Received Value From ocx
void CViewDesignPage::SetProperty( IPropertyMap *p_pIPropertyMap)
{
	// (2006/7/5 - Seung-Won, Bae) Use IPropertyMap
	CHART_CONVERT_BEGIN()
		CHART_CONVERT_INT(		p_pIPropertyMap, g_anPropertyShortKey[	TYPEBORDER						], m_nBorderShape,				_DPV_TYPEBORDER					);
		CHART_CONVERT_BOOL(		p_pIPropertyMap, g_anPropertyShortKey[	BWHOLEVIEW						], m_bDisplayTotalData,			_DPV_BWHOLEVIEW					);
		CHART_CONVERT_INT(		p_pIPropertyMap, g_anPropertyShortKey[	NONEPAGEDATACOUNT				], m_nDataCountPerOnePage,		_DPV_NONEPAGEDATACOUNT			);
		CHART_CONVERT_INT(		p_pIPropertyMap, g_anPropertyShortKey[	NLEFTMARGIN						], m_nLeftSpace,				_DPV_NLEFTMARGIN				);
		CHART_CONVERT_INT(		p_pIPropertyMap, g_anPropertyShortKey[	NRIGHTMARGIN					], m_nRightSpace,				_DPV_NRIGHTMARGIN				);
		CHART_CONVERT_INT(		p_pIPropertyMap, g_anPropertyShortKey[	NTOPMARGIN						], m_nTopSpace,					_DPV_NTOPMARGIN					);
		CHART_CONVERT_INT(		p_pIPropertyMap, g_anPropertyShortKey[	NBOTTOMMARGIN					], m_nBottomSpace,				_DPV_NBOTTOMMARGIN				);
		CHART_CONVERT_INT(		p_pIPropertyMap, g_anPropertyShortKey[	NMAGINBORDERTYPE				], m_nMarginBorderType,			_DPV_NMAGINBORDERTYPE			);
		CHART_CONVERT_BOOL(		p_pIPropertyMap, g_anPropertyShortKey[	BUSECONDITIONTOTITLE			], m_bUseConditiontoTitle,		_DPV_BUSECONDITIONTOTITLE		);
		CHART_CONVERT_BOOL(		p_pIPropertyMap, g_anPropertyShortKey[	BUSEDATATOTITLE					], m_bUseDatatoTitle,			_DPV_BUSEDATATOTITLE			);
		CHART_CONVERT_INT(		p_pIPropertyMap, g_anPropertyShortKey[	nHorzGap						], m_nHorzGap,					_DPV_nHorzGap					);
		CHART_CONVERT_INT(		p_pIPropertyMap, g_anPropertyShortKey[	nVertGap						], m_nVertGap,					_DPV_nVertGap					);
		CHART_CONVERT_INT(		p_pIPropertyMap, g_anPropertyShortKey[	nMinDataCountOnOnePage			], m_nMinDataCountOnOnePage,	_DPV_nMinDataCountOnOnePage		);	// (2003.11.12, 배승원) 한 화면에 보일 최소 Data 수를 Property로 관리하여 Map에 저장되도록 한다.
		CHART_CONVERT_INT(		p_pIPropertyMap, g_anPropertyShortKey[	BONGCOUNTMARGIN					], m_nBongCountMargin,			_DPV_BONGCOUNTMARGIN			);
		CHART_CONVERT_BOOL(		p_pIPropertyMap, g_anPropertyShortKey[	DISPLAYTRDATA					], m_bDisplayTRdata,			_DPV_DISPLAYTRDATA				);	//sy 2005.11.25.
	CHART_CONVERT_END()

	// (2006/7/5 - Seung-Won, Bae) Manage Property Map for Update
	m_pIPropertyMap = p_pIPropertyMap;
}

BOOL CViewDesignPage::OnApply() 
{
	// (2006/7/5 - Seung-Won, Bae) Use IPropertyMap
	CHART_ADDTOSTRLIST_START( TRUE)
		CHART_ADDTOSTRLIST_INT(		m_pIPropertyMap, g_anPropertyShortKey[	TYPEBORDER				], m_nBorderShape,				_DPV_TYPEBORDER				);
		CHART_ADDTOSTRLIST_BOOL(	m_pIPropertyMap, g_anPropertyShortKey[	BWHOLEVIEW				], m_bDisplayTotalData,			_DPV_BWHOLEVIEW				);
		CHART_ADDTOSTRLIST_INT(		m_pIPropertyMap, g_anPropertyShortKey[	NONEPAGEDATACOUNT		], m_nDataCountPerOnePage,		_DPV_NONEPAGEDATACOUNT		);
		CHART_ADDTOSTRLIST_INT(		m_pIPropertyMap, g_anPropertyShortKey[	NLEFTMARGIN				], m_nLeftSpace,				_DPV_NLEFTMARGIN			);
		CHART_ADDTOSTRLIST_INT(		m_pIPropertyMap, g_anPropertyShortKey[	NRIGHTMARGIN			], m_nRightSpace,				_DPV_NRIGHTMARGIN			);
		CHART_ADDTOSTRLIST_INT(		m_pIPropertyMap, g_anPropertyShortKey[	NTOPMARGIN				], m_nTopSpace,					_DPV_NTOPMARGIN				);
		CHART_ADDTOSTRLIST_INT(		m_pIPropertyMap, g_anPropertyShortKey[	NBOTTOMMARGIN			], m_nBottomSpace,				_DPV_NBOTTOMMARGIN			);
		CHART_ADDTOSTRLIST_INT(		m_pIPropertyMap, g_anPropertyShortKey[	NMAGINBORDERTYPE		], m_nMarginBorderType,			_DPV_NMAGINBORDERTYPE		);
		CHART_ADDTOSTRLIST_BOOL(	m_pIPropertyMap, g_anPropertyShortKey[	BUSECONDITIONTOTITLE	], m_bUseConditiontoTitle,		_DPV_BUSECONDITIONTOTITLE	);
		CHART_ADDTOSTRLIST_BOOL(	m_pIPropertyMap, g_anPropertyShortKey[	BUSEDATATOTITLE			], m_bUseDatatoTitle,			_DPV_BUSEDATATOTITLE		);
		CHART_ADDTOSTRLIST_INT(		m_pIPropertyMap, g_anPropertyShortKey[	nHorzGap				], m_nHorzGap,					_DPV_nHorzGap				);
		CHART_ADDTOSTRLIST_INT(		m_pIPropertyMap, g_anPropertyShortKey[	nVertGap				], m_nVertGap,					_DPV_nVertGap				);
		CHART_ADDTOSTRLIST_INT(		m_pIPropertyMap, g_anPropertyShortKey[	nMinDataCountOnOnePage	], m_nMinDataCountOnOnePage,	_DPV_nMinDataCountOnOnePage	);
		CHART_ADDTOSTRLIST_INT(		m_pIPropertyMap, g_anPropertyShortKey[	BONGCOUNTMARGIN			], m_nBongCountMargin,			_DPV_BONGCOUNTMARGIN		);
		CHART_ADDTOSTRLIST_BOOL(	m_pIPropertyMap, g_anPropertyShortKey[	DISPLAYTRDATA			], m_bDisplayTRdata,			_DPV_DISPLAYTRDATA			);
	CHART_ADDTOSTRLIST_END

	return CPropertyPage::OnApply();
}

// (2008/1/14 - Seung-Won, Bae) for MultiLanguage.
void CViewDesignPage::SetOcxHwnd( HWND p_hOcxWnd)
{
	m_hOcxWnd = p_hOcxWnd;
	m_toolTip.SetOcxHwnd( p_hOcxWnd);
}
