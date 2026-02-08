// DlgFiboSet.cpp : implementation file
//

#include "stdafx.h"
#include "analysistool.h"
#include "DlgFiboSet.h"
#include "Conversion.h"

#include "../Include_Chart/Dll_Load/IMetaTable.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

const int g_nRationCnt = 12;

char g_strRatio[][32] = {"All", "", "", "", "", "", 
									"", "", "", "", "", ""};
char g_strRatioTime[][32] = {"All", "0", "1", "2", "3", "5", 
									"8", "13", "21", "34", "55", "89"};
char g_strRatioOther[][32] = {"All", "0", "236", "382", "500", "618", "786", 
									"1000", "1272", "1618", "2000", "2618"};

/////////////////////////////////////////////////////////////////////////////
// CDlgFiboSet dialog

// 2011.02.08 by SYS >> 분차트 YYYY(년) 추가 : 날짜/시간 14Byte형태 변경 (YYYYMMDDhhmmss)
// CDlgFiboSet::CDlgFiboSet(CWnd* pParent,
// 						 const CString& strUserPath,
// 						 const CScaleBaseData::HORZSCALEDRAWERTYPE& nDataPeriod,
// 						 int nElementType,
// 						 int *pEquiDivType,
// 						 int *pPriceRangeDivType,	// (2008/8/15 - Seung-Won, Bae) Add option for using price range.
// 						 CTextPosition* pTextPosition,
// 						 CPenEnvironment* pPenEnvironment,
// 						 CElementPointList *pElementPointList,
// 						 CLineExtEnvironment* pLineExtEnvironment,
// 						 CDataTypeEnvironment* pDataEnvironment)

CDlgFiboSet::CDlgFiboSet(CWnd* pParent,
						 const CString& strUserPath,
						 const CScaleBaseData::HORZSCALEDRAWERTYPE& nDataPeriod,
						 int nElementType,
						 int *pEquiDivType,
						 int *pPriceRangeDivType,	// (2008/8/15 - Seung-Won, Bae) Add option for using price range.
						 CTextPosition* pTextPosition,
						 CPenEnvironment* pPenEnvironment,
						 CElementPointList *pElementPointList,
						 CLineExtEnvironment* pLineExtEnvironment,
						 CDataTypeEnvironment* pDataEnvironment,
						 IChartOCX *p_pIChartOCX,
						 CBlockIndex& blockIndex,
						 CString strFormatX)		// X축 날짜시간 포맷 (YYYYMMDDHHMMSS)
						 
// 2011.02.08 by SYS <<

						 
	: CDialog(CDlgFiboSet::IDD, pParent),
	m_pParent(pParent),
	m_strUserPath(strUserPath),
	m_nDataPeriod(nDataPeriod),
	m_pPenEnvironment(pPenEnvironment),
	m_basePenEnvironment(*pPenEnvironment),
	m_pElementPointList(pElementPointList),
	m_baseElementPointList(*pElementPointList),
	m_pEquiDivEnvironment(pEquiDivType),
	m_baseEquiDivEnvironment(*pEquiDivType),
	m_pPriceRangeDivEnvironment( pPriceRangeDivType),	// (2008/8/15 - Seung-Won, Bae) Add option for using price range.
	m_basePriceRangeDivEnvironment(*pPriceRangeDivType)
{
	//{{AFX_DATA_INIT(CDlgFiboSet)
	m_nWidth = 0;
	m_nStyle = 0;
	//}}AFX_DATA_INIT

	// 2011.02.08 by SYS >> 분차트 YYYY(년) 추가 : 날짜/시간 14Byte형태 변경 (YYYYMMDDhhmmss)
	m_pIChartOCX	= p_pIChartOCX;

 	if(blockIndex.IsAllValueBiggerThanZero())
 	{
 		m_blockIndex.SetRow(blockIndex.GetRow());
 		m_blockIndex.SetColumn(blockIndex.GetColumn());
 	}

	m_strFormatX	= strFormatX;
	// 2011.02.08 by SYS <<

	
	m_nTextPosType			= G_TEXT_POS_NONE;
	m_nLineExtType			= G_LINE_EXT_NONE;
	m_nEquiDivLine			= G_EQUI_DIV_NONE;
	m_nPriceRangeDivLine	= G_PRICE_RANGE_DIV_NONE;	// (2008/8/15 - Seung-Won, Bae) Add option for using price range.
	m_nElementType			= nElementType;
	m_pDataEnvironment		= pDataEnvironment;

	SetInitTextPosition(pTextPosition);
	
	m_pLineExtEnvironment = pLineExtEnvironment;
	m_baseLineExtEnvironment.Set(pLineExtEnvironment);

	m_hOcxWnd = ( pParent ? pParent->GetSafeHwnd() : NULL);
}

void CDlgFiboSet::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgFiboSet)
	DDX_Control(pDX, IDC_CHECK_PRICE_RANGE, m_chkPriceRangeDiv);
	DDX_Control(pDX, IDC_STYLE, m_ctlStyle);
	DDX_Control(pDX, IDC_WIDTH, m_ctlWidth);
	DDX_Control(pDX, IDC_COLOR, m_ctlColor);
	DDX_Control(pDX, IDC_CHECK_TEXT_LEFT, m_ctrlLeftText);
	DDX_Control(pDX, IDC_CHECK_TEXT_RIGHT, m_ctrlRightText);
	DDX_Control(pDX, IDC_CHECK_TEXT_TOP, m_ctrlTopText);
	DDX_Control(pDX, IDC_CHECK_TEXT_BOTTOM, m_ctrlBottomText);
	DDX_Control(pDX, IDC_CHECK_EXT_LEFT, m_ctrlLeftExt);
	DDX_Control(pDX, IDC_CHECK_EXT_RIGHT, m_ctrlRightExt);
	DDX_Control(pDX, IDC_CHECK_EQUIDIV_LINE, m_ctrlEquiDiv);
	DDX_Control(pDX, IDC_CHECK_FILL, m_ctrlCheckFill);
	DDX_Control(pDX, IDC_CHECK_HIGH_LOW, m_ctrlCheckHL);
	DDX_Control(pDX, IDC_EDIT_XDATE1, m_ctrlEditXValue1);
	DDX_Control(pDX, IDC_EDIT_XDATE2, m_ctrlEditXValue2);
	DDX_Control(pDX, IDC_EDIT_XDATE3, m_ctrlEditXValue3);
	DDX_Control(pDX, IDC_EDIT_VALUE1, m_ctrlEditValue1);
	DDX_Control(pDX, IDC_EDIT_VALUE2, m_ctrlEditValue2);
	DDX_Control(pDX, IDC_EDIT_VALUE3, m_ctrlEditValue3);
	DDX_Control(pDX, IDC_DATETIMEPICKER_XDATE1, m_ctrlXDateTime1);
	DDX_Control(pDX, IDC_DATETIMEPICKER_XDATE2, m_ctrlXDateTime2);
	DDX_Control(pDX, IDC_DATETIMEPICKER_XDATE3, m_ctrlXDateTime3);
	DDX_CBIndex(pDX, IDC_WIDTH, m_nWidth);
	DDX_CBIndex(pDX, IDC_STYLE, m_nStyle);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgFiboSet, CDialog)
	//{{AFX_MSG_MAP(CDlgFiboSet)
	ON_BN_CLICKED(IDC_COLOR, OnColor)
	ON_BN_CLICKED(IDC_APPLY_BT, OnApplyBt)
	ON_CBN_SELCHANGE(IDC_WIDTH, OnSelchangeWidth)
	ON_CBN_SELCHANGE(IDC_STYLE, OnSelchangeStyle)
	ON_NOTIFY(DTN_DATETIMECHANGE, IDC_DATETIMEPICKER_XDATE1, OnDatetimechangeDatetimepickerXdate1)
	ON_NOTIFY(DTN_DATETIMECHANGE, IDC_DATETIMEPICKER_XDATE2, OnDatetimechangeDatetimepickerXdate2)
	ON_NOTIFY(DTN_DATETIMECHANGE, IDC_DATETIMEPICKER_XDATE3, OnDatetimechangeDatetimepickerXdate3)
	ON_BN_CLICKED(IDC_CANCEL_BT, OnCancelBt)
	ON_BN_CLICKED(IDC_OK_BT, OnOkBt)
	//}}AFX_MSG_MAP
	ON_CONTROL_RANGE(BN_CLICKED, IDC_CHECK1, IDC_CHECK12, OnCheck)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgFiboSet message handlers

