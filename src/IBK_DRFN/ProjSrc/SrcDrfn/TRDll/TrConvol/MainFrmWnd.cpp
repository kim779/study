// MainFrmWnd.cpp : implementation file
//

#include "stdafx.h"
#include "MainFrmWnd.h"
#include "ScreenWnd.h"

#include <math.h>

//#include "StdDialog.h"
//#include "TrConVolView.h"

//#include "FilterSetDlg.h"
//#include "FilterSetWnd.h"

#include "FilterSet.h"
#include "FieldSet.h"
#include "TargetSet.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMainFrmWnd
#define WMU_SGNLCTRL_SIGNAL			(WM_APP + 0x303)
#define WMU_FILTER_DLG				(WM_USER + 1000)

extern HINSTANCE g_hInstance;

IMPLEMENT_DYNCREATE(CMainFrmWnd, CFrameWnd)

CMainFrmWnd::CMainFrmWnd()
{
	LOGFONT	  logFont;
	GETFONT(logFont, 12, 400, m_fontDefault);
	GETFONT(logFont, 12, 600, m_fontBold);

	GETFONT(logFont, 16, FW_BOLD, m_fontNotice1);
	GETFONT(logFont, 14, FW_BOLD, m_fontNotice2);
	
	m_dwAdviseKey = NULL;
	m_bFiltering = TRUE;
	m_pTargetSet = NULL;
	m_bCheckSync = FALSE;
	m_bRight	 = FALSE;

	m_pCommApiHelper = NULL;
	m_nConnectID	 = 0;

	m_pdlgFieldEdit = NULL;
	m_pDlgFilterSet = NULL;
	m_bSiseOnly = FALSE;

	m_bWatching = TRUE;

	m_bTimerChk = FALSE;
}

CMainFrmWnd::~CMainFrmWnd()
{
}


BEGIN_MESSAGE_MAP(CMainFrmWnd, CFrameWnd)
	//{{AFX_MSG_MAP(CMainFrmWnd)
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_WM_SIZE()
	ON_WM_ERASEBKGND()
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BTN_ERASE, OnClickbtnErase)
	ON_BN_CLICKED(IDC_BTN_FIELDSET, OnClickbtnFieldSet)
	ON_BN_CLICKED(IDC_BTN_CONDITIONSET, OnClickbntConditionSet)
	ON_BN_CLICKED(IDC_BTN_WATCHSET, OnClickbtnWatchSet)

	ON_MESSAGE(WM_BC_DATA_RECEIVED, OnGetBroad)
	ON_MESSAGE(WM_NOTIFY_GRID, OnNotify_Grid)
	ON_MESSAGE(WMU_SGNLCTRL_SIGNAL, OnTest)

END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMainFrmWnd message handlers

// 신호컨트롤의 신호발생시 통보되는 신호내용 관리 Structure
typedef struct tagSCSignal
{
	CString		strCode;				// 종목코드
	CString		strName;				// 종목명
	
	CString		strSgnlTime;			// 신호발생 시간
	CString		strSgnlCode;			// 신호종류
	CString		strSgnlTitle;			// 신호명
	COLORREF	strSgnlText;			// 신호내용에 적용할 폰트컬러
	
} SCSIGNAL, *LPSCSIGNAL;

LRESULT CMainFrmWnd::OnTest(WPARAM wParam, LPARAM lParam)
{
	SCSIGNAL lp;
	lp = *(SCSIGNAL*)wParam;

	return TRUE;
}

int CMainFrmWnd::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

// 	m_pCommApiHelper = new CCommApiHelper;
// 	m_pCommApiHelper->LoadCommHelper();
// 	m_nConnectID = m_pCommApiHelper->CommGetConnectionID(CONNECT_ID_RQ);

	m_clrMainSkin = COLOR_MAIN_CANVAS;

	m_ctrlGrid.CreateGrid(WS_CHILD | WS_VISIBLE | WS_BORDER, CRect(0, 0, 0, 0), this, IDC_GRID_CONVRESULT);
	m_ctrlGrid.HeaderSetup();

	CRect rtNULL(0, 0, 0, 0);

//	m_pBtnAdd.Create(_T("추가"), rtNULL, this, IDC_BTN_ADD);
//	SetBtnImg(&m_pBtnAdd, 2);
//	m_hBtnAdd = m_pBtnAdd.GetSafeHwnd();
//	
//	m_pBtnRemove.Create(_T("삭제"), rtNULL, this, IDC_BTN_REMOVE);
//	SetBtnImg(&m_pBtnRemove, 2);
//	m_hBtnRemove = m_pBtnRemove.GetSafeHwnd();
//	
//	m_pBtnRemoveAll.Create(_T("전체삭제"), rtNULL, this, IDC_BTN_REMOVEALL);
//	SetBtnImg(&m_pBtnRemoveAll, 4);
//	m_hBtnRemoveAll = m_pBtnRemoveAll.GetSafeHwnd();

	m_btnErase.Create(_T("지우기"), rtNULL, this, IDC_BTN_ERASE);
	SetBtnImg(&m_btnErase, 4);

	m_btnFieldSet.Create(_T("항목편집"), rtNULL, this, IDC_BTN_FIELDSET);
	SetBtnImg(&m_btnFieldSet, 4);
	
	m_btnConditionSet.Create(_T("조건설정"), rtNULL, this, IDC_BTN_CONDITIONSET);
	SetBtnImg(&m_btnConditionSet, 4);
	
	m_btnWatchSet.Create(_T("해제"), rtNULL, this, IDC_BTN_WATCHSET);
	SetBtnImg(&m_btnWatchSet, 2);

	return 0;
}

void CMainFrmWnd::OnDestroy() 
{
	KillTimer(0);

	DoSaveUserInfo();

	m_fontBold.DeleteObject();
	m_fontDefault.DeleteObject();
	m_fontNotice1.DeleteObject();
	m_fontNotice2.DeleteObject();
	
	if(m_pdlgFieldEdit)
	{
		delete m_pdlgFieldEdit;
		m_pdlgFieldEdit = NULL;
	}

	if(m_pDlgFilterSet)
	{
		delete m_pDlgFilterSet;
		m_pDlgFilterSet = NULL;
		GetParent()->SendMessage(RMSG_KSMOVECTRL, (WPARAM)FALSE, 0);
	}

	CFrameWnd::OnDestroy();

	// TODO: Add your message handler code here
}

void CMainFrmWnd::DoSaveUserInfo(LPCSTR _szSaveFile)
{
	CString strFilterPath = _szSaveFile;
	if(strFilterPath.GetLength()==0)
		strFilterPath = GetPath(CONVOL_FILE);

	CFieldSet::SetSave_FieldInfo(strFilterPath, &m_stMainFieldSet);
	CFilterSet::SetSave_ConVolInfo(strFilterPath, &m_stConVolSet);
	CTargetSet::SetSave_ConVolTargetInfo(strFilterPath, &m_stSEARCHTARGET);
}

void CMainFrmWnd::SetInit(BOOL bSiseOnly)
{	
 	m_bSiseOnly =  bSiseOnly;
	
	SetUserRight(TRUE);

 	memset(&m_stMainFieldSet, 0x00, sizeof(StFieldSet) );
 
 	GetConVol_FieldInfo();
 	GetConVol_FilterInfo();
 	GetConVol_TarGetInfo();

 	HeaderChange(&m_stMainFieldSet, TRUE);

	m_btnWatchSet.SetTextColor(RGB(255, 0, 0));
	Modual_SendReal(TRUE, "");

	SetTimer(0, 500, NULL);
	m_TimeEstimate.StartEstimate();

	return;
}

void CMainFrmWnd::Modual_SendReal(BOOL bReg, CString strKey)
{
// 	char* szDataName = "H0STFBC0";
// 	char* aFieldName = "*.*";
// 
// 	CString strAdvKey = strKey + "CVOL";
//
// 	if(bReg)
// 		m_pCommApiHelper->CommSB(m_hWnd, szDataName, strAdvKey, strAdvKey.GetLength(), 1, m_nConnectID);
// 	else
// 		m_pCommApiHelper->CommSBC(m_hWnd, szDataName, strAdvKey, strAdvKey.GetLength(), 1, m_nConnectID);
// 		
// 
// 	{
// 		int nSize = sizeof(STIDM2Info_RA) + strKey.GetLength()-sizeof(char);
// 		LPBYTE pInfoBuf = new BYTE[nSize+1];
// 		STIDM2Info_RA* pRA = (STIDM2Info_RA*)pInfoBuf;
// 		ZeroMemory(pRA, nSize);
// 		
// 		char* aFieldName="*";
// 		pRA->nInfoSize = nSize;
// 		sprintf(pRA->aKey, "%08X", this);
// 		memcpy(pRA->aDataName, szDataName, strlen(szDataName));
// 		memcpy(pRA->aFieldName, aFieldName, strlen(aFieldName));
// 		pRA->nKeyNameSize = strKey.GetLength();//strlen(KeyName);
// 		memcpy(pRA->aKeyName, (LPCSTR)strKey, strKey.GetLength());
// 		
// 		delete[] pInfoBuf;
// 	}

	if(bReg)
	{
		CScreenWnd* pScreenWnd = (CScreenWnd*)GetParent();
		
		int nType = 2;
		
		char* lpData = new char[sizeof(int) + 1];
		::memset(lpData, 0x00, sizeof(int) + 1);
		memcpy(lpData, &nType, sizeof(int) + 1);
		
		pScreenWnd->SendTR(TRNO_SET_REAL, lpData, sizeof(int), TRKEY_SET_REAL, US_KEY);
	}
}


