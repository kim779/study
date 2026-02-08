#include "PrintController.h"			// for CPrintController
#include "CfgFileVersionChecker.h"		// for CCfgFileVersionChecker

#define FILE_INDICATOR_CONDITIONS_USER "IndiCond_"

// (2006/4/14 - Seung-Won, Bae) Support OpenFromFile with Auto Saving Option.
BOOL CKTBChartCtrl::OpenFromFile(LPCTSTR p_szFileName, BOOL p_bAutoPrevSave) 
{
	// TODO: Add your dispatch handler code here
	CString strSaveFN;
	if( p_bAutoPrevSave) strSaveFN = m_strUserEnvironmentFileName;
	return SaveandOpenFromFileWithMSave( p_szFileName, strSaveFN, TRUE, TRUE);
}

#include "DumpLogger.h"									// for CDumpLogger
static int g_nSaveAndOpenFileCount = 0;
// 02.26.2002  현재의 화면 내용을 파일에 저장하고 지정된 파일을 읽어서 화면을 다시 구성한다
// 03.07.2002  modified indicator list save
BOOL CKTBChartCtrl::SaveandOpenFromFileWithASave(LPCTSTR strOpenFileName, LPCTSTR strSaveFileName, BOOL bOpenOption, BOOL bSaveOption)
{
	// TODO: Add your dispatch handler code here
	CString strSaveFN = strSaveFileName;
	if( bSaveOption && strSaveFN.IsEmpty()) strSaveFN = m_strUserEnvironmentFileName;
	return SaveandOpenFromFileWithMSave( strOpenFileName, strSaveFN, bOpenOption, bSaveOption);
}
// (2008/5/22 - Seung-Won, Bae) Use page info for User File Path Redirection.
BOOL CKTBChartCtrl::WriteUserFiles( void)
{
	// save environment 
	WriteUserFile(CSaveUserFile::ENVIRONMENT);
	// save indicator option		
	WriteUserFile(CSaveUserFile::INDICATORLIST);
	return TRUE;
}
BOOL CKTBChartCtrl::ReadUserFiles( void)
{
	// (2009/5/12 - Seung-Won, Bae) Skip Indicator Calcuatation during adding the graph.
	BOOL bCurrentEnableIndicatorCalculation = GetBEnableIndicatorCalculation();
	SetBEnableIndicatorCalculation( FALSE);

	// initialize
	//sy 2004.12.07. 특수&일반차트 저장과 관련된 부분.
	//	InvalidateChart(); -> ResetSaveChartCfg(0); -> ResetSaveChartCfg( GetChartCfg( 0), false);	
	//sy 2006.04.19. -> 화면의 scroll 관련 부분 복원을 해야 한다.
	// -> MainBlock에 반영한 뒤 scroll 반영하기 : MainBlock의 x축 scale data 에 의해 화면에 보일 갯수 지정하기 때문.
	if( SetOcxDataFromChartCfg( GetChartCfg( 0), false)) SetMainBlock(IsRunTimeMode());

	// (2006/12/10 - Seung-Won, Bae) Remove AllGraph for New Loadiong.
	//		in Graph Deleted, Indicaotr Info can be lost.
	m_pMainBlock->DeleteAllBlock();

	// Indicator속성을 먼저 읽어야 한다. -> user version을 가져오기 위함!
	// read indicator list config file
	BOOL bIndiUserFile = ReadUserFile(CSaveUserFile::INDICATORLIST);

	// read environment config file
	BOOL bChartUserFile = ReadUserFile(CSaveUserFile::ENVIRONMENT);
	if(bChartUserFile)
	{
		SetMainBlock( IsRunTimeMode(), FALSE, FALSE);
		//sy 2004.2.13.
		UserFileVersionChecker();

		m_pMainBlock->ChangeAllIndicatorInfoFromGraphData();

		// (2007/7/2 - Seung-Won, Bae) Do not Support Main Grid Type.
		//	for Old Version Map and User File.
		if( 0 <= m_nVertScaleType) m_pMainBlock->SetAllVertScaleGridType( (CScaleBaseData::SCALEGRIDTYPE)m_nVertScaleType);
	}

	// (2009/5/12 - Seung-Won, Bae) Skip Indicator Calcuatation during adding the graph.
	SetBEnableIndicatorCalculation( bCurrentEnableIndicatorCalculation);
	if( bIndiUserFile || bChartUserFile)
	{
		// 종찬(04/11/16) 조건문 수정
		// 종찬(04/09/07) SetScrollData부분 변경
		ResetScroll();
		InvalidateControl();
	}

	//sy end
	return TRUE;
}
BOOL CKTBChartCtrl::SaveandOpenFromFileWithMSave(LPCTSTR strOpenFileName, LPCTSTR strSaveFileName, BOOL bOpenOption, BOOL bSaveOption) 
{
	if( !m_pMainBlock) return FALSE;

	// TODO: Add your dispatch handler code here
	CString strSaveFileCopy, strOpenFileCopy;
	if( m_bEnableLogSaveAndOpenFromFile)
	{
		CString strLog, strItem;
		if( bSaveOption)
		{
			strSaveFileCopy.Format( "ChartFile%04dSave.cht", g_nSaveAndOpenFileCount++);
			strItem.Format( "%s - Save File Name : [%s]\r\n", strSaveFileCopy, strSaveFileName);
			strLog += strItem;
		}
		if( bOpenOption)
		{
			strOpenFileCopy.Format( "ChartFile%04dOpen.cht", g_nSaveAndOpenFileCount++);
			strItem.Format( "%s - Open File Name : [%s]\r\n", strOpenFileCopy, strOpenFileName);
			strLog += strItem;
		}
		if( strLog.IsEmpty()) strLog.Format( "No Oeration\r\n%s,%s,%d,%d\r\n", strOpenFileName, strSaveFileName, bOpenOption, bSaveOption);
		CDumpLogger::LogStringToFile( "CSfnChartCtrl::SaveandOpenFromFileWithMSave()", strLog, strLog.GetLength(), "SaveandOpenFromFileWithMSave");
	}

	CString strOpenFN = strOpenFileName, strSaveFN = strSaveFileName;
	if(strOpenFN.IsEmpty() && bOpenOption)
		return FALSE;

	// save
	// (2008/5/22 - Seung-Won, Bae) Use page info for User File Path Redirection.
	if( bSaveOption && !strSaveFN.IsEmpty() && !m_bFreezedPageState)
	{
		//sy 2005.07.29.
		SetUserEnvironmentAndIndicatorListFileName(strSaveFN);
		//m_strUserEnvironmentFileName = strSaveFN;
		//m_strUserIndicatorListFileName = FILE_INDICATOR_CONDITIONS_USER + m_strUserEnvironmentFileName;	
		//sy end

		// (2008/5/22 - Seung-Won, Bae) Use page info for User File Path Redirection.
		WriteUserFiles();

		if( m_bEnableLogSaveAndOpenFromFile)
			CopyFile( GetUserFilePathAndName( m_strUserEnvironmentFileName), "C:\\Logs\\" + strSaveFileCopy, FALSE);
	}
	// open
	if(bOpenOption && !strOpenFN.IsEmpty()) if( SetUserEnvironmentAndIndicatorListFileName( strOpenFN))
	{
		// (2008/5/22 - Seung-Won, Bae) Use page info for User File Path Redirection.
		ReadUserFiles();

		if( m_bEnableLogSaveAndOpenFromFile)
			CopyFile( GetUserFilePathAndName( m_strUserEnvironmentFileName), "C:\\Logs\\" + strOpenFileCopy, FALSE);
	}

	return TRUE;
}

