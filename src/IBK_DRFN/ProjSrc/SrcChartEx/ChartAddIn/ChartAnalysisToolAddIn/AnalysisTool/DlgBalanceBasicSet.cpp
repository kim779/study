// DlgFiboSet.cpp : implementation file
//

#include "stdafx.h"
#include "analysistool.h"
#include "DlgBalanceBasicSet.h"
#include "Conversion.h"

#include "../Include_Chart/Dll_Load/IMetaTable.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

const int m_naGBCtrlID[ _GB_VALUE_COUNT + 1 ] =
{
	IDC_EDT_GB_VALUE_1, IDC_EDT_GB_VALUE_2, IDC_EDT_GB_VALUE_3, IDC_EDT_GB_VALUE_4, IDC_EDT_GB_VALUE_5, IDC_EDT_GB_VALUE_6,
	IDC_EDT_GB_VALUE_7, IDC_EDT_GB_VALUE_8, IDC_EDT_GB_VALUE_9, IDC_EDT_GB_VALUE_10, IDC_EDT_GB_VALUE_11, IDC_EDT_GB_VALUE_12,
	0
};

/////////////////////////////////////////////////////////////////////////////
// CDlgBalanceBasicSet dialog

CDlgBalanceBasicSet::CDlgBalanceBasicSet(CWnd* pParent,
						 const CString& strUserPath,
						 const CScaleBaseData::HORZSCALEDRAWERTYPE& nDataPeriod,
						 int nElementType,
						 CTextPosition* pTextPosition,
						 CPenEnvironment* pPenEnvironment,
						 CElementPointList *pElementPointList,
						 CLineExtEnvironment* pLineExtEnvironment,
						 CDataTypeEnvironment* pDataEnvironment)
	: CDialog(CDlgBalanceBasicSet::IDD, pParent),
	m_pParent(pParent),
	m_strUserPath(strUserPath),
	m_nDataPeriod(nDataPeriod),
	m_pPenEnvironment(pPenEnvironment),
	m_basePenEnvironment(*pPenEnvironment),
	m_pElementPointList(pElementPointList),
	m_baseElementPointList(*pElementPointList)
{
	//{{AFX_DATA_INIT(CDlgBalanceBasicSet)
	m_nWidth = 0;
	m_nStyle = 0;
	//}}AFX_DATA_INIT

	m_nTextPosType			= G_TEXT_POS_ONE;
	m_nElementType			= nElementType;
	m_pDataEnvironment		= pDataEnvironment;

	m_pTextPosition = (CTextShowPosition *)pTextPosition;;
	m_baseTextPosition.Set(pTextPosition);

	m_pLineExtEnvironment = pLineExtEnvironment;
	m_baseLineExtEnvironment.Set(pLineExtEnvironment);

	m_hOcxWnd = ( pParent ? pParent->GetSafeHwnd() : NULL);
}

void CDlgBalanceBasicSet::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgBalanceBasicSet)
	DDX_Control(pDX, IDC_DATETIMEPICKER_XDATE, m_ctrlXDateTime1);
	DDX_Control(pDX, IDC_EDIT_PRICE_VALUE, m_ctrlEditValue1);
	DDX_Control(pDX, IDC_BTN_COLOR, m_ctlColor);
	DDX_Control(pDX, IDC_CMB_WIDTH, m_ctlWidth);
	DDX_CBIndex(pDX, IDC_CMB_WIDTH, m_nWidth);
	DDX_Control(pDX, IDC_CMB_STYLE, m_ctlStyle);
	DDX_CBIndex(pDX, IDC_CMB_STYLE, m_nStyle);
	DDX_Control(pDX, IDC_CHECK_SHOW_TEXT, m_chkShowText);
	DDX_Control(pDX, IDC_CHECK_EXT_LEFT, m_ctrlLeftExt);
	DDX_Control(pDX, IDC_CHECK_EXT_RIGHT, m_ctrlRightExt);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CDlgBalanceBasicSet, CDialog)
	//{{AFX_MSG_MAP(CDlgBalanceBasicSet)
	ON_BN_CLICKED(IDC_BTN_COLOR, OnColor)
	ON_BN_CLICKED(IDC_APPLY_BT, OnApplyBt)
	ON_CBN_SELCHANGE(IDC_CMB_WIDTH, OnSelchangeWidth)
	ON_CBN_SELCHANGE(IDC_CMB_STYLE, OnSelchangeStyle)
	ON_NOTIFY(DTN_DATETIMECHANGE, IDC_DATETIMEPICKER_XDATE, OnDatetimechangeDatetimepickerXdate1)
	ON_BN_CLICKED(IDC_CANCEL_BT, OnCancelBt)
	ON_BN_CLICKED(IDC_OK_BT, OnOkBt)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgBalanceBasicSet message handlers