long CMainFrmWnd::OnGetBroad(WPARAM wParam, LPARAM lParam)
{
// 	memset(&m_realItem, 0x00, sizeof(ST_REALITEM));
// 	memcpy(&m_realItem, (void*)lParam, sizeof(ST_REALITEM));
//	ST_REALITEM *pstRealData = (ST_REALITEM*)&m_realItem;
 	
	if(!m_bWatching)
		return FALSE;

	ST_REALITEM *pstRealData = (ST_REALITEM*)lParam;

	StTrConVol	stTrConVol;
	memset(&stTrConVol, 0x00, sizeof(StTrConVol));
	memcpy(&stTrConVol, pstRealData->szBcData, sizeof(StTrConVol));

	BOOL bFilter;
	bFilter = RunConVol_Filtering(stTrConVol);		//필터 적용
	if(bFilter)
	{
		StConVolClient		stConVolClient;
		memset(&stConVolClient, 0x00, sizeof(StConVolClient));	
		PacketConvertToClientMap(stConVolClient, stTrConVol);

		BOOL bInsert = (!m_stConVolSet.nItemPosition) ? TRUE : FALSE;

		if(m_bTimerChk && bInsert)
		{
			m_listTempStConVol.AddTail(stConVolClient);
			return 0;
		}
		else
			m_ctrlGrid.SetAddRow(stConVolClient, bInsert);

		if(m_stConVolSet.nChkSound)
			PlaySound();
		/*m_ctrlGrid.SetAddRow(stTrConVol, bInsert);*/

		if(!bInsert)
			m_ctrlGrid.SetScrollPos(SB_VERT, m_ctrlGrid.GetNumberRows());

		SetCopy_ConVolData(stConVolClient);
	}

	return 0;
}

LRESULT CMainFrmWnd::OnNotify_Grid(WPARAM wParam, LPARAM lParam)
{
	if (wParam == NM_GRID_CLK)
	{
		CString strCode = *(CString*)lParam;

		SetSharedDataToCode(strCode);
	}
	else if(wParam == NM_GRID_RCLK)
	{
		RButtonInfo stRInfo = *(RButtonInfo*)lParam;
		CPoint point;
		
		point.x = stRInfo.xPoint;
		point.y = stRInfo.yPoint;
		
		OnRButtonClick(&point, stRInfo.strCode);
	}

	return 0;
}

BOOL CMainFrmWnd::PacketConvertToClientMap(StConVolClient &stConVolClient, StTrConVol stTrConVol)
{	//화면 필드에서 사용하는 구조체만 추려냄
	CString strPosition;
	strPosition.Format("%d", m_stConVolSet.nItemPosition);
	memcpy(stConVolClient.chGridOpt,	 strPosition,				strPosition.GetLength());
	memcpy(stConVolClient.cSign,		 stTrConVol.sSign,			sizeof(stTrConVol.sSign));
	memcpy(stConVolClient.sTime,		 stTrConVol.sSignaltime,	sizeof(stTrConVol.sSignaltime));
	memcpy(stConVolClient.sCode,		 stTrConVol.sItemCode,		sizeof(stTrConVol.sItemCode));
	memcpy(stConVolClient.sItemName,	 stTrConVol.sItemName,		sizeof(stTrConVol.sItemName));
	memcpy(stConVolClient.sPriceNow,	 stTrConVol.sPrice,			sizeof(stTrConVol.sPrice));
	memcpy(stConVolClient.sComparison,	 stTrConVol.sChange,		sizeof(stTrConVol.sChange));
	memcpy(stConVolClient.sChgRate,		 stTrConVol.sChgrate,		sizeof(stTrConVol.sChgrate));
	memcpy(stConVolClient.sConVol,		 stTrConVol.sCvolume,		sizeof(stTrConVol.sCvolume));
	memcpy(stConVolClient.sAccumDealVol, stTrConVol.sVolume,		sizeof(stTrConVol.sVolume));
	memcpy(stConVolClient.sDealVolRate,	 stTrConVol.sVolumerate,	sizeof(stTrConVol.sVolumerate));
	memcpy(stConVolClient.sConVolRate,	 stTrConVol.sCvolumerate,	sizeof(stTrConVol.sCvolumerate));
	memcpy(stConVolClient.chGubun,		 stTrConVol.sCGubun,		sizeof(stTrConVol.sCGubun));
	memcpy(stConVolClient.sVolumePower,  stTrConVol.sVolumePower,	sizeof(stTrConVol.sVolumePower));	// 체결강도 (20090408 추가)
	memcpy(stConVolClient.sEntryCnt,	 stTrConVol.sEntryCnt,		sizeof(stTrConVol.sEntryCnt));
	return TRUE;
}


void CMainFrmWnd::FieldSet()
{
	HINSTANCE hInstSave = AfxGetResourceHandle();
	AfxSetResourceHandle(g_hInstance);	

	CFieldSet	dlgFieldSet(this, &m_stMainFieldSet);
	dlgFieldSet.m_strScreenNo = m_strSaveKey;
	if(dlgFieldSet.DoModal() == IDOK)
	{
		if(dlgFieldSet.m_bChange)
			HeaderChange(&m_stMainFieldSet);
		m_ctrlGrid.Invalidate();
	}

	AfxSetResourceHandle(hInstSave);	
}

void CMainFrmWnd::OnFieldSetBtnOK(StFieldSet *pstFieldSet)
{
	HeaderChange(pstFieldSet);
	m_ctrlGrid.Invalidate();
}

BOOL CMainFrmWnd::FilterSet()
{
	BOOL bOk = FALSE;

	HINSTANCE hInstSave = AfxGetResourceHandle();
	AfxSetResourceHandle(g_hInstance);

	m_arExceptItemCode.RemoveAll();
	m_arExceptItemName.RemoveAll();
	for(int i = 0; i < m_stSEARCHTARGET.saExceptCode.GetSize(); i++)
	{
		m_arExceptItemCode.Add(m_stSEARCHTARGET.saExceptCode.GetAt(i));
	}
	
	if(m_pDlgFilterSet == NULL)
	{
		m_pDlgFilterSet = new CFilterSet(this, m_strSaveKey, &m_stConVolSet, GetAxColor(66));
		m_pDlgFilterSet->Create(IDD_DLG_FILTERSET, this);

		//m_pDlgFilterSet->SetCodeInfo(m_pctrlSelectCodeInput, m_hSelectCodeHwnd, m_pctrlExceptCodeInput, m_hExceptCodeHwnd);
		//m_pDlgFilterSet->SetBtnInfo(&m_pBtnAdd, m_hBtnAdd, &m_pBtnRemove, m_hBtnRemove, &m_pBtnRemoveAll, m_hBtnRemoveAll);
		m_pDlgFilterSet->SetInitExceptItem(&m_arExceptItemCode, &m_arExceptItemName);
		m_pDlgFilterSet->SetInit();

		// 2012.4.20 박성경: PreTranslateMessage 호출 안되는 문제 수정
		CScreenWnd* pScreenWnd = NULL;
		pScreenWnd->SetPermanentHandle(m_pDlgFilterSet->GetSafeHwnd(), m_pDlgFilterSet);
		
	}
	else
	{
		//m_pDlgFilterSet->SetCodeInfo(m_pctrlSelectCodeInput, m_hSelectCodeHwnd, m_pctrlExceptCodeInput, m_hExceptCodeHwnd);
		//m_pDlgFilterSet->SetBtnInfo(m_pBtnAdd, m_hBtnAdd, m_pBtnRemove, m_hBtnRemove, m_pBtnRemoveAll, m_hBtnRemoveAll);
		m_pDlgFilterSet->SetInitExceptItem(&m_arExceptItemCode, &m_arExceptItemName);
	}

	m_bCheckSync = TRUE;
	m_pDlgFilterSet->SetUserRight(m_bRight);

	CRect rcCaller;
	GetWindowRect(rcCaller);
	m_pDlgFilterSet->CenterWindow();//NULL,rcCaller.left, rcCaller.top, 0, 0, SWP_NOZORDER|SWP_NOSIZE);
	m_pDlgFilterSet->OnSizeCtrl();
	m_pDlgFilterSet->ShowWindow(SW_SHOW);

/*
	if(m_pDlgFilterSet->DoModal() == IDOK)
	{
		bOk = TRUE;
		m_stSEARCHTARGET.saExceptCode.RemoveAll();
		for(int i = 0; i < m_arExceptItemCode.GetSize(); i++)
			m_stSEARCHTARGET.saExceptCode.Add(m_arExceptItemCode.GetAt(i));
			
	}
*/
	m_bCheckSync = FALSE;
	AfxSetResourceHandle(hInstSave);
	return bOk;
}

void CMainFrmWnd::OnFilterSetMessage(int nFlag)
{
	if(m_pDlgFilterSet)
	{
		if(nFlag == IDOK)
		{
			m_stSEARCHTARGET.saExceptCode.RemoveAll();
			for(int i = 0; i < m_arExceptItemCode.GetSize(); i++)
				m_stSEARCHTARGET.saExceptCode.Add(m_arExceptItemCode.GetAt(i));

			m_stConVolSet = m_pDlgFilterSet->m_stConVolSet;
		//KHD	AfxMessageBox(m_stConVolSet.strMarketCode);
		}
		else if(nFlag == IDCANCEL)
		{
			//m_pDlgFilterSet->SetInitExceptItem(&m_arExceptItemCode, &m_arExceptItemName);
			//delete m_pDlgFilterSet;			
			PostMessage(WMU_FILTER_DLG);			
		}
	}
}
void CMainFrmWnd::TargetSet()
{
	HINSTANCE hInstSave = AfxGetResourceHandle();
	AfxSetResourceHandle(g_hInstance);

	CTargetSet	dlgTargetSet(this, m_strSaveKey, &m_stSEARCHTARGET);
	m_pTargetSet = &dlgTargetSet;
	if(dlgTargetSet.DoModal() == IDOK)
	{
//		m_sect.Lock();
//		STCOPY_P_ST_SEARCHTARGET((&dlgTargetSet.m_stSearchTarget), (&m_stSEARCHTARGET));
//		m_sect.Unlock();ㅇ
	}
	m_pTargetSet = NULL;
	AfxSetResourceHandle(hInstSave);
}

void CMainFrmWnd::TargetItemSet()
{
	
}