BOOL CDlgFiboSet::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);
	mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
	
	InitializeData();

	int i;
	if( (CToolEnumData::ElementType)m_nElementType == CToolEnumData::Fibo_Time )
	{	// 피보시간대인 경우 설정값이 다르다.
		CString strDisplay;
		for( i=0; i<g_nRationCnt; i++ )
		{
			strcpy(g_strRatio[i], g_strRatioTime[i]);
			if( i == 0 )
				strDisplay = g_strRatioTime[i];
			else
				strDisplay.Format("%s(%.1f%s)", g_strRatioTime[i], atof(g_strRatioOther[i])/10.0, "%");
			SetDlgItemText(IDC_CHECK1+i, strDisplay);
		}
	}
	else
	{
		for( i=0; i<g_nRationCnt; i++ )
		{
			strcpy(g_strRatio[i], g_strRatioOther[i]);
		}
	}

	CString strExtData = m_pDataEnvironment->GetExtData();
	CString strBuf = strExtData;
	CString strR;

	if( strBuf.IsEmpty() )		// Default 값 설정
	{
		if( (CToolEnumData::ElementType)m_nElementType == CToolEnumData::Fibo_Time )		// 피보나치 시간대
			strBuf = "0,1,2,3,5,8,13,21,34,55";
		else
			strBuf = "0,236,382,500,618,786,1000";
	}

	while( !strBuf.IsEmpty() )
	{
		strR = CDataConversion::GetStringData(strBuf, ",");
		if( strR.IsEmpty() )		
		{
			strR = strBuf;
			strBuf.Empty();
		}

		for( i=0; i<g_nRationCnt; i++ )
		{
			if( strcmp(strR, g_strRatio[i]) == 0 )	break;
		}
		if( i < g_nRationCnt )
			((CButton *)GetDlgItem(IDC_CHECK1 + i))->SetCheck(TRUE);
	}

	if( ((CButton *)GetDlgItem(IDC_CHECK1))->GetCheck() )
	{
		for( i=1; i<g_nRationCnt; i++ )
			((CButton *)GetDlgItem(IDC_CHECK1 + i))->SetCheck(TRUE);
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgFiboSet::OnCheck( UINT nID ) 
{
	// TODO: Add your control notification handler code here
	if( nID == IDC_CHECK1 )
	{
		BOOL bCheck = ((CButton *)GetDlgItem(IDC_CHECK1))->GetCheck();

		for( int i=1; i<g_nRationCnt; i++ )
			((CButton *)GetDlgItem(IDC_CHECK1 + i))->SetCheck(bCheck);
	}
	else
	{
		((CButton *)GetDlgItem(IDC_CHECK1))->SetCheck(FALSE);
	}
	
}

void CDlgFiboSet::SaveData()
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

	strKey = _T("TopText");
	strData.Format(_T("%d"),m_stAnalysisTool.nTopText);		
	WritePrivateProfileString(strSection,strKey,strData,strFileName);

	strKey = _T("BottomText");
	strData.Format(_T("%d"),m_stAnalysisTool.nBottomText);		
	WritePrivateProfileString(strSection,strKey,strData,strFileName);

	strKey = _T("LeftText");
	strData.Format(_T("%d"),m_stAnalysisTool.nLeftText);		
	WritePrivateProfileString(strSection,strKey,strData,strFileName);

	strKey = _T("RightText");
	strData.Format(_T("%d"),m_stAnalysisTool.nRightText);		
	WritePrivateProfileString(strSection,strKey,strData,strFileName);

	strKey = _T("LeftExt");
	strData.Format(_T("%d"),m_stAnalysisTool.nLeftExt);		
	WritePrivateProfileString(strSection,strKey,strData,strFileName);

	strKey = _T("RightExt");
	strData.Format(_T("%d"),m_stAnalysisTool.nRightExt);		
	WritePrivateProfileString(strSection,strKey,strData,strFileName);

	strKey = _T("EquiDiv");
	strData.Format(_T("%d"),m_stAnalysisTool.nEquiDiv);		
	WritePrivateProfileString(strSection,strKey,strData,strFileName);

	// (2008/8/15 - Seung-Won, Bae) Add option for using price range.
	strKey = _T("PriceRangeDiv");
	strData.Format(_T("%d"),m_stAnalysisTool.nPriceRangeDiv);		
	WritePrivateProfileString(strSection,strKey,strData,strFileName);

	strKey = _T("HLPercent");
	strData.Format(_T("%d"),m_stAnalysisTool.nHLPercent);		
	WritePrivateProfileString(strSection,strKey,strData,strFileName);

	strKey = _T("FillRegion");
	strData.Format(_T("%d"),m_stAnalysisTool.nFillRegion);		
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
	
	strKey = _T("FiboCoefficient");
	strData = m_pDataEnvironment->GetExtData();
	WritePrivateProfileString(strSection,strKey,strData,strFileName);
}


void CDlgFiboSet::ApplyData()
{
	SetPenEnvironment();

	if(m_nTextPosType != G_TEXT_POS_NONE)
		SetTextPosition();

	if(m_nLineExtType != G_LINE_EXT_NONE)
		SetLineExtEnvironment();

	if(m_nEquiDivLine != G_EQUI_DIV_NONE)
	{
		(*m_pEquiDivEnvironment) = m_ctrlEquiDiv.GetCheck();
		m_stAnalysisTool.nEquiDiv = m_ctrlEquiDiv.GetCheck();		
	}

	// (2008/8/15 - Seung-Won, Bae) Add option for using price range.
	if( m_nPriceRangeDivLine != G_PRICE_RANGE_DIV_NONE)
	{
		( *m_pPriceRangeDivEnvironment) = m_chkPriceRangeDiv.GetCheck();
		m_stAnalysisTool.nPriceRangeDiv = m_chkPriceRangeDiv.GetCheck();		
	}
	
	SetDateValueLists();

	/*	All 인 경우 "All"만 저장하지 말고 값 전체를 저장한다.
		=> Load 시에 그냥 값을 읽어서 처리할 수 있도록
	BOOL bCheck = ((CButton *)GetDlgItem(IDC_CHECK1))->GetCheck();
	if( bCheck )
	{
		m_pDataEnvironment->SetExtData(g_strRatio[0]);
	}
	else
	*/
	{
		CString strData;
		for( int i=0; i<g_nRationCnt; i++ )
		{
			if( ((CButton *)GetDlgItem(IDC_CHECK1 + i))->GetCheck() )
			{
				strData += g_strRatio[i];
				strData += ",";
			}
		}
		m_pDataEnvironment->SetExtData(strData);
	}

	m_pParent->Invalidate();
}

void CDlgFiboSet::ApplyOrgData()
{
	m_pPenEnvironment->Set(m_basePenEnvironment);
			
	if(m_nTextPosType != G_TEXT_POS_NONE)
	{
		if(m_nTextPosType == G_TEXT_POS_HV)
			SetTextPosition(m_baseTextHVPosition.GetLeft(),m_baseTextHVPosition.GetRight(),
							m_baseTextHVPosition.GetTop(),m_baseTextHVPosition.GetBottom());		
		else if(m_nTextPosType == G_TEXT_POS_VERT)
			SetTextPosition(m_baseTextVPosition.GetTop(),m_baseTextVPosition.GetBottom());			
		else if(m_nTextPosType == G_TEXT_POS_ONE)
			SetTextPosition(m_baseTextPosition.IsShow());			
		else if(m_nTextPosType == G_TEXT_POS_HORZ)
			SetTextPosition(m_baseTextHPosition.GetLeft(),m_baseTextHPosition.GetRight());		
		
		CTextPosition* pTextPosition = GetTextPosition();
		CTextPosition* pOrgTextPosition = GetOrgTextPosition();
		if(pTextPosition != NULL && pOrgTextPosition != NULL)
			pTextPosition->Set(pOrgTextPosition);
	}

	if(m_nLineExtType != G_LINE_EXT_NONE)
	{
		SetLineExtEnvironment( ( m_baseLineExtEnvironment.GetLeftExt() ? true : false),
								( m_baseLineExtEnvironment.GetRightExt() ? true : false));
		
		CLineExtEnvironment* pLineExtEnvironment = GetLineExtEnvironment();
		CLineExtEnvironment* pOrgLineExtEnvironment = GetOrgLineExtEnvironment();
		if(pLineExtEnvironment != NULL && pOrgLineExtEnvironment != NULL)
			pLineExtEnvironment->Set(pOrgLineExtEnvironment);
	}

	if(m_nEquiDivLine != G_EQUI_DIV_NONE)
		(*m_pEquiDivEnvironment) = m_baseEquiDivEnvironment;		
	
	// (2008/8/15 - Seung-Won, Bae) Add option for using price range.
	if( m_nPriceRangeDivLine != G_PRICE_RANGE_DIV_NONE)
		( *m_pPriceRangeDivEnvironment) = m_basePriceRangeDivEnvironment;		

	m_pParent->Invalidate();
}

