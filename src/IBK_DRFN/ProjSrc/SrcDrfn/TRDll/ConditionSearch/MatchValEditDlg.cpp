// MathValEditDlg.cpp : implementation file
//

#include "stdafx.h"
#include "MatchValEditDlg.h"

#include "MainFrmWnd.h"
#include "AddControl/Grid_Comm.h"
#include "AddControl/Grid_Result.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define DLG_WIDTH			450 + 30
#define DLG_HEIGHT		350

extern HINSTANCE g_hInstance;

/////////////////////////////////////////////////////////////////////////////
// CMatchValEditDlg dialog
CMatchValEditDlg::CMatchValEditDlg(CWnd* pParent /*=NULL*/)
: CDialog(CMatchValEditDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CMatchValEditDlg)
	// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_pwndMain = pParent;
	m_bExpLogic = FALSE;

	for (int nIndex = 0; nIndex < _countof(MATCHVAL_OPT_LIST); nIndex++)
		m_saOpt.Add((CString)MATCHVAL_OPT_LIST[nIndex]);
}


void CMatchValEditDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMatchValEditDlg)
	// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CMatchValEditDlg, CDialog)
	//{{AFX_MSG_MAP(CMatchValEditDlg)
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_WM_SIZE()
	ON_WM_PAINT()
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP

	ON_BN_CLICKED(IDC_BTN_MATCH_OK, OnBtnOk)
	ON_BN_CLICKED(IDC_BTN_MATCH_CANCEL, OnBtnCancel)

	ON_BN_CLICKED(IDC_BTN_LEFTMOVE, OnBtnLeft)
	ON_BN_CLICKED(IDC_BTN_LEFTMOVE_ALL, OnBtnLeftAll)
	ON_BN_CLICKED(IDC_BTN_RIGHTMOVE, OnBtnRight)
	ON_BN_CLICKED(IDC_BTN_RIGHTMOVE_ALL, OnBtnRightAll)

	ON_BN_CLICKED(IDC_BTN_UPPOS, OnBtnUp)
	ON_BN_CLICKED(IDC_BTN_TOPPOS, OnBtnTop)
	ON_BN_CLICKED(IDC_BTN_DOWNPOS, OnBtnDown)
	ON_BN_CLICKED(IDC_BTN_BOTTOMPOS, OnBtnBottom)


	ON_BN_CLICKED(IDC_BTN_MATCH_EXP, OnBtnExp)

	ON_MESSAGE(WM_NOTIFY_GRIDEVENT, OnNotifyGrid)

END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMatchValEditDlg message handlers