//sy 2005.07.29.
BOOL CKTBChartCtrl::SetUserEnvironmentAndIndicatorListFileName(const CString& strFileName)
{
	if(strFileName.IsEmpty())
		return FALSE;

	m_strUserEnvironmentFileName = strFileName;
	// 확장자가 있는지 확인
	// -> 없는 경우 : ".cht" 붙히기
	if(strFileName.Find(".") < 0)
		m_strUserEnvironmentFileName += ".cht";

	m_strUserIndicatorListFileName = FILE_INDICATOR_CONDITIONS_USER + m_strUserEnvironmentFileName;

	// (2007/1/9 - Seung-Won, Bae) Check if Valid File
	BOOL bNotFound = FALSE;
	WIN32_FIND_DATA findFileData;
	CString strUserFile = GetUserFilePathAndName( m_strUserEnvironmentFileName);
	HANDLE hFile = FindFirstFile( strUserFile, &findFileData);
	if( hFile != INVALID_HANDLE_VALUE) FindClose( hFile);
	else bNotFound = TRUE;
	strUserFile = GetUserFilePathAndName( m_strUserIndicatorListFileName);
	hFile = FindFirstFile( strUserFile, &findFileData);
	if( hFile != INVALID_HANDLE_VALUE) FindClose( hFile);
	else bNotFound = TRUE;
	return !bNotFound;
}
//sy end

