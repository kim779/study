// ConditionSetWnd.cpp : implementation file
//
#include "stdafx.h"
#include "resource.h"
#include "MainFrmWnd.h"
#include "ConditionSetWnd.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern HINSTANCE g_hInstance;

#define MAXCNT_REGOK        30

/////////////////////////////////////////////////////////////////////////////
// CConditionSetWnd
CConditionSetWnd::CConditionSetWnd()
{
	LOGFONT	  logFont;
	GETFONT(logFont, 12, 400, m_fontDefault);
	GETFONT(logFont, 12, 600, m_fontBold);
	
	m_brushStatic.CreateSolidBrush(COLOR_SETWND_FACE);
	m_brushSubTitlePoint.CreateSolidBrush(COLOR_SUBTITLE_POINT);

	m_bShow_PF_CTL = TRUE;

	m_bHelpShowFlag = FALSE;
}

CConditionSetWnd::~CConditionSetWnd()
{
	m_fontDefault.DeleteObject();
	m_fontBold.DeleteObject();
	
	m_brushStatic.DeleteObject();
	m_brushSubTitlePoint.DeleteObject();
}


BEGIN_MESSAGE_MAP(CConditionSetWnd, CWnd)
//{{AFX_MSG_MAP(CConditionSetWnd)
ON_WM_CREATE()
ON_WM_SIZE()
ON_WM_CTLCOLOR()
ON_WM_PAINT()
ON_WM_DESTROY()
ON_WM_DRAWITEM()
//}}AFX_MSG_MAP

ON_BN_CLICKED(IDC_BTN_LEFT, OnBtnLeft)
ON_BN_CLICKED(IDC_BTN_LEFTALL, OnBtnLeftAll)
ON_BN_CLICKED(IDC_BTN_RIGHT, OnBtnRight)
ON_BN_CLICKED(IDC_BTN_RIGHTALL, OnBtnRightAll)
ON_BN_CLICKED(IDC_BTN_HELPSHOW, OnBtnShowHelp)

ON_MESSAGE(WM_NOTIFY_TREE, OnNotify_Tree)
ON_MESSAGE(WM_NOTIFY_SIGNALGRID, OnNotify_SignalGrid)

ON_WM_ERASEBKGND()
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CConditionSetWnd message handlers

BOOL CConditionSetWnd::Create(LPCTSTR lpszWindowName, int nType, CWnd* pParentWnd, UINT nID) 
{
	BOOL bReault;
	bReault = CWnd::Create(_T("STATIC"), lpszWindowName, WS_CHILD | WS_VISIBLE,
		CRect(0, 0, 0, 0), pParentWnd, nID);
	
	m_nType = nType;
	
	return bReault;
}