int CMatchValEditDlg::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;

	HINSTANCE hInstSave = AfxGetResourceHandle();
	AfxSetResourceHandle(g_hInstance);
	
	LOGFONT	  logFont;
	GETFONT(logFont, 12, 400, m_fontDefault);
	GETFONT(logFont, 12, 600, m_fontBold);
	m_brushBk.CreateSolidBrush(COLOR_DLG_CANVAS);
	m_brusEdit.CreateSolidBrush(RGB(255,255,255));

	CMainFrmWnd *pwndMainFrm = (CMainFrmWnd *)m_pwndMain;

	m_pgridSource = new CGrid_Comm(this, "MatchValSource", pwndMainFrm->GetAxColor(74));
	m_pgridSource->CreateGrid(
		WS_CHILD | WS_VISIBLE | WS_CLIPCHILDREN | WS_CLIPSIBLINGS | SS_SIMPLE | WS_BORDER,
		CRect(0, 0, 0, 0), this, ID_GRID_MATCHSOURCE);

	m_pgridDest = new CGrid_Comm(this, "MatchValDest", pwndMainFrm->GetAxColor(74));
	m_pgridDest->CreateGrid(
		WS_CHILD | WS_VISIBLE | WS_CLIPCHILDREN | WS_CLIPSIBLINGS | SS_SIMPLE | WS_BORDER,
		CRect(0, 0, 0, 0), this, ID_GRID_MATCHDEST); 

	m_btnLeft.Create(_T("추가"), CRect(0, 0, 0, 0), this, IDC_BTN_LEFTMOVE);
	pwndMainFrm->SetBtnImg(&m_btnLeft, 4);

	m_btnLeftAll.Create(_T("전체추가"), CRect(0, 0, 0, 0), this, IDC_BTN_LEFTMOVE_ALL);
	pwndMainFrm->SetBtnImg(&m_btnLeftAll, 4);

	m_btnRight.Create(_T("삭제"), CRect(0, 0, 0, 0), this, IDC_BTN_RIGHTMOVE);
	pwndMainFrm->SetBtnImg(&m_btnRight, 4);

	m_btnRightAll.Create(_T("전체삭제"), CRect(0, 0, 0, 0), this, IDC_BTN_RIGHTMOVE_ALL);
	pwndMainFrm->SetBtnImg(&m_btnRightAll, 4);

	m_ctrlPosBar.Create(_T(""), WS_CHILD | WS_VISIBLE,// | WS_BORDER,
		CRect(0, 0, 0, 0), this);

	m_btnUp.Create(_T("위로"), CRect(0, 0, 0, 0), this, IDC_BTN_UPPOS);
	pwndMainFrm->SetBtnImg(&m_btnUp, 4);

	m_btnTop.Create(_T("맨위로"), CRect(0, 0, 0, 0), this, IDC_BTN_TOPPOS);
	pwndMainFrm->SetBtnImg(&m_btnTop, 4);

	m_btnDown.Create(_T("아래"), CRect(0, 0, 0, 0), this, IDC_BTN_DOWNPOS);
	pwndMainFrm->SetBtnImg(&m_btnDown, 4);

	m_btnBottom.Create(_T("맨아래"), CRect(0, 0, 0, 0), this, IDC_BTN_BOTTOMPOS);
	pwndMainFrm->SetBtnImg(&m_btnBottom, 4);

	m_ctrlLogicTitle.Create(_T("값비교"), WS_CHILD | WS_VISIBLE,
		CRect(0, 0, 0, 0), this);
	m_ctrlLogicTitle.SetFont(&m_fontDefault);

	m_editLogic.Create(WS_CHILD | WS_VISIBLE | ES_MULTILINE | ES_READONLY,
		CRect(0, 0, 0, 0), this, IDC_EDIT_MATCH);
	m_editLogic.ModifyStyleEx(0, WS_EX_CLIENTEDGE);
	m_editLogic.SetFont(&m_fontDefault);

	m_btnExp.Create(_T(""), CRect(0, 0, 0, 0), this, IDC_BTN_MATCH_EXP);
	pwndMainFrm->SetBtnImg(&m_btnExp, "CS_Logic_Exp_Up");

	m_ctrlComment.Create(_T("조건 저장시 값비교 조건은 저장되지 않습니다."), 
		WS_CHILD | WS_VISIBLE,// | WS_BORDER,
		CRect(0, 0, 0, 0), this);
	m_ctrlComment.SetFont(&m_fontBold);

	m_btnOk.Create(_T("확인"), CRect(0, 0, 0, 0), this, IDC_BTN_MATCH_OK);
	pwndMainFrm->SetBtnImg(&m_btnOk, 4);

	m_btnCancel.Create(_T("취소"), CRect(0, 0, 0, 0), this, IDC_BTN_MATCH_CANCEL);
	pwndMainFrm->SetBtnImg(&m_btnCancel, 4);

	AfxSetResourceHandle(hInstSave);

	CRect reClient;
	GetWindowRect(reClient);

	reClient.right = reClient.left + (int)DLG_WIDTH;
	reClient.bottom = reClient.top + (int)DLG_HEIGHT;
	MoveWindow(reClient);

	return 0;
}


