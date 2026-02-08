// AnalystResultWnd.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include "AnalystResultWnd.h"

#include "../../DrCommon/Dr_Control/DrXLSFile.h"
//#include "../../ChartDll/Include_Chart/Dll_Load/IMetaTable.h"

#include "AnalystMainFrmWnd.h"
//#include "PortAnalystView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern HINSTANCE g_hInstance;
/////////////////////////////////////////////////////////////////////////////
// CAnalystResultWnd

CAnalystResultWnd::CAnalystResultWnd()
{
	LOGFONT	  logFont;
	GETFONT(logFont, 12, 400, m_fontDefault);
	
	m_clrResultSkin = COLOR_TAB_BACK;
	m_brush.CreateSolidBrush(m_clrResultSkin);
	
	m_nCodeCnt = 0;
	m_bDataSave = FALSE;
}

CAnalystResultWnd::~CAnalystResultWnd()
{
	m_fontDefault.DeleteObject();
	m_brush.DeleteObject();
}

BEGIN_MESSAGE_MAP(CAnalystResultWnd, CWnd)
//{{AFX_MSG_MAP(CAnalystResultWnd)
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_WM_SIZE()
	ON_WM_CTLCOLOR()
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BTN_TARGET_DAY, OnBtnTargetDay)
	ON_BN_CLICKED(IDC_BTN_TARGET_MIN, OnBtnTargetMin)

	ON_BN_CLICKED(IDC_BTNCHK_ALL, OnBtnChkAll)
	ON_BN_CLICKED(IDC_BTNCHK_KOSPI, OnBtnChkKospi)
	ON_BN_CLICKED(IDC_BTNCHK_KOSDAK, OnBtnChkKosdak)
	ON_BN_CLICKED(IDC_BTNCHK_KOSPIALL, OnBtnChkKospiAll)
	ON_BN_CLICKED(IDC_BTNCHK_KOSDAKALL, OnBtnChkKosdakAll)
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAnalystResultWnd message handlers
int CAnalystResultWnd::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	HINSTANCE hInstSave = AfxGetResourceHandle();
	AfxSetResourceHandle(g_hInstance);

	m_ctrlResultBar.Create(_T(""), WS_CHILD | WS_VISIBLE,// | WS_BORDER,
		CRect(0, 0, 0, 0), this, IDC_STATIC_RESULTBAR);
	
	m_btnTarget_Day.Create(_T("지정(일)"), WS_CHILD | WS_VISIBLE | BS_RADIOBUTTON, 
		CRect(0, 0, 0, 0), this, IDC_BTN_TARGET_DAY);
	m_btnTarget_Day.SetFont(&m_fontDefault);
	
	m_btnTarget_Min.Create(_T("지정(분)"), WS_CHILD | WS_VISIBLE | BS_RADIOBUTTON, 
		CRect(0, 0, 0, 0), this, IDC_BTN_TARGET_MIN);
	m_btnTarget_Min.SetFont(&m_fontDefault);
	
	//m_btnTarget_Min.EnableWindow(FALSE);
	
	m_btnChk_All.Create(_T("전체"), WS_CHILD | BS_AUTOCHECKBOX, 
		CRect(0, 0, 0, 0), this, IDC_BTNCHK_ALL);
	m_btnChk_All.SetFont(&m_fontDefault);
	
	m_btnChk_Kospi.Create(_T("거래소"), WS_CHILD | BS_AUTOCHECKBOX, 
		CRect(0, 0, 0, 0), this, IDC_BTNCHK_KOSPI);
	m_btnChk_Kospi.SetFont(&m_fontDefault);
	
	m_btnChk_Kosdaq.Create(_T("코스닥"), WS_CHILD | BS_AUTOCHECKBOX, 
		CRect(0, 0, 0, 0), this, IDC_BTNCHK_KOSDAK);
	m_btnChk_Kosdaq.SetFont(&m_fontDefault);
	
	m_btnChk_KospiAll.Create(_T("거래소지수"), WS_CHILD | BS_AUTOCHECKBOX, 
		CRect(0, 0, 0, 0), this, IDC_BTNCHK_KOSPIALL);
	m_btnChk_KospiAll.SetFont(&m_fontDefault);
	
	m_btnChk_KosdaqAll.Create(_T("코스닥지수"), WS_CHILD | BS_AUTOCHECKBOX, 
		CRect(0, 0, 0, 0), this, IDC_BTNCHK_KOSDAKALL);
	m_btnChk_KosdaqAll.SetFont(&m_fontDefault);
	
	m_chartProfit.Create(_T("Profit Chart"), WS_CHILD | WS_VISIBLE, 
		CRect(0, 0, 0, 0), this, IDC_CHART_TARGETRESULT);
	m_chartProfit.SetInit();
	
	m_chartProfit.SetAdd_Series(0, "기준선", RGB(0, 0, 0));
	m_chartProfit.SetAdd_Series(1, "전체", RGB(255, 0, 0));
	m_chartProfit.SetAdd_Series(2, "거래소", RGB(255, 128, 0));
	m_chartProfit.SetAdd_Series(3, "코스닥", RGB(0, 128, 0));
	m_chartProfit.SetAdd_Series(4, "거래소지수", RGB(0, 0, 255));
	m_chartProfit.SetAdd_Series(5, "코스닥지수", RGB(0, 0, 160));
	m_chartProfit.SetAdd_Series(6, "선택종목", RGB(128, 128, 128));
	
	m_chartProfit.SetSeriesShow(6, FALSE);
	
	m_gridProfit.Create(NULL, _T(""), WS_CHILD | WS_VISIBLE | WS_BORDER, CRect(0, 0, 0, 0), this, NULL);
	m_gridProfit.SetFont(&m_fontDefault);

	AfxSetResourceHandle(hInstSave);
	
	SetInit();
	
	return 0;
}

void CAnalystResultWnd::SetInit()
{
	SetSelectType(0);
	
	m_btnTarget_Day.SetCheck(TRUE);
	
	m_gridProfit.SetInit("PROFIT");
	m_gridProfit.SetHideCol(1, m_btnTarget_Day.GetCheck());
	
	m_chartProfit.SetDraw_Chart();
	
	m_btnChk_All.SetCheck(TRUE);
	m_btnChk_Kospi.SetCheck(TRUE);
	m_btnChk_Kosdaq.SetCheck(TRUE);
	m_btnChk_KospiAll.SetCheck(TRUE);
	m_btnChk_KosdaqAll.SetCheck(TRUE);
	
	GETPATH_APP(strFilePath);
//	strFilePath += "\\data\\PortAnalyst.ini";
	strFilePath = strFilePath + FOLDER_DATAROOT + "\\PortAnalyst.ini";
	GetLoadIniInfo(strFilePath);
}

void CAnalystResultWnd::SetInit_Val()
{
	m_gridProfit.SetHideCol(1, m_btnTarget_Day.GetCheck());
	
	// 2012.2.14 박성경: 50개 이상검색후 그리드 클릭시 종목 차트 안그려지는 오류 수정
	if(m_listAnalystInfo.GetCount() > 0)
	{
		m_SublistAnalystInfo.RemoveAll();

		AnalystInfo stAnalystInfo;
		for(int index = 0; index < m_listAnalystInfo.GetCount(); index++)
		{
			POSITION pos = m_listAnalystInfo.FindIndex(index);
			stAnalystInfo = m_listAnalystInfo.GetAt(pos);
			m_SublistAnalystInfo.AddTail(stAnalystInfo);
		}
	}
	m_listAnalystInfo.RemoveAll();
}

void CAnalystResultWnd::OnSize(UINT nType, int cx, int cy) 
{
	CWnd::OnSize(nType, cx, cy);
	
	CRect reClient;
	GetClientRect(reClient);
	
	OnSize(reClient);
}