BOOL CDlgBalanceBasicSet::OnInitDialog()
{
	CDialog::OnInitDialog();

	mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);
	mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);

	InitializeData();

	// 일목수치 계수를 설정한다.
	InitializeData_Coefficient();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgBalanceBasicSet::InitializeData_Coefficient()
{
	// 현재 설정된 계수를 얻는다.
	CString strExtData = m_pDataEnvironment->GetExtData();

	// 비었으면 기본값을 설정한다.
	if( strExtData.IsEmpty() )		
		strExtData = "9,17,26,33,42,65,76,129,172,257,0,0";

	int nIndex = 0;
	CString strGBValue("");
	while( !strExtData.IsEmpty() )
	{
		strGBValue = CDataConversion::GetStringData(strExtData, ",");
		if( strGBValue.IsEmpty() )
		{
			strGBValue = strExtData;
			strExtData.Empty();
		}

		GetDlgItem( m_naGBCtrlID[ nIndex ] )->SetWindowText( strGBValue);

		nIndex++;
	}
}

void CDlgBalanceBasicSet::SaveData()
{
	CString strFileName;
	strFileName.Format("%s%s",m_strUserPath,G_ANALYSIS_PROPERTY_INFO);
	CFileFind finder;
	BOOL bFind = finder.FindFile(strFileName);
	finder.Close();

	if(bFind != TRUE)
	{
		CFile file;
		if(file.Open(strFileName,CFile::modeCreate|CFile::modeReadWrite) == FALSE)
			return;
		file.Close();
	}

	CString strSection,strKey,strData,strName;
	strSection.Format("%d",(CToolEnumData::ElementType)m_nElementType);

	strKey = _T("LeftText");
	strData.Format(_T("%d"),m_stAnalysisTool.nLeftText);
	WritePrivateProfileString(strSection,strKey,strData,strFileName);

	strKey = _T("FillRegion");
	strData.Format(_T("%d"),m_stAnalysisTool.nFillRegion);
	WritePrivateProfileString(strSection,strKey,strData,strFileName);

	strKey = _T("LeftExt");
	strData.Format(_T("%d"),m_stAnalysisTool.nLeftExt);		
	WritePrivateProfileString(strSection,strKey,strData,strFileName);
	
	strKey = _T("RightExt");
	strData.Format(_T("%d"),m_stAnalysisTool.nRightExt);		
	WritePrivateProfileString(strSection,strKey,strData,strFileName);

	strKey = _T("LineColor");
	strData.Format(_T("%ld"),m_stAnalysisTool.crLineColor);
	WritePrivateProfileString(strSection,strKey,strData,strFileName);

	strKey = _T("LineStyle");
	strData.Format(_T("%d"),m_stAnalysisTool.nLineStyle);
	WritePrivateProfileString(strSection,strKey,strData,strFileName);

	strKey = _T("LineWeight");
	strData.Format(_T("%d"),m_stAnalysisTool.nLineWeight);
	WritePrivateProfileString(strSection,strKey,strData,strFileName);

	strKey = _T("BalanceCoefficient");
	strData = m_pDataEnvironment->GetExtData();
	WritePrivateProfileString(strSection,strKey,strData,strFileName);
}

void CDlgBalanceBasicSet::ApplyData()
{
	SetPenEnvironment();

	if(m_nTextPosType != G_TEXT_POS_NONE)
		SetTextPosition();

	SetLineExtEnvironment();

	SetDateValueLists();

	SetCoefficientValueLists();

	m_pParent->Invalidate();
}

