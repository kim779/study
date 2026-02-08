// ViewDesignPage2.cpp : implementation file
//

#include "stdafx.h"
#include "ChartDesignPropertyAddIn.h"
#include "ViewDesignPage2.h"

#include "../Include_Chart/xUtil_Chart.h"		// for CHART_CONVERT_BEGIN
#include "../Include_Chart/IPropertyMap.h"		// for IPropertyMap
#include "PropertiesData.h"
#include "ChartPropertySheet.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CViewDesignPage2 property page

IMPLEMENT_DYNCREATE(CViewDesignPage2, CPropertyPage)

CViewDesignPage2::CViewDesignPage2() : CPropertyPage(CViewDesignPage2::IDD),
	FILETYPE(".cht")
{
	//{{AFX_DATA_INIT(CViewDesignPage2)
	m_strUserEnvironmentSaveFileName = _T("");
	m_strInsertedIndicatorSaveFileName = _T("");
	m_nUserInsertedBlockCount = 0;
	m_nRButtonMenuType = -1;
	m_bHide = FALSE;
	m_bInputRequire = FALSE;
	m_bChartRButtonMenu = FALSE;
	m_bGraphDragandDrop = FALSE;
	m_bUseShortKey = FALSE;
	m_bBlockMoveDelete = FALSE;
	m_bSelectTitle = FALSE;
	m_bSelectGraph = FALSE;
	m_nGraphRButtonType = -1;
	m_bIsEnvironmentFile = FALSE;
	m_bIsIndicondFile = FALSE;
	m_bUseSkin = FALSE;
	m_strSaveMapName = _T("");
	//}}AFX_DATA_INIT
	m_nRButtonMenuType = 0;
	m_bShowWarningMessageBox = TRUE;
	m_bControlBlockSize = TRUE;
	m_bUseScaleRButtonMenu = TRUE;
	m_nGraphRButtonType = 1;

	// (2006/7/5 - Seung-Won, Bae) Manage Property Map for Update
	m_pIPropertyMap = NULL;

	// (2008/1/14 - Seung-Won, Bae) for MultiLanguage.
	m_hOcxWnd = NULL;
}

CViewDesignPage2::~CViewDesignPage2()
{
}

void CViewDesignPage2::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CViewDesignPage2)
	DDX_Control(pDX, IDC_CHARTRBUTTONMENU, m_ChartRButtonMenu_Bt);
	DDX_Control(pDX, IDC_USERINSERTEDBLOCKCOUNT, m_UserAcceptBlock_Edit);
	DDX_Control(pDX, IDC_INDICATOR_SAVE_FILE_NAME, m_Indicatorsave_Edit);
	DDX_Control(pDX, IDC_ENVIRONMENT_SAVE_FILE_NAME, m_EnvironMent_Edit);
	DDX_Text(pDX, IDC_ENVIRONMENT_SAVE_FILE_NAME, m_strUserEnvironmentSaveFileName);
	DDX_Text(pDX, IDC_INDICATOR_SAVE_FILE_NAME, m_strInsertedIndicatorSaveFileName);
	DDX_Text(pDX, IDC_USERINSERTEDBLOCKCOUNT, m_nUserInsertedBlockCount);
	DDX_Radio(pDX, IDC_RBUTTONMENUTYPE, m_nRButtonMenuType);
	DDX_Check(pDX, IDC_HIDE, m_bHide);
	DDX_Check(pDX, IDC_INPUT_REQUIRE, m_bInputRequire);
	DDX_Check(pDX, IDC_CHARTRBUTTONMENU, m_bChartRButtonMenu);
	DDX_Check(pDX, IDC_GRAPHDRAGANDDROP, m_bGraphDragandDrop);
	DDX_Check(pDX, IDC_USESHORTKEY, m_bUseShortKey);
	DDX_Check(pDX, IDC_CHECK_MESSAGEBOX, m_bShowWarningMessageBox);
	DDX_Check(pDX, IDC_CHECK_BLOCKCOMPART, m_bControlBlockSize);
	DDX_Check(pDX, IDC_CHECK_BLOCK, m_bBlockMoveDelete);
	DDX_Check(pDX, IDC_CHECK_GRAPHTITLE, m_bSelectTitle);
	DDX_Check(pDX, IDC_CHECK_GRAPH, m_bSelectGraph);
	DDX_Check(pDX, IDC_CHECK_SCALE, m_bUseScaleRButtonMenu);
	DDX_Radio(pDX, IDC_GRAPH_RBUTTONMENUTYPE, m_nGraphRButtonType);
	DDX_Check(pDX, IDC_ENVIRONMENTCHECK, m_bIsEnvironmentFile);
	DDX_Check(pDX, IDC_INDICONDCHECK, m_bIsIndicondFile);
	DDX_Check(pDX, IDC_USESKIN, m_bUseSkin);
	DDX_Text(pDX, IDC_SAVE_MAPNAME, m_strSaveMapName);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CViewDesignPage2, CPropertyPage)
	//{{AFX_MSG_MAP(CViewDesignPage2)
	ON_EN_CHANGE(IDC_USERINSERTEDBLOCKCOUNT, OnChangeUserinsertedblockcount)
	ON_BN_CLICKED(IDC_CHARTRBUTTONMENU, OnChartrbuttonmenu)
	ON_BN_CLICKED(IDC_INDICONDCHECK, OnIndicondcheck)
	ON_BN_CLICKED(IDC_ENVIRONMENTCHECK, OnEnvironmentcheck)
	ON_BN_CLICKED(IDC_USESKIN, OnUseSkin)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CViewDesignPage2 message handlers