void CAnalystResultWnd::OnSize(CRect reClient)
{
	CRect reTop, reTemp;
	
	reTemp = reClient;
	reTemp.top += 20;
	reTemp.bottom -= 1;
	reTemp.left += 2;
	reTemp.right -= 1;
	//m_ctrlResultBar.MoveWindow(&reTemp);
	
	
	/*
	reClient.top += 21;
	reClient.bottom -= 1;
	reClient.left += 1;
	reClient.right -= 1;
	*/
	
	reTop = reClient;
	reTop.bottom = reTop.top + 18;
	reTemp = reTop;
	
	reTemp.left += 2;
	reTemp.right = reTemp.left + 75;
	m_btnTarget_Day.MoveWindow(&reTemp);
	
	reTemp.left = reTemp.right + 2;
	reTemp.right = reTemp.left + 75;
	m_btnTarget_Min.MoveWindow(&reTemp);
	
	reTemp = reTop;
	
	reTemp.right -= 2;
	reTemp.left = reTemp.right - 80;
	m_btnChk_KosdaqAll.MoveWindow(&reTemp);
	
	reTemp.right = reTemp.left - 2;
	reTemp.left = reTemp.right - 80;
	m_btnChk_KospiAll.MoveWindow(&reTemp);
	
	reTemp.right = reTemp.left - 2;
	reTemp.left = reTemp.right - 60;
	m_btnChk_Kosdaq.MoveWindow(&reTemp);
	
	reTemp.right = reTemp.left - 2;
	reTemp.left = reTemp.right - 60;
	m_btnChk_Kospi.MoveWindow(&reTemp);
	
	reTemp.right = reTemp.left - 2;
	reTemp.left = reTemp.right - 45;
	m_btnChk_All.MoveWindow(&reTemp);
	
	
	reTop.top = reTop.bottom + 1;
	reTop.bottom = reClient.bottom - 3;
	reTop.left += 5;
	reTop.right -= 5;	
	m_chartProfit.MoveWindow(&reTop);
	m_gridProfit.MoveWindow(&reTop);	
}

void CAnalystResultWnd::OnDestroy() 
{
	CWnd::OnDestroy();
}

void CAnalystResultWnd::SetSelectType(int nType)
{
	m_btnChk_All.ShowWindow((nType == 0) ? SW_SHOW : SW_HIDE);
	m_btnChk_Kospi.ShowWindow((nType == 0) ? SW_SHOW : SW_HIDE);
	m_btnChk_Kosdaq.ShowWindow((nType == 0) ? SW_SHOW : SW_HIDE);
	m_btnChk_KospiAll.ShowWindow((nType == 0) ? SW_SHOW : SW_HIDE);
	m_btnChk_KosdaqAll.ShowWindow((nType == 0) ? SW_SHOW : SW_HIDE);
	
	m_gridProfit.ShowWindow((nType == 1) ? SW_SHOW : SW_HIDE);

	m_chartProfit.ShowWindow((nType == 0) ? SW_SHOW : SW_HIDE);
	
	Invalidate();
}

HBRUSH CAnalystResultWnd::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CWnd::OnCtlColor(pDC, pWnd, nCtlColor);
	
	if (nCtlColor == CTLCOLOR_STATIC)
	{
		CAnalystMainFrmWnd *pwndAnalystMainFrm = (CAnalystMainFrmWnd*)GetParent();

		pDC->SetBkMode(TRANSPARENT);
		pDC->SetBkColor(m_clrResultSkin);
		hbr = pwndAnalystMainFrm->GetAxBrush(pwndAnalystMainFrm->GetAxColor(66));
	}
	
	return hbr;
}

void CAnalystResultWnd::OnBtnTargetDay()
{
	m_btnTarget_Day.SetCheck(TRUE);
	m_btnTarget_Min.SetCheck(FALSE);
	
	CAnalystMainFrmWnd *pwndAnalystMainFrm = (CAnalystMainFrmWnd*)GetParent();
	pwndAnalystMainFrm->m_ctrlTargetCombo_Time.EnableWindow(FALSE);
	
	pwndAnalystMainFrm->SetSendTr_Code();
}

void CAnalystResultWnd::OnBtnTargetMin()
{
	m_btnTarget_Day.SetCheck(FALSE);
	m_btnTarget_Min.SetCheck(TRUE);
	
	CAnalystMainFrmWnd *pwndAnalystMainFrm = (CAnalystMainFrmWnd*)GetParent();
	pwndAnalystMainFrm->m_ctrlTargetCombo_Time.EnableWindow(TRUE);
	
	pwndAnalystMainFrm->SetSendTr_Code();
}

void CAnalystResultWnd::OnBtnChkAll()
{
	BOOL bShow = m_btnChk_All.GetCheck();
	
	m_chartProfit.SetSeriesShow(1, bShow);
	m_chartProfit.Invalidate();
}

void CAnalystResultWnd::OnBtnChkKospi()
{
	BOOL bShow = m_btnChk_Kospi.GetCheck();
	
	m_chartProfit.SetSeriesShow(2, bShow);
	m_chartProfit.Invalidate();
}

void CAnalystResultWnd::OnBtnChkKosdak()
{
	BOOL bShow = m_btnChk_Kosdaq.GetCheck();
	
	m_chartProfit.SetSeriesShow(3, bShow);
	m_chartProfit.Invalidate();
}

void CAnalystResultWnd::OnBtnChkKospiAll()
{
	BOOL bShow = m_btnChk_KospiAll.GetCheck();
	
	m_chartProfit.SetSeriesShow(4, bShow);
	m_chartProfit.Invalidate();
}

void CAnalystResultWnd::OnBtnChkKosdakAll()
{
	BOOL bShow = m_btnChk_KosdaqAll.GetCheck();
	
	m_chartProfit.SetSeriesShow(5, bShow);
	m_chartProfit.Invalidate();
}

void CAnalystResultWnd::SetData(GroupToBase_Out *pstGroupToBase_Out)
{
	m_nCodeCnt = 0;
	
	AnalystInfo	stAnalystInfo;
	CString strData;
	char *pchData;
	
	STR_CONVERT(pchData, pstGroupToBase_Out->sCnt, sizeof(pstGroupToBase_Out->sCnt), strData);
	m_nCodeCnt = atoi(strData);
	
	STR_CONVERT(pchData, pstGroupToBase_Out->sDataCnt, sizeof(pstGroupToBase_Out->sDataCnt), strData);
	int nDataCnt = atoi(strData);
	
	int nIndex, nIndex_Code;
	for (nIndex = 0; nIndex < nDataCnt; nIndex++)
	{
		STR_CONVERT(pchData, pstGroupToBase_Out->sDate[nIndex], sizeof(pstGroupToBase_Out->sDate[nIndex]), strData);
		stAnalystInfo.strDate = strData;
		
		if (strData.IsEmpty() == TRUE)
			break;
		
		STR_CONVERT(pchData, pstGroupToBase_Out->sTime[nIndex], sizeof(pstGroupToBase_Out->sTime[nIndex]), strData);
		stAnalystInfo.strTime = strData;
		
		memset(&stAnalystInfo.nCloseData, 0x00, sizeof(stAnalystInfo.nCloseData));
		for (nIndex_Code = 0; nIndex_Code < m_nCodeCnt; nIndex_Code++)
		{
			STR_CONVERT(pchData, pstGroupToBase_Out->stPrice[nIndex_Code][nIndex].sPrice, sizeof(pstGroupToBase_Out->stPrice[nIndex_Code][nIndex]), strData);
			stAnalystInfo.nCloseData[nIndex_Code] = atoi(strData);
			
			STR_CONVERT(pchData, pstGroupToBase_Out->stBase[nIndex_Code].sName, sizeof(pstGroupToBase_Out->stBase[nIndex_Code].sName), strData);
			stAnalystInfo.strName[nIndex_Code] = strData;
		}
		
		stAnalystInfo.dCloseData_Kosdaq = 0L;
		stAnalystInfo.dCloseData_Kospi = 0L;
 		
		stAnalystInfo.dParallel = 0L;
		stAnalystInfo.dAccumulate = 0L;
		stAnalystInfo.nUpCnt = 0;
		stAnalystInfo.nSteadyCnt = 0;
		stAnalystInfo.nDownCnt = 0;
		
		stAnalystInfo.dParallel_Kospi = 0L;
		stAnalystInfo.dParallel_Kosdaq = 0L;
		stAnalystInfo.dParallel_Kospi_All = 0L;
		stAnalystInfo.dParallel_Kosdaq_All = 0L;
		
		m_listAnalystInfo.AddTail(stAnalystInfo);
	}
}