BOOL CMatchValEditDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	this->SetWindowText((CString)PROGRAM_TITLE + " [값비교 편집]");

	CMainFrmWnd *pwndMainFrm = (CMainFrmWnd *)m_pwndMain;
	/*m_listFixHeadInfo = &((CConditionResultWnd*)pwndMainFrm->GetWnd("RESULT"))->m_listFixHeadUser;*/
	
	HeadFixInfo	stHeadFixInfo;
	CList<HeadInfoType, HeadInfoType&> *m_pListField;
	m_pListField = (((CConditionResultWnd*)pwndMainFrm->GetWnd("RESULT"))->m_pGridCtrl->m_plistField);

	POSITION pos;
	for(int i = 0; i < m_pListField->GetCount(); i++)
	{
		stHeadFixInfo.bView = TRUE;
		stHeadFixInfo.nColNum = i + 1;
		
		pos = m_pListField->FindIndex(i);
		stHeadFixInfo.stHeadType = m_pListField->GetAt(pos);
		m_listFixHeadInfo.AddTail(stHeadFixInfo);
	}
	
	int nRow, nIndex;
	nRow = 0;

	//# Init!
	m_pgridSource->ClearAll();
	m_pgridDest->ClearAll();

	for (nIndex = 0; nIndex < m_listFixHeadInfo.GetCount(); nIndex++)
	{
		pos = m_listFixHeadInfo.FindIndex(nIndex);
		stHeadFixInfo = m_listFixHeadInfo.GetAt(pos);

		if (stHeadFixInfo.bView == TRUE)
		{
			m_pgridSource->AppendRow();
			nRow = m_pgridSource->GetNumberRows() - 1;

			m_pgridSource->SetData_MatchValSource(nRow, "", -1, stHeadFixInfo.stHeadType, FALSE);
		}
		else if (stHeadFixInfo.bView == FALSE)
		{
			//# Only Val View Col!
			if (stHeadFixInfo.stHeadType.lHeadID == (int)GRID_MOVEFIELD_POS_INDEX)
				nRow = SetData_ValView(nRow);			//# Insert Val View
		}
	}

	int nMaxRow = 0;
	m_plistMatchInfo = &((CConditionMakeWnd*)pwndMainFrm->GetWnd("MAKE"))->m_listMatchInfo;

	MatchInfo		stMatchInfo;
	//SetClearAll_Grid(m_pgridDest);

	for (nIndex = 0; nIndex < m_plistMatchInfo->GetCount(); nIndex++)
	{
		pos = m_plistMatchInfo->FindIndex(nIndex);
		stMatchInfo = m_plistMatchInfo->GetAt(pos);

		m_pgridSource->SetData_MatchValSource(stMatchInfo.nGridSourceRow, TRUE);

		m_pgridDest->AppendRow();
		nRow = m_pgridDest->GetNumberRows() - 1;

		//# Opt 값 Set 확인!
		m_pgridDest->SetData_MatchValDest(nRow, stMatchInfo, TRUE);
	}
	m_pgridDest->RedrawAll();
	m_pgridSource->RedrawAll();

	if (m_plistMatchInfo->GetCount() > 0)
		m_editLogic.SetWindowText(GetMatchLogic());

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

int CMatchValEditDlg::SetData_ValView(int nOffset)
{
	int nRow = nOffset;

	POSITION pos;
	HeadFixInfo		stHeadFixInfo;
	CList<ConditionGroupInfo, ConditionGroupInfo&> *plistConditionGroup;

	CMainFrmWnd *pwndMainFrm = (CMainFrmWnd*)m_pwndMain;
	plistConditionGroup = &((CConditionMakeWnd*)pwndMainFrm->GetWnd("MAKE"))->m_DataAdmin.m_listConditionGroup;

	ConditionGroupInfo	stConditionGroupInfo;
	HeadMoveInfo				stHeadMoveInfo;
	for (int nIndex = 0; nIndex < plistConditionGroup->GetCount(); nIndex++)
	{
		pos = plistConditionGroup->FindIndex(nIndex);
		stConditionGroupInfo = plistConditionGroup->GetAt(pos);

		if ((stConditionGroupInfo.stOneConditionInfo.bFieldState == TRUE) &&
			(stConditionGroupInfo.stOneConditionInfo.bFieldVal == TRUE))
		{
			stHeadMoveInfo = stConditionGroupInfo.stOneConditionInfo.stHeadMoveInfo;
			for (int nIndex2 = 0; nIndex2 < stHeadMoveInfo.nColCnt; nIndex2++)
			{
				m_pgridSource->AppendRow();
				nRow = m_pgridSource->GetNumberRows() - 1;

				m_pgridSource->SetData_MatchValSource(nRow, stHeadMoveInfo.strElement, nIndex2, stHeadMoveInfo.stHeadType[nIndex2], FALSE);
			}
		}
	}

	return nRow;
}

void CMatchValEditDlg::SetSyncMatchInfo(CList <MatchInfo, MatchInfo&>	*plistSource,
										CList <MatchInfo, MatchInfo&>	*plistDest)
{
	POSITION posSource, posDest;
	MatchInfo	stMatchInfo_Source, stMatchInfo_Dest;
	BOOL bState;

	int nSourceIndex, nDestIndex;
	for (nSourceIndex = 0; nSourceIndex < plistSource->GetCount(); nSourceIndex++)
	{
		posSource = plistSource->FindIndex(nSourceIndex);
		stMatchInfo_Source = plistSource->GetAt(posSource);

		bState = FALSE;
		for (nDestIndex = 0; nDestIndex < plistDest->GetCount(); nDestIndex++)
		{
			posDest = plistDest->FindIndex(nDestIndex);
			stMatchInfo_Dest = plistDest->GetAt(posDest);

			if (stMatchInfo_Dest.strElement == stMatchInfo_Source.strElement)
				bState = TRUE;
		}

		plistDest->AddTail(stMatchInfo_Source);
	}
}