//Create tooltip object and Add text to StringArray 
void CViewDesignPage2::CreateToolTip()
{
	ML_SET_LANGUAGE_RES();
	m_toolTip.Create( this );
	CDWordArray dwaInfo;

	dwaInfo.RemoveAll();
	dwaInfo.Add( IDS_USERINSERTEDBLOCKCOUNT_TOOLTIP);
	m_toolTip.AddControlInfo( IDC_USERINSERTEDBLOCKCOUNT, dwaInfo);

	dwaInfo.RemoveAll();
	dwaInfo.Add( IDS_CHARTRBUTTONMENU_TOOLTIP_1);
	dwaInfo.Add( IDS_CHARTRBUTTONMENU_TOOLTIP_2);
	m_toolTip.AddControlInfo( IDC_CHARTRBUTTONMENU, dwaInfo);

	dwaInfo.RemoveAll();
	dwaInfo.Add( IDS_GRAPHDRAGANDDROP_TOOLTIP);
	m_toolTip.AddControlInfo( IDC_GRAPHDRAGANDDROP, dwaInfo);

	dwaInfo.RemoveAll();
	dwaInfo.Add( IDS_CONTROLHIDE_TOOLTIP);
	m_toolTip.AddControlInfo( IDC_HIDE, dwaInfo );	

	dwaInfo.RemoveAll();
	dwaInfo.Add( IDS_INPUT_REQUIRE_TOOLTIP);
	m_toolTip.AddControlInfo( IDC_INPUT_REQUIRE, dwaInfo );
	
	dwaInfo.RemoveAll();
	dwaInfo.Add( IDS_INDICATOR_SAVE_FILE_NAME_TOOLTIP);
	m_toolTip.AddControlInfo( IDC_STATIC35, dwaInfo );		
	m_toolTip.AddControlInfo( IDC_INDICATOR_SAVE_FILE_NAME, dwaInfo );		

	dwaInfo.RemoveAll();
	dwaInfo.Add( IDS_ENVIRONMENT_SAVE_FILE_NAME_TOOLTIP);
	m_toolTip.AddControlInfo( IDC_STATIC37, dwaInfo );		
	m_toolTip.AddControlInfo( IDC_ENVIRONMENT_SAVE_FILE_NAME, dwaInfo );		
}

BOOL CViewDesignPage2::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();

	CreateToolTip();
	if(m_bChartRButtonMenu == false)
	{
		//sy 2005.04.26.
		EnableWindow(IDC_RBUTTONMENUTYPE, false);
		EnableWindow(IDC_RBUTTONTYPE_COMPLEX, false);
	}

	//sy 2003.7.28.
	EnableWindow(m_strInsertedIndicatorSaveFileName, m_Indicatorsave_Edit);
	EnableWindow(m_strUserEnvironmentSaveFileName, m_EnvironMent_Edit);

	//sy 2005.04.26.
	OnUseSkin();
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CViewDesignPage2::OnChangeUserinsertedblockcount() 
{
	CString str;
	CEdit* pBlockCount = (CEdit* )GetDlgItem(IDC_USERINSERTEDBLOCKCOUNT);
	pBlockCount->GetWindowText(str);

	m_nUserInsertedBlockCount = atoi(str);
	if(m_nUserInsertedBlockCount <1 || m_nUserInsertedBlockCount>10)
	{
		ML_SET_LANGUAGE_RES();
		CString strMsg;
		strMsg.LoadString( IDS_ENTER_NUMBER_1TO10);
		AfxMessageBox( strMsg);
	}
}