void CAnalystResultWnd::SetData(GroupToBase_UPOut *pstGroupToBase_UPOut)
{
	if (m_listAnalystInfo.GetCount() <= 0)
		return;
	
	char *pchData;
	CString strData;
	
	int nUpCodeCnt, nUpCode_Index;
	STR_CONVERT(pchData, pstGroupToBase_UPOut->sCnt, sizeof(pstGroupToBase_UPOut->sCnt), strData);
	nUpCodeCnt = atoi(strData);
	
	POSITION pos;
	AnalystInfo	stAnalystInfo;
	for (int nIndex = 0; nIndex < (int)m_listAnalystInfo.GetCount(); nIndex++)
	{
		pos = m_listAnalystInfo.FindIndex(nIndex);
		stAnalystInfo = m_listAnalystInfo.GetAt(pos);
		
		for (nUpCode_Index = 0; nUpCode_Index < nUpCodeCnt; nUpCode_Index++)
		{
			STR_CONVERT(pchData, pstGroupToBase_UPOut->stPrice[nUpCode_Index][nIndex].sPrice, sizeof(pstGroupToBase_UPOut->stPrice[nUpCode_Index][nIndex]), strData);

			if (nUpCode_Index == 0)
				stAnalystInfo.dCloseData_Kospi = atof(strData);
			else
				stAnalystInfo.dCloseData_Kosdaq = atof(strData);
		}
		
		m_listAnalystInfo.SetAt(pos, stAnalystInfo);
	}
}


