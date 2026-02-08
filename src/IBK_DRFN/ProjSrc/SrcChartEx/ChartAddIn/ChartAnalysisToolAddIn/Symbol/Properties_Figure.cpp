// Properties_Figure.cpp : implementation file
//

#include "stdafx.h"
#include "symbol.h"
#include "Properties_Figure.h"
#include "../Include_Chart/Dll_Load/IMetaTable.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#include "PropertiesData_text.h"
const TCHAR *const G_ANALYSIS_PROPERTY_INFO	= "AnalysisPropInfo.ans";
/////////////////////////////////////////////////////////////////////////////
// CProperties_Figure dialog


// 2011.02.08 by SYS >> 분차트 YYYY(년) 추가 : 날짜/시간 14Byte형태 변경 (YYYYMMDDhhmmss)
// CProperties_Figure::CProperties_Figure( CPropertiesData_text* pProperties,
// 									    const CSymbolBaseData::TOOLTYPE &nElementType,
// 									    const CString& strUserPath,
// 									    const CScaleBaseData::HORZSCALEDRAWERTYPE& nDataPeriod,
// 									    const CCoordinate& coordStart,
// 										const CCoordinate& coordEnd,CWnd* pParent /*=NULL*/)
CProperties_Figure::CProperties_Figure( IChartManager* pMainBlock,
										const CBlockIndex& blockIndex, 
										const CString & strFormatX,
										CPropertiesData_text* pProperties,
									    const CSymbolBaseData::TOOLTYPE &nElementType,
									    const CString& strUserPath,
									    const CScaleBaseData::HORZSCALEDRAWERTYPE& nDataPeriod,
									    const CCoordinate& coordStart,
										const CCoordinate& coordEnd,CWnd* pParent /*=NULL*/)
// 2011.02.08 by SYS <<
	: CDialog(CProperties_Figure::IDD, pParent)
{
	//{{AFX_DATA_INIT(CProperties_Figure)
	m_nWidth = 0;
	m_nStyle = 0;
	//}}AFX_DATA_INIT
	m_pProperties = pProperties;
	m_nDataPeriod = nDataPeriod;
	m_coordStart = coordStart;
	m_coordEnd = coordEnd;
	m_strUserPath = strUserPath;
	m_nElementType = nElementType;

	m_hOcxWnd = ( pParent ? pParent->GetSafeHwnd() : NULL);

	// 2011.02.08 by SYS >> 분차트 YYYY(년) 추가 : 날짜/시간 14Byte형태 변경 (YYYYMMDDhhmmss)
	m_pMainBlock = pMainBlock;

	if(blockIndex.IsAllValueBiggerThanZero())
	{
		m_blockIndex.SetRow(blockIndex.GetRow());
		m_blockIndex.SetColumn(blockIndex.GetColumn());
	}
	m_strFormatX	= strFormatX;
	// 2011.02.08 by SYS <<
}

void CProperties_Figure::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CProperties_Figure)
	DDX_Control(pDX, IDC_CHECK, m_Fill_Bt);
	DDX_Control(pDX, IDC_COLOR, m_Color_Bt);
	DDX_Control(pDX, IDC_STYLE, m_ctlStyle);
	DDX_Control(pDX, IDC_WIDTH, m_ctlWidth);

	DDX_Control(pDX, IDC_EDIT_VALUE1, m_ctrlEditValue1);
	DDX_Control(pDX, IDC_EDIT_VALUE2, m_ctrlEditValue2);
	
	DDX_Control(pDX, IDC_EDIT_XDATE1, m_ctrlEditXValue1);
	DDX_Control(pDX, IDC_EDIT_XDATE2, m_ctrlEditXValue2);
	
	DDX_Control(pDX, IDC_DATETIMEPICKER_XDATE1, m_ctrlXDateTime1);
	DDX_Control(pDX, IDC_DATETIMEPICKER_XDATE2, m_ctrlXDateTime2);

	DDX_CBIndex(pDX, IDC_WIDTH, m_nWidth);
	DDX_CBIndex(pDX, IDC_STYLE, m_nStyle);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CProperties_Figure, CDialog)
	//{{AFX_MSG_MAP(CProperties_Figure)
	ON_BN_CLICKED(IDC_COLOR, OnColor)
	ON_BN_CLICKED(IDC_CHECK, OnCheck)
	ON_CBN_SELCHANGE(IDC_WIDTH, OnSelchangeWidth)
	ON_CBN_SELCHANGE(IDC_STYLE, OnSelchangeStyle)
	ON_NOTIFY(DTN_DATETIMECHANGE, IDC_DATETIMEPICKER_XDATE1, OnDatetimechangeDatetimepickerXdate1)
	ON_NOTIFY(DTN_DATETIMECHANGE, IDC_DATETIMEPICKER_XDATE2, OnDatetimechangeDatetimepickerXdate2)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CProperties_Figure message handlers