int CConditionSetWnd::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	HINSTANCE hInstSave = AfxGetResourceHandle();
	AfxSetResourceHandle(g_hInstance);
	
	CMainFrmWnd *pwndMainFrm = (CMainFrmWnd*)GetParent();
	
	m_ctrlLeftTitlePoint.Create(_T(""), WS_CHILD | WS_VISIBLE | WS_BORDER, 
		CRect(0, 0, 0, 0), this, IDC_SIATIC_SUBTITLEPOINT);
	
	m_ctrlLeftTitle.Create(_T("적용가능항목"), WS_CHILD | WS_VISIBLE | SS_LEFT, 
		CRect(0, 0, 0, 0), this, IDC_SIATIC_SUBTITLE);
	m_ctrlLeftTitle.SetFont(&m_fontBold);
	
	
	m_treeCondition.Create(this, IDC_TREE_CONDITION);
	m_treeCondition.SetFont(&m_fontDefault);
	
	m_btnLinkSrc.Create(_T("조건검색"), CRect(0, 0, 0, 0), this, IDC_BTN_LINKSRC);
	pwndMainFrm->SetBtnImg(&m_btnLinkSrc, 10);

	m_btnTip.Create(_T("Tip"), CRect(0, 0, 0, 0), this, IDC_BTN_TIP);
	pwndMainFrm->SetBtnImg(&m_btnTip, 2);
	
	m_btnLeft.Create(_T("삭제"), CRect(0, 0, 0, 0), this, IDC_BTN_LEFT, FALSE, TRUE);
	pwndMainFrm->SetBtnImg(&m_btnLeft, 4);
	
	m_btnLeftAll.Create(_T("전체삭제"), CRect(0, 0, 0, 0), this, IDC_BTN_LEFTALL, FALSE, TRUE);
	pwndMainFrm->SetBtnImg(&m_btnLeftAll, 4);
	
	m_btnRight.Create(_T("추가"), CRect(0, 0, 0, 0), this, IDC_BTN_RIGHT, FALSE, TRUE);
	pwndMainFrm->SetBtnImg(&m_btnRight, 4);
	
	m_btnRightAll.Create(_T("전체추가"), CRect(0, 0, 0, 0), this, IDC_BTN_RIGHTALL, FALSE, TRUE);
	pwndMainFrm->SetBtnImg(&m_btnRightAll, 4);
	m_btnRightAll.ShowWindow(SW_HIDE);
	
	m_ctrlRightTitlePoint.Create(_T(""), WS_CHILD | WS_VISIBLE | WS_BORDER,
		CRect(0, 0, 0, 0), this, IDC_SIATIC_SUBTITLEPOINT + 1);
	
	m_ctrlRightTitle.Create(_T("항목선택 / 통보설정"), WS_CHILD | WS_VISIBLE | SS_LEFT, 
		CRect(0, 0, 0, 0), this, IDC_SIATIC_SUBTITLE + 1);
	m_ctrlRightTitle.SetFont(&m_fontBold);
	
	m_ctrlAddCnt.Create(_T("적용항목수 : n"), WS_CHILD | WS_VISIBLE | SS_LEFT, 
		CRect(0, 0, 0, 0), this);
	m_ctrlAddCnt.SetFont(&m_fontDefault);
	
	/*
	m_gridSetSignal.CreateGrid(
    WS_CHILD | WS_VISIBLE | WS_CLIPCHILDREN | WS_CLIPSIBLINGS | WS_BORDER | SS_SIMPLE,
    CRect(0, 0, 0, 0), this, IDC_GRID_SETSIGNAL);
	*/
	
	m_gridSetSignal.Create(NULL, _T(""), WS_CHILD | WS_VISIBLE | WS_BORDER,
		CRect(0, 0, 0, 0), this, NULL);
	
	m_ctrlNote.Create(_T("<주의> 1개의 조건검색 신호를 등록할 수 있으며, 등록된 조건검색 신호는 HTS 종료 시 자동삭제 됩니다."), 
		WS_CHILD | WS_VISIBLE | SS_LEFT, 
		CRect(0, 0, 0, 0), this);
	m_ctrlNote.SetFont(&m_fontDefault);
	
	m_btnHelpShow.Create(_T("선택조건설명 ▲"), CRect(0, 0, 0, 0), this, IDC_BTN_HELPSHOW, FALSE, TRUE);
	pwndMainFrm->SetBtnImg(&m_btnHelpShow, 8);

	m_editHelp.Create(WS_CHILD | ES_MULTILINE | ES_READONLY | WS_VSCROLL, 
		CRect(0, 0, 0, 0), this, IDC_EDIT_ITEMHELP);
	m_editHelp.SetFont(&m_fontDefault);
	m_editHelp.ModifyStyleEx(0, WS_EX_CLIENTEDGE);

	AfxSetResourceHandle(hInstSave);

	if (m_nType == 0)
	{
		m_btnLinkSrc.ShowWindow(SW_HIDE);
		m_btnTip.ShowWindow(SW_HIDE);
		m_ctrlNote.ShowWindow(SW_HIDE);
	}
	
	return 0;
}

void CConditionSetWnd::SetInit_SetVal()
{
	m_gridSetSignal.ClearAll();
	//--> 0016487: [7134] 신호검색 설정화면에서 신호 항목 변경후 [초기화] 클릭시 정렬만 되는 현상 수정
	SetClear_SignalSet(&m_mapSignalSet);
	CMainFrmWnd *pwndMainFrm = (CMainFrmWnd*)GetParent();
	pwndMainFrm->GetSignalDef();
	//<--
	
	POSITION pos;
	CString strKey;
	SetSignalData	*pstSetSignalData;
	for (pos = m_mapSignalSet.GetStartPosition(); pos != NULL;)
	{
		m_mapSignalSet.GetNextAssoc(pos, strKey, (void*&)pstSetSignalData);
		
		if (pstSetSignalData != NULL)
		{
			pstSetSignalData->nPoint = pstSetSignalData->stConditionData.nBasePoint;
			m_mapSignalSet.SetAt(strKey, pstSetSignalData);
			
			m_gridSetSignal.SetAdd_Row(pstSetSignalData);
		}
	}

	//--> 0016487: [7134] 신호검색 설정화면에서 신호 항목 변경후 [초기화] 클릭시 정렬만 되는 현상 수정
	CString strTemp;
	strTemp.Format("적용항목수 : %d", m_gridSetSignal.GetNumberRows());
	m_ctrlAddCnt.SetWindowText(strTemp);

	if(m_gridSetSignal.GetNumberRows() > 0)
	{
		m_btnLeft.EnableWindow(TRUE);
		m_btnLeftAll.EnableWindow(TRUE);
	}
	else
	{
		m_btnLeft.EnableWindow(FALSE);
		m_btnLeftAll.EnableWindow(FALSE);
	}
	//<--
	
	m_gridSetSignal.RedrawAll();
}