void CMainFrmWnd::OnSize(UINT nType, int cx, int cy) 
{
	CFrameWnd::OnSize(nType, cx, cy);

	CRect rcClient;
	GetClientRect(rcClient);
	
	rcClient.top += 3;
	rcClient.left += 3;
	rcClient.right -= 3;
	rcClient.bottom -= 3;

	CRect reTemp = rcClient;
	reTemp.bottom = rcClient.top + 20;
	reTemp.right = reTemp.left + 60;
	m_btnErase.MoveWindow(reTemp);

	reTemp.left = reTemp.right + 2;
	reTemp.right = reTemp.left + 36;
	m_btnWatchSet.MoveWindow(reTemp);

	reTemp.left = reTemp.right + 2;
	reTemp.right = reTemp.left + 60;
	m_btnFieldSet.MoveWindow(reTemp);

	reTemp.left = reTemp.right + 2;
	reTemp.right = reTemp.left + 60;
	m_btnConditionSet.MoveWindow(reTemp);

	reTemp.left = rcClient.left;
	reTemp.top = reTemp.bottom + 3;
	reTemp.right = rcClient.right;
	reTemp.bottom = rcClient.bottom;

	m_ctrlGrid.MoveWindow(reTemp);	
}

CString CMainFrmWnd::GetPath(CString strdiv)
{
	CString strUserPath;
	strUserPath.Format("%s%s\\%s\\%s\\%s", Variant(homeCC), USRDIR, Variant(nameCC), FOLDER_FINDDATA ,strdiv);

	return strUserPath;
}

BOOL CMainFrmWnd::GetConVol_FieldInfo()
{
	BOOL bResult = TRUE;
	CString strFieldPath = GetPath(CONVOL_FILE);

	CFieldSet::GetLoad_FieldInfo(strFieldPath, &m_stMainFieldSet);

	CString strMaxRow;
	strMaxRow.Format("%-3.3s", m_stMainFieldSet.chColNum);
	m_nOldShow = atoi(strMaxRow);

	m_ctrlGrid.m_nMaxRow = m_nOldShow;
	return bResult;
}

BOOL CMainFrmWnd::GetConVol_FilterInfo()
{
	BOOL bResult = TRUE;
	CString strFilterPath = GetPath(CONVOL_FILE);

	CFilterSet::GetLoad_ConVolInfo(strFilterPath, &m_stConVolSet, m_bRight);
	return bResult;
}


BOOL CMainFrmWnd::GetConVol_TarGetInfo()
{
	BOOL bResult = TRUE;

	CString strFilterPath = GetPath(CONVOL_FILE);
	CTargetSet::GetLoad_ConVolTargetInfo(strFilterPath, &m_stSEARCHTARGET);

	return bResult;
}


BOOL CMainFrmWnd::RunConVol_Filtering(StTrConVol	stTrConVol)
{
	BOOL bResult = TRUE;

	if(m_bFiltering == FALSE)  //필터링 사용안함
		return bResult;
// #검색범위 필터 정보#
	
// 	CString strGroupName;
// 	int		nMarket = 0;
// 	strGroupName = m_stSEARCHTARGET.strGroupName;
// 
// 	if(strcmp(strGroupName, "거래소") == 0)
// 		nMarket = 1;
// 	else if(strcmp(strGroupName, "코스닥") == 0)
// 		nMarket = 2;
// 	else 
// 		nMarket = 100;
// // 시장 정보 확인 (선택 그룹이 거래소, 코스닥일 경우만 체크)
// 	if (nMarket == 1 || nMarket ==2)
// 	{
// 		char sMarket[1+1]={0,};
// 		sMarket[0] = stTrConVol.sMarket[0];
// 		if( atoi(sMarket) != nMarket)
// 		return FALSE;
// 	}
	
	switch (m_stConVolSet.nChkTargetUpjong)
	{
		case 0 :		// 전체
			break;

		case 1 :		// 거래소
			if(stTrConVol.sMflag[0] != '1')
				return FALSE;
			break;

		case 2 :		// 코스닥
			if(stTrConVol.sMflag[0] != '2')
				return FALSE;
			break;

		case 3 :		// 업종선택
// 			CStringArray saMarketCode;
// 			
// 			char buf[8];
// 			memset(buf, 0x00, sizeof(buf));
// 			memcpy(buf, stTrConVol.sCategorybranch_l, sizeof(stTrConVol.sCategorybranch_l)); // 대분류
// 			saMarketCode.Add(buf);
// 
// 			memcpy(buf, stTrConVol.sCategorybranch_m, sizeof(stTrConVol.sCategorybranch_m)); // 중분류
// 			saMarketCode.Add(buf);
// 
// 			memcpy(buf, stTrConVol.sCategorybranch_s, sizeof(stTrConVol.sCategorybranch_s)); // 소분류
// 			saMarketCode.Add(buf);
// 
// 			BOOL bChk = FALSE;
// 
// 
// 
// 			for(int i = 0; i < saMarketCode.GetSize(); i++)
// 			{
// 				if(saMarketCode.GetAt(i) == m_stConVolSet.strMarketCode)
// 				{
// 					bChk = TRUE;
// 					break;
// 				}
// 			}

			CStringArray arrCode, arrName;
			CString strCode;
			UINT Jgub, Upgub;
			BOOL bChk = FALSE;
			char* pBuff;
			
			STR_CONVERT(pBuff, stTrConVol.sItemCode, sizeof(stTrConVol.sItemCode), strCode);

			Jgub = _ttoi(m_stConVolSet.strMarketCode.Mid(0, 1)) * 100;
			Upgub = _ttoi(m_stConVolSet.strMarketCode);
			GetStockMasterData(arrCode, arrName, Jgub, Upgub);

			for(int index = 0; index < arrCode.GetSize(); index++)
			{
				if(strCode == arrCode.GetAt(index))
				{
					bChk = TRUE;
					break;
				}
			}

			if(!bChk)
				return FALSE;

			break;
	}

	CString strData;
	CString strCode;
	CString strName;
	char* pBuff;
	char* sCode;
	int nSize;
//제외 종목 코드 확인
//	nSize = m_stSEARCHTARGET.nSizeExceptCode;
	nSize = m_stSEARCHTARGET.saExceptCode.GetSize();
	STR_CONVERT(pBuff, stTrConVol.sItemCode, sizeof(stTrConVol.sItemCode), strCode);

	int nIndex = 0;
	for(nIndex = 0; nIndex < nSize; nIndex++)
	{
		strData = m_stSEARCHTARGET.saExceptCode.GetAt(nIndex);
		sCode = StringDivideCodeAndName(strData, strName);
		if(strCode.Compare(sCode)== 0)
		{
			delete sCode;
			return FALSE;
		}
		delete sCode;		
	}
		
//선택 그룹 코드 확인 (KOSPI200, KRX100, 관심종목, 보유종목, 최근조회, 테마종목)
//	nSize = m_stSEARCHTARGET.nSizeTargetcode;                     
	nSize = m_stSEARCHTARGET.saTargetCode.GetSize();
	if(nSize > 0)
	{
		strData = m_stSEARCHTARGET.saTargetCode.GetAt(0); 
//		int nLength = strData.GetLength();

		int nPos = strData.Find(":");
		if(nPos == 3) //Ex) 101:KOSPI200, 
			strData = strData.Left(3); 
		
/*		if((nSize == 1 && nLength < 4))    //업종 대표 코드 선택시(Ex)거래소의 종합
		{	
			STR_CONVERT(pBuff, stTrConVol.sCategorybranch_m, sizeof(stTrConVol.sCategorybranch_m), strCode);
			if(strData.Compare(strCode) != 0)
				return FALSE;		
		}	
*/		BOOL IsTargetCode = FALSE;
		if(nPos == 3)
		{
			STR_CONVERT(pBuff, stTrConVol.sCategorybranch_m, sizeof(stTrConVol.sCategorybranch_m), strCode);
			for(nIndex = 0; nIndex < nSize; nIndex++)
			{
				strData = m_stSEARCHTARGET.saTargetCode.GetAt(nIndex);
				strData = strData.Left(3);
				
				if(strData.Compare(strCode) != 0)
				{
					IsTargetCode = TRUE;
				}				
			}	
			if(!IsTargetCode)
				return FALSE;
		}
		else   //일반 코드 
		{
			STR_CONVERT(pBuff, stTrConVol.sItemCode, sizeof(stTrConVol.sItemCode), strCode);
			
			for(nIndex = 0; nIndex < nSize; nIndex++)
			{
				strData = m_stSEARCHTARGET.saTargetCode.GetAt(nIndex);
				sCode = StringDivideCodeAndName(strData, strName);			
				
				if(strcmp((LPCSTR)strCode, sCode) == 0)
				{
					IsTargetCode = TRUE;		
				}
				delete sCode;
			}
			if(!IsTargetCode)
				return FALSE;
		}
	}

//대상 제외 항목 확인
	int nChk = 0;
/*
	nChk = m_stSEARCHTARGET.nChkManagement;
	if(nChk > 0)
	{
		if(stTrConVol.sManagement[0] == '1')
			return FALSE;
	}

	nChk = m_stSEARCHTARGET.nChkTradeStop;
	if(nChk > 0)
	{
		if( stTrConVol.sTradestop[0] =='1')
			return FALSE;
	}

	nChk = m_stSEARCHTARGET.nChkPrefferShare;
	if(nChk > 0)
	{
		if( stTrConVol.sPreferShare[0] == '1' )
			return FALSE;
	}

	nChk = m_stSEARCHTARGET.nChkDepositMoney;
	if(nChk > 0)
	{
		if( stTrConVol.sDepositmoney[0] == '1' )
			return FALSE;
	}

	nChk = m_stSEARCHTARGET.nChkLogicSell;
	if(nChk > 0)
	{
		if( stTrConVol.sJungrimeme[0] == '1' )
			return FALSE;
	}

	nChk = m_stSEARCHTARGET.nChkInsincerityNotice;
	if(nChk > 0)
	{
		if( stTrConVol.sInsinceritynotice[0] =='1')
			return FALSE;
	}

	nChk = m_stSEARCHTARGET.nChkInvestRisk;
	if(nChk > 0)
	{
		if( stTrConVol.sInvestrisk[0] == '1' )
			return FALSE;
	}

	nChk = m_stSEARCHTARGET.nChkInvestWarn;
	if(nChk > 0)
	{
		if( stTrConVol.sInvestwarn[0] == '1' )
			return FALSE;
	}

	nChk = m_stSEARCHTARGET.nChkInvestAttention;
	if(nChk > 0)
	{
		if( stTrConVol.sInvestattention[0] == '1' )
			return FALSE;
	}

	//ELW 항목
	nChk = m_stSEARCHTARGET.nChkELW;
	if(nChk > 0)
	{
		if( stTrConVol.sItemCode[0]=='5' || stTrConVol.sItemCode[0]=='6' )
			return FALSE;
	}


	nChk = m_stSEARCHTARGET.nChkInvestRiskNotice;
	if(nChk > 0)
	{
		if( stTrConVol.sInvestrisknotice[0] == '1')
			return FALSE;
	}
*/
	nChk = m_stConVolSet.nChkManagement;			// 관리종목
	if(nChk > 0)
	{
		if(stTrConVol.sManagement[0] == '1')
			return FALSE;
	}

	nChk = m_stConVolSet.nChkPrefferShare;			// 우선주
	if(nChk > 0)
	{
		if(stTrConVol.sPreferShare[0] == '1')
			return FALSE;
	}

	nChk = m_stConVolSet.nChkInvestAttention;		// 투자주의
	if(nChk > 0)
	{
		if(stTrConVol.sInvestattention[0] == '1')
			return FALSE;
	}

	nChk = m_stConVolSet.nChkInvestWarning;			// 투자경고
	if(nChk > 0)
	{
		if(stTrConVol.sInvestwarn[0] == '1')
			return FALSE;
	}

	nChk = m_stConVolSet.nChkPreInvestRisk;			// 투자위험예고
	if(nChk > 0)
	{
		if(stTrConVol.sInvestrisknotice[0] == '1')
			return FALSE;
	}

	nChk = m_stConVolSet.nChkInvestRisk;			// 투자위험
	if(nChk > 0)
	{
		if(stTrConVol.sInvestrisk[0] == '1')
			return FALSE;
	}

	nChk = m_stConVolSet.nChkTidyDeal;				// 정리매매
	if(nChk > 0)
	{
		if(stTrConVol.sJungrimeme[0] == '1')
			return FALSE;
	}

	nChk = m_stConVolSet.nChkDealStop;				// 거래정지
	if(nChk > 0)
	{
		if(stTrConVol.sTradestop[0] == '1')
			return FALSE;
	}

	nChk = m_stConVolSet.nChkInsincerityNotice;		// 불성실공시
	if(nChk > 0)
	{
		if(stTrConVol.sInsinceritynotice[0] == '1')
			return FALSE;
	}

	nChk = m_stConVolSet.nChkDeposit;				// 증거금 100%
	if(nChk > 0)
	{
		if((LPSTR)(stTrConVol.sDepositmoney) == "100")
			return FALSE;
	}

	for(int i = 0; i < m_arExceptItemCode.GetSize(); i++)	//제외종목
	{
		if(stTrConVol.sItemCode == m_arExceptItemCode.GetAt(i))
			return FALSE;
	}


// #순간체결량 설정 화면 Filter 정보#
	int nConditionCheck = m_stConVolSet.nConditionCheck; //체결량 , 거래대금 #0 : 둘다만족 #1 : 하나이상

	char sCvolume[9+1]={0,};
	memcpy(sCvolume, stTrConVol.sCvolume, 9); //stTrConVol.sCvolume

	char sAmount[9+1]={0,};
//	memcpy(sAmount, stTrConVol.sAmount, 9);
	memcpy(sAmount, stTrConVol.sCamount, 9);

// 	if( atol(sCvolume) < m_stConVolSet.lConVol)
// 	{
// 		if(nConditionCheck == 0) //둘다 만족해야 할 경우
// 			return FALSE;	
// 		else
// 		{
// 		//	char sAmount[9+1]={0,};
// 		//	memcpy(sAmount, stTrConVol.sAmount, 9);
// 			if( atol(sAmount) < m_stConVolSet.lDealPrice)
// 				return FALSE;
// 		}
// 	}
// 	else
// 	{
// 		if(nConditionCheck == 0)
// 		{
// 			if( atol(sAmount) < m_stConVolSet.lDealPrice)
// 				return FALSE;
// 		}
// 	}

	switch(nConditionCheck)
	{
		case 0 :	// 둘다 만족
			if(!(atol(sCvolume) >= m_stConVolSet.lConVol && atol(sAmount) >= m_stConVolSet.lDealPrice))
				return FALSE;
			break;
		
		case 1 :	// 하나 이상 만족
			if(!(atol(sCvolume) >= m_stConVolSet.lConVol || atol(sAmount) >= m_stConVolSet.lDealPrice))
				return FALSE;
			break;
		
		case 2 :	// 거래대금만
			if(!(atol(sCvolume) < m_stConVolSet.lConVol && atol(sAmount) >= m_stConVolSet.lDealPrice))
				return FALSE;
			break;
		
		case 3 :	// 체결량만
			if(!(atol(sCvolume) >= m_stConVolSet.lConVol && atol(sAmount) < m_stConVolSet.lDealPrice))
				return FALSE;
			break;
	}


	if(m_stConVolSet.nChkAccumDealVol == 1)
	{
		memcpy(sCvolume, stTrConVol.sVolume, 9); sCvolume[9]=0;
		if(atol(sCvolume) < m_stConVolSet.lAccumDealVol)
			return FALSE;
	}

	if(m_stConVolSet.nChkSetPrice == 1)
	{
		char sPrice[8+1]={0,};
		memcpy(sPrice, stTrConVol.sPrice, 8); sPrice[8]=0;
		if(atol(sPrice) < m_stConVolSet.lSetPriceUp  || 
				atol(sPrice) > m_stConVolSet.lSetPriceDown)
			return FALSE;
	}

	if(m_stConVolSet.nChkChgRate == 1)
	{
		strData = "";
		char sChange[8+1]={0,};
		char sSign[1+1]={0,};
		memcpy(sChange, stTrConVol.sChgrate, 8);
		memcpy(sSign, stTrConVol.sSign, 1);
		
		if( strcmp(sSign, "4") ==0 || strcmp(sSign, "5") ==0 )
			strData = "-";
		
		strData += sChange;
		strData.Replace(" ", "");
		if(atof(strData) < m_stConVolSet.dChgRateUp  ||  
				atof(strData) > m_stConVolSet.dChgRateDown)
			return FALSE;
	}

	return bResult;
}