void CDlgFiboSet::OnColor() 
{
	CColorDialog colorDlg;

	if(colorDlg.DoModal() != IDOK)
		return;

	SetColor(colorDlg.GetColor());
}

///////////////////////////////////////////////////////////////////////////////
// protected ==================================================================
void CDlgFiboSet::InitializeData()
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
	UpdateData(FALSE);

	GetDlgItem(IDC_STATIC_XDATE3)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_STATIC_YVALUE3)->ShowWindow(SW_HIDE);
	m_ctrlXDateTime3.ShowWindow(SW_HIDE);
	m_ctrlEditValue3.ShowWindow(SW_HIDE);

	m_ctrlEditXValue1.ShowWindow(SW_HIDE);
	m_ctrlEditXValue2.ShowWindow(SW_HIDE);
	m_ctrlEditXValue3.ShowWindow(SW_HIDE);

	m_ctrlEquiDiv.ShowWindow(SW_HIDE);
	m_ctrlCheckFill.ShowWindow(SW_HIDE);

	m_ctrlLeftExt.ShowWindow(SW_HIDE);
	m_ctrlRightExt.ShowWindow(SW_HIDE);
	m_ctrlTopText.ShowWindow(SW_HIDE);
	m_ctrlBottomText.ShowWindow(SW_HIDE);

	///////////////////////////////////////////
	m_ctrlCheckHL.ShowWindow(SW_HIDE);			//가격변화선에서 사용
	///////////////////////////////////////////

	// (2008/8/15 - Seung-Won, Bae) Add option for using price range.
	m_chkPriceRangeDiv.ShowWindow( SW_HIDE);

	if((CToolEnumData::ElementType)m_nElementType == CToolEnumData::Pencil_Line)
	{
		m_ctrlLeftText.ShowWindow(SW_HIDE);
		m_ctrlRightText.ShowWindow(SW_HIDE);

		//시작 날짜와 가격
		m_ctrlXDateTime1.ShowWindow(SW_HIDE);
		m_ctrlEditValue1.ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_XDATE1)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_YVALUE1)->ShowWindow(SW_HIDE);

		m_ctrlXDateTime2.ShowWindow(SW_HIDE);
		m_ctrlEditValue2.ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_XDATE2)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_YVALUE2)->ShowWindow(SW_HIDE);

		GetDlgItem(IDC_TEXT_STATIC)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_DATE_N_VALUE_STATIC)->ShowWindow(SW_HIDE);

		///////////////////////////////////////////////////////////////////////////////

		CRect rect;
		GetWindowRect(&rect);
		 
		int iExpandSize = 140;
		rect.bottom -= iExpandSize;		
		MoveWindow(rect);	
		
		CRect rectPos;
		CStatic *pStatic = (CStatic *)GetDlgItem(IDC_DATE_N_VALUE_STATIC);
		pStatic->GetWindowRect(&rectPos);
		ScreenToClient(&rectPos);	

		int nTempX = rectPos.left;

		pStatic = (CStatic *)GetDlgItem(IDC_PEN_STATIC);
		pStatic->GetClientRect(&rect);
		pStatic->MoveWindow(rectPos.left, rectPos.top, rect.Width(), rect.Height());

		GetDlgItem(IDC_STATIC_PEN_COLOR)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_STATIC_PEN_STYLE)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_STATIC_PEN_WIDTH)->ShowWindow(SW_SHOW);

		IMetaTable::E_LANGUAGEID eLanguageID = g_iMetaTable.GetCurrentLanguageID( m_hOcxWnd); 
		switch(eLanguageID)
		{
		case IMetaTable::ENGLISH:
			{
				rectPos.left += 50;
				rectPos.top += 20;
				CButton *pBtn = (CButton *)GetDlgItem(IDC_COLOR);
				pBtn->GetClientRect(&rect);
				pBtn->MoveWindow(rectPos.left, rectPos.top, rect.Width(), rect.Height());

				rectPos.top -= 3;
				rectPos.left = rectPos.left + rect.Width() + 75;
				CComboBox *pCombo = (CComboBox *)GetDlgItem(IDC_WIDTH);
				pCombo->GetClientRect(&rect);
				pCombo->MoveWindow(rectPos.left, rectPos.top, rect.Width(), rect.Height());

				rectPos.left = rectPos.left + rect.Width() + 40;
				pCombo = (CComboBox *)GetDlgItem(IDC_STYLE);
				pCombo->GetClientRect(&rect);
				pCombo->MoveWindow(rectPos.left, rectPos.top, rect.Width(), rect.Height());

				nTempX += 183;
				pBtn = (CButton *)GetDlgItem(IDC_OK_BT);
				pBtn->GetClientRect(&rect);
				pBtn->MoveWindow(nTempX, rectPos.top + 45, rect.Width(), rect.Height());

				nTempX = nTempX + rect.Width() + 2;
				pBtn = (CButton *)GetDlgItem(IDC_CANCEL_BT);
				pBtn->GetClientRect(&rect);
				pBtn->MoveWindow(nTempX, rectPos.top + 45, rect.Width(), rect.Height());

				nTempX = nTempX + rect.Width() + 2;
				pBtn = (CButton *)GetDlgItem(IDC_APPLY_BT);
				pBtn->GetClientRect(&rect);
				pBtn->MoveWindow(nTempX, rectPos.top + 45, rect.Width(), rect.Height());
			}
			break;
		case IMetaTable::KOREAN:
			{
				rectPos.left += 35;
				rectPos.top += 20;
				CButton *pBtn = (CButton *)GetDlgItem(IDC_COLOR);
				pBtn->GetClientRect(&rect);
				pBtn->MoveWindow(rectPos.left, rectPos.top, rect.Width(), rect.Height());
 
				rectPos.top -= 3;
				rectPos.left = rectPos.left + rect.Width() + 35;
				CComboBox *pCombo = (CComboBox *)GetDlgItem(IDC_WIDTH);
				pCombo->GetClientRect(&rect);
				pCombo->MoveWindow(rectPos.left, rectPos.top, rect.Width(), rect.Height());

				rectPos.left = rectPos.left + rect.Width() + 40;
				pCombo = (CComboBox *)GetDlgItem(IDC_STYLE);
				pCombo->GetClientRect(&rect);
				pCombo->MoveWindow(rectPos.left, rectPos.top, rect.Width(), rect.Height());

				nTempX += 130;
				pBtn = (CButton *)GetDlgItem(IDC_OK_BT);
				pBtn->GetClientRect(&rect);
				pBtn->MoveWindow(nTempX, rectPos.top + 45, rect.Width(), rect.Height());

				nTempX = nTempX + rect.Width() + 2;
				pBtn = (CButton *)GetDlgItem(IDC_CANCEL_BT);
				pBtn->GetClientRect(&rect);
				pBtn->MoveWindow(nTempX, rectPos.top + 45, rect.Width(), rect.Height());

				nTempX = nTempX + rect.Width() + 2;
				pBtn = (CButton *)GetDlgItem(IDC_APPLY_BT);
				pBtn->GetClientRect(&rect);
				pBtn->MoveWindow(nTempX, rectPos.top + 45, rect.Width(), rect.Height());
			}
			break;
		default:
			ASSERT(FALSE);
		};
		