void CConditionSetWnd::SetInit()
{
	if (m_nType == 0)		//# Sys
		SetLoadFile();
	else if (m_nType == 1)		//# User
		SetLoadFile();
	
	SetLoadHelp();

	m_treeCondition.SetInit(&m_listConditionData);
	m_gridSetSignal.SetInit();
    
	POSITION pos;
	CString strKey;
	SetSignalData	*pstSetSignalData;
	for (pos = m_mapSignalSet.GetStartPosition(); pos != NULL;)
	{
		m_mapSignalSet.GetNextAssoc(pos, strKey, (void*&)pstSetSignalData);
		
		if (pstSetSignalData != NULL)
			m_gridSetSignal.SetAdd_Row(pstSetSignalData);
	}
	m_gridSetSignal.RedrawAll();
	
	CString strTemp;
	strTemp.Format("적용항목수 : %d", m_gridSetSignal.GetNumberRows());
	m_ctrlAddCnt.SetWindowText(strTemp);
	
	int nTotCnt_Condition = 0;
	ConditionDataGroup  stConditionDataGroup;
	for (int nIndex = 0; nIndex < m_listConditionData.GetCount(); nIndex++)
	{
		pos = m_listConditionData.FindIndex(nIndex);
		stConditionDataGroup = m_listConditionData.GetAt(pos);
		
		nTotCnt_Condition += stConditionDataGroup.plistData->GetCount();
	}
	
	if (m_gridSetSignal.GetNumberRows() == nTotCnt_Condition)
	{
		m_btnLeft.EnableWindow(TRUE);
		m_btnLeftAll.EnableWindow(TRUE);
		
		m_btnRight.EnableWindow(FALSE);
		m_btnRightAll.EnableWindow(FALSE);
	}
	else
	{
		m_btnLeft.EnableWindow(TRUE);
		m_btnLeftAll.EnableWindow(TRUE);
		
		m_btnRight.EnableWindow(TRUE);
		m_btnRightAll.EnableWindow(TRUE);
	}

	CMainFrmWnd *pwndMainFrm = (CMainFrmWnd*)GetParent();
	
	ST_GRIDCOLOR stGridColor;
	stGridColor.clrGridHeader = pwndMainFrm->GetAxColor(74);
	stGridColor.clrGridHilight = pwndMainFrm->GetAxColor(78);
	
	ChangeGridColor(stGridColor);
}

void CConditionSetWnd::OnDestroy() 
{
	//	CWnd::OnDestroy();
	
	POSITION pos;
	ConditionDataGroup	stConditionDataGroup;
	for (int nIndex = 0; nIndex < m_listConditionData.GetCount(); nIndex++)
	{
		pos = m_listConditionData.FindIndex(nIndex);
		if (pos != NULL)
		{
			stConditionDataGroup = m_listConditionData.GetAt(pos);
			
			stConditionDataGroup.plistData->RemoveAll();
			delete stConditionDataGroup.plistData;
		}
	}
	m_listConditionData.RemoveAll();
	
	SetClear_SignalSet(&m_mapSignalSet);
	
	CWnd::OnDestroy();
}

void CConditionSetWnd::SetClear_SignalSet(CMapStringToPtr *pmapSignalSet)
{
	POSITION pos;
	CString strKey;
	SetSignalData	*pstSetSignalData;
	for (pos = pmapSignalSet->GetStartPosition(); pos != NULL;)
	{
		pmapSignalSet->GetNextAssoc(pos, strKey, (void*&)pstSetSignalData);
		if (pstSetSignalData != NULL)
		{
			delete pstSetSignalData;
			pstSetSignalData = NULL;
		}
		pmapSignalSet->RemoveKey(strKey);
	}
	pmapSignalSet->RemoveAll();
}

void CConditionSetWnd::OnSize(UINT nType, int cx, int cy) 
{
	CWnd::OnSize(nType, cx, cy);
	
	CRect reClient, reLeft;
	GetClientRect(reClient);
	
	reClient.top += 7;
//	reClient.bottom -= 11;
// 	reClient.left += 7;
// 	reClient.right -= 7;
	
	reLeft = reClient;
	
	reLeft.right = reLeft.left + 180;
	OnSize_Left(reLeft);		//# left
	
	reLeft.left = reLeft.right + 5;
	reLeft.right = reLeft.left + 60;
	OnSize_Move(reLeft);		//# move
	
	reLeft.left = reLeft.right + 5;
	reLeft.right = reClient.right;
	OnSize_Right(reLeft);		//# right

	OnPF_Contorl_Move();
}

void CConditionSetWnd::OnSize_Left(CRect reClient) 
{
	CRect reTop, reTemp;
	reTop = reClient;
	
	reTop.bottom = reTop.top + 11;
	reTemp = reTop;
	
	reTemp.right = reTemp.left + 3;
	m_ctrlLeftTitlePoint.MoveWindow(&reTemp);
	
	reTemp.left = reTemp.right + 6;
	reTemp.right = reTop.right;
	m_ctrlLeftTitle.MoveWindow(&reTemp);
	
	CRect reBottom;
	reBottom = reClient;
	
	if (m_bHelpShowFlag == TRUE)
		reBottom.top = reBottom.bottom - 100;
	else
		reBottom.top = reBottom.bottom + 3;
	m_editHelp.MoveWindow(&reBottom);
	
	reBottom.bottom = reBottom.top - 3;
	reBottom.top = reBottom.bottom - 18;
	m_btnHelpShow.MoveWindow(&reBottom);

	if (m_nType == 0)
	{
		reTop.top = reTop.bottom + 11;
		reTop.bottom = reBottom.top - 4;
		m_treeCondition.MoveWindow(&reTop);
	}
	else if (m_nType == 1)
	{
		int nHeight_Tool = 24;
		reTop.top = reTop.bottom + 11;
		reTop.bottom = reClient.bottom - nHeight_Tool;
		m_treeCondition.MoveWindow(&reTop);
		
		reTop.top = reTop.bottom + 11;
		reTop.bottom = reTop.top + 20;
		
		reTemp = reTop;
		reTemp.left += 5;
		reTemp.right = reTemp.left + 60;
		m_btnLinkSrc.MoveWindow(&reTemp);
		
		reTemp.left = reTemp.right + 2;
		reTemp.right = reTemp.left + 30;
		m_btnTip.MoveWindow(&reTemp);
	}
}

