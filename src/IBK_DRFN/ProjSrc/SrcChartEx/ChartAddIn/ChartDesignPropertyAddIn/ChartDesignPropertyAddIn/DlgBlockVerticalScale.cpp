// DlgBlockVerticalScale.cpp : implementation file
//

#include "stdafx.h"
#include "ChartDesignPropertyAddIn.h"
#include "DlgBlockVerticalScale.h"

#include <math.h>										// for HUGE_VAL

#include "../Include_Chart/Dll_Load/IMetaTable.h"		// for _LTEXT6()
#include "ChartLink.h"
#include "Conversion.h"
#include "ToolListData.h"								// for NUM_PACKETNAMEITEM

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgBlockVerticalScale dialog


CDlgBlockVerticalScale::CDlgBlockVerticalScale( CChartLink *pItem, HWND p_hOcxWnd, CWnd* pParent /*=NULL*/)
	: CDialog(CDlgBlockVerticalScale::IDD, pParent)
{
	// (2008/1/14 - Seung-Won, Bae) for MultiLanguage.
	m_hOcxWnd = p_hOcxWnd;

	//{{AFX_DATA_INIT(CDlgBlockVerticalScale)
	m_nScalePosition = 2;
	m_strScaleData = _T("");
	m_nScaleCompartType = -1;
	m_nScaleExpressionType = 0;
	m_nMinMaxType = 0;
	m_dMax = 0.0;
	m_dMin = 0.0;
	m_bMinMaxLock = FALSE;
	m_nDownSpace = 0;
	m_nUpSpace = 0;
	m_dHogaNumber = 0.0;
	m_dUserNumber = 0.0;
	m_nUserHogaType = 0;
	m_bIsInvert = FALSE;
	//}}AFX_DATA_INIT
	m_pItem = pItem;

	//sy 2004.3.12.
	if(pItem != NULL)
	{
		m_nScalePosition = pItem->m_nScalePosition;
		m_strScaleData = pItem->m_strScaleStyle; 
		m_nScaleCompartType = pItem->m_nScaleCompartType;
		m_nScaleExpressionType = pItem->m_nScaleExpressionType;

		// (2006/11/11 - Seung-Won, Bae) Min/Max Setting
		m_bMinMaxLock	= pItem->m_bMinMaxLock;
		m_nMinMaxType	= pItem->m_eMinMaxType;
		m_dMax			= pItem->m_dMax;
		m_dMin			= pItem->m_dMin;

		// (2007/7/19 - Seung-Won, Bae) Support Up/Down Space
		m_nUpSpace		= pItem->m_nUpSpace;
		m_nDownSpace	= pItem->m_nDownSpace;

		//2007.01.22 Scale사용자 설정,호가단위별
		m_dHogaNumber = pItem->m_dHogaNumber;
		m_dUserNumber = pItem->m_dUserNumber;
		m_nUserHogaType = pItem->m_nUserHogaType;

		// 지표 뒤집기 - ojtaso (20071023)
		m_bIsInvert = pItem->m_bIsInvert;
	}
}