//sy 2004.2.13.
void CKTBChartCtrl::UserFileVersionChecker()
{
	// user file 과 Indicond.cfg의 버전 체크를 위함.
	// 단, Indicond관련 user 은 IndicatorCondition.dll 에서 체크함
	// 여기서는 MainBlock에 관련된 내용만 수정하면 된다.
	bool bIsCalculate = false;
	BOOL bResult = CCfgFileVersionChecker().UserFileVersionChecker((CKTBChartCtrl*) this, m_pMainBlock, bIsCalculate);
	if(!bResult)
		return;

	// 버전이 틀린경우 한번이라도 지표가 전략에 등록 되어 있으면 packet을 확인하지 않기 때문에 dll을 다시 로드한다.
	if(bIsCalculate && m_pAddInManager != NULL)
	{
		// (2006/8/25 - Seung-Won, Bae) ST Reload for Dynamic Graph Reformming.
		//		Cuation. ST Indicator DLL's Re-Initializing Interface is required for error of drawing or mouse order.
		if( m_pAddInManager->IsAddInLoaded(	g_aszAddInLoadString[ AID_ST_INDICATOR]))
		{
			m_pAddInManager->UnloadAddIn(	g_aszAddInLoadString[ AID_ST_INDICATOR]);
			m_pAddInManager->LoadAddIn(		g_aszAddInLoadString[ AID_ST_INDICATOR]);
		
			OnPacketTRData();
		}
	}
}
//sy end


// 03.01.2002
// Block 수를 조절해주는 메쏘드 -> 비교차트를 위해서 그래프를 하나도 버리지 않는다
BOOL CKTBChartCtrl::JoinandDevisionforCompareChart(long nRowCount, long nColumnCount, BOOL bJoin) 
{
	if(!m_pMainBlock)	return FALSE;
	int i = 0;
	// 원래 블럭수가 더 적으면
	if(nRowCount > m_nBlockRowCount)
	{
		// 일단 블럭수를 더 늘인다
		CString strAllData;
		m_pMainBlock->GetAllBlocksData( strAllData);
		m_pMainBlock->SetBlockCount(CBlockIndex(nRowCount, nColumnCount));
		m_pMainBlock->BuildGraphsAndScales(strAllData);
		// 첫번째 Block에 있는 차트를 추가된 Block으로 옮긴다
		for( i = 0 ; i < m_nBlockRowCount ; i++)
			m_pMainBlock->SetHorzScalePosition(CBlockIndex(i, 0), CScaleBaseData::HORZ_HIDE);
		for(i = m_nBlockRowCount ; i < nRowCount ; i++)	{
			// scale group  이동 -> scale group이 하나이면 graph 이동
			if(!m_pMainBlock->MoveGraph(CBlockIndex(0, 0), 1, 0, CBlockIndex(i, 0)))
				m_pMainBlock->MoveGraph(CBlockIndex(0, 0), 0, 1, CBlockIndex(i, 0));				
			// 가로 scale 설정
			m_pMainBlock->SetHorzScaleData(CBlockIndex(i, 0), _MTEXT( C0_DATE_TIME));
			m_pMainBlock->SetHorzScalePosition(CBlockIndex(i, 0), CScaleBaseData::HORZ_HIDE);
		}
		m_pMainBlock->SetHorzScaleCompart(CBlockIndex(nRowCount - 1, 0), "/", ":");
		m_pMainBlock->SetHorzScalePosition(CBlockIndex(nRowCount - 1, 0), CScaleBaseData::HORZ_BOTTOM);
	}
	// 원래 블럭수가 더 많으면
	else if(nRowCount < m_nBlockRowCount)	{
		// 삭제된 Block에 있는 차트를 맨 위 block으로 옮긴다
		for( i = nRowCount ; i < m_nBlockRowCount ; i++)	{
			if(bJoin)	
				m_pMainBlock->MoveAllGraphs(CBlockIndex(nRowCount, 0), 0, CBlockIndex(0, 0), 0);
			else
				m_pMainBlock->MoveAllVertScaleGroups(CBlockIndex(nRowCount, 0), CBlockIndex(0, 0));
		}
		m_pMainBlock->SetHorzScalePosition(CBlockIndex(nRowCount - 1, 0), CScaleBaseData::HORZ_BOTTOM);
	}
	else if(nRowCount == m_nBlockRowCount)	{
		for( i = 0 ; i < nRowCount ; i++)	{
			m_pMainBlock->JoinAllVertScaleGroups(CBlockIndex(i, 0), CDataConversion::atob(bJoin));
		}
	}
	m_nBlockRowCount = nRowCount;
	m_nBlockColumnCount = nColumnCount;

	//sy 2006.03.02
	SetOneChart();

	InvalidateControl();
	return TRUE;
}