void CConditionSetWnd::OnSize_Move(CRect reClient) 
{
	CRect reTop;
	
// 	reClient.left += 3;
// 	reClient.right -= 3;
	
	int nHeight_Btn = 20;
	int nGap_Btn = 3;
	
	int nOffset;
	nOffset = (reClient.bottom - reClient.top) - (nHeight_Btn * 5 + nGap_Btn * 3);
	nOffset = nOffset / 2;
	
	reTop = reClient;
	reTop.top = nOffset;
	reTop.bottom = reTop.top + nHeight_Btn;
	m_btnRightAll.MoveWindow(&reTop);
	
	reTop.top = reTop.bottom + nGap_Btn;
	reTop.bottom = reTop.top + nHeight_Btn;
	m_btnRight.MoveWindow(&reTop);
	
	reTop.top = reTop.bottom + nHeight_Btn;
	reTop.bottom = reTop.top + nHeight_Btn;
	m_btnLeftAll.MoveWindow(&reTop);
	
	reTop.top = reTop.bottom + nGap_Btn;
	reTop.bottom = reTop.top + nHeight_Btn;
	m_btnLeft.MoveWindow(&reTop);
}

void CConditionSetWnd::OnSize_Right(CRect reClient) 
{
	CRect reTop, reTemp;
	
	reTop = reClient;
	reTop.bottom = reTop.top + 11;
	
	reTemp = reTop;
	reTemp.right = reTemp.left + 150;
	
	reTemp.right = reTemp.left + 3;
	m_ctrlRightTitlePoint.MoveWindow(&reTemp);
	
	reTemp.left = reTemp.right + 6;
	reTemp.right = reTop.right;
	m_ctrlRightTitle.MoveWindow(&reTemp);
	
	reTemp = reTop;
	/*reTemp.right -= 5;*/
	reTemp.left = reTemp.right - 95;
	m_ctrlAddCnt.MoveWindow(&reTemp);
	
	if (m_nType == 0)
	{
		reTop.top = reTop.bottom + 11;
		reTop.bottom = reClient.bottom;
		m_gridSetSignal.MoveWindow(&reTop);
	}
	else if (m_nType == 1)
	{
		reTop.top = reTop.bottom + 11;
		reTop.bottom = reClient.bottom - 26;
		m_gridSetSignal.MoveWindow(&reTop);
		
		reTop.top = reTop.bottom;
		reTop.bottom = reClient.bottom;
		m_ctrlNote.MoveWindow(&reTop);
	}
}

void CConditionSetWnd::SetLoadFile()
{
	FILE *fr = NULL;
	char buffer[1024] = {0,};
	
	CString strAppPath;
	GetDir_DrfnRoot(strAppPath);
	CString strFileName = strAppPath + FOLDER_DATA + FOLDER_MASTER + NEWS_SYSLIST_FILE;
	
	if ((fr = fopen((LPTSTR)(LPCTSTR)strFileName,"r")) == NULL) 
		return;
	
	CString strGroup;
	ConditionData		stConditionData;
	POSITION pos;
	
	int nIndex = 0;
	while(!feof(fr)) 
	{
		memset(buffer, 0x00, 1024);
		if (fgets(buffer, sizeof(buffer), fr) == NULL) 
			break;
		buffer[strlen(buffer)-1] = 0x00;
		
		if (SetLoadFile_Data(buffer, strGroup, stConditionData) == TRUE)
		{
			ConditionDataGroup	stConditionDataGroup;
			if (GetFind_ConditionGroup(strGroup, stConditionDataGroup) == NULL)
			{
				stConditionDataGroup.strGroup = strGroup;
				stConditionDataGroup.plistData = new CList <ConditionData, ConditionData&>;
				
				stConditionData.nIndex = nIndex;
				stConditionDataGroup.plistData->AddTail(stConditionData);
				
				pos = m_listConditionData.AddTail(stConditionDataGroup);
			}
			else
			{
				stConditionData.nIndex = nIndex;
				
				stConditionDataGroup.plistData->AddTail(stConditionData);
				m_listConditionData.SetAt(pos, stConditionDataGroup);
			}
			nIndex++;
		}
	}
	
	fclose(fr);
	fr = NULL;
}