void CDlgBlockVerticalScale::DoDataExchange(CDataExchange* pDX)
{
// (2007/7/16 - Seung-Won, Bae) Check Invalid Value.
BOOL bException = FALSE;
TRY
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgBlockVerticalScale)
	DDX_Control(pDX, IDC_COMBO_SACLESTYLE, m_ScaleDataComboBox);
	DDX_Radio(pDX, IDC_SCALEPOS, m_nScalePosition);
	DDX_CBString(pDX, IDC_COMBO_SACLESTYLE, m_strScaleData);
	DDX_Radio(pDX, IDC_GRID_SHAPE, m_nScaleCompartType);
	DDX_Radio(pDX, IDC_GRID_EXPRESSIONTYPE, m_nScaleExpressionType);
	DDX_Radio(pDX, IDC_RDO_MINMAX_TYPE0, m_nMinMaxType);
	DDX_Text(pDX, IDC_EDT_MAX, m_dMax);
	DDX_Text(pDX, IDC_EDT_MIN, m_dMin);
	DDX_Check(pDX, IDC_CHK_LOCK, m_bMinMaxLock);
	DDX_Text(pDX, IDC_EDT_DSPACE, m_nDownSpace);
	DDV_MinMaxInt(pDX, m_nDownSpace, 0, 90);
	DDX_Text(pDX, IDC_EDT_USPACE, m_nUpSpace);
	DDV_MinMaxInt(pDX, m_nUpSpace, 0, 90);
	DDX_Text(pDX, IDC_HOGASCALE_EDIT, m_dHogaNumber);
	DDX_Text(pDX, IDC_USERSCALE_EDIT, m_dUserNumber);
	DDX_Radio(pDX, IDC_RDO_USERHOGATYPE0, m_nUserHogaType);
	DDX_Check(pDX, IDC_INVERT, m_bIsInvert);
	//}}AFX_DATA_MAP
}
AND_CATCH_ALL(e)
{
	// validation failed due to OOM or other resource failure
	bException = TRUE;
}
END_CATCH_ALL

	// (2007/7/16 - Seung-Won, Bae) Do not show the INF Value in UI.
	if( pDX->m_bSaveAndValidate || ( -HUGE_VAL != m_dMax && -HUGE_VAL != m_dMin))
	{
		DDX_Text(pDX, IDC_EDT_MAX, m_dMax);
		DDX_Text(pDX, IDC_EDT_MIN, m_dMin);
	}

	// (2007/7/16 - Seung-Won, Bae) Support Up/Down Space.
	int nPreUpSpace		= m_nUpSpace;
	int nPreDownSpace	= m_nDownSpace;
	if( m_nUpSpace < 0)		m_nUpSpace = 0;
	if( 90 < m_nUpSpace)	m_nUpSpace = 90;
	if( m_nDownSpace < 0)	m_nDownSpace = 0;
	if( 90 < m_nDownSpace)	m_nDownSpace = 90;
	CheckValue( nPreUpSpace,	m_nUpSpace,		IDC_EDT_USPACE);
	CheckValue( nPreDownSpace,	m_nDownSpace,	IDC_EDT_DSPACE);
	if( 90 < m_nUpSpace + m_nDownSpace)
	{
		ML_SET_LANGUAGE_RES();
		double dFactor = 90. / ( m_nUpSpace + m_nDownSpace);
		m_nUpSpace		= int( m_nUpSpace	* dFactor);
		m_nDownSpace	= int( m_nDownSpace	* dFactor);
		CString strMsg;
		strMsg.LoadString( IDS_INDENT_LIMIT_90);
		AfxMessageBox( strMsg);
		bException = TRUE;
	}
	CheckValue( nPreUpSpace,	m_nUpSpace,		IDC_EDT_USPACE);
	CheckValue( nPreDownSpace,	m_nDownSpace,	IDC_EDT_DSPACE);

	if( bException) AfxThrowUserException();
}

void CDlgBlockVerticalScale::CheckValue( int p_nPreSpace, int p_nNewSpace, int p_nID)
{
	if( p_nPreSpace != p_nNewSpace)
	{
		CString strData;
		strData.Format( "%d", p_nNewSpace);
		GetDlgItem( p_nID)->SetWindowText( strData);
	}
}


BEGIN_MESSAGE_MAP(CDlgBlockVerticalScale, CDialog)
	//{{AFX_MSG_MAP(CDlgBlockVerticalScale)
	ON_CBN_SELCHANGE(IDC_COMBO_SACLESTYLE, OnSelchangeComboSaclestyle)
	//}}AFX_MSG_MAP
	ON_CONTROL_RANGE( BN_CLICKED, IDC_RDO_MINMAX_TYPE0, IDC_RDO_MINMAX_TYPE2, OnRdoMinmaxType)
	ON_CONTROL_RANGE( BN_CLICKED, IDC_RDO_USERHOGATYPE0, IDC_RDO_USERHOGATYPE1, OnRdoUserHogaType)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgBlockVerticalScale message handlers


void CDlgBlockVerticalScale::SetAllItemInfoOfPacketPage(const CString &strAllItemInfoOfPacketPage)
{
	m_strAllItemInfoOfPacketPage = strAllItemInfoOfPacketPage;
}