void CDlgBalanceBasicSet::ApplyOrgData()
{
	m_pPenEnvironment->Set(m_basePenEnvironment);

	if(m_nTextPosType != G_TEXT_POS_NONE)
	{
		if(m_nTextPosType == G_TEXT_POS_ONE)
			SetTextPosition(m_baseTextPosition.IsShow());

		CTextPosition* pTextPosition = GetTextPosition();
		CTextPosition* pOrgTextPosition = GetOrgTextPosition();
		if(pTextPosition != NULL && pOrgTextPosition != NULL)
			pTextPosition->Set(pOrgTextPosition);
	}

	SetLineExtEnvironment( ( m_baseLineExtEnvironment.GetLeftExt() ? true : false),
		( m_baseLineExtEnvironment.GetRightExt() ? true : false));
		
	CLineExtEnvironment* pLineExtEnvironment = GetLineExtEnvironment();
	CLineExtEnvironment* pOrgLineExtEnvironment = GetOrgLineExtEnvironment();
	if(pLineExtEnvironment != NULL && pOrgLineExtEnvironment != NULL)
		pLineExtEnvironment->Set(pOrgLineExtEnvironment);

	m_pParent->Invalidate();
}

void CDlgBalanceBasicSet::OnColor()
{
	CColorDialog colorDlg( 0, 0, this);

	if(colorDlg.DoModal() != IDOK)
		return;

	SetColor(colorDlg.GetColor());
}

///////////////////////////////////////////////////////////////////////////////
// protected ==================================================================
void CDlgBalanceBasicSet::InitializeData()
{
	m_stAnalysisTool.crLineColor = m_pPenEnvironment->GetColor();
	m_stAnalysisTool.nLineStyle = m_pPenEnvironment->GetStyle();
	m_stAnalysisTool.nLineWeight = m_pPenEnvironment->GetWidth();
	m_stAnalysisTool.dEndXPos = G_NOTAVAILABLE;
	m_stAnalysisTool.dEndYValue = G_NOTAVAILABLE;
	m_stAnalysisTool.dStartXPos = G_NOTAVAILABLE;
	m_stAnalysisTool.dStartYValue = G_NOTAVAILABLE;
	m_stAnalysisTool.dThirdXPos = G_NOTAVAILABLE;
	m_stAnalysisTool.dThirdYValue = G_NOTAVAILABLE;
	m_stAnalysisTool.nEquiDiv = -1;
	m_stAnalysisTool.nPriceRangeDiv = -1;	// (2008/8/15 - Seung-Won, Bae) Add option for using price range.
	m_stAnalysisTool.nHLPercent = -1;
	m_stAnalysisTool.nFillRegion = -1;
	m_stAnalysisTool.nLeftExt = -1;
	m_stAnalysisTool.nRightExt = -1;
	m_stAnalysisTool.nTopText = -1;
	m_stAnalysisTool.nBottomText = -1;
	m_stAnalysisTool.nLeftText = -1;
	m_stAnalysisTool.nRightText = -1;

	SetColor(m_basePenEnvironment.GetColor());

	InitializeData_Width();
	InitializeData_Style();

	m_ctrlLeftExt.SetCheck(m_pLineExtEnvironment->GetLeftExt());
	m_ctrlRightExt.SetCheck(m_pLineExtEnvironment->GetRightExt());

	m_stAnalysisTool.nLeftExt = m_pLineExtEnvironment->GetLeftExt();
	m_stAnalysisTool.nRightExt = m_pLineExtEnvironment->GetRightExt();

	if(m_pElementPointList != NULL)
	{
		CCoordinate *pCoordinate = NULL;
		for(int i=0;i<m_pElementPointList->GetCount();i++)
		{
			pCoordinate = m_pElementPointList->GetAt(i);
			SetXDateTime(pCoordinate->GetX(),i);
			SetYValue(pCoordinate->GetY(),i);
		}
	}
	UpdateData(FALSE);

	m_chkShowText.SetCheck( m_pTextPosition->IsShow() );
}

// private ====================================================================
void CDlgBalanceBasicSet::InitializeData_Width()
{
	ML_SET_DEFAULT_RES();

	m_ctlWidth.LoadBitmap(IDB_LINEWIDTH, 5);
	if(m_basePenEnvironment.GetWidth() <= 0)
		m_nWidth = 0;
	else if(m_basePenEnvironment.GetWidth() >= 6)
		m_nWidth = 4;
	else
		m_nWidth = m_basePenEnvironment.GetWidth() -1;
}

void CDlgBalanceBasicSet::InitializeData_Style()
{
	ML_SET_DEFAULT_RES();

	m_ctlStyle.LoadBitmap(IDB_LINESTYLE, 5);
	m_nStyle = (int) m_basePenEnvironment.GetStyle();
}