void CProperties_Figure::OnColor() 
{
	CColorDialog color;
	if(color.DoModal() == IDOK) {
		m_pProperties->SetColor(color.GetColor());
		m_Color_Bt.SetColor(m_pProperties->GetColor());
		m_Color_Bt.Invalidate();
	}	
}

void CProperties_Figure::InitializeData_Width()
{
	ML_SET_DEFAULT_RES();

	m_ctlWidth.LoadBitmap(IDB_LINEWIDTH, 5);

	if(m_pProperties->GetWeight() <= 0)
		m_nWidth = 0;
	else if(m_pProperties->GetWeight() >= 6)
		m_nWidth = 4;
	else
		m_nWidth = m_pProperties->GetWeight() - 1;
}

void CProperties_Figure::InitializeData_Style()
{
	ML_SET_DEFAULT_RES();

	m_ctlStyle.LoadBitmap(IDB_LINESTYLE, 5);

	m_nStyle = (int)m_pProperties->GetStyle();
}

BOOL CProperties_Figure::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	//color
	m_Color_Bt.SetColor(m_pProperties->GetColor());
	m_Fill_Bt.SetCheck(m_pProperties->GetFillState());

	InitializeData_Width();
	InitializeData_Style();
	UpdateData(FALSE);

	// 2011.02.08 by SYS >> 분차트 YYYY(년) 추가 : 날짜/시간 14Byte형태 변경 (YYYYMMDDhhmmss)

// 	SetXDateTime(m_coordStart.GetX(),0);
// 	SetYValue(m_coordStart.GetY(),0);
// 
// 	SetXDateTime(m_coordEnd.GetX(),1);
// 	SetYValue(m_coordEnd.GetY(),1);

	int i = 0;
	for (i = 0; i < 2; i++)
	{
		double dX = 0;
		double dY = 0;
		if (i == 0)
		{
			dX = m_coordStart.GetX();
			dY = m_coordStart.GetY();
		}
		else if (i == 1)
		{
			dX = m_coordEnd.GetX();
			dY = m_coordEnd.GetY();
		}

		if (m_strFormatX == _T("YYYYMMDDHHMMSS"))
		{
			if( m_nDataPeriod == CScaleBaseData::HORZ_WEEKLEY	||
				m_nDataPeriod == CScaleBaseData::HORZ_MONTHLY)
			{
				// 주간, 월간은 현재 차트 날짜주기로 변경한다.
				if (m_pMainBlock) 
				{
					// 
					int nRow = m_blockIndex.GetRow();
					int nCol = m_blockIndex.GetColumn();

					int nCenter_x;
					int nOrgXPos		= m_pMainBlock->GetXFromDouble(nRow, nCol, dX, nCenter_x);

					int nBlockColumn;
					double dModifyData	= m_pMainBlock->GetDoubleFromX(nRow, nCol, nOrgXPos, &nBlockColumn, nCenter_x);
					dX = dModifyData;
				}
			}
		}

		SetXDateTime(dX,i);
		SetYValue(dY,i);
	}
	// 2011.02.08 by SYS <<

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CProperties_Figure::OnCheck() 
{
	m_pProperties->SetFillState(m_Fill_Bt.GetCheck() ? true : false);
}

