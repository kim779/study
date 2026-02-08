// DlgBlockHorizontalScale.cpp : implementation file
//

#include "stdafx.h"
#include "ChartDesignPropertyAddIn.h"
#include "DlgBlockHorizontalScale.h"

#include "../Include_Chart/Dll_Load/IMetaTable.h"		// for _LTEXT6()

#include "ChartLink.h"
#include "Conversion.h"
#include "ToolListData.h"			// for BLOCK

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgBlockHorizontalScale dialog


CDlgBlockHorizontalScale::CDlgBlockHorizontalScale(CChartLink *pItem, HWND p_hOcxWnd, CWnd* pParent /*=NULL*/)
	: CDialog(CDlgBlockHorizontalScale::IDD, pParent)
{
	// (2008/1/14 - Seung-Won, Bae) for MultiLanguage.
	m_hOcxWnd = p_hOcxWnd;
	m_toolTip.SetOcxHwnd( m_hOcxWnd);

	m_strScaleData = _LTEXT6( C6_DATE_TIME);
	//{{AFX_DATA_INIT(CDlgBlockHorizontalScale)
	m_nHorizonScalePos = -1;
	m_strDateCompartType = _T("");
	m_StrTypeTimeCompartType = _T("");
	m_nStyleHorizonScale = -1;
	m_nScaleCompartType = -1;
	//}}AFX_DATA_INIT
	m_pItem = pItem;

	//sy 2004.3.12.
	if(pItem != NULL)
	{
		m_nHorizonScalePos = pItem->m_nHorizonpos;
		m_strDateCompartType = pItem->m_strDateScaleCompart;
		m_StrTypeTimeCompartType = pItem->m_strTimeScaleCompart;
		m_nStyleHorizonScale = pItem->m_nHorizonStyle;
		m_strScaleData = pItem->m_strScaleStyle;
		m_nScaleCompartType = pItem->m_nScaleCompartType;
	}
}


void CDlgBlockHorizontalScale::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgBlockHorizontalScale)
	DDX_Control(pDX, IDC_COMBO_HORIZONSACLESTYLE, m_ScaleDataComboBox);
	DDX_Control(pDX, IDC_DATADATESCALESTYLE, m_DateCompartComboBox);
	DDX_Control(pDX, IDC_DATAHOURSCALESTYLE, m_TypeTimeCompartComboBox);
	DDX_Control(pDX, IDC_TEXTSCALESTYLE, m_TextCombo);
	DDX_Radio(pDX, IDC_RADIO_SCALEPOSITION, m_nHorizonScalePos);
	DDX_CBString(pDX, IDC_DATADATESCALESTYLE, m_strDateCompartType);
	DDX_CBString(pDX, IDC_DATAHOURSCALESTYLE, m_StrTypeTimeCompartType);
	DDX_CBIndex(pDX, IDC_TEXTSCALESTYLE, m_nStyleHorizonScale);
	DDX_CBString(pDX, IDC_COMBO_HORIZONSACLESTYLE, m_strScaleData);
	DDX_Radio(pDX, IDC_GRIDSHAPE, m_nScaleCompartType);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgBlockHorizontalScale, CDialog)
	//{{AFX_MSG_MAP(CDlgBlockHorizontalScale)
	ON_CBN_SELCHANGE(IDC_COMBO_HORIZONSACLESTYLE, OnSelchangeComboHorizonsaclestyle)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgBlockHorizontalScale message handlers