BOOL CConditionSetWnd::SetLoadFile_Data(const char* pData, CString &strGroup, ConditionData &stConditionData)
{
	BOOL bResult = FALSE;
	
	CString strData = pData;
	CString strElement;
	int nFirst = 0;
	
	int nFind_Pos = strData.Find(":", nFirst);
	strElement = strData.Mid(nFirst, nFind_Pos);
	
	int nIndex = 0;
	while (nFind_Pos != -1)
	{
		if (nIndex == 0)			//# Index
			stConditionData.strCode = strElement;
		else if (nIndex == 1)			//# Group
			strGroup = strElement;
		else if (nIndex == 2)			//# Point
			stConditionData.nBasePoint = atoi(strElement);
		else if (nIndex == 3)			//# Title
		{
			stConditionData.strTitle = strElement;
			bResult = TRUE;
		}
		
		nIndex++;
		
		nFirst = nFind_Pos + 1;
		nFind_Pos = strData.Find(":", nFirst);
		strElement = strData.Mid(nFirst, nFind_Pos - nFirst);
	}
	
	return bResult;
}

POSITION CConditionSetWnd::GetFind_ConditionGroup(CString strGroup, ConditionDataGroup &stConditionDataGroup)
{
	POSITION posResult = NULL;
	
	POSITION pos;
	for (int nIndex = 0; nIndex < m_listConditionData.GetCount(); nIndex++)
	{
		pos = m_listConditionData.FindIndex(nIndex);
		stConditionDataGroup = m_listConditionData.GetAt(pos);
		
		if (stConditionDataGroup.strGroup == strGroup)
		{
			posResult = pos;
			break;
		}
	}
	
	return posResult;
}

//void CConditionSetWnd::OnNotify_Tree(WPARAM wParam, LPARAM lParam)
LRESULT CConditionSetWnd::OnNotify_Tree(WPARAM wParam, LPARAM lParam)
{
	ConditionData	stConditionData;
	if (wParam == NM_ITEM_CLK)
	{
		stConditionData = *(ConditionData *)lParam;

		CString strExplanation;
		if (m_mapConditionHelp.Lookup( (LPCTSTR)stConditionData.strCode, strExplanation) == TRUE)
		{
			m_editHelp.SetWindowText(strExplanation);
		}
		else
		{
// 			strExplanation = _T("설명 내역이 존재하지 않습니다.");
			m_editHelp.SetWindowText(strExplanation);
		}
	}
	else if (wParam == NM_ITEM_DBCLK)
	{
		stConditionData = *(ConditionData *)lParam;
		SetAdd_SetSignalData(stConditionData);
	}

	return TRUE;
}

//void CConditionSetWnd::OnNotify_SignalGrid(WPARAM wParam, LPARAM lParam)
LRESULT CConditionSetWnd::OnNotify_SignalGrid(WPARAM wParam, LPARAM lParam)
{
	int nIndex, nCol;
	
	if (wParam == NM_ITEM_CELLTYPE)
	{
		CStringArray *psaData = (CStringArray*)lParam;
		
		SetSignalData	*pstSetSignalData;
		nIndex = 1;
		if (m_mapSignalSet.Lookup(psaData->GetAt(nIndex), (void*&)pstSetSignalData) == TRUE)
		{
			nIndex = 2;
			nCol = atoi(psaData->GetAt(nIndex));
			
			nIndex = 3;
			if (nCol == 1)		//# 신호강도
				pstSetSignalData->nPoint = atoi(psaData->GetAt(nIndex));
			else if (nCol == 2)		//# 신호등
				pstSetSignalData->nSend_Signal = atoi(psaData->GetAt(nIndex));
			else if (nCol == 3)		//# 티커
				pstSetSignalData->nSend_Ticker = atoi(psaData->GetAt(nIndex));
			else if (nCol == 4)		//# 신호주문
				pstSetSignalData->nSend_Order = atoi(psaData->GetAt(nIndex));
			else if (nCol == 5)		//# 작동상태
				pstSetSignalData->nState = atoi(psaData->GetAt(nIndex));
			
			nIndex = 1;
			m_mapSignalSet.SetAt(psaData->GetAt(nIndex), (void*&)pstSetSignalData);
		}
	}
	else if (wParam == NM_TH_CLK)
	{
		CUIntArray *piaData = (CUIntArray*)lParam;
		
		nIndex = 0;
		nCol = piaData->GetAt(nIndex);
		
		POSITION pos;
		CString strKey;
		SetSignalData	*pstSetSignalData;
		
		nIndex = 1;
		for (pos = m_mapSignalSet.GetStartPosition(); pos != NULL;)
		{
			m_mapSignalSet.GetNextAssoc(pos, strKey, (void*&)pstSetSignalData);
			
			if (nCol == 2)		//# 신호등
				pstSetSignalData->nSend_Signal = piaData->GetAt(nIndex);
			else if (nCol == 3)		//# 티커
				pstSetSignalData->nSend_Ticker = piaData->GetAt(nIndex);
			else if (nCol == 4)		//# 신호주문
				pstSetSignalData->nSend_Order = piaData->GetAt(nIndex);
			else if (nCol == 5)		//# 작동상태
				pstSetSignalData->nState = piaData->GetAt(nIndex);
			
			m_mapSignalSet.SetAt(strKey, (void*&)pstSetSignalData);
		}
	}
	else if (wParam == NM_ITEM_DBCLK)
		OnBtnLeft();

	return TRUE;
}