LPSTR CMainFrmWnd::StringDivideCodeAndName(CString strData, CString &strName)
{
	CString strCode;

	strName = strData;
	int nPos = strName.Find(":"); 
	strCode = strName.Left(nPos);
	if(nPos == -1)
		strCode = strData;
	strName = strName.Mid(nPos+1, strName.GetLength() - nPos);

	char *sCode;
	sCode = new char[strCode.GetLength() + 1];
	memset(sCode, 0x00, strCode.GetLength() + 1);
	memcpy(sCode, (LPSTR)(LPCTSTR)strCode, strCode.GetLength());
	return sCode;
}


// void CMainFrmWnd::ChangeSkinColor()
// {
// 	//m_clrMainSkin = COLOR_MAIN_CANVAS;	// GetParent()->SendMessage(RMSG_GET_SKINCOLOR, 0, 0);
// }

BOOL CMainFrmWnd::OnEraseBkgnd(CDC* pDC) 
{
	CBrush BackBrush(m_clrMainSkin);

	CRect rect, groupRect;
	pDC->GetClipBox(rect);

	CRgn rgnClient, rgnGrid;
	rgnClient.CreateRectRgnIndirect(rect);

	CRect rcGrid;
	m_ctrlGrid.GetWindowRect(rcGrid);
	ScreenToClient(rcGrid);

	rgnGrid.CreateRectRgnIndirect(rcGrid);
	rgnClient.CombineRgn(&rgnClient, &rgnGrid, RGN_DIFF);

	pDC->FillRgn(&rgnClient, &BackBrush);
	
	CRect rcClient;
	GetClientRect(&rcClient);
	rcClient.bottom = rcClient.top + 2;
	pDC->FillSolidRect(rcClient, m_clrMainSkin);

	BackBrush.DeleteObject();

	rgnClient.DeleteObject();
	rgnGrid.DeleteObject();
  
	return TRUE;
}

void CMainFrmWnd::PlaySound()
{	
	CString strFilePath = m_stConVolSet.strPathSound;	

	if(strFilePath.GetLength() > 0)
		sndPlaySound(strFilePath, SND_ASYNC );	
}

void CMainFrmWnd::SetUserRight(BOOL bRight)
{
	m_bRight = TRUE; // bRight; -hidden-
}

void CMainFrmWnd::ClearAll()
{
	m_ctrlGrid.ClearAll();
	m_ctrlGrid.m_nRow = -1;
}

void CMainFrmWnd::HeaderChange(StFieldSet *pstFieldSet, BOOL bFirst)
{
	char* sField = (char*)pstFieldSet;
	char* pBuff;
	int nShow;


	int nMaxSize=0;
	int j=0;

	int nColSizeBefore = m_ctrlGrid.GetNumberCols();
	for(int nIndex = 0; nIndex < _countof(DGN_HEADER_NAME); ++nIndex)
	{	
		INT_CONVERT(pBuff, sField + (nIndex), 1, nShow);

		if(nShow == 0)
			m_ctrlGrid.HideColumn(nIndex, TRUE);
		else
		{
			m_ctrlGrid.HideColumn(nIndex, FALSE);
			nMaxSize += m_ctrlGrid.GetColWidth(nIndex);
		}
	}

	//20090115 SJ_KIM 그리드 추가 방식 변경
	int nMaxRow;
	INT_CONVERT(pBuff, pstFieldSet->chColNum, 3, nMaxRow);

	int nGap = nMaxRow - m_ctrlGrid.m_nMaxRow ;
	m_ctrlGrid.m_nMaxRow = nMaxRow;	

	int nRow = m_ctrlGrid.GetNumberRows() - 1;
	if(nGap < 0)
	{
		for(int nIndex = 0; nIndex < fabs((double)nGap); nIndex++)
		{
			m_ctrlGrid.DeleteRow(nRow--);
		}		
 	}

	
	BOOL bInsert = (!m_stConVolSet.nItemPosition) ? TRUE : FALSE;
	int nRowHeight = 0;
	if(!bInsert) nRowHeight = GRID_ROW_HEIGHT;

	if(!bFirst)
	{
// 		CTrConVolView* pParentView = (CTrConVolView*)GetParent();
// 		pParentView->FrameResize(nMaxSize + 8 + 20,  nRowHeight * (nMaxRow + 5));
	}
}