//		rectPos.left += 35;
//		rectPos.top += 20;
//		CButton *pBtn = (CButton *)GetDlgItem(IDC_COLOR);
//		pBtn->GetClientRect(&rect);
//		pBtn->MoveWindow(rectPos.left, rectPos.top, rect.Width(), rect.Height());
// 
//		rectPos.top -= 3;
//		rectPos.left = rectPos.left + rect.Width() + 35;
//		CComboBox *pCombo = (CComboBox *)GetDlgItem(IDC_WIDTH);
//		pCombo->GetClientRect(&rect);
//		pCombo->MoveWindow(rectPos.left, rectPos.top, rect.Width(), rect.Height());
//
//		rectPos.left = rectPos.left + rect.Width() + 40;
//		pCombo = (CComboBox *)GetDlgItem(IDC_STYLE);
//		pCombo->GetClientRect(&rect);
//		pCombo->MoveWindow(rectPos.left, rectPos.top, rect.Width(), rect.Height());
//
//		nTempX += 130;
//		pBtn = (CButton *)GetDlgItem(IDC_OK_BT);
//		pBtn->GetClientRect(&rect);
//		pBtn->MoveWindow(nTempX, rectPos.top + 45, rect.Width(), rect.Height());
//
//		nTempX = nTempX + rect.Width() + 2;
//		pBtn = (CButton *)GetDlgItem(IDC_CANCEL_BT);
//		pBtn->GetClientRect(&rect);
//		pBtn->MoveWindow(nTempX, rectPos.top + 45, rect.Width(), rect.Height());
//
//		nTempX = nTempX + rect.Width() + 2;
//		pBtn = (CButton *)GetDlgItem(IDC_APPLY_BT);
//		pBtn->GetClientRect(&rect);
//		pBtn->MoveWindow(nTempX, rectPos.top + 45, rect.Width(), rect.Height());

		///////////////////////////////////////////////////////////////////////////////
	}
	else if((CToolEnumData::ElementType)m_nElementType == CToolEnumData::Cross_Line)		
	{
		m_ctrlTopText.ShowWindow(SW_SHOW);
		m_ctrlBottomText.ShowWindow(SW_SHOW);

		m_nTextPosType = G_TEXT_POS_HV;
		m_ctrlLeftText.SetCheck(m_pTextHVPosition->GetLeft());
		m_ctrlRightText.SetCheck(m_pTextHVPosition->GetRight());
		m_ctrlTopText.SetCheck(m_pTextHVPosition->GetTop());
		m_ctrlBottomText.SetCheck(m_pTextHVPosition->GetBottom());		

		m_stAnalysisTool.nTopText = m_pTextHVPosition->GetTop();
		m_stAnalysisTool.nBottomText = m_pTextHVPosition->GetBottom();
		m_stAnalysisTool.nLeftText = m_pTextHVPosition->GetLeft();
		m_stAnalysisTool.nRightText = m_pTextHVPosition->GetRight();
		
		//시작 날짜와 가격
		m_ctrlXDateTime2.ShowWindow(SW_HIDE);
		m_ctrlEditValue2.ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_XDATE2)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_YVALUE2)->ShowWindow(SW_HIDE);
	}
	else if((CToolEnumData::ElementType)m_nElementType == CToolEnumData::Horz_Line)	//수평선
	{
		m_nTextPosType = G_TEXT_POS_HORZ;
		m_ctrlLeftText.SetCheck(m_pTextHPosition->GetLeft());
		m_ctrlRightText.SetCheck(m_pTextHPosition->GetRight());

		m_stAnalysisTool.nLeftText = m_pTextHPosition->GetLeft();
		m_stAnalysisTool.nRightText = m_pTextHPosition->GetRight();

		//시작 가격
		m_ctrlXDateTime2.ShowWindow(SW_HIDE);
		m_ctrlEditValue2.ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_XDATE2)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_YVALUE2)->ShowWindow(SW_HIDE);
	} 	
	else if((CToolEnumData::ElementType)m_nElementType == CToolEnumData::Vert_Line ||
			(CToolEnumData::ElementType)m_nElementType == CToolEnumData::Fibo_Time)	//수직선
	{
		if((CToolEnumData::ElementType)m_nElementType == CToolEnumData::Vert_Line)
		{
			m_nTextPosType = G_TEXT_POS_VERT;

			ML_SET_LANGUAGE_RES();

			CString strTitle;
			strTitle.LoadString(IDS_UPPEREND);
			SetDlgItemText(IDC_CHECK_TEXT_LEFT,strTitle);
			strTitle.LoadString(IDS_LOWEREND);
			SetDlgItemText(IDC_CHECK_TEXT_RIGHT,strTitle);
			m_ctrlLeftText.SetCheck(m_pTextVPosition->GetTop());
			m_ctrlRightText.SetCheck(m_pTextVPosition->GetBottom());
			
			m_stAnalysisTool.nTopText = m_pTextVPosition->GetTop();
			m_stAnalysisTool.nBottomText = m_pTextVPosition->GetBottom();
		}
		else
		{
			// 2008.08.20 김진순 피보나치 확장=>	피보나치수열에도 Text 보여주기 위해<<
			/*
			GetDlgItem(IDC_TEXT_STATIC)->ShowWindow(SW_HIDE);
			m_ctrlLeftText.ShowWindow(SW_HIDE);
			m_ctrlRightText.ShowWindow(SW_HIDE);
			*/
			m_nTextPosType = G_TEXT_POS_ONE;
			
			ML_SET_LANGUAGE_RES();

			CString strTitle;
			strTitle.LoadString(IDS_SHOW);
			SetDlgItemText(IDC_CHECK_TEXT_LEFT,strTitle);
			
			m_ctrlRightText.ShowWindow(SW_HIDE);		
			m_ctrlLeftText.SetCheck(m_pTextPosition->IsShow());
			
			m_stAnalysisTool.nLeftText = m_pTextPosition->IsShow();
			//>>
		}

		//시작 날짜
		m_ctrlEditValue1.EnableWindow(FALSE);
		m_ctrlXDateTime2.ShowWindow(SW_HIDE);
		m_ctrlEditValue2.ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_XDATE2)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_YVALUE2)->ShowWindow(SW_HIDE);
	}
	else if((CToolEnumData::ElementType)m_nElementType == CToolEnumData::Line		||	//추세선
			(CToolEnumData::ElementType)m_nElementType == CToolEnumData::Period_Summary)	
	{
		m_nTextPosType = G_TEXT_POS_HORZ;
		m_nLineExtType = G_LINE_EXT_BOTH;
		m_ctrlLeftText.SetCheck(m_pTextHPosition->GetLeft());
		m_ctrlRightText.SetCheck(m_pTextHPosition->GetRight());

		m_stAnalysisTool.nLeftText = m_pTextHPosition->GetLeft();
		m_stAnalysisTool.nRightText = m_pTextHPosition->GetRight();
		
		m_ctrlLeftExt.ShowWindow(SW_SHOW);
		m_ctrlRightExt.ShowWindow(SW_SHOW);

		m_ctrlLeftExt.SetCheck(m_pLineExtEnvironment->GetLeftExt());
		m_ctrlRightExt.SetCheck(m_pLineExtEnvironment->GetRightExt());
		
		m_stAnalysisTool.nLeftExt = m_pLineExtEnvironment->GetLeftExt();
		m_stAnalysisTool.nRightExt = m_pLineExtEnvironment->GetRightExt();

		if((CToolEnumData::ElementType)m_nElementType != CToolEnumData::Line)
		{
			m_ctrlEquiDiv.ShowWindow(SW_SHOW);
			m_ctrlEquiDiv.SetCheck(m_baseEquiDivEnvironment);
			m_nEquiDivLine = m_baseEquiDivEnvironment;		

			m_stAnalysisTool.nEquiDiv = m_nEquiDivLine;
		}
	}
	// (2008/8/15 - Seung-Won, Bae) Add option for using price range.
	else if((CToolEnumData::ElementType)m_nElementType == CToolEnumData::Trisection	||
			(CToolEnumData::ElementType)m_nElementType == CToolEnumData::Quadrisect)	
	{
		m_nTextPosType = G_TEXT_POS_HORZ;
		m_nLineExtType = G_LINE_EXT_BOTH;
		m_ctrlLeftText.SetCheck(m_pTextHPosition->GetLeft());
		m_ctrlRightText.SetCheck(m_pTextHPosition->GetRight());

		m_stAnalysisTool.nLeftText = m_pTextHPosition->GetLeft();
		m_stAnalysisTool.nRightText = m_pTextHPosition->GetRight();
		
		m_ctrlLeftExt.ShowWindow(SW_SHOW);
		m_ctrlRightExt.ShowWindow(SW_SHOW);

		m_ctrlLeftExt.SetCheck(m_pLineExtEnvironment->GetLeftExt());
		m_ctrlRightExt.SetCheck(m_pLineExtEnvironment->GetRightExt());
		
		m_stAnalysisTool.nLeftExt = m_pLineExtEnvironment->GetLeftExt();
		m_stAnalysisTool.nRightExt = m_pLineExtEnvironment->GetRightExt();

		m_ctrlEquiDiv.ShowWindow(SW_SHOW);
		m_ctrlEquiDiv.SetCheck(m_baseEquiDivEnvironment);
		m_nEquiDivLine = m_baseEquiDivEnvironment;		
		m_stAnalysisTool.nEquiDiv = m_nEquiDivLine;

		// (2008/8/15 - Seung-Won, Bae) Add option for using price range.
		m_chkPriceRangeDiv.ShowWindow( SW_SHOW);
		m_chkPriceRangeDiv.SetCheck( m_basePriceRangeDivEnvironment);
		m_nPriceRangeDivLine = m_basePriceRangeDivEnvironment;		
		m_stAnalysisTool.nPriceRangeDiv = m_nPriceRangeDivLine;
		if( G_PRICE_RANGE_DIV_CHECK == m_nPriceRangeDivLine)
		{
			m_ctrlEditValue1.EnableWindow( FALSE);
			m_ctrlEditValue2.EnableWindow( FALSE);
		}
	}
	else if((CToolEnumData::ElementType)m_nElementType == CToolEnumData::Fibo_Retrace	||
			(CToolEnumData::ElementType)m_nElementType == CToolEnumData::Fibo_Semicircle ||
			(CToolEnumData::ElementType)m_nElementType == CToolEnumData::Fibo_Circle	||
			(CToolEnumData::ElementType)m_nElementType == CToolEnumData::Fibo_Fan_Line	||
			(CToolEnumData::ElementType)m_nElementType == CToolEnumData::Gann_Fan1	||
			(CToolEnumData::ElementType)m_nElementType == CToolEnumData::Gann_Fan2	||
			(CToolEnumData::ElementType)m_nElementType == CToolEnumData::Gann_Fan3	||
			(CToolEnumData::ElementType)m_nElementType == CToolEnumData::Gann_Fan4)
	{
		m_nTextPosType = G_TEXT_POS_ONE;
		
		ML_SET_LANGUAGE_RES();

		CString strTitle;
		strTitle.LoadString(IDS_SHOW);
		SetDlgItemText(IDC_CHECK_TEXT_LEFT,strTitle);
		
		m_ctrlRightText.ShowWindow(SW_HIDE);		
		m_ctrlLeftText.SetCheck(m_pTextPosition->IsShow());
		
		m_stAnalysisTool.nLeftText = m_pTextPosition->IsShow();

		if((CToolEnumData::ElementType)m_nElementType == CToolEnumData::Fibo_Retrace)
		{
			m_nLineExtType = G_LINE_EXT_BOTH;
			m_ctrlLeftExt.ShowWindow(SW_SHOW);
			m_ctrlRightExt.ShowWindow(SW_SHOW);

			m_ctrlLeftExt.SetCheck(m_pLineExtEnvironment->GetLeftExt());
			m_ctrlRightExt.SetCheck(m_pLineExtEnvironment->GetRightExt());

			m_stAnalysisTool.nLeftExt = m_pLineExtEnvironment->GetLeftExt();
			m_stAnalysisTool.nRightExt = m_pLineExtEnvironment->GetRightExt();
		}
	} 
	else if((CToolEnumData::ElementType)m_nElementType == CToolEnumData::Angle_Line)
	{
		m_nTextPosType = G_TEXT_POS_ONE;
		
		ML_SET_LANGUAGE_RES();

		CString strTitle;
		strTitle.LoadString(IDS_ANGLE);
		SetDlgItemText(IDC_CHECK_TEXT_LEFT,strTitle);
		strTitle.LoadString(IDS_EXTENSIONLINE);
		SetDlgItemText(IDC_CHECK_EXT_LEFT,strTitle);		

		m_ctrlRightText.ShowWindow(SW_HIDE);		
		m_ctrlLeftText.SetCheck(m_pTextPosition->IsShow());
		
		m_stAnalysisTool.nLeftText = m_pTextPosition->IsShow();
		
		m_nLineExtType = G_LINE_EXT_RIGHT;
		m_ctrlLeftExt.ShowWindow(SW_SHOW);
		m_ctrlLeftExt.SetCheck(m_pLineExtEnvironment->GetLeftExt());

		m_stAnalysisTool.nLeftExt = m_pLineExtEnvironment->GetLeftExt();
	}
	else if((CToolEnumData::ElementType)m_nElementType == CToolEnumData::Candle_Line)	//봉중심선
	{
		m_nTextPosType = G_TEXT_POS_ONE;

		ML_SET_LANGUAGE_RES();

		CString strTitle;
		strTitle.LoadString(IDS_SHOW);
		SetDlgItemText(IDC_CHECK_TEXT_LEFT,strTitle);
		m_ctrlRightText.ShowWindow(SW_HIDE);		
		m_ctrlLeftText.SetCheck(m_pTextPosition->IsShow());

		m_stAnalysisTool.nLeftText = m_pTextPosition->IsShow();

		//시작 가격
		m_ctrlXDateTime2.ShowWindow(SW_HIDE);
		m_ctrlEditValue2.ShowWindow(SW_HIDE);

		GetDlgItem(IDC_STATIC_XDATE2)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_YVALUE2)->ShowWindow(SW_HIDE);
	} 
	else if((CToolEnumData::ElementType)m_nElementType == CToolEnumData::Linear_Regression  ||
			(CToolEnumData::ElementType)m_nElementType == CToolEnumData::Raff_Regression)//	||
			//(CToolEnumData::ElementType)m_nElementType == CToolEnumData::Standard_Deviation	||
			//(CToolEnumData::ElementType)m_nElementType == CToolEnumData::Standard_Error
	{
		GetDlgItem(IDC_TEXT_STATIC)->ShowWindow(SW_HIDE);
		m_ctrlLeftText.ShowWindow(SW_HIDE);
		m_ctrlRightText.ShowWindow(SW_HIDE);

		m_nLineExtType = G_LINE_EXT_BOTH;
		m_ctrlLeftExt.ShowWindow(SW_SHOW);
		m_ctrlRightExt.ShowWindow(SW_SHOW);

		m_ctrlLeftExt.SetCheck(m_pLineExtEnvironment->GetLeftExt());
		m_ctrlRightExt.SetCheck(m_pLineExtEnvironment->GetRightExt());

		m_stAnalysisTool.nLeftExt = m_pLineExtEnvironment->GetLeftExt();
		m_stAnalysisTool.nRightExt = m_pLineExtEnvironment->GetRightExt();
	} 
	else if((CToolEnumData::ElementType)m_nElementType == CToolEnumData::AndrewsPitchfork)
	{
		ML_SET_LANGUAGE_RES();

		CString strTitle;
		strTitle.LoadString(IDS_MIDDATE);
		SetDlgItemText(IDC_STATIC_XDATE2,strTitle);

		GetDlgItem(IDC_STATIC_XDATE3)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_STATIC_YVALUE3)->ShowWindow(SW_SHOW);
		m_ctrlXDateTime3.ShowWindow(SW_SHOW);
		m_ctrlEditValue3.ShowWindow(SW_SHOW);

		GetDlgItem(IDC_TEXT_STATIC)->ShowWindow(SW_HIDE);
		m_ctrlLeftText.ShowWindow(SW_HIDE);
		m_ctrlRightText.ShowWindow(SW_HIDE);
	}
	else if(//(CToolEnumData::ElementType)m_nElementType == CToolEnumData::Fibo_Semicircle ||
			//(CToolEnumData::ElementType)m_nElementType == CToolEnumData::Fibo_Circle	 ||
			//(CToolEnumData::ElementType)m_nElementType == CToolEnumData::Fibo_Fan_Line	 ||
			(CToolEnumData::ElementType)m_nElementType == CToolEnumData::Gann_Line1		 ||
			(CToolEnumData::ElementType)m_nElementType == CToolEnumData::Gann_Line2		 ||
			(CToolEnumData::ElementType)m_nElementType == CToolEnumData::Gann_Fan1		 ||
			(CToolEnumData::ElementType)m_nElementType == CToolEnumData::Gann_Fan2		 ||
			(CToolEnumData::ElementType)m_nElementType == CToolEnumData::Gann_Fan3		 ||
			(CToolEnumData::ElementType)m_nElementType == CToolEnumData::Gann_Fan4		 ||
			(CToolEnumData::ElementType)m_nElementType == CToolEnumData::Gann_Grid		 ||
			(CToolEnumData::ElementType)m_nElementType == CToolEnumData::Speed_Line		 ||
			(CToolEnumData::ElementType)m_nElementType == CToolEnumData::Speed_Fan		 ||
			(CToolEnumData::ElementType)m_nElementType == CToolEnumData::Speed_Arc		 ||
			(CToolEnumData::ElementType)m_nElementType == CToolEnumData::Cycle_Line		 ||
			(CToolEnumData::ElementType)m_nElementType == CToolEnumData::ElliotWave_Line)
	{
		if((CToolEnumData::ElementType)m_nElementType == CToolEnumData::Gann_Fan1		 ||
			(CToolEnumData::ElementType)m_nElementType == CToolEnumData::Gann_Fan2		 ||
			(CToolEnumData::ElementType)m_nElementType == CToolEnumData::Gann_Fan3		 ||
			(CToolEnumData::ElementType)m_nElementType == CToolEnumData::Gann_Fan4)
		{
			m_ctrlXDateTime2.ShowWindow(SW_HIDE);
			m_ctrlEditValue2.ShowWindow(SW_HIDE);
			GetDlgItem(IDC_STATIC_XDATE2)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_STATIC_YVALUE2)->ShowWindow(SW_HIDE);
		}
		
		GetDlgItem(IDC_TEXT_STATIC)->ShowWindow(SW_HIDE);
		m_ctrlLeftText.ShowWindow(SW_HIDE);
		m_ctrlRightText.ShowWindow(SW_HIDE);
	}

	if((CToolEnumData::ElementType)m_nElementType != CToolEnumData::Pencil_Line)
	{
		if(m_pElementPointList != NULL)
		{
			CCoordinate *pCoordinate = NULL;
			for(int i=0;i<m_pElementPointList->GetCount();i++)
			{
				pCoordinate = m_pElementPointList->GetAt(i);

				// 2011.02.08 by SYS >> 분차트 YYYY(년) 추가 : 날짜/시간 14Byte형태 변경 (YYYYMMDDhhmmss)
				//SetXDateTime(pCoordinate->GetX(),i);

				double dX = pCoordinate->GetX();
				if (m_strFormatX == _T("YYYYMMDDHHMMSS"))
				{
					if( m_nDataPeriod == CScaleBaseData::HORZ_WEEKLEY	||
						m_nDataPeriod == CScaleBaseData::HORZ_MONTHLY)
					{
						// 주간, 월간은 현재 차트 날짜주기로 변경한다.
						IChartManager *pIChartManager = m_pIChartOCX->GetIChartManager();
						if (pIChartManager) 
						{
							// 
							int nRow = m_blockIndex.GetRow();
							int nCol = m_blockIndex.GetColumn();

							int nCenter_x;
							int nOrgXPos		= pIChartManager->GetXFromDouble(nRow, nCol, dX, nCenter_x);

							int nBlockColumn;
							double dModifyData	= pIChartManager->GetDoubleFromX(nRow, nCol, nOrgXPos, &nBlockColumn, nCenter_x);
							dX = dModifyData;

							pIChartManager->Release();
							pIChartManager = NULL;
						}
					}
				}

				SetXDateTime(dX,i);
				// 2011.02.08 by SYS <<
				
				SetYValue(pCoordinate->GetY(),i);
			}
		}
	}	
}