int CMatchValEditDlg::GetComparisonRow_Source(CString strFiled)
{
	int nResult = -1;

	CString strField_Comparison;
	for (int nRow = 0; nRow < m_pgridSource->GetNumberRows(); nRow++)
	{
		strField_Comparison = m_pgridSource->QuickGetText(1, nRow);

		if (strFiled == strField_Comparison)
		{
			nResult = nRow;
			break;
		}
	}

	return nResult;
}

void CMatchValEditDlg::OnDestroy() 
{
	//	CDialog::OnDestroy();

	if (m_pgridSource->GetSafeHwnd() != NULL)
	{
		m_pgridSource->DestroyWindow();
		delete m_pgridSource;
	}

	if (m_pgridDest->GetSafeHwnd() != NULL)
	{
		m_pgridDest->DestroyWindow();
		delete m_pgridDest;
	}

	m_fontDefault.DeleteObject();
	m_fontBold.DeleteObject();
	m_brushBk.DeleteObject();
	m_brusEdit.DeleteObject();

	CDialog::OnDestroy();
}

void CMatchValEditDlg::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);
	OnSize();
}

void CMatchValEditDlg::OnSize() 
{
	int btnHeight = 20;

	CRect reClient, reBottom, reTemp;
	GetClientRect(reClient);
	reBottom = reClient;

	reBottom.top += 5;
	reBottom.bottom -= 5;
	reBottom.left += 5;
	reBottom.right -= 5;

	reBottom.top = reBottom.bottom - btnHeight;
	reTemp = reBottom;

	reTemp.left = reTemp.right - 60;
	m_btnCancel.MoveWindow(reTemp);

	reTemp.right = reTemp.left - 2;
	reTemp.left = reTemp.right - 60;
	m_btnOk.MoveWindow(reTemp);

	reTemp.top += 5;
	reTemp.right = reTemp.left - 2;
	reTemp.left = reBottom.left;
	m_ctrlComment.MoveWindow(reTemp);


	reBottom.bottom = reBottom.top - 3;

	if (m_bExpLogic == TRUE)
		reBottom.top = reBottom.bottom - btnHeight * 3;
	else
		reBottom.top = reBottom.bottom - btnHeight;
	reTemp = reBottom;

	reTemp.top = reBottom.top + 3;
	reTemp.bottom = reTemp.top + 14;
	reTemp.right = reTemp.left + 40;
	m_ctrlLogicTitle.MoveWindow(reTemp);

	reTemp.top = reBottom.top;
	reTemp.bottom = reBottom.bottom;
	reTemp.left = reTemp.right + 3;
	reTemp.right = reClient.right - 5 - 22;
	m_editLogic.MoveWindow(reTemp);

	reTemp.top = reBottom.top;
	reTemp.bottom = reTemp.top + btnHeight;
	reTemp.left = reTemp.right + 2;
	reTemp.right = reTemp.left + 20;
	m_btnExp.MoveWindow(reTemp);

	reBottom.bottom = reBottom.top - 3;
	reBottom.top = reClient.top;
	OnSizeTopBar(reBottom);

	Invalidate();
}


void CMatchValEditDlg::OnSizeTopBar(CRect reClient)
{
	CRect reLeft;
	reLeft = reClient;

	reLeft.top += 5;
	reLeft.right = reLeft.left + 180;
	if (m_pgridSource->GetSafeHwnd() != NULL)
		m_pgridSource->MoveWindow(reLeft);

	reLeft.left = reLeft.right;
	reLeft.right = reLeft.left + 60 + 10;
	OnSizeMoveBar(reLeft);

	reLeft.left = reLeft.right;
	reLeft.right = reClient.right;
	OnSizeMakeBar(reLeft);
}

