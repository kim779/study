// NewsSetWnd.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include "NewsSetWnd.h"

#include "MainFrmWnd.h"
#include "RegKeywordDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern HINSTANCE g_hInstance;
/////////////////////////////////////////////////////////////////////////////
// CNewsSetWnd

CNewsSetWnd::CNewsSetWnd()
{
	//# Change Samsung! (2007.1 : Danny)
	BOOL m_bReg_UserKeyword = FALSE;
	
	
	LOGFONT	  logFont;
	GETFONT(logFont, 12, 400, m_fontDefault);
	GETFONT(logFont, 12, 600, m_fontBold);
	
	m_brushStatic.CreateSolidBrush(COLOR_SETWND_FACE);
	m_brushSubTitlePoint.CreateSolidBrush(COLOR_SUBTITLE_POINT);
	
	CString strTemp;
	strTemp = "1. 사용자 등록 상황문은 목록에서 텍스트 색상으로 구분 됩니다.\n";
	m_strInfoTip = strTemp;
	strTemp = "2. 사용자 등록 상확문을 활용한 신호화 과정은 PC에서 처리되어 과거 뉴스의\n";
	m_strInfoTip += strTemp;
	m_nTipMaxLength = strTemp.GetLength();
	strTemp = "   조회와 실시간 수신 간의 뉴스강도의 차이가 발생할 수 있습니다.";
	m_strInfoTip += strTemp;

	m_bShow_PF_CTL = TRUE;
}

CNewsSetWnd::~CNewsSetWnd()
{
	m_fontDefault.DeleteObject();
	m_fontBold.DeleteObject();
	
	m_brushStatic.DeleteObject();
	m_brushSubTitlePoint.DeleteObject();
}

BEGIN_MESSAGE_MAP(CNewsSetWnd, CWnd)
//{{AFX_MSG_MAP(CNewsSetWnd)
ON_WM_CREATE()
ON_WM_SIZE()
ON_WM_CTLCOLOR()
ON_WM_PAINT()
ON_WM_DESTROY()
//}}AFX_MSG_MAP

ON_BN_CLICKED(IDC_BTN_LEFT, OnBtnLeft)
ON_BN_CLICKED(IDC_BTN_LEFTALL, OnBtnLeftAll)
ON_BN_CLICKED(IDC_BTN_RIGHT, OnBtnRight)
ON_BN_CLICKED(IDC_BTN_RIGHTALL, OnBtnRightAll)

ON_BN_CLICKED(IDC_BTN_FIND, OnBtnFind)
ON_BN_CLICKED(IDC_BTN_PRE, OnBtnPre)
ON_BN_CLICKED(IDC_BTN_NEXT, OnBtnNext)

ON_BN_CLICKED(IDC_BTN_ADD, OnBtnAdd)
ON_BN_CLICKED(IDC_BTN_DELETE, OnBtnDelete)
ON_BN_CLICKED(IDC_BTN_TIP, OnBtnTip)

ON_MESSAGE(WM_NOTIFY_GRID, OnNotify_Grid)
ON_MESSAGE(WM_NOTIFY_SIGNALGRID, OnNotify_SignalGrid)
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CNewsSetWnd message handlers
BOOL CNewsSetWnd::Create(LPCTSTR lpszWindowName, CWnd* pParentWnd, UINT nID) 
{ 
	BOOL bReault;
	bReault = CWnd::Create(_T("STATIC"), lpszWindowName, WS_CHILD | WS_VISIBLE,
		CRect(0, 0, 0, 0), pParentWnd, nID);
	
	return bReault;
}