void CConditionSetWnd::OnBtnLeft()
{
	SetDelete_SetSignalData(m_gridSetSignal.GetSelectRow());
	
	if (m_gridSetSignal.GetNumberRows() == 0)
	{
		m_btnLeft.EnableWindow(FALSE);
		m_btnLeftAll.EnableWindow(FALSE);
		
		m_btnRight.EnableWindow(TRUE);
		m_btnRightAll.EnableWindow(TRUE);
	}
	else
	{
		m_btnLeft.EnableWindow(TRUE);
		m_btnLeftAll.EnableWindow(TRUE);
		
		m_btnRight.EnableWindow(TRUE);
		m_btnRightAll.EnableWindow(TRUE);
	}
	Invalidate();
}

void CConditionSetWnd::SetDelete_SetSignalData(int nRow, BOOL bMsg /*= TRUE*/)
{
	CString strKey = m_gridSetSignal.GetRowToKey(nRow);
	
	SetSignalData	*pstSetSignalData;
	if (m_mapSignalSet.Lookup(strKey, (void*&)pstSetSignalData) == TRUE)
	{
		delete pstSetSignalData;
		pstSetSignalData = NULL;
		m_mapSignalSet.RemoveKey(strKey);
		
		m_gridSetSignal.SetDelete_Row(nRow);
		
		CString strTemp;
		strTemp.Format("적용항목수 : %d", m_gridSetSignal.GetNumberRows());
		m_ctrlAddCnt.SetWindowText(strTemp);
	}
}

void CConditionSetWnd::OnBtnLeftAll()
{
	m_btnLeft.EnableWindow(FALSE);
	m_btnLeftAll.EnableWindow(FALSE);
	
	m_btnRight.EnableWindow(TRUE);
	m_btnRightAll.EnableWindow(TRUE);
	
	Invalidate();
	
	m_gridSetSignal.ClearAll();
	SetClear_SignalSet(&m_mapSignalSet);
	
	CString strTemp;
	strTemp.Format("적용항목수 : %d", m_gridSetSignal.GetNumberRows());
	m_ctrlAddCnt.SetWindowText(strTemp);
}

CString CConditionSetWnd::GetSignalDataToKey(SetSignalData	*pstSetSignalData)
{
	CString strKey = "";
	
	if (pstSetSignalData->nType == 0)		//# Keyword
	{
		if (pstSetSignalData->stKeywordData.nType == 0)
			strKey = pstSetSignalData->stKeywordData.strCode;
		else if (pstSetSignalData->stKeywordData.nType == 1)
			strKey = pstSetSignalData->stKeywordData.strName_S;
	}
	else if (pstSetSignalData->nType == 1)		//# Condition
		strKey = pstSetSignalData->stConditionData.strCode;
	
	return strKey;
}

void CConditionSetWnd::SetAdd_SetSignalData(ConditionData	stConditionData, BOOL bMsg /*= TRUE*/)
{
	SetSignalData	stSetSignalData;
	GetData_DefultSignal(stConditionData, stSetSignalData);
	
	if (m_gridSetSignal.GetHaveState(stSetSignalData.strTitle) == FALSE)
	{ 
		int nCnt_Condition = 0;
		nCnt_Condition = m_gridSetSignal.GetNumberRows();
		if ((int)MAXCNT_REGOK <= nCnt_Condition)
		{
			CString strMsg;
			strMsg.Format("등록 조건수(%d개)를 초과 했습니다.\n 더이상 조건을 추가 할수 없습니다.", (int)MAXCNT_REGOK);
			SetMessageBox(strMsg);
			return;
		}
		
		SetSignalData	*pstSetSignalData = new SetSignalData;
		SETINIT_SIGNALDATA(pstSetSignalData);
		SETCOPY_SIGNALDATA(stSetSignalData, pstSetSignalData); 
		
		m_mapSignalSet.SetAt(pstSetSignalData->strKey, pstSetSignalData);
		m_gridSetSignal.SetAdd_Row(pstSetSignalData, TRUE);
		
		nCnt_Condition = m_gridSetSignal.GetNumberRows();
		
		CString strTemp;
		strTemp.Format("적용항목수 : %d", nCnt_Condition);
		m_ctrlAddCnt.SetWindowText(strTemp);
		if (bMsg == TRUE)
		{
		/*
		int nTotCnt_Condition;
		nTotCnt_Condition = m_listConditionData.GetCount();
		if (m_treeCondition.GetDataCnt() == nTotCnt_Condition)
			*/
			
			if ((int)MAXCNT_REGOK == nCnt_Condition)
			{
				m_btnLeft.EnableWindow(TRUE);
				m_btnLeftAll.EnableWindow(TRUE);
				
				m_btnRight.EnableWindow(FALSE);
				m_btnRightAll.EnableWindow(FALSE);
			}
			else
			{
				m_btnLeft.EnableWindow(TRUE);
				m_btnLeftAll.EnableWindow(TRUE);
				
				m_btnRight.EnableWindow(TRUE);
				m_btnRightAll.EnableWindow(TRUE);
			}
		}
	}
	else
	{
		if (bMsg == TRUE)
			SetMessageBox("선택 조건은 이미 추가되었습니다.");
	}
}