void CMainFrmWnd::ChangeConditionSet()
{
	int nCurInsertType = m_stConVolSet.nItemPosition;

	BOOL bOK = FilterSet();

	char chColNum[4]={0,};
	memcpy(chColNum, m_stMainFieldSet.chColNum,3);
	m_nOldShow = atoi(chColNum);

	int nNewInsertType = m_stConVolSet.nItemPosition;

	if(nCurInsertType != nNewInsertType)			//그리드 삽입 방법 변경시
		Change_GridInsertType(nNewInsertType);
}

void CMainFrmWnd::Change_GridInsertType(int nType)
{
	m_ctrlGrid.ClearAll();

	BOOL bInsert = TRUE;
	CString strMaxRows;
	strMaxRows.Format("%-3.3s", m_stMainFieldSet.chColNum);

	if(nType == 1)				// Append
		bInsert = FALSE;

	m_ctrlGrid.m_nRow = -1;
	POSITION pos;
	for(int nIndex = 0; nIndex < m_listStConVol.GetCount(); nIndex++)
	{
		StConVolClient stConVolClient;
		pos = m_listStConVol.FindIndex(nIndex);
		stConVolClient = m_listStConVol.GetAt(pos);	
		m_ctrlGrid.SetAddRow(stConVolClient, bInsert);		
	}
	HeaderChange(&m_stMainFieldSet);
}

void CMainFrmWnd::SetCopy_ConVolData(StConVolClient stConVolClient)
{
	if(m_listStConVol.GetCount() >= 40 || m_listStConVol.GetCount() >= m_nOldShow )
		m_listStConVol.RemoveAll();
	
	m_listStConVol.AddTail(stConVolClient);
}


BOOL CMainFrmWnd::PreTranslateMessage(MSG* pMsg) 
{
	if(WM_RBUTTONUP == pMsg->message)
	{
		::PostMessage(GetParent()->m_hWnd, WM_CONTEXTMENU, 0, 0);
	}
	
	return CFrameWnd::PreTranslateMessage(pMsg);
}

void CMainFrmWnd::ChangeSkinColor(COLORREF clrBkgnd, ST_TABCOLOR stTabColor)
{
	if(!clrBkgnd)
		return;
	
	m_clrMainSkin = clrBkgnd;
	
	m_clrBkgnd = m_clrMainSkin;
	m_stTabColor = stTabColor;
	
	ST_GRIDCOLOR stGridColor;
	stGridColor.clrGridHeader = GetAxColor(74);
	stGridColor.clrGridHilight = GetAxColor(78);

	ChangeGridColor(stGridColor);

	Invalidate();	
}

void CMainFrmWnd::ChangeGridColor(ST_GRIDCOLOR stGridColor)
{
	m_ctrlGrid.ChangeGridColor(stGridColor);
	m_ctrlGrid.Invalidate();
}

void CMainFrmWnd::OnBtnFieldEdit()
{
	HINSTANCE hInstSave = AfxGetResourceHandle();
	AfxSetResourceHandle(g_hInstance);
	
	CFieldEditDlg dlg(this, 0);

	if (dlg.DoModal() == IDOK)
	{
		SetSaveFixUserFieldData(&m_listFixHeadUser, m_strFixFieldVersion);

		SetGrid_Head();
		SetGrid_Head(TRUE);
		/*OnBtnSearch();*/
	}

	AfxSetResourceHandle(hInstSave);
}

BOOL CMainFrmWnd::SetSaveFixUserFieldData(CList<HeadFixInfo, HeadFixInfo&> *plistFixUser,
										  CString strVersion)
{
	BOOL bResult = TRUE;

	GETPATH_APP(strRoot);
	CString strFileRoot = strRoot + FOLDER_DATA + "\\" + FIXFIELD_USER_TRCONVOL;

	MSXML::IXMLDOMNodePtr						pNIndex;
	MSXML::IXMLDOMNamedNodeMapPtr		pNAIndex;

	MSXML::IXMLDOMNodePtr						pN_FItem;
	MSXML::IXMLDOMNamedNodeMapPtr		pNA_FItem;

	try 
	{
		CString strTemp;

		MSXML::IXMLDOMDocumentPtr pDOMIndex ("Microsoft.XMLDOM", NULL, CLSCTX_INPROC_SERVER);

		MSXML::IXMLDOMProcessingInstructionPtr pPI;
		MSXML::IXMLDOMNamedNodeMapPtr					pNAPI;
		MSXML::IXMLDOMNodePtr									pNEncoding;

		pPI = pDOMIndex->createProcessingInstruction("xml", "version=\"1.0\"");

		pNEncoding = pDOMIndex->createAttribute("encoding");
		pNEncoding->text = "KSC_5601";

		pNAPI = pPI->attributes;
		pNAPI->setNamedItem(pNEncoding);

		pDOMIndex->appendChild(pPI);

		pNIndex = pDOMIndex->createElement("INDEX");
		pNAIndex = pNIndex->attributes;

		MSXML::IXMLDOMNodePtr pNAttributeTemp;
		pNAttributeTemp = pDOMIndex->createAttribute("VERSION");
		pNAttributeTemp->text = (LPCTSTR)strVersion;
		pNAIndex->setNamedItem(pNAttributeTemp);

		pNAttributeTemp = pDOMIndex->createAttribute("FIX_COL");
		strTemp.Format("%d", m_nFixCol);
		pNAttributeTemp->text = (LPCTSTR)strTemp;
		pNAIndex->setNamedItem(pNAttributeTemp);

		pDOMIndex->appendChild(pNIndex);

		POSITION pos;
		HeadFixInfo	stHeadFixInfo;
		int nFieldCount = plistFixUser->GetCount();
		for (int nIndex = 0;  nIndex < nFieldCount; nIndex++)	
		{
			pos = plistFixUser->FindIndex(nIndex);
			stHeadFixInfo = plistFixUser->GetAt(pos);

			MSXML::IXMLDOMNodePtr pNAttributeTemp;

			pN_FItem = pDOMIndex->createElement("FITEM");
			pNIndex->appendChild(pN_FItem);
			pNA_FItem = pN_FItem->attributes;

			//ID
			pNAttributeTemp = pDOMIndex->createAttribute("ID");
			strTemp.Format("%d", stHeadFixInfo.stHeadType.lHeadID);
			pNAttributeTemp->text = (LPCTSTR)strTemp;
			pNA_FItem->setNamedItem(pNAttributeTemp);

			//COL_NO
			pNAttributeTemp = pDOMIndex->createAttribute("COL_NO");
			strTemp.Format("%d", stHeadFixInfo.nColNum);
			pNAttributeTemp->text = (LPCTSTR)strTemp;
			pNA_FItem->setNamedItem(pNAttributeTemp);

			//FIELD_NAME
			pNAttributeTemp = pDOMIndex->createAttribute("FIELD_NAME");
			pNAttributeTemp->text = (LPCTSTR)stHeadFixInfo.stHeadType.strHeadName;;
			pNA_FItem->setNamedItem(pNAttributeTemp);

			//UNIT
			pNAttributeTemp = pDOMIndex->createAttribute("UNIT");
			pNAttributeTemp->text = (LPCTSTR)"1";
			pNA_FItem->setNamedItem(pNAttributeTemp);

			//ALIGN
			pNAttributeTemp = pDOMIndex->createAttribute("ALIGN");
			pNAttributeTemp->text = (LPCTSTR)stHeadFixInfo.stHeadType.strAlignment;
			pNA_FItem->setNamedItem(pNAttributeTemp);

			//COL_WIDTH
			pNAttributeTemp = pDOMIndex->createAttribute("COL_WIDTH");
			strTemp.Format("%d", stHeadFixInfo.stHeadType.nColWidth);
			pNAttributeTemp->text = (LPCTSTR)strTemp;
			pNA_FItem->setNamedItem(pNAttributeTemp);

			//DATA_TYPE
			pNAttributeTemp = pDOMIndex->createAttribute("DATA_TYPE");
			pNAttributeTemp->text = (LPCTSTR)stHeadFixInfo.stHeadType.strDataType;
			pNA_FItem->setNamedItem(pNAttributeTemp);

			//VIEW_NAME
			pNAttributeTemp = pDOMIndex->createAttribute("VIEW_NAME");
			pNAttributeTemp->text = (LPCTSTR)stHeadFixInfo.stHeadType.strHeadName;
			pNA_FItem->setNamedItem(pNAttributeTemp);

			//VISIBLE
			pNAttributeTemp = pDOMIndex->createAttribute("VISIBLE");
			strTemp = (stHeadFixInfo.bView == TRUE)?"TRUE":"FALSE";
			pNAttributeTemp->text =	(LPCTSTR)strTemp;
			pNA_FItem->setNamedItem(pNAttributeTemp);

			//UNIT_NUM
			pNAttributeTemp = pDOMIndex->createAttribute("UNIT_NUM");
			strTemp.Format("%d", stHeadFixInfo.stHeadType.nUnitNum);
			pNAttributeTemp->text = (LPCTSTR)strTemp;
			pNA_FItem->setNamedItem(pNAttributeTemp);

			//UNIT_TEXT
			pNAttributeTemp = pDOMIndex->createAttribute("UNIT_TEXT");
			pNAttributeTemp->text = (LPCTSTR)stHeadFixInfo.stHeadType.strUnitText;
			pNA_FItem->setNamedItem(pNAttributeTemp);

			//DATA_LENGTH
			pNAttributeTemp = pDOMIndex->createAttribute("DATA_LENGTH");
			strTemp.Format("%d", stHeadFixInfo.stHeadType.nDataLength);
			pNAttributeTemp->text = (LPCTSTR)strTemp;
			pNA_FItem->setNamedItem(pNAttributeTemp);
		}

		pDOMIndex->save(_bstr_t(strFileRoot));
	}	
	catch(_com_error e)
	{
		bResult = FALSE;
	}

	return bResult;
}