void CProperties_Figure::SetXDateTime(double dXDateTime,int nIndex)
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
			}

			int nYear = atoi(strXValue.Left(4));
			int nMonth = atoi(strXValue.Mid(4,2));
			int nDay = atoi(strXValue.Mid(6,2));

			CTime stime(nYear,nMonth,max(1,nDay),0,0,0);
			if(nIndex == 0)
				m_ctrlXDateTime1.SetTime(&stime);	
			else if(nIndex == 1)
				m_ctrlXDateTime2.SetTime(&stime);	
		}
		else
		{	
			if(nIndex == 0)
			{
				m_ctrlXDateTime1.ShowWindow(SW_HIDE);
				m_ctrlXDateTime2.ShowWindow(SW_HIDE);
			}

			CString strFormat;		

			if(m_nDataPeriod == CScaleBaseData::HORZ_TICK)
			{
				// 2011.01.17 by SYS >> 분차트 YYYY(년) 추가 : 날짜/시간 14Byte형태 변경 (YYYYMMDDhhmmss)
				// DDHHMM
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
				if(strXValue.GetLength()==14)
					strFormat.Format(_T("%s/%s-%s:%s"),strXValue.Mid(4,2),strXValue.Mid(6,2),
						strXValue.Mid(8,2),strXValue.Mid(10,2));
				else
					// 2011.01.06 by SYS <<
					strFormat.Format(_T("%s/%s-%s:%s"),strXValue.Left(2),strXValue.Mid(2,2),
						strXValue.Mid(4,2),strXValue.Mid(6,2));
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
		}
	}
}

void CProperties_Figure::SetYValue(double dYValue,int nIndex)
{
	CString strYValue;
	// 20080911 김진순	환경설정 화면에서 소수점 이하 자리수 잘리는 문제 수정
	//strYValue.Format(_T("%.f"),dYValue);
	strYValue.Format(_T("%f"),dYValue);
	if(strYValue.IsEmpty() == FALSE)
	{	
		if(nIndex == 0)
			m_ctrlEditValue1.SetWindowText(strYValue);	
		else if(nIndex == 1)
			m_ctrlEditValue2.SetWindowText(strYValue);	
	}
}

void CProperties_Figure::OnDatetimechangeDatetimepickerXdate1(NMHDR* pNMHDR, LRESULT* pResult) 
{
	CString strXDate;
	CTime xTime;
	m_ctrlXDateTime1.GetTime(xTime);

	// 2011.02.08 by SYS >> 분차트 YYYY(년) 추가 : 날짜/시간 14Byte형태 변경 (YYYYMMDDhhmmss)
	//strXDate.Format(_T("%04d%02d%02d"),xTime.GetYear(),xTime.GetMonth(),xTime.GetDay());
	//m_coordStart.SetX(atof(strXDate));
	strXDate.Format(_T("%04d%02d%02d%s"),xTime.GetYear(),xTime.GetMonth(),xTime.GetDay(), _T("000000"));
	__int64 lMakeTime = _atoi64(strXDate);
	m_coordStart.SetX((double)lMakeTime);
	// 2011.02.08 by SYS <<
	
	*pResult = 0;
}

void CProperties_Figure::OnDatetimechangeDatetimepickerXdate2(NMHDR* pNMHDR, LRESULT* pResult) 
{
	CString strXDate;
	CTime xTime;
	m_ctrlXDateTime2.GetTime(xTime);

	// 2011.02.08 by SYS >> 분차트 YYYY(년) 추가 : 날짜/시간 14Byte형태 변경 (YYYYMMDDhhmmss)
	//strXDate.Format(_T("%04d%02d%02d"),xTime.GetYear(),xTime.GetMonth(),xTime.GetDay());
	//m_coordEnd.SetX(atof(strXDate));
	strXDate.Format(_T("%04d%02d%02d%s"),xTime.GetYear(),xTime.GetMonth(),xTime.GetDay(), _T("000000"));
	__int64 lMakeTime = _atoi64(strXDate);
	m_coordEnd.SetX((double)lMakeTime);
	// 2011.02.08 by SYS <<

	*pResult = 0;
}