void CConditionSetWnd::OnBtnRight()
{
	ConditionData	stConditionData;
	if (m_treeCondition.GetData_SelItem(stConditionData) == TRUE)
		SetAdd_SetSignalData(stConditionData);
	else
		SetMessageBox("선택된 조건이 존재하지 않습니다.");
}

void CConditionSetWnd::GetData_DefultSignal(ConditionData stConditionData, SetSignalData	&stSetSignalData)
{
	stSetSignalData.nIndex = 0;
	stSetSignalData.strKey = stConditionData.strCode;
	
	stSetSignalData.nType = (m_nType == 0) ? 1 : 2;
	stSetSignalData.stConditionData = stConditionData;
	
	stSetSignalData.strTitle = stConditionData.strTitle;
	stSetSignalData.nPoint = stConditionData.nBasePoint;
	
	stSetSignalData.nSend_Signal = 1;
	stSetSignalData.nSend_Ticker = 0;
	stSetSignalData.nSend_Order = 0;
	
	stSetSignalData.nState = 1;
}

void CConditionSetWnd::OnBtnRightAll()
{
	POSITION pos, posItem;
	ConditionDataGroup	stConditionDataGroup;
	ConditionData				stConditionData;
	
	int nIndex, nIndex_Item;
	for (nIndex = 0; nIndex < m_listConditionData.GetCount(); nIndex++)
	{
		pos = m_listConditionData.FindIndex(nIndex);
		stConditionDataGroup = m_listConditionData.GetAt(pos);
		
		for (nIndex_Item = 0; nIndex_Item < stConditionDataGroup.plistData->GetCount(); nIndex_Item++)
		{
			posItem = stConditionDataGroup.plistData->FindIndex(nIndex_Item);
			stConditionData = stConditionDataGroup.plistData->GetAt(posItem);
			
			SetAdd_SetSignalData(stConditionData, FALSE);
		}
	}
	
	m_btnLeft.EnableWindow(TRUE);
	m_btnLeftAll.EnableWindow(TRUE);
	
	m_btnRight.EnableWindow(FALSE);
	m_btnRightAll.EnableWindow(FALSE);
}

int CConditionSetWnd::SetMessageBox(LPCTSTR lpszText, LPCTSTR lpszCaption, UINT nType)
{
	CMainFrmWnd *pwndMainFrm = (CMainFrmWnd*)GetParent();
	return pwndMainFrm->SetMessageBox(lpszText, lpszCaption, nType);
}

HBRUSH CConditionSetWnd::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CWnd::OnCtlColor(pDC, pWnd, nCtlColor);
	
	if (nCtlColor == CTLCOLOR_STATIC)
	{
		if ((pWnd->GetDlgCtrlID() == IDC_SIATIC_SUBTITLEPOINT) || (pWnd->GetDlgCtrlID() == IDC_SIATIC_SUBTITLEPOINT + 1))
		{
			pDC->SetBkMode(TRANSPARENT);
			pDC->SetBkColor(COLOR_SUBTITLE_POINT);
			hbr = m_brushSubTitlePoint;
		}
		else
		{
			CMainFrmWnd *pwndMainFrm = (CMainFrmWnd*)GetParent();

			if ((pWnd->GetDlgCtrlID() == IDC_SIATIC_SUBTITLE) ||  (pWnd->GetDlgCtrlID() == IDC_SIATIC_SUBTITLE + 1))
				pDC->SetTextColor(COLOR_SUBTITLE_TEXT);
			
			pDC->SetBkMode(TRANSPARENT);
			//hbr = m_brushStatic;
			hbr = pwndMainFrm->GetAxBrush(pwndMainFrm->GetAxColor(66));
		}
	}
	
	return hbr;
}

void CConditionSetWnd::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	
}

void CConditionSetWnd::SetDrewBox(CPaintDC *pDC, CRect reData)
{
	CPen *pOldPen;
	CPen penLine(PS_SOLID, 0, COLOR_OUTLINE);
	
	pOldPen = pDC->SelectObject(&penLine);
	pDC->MoveTo(reData.left, reData.top);
	pDC->LineTo(reData.right, reData.top);
	pDC->LineTo(reData.right, reData.bottom);
	pDC->LineTo(reData.left, reData.bottom);
	pDC->LineTo(reData.left, reData.top);
	
	pDC->SelectObject(pOldPen);
	penLine.DeleteObject();
}

void CConditionSetWnd::SetDrewBox(CDC *pDC, CRect reData)
{
	CPen *pOldPen;
	CPen penLine(PS_SOLID, 0, COLOR_OUTLINE);

	pOldPen = pDC->SelectObject(&penLine);
	pDC->MoveTo(reData.left, reData.top);
	pDC->LineTo(reData.right, reData.top);
	pDC->LineTo(reData.right, reData.bottom);
	pDC->LineTo(reData.left, reData.bottom);
	pDC->LineTo(reData.left, reData.top);

	pDC->SelectObject(pOldPen);
	penLine.DeleteObject();
}

void CConditionSetWnd::OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct) 
{
	// TODO: Add your message handler code here and/or call default
	
	//if (nIDCtl == 
	
	
	CWnd::OnDrawItem(nIDCtl, lpDrawItemStruct);
}