BOOL CMainFrmWnd::GetLoadFixUserFieldData(MSXML::IXMLDOMNodePtr pNIndex,
										  CList<HeadFixInfo, HeadFixInfo&> &listFixList)
{
	BOOL bResult = TRUE;

	MSXML::IXMLDOMNamedNodeMapPtr	pNAIndex;

	MSXML::IXMLDOMNodeListPtr			pNS_FItem;		// Item List
	MSXML::IXMLDOMNodePtr					pN_FItem;	// Item
	MSXML::IXMLDOMNamedNodeMapPtr	pNA_FItem;		// Item Attribute

	CString							strView;
	CString							strViewName;
	try 
	{
		MSXML::IXMLDOMNodePtr	pNAttributeTemp;
		pNAIndex = pNIndex->attributes;

		pNAttributeTemp = pNAIndex->getNamedItem("VERSION");
		//	m_strFileVersion = CString((PCHAR)pNAttributeTemp->text);			

		pNAttributeTemp = pNAIndex->getNamedItem("FIX_COL");
		m_nFixCol = atoi(CString((PCHAR)pNAttributeTemp->text));

		pNS_FItem  = pNIndex->selectNodes("FITEM");

		//Attribute를 얻어온다
		int nCount, nLength;
		nLength = pNS_FItem->length;
		for (nCount = 0; nCount < nLength; nCount++)
		{
			HeadFixInfo stHeadFixInfo;

			//node를 얻어온다.
			pN_FItem = pNS_FItem->item[nCount];	

			//속성을 얻어온다.
			MSXML::IXMLDOMNodePtr	pNAttributeTemp;
			pNA_FItem = pN_FItem->attributes;

			//ID
			pNAttributeTemp = pNA_FItem->getNamedItem("ID");
			stHeadFixInfo.stHeadType.lHeadID = atol(CString((PCHAR)pNAttributeTemp->text));

			//COL_NO
			pNAttributeTemp = pNA_FItem->getNamedItem("COL_NO");
			stHeadFixInfo.nColNum = atoi(CString((PCHAR)pNAttributeTemp->text));

			//FIELD_NAME
			pNAttributeTemp = pNA_FItem->getNamedItem("FIELD_NAME");
			stHeadFixInfo.stHeadType.strHeadName = CString((PCHAR)pNAttributeTemp->text);

			//UNIT_NUM
			pNAttributeTemp = pNA_FItem->getNamedItem("UNIT_NUM");
			stHeadFixInfo.stHeadType.nUnitNum = atoi(CString((PCHAR)pNAttributeTemp->text));

			//UNIT_TEXT
			pNAttributeTemp = pNA_FItem->getNamedItem("UNIT_TEXT");
			stHeadFixInfo.stHeadType.strUnitText = CString((PCHAR)pNAttributeTemp->text);

			//ALIGN
			pNAttributeTemp = pNA_FItem->getNamedItem("ALIGN");
			stHeadFixInfo.stHeadType.strAlignment = CString((PCHAR)pNAttributeTemp->text);

			//COL_WIDTH
			pNAttributeTemp = pNA_FItem->getNamedItem("COL_WIDTH");
			stHeadFixInfo.stHeadType.nColWidth = atoi(CString((PCHAR)pNAttributeTemp->text));

			//DATA_TYPE
			pNAttributeTemp = pNA_FItem->getNamedItem("DATA_TYPE");
			stHeadFixInfo.stHeadType.strDataType = CString((PCHAR)pNAttributeTemp->text);

			//DATA_LENGTH
			pNAttributeTemp = pNA_FItem->getNamedItem("DATA_LENGTH");
			stHeadFixInfo.stHeadType.nDataLength = atoi(CString((PCHAR)pNAttributeTemp->text));


			//VIEW_NAME
			pNAttributeTemp = pNA_FItem->getNamedItem("VIEW_NAME");
			strViewName = CString((PCHAR)pNAttributeTemp->text);

			//VISIBLE
			pNAttributeTemp = pNA_FItem->getNamedItem("VISIBLE");
			stHeadFixInfo.bView = (CString((PCHAR)pNAttributeTemp->text) == "TRUE")?TRUE:FALSE;

			listFixList.AddTail(stHeadFixInfo);
		}
	}
	catch(_com_error e) 
	{
		CString strMsg = "사용자 파일을 읽어오는 중 에러가 발생하였습니다. 고객지원센터로 연락 바랍니다.";
		//SetSendMainMsg(strMsg);
		bResult = FALSE;
	}

	return bResult;
}

void CMainFrmWnd::SetLoad_FieldInfo()
{
	MSXML::IXMLDOMDocumentPtr pXmlDom ("Microsoft.XMLDOM", NULL, CLSCTX_INPROC_SERVER);

	CString strFileRoot;
	GETPATH_APP(strRoot);
	strFileRoot = strRoot + FOLDER_DATA + "\\" + FIXFIELD_SYS_FILE;

	if (GetLoadXml(strFileRoot, pXmlDom) == TRUE)
	{
		GetLoadFixSysFieldData(pXmlDom->selectSingleNode("INDEX"), m_listFixHeadSys);
		m_strFixFieldVersion = GetVersion_FieldFile(pXmlDom->selectSingleNode("INDEX"));
	}

	BOOL bSetInit = TRUE;
	strFileRoot = strRoot + FOLDER_DATA + "\\" + FIXFIELD_USER_TRCONVOL;
	if (GetLoadXml(strFileRoot, pXmlDom) == TRUE)
	{
		//# Fix Field User Version Check!
		if (m_strFixFieldVersion == GetVersion_FieldFile(pXmlDom->selectSingleNode("INDEX")))
		{
			GetLoadFixUserFieldData(pXmlDom->selectSingleNode("INDEX"), m_listFixHeadUser);

			if (m_listFixHeadUser.GetCount() == 0)
				bSetInit = FALSE;
			else
				SetUpdateUserFieldData(&m_listFixHeadSys, &m_listFixHeadUser);		//# Info UpDate!

			bSetInit = FALSE;
		}
	}

	if (bSetInit == TRUE)
	{
// 		m_nFixCol = 2;
// 		m_listFixHeadUser.RemoveAll();
// 
// 		CStringArray saFieldId;
// 		if (GetLoadFixField_Def(saFieldId) == TRUE)
// 		{
// 			POSITION pos;
// 			HeadFixInfo		stHeadFixInfo;
// 			HeadInfoType	stHeadInfoType;
// 			for (int nIndex = 0; nIndex < m_listFixHeadSys.GetCount(); nIndex++)
// 			{
// 				pos = m_listFixHeadSys.FindIndex(nIndex);
// 				stHeadInfoType = m_listFixHeadSys.GetAt(pos);
// 
// 				CString strFieldId;
// 				for (int nItem = 0; nItem < saFieldId.GetSize(); nItem++)
// 				{
// 					strFieldId = saFieldId.GetAt(nItem);
// 					if (stHeadInfoType.lHeadID == atol(strFieldId))
// 					{
// 						stHeadFixInfo.stHeadType = stHeadInfoType;
// 						stHeadFixInfo.nColNum = m_listFixHeadUser.GetCount();
// 						stHeadFixInfo.bView = TRUE;
// 
// 						m_listFixHeadUser.AddTail(stHeadFixInfo);
// 						break;
// 					}
// 				}
// 			}
// 		}
	}

	SetSaveFixUserFieldData(&m_listFixHeadUser, m_strFixFieldVersion);
}

BOOL CMainFrmWnd::GetLoadXml(CString strFileRoot, MSXML::IXMLDOMDocumentPtr &pXmlDom)
{
	char msg[500];
	CString strDebug;

	pXmlDom->async = false;
	if (!pXmlDom->load (strFileRoot.AllocSysString())) 
	{	
		MSXML::IXMLDOMParseErrorPtr pParsingErr = pXmlDom->parseError;			
		sprintf(msg, "Error 0x%.8X on line %d, position %d\r\n", 
			pParsingErr->errorCode,pParsingErr->line, pParsingErr->linepos);

		CString strTemp(BSTR(pParsingErr->reason));	
		strDebug = strTemp + CString(msg) + "\n" + strFileRoot;

		//SetMessageBox(strDebug, "[XML Files Error]", MB_OK);		
		return FALSE;
	}

	return TRUE;
}

void CMainFrmWnd::SetGrid_Head(BOOL bView /*= FALSE*/)
{
	int nCol;
	POSITION pos;

	if (bView == TRUE)
	{
		m_ctrlGrid.SetHeader(&m_listField);
		m_ctrlGrid.m_plistField = &m_listField;

		m_ctrlGrid.SetLockColumns(m_nFixCol, TRUE);	
	}
	else if (bView == FALSE)
	{
		m_listField.RemoveAll();

		HeadFixInfo	stHeadFixInfo;
		for (nCol = 0; nCol < m_listFixHeadUser.GetCount(); nCol++)
		{
			pos = m_listFixHeadUser.FindIndex(nCol);
			stHeadFixInfo = m_listFixHeadUser.GetAt(pos);

			if (stHeadFixInfo.bView == TRUE)
				m_listField.AddTail(stHeadFixInfo.stHeadType);
		}
	}
}