void CAnalystResultWnd::SetDataCalculation(CUIntArray *psiUserCodeRow)
{
	int nIndex_Data, nIndex_Code;
	double dData1, dData2;
	
	POSITION pos, pos_Befor;
	AnalystInfo	stAnalystInfo, stAnalystInfo_Befor;

	CAnalystMainFrmWnd *pwndAnalystMainFrm = (CAnalystMainFrmWnd*)GetParent();
	
	pwndAnalystMainFrm->SetWaitCursor(TRUE);
	pwndAnalystMainFrm->SetSendMainMsg("데이터 분석중...");
	pwndAnalystMainFrm->SetPassResult(0);
	
	int nKospiCnt, nKosdaqCnt;
	pwndAnalystMainFrm->m_gridCode.GetMarketTypeCnt(nKospiCnt, nKosdaqCnt);
	if (pwndAnalystMainFrm->m_nAnalystData_TargetType == 1)
		nKosdaqCnt = 0;
	else if (pwndAnalystMainFrm->m_nAnalystData_TargetType == 2)
		nKospiCnt = 0; 
	
	int nDataCnt = m_listAnalystInfo.GetCount();
	//int *pCloseData = new int[nKospiCnt + nKosdaqCnt]; //첫번째 기준이 될 데이터 저장.
	//int *pCloseData = (int*)malloc( sizeof(int)*(nKospiCnt+nKosdaqCnt)); //첫번째 기준이 될 데이터 저장.
	int pCloseData[4000];

	//for (nIndex_Data = 0; nIndex_Data < nDataCnt; nIndex_Data++)
	for (nIndex_Data = 0; nIndex_Data < nKospiCnt+nKosdaqCnt; nIndex_Data++)
		pCloseData[nIndex_Data] = 0;

	for (nIndex_Data = 0; nIndex_Data < nDataCnt; nIndex_Data++)
	{
		pos = m_listAnalystInfo.FindIndex(nIndex_Data);
		
		if (nIndex_Data == 0)
		{
			continue;
		}
		else
		{
			stAnalystInfo = m_listAnalystInfo.GetAt(pos);

			pos_Befor = m_listAnalystInfo.FindIndex(nIndex_Data - 1);
			stAnalystInfo_Befor = m_listAnalystInfo.GetAt(pos_Befor);
		}

		int nType, nCloseData1, nCloseData2; // nCloseData1 기존누적, nCloseData2, 수정누적.
		CString strData;
		double dParallel = 0.00;
		for (nIndex_Code = 0; nIndex_Code < (nKospiCnt + nKosdaqCnt); nIndex_Code++)
		{
			if(pCloseData[nIndex_Code]==0)
				pCloseData[nIndex_Code] = stAnalystInfo_Befor.nCloseData[nIndex_Code];

			nCloseData1 = stAnalystInfo.nCloseData[nIndex_Code] - stAnalystInfo_Befor.nCloseData[nIndex_Code];
			nCloseData2 = stAnalystInfo.nCloseData[nIndex_Code] - pCloseData[nIndex_Code];

			if (stAnalystInfo_Befor.nCloseData[nIndex_Code] == 0)
			{
//			if (pCloseData[nIndex_Code] == 0)
				dData1 = 0;
				dData2 = 0;
			}
			else 
			{
				dData1 = nCloseData1 / (double)stAnalystInfo_Befor.nCloseData[nIndex_Code] * 100;
				dData2 = nCloseData2 / (double)pCloseData[nIndex_Code] * 100;
			}

			if (nCloseData1 > 0)
				stAnalystInfo.nUpCnt++;
			else if (nCloseData1 < 0)
				stAnalystInfo.nDownCnt++;
			else
				stAnalystInfo.nSteadyCnt++;

			stAnalystInfo.dParallel += dData1;
			dParallel += dData2;
			nType = pwndAnalystMainFrm->m_gridCode.GetRowToMarketType(psiUserCodeRow->GetAt(nIndex_Code));

//			if (nType == 1)		//# 거래소
//				stAnalystInfo.dParallel_Kospi += dData2;
//			else if (nType == 2)	//# 코스닥
//				stAnalystInfo.dParallel_Kosdaq += dData2;
			if (nType == 1)		//# 거래소
				stAnalystInfo.dParallel_Kospi += dData2;
			else if (nType == 2)	//# 코스닥
				stAnalystInfo.dParallel_Kosdaq += dData2;
		}

		strData.Format("%.4f", stAnalystInfo.dParallel);
		stAnalystInfo.dParallel = atof(strData);

		strData.Format("%.4f", dParallel);
		dParallel = atof(strData);

		stAnalystInfo.dParallel = stAnalystInfo.dParallel / (nKospiCnt + nKosdaqCnt);
		dParallel = dParallel / (nKospiCnt + nKosdaqCnt);
		//stAnalystInfo.dAccumulate = stAnalystInfo_Befor.dAccumulate + stAnalystInfo.dParallel;
		//수정후..  결과자체가 누적이므로 그냥 대입만하면 됨.
		//stAnalystInfo.dAccumulate = stAnalystInfo.dParallel;
		stAnalystInfo.dAccumulate = dParallel;

		if (nKospiCnt > 0)
			stAnalystInfo.dParallel_Kospi = stAnalystInfo.dParallel_Kospi / nKospiCnt;
		else
			stAnalystInfo.dParallel_Kospi = 0;

		if (nKosdaqCnt > 0)
			stAnalystInfo.dParallel_Kosdaq = stAnalystInfo.dParallel_Kosdaq / nKosdaqCnt;
		else
			stAnalystInfo.dParallel_Kosdaq = 0;
		
		stAnalystInfo.dParallel_Kospi_All = (stAnalystInfo.dCloseData_Kospi - stAnalystInfo_Befor.dCloseData_Kospi) / (double)stAnalystInfo_Befor.dCloseData_Kospi * 100;
		stAnalystInfo.dParallel_Kosdaq_All = (stAnalystInfo.dCloseData_Kosdaq - stAnalystInfo_Befor.dCloseData_Kosdaq) / (double)stAnalystInfo_Befor.dCloseData_Kosdaq * 100;
		
		m_listAnalystInfo.SetAt(pos, stAnalystInfo);
	}
	//if(pCloseData) free(pCloseData);

	m_gridProfit.ClearAll();
	m_chartProfit.SetClearAll();
	
	double dAccumulate_Kospi = 0;
	double dAccumulate_Kosdaq = 0;
	double dAccumulate_Kospi_All = 0;
	double dAccumulate_Kosdaq_All = 0;
	
	if (m_btnTarget_Day.GetCheck() == TRUE)
		m_chartProfit.m_nXMask_OffSet = 50;
	else if (m_btnTarget_Min.GetCheck() == TRUE)
		m_chartProfit.m_nXMask_OffSet = 70;
	
	int nRunVal;
	CString	strData, strCalculationTerm;
	strCalculationTerm = "분석기간:";
	for (nIndex_Data = 0; nIndex_Data < nDataCnt; nIndex_Data++)
	{
		pos = m_listAnalystInfo.FindIndex(nIndex_Data);
		stAnalystInfo = m_listAnalystInfo.GetAt(pos);

		if (nIndex_Data == 0)
		{
			if (m_btnTarget_Day.GetCheck() == TRUE)
				strData.Format("%s/%s/%s", stAnalystInfo.strDate.Left(4), stAnalystInfo.strDate.Mid(4, 2), stAnalystInfo.strDate.Right(2));
			else if (m_btnTarget_Min.GetCheck() == TRUE)
				strData.Format("%s/%s/%s %s:%s", stAnalystInfo.strDate.Left(4), stAnalystInfo.strDate.Mid(4, 2), stAnalystInfo.strDate.Right(2), 
				stAnalystInfo.strTime.Left(2), stAnalystInfo.strTime.Right(2));
			
			CTime timeServerLast(atoi(stAnalystInfo.strDate.Left(4)), atoi(stAnalystInfo.strDate.Mid(4, 2)), atoi(stAnalystInfo.strDate.Right(2)));
			// 2011.11.10 박성경 >>
			//CPortAnalystView* pView = (CPortAnalystView*)GetParent()->GetParent();
			//CTime timeSetUser(atoi(pView->m_ctrlCalendar.GetYear()), atoi(pView->m_ctrlCalendar.GetMonth()), atoi(pView->m_ctrlCalendar.GetDay()));

// 			if(timeSetUser < timeServerLast)
// 			{
// 				CString strMsg;
// 				strMsg.Format("과거 데이터 %s년 %s월 %s일부터 조회가능합니다.", stAnalystInfo.strDate.Left(4), stAnalystInfo.strDate.Mid(4, 2), stAnalystInfo.strDate.Right(2));
// 				g_iMetaTable.ShowMessageBox(m_hWnd, strMsg);
// 				pView->m_ctrlCalendar.SetDate(stAnalystInfo.strDate.Left(4), stAnalystInfo.strDate.Mid(4, 2), stAnalystInfo.strDate.Right(2));
//			}
			
			CAnalystMainFrmWnd *pwndAnalystMainFrm = (CAnalystMainFrmWnd*)GetParent();
// 			CTime timeSetUser(atoi(pwndAnalystMainFrm->m_ctrlTarget_Data.GetYear()), atoi(pwndAnalystMainFrm->m_ctrlTarget_Data.GetMonth()), 
// 								atoi(pwndAnalystMainFrm->m_ctrlTarget_Data.GetDay()));
			CTime timeTemp;
			pwndAnalystMainFrm->m_ctrlTarget_Data.GetTime(timeTemp);
			CString strTemp;
			strTemp.Format("%04d%02d%02d", timeTemp.GetYear(), timeTemp.GetMonth(), timeTemp.GetDay());
			CTime timeSetUser(atoi(strTemp.Left(4)), atoi(strTemp.Mid(4, 2)), atoi(strTemp.Right(2)));

			if(timeSetUser < timeServerLast)
			{
				CString strMsg;
				strMsg.Format("과거 데이터 %s년 %s월 %s일부터 조회가능합니다.", stAnalystInfo.strDate.Left(4), stAnalystInfo.strDate.Mid(4, 2), stAnalystInfo.strDate.Right(2));
				SetMessageBox(strMsg, PROGRAM_TITLE, MB_YESNO | MB_ICONQUESTION);
				
				int nYear = atoi(stAnalystInfo.strDate.Left(4));
				int nMon  = atoi(stAnalystInfo.strDate.Mid(4, 2));
				int nDay  = atoi(stAnalystInfo.strDate.Right(2));
				
				CTime time(nYear, nMon, nDay, 0, 0, 0);
				pwndAnalystMainFrm->m_ctrlTarget_Data.SetTime(&time);
			}
			// 2011.11.10 박성경 <<
				

			strCalculationTerm += strData;
		}
		else if (nIndex_Data == nDataCnt - 1)
		{
			if (m_btnTarget_Day.GetCheck() == TRUE)
				strData.Format("%s/%s/%s", stAnalystInfo.strDate.Left(4), stAnalystInfo.strDate.Mid(4, 2), stAnalystInfo.strDate.Right(2));
			else if (m_btnTarget_Min.GetCheck() == TRUE)
				strData.Format("%s/%s/%s %s:%s", stAnalystInfo.strDate.Left(4), stAnalystInfo.strDate.Mid(4, 2), stAnalystInfo.strDate.Right(2), 
				stAnalystInfo.strTime.Left(2), stAnalystInfo.strTime.Right(2));
			
			strCalculationTerm += " ~ " + strData;
		}

		m_gridProfit.SetAddRow(stAnalystInfo);
		if (m_btnTarget_Day.GetCheck() == TRUE)
			strData.Format("%s/%s", stAnalystInfo.strDate.Mid(4, 2), stAnalystInfo.strDate.Right(2));
		else if (m_btnTarget_Min.GetCheck() == TRUE)
			strData.Format("%s %s:%s", stAnalystInfo.strDate.Right(2), 
		stAnalystInfo.strTime.Left(2), stAnalystInfo.strTime.Right(2));
		m_chartProfit.SetAdd_XData(strData, atof(strData));
		
		m_chartProfit.SetAdd_YData(0, nIndex_Data, 0);
		m_chartProfit.SetAdd_YData(1, nIndex_Data, stAnalystInfo.dAccumulate);
		
		//dAccumulate_Kospi += stAnalystInfo.dParallel_Kospi;
		dAccumulate_Kospi = stAnalystInfo.dParallel_Kospi;
		if (nKospiCnt == 0)
			m_chartProfit.SetAdd_YData(2, nIndex_Data, 0);
		else
			m_chartProfit.SetAdd_YData(2, nIndex_Data, dAccumulate_Kospi);
		
		//dAccumulate_Kosdaq += stAnalystInfo.dParallel_Kosdaq;
		dAccumulate_Kosdaq = stAnalystInfo.dParallel_Kosdaq;
		if (nKosdaqCnt == 0)
			m_chartProfit.SetAdd_YData(3, nIndex_Data, 0);
		else
			m_chartProfit.SetAdd_YData(3, nIndex_Data, dAccumulate_Kosdaq);
		
		dAccumulate_Kospi_All += stAnalystInfo.dParallel_Kospi_All; // 수정전.
		//dAccumulate_Kospi_All = stAnalystInfo.dParallel_Kospi_All;
		m_chartProfit.SetAdd_YData(4, nIndex_Data, dAccumulate_Kospi_All);
		
		dAccumulate_Kosdaq_All += stAnalystInfo.dParallel_Kosdaq_All; //수정전.
		//dAccumulate_Kosdaq_All = stAnalystInfo.dParallel_Kosdaq_All;
		m_chartProfit.SetAdd_YData(5, nIndex_Data, dAccumulate_Kosdaq_All);
		
		nRunVal = ((nIndex_Data + 1) * 100 / nDataCnt);
		pwndAnalystMainFrm->SetPassResult(nRunVal);
	}
	
	BOOL bShow;
	bShow = m_btnChk_All.GetCheck();
	m_chartProfit.SetSeriesShow(1, bShow);
	
	bShow = m_btnChk_Kospi.GetCheck();
	m_chartProfit.SetSeriesShow(2, bShow);
	
	bShow = m_btnChk_Kosdaq.GetCheck();
	m_chartProfit.SetSeriesShow(3, bShow);
	
	bShow = m_btnChk_KospiAll.GetCheck();
	m_chartProfit.SetSeriesShow(4, bShow);
	
	bShow = m_btnChk_KosdaqAll.GetCheck();
	m_chartProfit.SetSeriesShow(5, bShow);
	
	m_chartProfit.SetSeriesShow(6, FALSE);
	
	m_chartProfit.SetDraw_Chart();
	m_gridProfit.SetSelectRow(0);
	
	SetDataCalculation_Target(psiUserCodeRow);
	pwndAnalystMainFrm->m_ctrlCalculationTerm.SetWindowText(strCalculationTerm);
	
	pwndAnalystMainFrm->SetSendMainMsg("데이터 분석 완료.");
	
	if (m_bDataSave == TRUE)
	{
		GETPATH_APP(strFilePath);
	//	strFilePath += "\\data\\PortAnalyst.xls";
		strFilePath = strFilePath + FOLDER_DATAROOT + "\\PortAnalyst.xls";
		SetSaveGridToExcel(strFilePath);
	}

	pwndAnalystMainFrm->SetWaitCursor(FALSE);
}