void CDlgFiboSet::SetInitTextPosition(CTextPosition* pTextPosition)
{
	if(m_nElementType == CToolEnumData::Cross_Line)
	{
		m_pTextPosition = NULL;
		m_pTextHPosition = NULL;
		m_pTextVPosition = NULL;
		
		m_pTextHVPosition = (CTextHorzVertPosition *)pTextPosition;
		m_baseTextHVPosition.Set(pTextPosition);
	}
	else if(m_nElementType == CToolEnumData::Vert_Line)
	{
		m_pTextPosition = NULL;
		m_pTextHPosition = NULL;
		m_pTextHVPosition = NULL;
		
		m_pTextVPosition = (CTextVertPosition *)pTextPosition;
		m_baseTextVPosition.Set(pTextPosition);
	}
	else if(m_nElementType == CToolEnumData::Fibo_Retrace	||
			m_nElementType == CToolEnumData::Fibo_Semicircle	||
			m_nElementType == CToolEnumData::Fibo_Circle	||
			m_nElementType == CToolEnumData::Fibo_Fan_Line	||
			m_nElementType == CToolEnumData::Fibo_Time	||	// 2008.08.20 김진순 피보나치수열에도 Text 보여주기 위해
			m_nElementType == CToolEnumData::Angle_Line		||
			m_nElementType == CToolEnumData::Candle_Line	||
			m_nElementType == CToolEnumData::Gann_Fan1	||
			m_nElementType == CToolEnumData::Gann_Fan2	||
			m_nElementType == CToolEnumData::Gann_Fan3	||
			m_nElementType == CToolEnumData::Gann_Fan4)
	{
		m_pTextVPosition = NULL;
		m_pTextHPosition = NULL;
		m_pTextHVPosition = NULL;
		
		m_pTextPosition = (CTextShowPosition *)pTextPosition;
		m_baseTextPosition.Set(pTextPosition);
	}
	else if(m_nElementType == CToolEnumData::Line		||
			m_nElementType == CToolEnumData::Horz_Line	||
			m_nElementType == CToolEnumData::Trisection	||
			m_nElementType == CToolEnumData::Quadrisect	||
			m_nElementType == CToolEnumData::Period_Summary)
	{
		m_pTextPosition = NULL;
		m_pTextVPosition = NULL;
		m_pTextHVPosition = NULL;

		m_pTextHPosition = (CTextHorzPosition *)pTextPosition;
		m_baseTextHPosition.Set(pTextPosition);
	}
	else		//NoText
	{
		m_pTextPosition = NULL;
		m_pTextVPosition = NULL;
		m_pTextHPosition = NULL;
		m_pTextHVPosition = NULL;
	}
}