void CMatchValEditDlg::OnSizeMoveBar(CRect reClient)
{
	CRect reTemp;
	reTemp = reClient;

	reTemp.left += 5;
	reTemp.right -= 5;

	int nBtnHeight = 20;
	int nTopOffset;
	nTopOffset = ((reClient.bottom - reClient.top) - (nBtnHeight * 4 + 2 * 5)) / 2;

	reTemp.top += nTopOffset;
	reTemp.bottom = reTemp.top + nBtnHeight;
	/*m_btnLeftAll.MoveWindow(reTemp);*/
	m_btnLeftAll.ShowWindow(SW_HIDE);
	m_btnTop.MoveWindow(reTemp);

	reTemp.top = reTemp.bottom + 3;
	reTemp.bottom = reTemp.top + nBtnHeight;
	/*m_btnLeft.MoveWindow(reTemp);*/
	m_btnLeft.ShowWindow(SW_HIDE);
	m_btnUp.MoveWindow(reTemp);

	reTemp.top = reTemp.bottom + 3;
	reTemp.bottom = reTemp.top + nBtnHeight;
	/*m_btnRight.MoveWindow(reTemp);*/
	m_btnRight.ShowWindow(SW_HIDE);
	m_btnDown.MoveWindow(reTemp);

	reTemp.top = reTemp.bottom + 3;
	reTemp.bottom = reTemp.top + nBtnHeight;
	/*m_btnRightAll.MoveWindow(reTemp);*/
	m_btnRightAll.ShowWindow(SW_HIDE);
	m_btnBottom.MoveWindow(reTemp);
}


void CMatchValEditDlg::OnSizeMakeBar(CRect reClient)
{
	CRect reBottom, reTemp;
	reBottom = reClient;

	reBottom.top = reBottom.bottom - 26;
/*	m_ctrlPosBar.MoveWindow(reBottom);*/

	reTemp = reBottom;
	reTemp.top += 3;
	reTemp.bottom -= 3;

	int nWidth_MoveBtn = GetBtnWidth(3);//34;

	int nLeftOffset;
	nLeftOffset = reTemp.right - reTemp.left;
	nLeftOffset = (nLeftOffset - (nWidth_MoveBtn * 4 + 2 * 3)) / 2;

// 	reTemp.left += nLeftOffset;
// 	reTemp.right = reTemp.left + nWidth_MoveBtn;
// 	m_btnTop.MoveWindow(reTemp);
// 
// 	reTemp.left = reTemp.right + 2;
// 	reTemp.right = reTemp.left + nWidth_MoveBtn;
// 	m_btnUp.MoveWindow(reTemp);
// 
// 	reTemp.left = reTemp.right + 2;
// 	reTemp.right = reTemp.left + nWidth_MoveBtn;
// 	m_btnDown.MoveWindow(reTemp);
// 
// 	reTemp.left = reTemp.right + 2;0.

// 	reTemp.right = reTemp.left + nWidth_MoveBtn;
// 	m_btnBottom.MoveWindow(reTemp);

	reBottom.bottom = reClient.bottom;
	reBottom.top = reClient.top;
	if (m_pgridDest->GetSafeHwnd() != NULL)
		m_pgridDest->MoveWindow(reBottom);
}

//void CMatchValEditDlg::OnNotifyGrid(WPARAM wParam, LPARAM lParam)
LRESULT CMatchValEditDlg::OnNotifyGrid(WPARAM wParam, LPARAM lParam)
{
	GridEventInfo		stGridEventInfo;

	stGridEventInfo = *(GridEventInfo*)lParam;
	if (wParam == NM_CLICK_L)
	{

	}
	else if (wParam == NM_CLICK_R)
	{

	}
	else if (wParam == NM_CLICK_CELLTYPE)
	{
		if (stGridEventInfo.strType == "BUTTON")
		{

		}
		else if (stGridEventInfo.strType == "CHECK")
		{			
			CString strState = m_pgridSource->QuickGetText(0, stGridEventInfo.nRow);
			BOOL bState = atoi(strState);

			if(bState)
			{
				SetDest_Insert(stGridEventInfo.nRow);
			}
			else
			{
				SetDest_Delete(stGridEventInfo.nRow);
			}
			
// 			if (stGridEventInfo.strGridType == "MatchValSource")
// 				
// 			else if (stGridEventInfo.strGridType == "MatchValDest")
				
		}
		else if (stGridEventInfo.strType == "COMBO")
		{
			if (stGridEventInfo.strGridType == "MatchValDest")
				m_editLogic.SetWindowText(GetMatchLogic());
		}
	}
	else if (wParam == NM_DBCLICK_L)
	{
		
	}

	return TRUE;
}

void CMatchValEditDlg::OnBtnOk()
{
	//# Data UpDate!
	m_plistMatchInfo->RemoveAll();

	MatchInfo		stMatchInfo;
	for (int nRow = 0; nRow < m_pgridDest->GetNumberRows(); nRow++)
	{
		m_pgridDest->GetData_MatchValDest(nRow, stMatchInfo);
		m_plistMatchInfo->AddTail(stMatchInfo);
	}

	CMainFrmWnd *pwndMainFrm = (CMainFrmWnd *)m_pwndMain;
	((CConditionMakeWnd*)pwndMainFrm->GetWnd("MAKE"))->OnBtnMatchValOk();
	//DestroyWindow();
	OnOK();
}