/*
void CAnalystResultWnd::SetDataCalculation(CUIntArray *psiUserCodeRow)
{
	int nIndex_Data, nIndex_Code;
	double dData;
	
	POSITION pos, pos_Befor;
	AnalystInfo	stAnalystInfo, stAnalystInfo_Befor;
	
	CAnalystMainFrmWnd *pwndAnalystMainFrm = (CAnalystMainFrmWnd*)GetParent();
	
	pwndAnalystMainFrm->SetWaitCursor(TRUE);
	pwndAnalystMainFrm->SetSendMainMsg("데이터 분석중...");
	pwndAnalystMainFrm->SetPassResult(0);
	
	int nKospiCnt, nKosdaqCnt;
	pwndAnalystMainFrm->m_gridCode.GetMarketTypeCnt(nKospiCnt, nKosdaqCnt);
	if (pwndAnalystMainFrm->m_nAnalystData_TargetType == 1)
		nKosdaqCnt = 0;
	else if (pwndAnalystMainFrm->m_nAnalystData_TargetType == 2)
		nKospiCnt = 0; 
	
	int nDataCnt = m_listAnalystInfo.GetCount();
	for (nIndex_Data = 0; nIndex_Data < nDataCnt; nIndex_Data++)
	{
		pos = m_listAnalystInfo.FindIndex(nIndex_Data);
		
		if (nIndex_Data == 0)
			continue;
		else
		{
			stAnalystInfo = m_listAnalystInfo.GetAt(pos);
			
			pos_Befor = m_listAnalystInfo.FindIndex(nIndex_Data - 1);
			stAnalystInfo_Befor = m_listAnalystInfo.GetAt(pos_Befor);
		}
		
		int nType, nCloseData;
		CString strData;
		for (nIndex_Code = 0; nIndex_Code < (nKospiCnt + nKosdaqCnt); nIndex_Code++)
		{
			nCloseData = stAnalystInfo.nCloseData[nIndex_Code] - stAnalystInfo_Befor.nCloseData[nIndex_Code];
			
			if (stAnalystInfo_Befor.nCloseData[nIndex_Code] == 0)
				dData = 0;
			else
				dData = nCloseData / (double)stAnalystInfo_Befor.nCloseData[nIndex_Code] * 100;
			
			if (nCloseData > 0)
				stAnalystInfo.nUpCnt++;
			else if (nCloseData < 0)
				stAnalystInfo.nDownCnt++;
			else
				stAnalystInfo.nSteadyCnt++;
			
			stAnalystInfo.dParallel += dData;
			nType = pwndAnalystMainFrm->m_gridCode.GetRowToMarketType(psiUserCodeRow->GetAt(nIndex_Code));
			if (nType == 1)		//# 거래소
				stAnalystInfo.dParallel_Kospi += dData;
			else if (nType == 2)	//# 코스닥
				stAnalystInfo.dParallel_Kosdaq += dData;
		}
		
		strData.Format("%.4f", stAnalystInfo.dParallel);
		stAnalystInfo.dParallel = atof(strData);
		
		stAnalystInfo.dParallel = stAnalystInfo.dParallel / (nKospiCnt + nKosdaqCnt);
		stAnalystInfo.dAccumulate = stAnalystInfo_Befor.dAccumulate + stAnalystInfo.dParallel;
		
		if (nKospiCnt > 0)
			stAnalystInfo.dParallel_Kospi = stAnalystInfo.dParallel_Kospi / nKospiCnt;
		else
			stAnalystInfo.dParallel_Kospi = 0;
		
		if (nKosdaqCnt > 0)
			stAnalystInfo.dParallel_Kosdaq = stAnalystInfo.dParallel_Kosdaq / nKosdaqCnt;
		else
			stAnalystInfo.dParallel_Kosdaq = 0;
		
		stAnalystInfo.dParallel_Kospi_All = (stAnalystInfo.dCloseData_Kospi - stAnalystInfo_Befor.dCloseData_Kospi) / (double)stAnalystInfo_Befor.dCloseData_Kospi * 100;
		stAnalystInfo.dParallel_Kosdaq_All = (stAnalystInfo.dCloseData_Kosdaq - stAnalystInfo_Befor.dCloseData_Kosdaq) / (double)stAnalystInfo_Befor.dCloseData_Kosdaq * 100;
		
		m_listAnalystInfo.SetAt(pos, stAnalystInfo);
	}
	
	m_gridProfit.ClearAll();
	m_chartProfit.SetClearAll();
	
	double dAccumulate_Kospi = 0;
	double dAccumulate_Kosdaq = 0;
	double dAccumulate_Kospi_All = 0;
	double dAccumulate_Kosdaq_All = 0;
	
	if (m_btnTarget_Day.GetCheck() == TRUE)
		m_chartProfit.m_nXMask_OffSet = 50;
	else if (m_btnTarget_Min.GetCheck() == TRUE)
		m_chartProfit.m_nXMask_OffSet = 70;
	
	int nRunVal;
	CString	strData, strCalculationTerm;
	strCalculationTerm = "분석기간:";
	for (nIndex_Data = 0; nIndex_Data < nDataCnt; nIndex_Data++)
	{
		pos = m_listAnalystInfo.FindIndex(nIndex_Data);
		stAnalystInfo = m_listAnalystInfo.GetAt(pos);
		
		if (nIndex_Data == 0)
		{
			if (m_btnTarget_Day.GetCheck() == TRUE)
				strData.Format("%s/%s/%s", stAnalystInfo.strDate.Left(4), stAnalystInfo.strDate.Mid(4, 2), stAnalystInfo.strDate.Right(2));
			else if (m_btnTarget_Min.GetCheck() == TRUE)
				strData.Format("%s/%s/%s %s:%s", stAnalystInfo.strDate.Left(4), stAnalystInfo.strDate.Mid(4, 2), stAnalystInfo.strDate.Right(2), 
				stAnalystInfo.strTime.Left(2), stAnalystInfo.strTime.Right(2));
			
			strCalculationTerm += strData;
		}
		else if (nIndex_Data == nDataCnt - 1)
		{
			if (m_btnTarget_Day.GetCheck() == TRUE)
				strData.Format("%s/%s/%s", stAnalystInfo.strDate.Left(4), stAnalystInfo.strDate.Mid(4, 2), stAnalystInfo.strDate.Right(2));
			else if (m_btnTarget_Min.GetCheck() == TRUE)
				strData.Format("%s/%s/%s %s:%s", stAnalystInfo.strDate.Left(4), stAnalystInfo.strDate.Mid(4, 2), stAnalystInfo.strDate.Right(2), 
				stAnalystInfo.strTime.Left(2), stAnalystInfo.strTime.Right(2));
			
			strCalculationTerm += " ~ " + strData;
		}
		
		m_gridProfit.SetAddRow(stAnalystInfo);
		if (m_btnTarget_Day.GetCheck() == TRUE)
			strData.Format("%s/%s", stAnalystInfo.strDate.Mid(4, 2), stAnalystInfo.strDate.Right(2));
		else if (m_btnTarget_Min.GetCheck() == TRUE)
			strData.Format("%s %s:%s", stAnalystInfo.strDate.Right(2), 
			stAnalystInfo.strTime.Left(2), stAnalystInfo.strTime.Right(2));
		m_chartProfit.SetAdd_XData(strData, atof(strData));
		
		m_chartProfit.SetAdd_YData(0, nIndex_Data, 0);
		m_chartProfit.SetAdd_YData(1, nIndex_Data, stAnalystInfo.dAccumulate);
		
		dAccumulate_Kospi += stAnalystInfo.dParallel_Kospi;
		if (nKospiCnt == 0)
			m_chartProfit.SetAdd_YData(2, nIndex_Data, 0);
		else
			m_chartProfit.SetAdd_YData(2, nIndex_Data, dAccumulate_Kospi);
		
		dAccumulate_Kosdaq += stAnalystInfo.dParallel_Kosdaq;
		if (nKosdaqCnt == 0)
			m_chartProfit.SetAdd_YData(3, nIndex_Data, 0);
		else
			m_chartProfit.SetAdd_YData(3, nIndex_Data, dAccumulate_Kosdaq);
		
		dAccumulate_Kospi_All += stAnalystInfo.dParallel_Kospi_All;
		m_chartProfit.SetAdd_YData(4, nIndex_Data, dAccumulate_Kospi_All);
		
		dAccumulate_Kosdaq_All += stAnalystInfo.dParallel_Kosdaq_All;
		m_chartProfit.SetAdd_YData(5, nIndex_Data, dAccumulate_Kosdaq_All);
		
		nRunVal = ((nIndex_Data + 1) * 100 / nDataCnt);
		pwndAnalystMainFrm->SetPassResult(nRunVal);
	}
	
	BOOL bShow;
	bShow = m_btnChk_All.GetCheck();
	m_chartProfit.SetSeriesShow(1, bShow);
	
	bShow = m_btnChk_Kospi.GetCheck();
	m_chartProfit.SetSeriesShow(2, bShow);
	
	bShow = m_btnChk_Kosdaq.GetCheck();
	m_chartProfit.SetSeriesShow(3, bShow);
	
	bShow = m_btnChk_KospiAll.GetCheck();
	m_chartProfit.SetSeriesShow(4, bShow);
	
	bShow = m_btnChk_KosdaqAll.GetCheck();
	m_chartProfit.SetSeriesShow(5, bShow);
	
	m_chartProfit.SetSeriesShow(6, FALSE);
	
	m_chartProfit.SetDraw_Chart();
	m_gridProfit.SetSelectRow(0);
	
	SetDataCalculation_Target(psiUserCodeRow);
	pwndAnalystMainFrm->m_ctrlCalculationTerm.SetWindowText(strCalculationTerm);
	
	pwndAnalystMainFrm->SetSendMainMsg("데이터 분석 완료.");
	
	if (m_bDataSave == TRUE)
	{
		GETPATH_APP(strFilePath);
		strFilePath += "\\data\\PortAnalyst.xls";
		SetSaveGridToExcel(strFilePath);
	}
	
	pwndAnalystMainFrm->SetWaitCursor(FALSE);
}
*/