// private ====================================================================
void CDlgFiboSet::InitializeData_Width()
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

void CDlgFiboSet::InitializeData_Style()
{
	ML_SET_DEFAULT_RES();

	m_ctlStyle.LoadBitmap(IDB_LINESTYLE, 5);
	m_nStyle = (int) m_basePenEnvironment.GetStyle();
}

void CDlgFiboSet::SetPenEnvironment()
{
	UpdateData(TRUE);
	m_pPenEnvironment->SetColor(m_ctlColor.GetColor());
	m_pPenEnvironment->SetWidth(m_nWidth+1);
	m_pPenEnvironment->SetStyle((CToolEnumData::PenStyle) m_nStyle);
	
	m_stAnalysisTool.crLineColor = m_ctlColor.GetColor();
	m_stAnalysisTool.nLineStyle = m_nStyle;
	m_stAnalysisTool.nLineWeight = m_nWidth+1;	
}

void CDlgFiboSet::SetLineExtEnvironment()
{
	UpdateData(TRUE);
	m_pLineExtEnvironment->SetLineExt(m_ctrlLeftExt.GetCheck(),m_ctrlRightExt.GetCheck());

	m_stAnalysisTool.nLeftExt = m_ctrlLeftExt.GetCheck();
	m_stAnalysisTool.nRightExt = m_ctrlRightExt.GetCheck();	
}

void CDlgFiboSet::SetLineExtEnvironment(const bool bLeft,const bool bRight)
{
	m_pLineExtEnvironment->SetLineExt(bLeft,bRight);
}

void CDlgFiboSet::SetColor(const COLORREF& color)
{
	m_ctlColor.SetColor(color);
	m_ctlColor.Invalidate();
}

void CDlgFiboSet::SetTextPosition()
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

	if(m_nTextPosType == G_TEXT_POS_HV)
	{
		SetTextPosition( ( m_ctrlLeftText.GetCheck() ? true : false),
							( m_ctrlRightText.GetCheck() ? true : false),
							( m_ctrlTopText.GetCheck() ? true : false),
							( m_ctrlBottomText.GetCheck() ? true : false));

		m_stAnalysisTool.nTopText = (int)m_ctrlTopText.GetCheck();
		m_stAnalysisTool.nBottomText = (int)m_ctrlBottomText.GetCheck();
		m_stAnalysisTool.nLeftText = (int)m_ctrlLeftText.GetCheck();
		m_stAnalysisTool.nRightText = (int)m_ctrlRightText.GetCheck();	
	}
	else if(m_nTextPosType == G_TEXT_POS_ONE)
	{
		SetTextPosition( m_ctrlLeftText.GetCheck() ? true : false);

		m_stAnalysisTool.nLeftText = (int)m_ctrlLeftText.GetCheck();
	}
	else 
	{
		SetTextPosition( ( m_ctrlLeftText.GetCheck() ? true : false),
							( m_ctrlRightText.GetCheck() ? true : false));
	}	
}

void CDlgFiboSet::SetTextPosition(const bool bShow)
{
	m_pTextPosition->GetTextEnvironment().SetColor(m_ctlColor.GetColor());
	m_pTextPosition->SetShow(bShow);
}

void CDlgFiboSet::SetTextPosition(const bool bLeft, const bool bRight)
{
	if(m_nTextPosType == G_TEXT_POS_VERT && m_pTextVPosition != NULL)
	{
		m_pTextVPosition->SetTop(bLeft);
		m_pTextVPosition->SetBottom(bRight);
		m_pTextVPosition->GetTextEnvironment().SetColor(m_ctlColor.GetColor());

		m_stAnalysisTool.nTopText = (int)bLeft;
		m_stAnalysisTool.nBottomText = (int)bRight;
	}
	else if(m_nTextPosType == G_TEXT_POS_HORZ && m_pTextHPosition != NULL)
	{
		m_pTextHPosition->SetLeft(bLeft);
		m_pTextHPosition->SetRight(bRight);
		m_pTextHPosition->GetTextEnvironment().SetColor(m_ctlColor.GetColor());

		m_stAnalysisTool.nLeftText = (int)bLeft;
		m_stAnalysisTool.nRightText = (int)bRight;	
	}	
}