void CMatchValEditDlg::OnBtnCancel()
{
	OnCancel();
	//DestroyWindow();
}

void CMatchValEditDlg::OnBtnLeft()
{
	int nSelectRow = m_pgridSource->m_nSelectRow;

	if ((nSelectRow > -1) && (m_pgridSource->GetNumberRows() > nSelectRow))
		SetDest_Insert(nSelectRow);
}

void CMatchValEditDlg::SetDest_Insert(int nSelectRow)
{
	MatchInfo	stMatchInfo;
	m_pgridSource->GetData_MatchValSource(nSelectRow, stMatchInfo);
	m_pgridSource->SetData_MatchValSource(nSelectRow, TRUE);

	m_pgridDest->AppendRow();
	int nRow = m_pgridDest->GetNumberRows() - 1;

	stMatchInfo.nOptIndex = 1;
	m_pgridDest->SetData_MatchValDest(nRow, stMatchInfo, TRUE);

	m_editLogic.SetWindowText(GetMatchLogic());
	
	
// 	if (stMatchInfo.bUser == FALSE)
// 	{
// 		
// 		m_pgridSource->SetData_MatchValSource(nSelectRow, TRUE);
// 
// 		m_pgridDest->AppendRow();
// 		int nRow = m_pgridDest->GetNumberRows() - 1;
// 
// 		stMatchInfo.nOptIndex = 1;
// 		m_pgridDest->SetData_MatchValDest(nRow, stMatchInfo, TRUE);
// 
// 		m_editLogic.SetWindowText(GetMatchLogic());
// 	}
// 	else
// 	{
// 		CString strErrorMsg;
// 		strErrorMsg.Format("이미 추가된 필드명입니다.");
// 		SetMessageBox(strErrorMsg, (CString)PROGRAM_TITLE, MB_OK);
// 	}
}

void CMatchValEditDlg::OnBtnLeftAll()
{
	MatchInfo	stMatchInfo_Source, stMatchInfo_Dest;
	int nRow_Source, nRow_Dest, nDestRowCnt;
	BOOL bInsertFlag;

	for (nRow_Source = 0; nRow_Source < m_pgridSource->GetNumberRows(); nRow_Source++)
	{
		if (m_pgridSource->GetData_MatchValSource(nRow_Source, stMatchInfo_Source) == TRUE)
		{
			bInsertFlag = TRUE;
			nDestRowCnt = m_pgridDest->GetNumberRows();
			for (nRow_Dest = 0; nRow_Dest < nDestRowCnt; nRow_Dest++)
			{
				m_pgridDest->GetData_MatchValDest(nRow_Dest, stMatchInfo_Dest);

				if (stMatchInfo_Dest.nFieldIndex == -1)
				{
					if (stMatchInfo_Source.strField == stMatchInfo_Dest.strField)
					{
						bInsertFlag = FALSE;
						break;
					}
				}
				else
				{
					if ((stMatchInfo_Source.strElement == stMatchInfo_Dest.strElement) &&
						(stMatchInfo_Source.strField == stMatchInfo_Dest.strField) &&
						(stMatchInfo_Source.nFieldIndex == stMatchInfo_Dest.nFieldIndex))
					{
						bInsertFlag = FALSE;
						break;
					}
				}
			}

			if (bInsertFlag == TRUE)
			{
				m_pgridSource->SetData_MatchValSource(nRow_Source, TRUE);

				stMatchInfo_Source.nOptIndex = 0;
				m_pgridDest->AppendRow();
				m_pgridDest->SetData_MatchValDest(nDestRowCnt, stMatchInfo_Source, TRUE);
			}
		}
	}

	m_editLogic.SetWindowText(GetMatchLogic());
}

void CMatchValEditDlg::OnBtnRight()
{
	int nSelectRow = m_pgridDest->m_nSelectRow;

	if ((nSelectRow > -1) && (m_pgridDest->GetNumberRows() > nSelectRow))
		SetDest_Delete(nSelectRow);
}