void CAnalystResultWnd::SetDataCalculation_Target(CUIntArray *psiUserCodeRow)
{
	int nIndex_Code;
	double dData;
	
	POSITION pos, pos_Befor;
	AnalystInfo	stAnalystInfo, stAnalystInfo_Befor;
	
	CAnalystMainFrmWnd *pwndAnalystMainFrm = (CAnalystMainFrmWnd*)GetParent();
	
	int nKospiCnt, nKosdaqCnt;
	pwndAnalystMainFrm->m_gridCode.GetMarketTypeCnt(nKospiCnt, nKosdaqCnt);
	if (pwndAnalystMainFrm->m_nAnalystData_TargetType == 1)
		nKosdaqCnt = 0;
	else if (pwndAnalystMainFrm->m_nAnalystData_TargetType == 2)
		nKospiCnt = 0; 
	
	int nDataCnt = m_listAnalystInfo.GetCount();
	pos = m_listAnalystInfo.FindIndex(nDataCnt - 1);
	stAnalystInfo = m_listAnalystInfo.GetAt(pos);
	
	pos_Befor = m_listAnalystInfo.FindIndex(0);
	stAnalystInfo_Befor = m_listAnalystInfo.GetAt(pos_Befor);
	
	stAnalystInfo.nUpCnt = 0;
	stAnalystInfo.nDownCnt = 0;
	stAnalystInfo.nSteadyCnt = 0;
	
	int nType, nCloseData;
	CString strData;
	for (nIndex_Code = 0; nIndex_Code < (nKospiCnt + nKosdaqCnt); nIndex_Code++)
	{
		nCloseData = stAnalystInfo.nCloseData[nIndex_Code] - stAnalystInfo_Befor.nCloseData[nIndex_Code];
		
		if (stAnalystInfo_Befor.nCloseData[nIndex_Code] == 0)
			dData = 0;
		else
			dData = nCloseData / (double)stAnalystInfo_Befor.nCloseData[nIndex_Code] * 100;
		
		if (nCloseData > 0)
			stAnalystInfo.nUpCnt++;
		else if (nCloseData < 0)
			stAnalystInfo.nDownCnt++;
		else
			stAnalystInfo.nSteadyCnt++;
		
		stAnalystInfo.dParallel += dData;
		nType = pwndAnalystMainFrm->m_gridCode.GetRowToMarketType(psiUserCodeRow->GetAt(nIndex_Code));
		if (nType == 1)		//# 거래소
			stAnalystInfo.dParallel_Kospi += dData;
		else if (nType == 2)	//# 코스닥
			stAnalystInfo.dParallel_Kosdaq += dData;
	}
	
	/*
	strData.Format("%.4f", stAnalystInfo.dParallel);
	stAnalystInfo.dParallel = atof(strData);
	
	  stAnalystInfo.dParallel = stAnalystInfo.dParallel / (nKospiCnt + nKosdaqCnt);
	  stAnalystInfo.dAccumulate = stAnalystInfo_Befor.dAccumulate + stAnalystInfo.dParallel;
	  
		if (nKospiCnt > 0)
		stAnalystInfo.dParallel_Kospi = stAnalystInfo.dParallel_Kospi / nKospiCnt;
		else
		stAnalystInfo.dParallel_Kospi = 0;
		
		  if (nKosdaqCnt > 0)
		  stAnalystInfo.dParallel_Kosdaq = stAnalystInfo.dParallel_Kosdaq / nKosdaqCnt;
		  else
		  stAnalystInfo.dParallel_Kosdaq = 0;
		  
			stAnalystInfo.dParallel_Kospi_All = (stAnalystInfo.dCloseData_Kospi - stAnalystInfo_Befor.dCloseData_Kospi) / (double)stAnalystInfo_Befor.dCloseData_Kospi * 100;
			stAnalystInfo.dParallel_Kosdaq_All = (stAnalystInfo.dCloseData_Kosdaq - stAnalystInfo_Befor.dCloseData_Kosdaq) / (double)stAnalystInfo_Befor.dCloseData_Kosdaq * 100;
	*/
	
	pwndAnalystMainFrm->m_gridResult.SetAddRow_Result(stAnalystInfo);
}