void CViewDesignPage2::OnChartrbuttonmenu() 
{
	int nCheck = m_ChartRButtonMenu_Bt.GetCheck();
	CButton* pType = (CButton*) GetDlgItem(IDC_RBUTTONMENUTYPE);
	CButton* pType2 = (CButton*) GetDlgItem(IDC_RBUTTONTYPE_COMPLEX);

	if(nCheck == 0) {//uncheck
		pType->EnableWindow(FALSE);
		pType2->EnableWindow(FALSE);
	}
	else {
		pType->EnableWindow(TRUE);
		pType2->EnableWindow(TRUE);
	}

}

CString CViewDesignPage2::MakeFileName(CString& strFileName)
{
	if(strFileName.Find(".") >= 0) {
		strFileName.Delete(strFileName.Find("."), strFileName.GetLength() - strFileName.Find("."));
		return strFileName += FILETYPE;
	}
	else if(strFileName.IsEmpty())
		return "";
	else
		return strFileName += FILETYPE;
}

//sy 2003.7.28
void CViewDesignPage2::OnIndicondcheck() 
{
	UpdateData();
	if(m_bIsIndicondFile){
		m_strInsertedIndicatorSaveFileName = "Indicond_" + m_strUserEnvironmentSaveFileName;
		m_Indicatorsave_Edit.EnableWindow(TRUE);
		m_Indicatorsave_Edit.SetFocus();
	}
	else{
		m_strInsertedIndicatorSaveFileName = "";
		m_Indicatorsave_Edit.EnableWindow(FALSE);
	}
	UpdateData(FALSE);
}

void CViewDesignPage2::OnEnvironmentcheck() 
{
	UpdateData();
	if(m_bIsEnvironmentFile){
		m_strUserEnvironmentSaveFileName = m_strInsertedIndicatorSaveFileName;
		m_strUserEnvironmentSaveFileName.Replace("Indicond_", "");
		m_EnvironMent_Edit.EnableWindow(TRUE);
		m_EnvironMent_Edit.SetFocus();
	}
	else{
		m_strUserEnvironmentSaveFileName = "";
		m_EnvironMent_Edit.EnableWindow(FALSE);
	}
	UpdateData(FALSE);
}

void CViewDesignPage2::SetProperty_FileName(const CString& strItemValue, CString& strItem, BOOL& bCheckItem)
{
	strItem = strItemValue;
	if(!strItem.IsEmpty() && strItem.Find(FILETYPE) != -1)
		strItem.Replace(FILETYPE, "");

	if(strItem.IsEmpty())
		bCheckItem = FALSE;
	else
		bCheckItem = TRUE;
}

void CViewDesignPage2::EnableWindow(const CString& strItem, CEdit& editItem)
{
	if(strItem.IsEmpty())
		editItem.EnableWindow(FALSE);
	else
		editItem.EnableWindow(TRUE);
}
//sy end

//sy 2005.04.26.
void CViewDesignPage2::OnUseSkin() 
{
	UpdateData();

	// skin을 사용할 경우는 저장을 하지 않는다.
	if(m_bUseSkin)
	{
		m_bIsEnvironmentFile = false;
		m_bIsIndicondFile = false;
		m_strUserEnvironmentSaveFileName = "";
		m_strInsertedIndicatorSaveFileName = "";
		EnableWindow_SaveOption(false);
	}
	else
	{
		EnableWindow_SaveOption(true);
	}

	UpdateData(FALSE);
}

void CViewDesignPage2::EnableWindow_SaveOption(const bool bEnable)
{
	EnableWindow(m_strInsertedIndicatorSaveFileName, m_Indicatorsave_Edit);
	EnableWindow(m_strUserEnvironmentSaveFileName, m_EnvironMent_Edit);

	EnableWindow(IDC_ENVIRONMENTCHECK, bEnable);
	EnableWindow(IDC_INDICONDCHECK, bEnable);
}

void CViewDesignPage2::EnableWindow(const int nID, const bool bEnable)
{
	CWnd* pWnd = (CWnd*) GetDlgItem(nID);
	if(pWnd != NULL)
		pWnd->EnableWindow(bEnable);
}
//sy end