BOOL CMainFrmWnd::GetLoadFixSysFieldData(MSXML::IXMLDOMNodePtr pNIndex,
										 CList<HeadInfoType, HeadInfoType&> &listFixList)
{
	BOOL bResult = TRUE;

	MSXML::IXMLDOMNodeListPtr		pNS_FItem;	// Item List
	MSXML::IXMLDOMNodePtr				pN_FItem;	// Item
	MSXML::IXMLDOMNamedNodeMapPtr	pNA_FItem;	// Item Attribute 	

	try
	{
		pNS_FItem  = pNIndex->selectNodes("FITEM");

		//System의 version은 읽어올 필요없다
		//해당 모듈이 로드되기 전에 이미 version처리 되어진다.
		int nCount, nLength;
		nLength = pNS_FItem->length;
		for (nCount = 0; nCount < nLength; nCount++)
		{
			HeadInfoType stHeadInfoType;

			//node를 얻어온다.
			pN_FItem = pNS_FItem->item[nCount];	

			//속성을 얻어온다.
			MSXML::IXMLDOMNodePtr	pNAttributeTemp;
			pNA_FItem= pN_FItem->attributes;

			//ID
			pNAttributeTemp = pNA_FItem->getNamedItem("ID");
			stHeadInfoType.lHeadID = atol(CString((PCHAR)pNAttributeTemp->text));

			//FIELD_NAME
			pNAttributeTemp = pNA_FItem->getNamedItem("FIELD_NAME");
			stHeadInfoType.strHeadName = CString((PCHAR)pNAttributeTemp->text);

			//ALIGN
			pNAttributeTemp = pNA_FItem->getNamedItem("ALIGN");
			stHeadInfoType.strAlignment = CString((PCHAR)pNAttributeTemp->text);

			//COL_WIDTH
			pNAttributeTemp = pNA_FItem->getNamedItem("COL_WIDTH");
			stHeadInfoType.nColWidth = atoi(CString((PCHAR)pNAttributeTemp->text));

			//DATA_TYPE
			pNAttributeTemp = pNA_FItem->getNamedItem("DATA_TYPE");
			stHeadInfoType.strDataType = CString((PCHAR)pNAttributeTemp->text);

			//UNIT_NUM
			pNAttributeTemp = pNA_FItem->getNamedItem("UNIT_NUM");
			stHeadInfoType.nUnitNum = atoi(CString((PCHAR)pNAttributeTemp->text));

			//UNIT_TEXT
			pNAttributeTemp = pNA_FItem->getNamedItem("UNIT_TEXT");
			stHeadInfoType.strUnitText = CString((PCHAR)pNAttributeTemp->text);

			//DATA_LENGTH
			pNAttributeTemp = pNA_FItem->getNamedItem("DATA_LENGTH");
			stHeadInfoType.nDataLength = atoi(CString((PCHAR)pNAttributeTemp->text));

			//			//UNIT
			//			pNAttributeTemp = pNA_FItem->getNamedItem("UNIT");
			//			stHeadInfoType.nDataLength = atoi(CString((PCHAR)pNAttributeTemp->text));

			// 시스템용 리스트에 추가
			listFixList.AddTail(stHeadInfoType);
		}
	}
	catch(_com_error e)
	{
		CString strMsg = "시스템 파일을 읽어오는 중 에러가 발생하였습니다. 고객지원센터로 연락 바랍니다.";
		//SetSendMainMsg(strMsg);
		bResult = FALSE;
	}

	return bResult;
}

CString CMainFrmWnd::GetVersion_FieldFile(MSXML::IXMLDOMNodePtr pNIndex)
{
	CString strResult = XML_VERSION_FIXFIELDFILE;

	MSXML::IXMLDOMNamedNodeMapPtr	pNAIndex;
	try 
	{
		MSXML::IXMLDOMNodePtr	pNAttributeTemp;
		pNAIndex = pNIndex->attributes;

		pNAttributeTemp = pNAIndex->getNamedItem("VERSION");
		strResult = CString((PCHAR)pNAttributeTemp->text);
	}
	catch(_com_error e) 
	{
		strResult = "";
	}

	return strResult;
}

void CMainFrmWnd::SetUpdateUserFieldData(CList<HeadInfoType, HeadInfoType&> *plistFixSys, 
										 CList<HeadFixInfo, HeadFixInfo&> *plistFixUser)
{
	POSITION	posSys, posUser;
	HeadInfoType stHeadInfoType;
	HeadFixInfo	 stHeadFixInfo;

	int nSysCnt = plistFixSys->GetCount();
	int nUserCnt = plistFixUser->GetCount();
	for (int nSysIndex = 0; nSysIndex < nSysCnt; nSysIndex++)
	{
		posSys = plistFixSys->FindIndex(nSysIndex);
		stHeadInfoType = plistFixSys->GetAt(posSys);

		for (int nUserIndex = 0; nUserIndex < nUserCnt; nUserIndex++)
		{
			posUser = plistFixUser->FindIndex(nUserIndex);
			stHeadFixInfo = plistFixUser->GetAt(posUser);

			if (stHeadInfoType.lHeadID == stHeadFixInfo.stHeadType.lHeadID)
			{
				if (stHeadFixInfo.stHeadType.strHeadName.Compare(stHeadInfoType.strHeadName) == -1)
					stHeadFixInfo.stHeadType.strHeadName = stHeadInfoType.strHeadName;

				stHeadFixInfo.stHeadType.strDataType = stHeadInfoType.strDataType;
				stHeadFixInfo.stHeadType.nUnitNum	= stHeadInfoType.nUnitNum;
				stHeadFixInfo.stHeadType.strUnitText = stHeadInfoType.strUnitText;
				stHeadFixInfo.stHeadType.nDataLength = stHeadInfoType.nDataLength;
				stHeadFixInfo.stHeadType.lHeadID = stHeadInfoType.lHeadID;
				stHeadFixInfo.stHeadType.strAlignment	= stHeadInfoType.strAlignment;
				stHeadFixInfo.stHeadType.nColWidth = stHeadInfoType.nColWidth;
				//				//stHeadFixInfo.strUNIT						= stHeadInfoType.strUNIT;
				//				//stHeadFixInfo.strVISIBLE					= stHeadInfoType.strVISIBLE;

				plistFixUser->SetAt(posUser, stHeadFixInfo);
			}
		}
	}
}

BOOL CMainFrmWnd::GetLoadFixField_Def(CStringArray &saFieldId)
{
	CFileFind cFind;
	BOOL bRead = TRUE;

	CString strFilePath = GetFilePath("FINDERINFO");

	bRead = cFind.FindFile(strFilePath);
	if (!bRead)
	{
		CString strMsg = (CString)FINDERINFO_INIFILE + "을 찾지 못했습니다.";
	}
	else
	{
		char chColID[10];
		memset(chColID, ' ', sizeof(chColID));

		int nCount = GetPrivateProfileInt("DefaultColumn", "Line Count", 0, strFilePath);
		CString strTemp;
		for (int nIndex = 0; nIndex < nCount; nIndex++)
		{
			strTemp.Format("%d", nIndex + 1);
			GetPrivateProfileString("DefaultColumn", strTemp, "", chColID, sizeof(chColID), strFilePath);
			saFieldId.Add(chColID);
		}
	}

	return bRead;
}

int CMainFrmWnd::GetCount_FixHead(BOOL &bHaveCodeFiled, CUIntArray &iaFixHeadId)
{
	int nResult = 0;

	bHaveCodeFiled = FALSE;
	iaFixHeadId.RemoveAll();

	POSITION pos;
	HeadFixInfo		stHeadFixInfo;
	for (int nIndex = 0; nIndex < m_listFixHeadUser.GetCount(); nIndex++)
	{
		pos = m_listFixHeadUser.FindIndex(nIndex);
		stHeadFixInfo = m_listFixHeadUser.GetAt(pos);

		if (stHeadFixInfo.bView == TRUE)
		{
			if (stHeadFixInfo.stHeadType.strDataType == "code")
				bHaveCodeFiled = TRUE;
			else
				iaFixHeadId.Add(stHeadFixInfo.stHeadType.lHeadID);

			nResult++;
		}
		else
		{
			if (stHeadFixInfo.stHeadType.strHeadName == (CString)GRID_MOVEFIELD_POS_NAME)
				iaFixHeadId.Add(stHeadFixInfo.stHeadType.lHeadID);
		}
	}

	return nResult;
}

void CMainFrmWnd::GetTabClr(COLORREF* clrBkgnd, ST_TABCOLOR* stTabColor)
{
	*clrBkgnd = m_clrBkgnd;
	
	if(stTabColor)
		*stTabColor = m_stTabColor;
}

// void CMainFrmWnd::SetCodeInfo(CCodeCtrl *pSelectJCodeDisp, HWND hSelectControl, 
// 							  CCodeCtrl *pExceptJCodeDisp, HWND hExceptControl)
// {
// 	m_pctrlSelectCodeInput	= pSelectJCodeDisp;
// 	m_hSelectCodeHwnd		= hSelectControl;
// 
// 	m_pctrlExceptCodeInput	= pExceptJCodeDisp;
// 	m_hExceptCodeHwnd		= hExceptControl;
// }
// 
// void CMainFrmWnd::SetBtnInfo(CfxImgButton* pBtnAdd, HWND hBtnAdd,
// 							 CfxImgButton* pBtnRemove, HWND hbtnRemove,
// 							 CfxImgButton* pBtnRemoveAll, HWND hbtnRemoveAll)
// {
// 	m_pBtnAdd			= pBtnAdd;
// 	m_hBtnAdd			= hBtnAdd;
// 	
// 	m_pBtnRemove		= pBtnRemove;
// 	m_hBtnRemove		= hbtnRemove;
// 	
// 	m_hBtnRemoveAll		= hbtnRemoveAll;
// 	m_pBtnRemoveAll		= pBtnRemoveAll;
// }