void CProperties_Figure::SetDateValueLists()
{
	CString strXDate;
	CTime xTime;
	if(m_ctrlXDateTime1.IsWindowVisible() == TRUE)
	{
		m_ctrlXDateTime1.GetTime(xTime);

		// 2011.02.08 by SYS >> 분차트 YYYY(년) 추가 : 날짜/시간 14Byte형태 변경 (YYYYMMDDhhmmss)
		//strXDate.Format(_T("%04d%02d%02d"),xTime.GetYear(),xTime.GetMonth(),xTime.GetDay());
		//m_coordStart.SetX(atof(strXDate));
		strXDate.Format(_T("%04d%02d%02d%s"),xTime.GetYear(),xTime.GetMonth(),xTime.GetDay(), _T("000000"));
		__int64 lMakeTime = _atoi64(strXDate);
		m_coordStart.SetX((double)lMakeTime);
		// 2011.02.08 by SYS <<

		//m_stAnalysisTool.dStartXPos = atof(strXDate);
	}
	
	if(m_ctrlXDateTime2.IsWindowVisible() == TRUE)
	{
		m_ctrlXDateTime2.GetTime(xTime);

		// 2011.02.08 by SYS >> 분차트 YYYY(년) 추가 : 날짜/시간 14Byte형태 변경 (YYYYMMDDhhmmss)
		//strXDate.Format(_T("%04d%02d%02d"),xTime.GetYear(),xTime.GetMonth(),xTime.GetDay());
		//m_coordEnd.SetX(atof(strXDate));
		strXDate.Format(_T("%04d%02d%02d%s"),xTime.GetYear(),xTime.GetMonth(),xTime.GetDay(), _T("000000"));
		__int64 lMakeTime = _atoi64(strXDate);
		m_coordEnd.SetX((double)lMakeTime);
		// 2011.02.08 by SYS <<
		
		//m_stAnalysisTool.dEndXPos = atof(strXDate);
	}

	/////////////////////////////////////////////////////////////////////////////////////////////////
	CString strYValue;
	if(m_ctrlEditValue1.IsWindowVisible() == TRUE)
	{
		m_ctrlEditValue1.GetWindowText(strYValue);
		m_coordStart.SetY(atof(strYValue));
		//m_stAnalysisTool.dStartYValue = atof(strYValue);
	}
	
	if(m_ctrlEditValue2.IsWindowVisible() == TRUE)
	{
		m_ctrlEditValue2.GetWindowText(strYValue);
		m_coordEnd.SetY(atof(strYValue));
		//m_stAnalysisTool.dEndYValue = atof(strYValue);
	}
}

void CProperties_Figure::OnSelchangeWidth() 
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

void CProperties_Figure::OnSelchangeStyle() 
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

CCoordinate CProperties_Figure::GetStartCoord() const
{
	return m_coordStart;
}

CCoordinate CProperties_Figure::GetEndCoord() const
{
	return m_coordEnd;
}

void CProperties_Figure::OnOK() 
{
	SetDateValueLists();
	SaveData();
	CDialog::OnOK();
}

void CProperties_Figure::SaveData()
{
	UpdateData();
	m_pProperties->SetStyle(m_nStyle);
	m_pProperties->SetWeight(m_nWidth+1);

	/////////////////////////////////////////////////////
	CString strFileName;
	strFileName.Format("%s%s",m_strUserPath,G_ANALYSIS_PROPERTY_INFO);
	CFileFind finder;
	BOOL bFind = finder.FindFile(strFileName);
	finder.Close();
	
	if(bFind != TRUE)
	{		
		CFile file;
		if(file.Open(strFileName,CFile::modeCreate|CFile::modeReadWrite) == FALSE)
		{
			return;	
		}
		file.Close();
	}

	CString strSection,strKey,strData,strName;	
	if(m_nElementType == CSymbolBaseData::SYMBOL_SQUARE)
	{
		strSection.Format("%d",101);
	}
	else if(m_nElementType == CSymbolBaseData::SYMBOL_CIRCLE)
	{
		strSection.Format("%d",102);
	}
	else if(m_nElementType == CSymbolBaseData::SYMBOL_TRIANGLE)
	{
		strSection.Format("%d",103);
	}	
		
	strKey = _T("LineColor");
	strData.Format(_T("%ld"),m_pProperties->GetColor());		
	WritePrivateProfileString(strSection,strKey,strData,strFileName);

	strKey = _T("LineStyle");
	strData.Format(_T("%d"),m_pProperties->GetStyle());		
	WritePrivateProfileString(strSection,strKey,strData,strFileName);

	strKey = _T("LineWeight");
	strData.Format(_T("%d"),m_pProperties->GetWeight());		
	WritePrivateProfileString(strSection,strKey,strData,strFileName);

	strKey = _T("FillRegion");
	strData.Format(_T("%d"),m_pProperties->GetFillState());		
	WritePrivateProfileString(strSection,strKey,strData,strFileName);
}

void CProperties_Figure::OnCancel() 
{
	
	CDialog::OnCancel();
}