//Set Received Value From ocx
void CViewDesignPage2::SetProperty( IPropertyMap *p_pIPropertyMap)
{
	CString strUserEnvironmentFileName, strUserIndicatorListFileName;

	// (2006/7/5 - Seung-Won, Bae) Use IPropertyMap
	CHART_CONVERT_BEGIN()
		CHART_CONVERT_INT(		p_pIPropertyMap, g_anPropertyShortKey[	NUSERBLOCKADDCOUNT				], m_nUserInsertedBlockCount,		_DPV_NUSERBLOCKADDCOUNT				);
		CHART_CONVERT_BOOL(		p_pIPropertyMap, g_anPropertyShortKey[	BGRAPHDRAGDROP					], m_bGraphDragandDrop,				_DPV_BGRAPHDRAGDROP					);
		CHART_CONVERT_BOOL(		p_pIPropertyMap, g_anPropertyShortKey[	BADDGRAPHMENUSHOW				], m_bChartRButtonMenu,				_DPV_BADDGRAPHMENUSHOW				);
		CHART_CONVERT_STRING(	p_pIPropertyMap, g_anPropertyShortKey[	STRENVIRONMENTFILENAME			], strUserEnvironmentFileName,		_DPV_STRENVIRONMENTFILENAME			);
		CHART_CONVERT_STRING(	p_pIPropertyMap, g_anPropertyShortKey[	STRUSERINDICATORLISTFILENAME	], strUserIndicatorListFileName,	_DPV_STRUSERINDICATORLISTFILENAME	);
		CHART_CONVERT_BOOL(		p_pIPropertyMap, g_anPropertyShortKey[	BHIDE							], m_bHide,							_DPV_BHIDE							);
		CHART_CONVERT_BOOL(		p_pIPropertyMap, g_anPropertyShortKey[	BISINPUT						], m_bInputRequire,					_DPV_BISINPUT						);
		CHART_CONVERT_BOOL(		p_pIPropertyMap, g_anPropertyShortKey[	BUSESHORTKEY					], m_bUseShortKey,					_DPV_BUSESHORTKEY					);
		CHART_CONVERT_INT(		p_pIPropertyMap, g_anPropertyShortKey[	nRButtonMenuType				], m_nRButtonMenuType,				_DPV_nRButtonMenuType				);	// (2005/7/26 - Seung-Won, Bae) Default 1, see GetAllProperties2() & DoPropExchange()
		CHART_CONVERT_BOOL(		p_pIPropertyMap, g_anPropertyShortKey[	bShowWarningMessageBox			], m_bShowWarningMessageBox,		_DPV_bShowWarningMessageBox			);
		CHART_CONVERT_BOOL(		p_pIPropertyMap, g_anPropertyShortKey[	bControlBlockSize				], m_bControlBlockSize,				_DPV_bControlBlockSize				);
		CHART_CONVERT_BOOL(		p_pIPropertyMap, g_anPropertyShortKey[	bBlockMoveDelete				], m_bBlockMoveDelete,				_DPV_bBlockMoveDelete				);
		CHART_CONVERT_BOOL(		p_pIPropertyMap, g_anPropertyShortKey[	bSelectTitle					], m_bSelectTitle,					_DPV_bSelectTitle					);
		CHART_CONVERT_BOOL(		p_pIPropertyMap, g_anPropertyShortKey[	bSelectGraph					], m_bSelectGraph,					_DPV_bSelectGraph					);
		CHART_CONVERT_BOOL(		p_pIPropertyMap, g_anPropertyShortKey[	bUseScaleRButtonMenu			], m_bUseScaleRButtonMenu,			_DPV_bUseScaleRButtonMenu			);
		CHART_CONVERT_INT(		p_pIPropertyMap, g_anPropertyShortKey[	nGraphRButtonType				], m_nGraphRButtonType,				_DPV_nGraphRButtonType				);
		CHART_CONVERT_BOOL(		p_pIPropertyMap, g_anPropertyShortKey[	USESKIN							], m_bUseSkin,						_DPV_USESKIN						);	//sy 2005.04.26.
		CHART_CONVERT_STRING(	p_pIPropertyMap, g_anPropertyShortKey[	MAPNAME							], m_strSaveMapName,				_DPV_MAPNAME						);	//sy 2005.09.06.
	CHART_CONVERT_END()

	// (2006/7/5 - Seung-Won, Bae) Manage Property Map for Update
	m_pIPropertyMap = p_pIPropertyMap;

	SetProperty_FileName( strUserEnvironmentFileName,	m_strUserEnvironmentSaveFileName,	m_bIsEnvironmentFile);
	SetProperty_FileName( strUserIndicatorListFileName,	m_strInsertedIndicatorSaveFileName,	m_bIsIndicondFile);
}