/*
void CAnalystResultWnd::SetDrewIndexToLine(CodeInfo stCodeInfo)
{
	AnalystInfo	stAnalystInfoFirst;

	BOOL bSelectIndex = FALSE;
	int nRowToIndex, nIndex_Data;
	CAnalystMainFrmWnd *pwndAnalystMainFrm = (CAnalystMainFrmWnd*)GetParent();
	for (nRowToIndex = 0; nRowToIndex < pwndAnalystMainFrm->m_siUserCodeRow.GetSize(); nRowToIndex++)
	{
		stAnalystInfoFirst.nCloseData[nRowToIndex] = 0;

		if (stCodeInfo.nRow == (int)pwndAnalystMainFrm->m_siUserCodeRow.GetAt(nRowToIndex))
		{
			bSelectIndex = TRUE;
			break;
		}
	}
	
	int nDataCnt = m_listAnalystInfo.GetCount();
	if (bSelectIndex == FALSE)
	{
		m_chartProfit.SetSeriesShow(6, FALSE);
		
		for (nIndex_Data = 0; nIndex_Data < nDataCnt; nIndex_Data++)
			m_gridProfit.SetModifyRow_CodeData(nIndex_Data, 100);
		m_gridProfit.RedrawAll();
		
		return;
	}
	
	m_chartProfit.SetSeriesShow(6, TRUE);
	m_chartProfit.SetClear_YData(6);
	m_chartProfit.SetModify_Series(6, stCodeInfo.strName);

	if (stCodeInfo.nRow > -1)
	{
		POSITION pos, pos_Befor; 
		AnalystInfo	stAnalystInfo, stAnalystInfo_Befor;
		BOOL bFirstChk = TRUE;
		
		double dData, dAccumulate;
		for (nIndex_Data = 0; nIndex_Data < nDataCnt; nIndex_Data++)
		{
			pos = m_listAnalystInfo.FindIndex(nIndex_Data);

			if (nIndex_Data == 0)
			{
				dData = 0;
				dAccumulate = 0;

				if(bFirstChk)
				{
					stAnalystInfoFirst = m_listAnalystInfo.GetAt(pos);

					if(stAnalystInfoFirst.nCloseData[nRowToIndex]!=0)
						bFirstChk = FALSE;
				}
			}
			else
			{
				stAnalystInfo = m_listAnalystInfo.GetAt(pos);
				if(bFirstChk)
				{
					stAnalystInfoFirst = m_listAnalystInfo.GetAt(pos);

					if(stAnalystInfoFirst.nCloseData[nRowToIndex]!=0)
						bFirstChk = FALSE;
				}

				pos_Befor = m_listAnalystInfo.FindIndex(nIndex_Data - 1);
				stAnalystInfo_Befor = m_listAnalystInfo.GetAt(pos_Befor);

				if (stAnalystInfo_Befor.nCloseData[nRowToIndex] == 0)
					dData = 0;
				else
					dData = (stAnalystInfo.nCloseData[nRowToIndex] - stAnalystInfo_Befor.nCloseData[nRowToIndex]) / (double)stAnalystInfo_Befor.nCloseData[nRowToIndex] * 100; //원래
//					dData = (stAnalystInfo.nCloseData[nRowToIndex] - stAnalystInfoFirst.nCloseData[nRowToIndex]) / (double)stAnalystInfoFirst.nCloseData[nRowToIndex] * 100; // 수정
				
				dAccumulate += dData;	// 원래버전
//				dAccumulate = dData;	// 수정버전.
			}
			m_chartProfit.SetAdd_YData(6, nIndex_Data, dAccumulate);
			m_gridProfit.SetModifyRow_CodeData(nIndex_Data, dData);
		}
	}
	else
	{
		for (nIndex_Data = 0; nIndex_Data < nDataCnt; nIndex_Data++)
			m_gridProfit.SetModifyRow_CodeData(nIndex_Data, 100);
	}

	m_chartProfit.SetDraw_Chart();
	m_gridProfit.RedrawAll();
}
*/

void CAnalystResultWnd::SetDrewIndexToLine(CodeInfo stCodeInfo)
{
	BOOL bSelectIndex = FALSE;
	int nRowToIndex, nIndex_Data;
	CAnalystMainFrmWnd *pwndAnalystMainFrm = (CAnalystMainFrmWnd*)GetParent();
	for (nRowToIndex = 0; nRowToIndex < pwndAnalystMainFrm->m_siUserCodeRow.GetSize(); nRowToIndex++)
	{
		if (stCodeInfo.nRow == (int)pwndAnalystMainFrm->m_siUserCodeRow.GetAt(nRowToIndex))
		{
			bSelectIndex = TRUE;
			break;
		}
	}
	
	int nDataCnt = m_listAnalystInfo.GetCount();
	if (bSelectIndex == FALSE)
	{
		m_chartProfit.SetSeriesShow(6, FALSE);
		
		for (nIndex_Data = 0; nIndex_Data < nDataCnt; nIndex_Data++)
			m_gridProfit.SetModifyRow_CodeData(nIndex_Data, 100);
		m_gridProfit.RedrawAll();
		
		return;
	}

	m_chartProfit.SetSeriesShow(6, TRUE);
	m_chartProfit.SetClear_YData(6);
	m_chartProfit.SetModify_Series(6, stCodeInfo.strName);

	if (stCodeInfo.nRow > -1)
	{
		POSITION pos, pos_Befor; 
		AnalystInfo	stAnalystInfo, stAnalystInfo_Befor;
		
		double dData, dAccumulate;
		for (nIndex_Data = 0; nIndex_Data < nDataCnt; nIndex_Data++)
		{
			pos = m_listAnalystInfo.FindIndex(nIndex_Data);
			
			if (nIndex_Data == 0)
			{
				dData = 0;
				dAccumulate = 0;
			}
			else
			{
				stAnalystInfo = m_listAnalystInfo.GetAt(pos);
				
				pos_Befor = m_listAnalystInfo.FindIndex(nIndex_Data - 1);
				stAnalystInfo_Befor = m_listAnalystInfo.GetAt(pos_Befor);
				
				if (stAnalystInfo_Befor.nCloseData[nRowToIndex] == 0)
					dData = 0;
				else
					dData = (stAnalystInfo.nCloseData[nRowToIndex] - stAnalystInfo_Befor.nCloseData[nRowToIndex]) / (double)stAnalystInfo_Befor.nCloseData[nRowToIndex] * 100;
				
				dAccumulate += dData;
			}
			m_chartProfit.SetAdd_YData(6, nIndex_Data, dAccumulate);
			m_gridProfit.SetModifyRow_CodeData(nIndex_Data, dData);
		}
	}
	else
	{
		for (nIndex_Data = 0; nIndex_Data < nDataCnt; nIndex_Data++)
			m_gridProfit.SetModifyRow_CodeData(nIndex_Data, 100);
	}

	m_chartProfit.SetDraw_Chart();
	m_gridProfit.RedrawAll();
}

void CAnalystResultWnd::SetResultSave()
{
	CFileFind FileFinder;

	BOOL bExist = FALSE;
	BOOL bSave = TRUE;
	
	int nReturn = 0;
	CString strMsg = "";
	CString strFileName, strFileType;
	CString strDefaultName = "임시저장";
	
	static char BASED_CODE szFilter[] = "WorkSheet Files (*.xls)|*.xls|EZ-Finder Data Files (*.txt)|*.txt||";
	CFileDialog saveDlg(FALSE, _T("xls"), strDefaultName + _T(".xls"), NULL, szFilter, NULL);
	
	while (bSave)
	{
		if (saveDlg.DoModal() == IDOK)
		{
			strFileType = saveDlg.GetFileExt();
			strFileName = saveDlg.GetPathName();
			
			bExist = FileFinder.FindFile(strFileName);
			if (bExist == TRUE)
			{
				strMsg.Format("%s 은(는) 이미 있습니다.\n바꾸시겠습니까?", strFileName);
				nReturn = SetMessageBox(strMsg, PROGRAM_TITLE, MB_YESNO | MB_ICONQUESTION);
				if (nReturn == IDYES)
				{
					if (!strFileName.IsEmpty())
					{
						if (!strFileType.CompareNoCase("xls"))				
							m_gridProfit.SetSaveGridToExcel(strFileName);
						else if (!strFileType.CompareNoCase("txt"))				
							SetSaveGridToText(strFileName);
					}
					bSave = FALSE;
					break;
				}
				else
					bSave = TRUE;
			}
			else
			{
				if(!strFileName.IsEmpty())
				{
					if (!strFileType.CompareNoCase("xls"))				
						m_gridProfit.SetSaveGridToExcel(strFileName);
					else if (!strFileType.CompareNoCase("txt"))				
						SetSaveGridToText(strFileName); //txt file save 
				}
				bSave = FALSE;
				break;
			}
		}
		else
		{
			bSave = FALSE;
			break;
		}
		
		if (bSave == FALSE)
			break;
	}
}