//sy 2006.03.02
void CKTBChartCtrl::SetOneChart()
{
	if(m_pToolBarManager == NULL)
		return;

	// 종찬(05/01/04) 특수차표일때 맵저장된후, 일반차트등으로 변환시 SliderCtrl이 Disable되는 버그 수정
	// (2005.01.12, 배승원) Design Time시에 ToolBar가 없는 경우를 감안한다.
	if(IsRunningOneChart())
	{
		// (2009/11/6 - Seung-Won, Bae) Do not disable zoom and scroll on OneChart
		m_pToolBarManager->SetOneChart(true, IsRunningNoZoomChart());
		// OneChart인지 : 복수종목 - ojtaso (20070627)
		m_pPacketListManager->SetOneChart(TRUE, m_strRQ);
	}
	else
	{
		m_pToolBarManager->SetOneChart(false, false);
		// OneChart인지 : 복수종목 - ojtaso (20070627)
		m_pPacketListManager->SetOneChart(FALSE, m_strRQ);
	}

	ResetScroll( TRUE);
}
//sy end

// 03.19.2002  Menu : Display 개수 초기화
void CKTBChartCtrl::OnReturnOrgDisplay() 
{
	CChartCfg * pChart = m_SaveChartcfg.GetChartCfg(0);
	if(!pChart)
		return;

	m_bWholeView = pChart->IsWholeView();
	SetOnePageDataCount(pChart->GetOnePageDataCount());
	ResetScroll();

	// (2009/2/19 - Seung-Won, Bae) Notify to Container for user changing.
	OnUserZoomChanged( m_nOnePageDataCount);
}

// 03.20.2002  Change Title
BOOL CKTBChartCtrl::ChangeGraphTitle(short nRowIndex, short nColumnIndex, short nGraphIndex, LPCTSTR strNewTitle2) 
{
	if(!m_pMainBlock)	return FALSE;

	CString strNewTitle = _STOM1( strNewTitle2);

	CString strName = strNewTitle;
	strName.TrimRight();
	BOOL b = m_pMainBlock->ChangeSubGraphTitle(CBlockIndex(nRowIndex, nColumnIndex), 0, nGraphIndex, 0, strName);
	if(!b)	{
		if(m_pMainBlock->ChangeSubGraphTitle(CBlockIndex(nRowIndex, nColumnIndex), nGraphIndex, 0, 0, strName))	{
			InvalidateControl();
			return TRUE;
		}
		else
			return FALSE;
	}
	InvalidateControl();
	return TRUE;
}

// 03.20.2002  Menu : 인쇄
void CKTBChartCtrl::OnPrint() 
{
	CPrintController controller;
	controller.OnPrint(m_rcMain, this);
}

// 03.22.2002  graph가 select되었을 때 처리
LRESULT CKTBChartCtrl::OnGraphSelectedIndex(WPARAM wParam, LPARAM lParam)
// wparam : data index
// lparam : indicator function name
{
	int nDataIndex = (int)wParam;
	CString strGraphName = (LPCTSTR)lParam;

	// 해당 index의 날짜를 얻어온다
	int nDate = (int)m_pPacketList->GetDataFromIndex( _MTEXT( C0_DATE_TIME), nDataIndex);
	CString strDate = CDataConversion::IntToString(nDate);
	// news 일때 이벤트 발생
	if(strGraphName == "News")
		NeoRequestMapLoad(3, "", strDate);
	// 거래내역일때 이벤트 발생
	else if(strGraphName == _MTEXT( C2_DEAL_LIST))
		NeoRequestMapLoad(4, "", strDate);
//	CString msg;
//	msg.Format("%s, %d, %s", strGraphName, nDataIndex, strDate);
//	AfxMessageBox(msg);
	return 1L;
}