void CMatchValEditDlg::SetDest_Delete(int nSelectRow)
{
	MatchInfo	stMatchInfo;
	MatchInfo	stDestMatchInfo;

	CString strField;

	m_pgridSource->GetData_MatchValSource(nSelectRow, stMatchInfo);
	m_pgridSource->SetData_MatchValSource(nSelectRow, FALSE);

	for(int i = 0; i < m_pgridDest->GetNumberRows(); i++)
	{
		m_pgridDest->GetData_MatchValDest(i , stDestMatchInfo);
		if(stMatchInfo.strField == stDestMatchInfo.strField)
		{
			m_pgridDest->DeleteRow(i);
			break;
		}
	}

	m_editLogic.SetWindowText(GetMatchLogic());
	
	
// 	MatchInfo	stMatchInfo;
// 	m_pgridDest->GetData_MatchValDest(nSelectRow, stMatchInfo);
// 	m_pgridSource->SetData_MatchValSource(stMatchInfo.nGridSourceRow, FALSE);
// 
// 	m_pgridDest->DeleteRow(nSelectRow);
// 
// 	int nRow;
// 	if (nSelectRow > m_pgridDest->GetNumberRows() - 1)
// 		nRow = m_pgridDest->GetNumberRows() - 1;
// 	else if (nSelectRow < m_pgridDest->GetNumberRows() - 1)
// 		nRow = nSelectRow;
// 	m_pgridDest->SetSelRow(nRow);
// 
// 	m_editLogic.SetWindowText(GetMatchLogic());
}

void CMatchValEditDlg::OnBtnRightAll()
{
	m_pgridDest->ClearAll();
	m_editLogic.SetWindowText("");

	for (int nRow = 0; nRow < m_pgridSource->GetNumberRows(); nRow++)
	{
		m_pgridSource->QuickSetText(0, nRow, "0");
		m_pgridSource->SetData_MatchValSource(nRow, FALSE);
	}
}

void CMatchValEditDlg::OnBtnUp()
{
	CString strErrorMsg;
	int nSelectDestRow = m_pgridDest->m_nSelectRow;
	if (nSelectDestRow == 0)
	{
		strErrorMsg.Format("제일 상단의 필드명입니다.");
		SetMessageBox(strErrorMsg, (CString)PROGRAM_TITLE, MB_OK);
		return;
	}
	else if (nSelectDestRow == -1)
	{
		strErrorMsg.Format("선택 필드명이 없습니다.");
		SetMessageBox(strErrorMsg, (CString)PROGRAM_TITLE, MB_OK);
		return;
	}

	MatchInfo	stMatchInfo;
	m_pgridDest->GetData_MatchValDest(nSelectDestRow, stMatchInfo);

	m_pgridDest->DeleteRow(nSelectDestRow);

	m_pgridDest->InsertRow(nSelectDestRow - 1);
	m_pgridDest->SetData_MatchValDest(nSelectDestRow - 1, stMatchInfo, TRUE);

	if (nSelectDestRow == m_pgridDest->GetNumberRows() - 1)
	{
		m_pgridDest->QuickSetCellType(1, nSelectDestRow, UGCT_NORMAL);
		m_pgridDest->QuickSetText(1, nSelectDestRow, "");
		m_pgridDest->RedrawAll();
	}

	m_editLogic.SetWindowText(GetMatchLogic());
}

void CMatchValEditDlg::OnBtnTop()
{
	CString strErrorMsg;
	int nSelectDestRow = m_pgridDest->m_nSelectRow;
	if (nSelectDestRow == 0)
	{
		strErrorMsg.Format("제일 상단의 필드명입니다.");
		SetMessageBox(strErrorMsg, (CString)PROGRAM_TITLE, MB_OK);
		return;
	}
	else if (nSelectDestRow == -1)
	{
		strErrorMsg.Format("선택 필드명이 없습니다.");
		SetMessageBox(strErrorMsg, (CString)PROGRAM_TITLE, MB_OK);
		return;
	}

	MatchInfo	stMatchInfo;
	m_pgridDest->GetData_MatchValDest(nSelectDestRow, stMatchInfo);

	m_pgridDest->DeleteRow(nSelectDestRow);

	m_pgridDest->InsertRow(0);
	m_pgridDest->SetData_MatchValDest(0, stMatchInfo, TRUE);

	if (nSelectDestRow == m_pgridDest->GetNumberRows() - 1)
	{
		m_pgridDest->QuickSetCellType(1, nSelectDestRow, UGCT_NORMAL);
		m_pgridDest->QuickSetText(1, nSelectDestRow, "");
		m_pgridDest->RedrawAll();
	}

	m_editLogic.SetWindowText(GetMatchLogic());
}