void CConditionSetWnd::OnPF_Contorl_Move()
{
	if(!GetParent()) return;

	ST_MOVEITEM stItem;	
	stItem.nType = PF_ITEM_BTN;	
	
	SET_ST_MOVEITEM(stItem, "btnAdd", &m_btnRight);	
	GetParent()->SendMessage(MF_PFCONTORL_MOVE, m_bShow_PF_CTL, (LPARAM)&stItem);
	
	SET_ST_MOVEITEM(stItem, "btnRemoveAll", &m_btnLeftAll);
	GetParent()->SendMessage(MF_PFCONTORL_MOVE, m_bShow_PF_CTL, (LPARAM)&stItem);
	
	SET_ST_MOVEITEM(stItem, "btnRemove", &m_btnLeft);
 	GetParent()->SendMessage(MF_PFCONTORL_MOVE, m_bShow_PF_CTL, (LPARAM)&stItem);
}

void CConditionSetWnd::PlatFormBtnAdd()
{
	OnBtnRight();
}
void CConditionSetWnd::PlatFormBtnRemoveAll()
{
	OnBtnLeftAll();
}
void CConditionSetWnd::PlatFormBtnRemove()
{
	OnBtnLeft();
}

BOOL CConditionSetWnd::OnEraseBkgnd(CDC* pDC)
{
	CMainFrmWnd *pwndMainFrm = (CMainFrmWnd*)GetParent();

	CRect reClient;
	GetClientRect(reClient);
	//pDC->FillSolidRect(reClient, COLOR_SETWND_FACE);
	pDC->FillSolidRect(reClient, pwndMainFrm->GetAxColor(66));

//	SetDrewBox(pDC, reClient);
	return FALSE;
}

void CConditionSetWnd::ChangeSkinColor(COLORREF clrBkgnd)
{
	if(!clrBkgnd)
		return;
	
	m_clrMainSkin = clrBkgnd;

	CMainFrmWnd *pwndMainFrm = (CMainFrmWnd*)GetParent();

	ST_GRIDCOLOR stGridColor;
	stGridColor.clrGridHeader = pwndMainFrm->GetAxColor(74);
	stGridColor.clrGridHilight = pwndMainFrm->GetAxColor(78);
	
	ChangeGridColor(stGridColor);
	
	Invalidate();
}

void CConditionSetWnd::ChangeGridColor(ST_GRIDCOLOR stGridColor)
{
	m_gridSetSignal.ChangeGridColor(stGridColor);
	m_gridSetSignal.Invalidate();
}

void CConditionSetWnd::OnBtnShowHelp()
{
	if (m_bHelpShowFlag == TRUE)
	{
		m_btnHelpShow.SetWindowText("선택지표설명 ▲");
		m_editHelp.ShowWindow(SW_HIDE);
	}
	else
	{
		m_btnHelpShow.SetWindowText("선택지표설명 ▼");
		m_editHelp.ShowWindow(SW_SHOW);
	}
	m_bHelpShowFlag = !m_bHelpShowFlag;
	
	CRect reClient, reLeft;
	GetClientRect(reClient);
	reClient.top += 7;
	reLeft = reClient;
	reLeft.right = reLeft.left + 180;
	OnSize_Left(reLeft);
}

#define SIGNALSET_HELP_FILE	"signalset_helpinfo.dat"
void CConditionSetWnd::SetLoadHelp()
{
	FILE *fr = NULL;
	char buffer[1024] = {0,};
	
	CString strAppPath;
	GetDir_DrfnRoot(strAppPath);
	CString strFileName = strAppPath + FOLDER_DATA + "\\" + FILE_EXP + "\\" + SIGNALSET_HELP_FILE;
	
	if ((fr = fopen((LPTSTR)(LPCTSTR)strFileName,"r")) == NULL) 
		return;
	
	CString strKey;
	CString strExplanation;
	POSITION pos = NULL;
	while(!feof(fr)) 
	{
		memset(buffer, 0x00, 1024);
		if (fgets(buffer, sizeof(buffer), fr) == NULL) 
			break;
		buffer[strlen(buffer)-1] = 0x00;
		
		if (SetLoadHelp_Data(buffer, strKey, strExplanation) == TRUE)
		{
			m_mapConditionHelp.SetAt(strKey, strExplanation);
		}
	}
	
	fclose(fr);
	fr = NULL;
}

BOOL CConditionSetWnd::SetLoadHelp_Data(const char* pData, CString &strKey, CString &strExplanation)
{
	BOOL bResult = FALSE;
	
	CString strData = pData;
	CString strElement;
	int nFirst = 0;
	
	int nFind_Pos = strData.Find(":", nFirst);
	strElement = strData.Mid(nFirst, nFind_Pos);
	
	int nIndex = 0;
	while (nFind_Pos != -1)
	{
		if (nIndex == 0)			//# Index
			strKey = strElement;
		else if (nIndex == 1)		//# Explanation
		{
			strExplanation = strElement;
			bResult = TRUE;
		}
		
		nIndex++;
		
		nFirst = nFind_Pos + 1;
		nFind_Pos = strData.Find(":", nFirst);
		strElement = strData.Mid(nFirst, nFind_Pos - nFirst);
	}
	
	return bResult;
}