// 03.26.2002  파일에서 데이타 읽기
#include <io.h>
short CKTBChartCtrl::SetChartDataFromFile(LPCTSTR szFileName, short nType) 
{
	//sy 2003.8.19.
	CString strFilePathAndName = GetDefaultFilePathAndName(szFileName);
	if(strFilePathAndName.IsEmpty())
		return FALSE;

	FILE* pStream = fopen(strFilePathAndName, "rb");
	if(pStream == NULL)
		return FALSE;

	//fseek(pStream, 0L, SEEK_SET);
	//long nbytes = _filelength(pStream->_file);

	fseek(pStream, 0L, SEEK_END);//tour2k
	long nbytes = ftell(pStream);
	fseek(pStream, 0L, SEEK_SET);

	char* buffer = new char[nbytes + 1];
	ZeroMemory(buffer, nbytes + 1);
	fread(buffer, sizeof(char), nbytes, pStream);
    fclose(pStream);

	SetChartDataStr((CString)buffer, 0, nType);
	delete[] buffer;
	return 1;
}

// 04.03.2002  Graph 삭제
BOOL CKTBChartCtrl::RemoveChart( LPCTSTR szIndicatorName2) 
{
	if(!m_pMainBlock) return FALSE;

	CString szIndicatorName = _STOM2( szIndicatorName2);
	if( !m_pMainBlock->DeleteAllIndicator( szIndicatorName)) return FALSE;
	InvalidateControl();
	return TRUE;
}

// (2007/1/20 - Seung-Won, Bae) Remove Graph with Graph Name.
BOOL CKTBChartCtrl::RemoveGraphWithGraphName( const char *p_szGraphName)
{
	if(!m_pMainBlock) return FALSE;
	if( !m_pMainBlock->DeleteGraph( p_szGraphName)) return FALSE;
	InvalidateControl();
	return TRUE;
}

// 04.03.2002  toolbar : 전체 최대최소 보기
void CKTBChartCtrl::OnFullMinMax(int nOption)
{
	CClientDC dc(this);
	CFont* pOldFont = dc.SelectObject(m_pRscFont);
	m_pMainBlock->SetAllVScaleMinMaxType( nOption == 1 ? CScaleBaseData::VERT_MINMAX_OF_FULL : CScaleBaseData::VERT_MINMAX_OF_VIEW, &dc);
	dc.SelectObject(pOldFont);
	InvalidateControl();
}

// 04.11.2002   Method : graph의 타입만을 바꾼다
BOOL CKTBChartCtrl::ChangeGraphType(LPCTSTR strGraphName2, LPCTSTR strGraphType2, LPCTSTR strGraphStyle2, LPCTSTR strPacketNameList2, BOOL bRunatOnce) 
{
	CString strGraphName = _STOM2( strGraphName2);
	CString strGraphType = _STOM5( strGraphType2);
	CString strGraphStyle = _STOM5( strGraphStyle2);
	CString strPacketNameList = _STOMS( strPacketNameList2, ",;", 0);

	// (2004.11.05, 배승원) m_infoChangeStyle를 설정만 하고 참조하는 Routine이 없어서 주석처리한다.
//	if(bRunatOnce)
//	{
//		m_infoChangeStyle.bUse = FALSE;
		ChangeChartType(strGraphName, strGraphType, strGraphStyle, strPacketNameList);		
//	}
//	else	{
//		m_infoChangeStyle.bUse = TRUE;
//		m_infoChangeStyle.bWait = TRUE;
//		m_infoChangeStyle.szGraphName = strGraphName;
//		m_infoChangeStyle.szGraphType = strGraphType;
//		m_infoChangeStyle.szGraphStyle = strGraphStyle;
//		m_infoChangeStyle.szPacketNameList = strPacketNameList;
//	}
	return TRUE;
}

