// DlgDataSimulator.cpp : implementation file
//

#include "stdafx.h"
#include "ChartDataListViewerAddIn.h"
#include "DlgDataSimulator.h"


#include "../Include_Chart/Conversion.h"				// for CDataConversion
#include "../Include_Chart/DLL_Load/IMetaTable.h"		// for ML_LANGUAGE_RES

#include "DlgInputDayWeek.h"
#include "DlgInputMonth.h"
#include "DlgInputMin.h"
#include "DlgInputTick.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgDataSimulator dialog


CDlgDataSimulator::CDlgDataSimulator( CChartInfo::CHART_MODE p_eChartMode, CWnd* pParent /*=NULL*/)
	: CDialog(CDlgDataSimulator::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgDataSimulator)
	m_strTitle = _T("");
	//}}AFX_DATA_INIT
	m_pChartDataGridManager = NULL;
	m_hOcxWnd				= NULL;
	m_pChartLastRecordSet	= NULL;

	// (2009/9/14 - Seung-Won, Bae) for World-On
	m_eChartMode = p_eChartMode;
}


void CDlgDataSimulator::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgDataSimulator)
		DDX_Control(pDX, IDC_DATA_GRID, m_Grid);             // associate the grid window with a C++ object
	DDX_Text(pDX, IDC_STC_TITLE, m_strTitle);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgDataSimulator, CDialog)
	//{{AFX_MSG_MAP(CDlgDataSimulator)
	ON_BN_CLICKED(IDC_BTN_ADD,			OnBtnAdd)
	ON_BN_CLICKED(IDC_BTN_DEL,			OnBtnDel)
	ON_BN_CLICKED(IDC_BTN_EXCEL_EXPORT, OnBtnExcelExport)	
	ON_BN_CLICKED(IDC_BTN_EXPORT,		OnBtnExport)
	ON_BN_CLICKED(IDC_BTN_IMPORT,		OnBtnImport)
	ON_BN_CLICKED(IDC_BTN_INIT,			OnBtnInit)
	ON_BN_CLICKED(IDC_BTN_REMOVE_ALL,	OnBtnRemoveAll)
	ON_BN_CLICKED(IDM_BTN_OK, OnBtnOk)
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgDataSimulator message handlers