BOOL CDlgBlockHorizontalScale::OnInitDialog() 
{
	CDialog::OnInitDialog();

	ML_SET_LANGUAGE_RES();
	m_DateCompartComboBox.AddString(".");
	m_DateCompartComboBox.AddString("-");
	m_DateCompartComboBox.AddString(":");
	m_DateCompartComboBox.AddString("/");
	CString strCompart;
	strCompart.LoadString( IDS_YEAR_MONTH_DAY);
	m_DateCompartComboBox.AddString( strCompart);

	m_TypeTimeCompartComboBox.AddString(".");
	m_TypeTimeCompartComboBox.AddString("-");
	m_TypeTimeCompartComboBox.AddString(":");
	m_TypeTimeCompartComboBox.AddString("/");
	strCompart.LoadString( IDS_HOUR_MINUTE_SECOND);
	m_DateCompartComboBox.AddString( strCompart);

	AddStringToScaleDataComboBox();
//	AddStringToPacketCombo();
	SetEnableWindowToTextCombo(m_ScaleDataComboBox.FindString(0, m_strScaleData));

	//툴팁 띄우기
	CreateToolTip();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

//툴팁.
void CDlgBlockHorizontalScale::CreateToolTip()
{
	ML_SET_LANGUAGE_RES();
	m_toolTip.Create( this );
	CDWordArray dwaInfo;

	dwaInfo.RemoveAll();
	dwaInfo.Add( IDS_HORIZONSACLESTYLE_TOOLTIP);
	m_toolTip.AddControlInfo( IDC_STATIC23, dwaInfo);
	m_toolTip.AddControlInfo( IDC_COMBO_HORIZONSACLESTYLE, dwaInfo);

	dwaInfo.RemoveAll();
	dwaInfo.Add( IDS_SCALEPOSITION_TOOLTIP_1);
	dwaInfo.Add( IDS_SCALEPOSITION_TOOLTIP_2);
	m_toolTip.AddControlInfo( IDC_STATIC24, dwaInfo);
	m_toolTip.AddControlInfo( IDC_RADIO_SCALEPOSITION, dwaInfo);

	dwaInfo.RemoveAll();
	dwaInfo.Add( IDS_DATADATESCALESTYLE_TOOLTIP);
	m_toolTip.AddControlInfo( IDC_STATIC27, dwaInfo);
	m_toolTip.AddControlInfo( IDC_DATADATESCALESTYLE, dwaInfo);

	dwaInfo.RemoveAll();
	dwaInfo.Add( IDS_DATAHOURSCALESTYLE_TOOLTIP);
	m_toolTip.AddControlInfo( IDC_STATIC28, dwaInfo);
	m_toolTip.AddControlInfo( IDC_DATAHOURSCALESTYLE, dwaInfo);

	dwaInfo.RemoveAll();
	dwaInfo.Add( IDS_TEXTSCALESTYLE_TOOLTIP);
	m_toolTip.AddControlInfo( IDC_STATIC29, dwaInfo);
	m_toolTip.AddControlInfo( IDC_TEXTSCALESTYLE, dwaInfo);
}

BOOL CDlgBlockHorizontalScale::PreTranslateMessage(MSG* pMsg) 
{
	POINT pt = pMsg->pt;	
	ScreenToClient( &pt );

	if( pMsg->message == WM_RBUTTONDOWN )	{
		m_toolTip.ShowToolTip( (CPoint)pt );
	}
	if(pMsg->message == WM_MOUSEMOVE){
		m_toolTip.HideToolTip( (CPoint)pt );
	}
	return CDialog::PreTranslateMessage(pMsg);
}

void CDlgBlockHorizontalScale::OnOK() 
{
	if(m_pItem->m_iType == ROOT)
	{
		ML_SET_LANGUAGE_RES();
		CString text = "";
		text.LoadString( IDS_BLOCK_SETTING_SET_ALL);
		CString strTitle;
		strTitle.LoadString( IDS_BLOCK_SETTING);
		int nOk = MessageBox( text, strTitle, MB_OKCANCEL | MB_ICONQUESTION);
		if(nOk == IDOK)
			CDialog::OnOK();
		else
			CDialog::OnCancel();
	}
	else
		CDialog::OnOK();
}

void CDlgBlockHorizontalScale::OnSelchangeComboHorizonsaclestyle() 
{
	int nIndex = m_ScaleDataComboBox.GetCurSel();
	SetEnableWindowToTextCombo(nIndex);		
}

void CDlgBlockHorizontalScale::SetEnableWindowToTextCombo(const int &nIndex)
{
	//항상 가로로..
	if( !m_ScaleDataComboBox.GetItemData( nIndex))
	{
		m_nStyleHorizonScale = 0;	
		m_TextCombo.SetCurSel(0);
		m_TextCombo.EnableWindow(FALSE);
	}
	else
		m_TextCombo.EnableWindow(TRUE);
}

void CDlgBlockHorizontalScale::AddStringToScaleDataComboBox()
{
	m_ScaleDataComboBox.AddString(	_LTEXT6( C6_DATE_TIME));
	m_ScaleDataComboBox.SetItemData( m_ScaleDataComboBox.GetCount() - 1, 0);
	m_ScaleDataComboBox.AddString(	_LTEXT6( C6_TEXT));
	m_ScaleDataComboBox.SetItemData( m_ScaleDataComboBox.GetCount() - 1, 1);
	m_ScaleDataComboBox.AddString(	_LTEXT6( C6_EXERCISED_PRICE));
	m_ScaleDataComboBox.SetItemData( m_ScaleDataComboBox.GetCount() - 1, 0);
	m_ScaleDataComboBox.AddString(	_LTEXT0( C0_INVERSETIMELINE_VOLUME));
	m_ScaleDataComboBox.SetItemData( m_ScaleDataComboBox.GetCount() - 1, 0);
	m_ScaleDataComboBox.AddString(	_LTEXT3( C3_MARKET_PROFILE));
	m_ScaleDataComboBox.SetItemData( m_ScaleDataComboBox.GetCount() - 1, 0);

	//사용자가 입력한 packet에 대해서 콤보리스트에 추가.
	CString _szData = m_strAllItemInfoOfPacketPage;
	int nCount = CDataConversion::GetStringDataCount(_szData, "\r\n");
	//사용자가 입력한 scale 형태와 구분하기 위함
	if(nCount > 0)
	{
		m_ScaleDataComboBox.AddString("--------------");
		m_ScaleDataComboBox.SetItemData( m_ScaleDataComboBox.GetCount() - 1, 1);
	}

	for(int i =0 ; i< nCount; i++) {
		CString szItemData = CDataConversion::GetStringData(_szData, "\r\n");
 		int nCheck= atoi(CDataConversion::GetStringData(szItemData, ";"));
		if(nCheck) {
			bool bUserDefinePacketItem = true;
			CString selectedData = CDataConversion::GetStringData(szItemData, ";");
			for(int j = 0 ; j < NUM_PACKETNAMEITEM ; j++) {
				if(selectedData.Compare( _MTEXTN( ePacketNameItem[ j])) == 0) 
					bUserDefinePacketItem = false;
			}
			if(bUserDefinePacketItem == true)
			{
				m_ScaleDataComboBox.AddString( _MTOL0( selectedData));
				m_ScaleDataComboBox.SetItemData( m_ScaleDataComboBox.GetCount() - 1, ( 0 <= szItemData.Find( CString( ";") + _MTEXT( C6_CHARACTER) + ";")));
			}
		}
	}
}

void CDlgBlockHorizontalScale::SetAllItemInfoOfPacketPage(const CString &strAllItemInfoOfPacketPage)
{
	m_strAllItemInfoOfPacketPage = strAllItemInfoOfPacketPage;
}