bool CKTBChartCtrl::OnReturnOrg2(const BOOL bIsMessageBox)
{
	//sy 2004.05.12.
	if(bIsMessageBox)
	{
		ML_SET_LANGUAGE_RES();
		CString strTitle;
		strTitle.LoadString( IDS_CHART_SETUP);
		CString text;
		text.LoadString( IDS_RESET_CHART);
		int nOk = MessageBox(text, strTitle, MB_OKCANCEL | MB_ICONQUESTION);
		if(nOk != IDOK)
			return false;
	}

	// (2004.08.12, 배승원) 분석도구도 모두 Clear한다.
	RunToolFunction( CToolOptionInfo::T_DELETE_ALL, 3);
		
	// (2004.05.20, 배승원, 분석) Indicator List의 User정보를 Clear시키면,
	//		MainBlock이 관리하는 Indicator Info Pointer 정보도 같이 Clear시켜야 한다.
	//		그렇지 않으면 Pointer 오류가 발생된다.
	if(m_pIndicatorList == NULL)
		return false;
	
	// xScale 초기화 : xScaleManager - ojtaso (20070628)
	if(m_pxScaleManager)
		m_pxScaleManager->Initialize();

	m_pIndicatorList->RemoveAllIndicatorInfo( EILT_USER);

	//sy 2006.03.02
	//sy 2003.3.7 - 속도 개선
	//IndicatorInfo 에 의해서 MainBlock이 가지고 있는 info의 주소도 초기화
	if(m_pMainBlock != NULL) 
	{
		m_pMainBlock->Initialize_IndicatorInfo(true);
		// 블럭 최대화 추가 - ojtaso (20080519)
		m_pMainBlock->SetMaximizedBlock( CBlockIndex( -1, -1));
	}

	// 초기 상태를 불러와서 세팅한다
	//sy 2004.12.07. 특수&일반차트 저장과 관련된 부분.
	ResetSaveChartCfg( 0);
	//sy end

	// 2008.10.31 JS.Kim  사용자가 선택한 시간대 초기화
	m_pPacketListManager->ResetTimeDiff();

	// (2004.10.11, 배승원) AddIn DLL들에게 ResetChart 상황을 알려 처리되도록 한다.
	if( m_pAddInManager) m_pAddInManager->OnResetChart();

	NeoReturnOrgChart(m_nOnePageDataCount); //sy 2005.12.05

	return true;
}

void CKTBChartCtrl::ReplaceMATitle(CString &strma, const int nOption)
{
	if(nOption == 0 && strma.Find( CString( _MTEXT( C3_PRICE_MA_1)) + ";" + _MTEXT( C3_PRICE_MA_1)))
	{
		strma.Replace( CString( _MTEXT( C3_PRICE_MA_1)) + ";" + _MTEXT( C3_PRICE_MA_1), CString( _MTEXT( C3_PRICE_MA_1)) + ";5");
		strma.Replace( CString( _MTEXT( C3_PRICE_MA_2)) + ";" + _MTEXT( C3_PRICE_MA_2), CString( _MTEXT( C3_PRICE_MA_2)) + ";10");
		strma.Replace( CString( _MTEXT( C3_PRICE_MA_3)) + ";" + _MTEXT( C3_PRICE_MA_3), CString( _MTEXT( C3_PRICE_MA_3)) + ";20");
		strma.Replace( CString( _MTEXT( C3_PRICE_MA_4)) + ";" + _MTEXT( C3_PRICE_MA_4), CString( _MTEXT( C3_PRICE_MA_4)) + ";60");
		strma.Replace( CString( _MTEXT( C3_PRICE_MA_5)) + ";" + _MTEXT( C3_PRICE_MA_5), CString( _MTEXT( C3_PRICE_MA_5)) + ";120");
		strma.Replace( CString( _MTEXT( C3_PRICE_MA_6)) + ";" + _MTEXT( C3_PRICE_MA_6), CString( _MTEXT( C3_PRICE_MA_6)) + ";240");
	}
	else if(nOption == 1 && strma.Find( CString( _MTEXT( C3_VOLUME_MA_1)) + ";" + _MTEXT( C3_VOLUME_MA_1)))	{
		strma.Replace( CString( _MTEXT( C3_VOLUME_MA_1)) + ";" + _MTEXT( C3_VOLUME_MA_1), CString( _MTEXT( C3_VOLUME_MA_1)) + ";5");
		strma.Replace( CString( _MTEXT( C3_VOLUME_MA_2)) + ";" + _MTEXT( C3_VOLUME_MA_2), CString( _MTEXT( C3_VOLUME_MA_2)) + ";20");
		strma.Replace( CString( _MTEXT( C3_VOLUME_MA_3)) + ";" + _MTEXT( C3_VOLUME_MA_3), CString( _MTEXT( C3_VOLUME_MA_3)) + ";60");
		strma.Replace( CString( _MTEXT( C3_VOLUME_MA_4)) + ";" + _MTEXT( C3_VOLUME_MA_4), CString( _MTEXT( C3_VOLUME_MA_4)) + ";120");
	}
}