void CAnalystResultWnd::SetSaveGridToExcel(CString strFileName)
{
	CDrXLSFile xlsfile;
	CString strData = "";
	
	if (!xlsfile.OpenFile(strFileName))
	{
		CString strMsg = "지정한 파일이 다른 프로그램에서 사용중입니다";
		SetMessageBox(strMsg, (CString)PROGRAM_TITLE, MB_OK);
		return;
	}
	else
	{
		POSITION pos;
		AnalystInfo	stAnalystInfo;
		
		int nRow = 0, nCol = 0;
		for (int nIndex = (int)m_listAnalystInfo.GetCount() - 1; nIndex >= 0 ; nIndex--)
		{
			pos = m_listAnalystInfo.FindIndex(nIndex);
			stAnalystInfo = m_listAnalystInfo.GetAt(pos);
			
			nRow++;
			
			nCol = 0;
			strData = stAnalystInfo.strDate;
			xlsfile.WriteLabel(nRow, nCol, (LPCSTR)strData);
			
			nCol = 1;
			strData = stAnalystInfo.strTime;
			xlsfile.WriteLabel(nRow, nCol, (LPCSTR)strData);
			
			for (nCol = 2; nCol < (int)CODE_MAX_CNT; nCol++)
			{
				if (nIndex == 0)
				{
					strData = stAnalystInfo.strName[nCol - 2];
					xlsfile.WriteLabel(0, nCol, (LPCSTR)strData);
				}
				
				xlsfile.WriteNumber(nRow, nCol, stAnalystInfo.nCloseData[nCol - 2]);
				/*
				strData.Format("%d", stAnalystInfo.nCloseData[nCol - 2]);
				xlsfile.Write(nRow, nCol, (LPCSTR)strData);
				*/
			}
			
			nCol = (int)CODE_MAX_CNT + 1;
			if (nIndex == 0)
				xlsfile.WriteLabel(0, nCol, "거래소 종합");
			strData.Format("%f", stAnalystInfo.dCloseData_Kospi);
			xlsfile.WriteLabel(nRow, nCol, (LPCSTR)strData);
			
			nCol = (int)CODE_MAX_CNT + 2;
			/*
			strData.Format("%f", stAnalystInfo.dParallel_Kospi_All);
			xlsfile.Write(nRow, nCol, (LPCSTR)strData);
			*/
			xlsfile.WriteNumber(nRow, nCol, stAnalystInfo.dParallel_Kospi_All);
			
			nCol = (int)CODE_MAX_CNT + 3;
			if (nIndex == 0)
				xlsfile.WriteLabel(0, nCol, "코스닥 종합");
			strData.Format("%f", stAnalystInfo.dCloseData_Kosdaq);
			xlsfile.Write(nRow, nCol, (LPCSTR)strData);
			
			nCol = (int)CODE_MAX_CNT + 4;
			xlsfile.WriteNumber(nRow, nCol, stAnalystInfo.dParallel_Kosdaq_All);
			/*
			strData.Format("%f", stAnalystInfo.dParallel_Kosdaq_All);
			xlsfile.Write(nRow, nCol, (LPCSTR)strData);
			*/
			
			nCol = (int)CODE_MAX_CNT + 5;
			xlsfile.WriteNumber(nRow, nCol, stAnalystInfo.dParallel);
		}
	}
	xlsfile.CloseFile();
}


void CAnalystResultWnd::SetSaveGridToText(CString strFileName)
{
	CFile SaveFile(strFileName,CFile::modeCreate | CFile::modeWrite);
	
	/*
	int nRowCnt = m_pGridCtrl->GetNumberRows();
	
	  CString strLineData, strCell;
	  int nCol, nRow;
	  for (nCol = 0; nCol < nRowCnt; nCol++)
	  {
	  // 타이틀 저장용
	  if (nCol == 0)
	  {
	  strCell = m_pGridCtrl->GetCellData(0, 0);
	  strLineData = strCell;	// 종목명
	  strLineData += " ";
	  
		for (nRow = 1; nRow < m_pGridCtrl->GetNumberCols(); nRow++)
		{
		strCell = m_pGridCtrl->GetCellData(0, nRow);
		strLineData += strCell;	// 각 Item 정보
		strLineData += " ";
		}
		
		  strLineData += "\r\n";
		  SaveFile.Write( strLineData, strLineData.GetLength() );			
		  }
		  
			strCell = m_pGridCtrl->GetCellData(nCol, 0);
			strLineData = strCell;	// 종목명
			strLineData += " ";
			
			  for (nRow = 1; nRow < m_pGridCtrl->GetNumberCols();  nRow++)
			  {
			  strCell = m_pGridCtrl->GetCellData(nCol, nRow);
			  strLineData += strCell;	// 각 Item 정보
			  strLineData += " ";
			  }
			  
				strLineData += "\r\n";
				SaveFile.Write(strLineData, strLineData.GetLength());
				}
				
				  SaveFile.Close();
	*/
}


BOOL CAnalystResultWnd::GetLoadIniInfo(CString strFile)
{
	CFileFind cFind;
	BOOL bRead = TRUE;
	char chData[10];
	
	bRead = cFind.FindFile(strFile);
	if (!bRead)
	{
		CString strMsg = strFile + "을 찾지 못했습니다.";
		//SetSendMainMsg(strMsg);
	}
	else
	{
		GetPrivateProfileString("PortAnalyst", "DataSave", "0", chData, sizeof(chData), strFile);
		m_bDataSave = (atoi(chData) == 1) ? TRUE : FALSE;
	}
	
	return bRead;
}

int CAnalystResultWnd::SetMessageBox(LPCTSTR lpszText, LPCTSTR lpszCaption, UINT nType)
{
	return ::MessageBox(m_hWnd, lpszText, lpszCaption, nType);
	
	/*
	CMainFrmWnd *pwndMainFrm = (CMainFrmWnd*)GetParent();
	pwndMainFrm->SetSendMainMsg(strMsg);
	*/
}

void CAnalystResultWnd::OnPaint() 
{
	CPaintDC dc(this); // device context for painting

	CAnalystMainFrmWnd *pwndAnalystMainFrm = (CAnalystMainFrmWnd*)GetParent();

	CRect reClient;
	GetClientRect(reClient);

	dc.FillSolidRect(reClient, pwndAnalystMainFrm->GetAxColor(66));
	
}

void CAnalystResultWnd::ChangeSkinColor(COLORREF clrBkgnd, ST_TABCOLOR stTabColor)
{
	if(!clrBkgnd)
		return;
	
	m_clrResultSkin = clrBkgnd;
	
	if(m_brush.DeleteObject())
		m_brush.CreateSolidBrush(m_clrResultSkin);

	m_btnTarget_Day.Invalidate();
	m_btnTarget_Min.Invalidate();
	m_btnChk_All.Invalidate();
	m_btnChk_Kospi.Invalidate();
	m_btnChk_Kosdaq.Invalidate();
	m_btnChk_Kosdaq.Invalidate();
	m_btnChk_KospiAll.Invalidate();
	m_btnChk_KosdaqAll.Invalidate();

	Invalidate();
	
}

void CAnalystResultWnd::ChangeGridColor(ST_GRIDCOLOR stGridColor)
{
	m_gridProfit.ChangeGridColor(stGridColor);
}


BOOL CAnalystResultWnd::OnEraseBkgnd(CDC* pDC)
{
	CRect reClient;
	GetClientRect(reClient);
	pDC->FillSolidRect(reClient, m_clrResultSkin);

	return FALSE;
}

// 2012.2.14 박성경: 50개 이상검색후 그리드 클릭시 종목 차트 안그려지는 오류 수정
void CAnalystResultWnd::SetInit_AnalystInfo()
{
	if(m_SublistAnalystInfo.GetCount() > 0)
	{
		AnalystInfo stAnalystInfo;
		for(int index = 0; index < m_SublistAnalystInfo.GetCount(); index++)
		{
			POSITION pos = m_SublistAnalystInfo.FindIndex(index);
			stAnalystInfo = m_SublistAnalystInfo.GetAt(pos);
			m_listAnalystInfo.AddTail(stAnalystInfo);
		}
	}
}