void CDlgBlockVerticalScale::AddStringToScaleDataComboBox()
{
	m_ScaleDataComboBox.AddString( _LTEXT6( C6_TEXT));
	m_ScaleDataComboBox.AddString( _LTEXT6( C6_INDICATOR_DATA));
	m_ScaleDataComboBox.AddString( _LTEXT6( C6_PERCENT));

	//사용자가 입력한 packet에 대해서 콤보리스트에 추가.
	CString _szData = m_strAllItemInfoOfPacketPage;
	int nCount = CDataConversion::GetStringDataCount(_szData, "\r\n");
	for(int i =0 ; i< nCount; i++) {
		CString szItemData = CDataConversion::GetStringData(_szData, "\r\n");
 		int nCheck= atoi(CDataConversion::GetStringData(szItemData, ";"));
		if(nCheck) {
			bool bUserDefinePacketItem = true;
			CString selectedData = CDataConversion::GetStringData(szItemData, ";");
			for(int j = 0 ; j < NUM_PACKETNAMEITEM ; j++) {
				if(selectedData.Compare( _MTEXTN( ePacketNameItem[j])) == 0) 
					bUserDefinePacketItem = false;
			}
			if(bUserDefinePacketItem == true)
			{
				CString strScaleData = _MTOL6_N( selectedData);
				if( strScaleData.IsEmpty()) strScaleData = _MTOL0( selectedData);
				m_ScaleDataComboBox.AddString( strScaleData);
			}
		}
	}

	CheckingItems();
}

void CDlgBlockVerticalScale::CheckingItems()
{
	CString _szData = m_strScaleData;

	if(_szData.Right(1) != ",")
		_szData += ",";

	CString szItemData;
	int nCount = CDataConversion::GetStringDataCount(_szData, ",");
	for(int i = 0 ; i < nCount ; i++) {
       szItemData = CDataConversion::GetStringData(_szData, ",");
	   szItemData.TrimLeft();
	   int nIndex = 0;
	   if((nIndex=m_ScaleDataComboBox.FindString(-1, szItemData)) != LB_ERR)
			m_ScaleDataComboBox.SetCheck(nIndex, TRUE);
	}


}

BOOL CDlgBlockVerticalScale::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	AddStringToScaleDataComboBox();
	m_ScaleDataComboBox.m_hListBox = 0;

	// (2006/11/11 - Seung-Won, Bae) Min/Max Type Init.
	OnRdoMinmaxType( m_nMinMaxType == 2 ? IDC_RDO_MINMAX_TYPE2 : 0);
	//2007.01.22 Scale사용자 설정,호가단위별
	OnRdoUserHogaType(m_nUserHogaType == 0 ? IDC_RDO_USERHOGATYPE0 : IDC_RDO_USERHOGATYPE1);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgBlockVerticalScale::OnSelchangeComboSaclestyle() 
{
/*	int nIndex = m_ScaleDataComboBox.GetCurSel();
	
	if(nIndex == 0 || nIndex == 1 || nIndex == 2) {
		if(m_ScaleDataComboBox.GetCheck(nIndex) == true) {
			m_ScaleDataComboBox.SetCheck(0, 0);
			m_ScaleDataComboBox.SetCheck(1, 0);
			m_ScaleDataComboBox.SetCheck(2, 0);

			m_ScaleDataComboBox.SetCheck(nIndex, 1);
		}
	}
	else {
		if(m_ScaleDataComboBox.GetCheck(nIndex) == true) {
			m_ScaleDataComboBox.SetCheck(0, 0);
			m_ScaleDataComboBox.SetCheck(1, 0);
			m_ScaleDataComboBox.SetCheck(2, 0);
		}
	}
*/
	PostMessage(WM_SETFOCUS);		
}

// (2006/11/11 - Seung-Won, Bae) Min/Max Type Changed.
void CDlgBlockVerticalScale::OnRdoMinmaxType( UINT nID) 
{
	// TODO: Add your control notification handler code here
	GetDlgItem( IDC_EDT_MIN)->EnableWindow( nID == IDC_RDO_MINMAX_TYPE2);
	GetDlgItem( IDC_EDT_MAX)->EnableWindow( nID == IDC_RDO_MINMAX_TYPE2);

	// (2007/7/16 - Seung-Won, Bae) Support Up/Down Space.
	GetDlgItem( IDC_EDT_USPACE)->EnableWindow( nID != IDC_RDO_MINMAX_TYPE2);
	GetDlgItem( IDC_EDT_DSPACE)->EnableWindow( nID != IDC_RDO_MINMAX_TYPE2);
}

//2007.01.22 Scale사용자 설정,호가단위별
void CDlgBlockVerticalScale::OnRdoUserHogaType( UINT nID) 
{
	// TODO: Add your control notification handler code here
	GetDlgItem( IDC_USERSCALE_EDIT)->EnableWindow( nID == IDC_RDO_USERHOGATYPE0);
	GetDlgItem( IDC_HOGASCALE_EDIT)->EnableWindow( nID == IDC_RDO_USERHOGATYPE1);
}