void CDlgFiboSet::SetTextPosition(const bool bLeft, const bool bRight,const bool bTop, const bool bBottom)
{
	if(m_pTextHVPosition == NULL)
		return;

	m_pTextHVPosition->SetLeft(bLeft);
	m_pTextHVPosition->SetRight(bRight);
	m_pTextHVPosition->SetTop(bTop);
	m_pTextHVPosition->SetBottom(bBottom);
	m_pTextHVPosition->GetTextEnvironment().SetColor(m_ctlColor.GetColor());
}

CTextPosition* CDlgFiboSet::GetTextPosition()
{
	if(m_nTextPosType == G_TEXT_POS_HV)
		return m_pTextHVPosition;
	else if(m_nTextPosType == G_TEXT_POS_VERT)
		return m_pTextVPosition;
	else if(m_nTextPosType == G_TEXT_POS_HORZ)
		return m_pTextHPosition;
	else if(m_nTextPosType == G_TEXT_POS_ONE)
		return m_pTextPosition;

	return NULL;
}

CTextPosition* CDlgFiboSet::GetOrgTextPosition()
{
	if(m_nTextPosType == G_TEXT_POS_HV)
		return &m_baseTextHVPosition;
	else if(m_nTextPosType == G_TEXT_POS_VERT)
		return &m_baseTextVPosition;
	else if(m_nTextPosType == G_TEXT_POS_HORZ)
		return &m_baseTextHPosition;
	else if(m_nTextPosType == G_TEXT_POS_ONE)
		return &m_baseTextPosition;

	return NULL;
}

CLineExtEnvironment* CDlgFiboSet::GetLineExtEnvironment()
{
	return m_pLineExtEnvironment;
}

CLineExtEnvironment* CDlgFiboSet::GetOrgLineExtEnvironment()
{
	return &m_baseLineExtEnvironment;
}

CPenEnvironment* CDlgFiboSet::GetCurrentPenEnvironment() const
{
	return m_pPenEnvironment;
}

void CDlgFiboSet::SetXDateTime(double dXDateTime,int nIndex)
{
	CString strXValue;
	strXValue.Format(_T("%.f"),dXDateTime);	
	if(strXValue.IsEmpty() == FALSE && dXDateTime > 0.0f)
	{	
		if( m_nDataPeriod == CScaleBaseData::HORZ_DAILY		||
			m_nDataPeriod == CScaleBaseData::HORZ_WEEKLEY	||
			m_nDataPeriod == CScaleBaseData::HORZ_MONTHLY)
		{
			if(nIndex == 0)
			{
				m_ctrlEditXValue1.ShowWindow(SW_HIDE);
				m_ctrlEditXValue2.ShowWindow(SW_HIDE);
				m_ctrlEditXValue3.ShowWindow(SW_HIDE);
			}

			int nYear = atoi(strXValue.Left(4));
			int nMonth = atoi(strXValue.Mid(4,2));
			int nDay = atoi(strXValue.Mid(6,2));

			CTime stime(nYear,nMonth,max(1,nDay),0,0,0);
			if(nIndex == 0)
				m_ctrlXDateTime1.SetTime(&stime);	
			else if(nIndex == 1)
				m_ctrlXDateTime2.SetTime(&stime);	
			else if(nIndex == 2)
				m_ctrlXDateTime3.SetTime(&stime);	
		}
		else
		{	
			if(nIndex == 0)
			{
				m_ctrlXDateTime1.ShowWindow(SW_HIDE);
				m_ctrlXDateTime2.ShowWindow(SW_HIDE);
				m_ctrlXDateTime3.ShowWindow(SW_HIDE);
			}

			CString strFormat;		
			if(m_nDataPeriod == CScaleBaseData::HORZ_TICK)
			{
				// 2011.01.17 by SYS >> 분차트 YYYY(년) 추가 : 날짜/시간 14Byte형태 변경 (YYYYMMDDhhmmss)
				// DDHHMM 로 만듬
				if (strXValue.GetLength()==14)
					strFormat.Format(_T("%s/%s:%s"),strXValue.Mid(6,2),strXValue.Mid(8,2),strXValue.Mid(10,2));
				else								
					strFormat.Format(_T("%s/%s:%s"),strXValue.Left(2),strXValue.Mid(2,2),strXValue.Mid(4,2));
				// 2011.01.17 by SYS <<
			}
			else
			{
				if(strXValue.GetLength() < 8)
					strXValue = "0" + strXValue;
				
				// 2011.01.06 by SYS >> 분차트 YYYY(년) 추가
				// MMDDHHMM 로 만듬
				if(strXValue.GetLength() == 14)
					strFormat.Format(_T("%s/%s-%s:%s"),strXValue.Mid(4,2),strXValue.Mid(6,2), strXValue.Mid(8,2),strXValue.Mid(10,2));
				else
					strFormat.Format(_T("%s/%s-%s:%s"),strXValue.Left(2),strXValue.Mid(2,2), strXValue.Mid(4,2),strXValue.Mid(6,2));
				// 2011.01.06 by SYS <<
			}
			
			if(nIndex == 0)
			{
				m_ctrlEditXValue1.ShowWindow(SW_SHOW);
				m_ctrlEditXValue1.SetWindowText(strFormat);
				m_ctrlEditXValue1.EnableWindow(FALSE);
			}
			else if(nIndex == 1)
			{
				m_ctrlEditXValue2.ShowWindow(SW_SHOW);
				m_ctrlEditXValue2.SetWindowText(strFormat);
				m_ctrlEditXValue2.EnableWindow(FALSE);
			}
			else if(nIndex == 2)
			{
				m_ctrlEditXValue3.ShowWindow(SW_SHOW);
				m_ctrlEditXValue3.SetWindowText(strFormat);
				m_ctrlEditXValue3.EnableWindow(FALSE);
			}
		}
	}
}

void CDlgFiboSet::SetYValue(double dYValue,int nIndex)
{
	CString strYValue;
	// 20080911 김진순	환경설정 화면에서 소수점 이하 자리수 잘리는 문제 수정
	//	strYValue.Format(_T("%.f"),dYValue);
	strYValue.Format(_T("%f"),dYValue);
	if(strYValue.IsEmpty() == FALSE)
	{	
		if(nIndex == 0)
			m_ctrlEditValue1.SetWindowText(strYValue);	
		else if(nIndex == 1)
			m_ctrlEditValue2.SetWindowText(strYValue);	
		else if(nIndex == 2)
			m_ctrlEditValue3.SetWindowText(strYValue);	
	}
}

void CDlgFiboSet::OnDatetimechangeDatetimepickerXdate1(NMHDR* pNMHDR, LRESULT* pResult) 
{
	CString strXDate;
	CTime xTime;
	m_ctrlXDateTime1.GetTime(xTime);

	// 2011.02.08 by SYS >> 분차트 YYYY(년) 추가 : 날짜/시간 14Byte형태 변경 (YYYYMMDDhhmmss)
	//strXDate.Format(_T("%04d%02d%02d"),xTime.GetYear(),xTime.GetMonth(),xTime.GetDay());
	//CCoordinate *pCoordinate = m_pElementPointList->GetAt(0);
	//pCoordinate->SetX(atof(strXDate),0);
	
	strXDate.Format(_T("%04d%02d%02d%s"),xTime.GetYear(),xTime.GetMonth(),xTime.GetDay(), _T("000000"));
	__int64 lMakeTime = _atoi64(strXDate);
	CCoordinate *pCoordinate = m_pElementPointList->GetAt(0);
	pCoordinate->SetX((double)lMakeTime,0);
	// 2011.02.08 by SYS <<

	*pResult = 0;
}

void CDlgFiboSet::OnDatetimechangeDatetimepickerXdate2(NMHDR* pNMHDR, LRESULT* pResult) 
{
	CString strXDate;
	CTime xTime;
	m_ctrlXDateTime2.GetTime(xTime);

	// 2011.02.08 by SYS >> 분차트 YYYY(년) 추가 : 날짜/시간 14Byte형태 변경 (YYYYMMDDhhmmss)
	//strXDate.Format(_T("%04d%02d%02d"),xTime.GetYear(),xTime.GetMonth(),xTime.GetDay());
	//CCoordinate *pCoordinate = m_pElementPointList->GetAt(1);
	//pCoordinate->SetX(atof(strXDate),1);
	
	strXDate.Format(_T("%04d%02d%02d%s"),xTime.GetYear(),xTime.GetMonth(),xTime.GetDay(), _T("000000"));
	__int64 lMakeTime = _atoi64(strXDate);
	CCoordinate *pCoordinate = m_pElementPointList->GetAt(1);
	pCoordinate->SetX((double)lMakeTime,1);
	// 2011.02.08 by SYS <<

	*pResult = 0;
}