void CDlgBalanceBasicSet::SetPenEnvironment()
{
	UpdateData(TRUE);
	m_pPenEnvironment->SetColor(m_ctlColor.GetColor());
	m_pPenEnvironment->SetWidth(m_nWidth+1);
	m_pPenEnvironment->SetStyle((CToolEnumData::PenStyle) m_nStyle);

	m_stAnalysisTool.crLineColor = m_ctlColor.GetColor();
	m_stAnalysisTool.nLineStyle = m_nStyle;
	m_stAnalysisTool.nLineWeight = m_nWidth+1;
}

void CDlgBalanceBasicSet::SetLineExtEnvironment()
{
	UpdateData(TRUE);
	m_pLineExtEnvironment->SetLineExt(m_ctrlLeftExt.GetCheck(),m_ctrlRightExt.GetCheck());
	
	m_stAnalysisTool.nLeftExt = m_ctrlLeftExt.GetCheck();
	m_stAnalysisTool.nRightExt = m_ctrlRightExt.GetCheck();	
}

void CDlgBalanceBasicSet::SetLineExtEnvironment(const bool bLeft,const bool bRight)
{
	m_pLineExtEnvironment->SetLineExt(bLeft,bRight);
}

void CDlgBalanceBasicSet::SetColor(const COLORREF& color)
{
	m_ctlColor.SetColor(color);
	m_ctlColor.Invalidate();
}

void CDlgBalanceBasicSet::SetTextPosition()
{
	m_stAnalysisTool.dEndXPos = G_NOTAVAILABLE;
	m_stAnalysisTool.dEndYValue = G_NOTAVAILABLE;
	m_stAnalysisTool.dStartXPos = G_NOTAVAILABLE;
	m_stAnalysisTool.dStartYValue = G_NOTAVAILABLE;
	m_stAnalysisTool.dThirdXPos = G_NOTAVAILABLE;
	m_stAnalysisTool.dThirdYValue = G_NOTAVAILABLE;
	m_stAnalysisTool.nEquiDiv = -1;
	m_stAnalysisTool.nPriceRangeDiv = -1;	// (2008/8/15 - Seung-Won, Bae) Add option for using price range.
	m_stAnalysisTool.nHLPercent = -1;
	m_stAnalysisTool.nFillRegion = -1;

	if(m_nTextPosType == G_TEXT_POS_ONE)
	{
		SetTextPosition( m_chkShowText.GetCheck() ? true : false);
		m_stAnalysisTool.nLeftText = (int)m_chkShowText.GetCheck();
	}
}

void CDlgBalanceBasicSet::SetTextPosition(const bool bShow)
{
	m_pTextPosition->GetTextEnvironment().SetColor(m_ctlColor.GetColor());
	m_pTextPosition->SetShow(bShow);
}

CTextPosition* CDlgBalanceBasicSet::GetTextPosition()
{
	if(m_nTextPosType == G_TEXT_POS_ONE)
		return m_pTextPosition;

	return NULL;
}

CTextPosition* CDlgBalanceBasicSet::GetOrgTextPosition()
{
	if(m_nTextPosType == G_TEXT_POS_ONE)
		return &m_baseTextPosition;

	return NULL;
}

CLineExtEnvironment* CDlgBalanceBasicSet::GetLineExtEnvironment()
{
	return m_pLineExtEnvironment;
}

CLineExtEnvironment* CDlgBalanceBasicSet::GetOrgLineExtEnvironment()
{
	return &m_baseLineExtEnvironment;
}

CPenEnvironment* CDlgBalanceBasicSet::GetCurrentPenEnvironment() const
{
	return m_pPenEnvironment;
}

void CDlgBalanceBasicSet::SetXDateTime(double dXDateTime,int nIndex)
{
	CString strXValue("");
	strXValue.Format(_T("%.f"),dXDateTime);

	if( strXValue.IsEmpty() 
		|| dXDateTime <= 0.0f )
		return;

	if( m_nDataPeriod == CScaleBaseData::HORZ_DAILY		
		|| m_nDataPeriod == CScaleBaseData::HORZ_WEEKLEY	
		|| m_nDataPeriod == CScaleBaseData::HORZ_MONTHLY)
	{
		int nYear = atoi(strXValue.Left(4));
		int nMonth = atoi(strXValue.Mid(4,2));
		int nDay = atoi(strXValue.Mid(6,2));

		CTime stime(nYear,nMonth,max(1,nDay),0,0,0);

		if( nIndex == 0 )
			m_ctrlXDateTime1.SetTime(&stime);
	}
	else
	{
		CString strFormat("");
		if( m_nDataPeriod == CScaleBaseData::HORZ_TICK )
		{
			strFormat.Format(_T("%s/%s:%s"),strXValue.Left(2),strXValue.Mid(2,2),strXValue.Mid(4,2));
		}
		else
		{
			if( strXValue.GetLength() < 8 )
				strXValue = "0" + strXValue;
			strFormat.Format(_T("%s/%s-%s:%s"),strXValue.Left(2),strXValue.Mid(2,2),
					strXValue.Mid(4,2),strXValue.Mid(6,2));
		}

		m_ctrlEditValue1.ShowWindow(SW_SHOW);
		m_ctrlEditValue1.SetWindowText(strFormat);
	}
}