void CMainFrmWnd::OnBtnAdd(CString szItemCode, CString szItemName)
{
	if(!m_pDlgFilterSet)
		return;

// 	int nIndexItemCode = 0;
// 	int nIndexItemName = 0;
// 
// 	nIndexItemCode = m_arItemCode.GetCount();
// 	nIndexItemName = m_arItemName.GetCount();
// 
// 	if(nIndexItemCode != nIndexItemName)
// 		g_iMetaTable.ShowMessageBox(m_hWnd, "err!");
// 
// 	for(int i = 0; i < nIndexItemCode; i++)
// 	{
// 		if(szItemCode == m_arItemCode.GetAt(i))
// 			return;
// 	}

// 	m_arItemCode.Add(szItemCode);
// 	m_arItemName.Add(szItemName);
	
	m_pDlgFilterSet->AddExceptItem(szItemCode, szItemName);
}

void CMainFrmWnd::OnBtnRemove()
{
	if(!m_pDlgFilterSet)
		return;

	m_pDlgFilterSet->RemoveExceptItem();
}

void CMainFrmWnd::OnBtnRemoveAll()
{
	if(!m_pDlgFilterSet)
		return;

	m_pDlgFilterSet->RemoveAllExceptItem();
}

CString CMainFrmWnd::GetFilePath(CString strLoadType)
{
	CString strFile, strFolder;
	CString strFilePath = "";

	BOOL bUserFolder = FALSE;
	BOOL bSetIndustry = TRUE;

	strFile = (CString)FINDERINFO_INIFILE;

	if (bUserFolder == TRUE)
	{
		CString strUserPath;
		strUserPath.Format("%s%s%s\\%s_%s", Variant(homeCC), USRDIR, Variant(nameCC), FOLDER_FINDDATA, CONVOL_FILE);

		strFilePath = (CString)strUserPath + (CString)FOLDER_FINDDATA + "\\" + strFile;
	}
	else
	{
		GETPATH_APP(strAppPath);
		strFilePath = strAppPath + (CString)FOLDER_DATA + "\\" + strFile;
	}

	return strFilePath;
}

LRESULT CMainFrmWnd::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	if(message == WMU_FILTER_DLG)
	{
		if(m_pDlgFilterSet)
		{
			if(IsWindow(m_pDlgFilterSet->GetSafeHwnd()))
				m_pDlgFilterSet->DestroyWindow();
			delete m_pDlgFilterSet;
			m_pDlgFilterSet = NULL;
			GetParent()->SendMessage(RMSG_KSMOVECTRL, (WPARAM)FALSE, 0);
		}
	}

	return CFrameWnd::WindowProc(message, wParam, lParam);
}

HRESULT CMainFrmWnd::ReceiveData(LPCSTR szTR, LPVOID aTRData, long dwTRDateLen)
{
	HRESULT dwResult = 0;
	
	CString strData;
	if (dwTRDateLen == 0)		//# 0 : err
	{
		return S_OK;
	}
	else if(!strcmp(szTR, TRNO_SET_REAL))
	{
		/*SetAdvData_Alert(TRUE);*/
	}
	else if(strcmp(szTR, QUERY_STRJONGMOK) == 0)//차트 TR
	{
		// 		KB_p0602_OutRec1_EX *pStOutput = (KB_p0602_OutRec1_EX *)aTRData;
		// 
		// 		{ //종목명표시.
		// 			CString szCodeName;
		// 			szCodeName.Format("%*.*s", sizeof(pStOutput->name), sizeof(pStOutput->name), pStOutput->name);
		// 			szCodeName.TrimRight();
		// 			m_chartCodeName.SetWindowText(szCodeName);
		// 		}
		// 
		// 		long lPosForChart = 0;
		// 		lPosForChart = dwTRDateLen - (&pStOutput->fcnt[0] - &pStOutput->name[0]);
		// 		
		// 
		// 		if (m_Chart)
		// 		{
		// 			m_Chart.RemoveRealItem();	
		// 
		// 			m_Chart.AddPacketAndGraph();
		// 			m_Chart.SetPacketData(m_Chart.GetPacketName(),(long)&pStOutput->fcnt,lPosForChart,TRUE);
		// 
		// 			m_Chart.SetDrdsAdvise(FALSE);
		// 			m_Chart.AddRealItem(m_strCode);
		// 			m_Chart.SetDrdsAdvise(TRUE);
		// 
		// 			CString strDateTime;
		// 			//BOOL bLoad = m_Chart.InvokeAddInStr("ChartPeriodSummaryAddIn.dll:DRAG_ZOOM_ANAL", "0", strDateTime);
		// 	
		// 			INT_CONVERT(pBuff, pStOutput->fcnt, sizeof(pStOutput->fcnt), nData);
		// 			m_Chart.SetOnePageDataCount(min(120, nData));
		//		}
		
		//	Invalidate();
	}
	return S_OK;
}

CFont* CMainFrmWnd::GetFont(BOOL bBold)
{
	CScreenWnd* pScreenWnd = (CScreenWnd*)GetParent();
	
	CFont* pFont;
	
	if(!bBold)
		pFont = pScreenWnd->GetDefFont();
	else
		pFont = pScreenWnd->GetBoldFont();
	
	return pFont;
}

COLORREF CMainFrmWnd::GetColor(int nColor)
{
	CScreenWnd* pScreenWnd = (CScreenWnd*)GetParent();
	
	return pScreenWnd->GetAxColor(nColor);
}

CString CMainFrmWnd::Variant(int nComm, CString strData)
{
	CScreenWnd* pScreenWnd = (CScreenWnd*)GetParent();
	
	return pScreenWnd->Variant(nComm, strData);
}

void CMainFrmWnd::SetBtnImg(CfxImgButton *pBtn, CString strFileName)
{
	CScreenWnd* pScreenWnd = (CScreenWnd*)GetParent();
	pScreenWnd->SetBtnImg(pBtn, strFileName);
}

void CMainFrmWnd::SetBtnImg(CfxImgButton *pBtn, int nTextLen, BOOL bBold)
{
	CScreenWnd* pScreenWnd = (CScreenWnd*)GetParent();
	pScreenWnd->SetBtnImg(pBtn, nTextLen, bBold);
}

COLORREF CMainFrmWnd::GetAxColor(int nColor)
{
	CScreenWnd* pScreenWnd = (CScreenWnd*)GetParent();
	return pScreenWnd->GetAxColor(nColor);
}

HBRUSH CMainFrmWnd::GetAxBrush(COLORREF clr)
{
	CScreenWnd* pScreenWnd = (CScreenWnd*)GetParent();
	return (HBRUSH)(*pScreenWnd->GetAxBrush(clr));
}

int CMainFrmWnd::SetMessageBox(LPCTSTR lpszText, LPCTSTR lpszCaption, UINT nType)
{
	return ::MessageBox(m_hWnd, lpszText, lpszCaption, nType);
}

CWnd* CMainFrmWnd::GetModuleMain()
{
	CScreenWnd* pScreenWnd = (CScreenWnd*)GetParent();
	return pScreenWnd->GetModuleMainWnd();
}

void CMainFrmWnd::SetSharedDataToCode(CString strCode)
{
	CScreenWnd* pScreenWnd = (CScreenWnd*)GetParent();
	pScreenWnd->ShareCode(strCode);
}

void CMainFrmWnd::OnClickbtnErase()
{
	ClearAll();
}

void CMainFrmWnd::OnClickbtnFieldSet()
{
	FieldSet();
}

void CMainFrmWnd::OnClickbntConditionSet()
{
	ChangeConditionSet();
}

void CMainFrmWnd::OnClickbtnWatchSet()
{
	m_bWatching = !m_bWatching;
	
	if(m_bWatching)
	{
		m_btnWatchSet.SetTextColor(RGB(255, 0, 0));
		m_btnWatchSet.SetWindowText("해제");
	}
	else
	{
		m_btnWatchSet.SetTextColor(RGB(0, 0, 0));
		m_btnWatchSet.SetWindowText("감시");
	}
	
	Modual_SendReal(m_bWatching, "0000");
}

// 2012.4.5 박성경: winix플랫폼 R메뉴 호출 >>
void CMainFrmWnd::OnRButtonClick(CPoint *point, CString strCode /*= ""*/)
{
	CWnd*		pModuleMainWnd;
	pModuleMainWnd = GetModuleMain();
	GetCursorPos(point);
	ScreenToClient(point);
	ClientToScreen(point);
	
	CString strLPARM;
	strLPARM.Format("%s\n%d\t%d\n1301\t%s\n", "IB713200", point->x, point->y, strCode);
	// mapN \n pt.x \t pt.y \n SYM \t DATA \n
	pModuleMainWnd->SendMessage(WM_USER, MAKEWPARAM(mouseDLL, FALSE), (LPARAM)strLPARM.operator LPCTSTR());
}
// 2012.4.5 박성경: winix플랫폼 R메뉴 호출 <<

BOOL CMainFrmWnd::GetStockMasterData(CStringArray &arrCode, CStringArray &arrName, UINT Jgub, UINT Upgub)
{
	CScreenWnd* pScreenWnd = (CScreenWnd*)GetParent();
	return pScreenWnd->GetStockMasterData(arrCode, arrName, Jgub, Upgub);
}

void CMainFrmWnd::OnTimer(UINT nIDEvent) 
{
	DWORD MicroSec = m_TimeEstimate.EndEstimate();

	if(MicroSec > 800)
		m_bTimerChk = TRUE;
	else
	{
		m_bTimerChk = FALSE;

		if(m_listTempStConVol.GetCount() > 0)
		{
			POSITION pos;
			BOOL bInsert = (!m_stConVolSet.nItemPosition) ? TRUE : FALSE;

			for(int nIndex = 0; nIndex < m_listTempStConVol.GetCount(); nIndex++)
			{
				StConVolClient stConVolClient;
				pos = m_listTempStConVol.FindIndex(nIndex);
				stConVolClient = m_listTempStConVol.GetAt(pos);	
				m_ctrlGrid.SetAddRow(stConVolClient, bInsert, TRUE);
			}
			m_ctrlGrid.RedrawAll();
			m_listTempStConVol.RemoveAll();

			if(!bInsert)
				m_ctrlGrid.SetScrollPos(SB_VERT, m_ctrlGrid.GetNumberRows());
		}
	}

	m_TimeEstimate.StartEstimate();

	CFrameWnd::OnTimer(nIDEvent);
}