void CDlgFiboSet::OnDatetimechangeDatetimepickerXdate3(NMHDR* pNMHDR, LRESULT* pResult) 
{
	CString strXDate;
	CTime xTime;
	m_ctrlXDateTime3.GetTime(xTime);

	// 2011.02.08 by SYS >> 분차트 YYYY(년) 추가 : 날짜/시간 14Byte형태 변경 (YYYYMMDDhhmmss)
	//strXDate.Format(_T("%04d%02d%02d"),xTime.GetYear(),xTime.GetMonth(),xTime.GetDay());
	//CCoordinate *pCoordinate = m_pElementPointList->GetAt(2);
	//pCoordinate->SetX(atof(strXDate),2);
	
	strXDate.Format(_T("%04d%02d%02d%s"),xTime.GetYear(),xTime.GetMonth(),xTime.GetDay(), _T("000000"));
	__int64 lMakeTime = _atoi64(strXDate);
	CCoordinate *pCoordinate = m_pElementPointList->GetAt(2);
	pCoordinate->SetX((double)lMakeTime,2);
	// 2011.02.08 by SYS <<

	*pResult = 0;
}

void CDlgFiboSet::SetDateValueLists()
{
	CString strXDate;
	CTime xTime;
	CCoordinate *pCoordinate = NULL;
	if(m_ctrlXDateTime1.IsWindowVisible() == TRUE)
	{
		m_ctrlXDateTime1.GetTime(xTime);

		// 2011.02.08 by SYS >> 분차트 YYYY(년) 추가 : 날짜/시간 14Byte형태 변경 (YYYYMMDDhhmmss)
// 		if( m_nDataPeriod == CScaleBaseData::HORZ_MONTHLY)
// 			strXDate.Format(_T("%04d%02d"),xTime.GetYear(),xTime.GetMonth());
// 		else
// 			strXDate.Format(_T("%04d%02d%02d"),xTime.GetYear(),xTime.GetMonth(),xTime.GetDay());
// 		pCoordinate = m_pElementPointList->GetAt(0);
// 		pCoordinate->SetX(atof(strXDate),0);
// 		m_stAnalysisTool.dStartXPos = atof(strXDate);

		//if( m_nDataPeriod == CScaleBaseData::HORZ_MONTHLY)
		//	strXDate.Format(_T("%04d%02d%02d%s"),xTime.GetYear(),xTime.GetMonth(), 1, _T("000000"));
		//else
		//	strXDate.Format(_T("%04d%02d%02d%s"),xTime.GetYear(),xTime.GetMonth(),xTime.GetDay(), _T("000000"));
		strXDate.Format(_T("%04d%02d%02d%s"),xTime.GetYear(),xTime.GetMonth(),xTime.GetDay(), _T("000000"));
		__int64 lMakeTime = _atoi64(strXDate);

		pCoordinate = m_pElementPointList->GetAt(0);
		pCoordinate->SetX((double)lMakeTime,0);
		m_stAnalysisTool.dStartXPos = (double)lMakeTime;
		// 2011.02.08 by SYS <<
	}
	
	if(m_ctrlXDateTime2.IsWindowVisible() == TRUE)
	{
		m_ctrlXDateTime2.GetTime(xTime);

		// 2011.02.08 by SYS >> 분차트 YYYY(년) 추가 : 날짜/시간 14Byte형태 변경 (YYYYMMDDhhmmss)
// 		if( m_nDataPeriod == CScaleBaseData::HORZ_MONTHLY)
// 			strXDate.Format(_T("%04d%02d"),xTime.GetYear(),xTime.GetMonth());
// 		else
// 			strXDate.Format(_T("%04d%02d%02d"),xTime.GetYear(),xTime.GetMonth(),xTime.GetDay());
// 		pCoordinate = m_pElementPointList->GetAt(1);
// 		pCoordinate->SetX(atof(strXDate),1);
// 		m_stAnalysisTool.dEndXPos = atof(strXDate);

		//if( m_nDataPeriod == CScaleBaseData::HORZ_MONTHLY)
		//	strXDate.Format(_T("%04d%02d%02d%s"),xTime.GetYear(),xTime.GetMonth(), 1, _T("000000"));
		//else
		//	strXDate.Format(_T("%04d%02d%02d%s"),xTime.GetYear(),xTime.GetMonth(),xTime.GetDay(), _T("000000"));
		strXDate.Format(_T("%04d%02d%02d%s"),xTime.GetYear(),xTime.GetMonth(),xTime.GetDay(), _T("000000"));
		__int64 lMakeTime = _atoi64(strXDate);

		pCoordinate = m_pElementPointList->GetAt(1);
		pCoordinate->SetX((double)lMakeTime,1);
		m_stAnalysisTool.dEndXPos = (double)lMakeTime;
		// 2011.02.08 by SYS <<
	}

	if(m_ctrlXDateTime3.IsWindowVisible() == TRUE)
	{
		m_ctrlXDateTime3.GetTime(xTime);

		// 2011.02.08 by SYS >> 분차트 YYYY(년) 추가 : 날짜/시간 14Byte형태 변경 (YYYYMMDDhhmmss)
// 		if( m_nDataPeriod == CScaleBaseData::HORZ_MONTHLY)
// 			strXDate.Format(_T("%04d%02d"),xTime.GetYear(),xTime.GetMonth());
// 		else
// 			strXDate.Format(_T("%04d%02d%02d"),xTime.GetYear(),xTime.GetMonth(),xTime.GetDay());
// 		pCoordinate = m_pElementPointList->GetAt(2);
// 		pCoordinate->SetX(atof(strXDate),2);
// 		m_stAnalysisTool.dThirdXPos = atof(strXDate);

		//if( m_nDataPeriod == CScaleBaseData::HORZ_MONTHLY)
		//	strXDate.Format(_T("%04d%02d%02d%s"),xTime.GetYear(),xTime.GetMonth(), 1, _T("000000"));
		//else
		//	strXDate.Format(_T("%04d%02d%02d%s"),xTime.GetYear(),xTime.GetMonth(),xTime.GetDay(), _T("000000"));
		strXDate.Format(_T("%04d%02d%02d%s"),xTime.GetYear(),xTime.GetMonth(),xTime.GetDay(), _T("000000"));
		__int64 lMakeTime = _atoi64(strXDate);

		pCoordinate = m_pElementPointList->GetAt(2);
		pCoordinate->SetX((double)lMakeTime,2);
		m_stAnalysisTool.dThirdXPos = (double)lMakeTime;
		// 2011.02.08 by SYS <<
	}

	/////////////////////////////////////////////////////////////////////////////////////////////////
	CString strYValue;
	if(m_ctrlEditValue1.IsWindowVisible() == TRUE)
	{
		m_ctrlEditValue1.GetWindowText(strYValue);
		pCoordinate = m_pElementPointList->GetAt(0);
		pCoordinate->SetY(atof(strYValue));
		m_stAnalysisTool.dStartYValue = atof(strYValue);
	}
	
	if(m_ctrlEditValue2.IsWindowVisible() == TRUE)
	{
		m_ctrlEditValue2.GetWindowText(strYValue);
		pCoordinate = m_pElementPointList->GetAt(1);
		pCoordinate->SetY(atof(strYValue));
		m_stAnalysisTool.dEndYValue = atof(strYValue);
	}

	if(m_ctrlEditValue3.IsWindowVisible() == TRUE)
	{
		m_ctrlEditValue3.GetWindowText(strYValue);
		pCoordinate = m_pElementPointList->GetAt(2);
		pCoordinate->SetY(atof(strYValue));
		m_stAnalysisTool.dThirdYValue = atof(strYValue);
	}
}

void CDlgFiboSet::OnSelchangeWidth() 
{
	int nWidth = m_ctlWidth.GetCurSel();
	int nStyle = m_ctlStyle.GetCurSel();
	if(nStyle != 0 && nWidth > 0)
	{
		ML_SET_LANGUAGE_RES();

		CString strNotice;
		strNotice.LoadString(IDS_CHANGE_THICKNESS);
		AfxMessageBox(strNotice);
		m_ctlStyle.SetCurSel(0);
	}
}

void CDlgFiboSet::OnSelchangeStyle() 
{
	int nWidth = m_ctlWidth.GetCurSel();
	int nStyle = m_ctlStyle.GetCurSel();
	if(nStyle > 0 && nWidth != 0)
	{
		ML_SET_LANGUAGE_RES();

		CString strNotice;
		strNotice.LoadString(IDS_CHANGE_THICKNESS);
		AfxMessageBox(strNotice);
		m_ctlWidth.SetCurSel(0);
	}
}


void CDlgFiboSet::OnCancelBt() 
{
	ApplyOrgData();

	CDialog::OnCancel();	
}

void CDlgFiboSet::OnOkBt() 
{
	ApplyData();
	SaveData();

	CDialog::OnOK();	
}

void CDlgFiboSet::OnApplyBt() 
{
	ApplyData();
}