void CDlgBalanceBasicSet::SetYValue(double dYValue,int nIndex)
{
	CString strYValue("");
	strYValue.Format(_T("%f"),dYValue);
	if( strYValue.GetLength() )
		m_ctrlEditValue1.SetWindowText(strYValue);
}

void CDlgBalanceBasicSet::OnDatetimechangeDatetimepickerXdate1(NMHDR* pNMHDR, LRESULT* pResult)
{
	CTime xTime;
	m_ctrlXDateTime1.GetTime(xTime);

	CString strXDate("");
	strXDate.Format(_T("%04d%02d%02d"),xTime.GetYear(),xTime.GetMonth(),xTime.GetDay());

	CCoordinate *pCoordinate = m_pElementPointList->GetAt(0);
	pCoordinate->SetX(atof(strXDate),0);

	*pResult = 0;
}

void CDlgBalanceBasicSet::SetDateValueLists()
{
	// 날짜
	CTime xTime;
	CCoordinate *pCoordinate = NULL;
	m_ctrlXDateTime1.GetTime(xTime);

	CString strXDate("");
	if( m_nDataPeriod == CScaleBaseData::HORZ_MONTHLY)
		strXDate.Format(_T("%04d%02d"),xTime.GetYear(),xTime.GetMonth());
	else
		strXDate.Format(_T("%04d%02d%02d"),xTime.GetYear(),xTime.GetMonth(),xTime.GetDay());
	pCoordinate = m_pElementPointList->GetAt(0);
	pCoordinate->SetX(atof(strXDate),0);
	m_stAnalysisTool.dStartXPos = atof(strXDate);

	//가격
	CString strYValue("");
	m_ctrlEditValue1.GetWindowText(strYValue);
	pCoordinate = m_pElementPointList->GetAt(0);
	pCoordinate->SetY(atof(strYValue));
	m_stAnalysisTool.dStartYValue = atof(strYValue);
}

void CDlgBalanceBasicSet::SetCoefficientValueLists()
{
	CString strData, strGBValue;
	for( int i=0; i<_GB_VALUE_COUNT; i++ )
	{
		GetDlgItem( m_naGBCtrlID[ i])->GetWindowText( strGBValue);

		if( strGBValue.IsEmpty() )
			strGBValue = "0";

		strData += strGBValue;
		strData += ",";
	}

	m_pDataEnvironment->SetExtData(strData);
}

void CDlgBalanceBasicSet::OnSelchangeWidth()
{
	int nWidth = m_ctlWidth.GetCurSel();
	int nStyle = m_ctlStyle.GetCurSel();

	if( nStyle != 0 && nWidth > 0)
	{
		ML_SET_LANGUAGE_RES();
		
		CString strNotice("");
		strNotice.LoadString(IDS_CHANGE_THICKNESS);
		AfxMessageBox(strNotice);
		m_ctlStyle.SetCurSel(0);
	}
}

void CDlgBalanceBasicSet::OnSelchangeStyle()
{
	int nWidth = m_ctlWidth.GetCurSel();
	int nStyle = m_ctlStyle.GetCurSel();
	
	if(nStyle > 0 && nWidth != 0)
	{		
		ML_SET_LANGUAGE_RES();

		CString strNotice("");
		strNotice.LoadString(IDS_CHANGE_THICKNESS);
		AfxMessageBox(strNotice);
		m_ctlWidth.SetCurSel(0);
	}
}

void CDlgBalanceBasicSet::OnCancelBt()
{
	ApplyOrgData();

	CDialog::OnCancel();
}

void CDlgBalanceBasicSet::OnOkBt()
{
	ApplyData();
	SaveData();

	CDialog::OnOK();
}

void CDlgBalanceBasicSet::OnApplyBt()
{
	ApplyData();
}