BOOL CDlgDataSimulator::OnInitDialog() 
{
	CDialog::OnInitDialog();

	if(!m_pChartDataGridManager) return TRUE;	

	InitGrid();
	
	MakeCopyChartData();

	MakeTitle();

	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgDataSimulator::MakeTitle()
{
	CString strTitle;
	CString strDataType;

	/////////////////////////////
	// 종목명, 종목코드
	strTitle = " " + m_pChartDataGridManager->m_strItemName + " ( " + m_pChartDataGridManager->m_strItemCode + " ) " + strTitle;


	//////////////////////////////
	// 주기 및 데이터 타입
	CString strDataTypeTemp;
	CStringArray strDataTypeArray; 
	strDataTypeArray.Add("일");strDataTypeArray.Add("주");strDataTypeArray.Add("월");strDataTypeArray.Add("분");strDataTypeArray.Add("틱");	

	if(m_pChartDataGridManager->m_nDataType < 0 || m_pChartDataGridManager->m_nDataType >= strDataTypeArray.GetSize())
		strDataTypeTemp = "";
	else
	{
		//30초인 경우 처리
		if(m_pChartDataGridManager->m_nDataType == MIN_TYPE && m_pChartDataGridManager->m_strDateTime == "DDHHMMSS")
			strDataTypeTemp = "초";
		else
			strDataTypeTemp = strDataTypeArray.GetAt(m_pChartDataGridManager->m_nDataType);
	}

	strDataType.Format(" %d %s ", m_pChartDataGridManager->m_nDataInterval, strDataTypeTemp);
	strTitle = strTitle + strDataType;


	//////////////////////////////////////
	// 기간 (데이터 시작 ~ 데이터 마지막 )
	CString strDateTitle;
	POSITION posHead, posTail;
	posHead = m_pChartDataGridManager->m_ChartDataList.GetHeadPosition();
	CChartDataGridHeaderInfo * pHeaderInfo	= m_pChartDataGridManager->m_ChartDataGridHeaderInfoArray.GetAt(DATE_TIME);
	if(posHead)
	{
		CChartRecordSet * pRecordSet	= m_pChartDataGridManager->m_ChartDataList.GetAt(posHead);
		strDateTitle					= pRecordSet->m_Data[DATE_TIME];		
		strDateTitle					= CDataConversion( m_hOcxWnd, m_eChartMode).GetStringFormatTrans( strDateTitle, pHeaderInfo->m_strPacketType);
	}
	if(m_pChartDataGridManager->m_ChartDataList.GetCount() > 1 && (posTail = m_pChartDataGridManager->m_ChartDataList.GetTailPosition()))
	{
		CChartRecordSet * pRecordSet	= m_pChartDataGridManager->m_ChartDataList.GetAt(posTail);
		CString strDateTitle1			= CDataConversion( m_hOcxWnd, m_eChartMode).GetStringFormatTrans( pRecordSet->m_Data[DATE_TIME], pHeaderInfo->m_strPacketType);
		strDateTitle = strDateTitle + "  ~  " + strDateTitle1;
	}

	strTitle = strTitle + "  (  " + strDateTitle + "  )  ";
	
	m_strTitle = strTitle;
}

void CDlgDataSimulator::MakeCopyChartData()
{
	if(!m_pChartDataGridManager) return;
	
	for( POSITION pos = m_pChartDataGridManager->m_ChartDataList.GetHeadPosition(); pos ; )
	{
		CChartRecordSet * pRecordSet	= m_pChartDataGridManager->m_ChartDataList.GetNext(pos);
		CChartRecordSet * pNewRecordSet = new CChartRecordSet;
		*pNewRecordSet = *pRecordSet;		
		m_CopyChartDataList.AddTail(pNewRecordSet);
	}	
}

void CDlgDataSimulator::InitChartData()
{
	while(!m_pChartDataGridManager->m_ChartDataList.IsEmpty()) delete m_pChartDataGridManager->m_ChartDataList.RemoveHead();

	for( POSITION pos = m_CopyChartDataList.GetHeadPosition(); pos; )
	{
		CChartRecordSet * pRecordSet	= m_CopyChartDataList.GetNext(pos);
		CChartRecordSet * pNewRecordSet = new CChartRecordSet;
		*pNewRecordSet = *pRecordSet;
		m_pChartDataGridManager->m_ChartDataList.AddTail(pNewRecordSet);
	}
}

void CDlgDataSimulator::InitGrid() 
{
    m_Grid.SetVirtualMode(TRUE);
    m_Grid.SetEditable(TRUE);
	m_Grid.EnableDragAndDrop(FALSE);
	m_Grid.GetDefaultCell(FALSE, FALSE)->SetBackClr(RGB(0xFF, 0xFF, 0xFF));	
	m_Grid.EnableTitleTips(TRUE);
	m_Grid.SetRowResize(FALSE);
	m_Grid.SetColumnResize(FALSE);
	m_Grid.SetGridLines(GVL_BOTH); //GVL_NONE, GVL_HORZ, GVL_VERT
	m_Grid.EnableSelection(FALSE);

    {    
        int nCols = m_pChartDataGridManager->m_ChartDataGridHeaderInfoArray.GetSize();
	    int nRows = m_pChartDataGridManager->m_ChartDataList.GetCount()+1;

	    TRY {

			///////////////////////////////////////////////////////
			//기본 줄높이 계산
			CDC * pDC = GetDC();
			CFont * pFont = m_Grid.GetFont();
			CFont * pOldFont = pDC->SelectObject(pFont);
			CSize sz = pDC->GetTextExtent("A");
			pDC->SelectObject(pOldFont);
			ReleaseDC(pDC);
			m_Grid.SetDefCellHeight(sz.cy * 2);


			//기본 컬럼너비 계산
			int nScrollW = GetSystemMetrics(SM_CXVSCROLL);
			CRect rectWnd;
			m_Grid.GetWindowRect(&rectWnd);
			int nColWidth = (rectWnd.Width()-nScrollW) / nCols;
			m_Grid.SetDefCellWidth(nColWidth - 1);
			////////////////////////////////////////////////////

		    m_Grid.SetRowCount(nRows);
		    m_Grid.SetColumnCount(nCols);
		    m_Grid.SetFixedRowCount(1);
		    m_Grid.SetFixedColumnCount(1);

			//셀을 마지막줄로 위치시킴
			m_Grid.EnsureVisible(m_Grid.GetRowCount()-1, 0);
	    }
	    CATCH (CMemoryException, e)
	    {
	    	e->ReportError();
    		return;
	    }
        END_CATCH
    }
    
    // Having weird Autosize problems with CE 2.11 - possibly just an emulation problem
#if !defined(_WIN32_WCE) || (_WIN32_WCE < 211)
	//m_Grid.AutoSize();
#endif
}

void CDlgDataSimulator::OnBtnAdd() 
{
	if(!m_pChartDataGridManager) return;

	ML_SET_LANGUAGE_RES();

	CString strNextDateTime;
	CString strLastDateTime;
	CChartRecordSet * pRecordSet = NULL;
	if(m_pChartDataGridManager->m_ChartDataList.GetCount() <= 0)
	{
		if(!m_pChartLastRecordSet) return;
		pRecordSet = m_pChartLastRecordSet;
	}
	else
	{	
		POSITION pos = m_pChartDataGridManager->m_ChartDataList.GetTailPosition();
		if(!pos) return;
		CChartDataGridHeaderInfo * pHeaderInfo = m_pChartDataGridManager->m_ChartDataGridHeaderInfoArray.GetAt(DATE_TIME);	
		pRecordSet		= m_pChartDataGridManager->m_ChartDataList.GetAt(pos);		
	}

	strLastDateTime = pRecordSet->m_Data[DATE_TIME];

	switch(m_pChartDataGridManager->m_nDataType)
	{
	case DAY_TYPE:
		{
			if(strLastDateTime.GetLength() < 8) return;
			CString strY = strLastDateTime.Left(4);
			CString strM = strLastDateTime.Mid(4,2);
			CString strD = strLastDateTime.Mid(6,2);

			CTime time(atoi(strY), atoi(strM), atoi(strD), 0, 0, 0);
			CTimeSpan timeSpan(m_pChartDataGridManager->m_nDataInterval, 0, 0, 0);
			CTime nextTime = time + timeSpan;
			struct tm	stTm = { 0 };
			struct tm * pTm = nextTime.GetGmtTm(&stTm);
			if(!pTm)
			{
				CString strMsg, strDlgTitle;
				GetWindowText(strDlgTitle);
				strMsg.LoadString( IDS_MSG_ADD_DATE_MAX_ERROR);
				MessageBox(strMsg, strDlgTitle);					
				return;
			}

			strNextDateTime.Format("%04d%02d%02d", nextTime.GetYear(), nextTime.GetMonth(), nextTime.GetDay());
			
			do{		

				CDlgInputDayWeek dlg;	
				dlg.m_strCurrentDate = strNextDateTime;
				if(dlg.DoModal() == IDCANCEL)
					return;

				if(m_pChartDataGridManager->m_ChartDataList.GetCount() <= 0 || strLastDateTime < dlg.m_strCurrentDate)
				{
					strNextDateTime = dlg.m_strCurrentDate;
					break;
				}
				
				CString strMsg, strDlgTitle;
				GetWindowText(strDlgTitle);
				strMsg.LoadString(IDS_MSG_ADD_DATE_ERROR);
				MessageBox(strMsg, strDlgTitle);					

			}while(1);				
		}
		break;
	case WEEK_TYPE:
		{
			if(strLastDateTime.GetLength() < 8) return;
			CString strY = strLastDateTime.Left(4);
			CString strM = strLastDateTime.Mid(4,2);
			CString strD = strLastDateTime.Mid(6,2);
			CTime time(atoi(strY), atoi(strM), atoi(strD), 0, 0, 0);
			CTimeSpan timeSpan(m_pChartDataGridManager->m_nDataInterval * 7 , 0, 0, 0);
			CTime nextTime = time + timeSpan;
			struct tm	stTm = { 0 };
			struct tm * pTm = nextTime.GetGmtTm(&stTm);
			if(!pTm)
			{
				CString strMsg, strDlgTitle;
				GetWindowText(strDlgTitle);
				strMsg.LoadString(IDS_MSG_ADD_DATE_MAX_ERROR);
				MessageBox(strMsg, strDlgTitle);					
				return;
			}
			strNextDateTime.Format("%04d%02d%02d", nextTime.GetYear(), nextTime.GetMonth(), nextTime.GetDay());


			do{

				CDlgInputDayWeek dlg;
				dlg.m_strCurrentDate = strNextDateTime;
				if(dlg.DoModal() == IDCANCEL)
					return;

				if(m_pChartDataGridManager->m_ChartDataList.GetCount() <= 0 || strLastDateTime < dlg.m_strCurrentDate)
				{
					strNextDateTime = dlg.m_strCurrentDate;
					break;
				}

				CString strMsg, strDlgTitle;
				GetWindowText(strDlgTitle);
				strMsg.LoadString(IDS_MSG_ADD_DATE_ERROR);
				MessageBox(strMsg, strDlgTitle);	
				
			}while(1);
		}
		break;
	
	case MONTH_TYPE:
		{
			if(strLastDateTime.GetLength() < 6) return;
			CString strY = strLastDateTime.Left(4);
			CString strM = strLastDateTime.Mid(4,2); 
			int nMonth   = atoi(strM);
			int nAddMonth= ( nMonth + m_pChartDataGridManager->m_nDataInterval ) % 12; 
			nAddMonth	 = (nAddMonth == 0) ? 12 : nAddMonth;
			int nAddYear = ( nMonth + m_pChartDataGridManager->m_nDataInterval - 1) / 12 ;
			if(nAddYear + atoi(strY) >= 2038)
			{
				CString strMsg, strDlgTitle;
				GetWindowText(strDlgTitle);
				strMsg.LoadString(IDS_MSG_ADD_DATE_MAX_ERROR);
				MessageBox(strMsg, strDlgTitle);					
				return;
			}

			strNextDateTime.Format("%04d%02d", nAddYear + atoi(strY), nAddMonth);

			do{

				CDlgInputMonth dlg( m_hOcxWnd);
				dlg.m_strCurrentDate = strNextDateTime;
				if(dlg.DoModal() == IDCANCEL)
					return;

				if(m_pChartDataGridManager->m_ChartDataList.GetCount() <= 0 || strLastDateTime < dlg.m_strCurrentDate)
				{
					strNextDateTime = dlg.m_strCurrentDate;
					break;
				}

				CString strMsg, strDlgTitle;
				GetWindowText(strDlgTitle);
				strMsg.LoadString(IDS_MSG_ADD_DATE_ERROR);
				MessageBox(strMsg, strDlgTitle);	
				
			}while(1);
		}
		break;
		
	case MIN_TYPE:
		{
			//분데이터 중 30초 데이터를 위한 처리.
			if(m_pChartDataGridManager->m_strDateTime == "DDHHMMSS")
			{
				if(strLastDateTime.GetLength() < 6) return;
				CString strD  = strLastDateTime.Mid(0, 2);
				CString strH  = strLastDateTime.Mid(2, 2);
				CString strM  = strLastDateTime.Mid(4, 2);
				CString strS  = strLastDateTime.Mid(6, 2);
							 
				CTime timeCur = CTime::GetCurrentTime();

				CTime time(timeCur.GetYear(), timeCur.GetMonth(), atoi(strD), atoi(strH), atoi(strM), atoi(strS));
				CTimeSpan timeSpan(0, 0, 0, m_pChartDataGridManager->m_nDataInterval);
				CTime nextTime= time + timeSpan;
				strNextDateTime.Format("%02d%02d%02d%02d", nextTime.GetDay(), nextTime.GetHour(), nextTime.GetMinute(), nextTime.GetSecond());

				do{

					CDlgInputTick dlg;
					dlg.m_strCurrentDate = strNextDateTime;
					if(dlg.DoModal() == IDCANCEL)
						return;

					strNextDateTime = dlg.m_strCurrentDate;				
					break;

				}while(1);
			}
			else
			{			
				if(strLastDateTime.GetLength() < 8) return;
				CString strM  = strLastDateTime.Left(2);
				CString strD  = strLastDateTime.Mid(2,2);
				CString strH  = strLastDateTime.Mid(4,2);
				CString strm  = strLastDateTime.Mid(6,2);
				
				CTime timeCur = CTime::GetCurrentTime();

				CTime time(timeCur.GetYear(), atoi(strM), atoi(strD), atoi(strH), atoi(strm), 0);
				CTimeSpan timeSpan(0, 0, m_pChartDataGridManager->m_nDataInterval, 0);
				CTime nextTime= time + timeSpan;
				strNextDateTime.Format("%02d%02d%02d%02d", nextTime.GetMonth(), nextTime.GetDay(), nextTime.GetHour(), nextTime.GetMinute());

				do{
					CDlgInputMin dlg( m_hOcxWnd);
					dlg.m_strCurrentDate = strNextDateTime;
					if(dlg.DoModal() == IDCANCEL)
						return;

					strNextDateTime = dlg.m_strCurrentDate;
					break;

				}while(1);
			}
		}
		break;
	case TICK_TYPE:
		{
			if(strLastDateTime.GetLength() < 6) return;
			CString strD  = strLastDateTime.Mid(0, 2);
			CString strH  = strLastDateTime.Mid(2, 2);
			CString strM  = strLastDateTime.Mid(4, 2);
			CString strS  = strLastDateTime.Mid(6, 2);
						 
			CTime timeCur = CTime::GetCurrentTime();

			CTime time(timeCur.GetYear(), timeCur.GetMonth(), atoi(strD), atoi(strH), atoi(strM), atoi(strS));
			CTimeSpan timeSpan(0, 0, 0, m_pChartDataGridManager->m_nDataInterval);
			CTime nextTime= time + timeSpan;
			strNextDateTime.Format("%02d%02d%02d%02d", nextTime.GetDay(), nextTime.GetHour(), nextTime.GetMinute(), nextTime.GetSecond());

			do{

				CDlgInputTick dlg;
				dlg.m_strCurrentDate = strNextDateTime;
				if(dlg.DoModal() == IDCANCEL)
					return;

				strNextDateTime = dlg.m_strCurrentDate;				
				break;

			}while(1);
		}
		break;
	}
	
	if(!pRecordSet) return;

	CChartRecordSet * pNewRecordSet = new CChartRecordSet;
	*pNewRecordSet = *pRecordSet;
	pNewRecordSet->m_Data[DATE_TIME]= strNextDateTime;
	m_pChartDataGridManager->m_ChartDataList.AddTail(pNewRecordSet);

	int nRowCount = m_Grid.GetRowCount();
	m_Grid.SetRowCount(nRowCount+1);
	m_Grid.EnsureVisible(nRowCount, 0);
}

void CDlgDataSimulator::OnBtnDel() 
{
	CCellID cellID = m_Grid.GetFocusCell();
	if(cellID.row <= 0 || cellID.row >= m_Grid.GetRowCount())
		return;

	if(m_Grid.DeleteRow(cellID.row))
	{
		POSITION pos = m_pChartDataGridManager->m_ChartDataList.FindIndex(cellID.row - 1);
		if(pos)
		{
			CChartRecordSet * pRecordSet = m_pChartDataGridManager->m_ChartDataList.GetAt(pos);
			if(m_pChartDataGridManager->m_ChartDataList.GetCount() == 1)
			{
				if(!m_pChartLastRecordSet) m_pChartLastRecordSet	= new CChartRecordSet;
				*m_pChartLastRecordSet	= *pRecordSet;				
			}
			delete pRecordSet;			
			m_pChartDataGridManager->m_ChartDataList.RemoveAt(pos);
		}

		m_Grid.Invalidate();

		if(m_Grid.GetRowCount() > cellID.row)
			m_Grid.SetFocusCell(cellID.row, cellID.col);
		else
			m_Grid.SetFocusCell(--cellID.row,cellID.col);
	}
}

void CDlgDataSimulator::OnBtnExcelExport() 
{

	BOOL bExcelRun = TRUE;
	int nCols = m_pChartDataGridManager->m_ChartDataGridHeaderInfoArray.GetSize();
	int nRows = m_pChartDataGridManager->m_ChartDataList.GetCount();


	SYSTEMTIME sTime;
	GetSystemTime(&sTime);

	CString  szFilePath ;
	szFilePath.Format("%sChartData_%04d%02d%02d_%02d%02d%02d.nc2", m_strUserFilePath, 
				sTime.wYear, sTime.wMonth, sTime.wDay, sTime.wHour+9, sTime.wMinute, sTime.wSecond);


	CString szExcel = "NO" ;
	CString strKey = "Excel.csv\\shell\\open\\command";
	int Ver = 60 ;
	HKEY hkey = HKEY_CLASSES_ROOT ;

	
	LONG err;
	HKEY openKey;
	BYTE Data[128];
	DWORD type;
	DWORD size=sizeof(Data);

	err=RegOpenKey(hkey,strKey,&openKey);
	if(ERROR ==err)
	   err=RegQueryValueEx(openKey,"",0,&type,Data,&size);
 
	if(err==ERROR_SUCCESS)
	{
		RegCloseKey(openKey);
		CString str(Data);
		szExcel = str;
	}
	else
	{
		RegCloseKey(openKey);
		szExcel = "NO";
	}


	szExcel.MakeLower() ;
	int start = szExcel.Find("excel.exe") ;
	szExcel = szExcel.Left(start + 9) ;
	if(szExcel.Left(1).Compare("\"") == 0)
		szExcel = szExcel.Right(szExcel.GetLength() - 1) ;


	CreateDirectory("C:\\Temp", NULL) ; 
	CFile file;
	CFileException e;
	if( !file.Open( szFilePath, CFile::modeCreate | CFile::modeWrite, &e ) )
	{
		CString strMsg, strDlgTitle;
		GetWindowText(strDlgTitle);
		strMsg.LoadString( IDS_FAIL_TO_CREATE_FILE);
		MessageBox(strMsg, strDlgTitle);
		return;
    }

	int i;
	CString szData, szTmp;
	
	for( i = 0 ; i < nCols ; i++ )
	{
		CChartDataGridHeaderInfo * pHeaderInfo = m_pChartDataGridManager->m_ChartDataGridHeaderInfoArray.GetAt(i);
		szData += pHeaderInfo->m_strTitle + _T("\t");
	}
	szData += _T("\r\n");
	file.Write(szData, szData.GetLength());

	szData.Empty();
	for(POSITION pos = m_pChartDataGridManager->m_ChartDataList.GetHeadPosition(); pos ;)
	{
		CChartRecordSet * pRecordSet = m_pChartDataGridManager->m_ChartDataList.GetNext(pos);

		for( i = 0 ; i < nCols && i < COL_SIZE ; i++ )
		{
			if( i == DATE_TIME )
			{
				CChartDataGridHeaderInfo * pHeaderInfo = m_pChartDataGridManager->m_ChartDataGridHeaderInfoArray.GetAt(i);
				CString strDate = CDataConversion( m_hOcxWnd, m_eChartMode).GetStringFormatTrans( pRecordSet->m_Data[i] , pHeaderInfo->m_strPacketType);
				szData += strDate + _T("\t");
			}
			else
				szData += pRecordSet->m_Data[i] + _T("\t");
		}
		szData += _T("\r\n");
	}
	file.Write(szData, szData.GetLength());

	file.Close();


	// 04.17.2002 파일명 앞뒤에 따옴표(" ")가 있어야 한다
	szExcel = szExcel + " \"" + szFilePath + "\"";	

	ML_SET_LANGUAGE_RES();
	CString strFormat;
	if(bExcelRun)
	{
		if(::WinExec((LPCTSTR)szExcel, SW_SHOW) > 31)
			return;
		else	
		{
			CString szMsg, strFormat;
			strFormat.LoadString( IDS_MSG_FILE_SAVE);
			szMsg.Format( strFormat, szFilePath);
			AfxMessageBox(szMsg); 
			return;
		}
		return;
	}
	else
	{
		CString szMsg, strFormat;
		strFormat.LoadString(IDS_MSG_FILE_SAVE);
		szMsg.Format( strFormat, szFilePath);		
		AfxMessageBox(szMsg); 
		return;
	}
}



void CDlgDataSimulator::OnBtnExport() 
{
	ML_SET_LANGUAGE_RES();

	if(!m_pChartDataGridManager)
	{
		CString strMsg, strTitle;
		strMsg.LoadString( IDS_MSG_PRICE_DATA_ERROR);
		GetWindowText(strTitle);
		MessageBox(strMsg, strTitle);
		return;
	}

	CString strFilter = "DSM Files (*.dsm)|*.dsm||";
	CFileDialog dlg(FALSE, "dsm", "test.dsm", OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT, strFilter);
	if(dlg.DoModal() == IDOK)
	{
		CString strFileName = dlg.GetPathName();

		CFile f;
		if(!f.Open(strFileName, CFile::modeWrite | CFile::modeCreate))
		{
			CString strMsg, strTitle;
			GetWindowText(strTitle);
			strMsg.LoadString( IDS_FAIL_TO_OPEN_FILE);
			MessageBox(strMsg, strTitle);
			return;
		}


		CString strContents, strTemp;
		int i, j, nSize = m_pChartDataGridManager->m_ChartDataGridHeaderInfoArray.GetSize();

		strTemp.Format("HELP_MSG=%s\r\n", m_pChartDataGridManager->m_strHelpMessage);	
		strContents += strTemp;

		strTemp.Format("DATA_TYPE=%d\r\n", m_pChartDataGridManager->m_nDataType);		
		strContents += strTemp;

		strTemp.Format("DATA_INTERVAL=%d\r\n", m_pChartDataGridManager->m_nDataInterval);
		strContents += strTemp;

		strTemp.Format("DATA_DATE_TIME=%s\r\n", m_pChartDataGridManager->m_strDateTime);
		strContents += strTemp;

		strTemp.Format("HEADER_CNT=%d\r\n", nSize); 
		strContents += strTemp;

		for( i = 0 ; i < m_pChartDataGridManager->m_ChartDataGridHeaderInfoArray.GetSize(); i++ )
		{
			CChartDataGridHeaderInfo * pHeaderInfo = m_pChartDataGridManager->m_ChartDataGridHeaderInfoArray.GetAt(i);
			
			strTemp.Format("HEADER_%d=%s\r\n", i, pHeaderInfo->m_strHeader);	strContents += strTemp;
			strTemp.Format("HEADER_TITLE_%d=%s\r\n",  i, pHeaderInfo->m_strTitle);	strContents += strTemp;
			strTemp.Format("HEADER_PACKETTYPE_%d=%s\r\n",i,pHeaderInfo->m_strPacketType);strContents += strTemp;
		}

		i = 0;
		strTemp = "[DATA_START]\r\n"; strContents += strTemp;
		for( POSITION pos = m_pChartDataGridManager->m_ChartDataList.GetHeadPosition(); pos ; i++)
		{
			strTemp = "";
			CChartRecordSet * pRecordSet = m_pChartDataGridManager->m_ChartDataList.GetNext(pos);
			for( j = 0 ; j < COL_SIZE ; j++)
			{			
				strTemp = strTemp + pRecordSet->m_Data[j] + ";";
			}

			strTemp		=	strTemp + "\r\n";
			strContents +=	strTemp;
		}
		
		f.Write(strContents, strContents.GetLength());//tour2k WriteHuge->Write

		f.Close();
	}	
}

void CDlgDataSimulator::OnBtnImport() 
{
	if(!m_pChartDataGridManager)
	{
		CString strMsg, strTitle;
		strMsg.LoadString(IDS_MSG_PRICE_DATA_ERROR);
		GetWindowText(strTitle);
		MessageBox(strMsg, strTitle);
		return;
	}

	CString strFilter = "DSM Files (*.dsm)|*.dsm||";
	CFileDialog dlg(TRUE, "dsm", "test.dsm", OFN_HIDEREADONLY|OFN_FILEMUSTEXIST, strFilter);
	if(dlg.DoModal() == IDOK)
	{
		CString strFileName = dlg.GetPathName();

		CFile f;
		if(!f.Open(strFileName, CFile::modeRead ))
		{
			CString strMsg, strTitle;
			GetWindowText(strTitle);
			strMsg.LoadString(IDS_FAIL_TO_OPEN_FILE);
			MessageBox(strMsg, strTitle);
			return;
		}

		int		 i, nLineLastPos, nFindStartPos = 0, nDataStartPos = 0;
		int		 nHeaderCnt = 0, nFindPos	= 0;
		long lFileSize			= (long)f.GetLength();
		char * pBuffer			= new char[lFileSize];		
		DWORD dwReadLength		= f.Read(pBuffer, lFileSize);//tour2k ReadHuge->Read
		CString strContents		= CString(pBuffer, dwReadLength);
		delete [] pBuffer;
		
		int		nColCount,nDataType, nDataInterval;
		CString strDevider;
		CStringArray	strRecordArray;
		CString strLine, strChartData, strTemp, strHelpMessage;		
		CString strHeader, strHeaderInfo, strHeaderTitle, strHeaderPacketType;
		CString strKey;
		CString strLineEndKey	= "\r\n";
		CChartDataList ChartDataList;
		CChartRecordSet * pNewRecordSet;
		CChartDataGridHeaderInfo * pHeaderInfo;
		CChartDataGridHeaderInfoArray GridHeaderInfoArray;


		//헤더와 데이터 분리..
		strKey		 = "[DATA_START]";
		if((nFindPos = strContents.Find(strKey, nFindStartPos)) < 0)			goto file_error;
		strHeader    = strContents.Left(nFindPos);//데이터에서 헤더 분리.
		nFindStartPos= nFindPos;
		

		if((nLineLastPos = strContents.Find(strLineEndKey, nFindStartPos)) < 0)	goto file_error;		
		nDataStartPos= nLineLastPos + strLineEndKey.GetLength();//데이터의 시작위치

		//////////////////////////////////////////////////////////////////////////////////////
		//헤더파싱

		//0. 보조메세지
		strKey = "HELP_MSG";
		if((nFindPos = strHeader.Find(strKey)) < 0) goto file_error;
		nFindStartPos= nFindPos;

		strKey = "=";		
		if((nFindPos = strHeader.Find(strKey, nFindStartPos)) < 0) goto file_error;		

		nFindStartPos   = nFindPos;
		if((nLineLastPos= strHeader.Find(strLineEndKey, nFindStartPos)) < 0) goto file_error;		
		strHelpMessage  = strHeader.Mid(nFindPos + strKey.GetLength(), nLineLastPos - (nFindStartPos + strKey.GetLength()));


		//1. 데이터 타입
		strKey = "DATA_TYPE";
		if((nFindPos = strHeader.Find(strKey)) < 0) goto file_error;
		nFindStartPos= nFindPos;

		strKey = "=";		
		if((nFindPos = strHeader.Find(strKey, nFindStartPos)) < 0) goto file_error;		

		nFindStartPos   = nFindPos;
		if((nLineLastPos = strHeader.Find(strLineEndKey, nFindStartPos)) < 0) goto file_error;		
		strTemp		= strHeader.Mid(nFindPos + strKey.GetLength(), nLineLastPos - (nFindStartPos + strKey.GetLength()));
		nDataType	= atoi(strTemp);

		//2. 데이터 주기
		strKey = "DATA_INTERVAL";
		if((nFindPos = strHeader.Find(strKey)) < 0) goto file_error;
		nFindStartPos= nFindPos;

		strKey = "=";		
		if((nFindPos = strHeader.Find(strKey, nFindStartPos)) < 0) goto file_error;		

		nFindStartPos   = nFindPos;
		if((nLineLastPos= strHeader.Find(strLineEndKey, nFindStartPos)) < 0) goto file_error;
		strTemp			= strHeader.Mid(nFindPos + strKey.GetLength(), nLineLastPos - (nFindStartPos + strKey.GetLength()));
		nDataInterval = atoi(strTemp);

		//3. 시간날짜 데이터 포멧
		strKey = "DATA_DATE_TIME";
		if((nFindPos = strHeader.Find(strKey)) < 0) goto file_error;
		nFindStartPos= nFindPos;

		strKey = "=";
		if((nFindPos = strHeader.Find(strKey, nFindStartPos)) < 0) goto file_error;		

		nFindStartPos   = nFindPos;
		if((nLineLastPos= strHeader.Find(strLineEndKey, nFindStartPos)) < 0) goto file_error;
		strTemp			= strHeader.Mid(nFindPos + strKey.GetLength(), nLineLastPos - (nFindStartPos + strKey.GetLength()));
		nDataInterval = atoi(strTemp);

		//4. 헤더 컬럼갯수.
		strKey = "HEADER_CNT";
		if((nFindPos = strHeader.Find(strKey)) < 0) goto file_error;
		nFindStartPos= nFindPos;

		strKey = "=";		
		if((nFindPos = strHeader.Find(strKey, nFindStartPos)) < 0) goto file_error;		

		nFindStartPos   = nFindPos;
		if((nLineLastPos = strHeader.Find(strLineEndKey, nFindStartPos)) < 0) goto file_error;
		i = strKey.GetLength();
		strTemp		= strHeader.Mid(nFindPos + strKey.GetLength(), nLineLastPos - (nFindStartPos + strKey.GetLength()));
		nHeaderCnt  = atoi(strTemp);

		//nHeaderCnt = atoi(strHeader.Mid(nFindPos, nLineLastPos - (nFindStartPos + strKey.GetLength())));
		
		if(nHeaderCnt <= 0) 
			goto file_error;
			
		
		//5. 헤더 컬럼개수만큼 각 헤더 정보 추출
		for( i = 0 ; i < nHeaderCnt ; i++ )
		{
			strKey.Format("HEADER_%d", i);
			if((nFindPos = strHeader.Find(strKey)) < 0) goto file_error;
			nFindStartPos= nFindPos;

			strKey = "=";		
			if((nFindPos = strHeader.Find(strKey, nFindStartPos)) < 0) goto file_error;		

			nFindStartPos   = nFindPos;
			if((nLineLastPos = strHeader.Find(strLineEndKey, nFindStartPos)) < 0) goto file_error;
			strHeaderInfo = strHeader.Mid(nFindPos + strKey.GetLength(), nLineLastPos - (nFindStartPos + strKey.GetLength()));


			strKey.Format("HEADER_TITLE_%d", i);
			if((nFindPos = strHeader.Find(strKey)) < 0) goto file_error;
			nFindStartPos= nFindPos;

			strKey = "=";		
			if((nFindPos = strHeader.Find(strKey, nFindStartPos)) < 0) goto file_error;		

			nFindStartPos   = nFindPos;
			if((nLineLastPos= strHeader.Find(strLineEndKey, nFindStartPos)) < 0) goto file_error;
			strHeaderTitle	= strHeader.Mid(nFindPos + strKey.GetLength(), nLineLastPos - (nFindStartPos + strKey.GetLength()));


			strKey.Format("HEADER_PACKETTYPE_%d", i);
			if((nFindPos = strHeader.Find(strKey)) < 0) goto file_error;
			nFindStartPos= nFindPos;

			strKey = "=";		
			if((nFindPos = strHeader.Find(strKey, nFindStartPos)) < 0) goto file_error;		

			nFindStartPos   = nFindPos;
			if((nLineLastPos= strHeader.Find(strLineEndKey, nFindStartPos)) < 0) goto file_error;
			strHeaderPacketType	= strHeader.Mid(nFindPos + strKey.GetLength(), nLineLastPos - (nFindStartPos + strKey.GetLength()));

			pHeaderInfo = new CChartDataGridHeaderInfo;
			pHeaderInfo->m_strTitle = strHeaderTitle;
			pHeaderInfo->m_strHeader= strHeaderInfo;
			pHeaderInfo->m_strPacketType=strHeaderPacketType;
			GridHeaderInfoArray.Add(pHeaderInfo);			

		}			

		//챠트 데이터 추출.
		nColCount		= GridHeaderInfoArray.GetSize();
		strDevider		= ";";
		nFindStartPos	= nDataStartPos;

		while((nLineLastPos = strContents.Find(strLineEndKey, nFindStartPos)) >= 0)
		{
			strLine = strContents.Mid(nFindStartPos, nLineLastPos - nFindStartPos);
			
			//컬럼추출.
			strRecordArray.RemoveAll();
			for( i = 0 , nFindStartPos = 0 ; i < nColCount; i++ )
			{
				if((nFindPos = strLine.Find( strDevider, nFindStartPos )) < 0) 
					strChartData = "0";
				else
				{
					strChartData = strLine.Mid(nFindStartPos, nFindPos - nFindStartPos);
					strChartData.TrimLeft(); strChartData.TrimRight();
				}
				
				strRecordArray.Add(strChartData);

				nFindStartPos = nFindPos + strDevider.GetLength();
			}
			
			pNewRecordSet = new CChartRecordSet;
			for( i = 0 ; i < COL_SIZE && i < strRecordArray.GetSize(); i++ )
				pNewRecordSet->m_Data[i] = strRecordArray.GetAt(i);
			
			ChartDataList.AddTail(pNewRecordSet);

			nFindStartPos = nLineLastPos + strLineEndKey.GetLength();
		}


		for( i = 0 ; i< m_pChartDataGridManager->m_ChartDataGridHeaderInfoArray.GetSize(); i++) 
			delete m_pChartDataGridManager->m_ChartDataGridHeaderInfoArray.GetAt(i);
		m_pChartDataGridManager->m_ChartDataGridHeaderInfoArray.RemoveAll();
		m_pChartDataGridManager->m_ChartDataGridHeaderInfoArray.Append(GridHeaderInfoArray);


		while(!m_pChartDataGridManager->m_ChartDataList.IsEmpty()) delete m_pChartDataGridManager->m_ChartDataList.RemoveHead();
		m_pChartDataGridManager->m_ChartDataList.AddTail(&ChartDataList);

		m_pChartDataGridManager->m_nDataType		= nDataType;

		m_pChartDataGridManager->m_nDataInterval	= nDataInterval;

		m_pChartDataGridManager->m_strHelpMessage	= strHelpMessage;

		f.Close();

		InitGrid();

		MakeTitle();

		UpdateData(FALSE);

		return;

file_error:
		{
			for(int i = 0 ; i< GridHeaderInfoArray.GetSize(); i++) delete GridHeaderInfoArray.GetAt(i);


			CString strMsg, strTitle;
			GetWindowText(strTitle);
			strMsg.LoadString( IDS_MSG_FILE_FORMAT_ERROR);
			MessageBox(strMsg, strTitle);


			f.Close();

			return;
		}
	}		
}

void CDlgDataSimulator::OnBtnInit() 
{
	InitChartData();
	InitGrid();
}

void CDlgDataSimulator::OnBtnRemoveAll() 
{
	if(!m_pChartDataGridManager) return;

	//마지막 레코드 저장해 놓는다.
	POSITION pos = m_pChartDataGridManager->m_ChartDataList.GetTailPosition();
	if(pos)
	{
		CChartRecordSet * pRecordSet= m_pChartDataGridManager->m_ChartDataList.GetAt(pos);
		if(!m_pChartLastRecordSet) m_pChartLastRecordSet = new CChartRecordSet;
		*m_pChartLastRecordSet		= *pRecordSet;
	}

	m_Grid.SetRowCount(1);
	while(!m_pChartDataGridManager->m_ChartDataList.IsEmpty())
		delete m_pChartDataGridManager->m_ChartDataList.RemoveHead();
	m_Grid.Invalidate();
}

void CDlgDataSimulator::OnOK() 
{
	return;
}


LRESULT CDlgDataSimulator::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	if(message == WM_NOTIFY)
	{	
		if (wParam == (WPARAM)m_Grid.GetDlgCtrlID())
		{
			GV_DISPINFO *pDispInfo = (GV_DISPINFO*)lParam;
			if (GVN_GETDISPINFO == pDispInfo->hdr.code)
			{				
				//헤더
				if(pDispInfo->item.row == 0)
				{
					if(pDispInfo->item.col < 0 || pDispInfo->item.col >= m_pChartDataGridManager->m_ChartDataGridHeaderInfoArray.GetSize())
						pDispInfo->item.strText = "";
					else
					{
						CChartDataGridHeaderInfo * pHeaderInfo = m_pChartDataGridManager->m_ChartDataGridHeaderInfoArray.GetAt(pDispInfo->item.col);
						pDispInfo->item.strText = pHeaderInfo->m_strTitle;
					}	
					//pDispInfo->item.mask	= pDispInfo->item.mask | GVIF_FORMAT;
					pDispInfo->item.nFormat= DT_CENTER|DT_VCENTER|DT_SINGLELINE|DT_NOPREFIX;
				}

				//데이터
				else
				{
					if(pDispInfo->item.col < 0 || pDispInfo->item.col >= COL_SIZE)
						pDispInfo->item.strText = "";
					else
					{
						POSITION pos = m_pChartDataGridManager->m_ChartDataList.FindIndex(pDispInfo->item.row - 1);
						if(pos)
						{
							CChartRecordSet * pChartRecordSet = m_pChartDataGridManager->m_ChartDataList.GetAt(pos);
							pDispInfo->item.strText = pChartRecordSet->m_Data[pDispInfo->item.col];
							if(pDispInfo->item.col == DATE_TIME )
							{							
								CChartDataGridHeaderInfo * pHeaderInfo = m_pChartDataGridManager->m_ChartDataGridHeaderInfoArray.GetAt(pDispInfo->item.col);
								pDispInfo->item.strText = CDataConversion( m_hOcxWnd, m_eChartMode).GetStringFormatTrans( pDispInfo->item.strText, pHeaderInfo->m_strPacketType);
							}
						}
					}
					pDispInfo->item.nFormat= DT_RIGHT|DT_VCENTER|DT_SINGLELINE|DT_NOPREFIX;
				}
				
				return TRUE;
			}
			else if(GVN_BEGINLABELEDIT	== pDispInfo->hdr.code)
			{
				//헤더
				if(pDispInfo->item.row == 0)
					return FALSE;
				
				//데이터
				else
				{
					if(pDispInfo->item.col <= 0 || pDispInfo->item.col >= COL_SIZE)
						return FALSE;						
					else
					{
						POSITION pos = m_pChartDataGridManager->m_ChartDataList.FindIndex(pDispInfo->item.row - 1);
						if(pos)
						{
							CChartRecordSet * pChartRecordSet = m_pChartDataGridManager->m_ChartDataList.GetAt(pos);
							CString str = pChartRecordSet->m_Data[pDispInfo->item.col];
							str.Remove(',');
							pChartRecordSet->m_Data[pDispInfo->item.col] = str;
						}
					}					
				}
				return TRUE;
			}
			else if(GVN_ENDLABELEDIT	== pDispInfo->hdr.code)
			{
				GV_DISPINFO *pDispInfo = (GV_DISPINFO*)lParam;
				CString strEdit = pDispInfo->item.strText;
				strEdit.TrimLeft(); strEdit.TrimRight();

				if(pDispInfo->item.col < 0 || pDispInfo->item.col >= m_pChartDataGridManager->m_ChartDataGridHeaderInfoArray.GetSize())
					return FALSE;

				///////////////////////////////////////////////////////
				//데이터 검사
				CChartDataGridHeaderInfo * pHeaderInfo = m_pChartDataGridManager->m_ChartDataGridHeaderInfoArray.GetAt(pDispInfo->item.col);
				if(CDataConversion::IsBaseFormat(pHeaderInfo->m_strPacketType)) 
				{								
					CString strOutput;
					if(!CDataConversion::GetDataToDec(strEdit, pHeaderInfo->m_strPacketType, strOutput)) 
						strEdit = "0";
					else
						strEdit = strOutput;
				}
				strEdit = CDataConversion( m_hOcxWnd, m_eChartMode).GetStringFormatTrans(strEdit, pHeaderInfo->m_strPacketType);						


				///////////////////////////////////////////////////////				
				if(pDispInfo->item.row == 0)
					return FALSE;
				else
				{
					if(pDispInfo->item.col <= 0 || pDispInfo->item.col >= COL_SIZE)
						return FALSE;						
					else
					{
						POSITION pos = m_pChartDataGridManager->m_ChartDataList.FindIndex(pDispInfo->item.row - 1);
						if(pos)
						{
							CChartRecordSet * pChartRecordSet = m_pChartDataGridManager->m_ChartDataList.GetAt(pos);
							pChartRecordSet->m_Data[pDispInfo->item.col] = strEdit;							
						}
					}
				}
				
				return TRUE;
			}
		}
	}
	return CDialog::WindowProc(message, wParam, lParam);
}