BOOL CViewDesignPage2::OnApply() 
{
	m_strUserEnvironmentSaveFileName	= MakeFileName( m_strUserEnvironmentSaveFileName);
	m_strInsertedIndicatorSaveFileName	= MakeFileName( m_strInsertedIndicatorSaveFileName);

	// (2006/7/5 - Seung-Won, Bae) Use IPropertyMap
	CHART_ADDTOSTRLIST_START( TRUE)
		CHART_ADDTOSTRLIST_INT(		m_pIPropertyMap, g_anPropertyShortKey[	NUSERBLOCKADDCOUNT				], m_nUserInsertedBlockCount,			_DPV_NUSERBLOCKADDCOUNT				);
		CHART_ADDTOSTRLIST_BOOL(	m_pIPropertyMap, g_anPropertyShortKey[	BGRAPHDRAGDROP					], m_bGraphDragandDrop,					_DPV_BGRAPHDRAGDROP					);
		CHART_ADDTOSTRLIST_BOOL(	m_pIPropertyMap, g_anPropertyShortKey[	BADDGRAPHMENUSHOW				], m_bChartRButtonMenu,					_DPV_BADDGRAPHMENUSHOW				);
		CHART_ADDTOSTRLIST_STRING(	m_pIPropertyMap, g_anPropertyShortKey[	STRENVIRONMENTFILENAME			], m_strUserEnvironmentSaveFileName,	_DPV_STRENVIRONMENTFILENAME			);
		CHART_ADDTOSTRLIST_STRING(	m_pIPropertyMap, g_anPropertyShortKey[	STRUSERINDICATORLISTFILENAME	], m_strInsertedIndicatorSaveFileName,	_DPV_STRUSERINDICATORLISTFILENAME	);
		CHART_ADDTOSTRLIST_BOOL(	m_pIPropertyMap, g_anPropertyShortKey[	BHIDE							], m_bHide,								_DPV_BHIDE							);
		CHART_ADDTOSTRLIST_BOOL(	m_pIPropertyMap, g_anPropertyShortKey[	BISINPUT						], m_bInputRequire,						_DPV_BISINPUT						);
		CHART_ADDTOSTRLIST_BOOL(	m_pIPropertyMap, g_anPropertyShortKey[	BUSESHORTKEY					], m_bUseShortKey,						_DPV_BUSESHORTKEY					);
		CHART_ADDTOSTRLIST_INT(		m_pIPropertyMap, g_anPropertyShortKey[	nRButtonMenuType				], m_nRButtonMenuType,					_DPV_nRButtonMenuType				);
		CHART_ADDTOSTRLIST_BOOL(	m_pIPropertyMap, g_anPropertyShortKey[	bShowWarningMessageBox			], m_bShowWarningMessageBox,			_DPV_bShowWarningMessageBox			);
		CHART_ADDTOSTRLIST_BOOL(	m_pIPropertyMap, g_anPropertyShortKey[	bControlBlockSize				], m_bControlBlockSize,					_DPV_bControlBlockSize				);
		CHART_ADDTOSTRLIST_BOOL(	m_pIPropertyMap, g_anPropertyShortKey[	bBlockMoveDelete				], m_bBlockMoveDelete,					_DPV_bBlockMoveDelete				);
		CHART_ADDTOSTRLIST_BOOL(	m_pIPropertyMap, g_anPropertyShortKey[	bSelectTitle					], m_bSelectTitle,						_DPV_bSelectTitle					);
		CHART_ADDTOSTRLIST_BOOL(	m_pIPropertyMap, g_anPropertyShortKey[	bSelectGraph					], m_bSelectGraph,						_DPV_bSelectGraph					);
		CHART_ADDTOSTRLIST_BOOL(	m_pIPropertyMap, g_anPropertyShortKey[	bUseScaleRButtonMenu			], m_bUseScaleRButtonMenu,				_DPV_bUseScaleRButtonMenu			);
		CHART_ADDTOSTRLIST_INT(		m_pIPropertyMap, g_anPropertyShortKey[	nGraphRButtonType				], m_nGraphRButtonType,					_DPV_nGraphRButtonType				);
		CHART_ADDTOSTRLIST_BOOL(	m_pIPropertyMap, g_anPropertyShortKey[	USESKIN							], m_bUseSkin,							_DPV_USESKIN						);
		CHART_ADDTOSTRLIST_STRING(	m_pIPropertyMap, g_anPropertyShortKey[	MAPNAME							], m_strSaveMapName,					_DPV_MAPNAME						);
	CHART_ADDTOSTRLIST_END

	return CPropertyPage::OnApply();
}

// (2008/1/14 - Seung-Won, Bae) for MultiLanguage.
void CViewDesignPage2::SetOcxHwnd( HWND p_hOcxWnd)
{
	m_hOcxWnd = p_hOcxWnd;
	m_toolTip.SetOcxHwnd( p_hOcxWnd);
}