void CMatchValEditDlg::OnBtnDown()
{

	CString strErrorMsg;
	int nSelectDestRow = m_pgridDest->m_nSelectRow;
	if (nSelectDestRow == m_pgridDest->GetNumberRows() - 1)
	{
		strErrorMsg.Format("제일 하단의 필드명입니다.");
		SetMessageBox(strErrorMsg, (CString)PROGRAM_TITLE, MB_OK);
		return;
	}
	else if (nSelectDestRow == -1)
	{
		strErrorMsg.Format("선택 필드명이 없습니다.");
		SetMessageBox(strErrorMsg, (CString)PROGRAM_TITLE, MB_OK);
		return;
	}

	MatchInfo	stMatchInfo;
	m_pgridDest->GetData_MatchValDest(nSelectDestRow, stMatchInfo);

	m_pgridDest->DeleteRow(nSelectDestRow);

	m_pgridDest->InsertRow(nSelectDestRow + 1);
	m_pgridDest->SetData_MatchValDest(nSelectDestRow + 1, stMatchInfo, TRUE);

	m_editLogic.SetWindowText(GetMatchLogic());
}

void CMatchValEditDlg::OnBtnBottom()
{
	CString strErrorMsg;
	int nSelectDestRow = m_pgridDest->m_nSelectRow;
	if (nSelectDestRow == m_pgridDest->GetNumberRows() - 1)
	{
		strErrorMsg.Format("제일 하단의 필드명입니다.");
		SetMessageBox(strErrorMsg, (CString)PROGRAM_TITLE, MB_OK);
		return;
	}
	else if (nSelectDestRow == -1)
	{
		strErrorMsg.Format("선택 필드명이 없습니다.");
		SetMessageBox(strErrorMsg, (CString)PROGRAM_TITLE, MB_OK);
		return;
	}

	MatchInfo	stMatchInfo;
	m_pgridDest->GetData_MatchValDest(nSelectDestRow, stMatchInfo);

	m_pgridDest->DeleteRow(nSelectDestRow);

	m_pgridDest->AppendRow();
	int nRow = m_pgridDest->GetNumberRows() - 1;
	m_pgridDest->SetData_MatchValDest(nRow, stMatchInfo, TRUE);

	m_editLogic.SetWindowText(GetMatchLogic());
}


CString CMatchValEditDlg::GetMatchLogic()
{
	CString strResult, strTemp;

	MatchInfo	stMatchInfo;
	int nRowCnt = m_pgridDest->GetNumberRows();
	for (int nRow = 0; nRow < nRowCnt; nRow++)
	{
		m_pgridDest->GetData_MatchValDest(nRow, stMatchInfo);

		strTemp = m_pgridDest->QuickGetText(0, nRow);
		if (nRow < nRowCnt - 1)
			strTemp = strTemp + " " + (CString)MATCHVAL_OPT_LIST[stMatchInfo.nOptIndex] + " ";

		strResult = strResult + strTemp;
	}

	return strResult;
}

void CMatchValEditDlg::OnBtnExp()
{
	CMainFrmWnd *pwndMainFrm = (CMainFrmWnd *)m_pwndMain;
	
	if (m_bExpLogic == TRUE)
		pwndMainFrm->SetBtnImg(&m_btnExp, "CS_Logic_Exp_Down");
	else
		pwndMainFrm->SetBtnImg(&m_btnExp, "CS_Logic_Exp_Up");

	m_bExpLogic = !m_bExpLogic;
	OnSize();
}

int CMatchValEditDlg::SetMessageBox(LPCTSTR lpszText, LPCTSTR lpszCaption, UINT nType)
{
	return ::MessageBox(m_hWnd, lpszText, lpszCaption, nType);
}

void CMatchValEditDlg::OnPaint() 
{
	CPaintDC dc(this); // device context for painting

	CMainFrmWnd *pwndMainFrm = (CMainFrmWnd *)m_pwndMain;

	CRect reClient;
	GetClientRect(reClient);

	dc.FillSolidRect(reClient, pwndMainFrm->GetAxColor(66));	
}

HBRUSH CMatchValEditDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	if (nCtlColor == CTLCOLOR_STATIC)
	{
		CMainFrmWnd *pwndMainFrm = (CMainFrmWnd *)m_pwndMain;

		pDC->SetBkMode(TRANSPARENT);
		hbr = pwndMainFrm->GetAxBrush(pwndMainFrm->GetAxColor(66));;
	}

	if (m_editLogic.GetDlgCtrlID() == pWnd->GetDlgCtrlID())
	{
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetBkColor(RGB(255, 255, 255));
		hbr = m_brusEdit;
	}

	return hbr;
}

BOOL CMatchValEditDlg::PreTranslateMessage(MSG* pMsg) 
{
	if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_RETURN)
		OnBtnOk();

	return CDialog::PreTranslateMessage(pMsg);
}