BOOL CDlgDataSimulator::PreTranslateMessage(MSG* pMsg) 
{
	if(pMsg->message == WM_KEYDOWN )
	{
		if(pMsg->wParam == VK_ESCAPE)
		{
			return TRUE;
		}
		else if(pMsg->wParam == VK_RETURN && pMsg->hwnd == m_Grid.GetSafeHwnd())
		{
			m_Grid.SendMessage(pMsg->message, pMsg->wParam, pMsg->lParam);
			return TRUE;
		}
	}
	if(pMsg->message == WM_KEYUP)
	{
		if(pMsg->wParam == VK_RETURN)
		{
			return FALSE;
		}
	}
	return CDialog::PreTranslateMessage(pMsg);
}

void CDlgDataSimulator::OnBtnOk() 
{
	if(!m_pChartDataGridManager || m_pChartDataGridManager->m_ChartDataList.GetCount() <= 0)
	{
		CString strTitle, strMsg;
		GetWindowText(strTitle);
		ML_SET_LANGUAGE_RES();
		strMsg.LoadString( IDS_MSG_NO_DATA_SET_TO_CHART);
		if(IDYES == MessageBox(strMsg, strTitle, MB_YESNO))
		{
			CDialog::OnCancel();
			return;
		}
		else
			return;
	}

	m_strLastDateTime = "99999999999999";

	switch(m_pChartDataGridManager->m_nDataType)
	{
	case DAY_TYPE: case WEEK_TYPE:
		{
			POSITION pos = m_pChartDataGridManager->m_ChartDataList.GetTailPosition();
			if(pos)
			{
				CChartRecordSet * pRecord = m_pChartDataGridManager->m_ChartDataList.GetAt(pos);
				m_strLastDateTime.Format("%s240000", pRecord->m_Data[DATE_TIME]);
			}
		}
		break;
	case MONTH_TYPE:
		{
			POSITION pos = m_pChartDataGridManager->m_ChartDataList.GetTailPosition();
			if(pos)
			{
				CChartRecordSet * pRecord = m_pChartDataGridManager->m_ChartDataList.GetAt(pos);
				m_strLastDateTime.Format("%s31240000", pRecord->m_Data[DATE_TIME]);
			}
		}
		break;
	case MIN_TYPE:
		{
			if(m_pChartDataGridManager->m_strDateTime == "DDHHMMSS")
			{
				int i,nY,nM;
				POSITION pos;
				CString strPrevDateTime;
				CTime time = CTime::GetCurrentTime();
				nY = time.GetYear(); nM = time.GetMonth();
				for(i = 0 , pos = m_pChartDataGridManager->m_ChartDataList.GetHeadPosition(); pos ; i++ )
				{
					CChartRecordSet * pRecordSet = m_pChartDataGridManager->m_ChartDataList.GetNext(pos);
					if( i == 0 )
					{
						strPrevDateTime = pRecordSet->m_Data[DATE_TIME];
					}
					else
					{
						if(strPrevDateTime > pRecordSet->m_Data[DATE_TIME])
						{
							int nAddMonth, nAddYear;						
							nAddMonth	= ( nM + 1 ) % 12; 
							nAddMonth	= ( nAddMonth == 0) ? 12 : nAddMonth;
							nAddYear	= ( nM ) / 12 + nY;						
							nY			= nAddYear;
							nM			= nAddMonth;
						}

						strPrevDateTime = pRecordSet->m_Data[DATE_TIME];
					}
				}
				m_strLastDateTime.Format("%04d%02d%s", nY, nM, strPrevDateTime);
			}
			else
			{			
				int i,nY;
				POSITION pos;
				CString strPrevDateTime;
				CTime time = CTime::GetCurrentTime();
				nY = time.GetYear();
				for(i = 0 , pos = m_pChartDataGridManager->m_ChartDataList.GetHeadPosition(); pos ; i++ )
				{
					CChartRecordSet * pRecordSet = m_pChartDataGridManager->m_ChartDataList.GetNext(pos);
					if( i == 0 )
					{
						strPrevDateTime = pRecordSet->m_Data[DATE_TIME];
					}
					else
					{
						if(strPrevDateTime > pRecordSet->m_Data[DATE_TIME])
							nY++;

						strPrevDateTime = pRecordSet->m_Data[DATE_TIME];
					}
				}
				m_strLastDateTime.Format("%04d%s00", nY, strPrevDateTime);
			}
		}
		break;
	case TICK_TYPE:
		{
			int i,nY,nM;
			POSITION pos;
			CString strPrevDateTime;
			CTime time = CTime::GetCurrentTime();
			nY = time.GetYear(); nM = time.GetMonth();
			for(i = 0 , pos = m_pChartDataGridManager->m_ChartDataList.GetHeadPosition(); pos ; i++ )
			{
				CChartRecordSet * pRecordSet = m_pChartDataGridManager->m_ChartDataList.GetNext(pos);
				if( i == 0 )
				{
					strPrevDateTime = pRecordSet->m_Data[DATE_TIME];
				}
				else
				{
					if(strPrevDateTime > pRecordSet->m_Data[DATE_TIME])
					{
						int nAddMonth, nAddYear;						
						nAddMonth	= ( nM + 1 ) % 12; 
						nAddMonth	= ( nAddMonth == 0) ? 12 : nAddMonth;
						nAddYear	= ( nM ) / 12 + nY;						
						nY			= nAddYear;
						nM			= nAddMonth;
					}

					strPrevDateTime = pRecordSet->m_Data[DATE_TIME];
				}
			}
			m_strLastDateTime.Format("%04d%02d%s", nY, nM, strPrevDateTime);
		}
		break;
	}

	CDialog::OnOK();	
}

void CDlgDataSimulator::OnDestroy() 
{
	CDialog::OnDestroy();
	
	if(m_pChartLastRecordSet) 
	{
		delete m_pChartLastRecordSet;
		m_pChartLastRecordSet = NULL;
	}

	while(!m_CopyChartDataList.IsEmpty()) 
		delete m_CopyChartDataList.RemoveHead();
	
}