int CNewsSetWnd::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	HINSTANCE hInstSave = AfxGetResourceHandle();
	AfxSetResourceHandle(g_hInstance);
	
	m_ctrlLeftTitlePoint.Create(_T(""), WS_CHILD | WS_VISIBLE | WS_BORDER, 
		CRect(0, 0, 0, 0), this, IDC_SIATIC_SUBTITLEPOINT);
	m_ctrlLeftTitlePoint.ShowWindow(SW_HIDE);
	
	m_ctrlLeftTitle.Create(_T("적용가능항목"), WS_CHILD | WS_VISIBLE | SS_LEFT, 
		CRect(0, 0, 0, 0), this, IDC_SIATIC_SUBTITLE);
	m_ctrlLeftTitle.SetFont(&m_fontBold);
	
	/*
	m_gridKeyword.CreateGrid(
    WS_CHILD | WS_VISIBLE | WS_CLIPCHILDREN | WS_CLIPSIBLINGS | SS_SIMPLE | WS_BORDER,
    CRect(0, 0, 0, 0), this, IDC_GRID_KEYWORD);
	*/
	
	m_gridKeyword.Create(NULL, _T(""), WS_CHILD | WS_VISIBLE | WS_BORDER,
		CRect(0, 0, 0, 0), this, NULL);
	
	m_ctrlFind.Create(_T("찾기"), WS_CHILD | WS_VISIBLE | SS_LEFT, 
		CRect(0, 0, 0, 0), this);
	m_ctrlFind.SetFont(&m_fontDefault);
	
	m_editFind.Create(WS_CHILD | WS_VISIBLE,// | WS_BORDER,
		CRect(0, 0, 0, 0), this, IDC_EDIT_FIND);
	m_editFind.SetFont(&m_fontDefault);
	m_editFind.ModifyStyleEx(0, WS_EX_CLIENTEDGE);
	
	m_btnFind.Create(_T("F"), WS_CHILD | WS_VISIBLE | BS_OWNERDRAW, 
		CRect(0, 0, 0, 0), this, IDC_BTN_FIND);
	m_btnFind.SetFont(&m_fontBold);
	m_btnFind.SetLoadImage(IDB_BTN_SEARCH);
	
	m_btnPre.Create(_T(">"), WS_CHILD | WS_VISIBLE | BS_OWNERDRAW, 
		CRect(0, 0, 0, 0), this, IDC_BTN_PRE);
	m_btnPre.SetFont(&m_fontBold);
	m_btnPre.SetLoadImage(IDB_BTN_PRE);
	
	m_btnNext.Create(_T("<"), WS_CHILD | WS_VISIBLE | BS_OWNERDRAW, 
		CRect(0, 0, 0, 0), this, IDC_BTN_NEXT);
	m_btnNext.SetFont(&m_fontBold);
	m_btnNext.SetLoadImage(IDB_BTN_NEXT);
	
	m_btnAdd.Create(_T("상황문등록"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 
		CRect(0, 0, 0, 0), this, IDC_BTN_ADD);
	m_btnAdd.SetFont(&m_fontDefault);
	m_btnAdd.SetBtnColor((int)STYLE_SET);
	
	m_btnDelete.Create(_T("상황문삭제"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 
		CRect(0, 0, 0, 0), this, IDC_BTN_DELETE);
	m_btnDelete.SetFont(&m_fontDefault);
	m_btnDelete.SetBtnColor((int)STYLE_SET);
	
	m_tipInfo.Create(this, "DOWN", 4);
	m_tipInfo.SetFont(&m_fontDefault);
	m_tipInfo.AddTool(this, (LPCTSTR)" ");
	
	m_btnTip.Create(_T("?"), WS_CHILD | WS_VISIBLE | BS_OWNERDRAW, 
		CRect(0, 0, 0, 0), this, IDC_BTN_TIP);
	m_btnTip.SetFont(&m_fontDefault);
	m_btnTip.SetBtnColor((int)STYLE_SET);
	
	m_btnLeft.Create(_T("삭제"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 
		CRect(0, 0, 0, 0), this, IDC_BTN_LEFT);
	m_btnLeft.SetFont(&m_fontDefault);
	m_btnLeft.SetBtnColor((int)STYLE_SET);
	
	m_btnLeftAll.Create(_T("전체삭제"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 
		CRect(0, 0, 0, 0), this, IDC_BTN_LEFTALL);
	m_btnLeftAll.SetFont(&m_fontDefault);
	m_btnLeftAll.SetBtnColor((int)STYLE_SET);
	
	m_btnRight.Create(_T("추가"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 
		CRect(0, 0, 0, 0), this, IDC_BTN_RIGHT);
	m_btnRight.SetFont(&m_fontDefault);
	m_btnRight.SetBtnColor((int)STYLE_SET);
	
	m_btnRightAll.Create(_T("전체추가"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 
		CRect(0, 0, 0, 0), this, IDC_BTN_RIGHTALL);
	m_btnRightAll.SetFont(&m_fontDefault);
	m_btnRightAll.SetBtnColor((int)STYLE_SET);
	
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

	AfxSetResourceHandle(hInstSave);
	
	return 0;
}

void CNewsSetWnd::SetInit_SetVal()
{
	m_gridSetSignal.ClearAll();
	
	POSITION pos;
	CString strKey;
	SetSignalData	*pstSetSignalData;
	for (pos = m_mapSignalSet.GetStartPosition(); pos != NULL;)
	{
		m_mapSignalSet.GetNextAssoc(pos, strKey, (void*&)pstSetSignalData);
		
		if (pstSetSignalData != NULL)
		{
			pstSetSignalData->nPoint = pstSetSignalData->stKeywordData.nBasePoint;
			m_mapSignalSet.SetAt(strKey, pstSetSignalData);
			
			m_gridSetSignal.SetAdd_Row(pstSetSignalData);
		}
	}
	m_gridSetSignal.RedrawAll();
}

void CNewsSetWnd::SetInit()
{
	m_gridKeyword.SetInit("KEYWORD");
	SetLoadFile();
	
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
	
	int nTotCnt_Keyword;
	nTotCnt_Keyword = m_listUserData.GetCount();
	if (m_gridSetSignal.GetNumberRows() == nTotCnt_Keyword)
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
	
	m_btnNext.EnableWindow(FALSE);
	m_btnPre.EnableWindow(FALSE); 
}

void CNewsSetWnd::OnDestroy() 
{
	//	CWnd::OnDestroy();
	
	//# Change Samsung! (2007.1 : Danny)
	if (m_bReg_UserKeyword == TRUE)
	{
		SetSaveFile();
		m_listUserData.RemoveAll();
	}
	
	SetClear_SignalSet(&m_mapSignalSet);
	
	CWnd::OnDestroy();
}

void CNewsSetWnd::SetClear_SignalSet(CMapStringToPtr *pmapSignalSet)
{
	POSITION pos;
	CString strKey;
	SetSignalData	*pstSetSignalData;
	for (pos = pmapSignalSet->GetStartPosition(); pos != NULL;)
	{
		pmapSignalSet->GetNextAssoc(pos, strKey, (void*&)pstSetSignalData);
		delete pstSetSignalData;
		pstSetSignalData = NULL;
		
		pmapSignalSet->RemoveKey(strKey);
	}
	pmapSignalSet->RemoveAll();
}

void CNewsSetWnd::OnSize(UINT nType, int cx, int cy) 
{
	CWnd::OnSize(nType, cx, cy);
	
	CRect reClient, reLeft;
	GetClientRect(reClient);
	
	reClient.top += 7;
	reClient.bottom -= 11;
	reClient.left += 7;
	reClient.right -= 7;
	
	reLeft = reClient;
	
	reLeft.right = reLeft.left + 180;
	OnSize_Left(reLeft);		//# left
	
	reLeft.left = reLeft.right + 3;
	reLeft.right = reLeft.left + 74;
	OnSize_Move(reLeft);		//# move
	
	reLeft.left = reLeft.right + 3;
	reLeft.right = reClient.right;
	OnSize_Right(reLeft);		//# right

	OnPF_Contorl_Move();
}

void CNewsSetWnd::OnSize_Left(CRect reClient) 
{
	CRect reTop, reBottom, reTemp;
	reTop = reClient;
	reBottom = reClient;
	
	reTop.bottom = reTop.top + 11;
	reTemp = reTop;
	
	reTemp.right = reTemp.left + 3;
	m_ctrlLeftTitlePoint.MoveWindow(&reTemp);
	
	reTemp.left = reTemp.right;// + 6;
	reTemp.right = reTop.right;
	m_ctrlLeftTitle.MoveWindow(&reTemp);
	
	reBottom.top = reBottom.bottom - 20;
	reTemp = reBottom;
	
	//# Change Samsung! (2007.1 : Danny)
	if (m_bReg_UserKeyword == TRUE)
	{
		reTemp.left = reTemp.right - 20;
		m_btnTip.MoveWindow(&reTemp);
		
		int nWidth_Btn = (reBottom.Width() - 24) / 2;
		reTemp.right = reTemp.left - 2;
		reTemp.left = reTemp.right - nWidth_Btn;
		m_btnDelete.MoveWindow(&reTemp);
		
		reTemp.right = reTemp.left - 2;
		reTemp.left = reTemp.right - nWidth_Btn;
		m_btnAdd.MoveWindow(&reTemp);
		
		reBottom.bottom = reBottom.top - 2;
		reBottom.top = reBottom.bottom - 20;
		reTemp = reBottom;
	}
	
	reTemp.left = reTemp.right - 20;
	m_btnNext.MoveWindow(&reTemp);
	
	reTemp.right = reTemp.left - 1;
	reTemp.left = reTemp.right - 20;
	m_btnPre.MoveWindow(&reTemp);
	
	reTemp.right = reTemp.left - 1;
	reTemp.left = reTemp.right - 20;
	m_btnFind.MoveWindow(&reTemp);
	
	reTemp.right = reTemp.left - 1;
	reTemp.left = reBottom.left + 25;
	reTemp.top += 1;
	reTemp.bottom = reTemp.top + 18;
	m_editFind.MoveWindow(&reTemp);
	
	reTemp.right = reTemp.left - 1;
	reTemp.left = reBottom.left;
	reTemp.top += 3;
	reTemp.bottom -= 1;
	m_ctrlFind.MoveWindow(&reTemp);
	
	
	
	reTop.top = reTop.bottom + 11;
	reTop.bottom = reBottom.top - 4;
	m_gridKeyword.MoveWindow(&reTop);
}

void CNewsSetWnd::OnSize_Move(CRect reClient) 
{
	CRect reTop;
	
	reClient.left += 3;
	reClient.right -= 3;
	
	int nHeight_Btn = 18;
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

void CNewsSetWnd::OnSize_Right(CRect reClient) 
{
	CRect reTop, reTemp;
	
	reTop = reClient;
	
	reTop.bottom = reTop.top + 11;
	reTemp = reTop;
	reTemp.right = reTemp.left + 3;
	m_ctrlRightTitlePoint.MoveWindow(&reTemp);
	
	reTemp.left = reTemp.right + 6;
	reTemp.right = reTop.right;
	m_ctrlRightTitle.MoveWindow(&reTemp);
	
	reTemp = reTop;
	reTemp.right -= 5;
	reTemp.left = reTemp.right - 105;
	m_ctrlAddCnt.MoveWindow(&reTemp);
	
	reTop.top = reTop.bottom + 11;
	reTop.bottom = reClient.bottom;
	m_gridSetSignal.MoveWindow(&reTop);
}

void CNewsSetWnd::SetLoadFile()
{
	CString strAppPath;
	GetDir_DrfnRoot(strAppPath);
	
	CString strFilePath = strAppPath + FOLDER_DATA + FOLDER_NEWS + NEWS_POSTURE_FILE;
	SetLoad_KeyWord(0, strFilePath);
	
	//# Change Samsung! (2007.1 : Danny)
	if (m_bReg_UserKeyword == TRUE)
	{
		strFilePath = strAppPath + FOLDER_DATA + FOLDER_NEWS + NEWS_POSTUREUSER_FILE;
		SetLoad_KeyWord(1, strFilePath);
	}
}

void CNewsSetWnd::SetLoad_KeyWord(int nType, CString strFilePath)
{
	int nFindPos, nIndex;
	CString strTemp;
	
	CFileFind			ffFile;
	if (ffFile.FindFile(strFilePath) == FALSE)
	{
		TRACE("\n Load File 실패! : %s", strFilePath);
		return;
	}
	
	CStdioFile fPostureData(strFilePath, CFile::modeRead);
	KeywordData		stKeywordData;
	while (fPostureData.ReadString(strTemp))
	{
		CString strData[10];
		nIndex = 0;
		while (nIndex < 10)
		{
			nFindPos = strTemp.Find(":", 0);
			
			strData[nIndex] = strTemp.Mid(0, nFindPos); 
			strTemp = strTemp.Mid(nFindPos + 1, strTemp.GetLength() - nFindPos);
			nIndex++;
		}
		
		stKeywordData.nType = nType;
		
		stKeywordData.strCode = strData[5];
		stKeywordData.strTitle = strData[2];
		
		strData[4].TrimRight(" ");
		if (strData[4].IsEmpty() == FALSE)
			stKeywordData.strTitle += "+" + strData[4];
		stKeywordData.nBasePoint = atoi(strData[6]);
		
		stKeywordData.nPoint_S = atoi(strData[7]);
		stKeywordData.strCode_S = strData[1];
		stKeywordData.strName_S = strData[2];
		
		stKeywordData.nPoint_V = atoi(strData[8]);
		stKeywordData.strCode_V = strData[3];
		stKeywordData.strName_V = strData[4];
		
		m_listUserData.AddTail(stKeywordData);
		m_gridKeyword.SetAdd_Row(stKeywordData);
	}
	m_gridKeyword.RedrawAll();
	
	fPostureData.Close();	
}

void CNewsSetWnd::SetSaveFile()
{
	if (m_listUserData.GetCount() == 0)
		return;
	
	CString strAppPath;
	GetDir_DrfnRoot(strAppPath);
	CString strFilePath = strAppPath + FOLDER_DATA + FOLDER_NEWS + NEWS_POSTUREUSER_FILE;
	
	POSITION pos;
	KeywordData		stKeywordData;
	CStdioFile fPostureData(strFilePath, CFile::modeCreate | CFile::modeWrite);
	
	CString strData;
	int nIndex_User = 0;
	for (int nIndex = 0; nIndex < m_listUserData.GetCount(); nIndex++)
	{
		pos = m_listUserData.FindIndex(nIndex);
		stKeywordData = m_listUserData.GetAt(pos);
		
		if (stKeywordData.nType == 1)
		{
			strData.Format("%d:%.6s:%s:%.6s:%s:%.6s:%d:%d:%d:\n",
				nIndex_User,
				stKeywordData.strCode_S,
				stKeywordData.strName_S,
				stKeywordData.strCode_V,
				stKeywordData.strName_V,
				stKeywordData.strCode,
				stKeywordData.nBasePoint,
				stKeywordData.nPoint_S,
				stKeywordData.nPoint_V);
			
			fPostureData.WriteString(strData);
			
			nIndex_User++;
		}
	}
	
	fPostureData.Close();
}

//void CNewsSetWnd::OnNotify_SignalGrid(WPARAM wParam, LPARAM lParam)
LRESULT CNewsSetWnd::OnNotify_SignalGrid(WPARAM wParam, LPARAM lParam)
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
			m_mapSignalSet.SetAt(psaData->GetAt(nIndex), pstSetSignalData);
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

//void CNewsSetWnd::OnNotify_Grid(WPARAM wParam, LPARAM lParam)
LRESULT CNewsSetWnd::OnNotify_Grid(WPARAM wParam, LPARAM lParam)
{
	KeywordData		stKeywordData;
	if (wParam == NM_ITEM_CLK)
	{
		stKeywordData = *(KeywordData*)lParam;
		
		if (stKeywordData.nType == 1)
			m_btnDelete.EnableWindow(TRUE);
		else
			m_btnDelete.EnableWindow(FALSE);
	}
	else if (wParam == NM_ITEM_DBCLK)
	{
		stKeywordData = *(KeywordData*)lParam;
		SetAdd_SetSignalData(stKeywordData);
	}

	return TRUE;
}

void CNewsSetWnd::OnBtnLeft()
{
	int nRow;
	nRow = m_gridSetSignal.GetSelectRow();
	
	if (m_gridSetSignal.GetNumberRows() == 0)
	{
		m_btnLeft.EnableWindow(FALSE);
		m_btnLeftAll.EnableWindow(FALSE);
		
		m_btnRight.EnableWindow(TRUE);
		m_btnRightAll.EnableWindow(TRUE);
		
		SetClear_SignalSet(&m_mapSignalSet);
	}
	else
	{
		SetDelete_SetSignalData(nRow);
		
		m_btnLeft.EnableWindow(TRUE);
		m_btnLeftAll.EnableWindow(TRUE);
		
		m_btnRight.EnableWindow(TRUE);
		m_btnRightAll.EnableWindow(TRUE);
	}
	//Invalidate();
	OnPF_Contorl_Move();
}

void CNewsSetWnd::OnBtnLeftAll()
{
	m_btnLeft.EnableWindow(FALSE);
	m_btnLeftAll.EnableWindow(FALSE);
	
	m_btnRight.EnableWindow(TRUE);
	m_btnRightAll.EnableWindow(TRUE);
	
	SetClear_SignalSet(&m_mapSignalSet);
	m_gridSetSignal.ClearAll();
	
	CString strTemp;
	strTemp.Format("적용항목수 : %d", m_gridSetSignal.GetNumberRows());
	m_ctrlAddCnt.SetWindowText(strTemp);
	
	//Invalidate();
	OnPF_Contorl_Move();
}

CString CNewsSetWnd::GetSignalDataToKey(SetSignalData	*pstSetSignalData)
{
	CString strKey = "";
	
	if (pstSetSignalData->nType == 0)		//# Keyword
	{
		if (pstSetSignalData->stKeywordData.nType == 0)
			strKey = pstSetSignalData->stKeywordData.strCode;
		else if (pstSetSignalData->stKeywordData.nType == 1)
			strKey = pstSetSignalData->stKeywordData.strName_S;
	}
	else if (pstSetSignalData->nType == 0)		//# Condition
		strKey = pstSetSignalData->stConditionData.strCode;
	
	return strKey;
}

void CNewsSetWnd::SetAdd_SetSignalData(KeywordData stKeywordData, BOOL bMsg)
{
	SetSignalData	stSetSignalData;
	GetData_DefultSignal(stKeywordData, stSetSignalData);
	
	SetSignalData	*pstSetSignalData = NULL;
	if (m_gridSetSignal.GetHaveState(stSetSignalData.strTitle) == FALSE)
	{
		pstSetSignalData = new SetSignalData;
		SETINIT_SIGNALDATA(pstSetSignalData);
		SETCOPY_SIGNALDATA(stSetSignalData, pstSetSignalData); 
		
		m_mapSignalSet.SetAt(pstSetSignalData->strKey, (void*&)pstSetSignalData);
		m_gridSetSignal.SetAdd_Row(pstSetSignalData, TRUE);
		
		CString strTemp;
		strTemp.Format("적용항목수 : %d", m_gridSetSignal.GetNumberRows());
		m_ctrlAddCnt.SetWindowText(strTemp);
		
		if (bMsg == TRUE)
		{
			int nTotCnt_Keyword;
			nTotCnt_Keyword = m_listUserData.GetCount();
			if (m_gridSetSignal.GetNumberRows() == nTotCnt_Keyword)
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
			SetMessageBox("선택 데이터는 이미 추가되었습니다.");
	}
	OnPF_Contorl_Move();
}

void CNewsSetWnd::OnBtnRight()
{
	KeywordData		stKeywordData;
	m_gridKeyword.GetData_Row(m_gridKeyword.GetSelectRow(), stKeywordData);
	SetAdd_SetSignalData(stKeywordData);
	OnPF_Contorl_Move();
}

void CNewsSetWnd::GetData_DefultSignal(KeywordData stKeywordData, SetSignalData	&stSetSignalData)
{
	stSetSignalData.nIndex = 0;
	
	if (stKeywordData.nType == 0)
		stSetSignalData.strKey = stKeywordData.strCode;
	else if (stKeywordData.nType == 1)
		stSetSignalData.strKey = stKeywordData.strName_S;
	
	stSetSignalData.nType = 0;
	stSetSignalData.stKeywordData = stKeywordData;
	
	if (stKeywordData.strName_V.IsEmpty() == TRUE)
		stSetSignalData.strTitle = stKeywordData.strName_S;
	else
		stSetSignalData.strTitle = stKeywordData.strName_S + "+" + stKeywordData.strName_V;
	stSetSignalData.nPoint = stKeywordData.nBasePoint;
	
	stSetSignalData.nSend_Signal = 1;
	stSetSignalData.nSend_Ticker = 0;
	stSetSignalData.nSend_Order = 0;
	
	stSetSignalData.nState = 1;
}

void CNewsSetWnd::OnBtnRightAll()
{
	POSITION pos;
	KeywordData		stKeywordData;
	for (int nIndex = 0; nIndex < m_listUserData.GetCount(); nIndex++)
	{
		pos = m_listUserData.FindIndex(nIndex);
		stKeywordData = m_listUserData.GetAt(pos);
		
		SetAdd_SetSignalData(stKeywordData, FALSE);
	}
	
	m_btnLeft.EnableWindow(TRUE);
	m_btnLeftAll.EnableWindow(TRUE);
	
	m_btnRight.EnableWindow(FALSE);
	m_btnRightAll.EnableWindow(FALSE);
	OnPF_Contorl_Move();
}

void CNewsSetWnd::OnBtnFind()
{
	CString strData;
	m_editFind.GetWindowText(strData);
	m_gridKeyword.GetFind_Keyword(strData, m_iaRow_FindKeyword);
	
	if (m_iaRow_FindKeyword.GetSize() <= 1)
	{
		m_btnPre.EnableWindow(FALSE);
		m_btnNext.EnableWindow(FALSE);
	}
	else if (m_iaRow_FindKeyword.GetSize() > 0)
	{
		m_nRow_FindKeyword = 0;
		m_gridKeyword.SetGoto(m_iaRow_FindKeyword.GetAt(0));
		
		m_btnPre.EnableWindow(FALSE);
		m_btnNext.EnableWindow(TRUE);
	}
	else
		SetMessageBox("검색내용이 존재하지 않습니다.");
}

void CNewsSetWnd::OnBtnPre()
{
	if (m_nRow_FindKeyword < 0)
		m_btnPre.EnableWindow(FALSE);
	
	m_nRow_FindKeyword--;
	
	if (m_nRow_FindKeyword == 0)
	{
		m_gridKeyword.SetGoto(m_iaRow_FindKeyword.GetAt(0));
		m_btnPre.EnableWindow(FALSE);
		
		if (m_iaRow_FindKeyword.GetSize() == 0)
			m_btnNext.EnableWindow(FALSE);
		else
			m_btnNext.EnableWindow(TRUE);
	}
	else
		m_gridKeyword.SetGoto(m_iaRow_FindKeyword.GetAt(m_nRow_FindKeyword));
}

void CNewsSetWnd::OnBtnNext()
{
	if (m_iaRow_FindKeyword.GetSize() < m_nRow_FindKeyword)
		return;
	
	m_nRow_FindKeyword++;
	if (m_iaRow_FindKeyword.GetSize() - 1 < m_nRow_FindKeyword)
	{
		if (m_iaRow_FindKeyword.GetSize() == 0)
			m_btnPre.EnableWindow(FALSE);
		else
			m_btnPre.EnableWindow(TRUE);
		
		m_btnNext.EnableWindow(FALSE);
	}
	else
		m_gridKeyword.SetGoto(m_iaRow_FindKeyword.GetAt(m_nRow_FindKeyword));
}

void CNewsSetWnd::OnBtnAdd()
{
	HINSTANCE hInstSave = AfxGetResourceHandle();
	AfxSetResourceHandle(g_hInstance);

	CRegKeywordDlg	dlgRegKeyword(this, &m_listUserData);
	if (dlgRegKeyword.DoModal() == IDOK)
	{
		KeywordData		stKeywordData;
		stKeywordData.nType = 1;
		
		CString strTitle;
		strTitle = dlgRegKeyword.m_saSetData.GetAt(0);
		if ((dlgRegKeyword.m_saSetData.GetAt(1)).IsEmpty() == FALSE)
			strTitle += "+" + dlgRegKeyword.m_saSetData.GetAt(1);
		
		stKeywordData.strTitle = strTitle;
		
		stKeywordData.strName_S = dlgRegKeyword.m_saSetData.GetAt(0);		    //# S
		stKeywordData.nPoint_S = 0;
		stKeywordData.strCode_S = "";
		
		stKeywordData.strName_V = dlgRegKeyword.m_saSetData.GetAt(1);		    //# V
		stKeywordData.nPoint_V = 0;
		stKeywordData.strCode_V = "";
		
		stKeywordData.nBasePoint = atoi(dlgRegKeyword.m_saSetData.GetAt(2));//# Point
		
		m_listUserData.AddTail(stKeywordData);
		
		int nRow = m_gridKeyword.SetAdd_Row(stKeywordData);
		m_gridKeyword.SetGoto(nRow, TRUE);
	}
	
	AfxSetResourceHandle(hInstSave);
}

POSITION CNewsSetWnd::GetUserKeywordPos(CString strName_S, CString strName_V)
{
	POSITION pos;
	KeywordData		stKeywordData;
	for (int nIndex = 0; nIndex < m_listUserData.GetCount(); nIndex++)
	{
		pos = m_listUserData.FindIndex(nIndex);
		stKeywordData = m_listUserData.GetAt(pos);
		
		if ((stKeywordData.nType == 1) &&
			(stKeywordData.strName_S == strName_S) &&
			(stKeywordData.strName_V == strName_V))
			return pos;
	}
	
	return NULL;
}

void CNewsSetWnd::OnBtnDelete()
{
	KeywordData		stKeywordData;
	
	int nSelRow = m_gridKeyword.GetSelectRow();
	if (nSelRow < 0)
	{
		SetMessageBox("삭제할 상황문을 선택하세요.");
		return;
	}
	
	m_gridKeyword.GetData_Row(nSelRow, stKeywordData);
	
	CString strMsg;
	strMsg = stKeywordData.strName_S;
	if (stKeywordData.strName_V.IsEmpty() == FALSE)
		strMsg += "+" + stKeywordData.strName_V;
	
	if (SetMessageBox(strMsg + "을(를) 삭제하시겠습니까?", "상황문 삭제", MB_OKCANCEL | MB_TOPMOST) == IDOK)
	{
		m_gridKeyword.SetDelete_Row(nSelRow);
		
		POSITION pos = GetUserKeywordPos(stKeywordData.strName_S, stKeywordData.strName_V);
		if (pos != NULL)
		{
			int nRow = m_gridSetSignal.GetTitleToRow(stKeywordData.strTitle);
			if (nRow > -1)
				SetDelete_SetSignalData(nRow);
			m_listUserData.RemoveAt(pos);
		}
	}
}

void CNewsSetWnd::SetDelete_SetSignalData(int nRow)
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

void CNewsSetWnd::OnBtnTip()
{
	m_tipInfo.UpdateToolTip(this, (LPCTSTR)m_strInfoTip, m_nTipMaxLength);
	m_tipInfo.Show(m_strInfoTip);
}

int CNewsSetWnd::SetMessageBox(LPCTSTR lpszText, LPCTSTR lpszCaption, UINT nType)
{
	CMainFrmWnd *pwndMainFrm = (CMainFrmWnd*)GetParent();
	return pwndMainFrm->SetMessageBox(lpszText, lpszCaption, nType);
}

HBRUSH CNewsSetWnd::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CWnd::OnCtlColor(pDC, pWnd, nCtlColor);
	
	if (nCtlColor == CTLCOLOR_STATIC)
	{
		if ((pWnd->GetDlgCtrlID() == IDC_SIATIC_SUBTITLEPOINT) ||
			(pWnd->GetDlgCtrlID() == IDC_SIATIC_SUBTITLEPOINT + 1))
		{
			pDC->SetBkMode(TRANSPARENT);
			pDC->SetBkColor(COLOR_SUBTITLE_POINT);
			hbr = m_brushSubTitlePoint;
		}
		else
		{
			if ((pWnd->GetDlgCtrlID() == IDC_SIATIC_SUBTITLE) ||
				(pWnd->GetDlgCtrlID() == IDC_SIATIC_SUBTITLE + 1))
				pDC->SetTextColor(COLOR_SUBTITLE_TEXT);
			
			pDC->SetBkMode(TRANSPARENT);
			hbr = m_brushStatic;
		}
	}
	
	return hbr;
}

void CNewsSetWnd::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	CRect reClient;
	GetClientRect(reClient);
	dc.FillSolidRect(reClient, COLOR_SETWND_FACE);
	
	SetDrewBox(&dc, reClient);
}

void CNewsSetWnd::SetDrewBox(CPaintDC *pDC, CRect reData)
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

BOOL CNewsSetWnd::PreTranslateMessage(MSG* pMsg) 
{
	if (m_editFind.m_hWnd == pMsg->hwnd)
	{
		if (pMsg->message == WM_KEYDOWN)
		{
			if (pMsg->wParam == VK_RETURN)
				OnBtnFind();
				/*
				else if (pMsg->wParam == VK_UP)
				OnBtnPre();
				else if (pMsg->wParam == VK_DOWN)
				OnBtnNext();
			*/
		}
	}
	
	return CWnd::PreTranslateMessage(pMsg);
}

void CNewsSetWnd::OnPF_Contorl_Move()
{
	if(!GetParent()) return;
	
	ST_MOVEITEM stItem;	
	stItem.nType = PF_ITEM_BTN;	
	
	SET_ST_MOVEITEM(stItem, "btnNewsAddAll", &m_btnRightAll);	
	GetParent()->SendMessage(MF_PFCONTORL_MOVE, m_bShow_PF_CTL, (LPARAM)&stItem);
	
	SET_ST_MOVEITEM(stItem, "btnNewsAdd", &m_btnRight);
	GetParent()->SendMessage(MF_PFCONTORL_MOVE, m_bShow_PF_CTL, (LPARAM)&stItem);
	
	SET_ST_MOVEITEM(stItem, "btnNewsRemoveAll", &m_btnLeftAll);
	GetParent()->SendMessage(MF_PFCONTORL_MOVE, m_bShow_PF_CTL, (LPARAM)&stItem);

	SET_ST_MOVEITEM(stItem, "btnNewsRemove", &m_btnLeft);
	GetParent()->SendMessage(MF_PFCONTORL_MOVE, m_bShow_PF_CTL, (LPARAM)&stItem);
}

void CNewsSetWnd::PlatFormBtnNewsAddAll()
{
	OnBtnRightAll();
}
void CNewsSetWnd::PlatFormBtnNewsAdd()
{
	OnBtnRight();
}
void CNewsSetWnd::PlatFormBtnNewsRemoveAll()
{
	OnBtnLeftAll();
}
void CNewsSetWnd::PlatFormBtnNewsRemove()
{
	OnBtnLeft();
}

