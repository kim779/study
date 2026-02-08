// QuickMakeDlg.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include "QuickMakeDlg.h"

#include "MainFrmWnd.h"
#include "AddControl/Grid_Comm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern HINSTANCE g_hInstance;

/////////////////////////////////////////////////////////////////////////////
// CQuickMakeDlg dialog
#define DLG_WIDTH			450
#define DLG_HEIGHT		353


#define QUICK_TITLE_LENGTH				10

CQuickMakeDlg::CQuickMakeDlg(CWnd* pParent)
: CDialog(CQuickMakeDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CQuickMakeDlg)
	// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	
	m_pwndParent = pParent;
}


void CQuickMakeDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CQuickMakeDlg)
	// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CQuickMakeDlg, CDialog)
//{{AFX_MSG_MAP(CQuickMakeDlg)
ON_WM_CREATE()
ON_WM_SIZE()
ON_WM_DESTROY()
ON_WM_PAINT()
ON_WM_CTLCOLOR()
//}}AFX_MSG_MAP

ON_BN_CLICKED(IDC_BTN_OK, OnBtnOk)
ON_BN_CLICKED(IDC_BTN_CANCEL, OnBtnCancel)

ON_BN_CLICKED(IDC_BTN_LEFTMOVE, OnBtnLeft)
ON_BN_CLICKED(IDC_BTN_LEFTMOVE_ALL, OnBtnLeftAll)
ON_BN_CLICKED(IDC_BTN_RIGHTMOVE, OnBtnRight)
ON_BN_CLICKED(IDC_BTN_RIGHTMOVE_ALL, OnBtnRightAll)

ON_BN_CLICKED(IDC_BTN_UPPOS, OnBtnUp)
ON_BN_CLICKED(IDC_BTN_TOPPOS, OnBtnTop)
ON_BN_CLICKED(IDC_BTN_DOWNPOS, OnBtnDown)
ON_BN_CLICKED(IDC_BTN_BOTTOMPOS, OnBtnBottom)

ON_MESSAGE(WM_NOTIFY_GRIDEVENT, OnNotifyGrid)

END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CQuickMakeDlg message handlers

int CQuickMakeDlg::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;

	HINSTANCE hInstSave = AfxGetResourceHandle();
	AfxSetResourceHandle(g_hInstance);
	
	LOGFONT	  logFont;
	GETFONT(logFont, 12, 400, m_fontDefault);
	m_brushBk.CreateSolidBrush(COLOR_DLG_CANVAS);
	
	CMainFrmWnd *pwndMainFrm = (CMainFrmWnd *)m_pwndParent;

	m_pgridSource = new CGrid_Comm(this, "QuickMakeSource", pwndMainFrm->GetAxColor(74));
	m_pgridSource->CreateGrid(
		WS_CHILD | WS_VISIBLE | WS_CLIPCHILDREN | WS_CLIPSIBLINGS | SS_SIMPLE | WS_BORDER,
		CRect(0, 0, 0, 0),
		this,
		IDC_GRID_SOURCE);
	
	m_pgridDest = new CGrid_Comm(this, "QuickMakeDest", pwndMainFrm->GetAxColor(74));
	m_pgridDest->CreateGrid(
		WS_CHILD | WS_VISIBLE | WS_CLIPCHILDREN | WS_CLIPSIBLINGS | SS_SIMPLE | WS_BORDER,
		CRect(0, 0, 0, 0),
		this,
		IDC_GRID_DEST);
	
	COLORREF crBg = COLOR_BUTTON;
	COLORREF crText = COLOR_BLACK;
	
	m_btnLeft.Create(_T(">"), 
		WS_CHILD | WS_VISIBLE | WS_BORDER, 
		CRect(0, 0, 0, 0), 
		this, 
		IDC_BTN_LEFTMOVE);
	m_btnLeft.SetFont(&m_fontDefault);
	m_btnLeft.SetColor(crBg, crText, crBg, crText);
	
	
	m_btnLeftAll.Create(_T(">>"), 
		WS_CHILD | WS_VISIBLE | WS_BORDER, 
		CRect(0, 0, 0, 0), 
		this, 
		IDC_BTN_LEFTMOVE_ALL);
	m_btnLeftAll.SetFont(&m_fontDefault);
	m_btnLeftAll.SetColor(crBg, crText, crBg, crText);
	
	
	m_btnRight.Create(_T("<"), 
		WS_CHILD | WS_VISIBLE | WS_BORDER, 
		CRect(0, 0, 0, 0), 
		this, 
		IDC_BTN_RIGHTMOVE);
	m_btnRight.SetFont(&m_fontDefault);
	m_btnRight.SetColor(crBg, crText, crBg, crText);
	
	m_btnRightAll.Create(_T("<<"), 
		WS_CHILD | WS_VISIBLE | WS_BORDER, 
		CRect(0, 0, 0, 0), 
		this, 
		IDC_BTN_RIGHTMOVE_ALL);
	m_btnRightAll.SetFont(&m_fontDefault);
	m_btnRightAll.SetColor(crBg, crText, crBg, crText);
	
	
	m_ctrlPosBar.Create(_T(""), 
		WS_CHILD | WS_VISIBLE,// | WS_BORDER,
		CRect(0, 0, 0, 0), this);
	
	
	m_btnUp.Create(_T("▲"), 
		WS_CHILD | WS_VISIBLE | WS_BORDER, 
		CRect(0, 0, 0, 0), 
		this, 
		IDC_BTN_UPPOS);
	m_btnUp.SetFont(&m_fontDefault);
	m_btnUp.SetColor(crBg, crText, crBg, crText);
	
	m_btnTop.Create(_T("▲▲"), 
		WS_CHILD | WS_VISIBLE | WS_BORDER, 
		CRect(0, 0, 0, 0), 
		this, 
		IDC_BTN_TOPPOS);
	m_btnTop.SetFont(&m_fontDefault);
	m_btnTop.SetColor(crBg, crText, crBg, crText);
	
	m_btnDown.Create(_T("▼"), 
		WS_CHILD | WS_VISIBLE | WS_BORDER, 
		CRect(0, 0, 0, 0), 
		this, 
		IDC_BTN_DOWNPOS);
	m_btnDown.SetFont(&m_fontDefault);
	m_btnDown.SetColor(crBg, crText, crBg, crText);
	
	m_btnBottom.Create(_T("▼▼"), 
		WS_CHILD | WS_VISIBLE | WS_BORDER, 
		CRect(0, 0, 0, 0), 
		this, 
		IDC_BTN_BOTTOMPOS);
	m_btnBottom.SetFont(&m_fontDefault);
	m_btnBottom.SetColor(crBg, crText, crBg, crText);
	
	
	
	m_btnOk.Create(_T("확인"), 
		WS_CHILD | WS_VISIBLE | WS_BORDER, 
		CRect(0, 0, 0, 0), 
		this, 
		IDC_BTN_OK);
	m_btnOk.SetFont(&m_fontDefault);
	m_btnOk.SetColor(crBg, crText, crBg, crText);
	
	m_btnCancel.Create(_T("취소"), 
		WS_CHILD | WS_VISIBLE | WS_BORDER, 
		CRect(0, 0, 0, 0), 
		this, 
		IDC_BTN_CANCEL);
	m_btnCancel.SetFont(&m_fontDefault);
	m_btnCancel.SetColor(crBg, crText, crBg, crText);

	AfxSetResourceHandle(hInstSave);
		
	CRect reClient;
	GetWindowRect(reClient);
	
	reClient.right = reClient.left + (int)DLG_WIDTH;
	reClient.bottom = reClient.top + (int)DLG_HEIGHT;
	MoveWindow(reClient);
	
	return 0;
}

void CQuickMakeDlg::OnDestroy() 
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
	m_brushBk.DeleteObject();
	
	CDialog::OnDestroy();
}

void CQuickMakeDlg::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);
	
	OnSize();
}

void CQuickMakeDlg::OnSize() 
{
	int btnHeight = 25;
	
	CRect reClient, reBottom, reTemp;
	GetClientRect(reClient);
	reBottom = reClient;
	
	reBottom.top += 5;
	reBottom.bottom -= 5;
	reBottom.left += 5;
	reBottom.right -= 5;
	
	reBottom.top = reBottom.bottom - btnHeight;
	reTemp = reBottom;
	
	reTemp.top += 5;
	reTemp.left = reTemp.right - 60;
	m_btnCancel.MoveWindow(reTemp);
	
	reTemp.right = reTemp.left - 2;
	reTemp.left = reTemp.right - 60;
	m_btnOk.MoveWindow(reTemp);
	
	reTemp.top -= 5;
	
	reTemp.left = reBottom.left;
	reTemp.right = reTemp.left + 180;
	OnSizePosBar(reTemp);
	
	reBottom.bottom = reBottom.top - 3;
	reBottom.top = reClient.top;
	OnSizeTopBar(reBottom);
	
	Invalidate();
}


void CQuickMakeDlg::OnSizeTopBar(CRect reClient)
{
	CRect reLeft;
	reLeft = reClient;
	
	reLeft.top += 5;
	reLeft.right = reLeft.left + 180;
	if (m_pgridSource->GetSafeHwnd() != NULL)
		m_pgridDest->MoveWindow(reLeft);
	
	reLeft.left = reLeft.right;
	reLeft.right = reLeft.left + 35;
	OnSizeMoveBar(reLeft);
	
	reLeft.left = reLeft.right;
	reLeft.right = reClient.right;
	if (m_pgridDest->GetSafeHwnd() != NULL)
		m_pgridSource->MoveWindow(reLeft);
}


void CQuickMakeDlg::OnSizeMoveBar(CRect reClient)
{
	CRect reTemp;
	reTemp = reClient;
	
	reTemp.left += 4;
	reTemp.right -= 4;
	
	int nBtnHeight = 40;
	int nTopOffset;
	nTopOffset = ((reClient.bottom - reClient.top) - (nBtnHeight * 4 + 2 * 3)) / 2;
	
	reTemp.top += nTopOffset;
	reTemp.bottom = reTemp.top + nBtnHeight;
	m_btnLeftAll.MoveWindow(reTemp);
	
	reTemp.top = reTemp.bottom + 2;
	reTemp.bottom = reTemp.top + nBtnHeight;
	m_btnLeft.MoveWindow(reTemp);
	
	reTemp.top = reTemp.bottom + 2;
	reTemp.bottom = reTemp.top + nBtnHeight;
	m_btnRight.MoveWindow(reTemp);
	
	reTemp.top = reTemp.bottom + 2;
	reTemp.bottom = reTemp.top + nBtnHeight;
	m_btnRightAll.MoveWindow(reTemp);
}


void CQuickMakeDlg::OnSizePosBar(CRect reClient)
{
	CRect reBottom, reTemp;
	reBottom = reClient;
	
	reBottom.top = reBottom.bottom - 26;
	m_ctrlPosBar.MoveWindow(reBottom);
	
	reTemp = reBottom;
	reTemp.top += 3;
	reTemp.bottom -= 3;
	
	int nLeftOffset;
	nLeftOffset = reTemp.right - reTemp.left;
	nLeftOffset = (nLeftOffset - (30 * 4 + 2 * 3)) / 2;
	
	reTemp.left += nLeftOffset;
	reTemp.right = reTemp.left + 30;
	m_btnTop.MoveWindow(reTemp);
	
	reTemp.left = reTemp.right + 2;
	reTemp.right = reTemp.left + 30;
	m_btnUp.MoveWindow(reTemp);
	
	reTemp.left = reTemp.right + 2;
	reTemp.right = reTemp.left + 30;
	m_btnDown.MoveWindow(reTemp);
	
	reTemp.left = reTemp.right + 2;
	reTemp.right = reTemp.left + 30;
	m_btnBottom.MoveWindow(reTemp);
}


BOOL CQuickMakeDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	CMainFrmWnd *pwndMainFrm = (CMainFrmWnd *)m_pwndParent;
	
	SetAdd_Dest(&pwndMainFrm->m_listQuickInfo);
	
	SetUpdateInfoFolder(&pwndMainFrm->m_listUserInfoFolder);
	SetUpdateInfoFolder(&pwndMainFrm->m_listSysInfoFolder);
	
	SetAdd_Source(&pwndMainFrm->m_listUserInfoFolder);
	SetAdd_Source(&pwndMainFrm->m_listSysInfoFolder);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CQuickMakeDlg::SetAdd_Source(CList <XMLTreeInfoFolder, XMLTreeInfoFolder&> *plistInfoFolder)
{
	POSITION posFolder, posFile;
	XMLTreeInfoFile	stXMLTreeInfoFile;
	XMLTreeInfoFolder	stXMLTreeInfoFolder;
	
	int nRow;
	int nFolderIndex, nFileIndex;
	CString strFolderPath, strFilePath;
	for (nFolderIndex = 0; nFolderIndex < plistInfoFolder->GetCount(); nFolderIndex++)
	{
		posFolder = plistInfoFolder->FindIndex(nFolderIndex);
		stXMLTreeInfoFolder = plistInfoFolder->GetAt(posFolder);
		
		strFolderPath = stXMLTreeInfoFolder.strPath;
		for (nFileIndex = 0; nFileIndex < stXMLTreeInfoFolder.plistXMLTreeInfoFile->GetCount(); nFileIndex++)
		{
			posFile = stXMLTreeInfoFolder.plistXMLTreeInfoFile->FindIndex(nFileIndex);
			stXMLTreeInfoFile = stXMLTreeInfoFolder.plistXMLTreeInfoFile->GetAt(posFile);
			
			m_pgridSource->AppendRow();
			nRow = m_pgridSource->GetNumberRows() - 1;
			m_pgridSource->SetData_QuickMakeSource(nRow, stXMLTreeInfoFile, TRUE);
		}
	}
}

void CQuickMakeDlg::SetAdd_Dest(CList <QuickList, QuickList> *plistQuickInfo)
{
	int nQuickRegCnt = plistQuickInfo->GetCount();
	
	if (nQuickRegCnt > 0)
	{
		POSITION pos;
		QuickList stQuickList;
		int nRow;
		for (int nIndex = 0; nIndex < nQuickRegCnt; nIndex++)
		{
			pos = plistQuickInfo->FindIndex(nIndex);
			stQuickList = plistQuickInfo->GetAt(pos);
			
			m_pgridDest->AppendRow();
			nRow = m_pgridDest->GetNumberRows() - 1;
			m_pgridDest->SetData_QuickMakeDest(nRow, stQuickList, TRUE);
		}
	}
}

void CQuickMakeDlg::OnBtnRight()
{
	int nSelectRow = m_pgridSource->m_nSelectRow;
	
	if ((nSelectRow > -1) && (m_pgridSource->GetNumberRows() > nSelectRow))
		SetInsert_Dest(nSelectRow);
}

void CQuickMakeDlg::SetInsert_Dest(int nSelectRow, BOOL bErrView/*= TRUE*/)
{
	XMLTreeInfoFile stXMLTreeInfoFile;
	m_pgridSource->GetData_QuickMakeSource(nSelectRow, stXMLTreeInfoFile);
	
	if (stXMLTreeInfoFile.bQuickReg == FALSE)
	{
		QuickList stQuickList;
		GetInfoFileToQuick(stXMLTreeInfoFile, stQuickList);
		
		m_pgridDest->AppendRow();
		int nRow = m_pgridDest->GetNumberRows() - 1;
		m_pgridDest->SetData_QuickMakeDest(nRow, stQuickList, TRUE);
		
		stXMLTreeInfoFile.bQuickReg = TRUE;
		m_pgridSource->SetData_QuickMakeSource(nSelectRow, stXMLTreeInfoFile, TRUE);
	}
	else
	{
		if (bErrView == TRUE)
		{
			CString strErrorMsg;
			strErrorMsg.Format("이미추가된 조건명입니다.");
			::MessageBox(m_hWnd, strErrorMsg, PROGRAM_TITLE, MB_OK);
		}
	}
}


void CQuickMakeDlg::GetInfoFileToQuick(XMLTreeInfoFile stXMLTreeInfoFile,
									   QuickList &stQuickList)
{
	stQuickList.bCheck = TRUE;
	
	CString strQuickName;
	strQuickName = stXMLTreeInfoFile.strFileName;
	strQuickName.TrimRight(" ");
	strQuickName.TrimLeft(" ");
	
	if (strQuickName.GetLength() > (int)QUICK_TITLE_LENGTH)
		strQuickName = strQuickName.Left(QUICK_TITLE_LENGTH);
	
	stQuickList.strQuickName = strQuickName;
	
	stQuickList.strFileName = stXMLTreeInfoFile.strFileName;
	stQuickList.strFilePath = stXMLTreeInfoFile.strFilePath;
	stQuickList.strFolderPath = stXMLTreeInfoFile.strFolderPath;
	stQuickList.bAlertReg = (stXMLTreeInfoFile.strAlertReg == "TRUE")?TRUE:FALSE;
	stQuickList.strType = stXMLTreeInfoFile.strType;
}	

void CQuickMakeDlg::OnBtnRightAll()
{
	XMLTreeInfoFile stXMLTreeInfoFile;
	for (int nRow = 0; nRow < m_pgridSource->GetNumberRows(); nRow++)
	{
		if (atoi(m_pgridSource->QuickGetText(0, nRow)) == 1)
		{
			SetInsert_Dest(nRow, FALSE);
			
			m_pgridSource->GetData_QuickMakeSource(nRow, stXMLTreeInfoFile);
			stXMLTreeInfoFile.bQuickReg = TRUE;
			m_pgridSource->SetData_QuickMakeSource(nRow, stXMLTreeInfoFile);
		}
	}
}

void CQuickMakeDlg::OnBtnLeft()
{
	int nSelectRow = m_pgridDest->m_nSelectRow;
	
	if ((nSelectRow > -1) && (m_pgridDest->GetNumberRows() > nSelectRow))
		SetDelete_Dest(nSelectRow);
}

void CQuickMakeDlg::SetDelete_Dest(int nSelectRow)
{
	QuickList stQuickList;
	m_pgridDest->GetData_QuickMakeDest(nSelectRow, stQuickList);
	
	m_pgridDest->DeleteRow(nSelectRow);
	
	int nRow;
	if (nSelectRow > m_pgridDest->GetNumberRows() - 1)
		nRow = m_pgridDest->GetNumberRows() - 1;
	else if (nSelectRow < m_pgridDest->GetNumberRows() - 1)
		nRow = nSelectRow;
	m_pgridDest->SetSelRow(nRow);
	
	XMLTreeInfoFile stXMLTreeInfoFile;
	for (nRow = 0; nRow < m_pgridSource->GetNumberRows(); nRow++)
	{
		m_pgridSource->GetData_QuickMakeSource(nRow, stXMLTreeInfoFile);
		if ((stQuickList.strFilePath == stXMLTreeInfoFile.strFilePath) &&
			(stQuickList.strFolderPath == stXMLTreeInfoFile.strFolderPath))
		{
			stXMLTreeInfoFile.bQuickReg = FALSE;
			m_pgridSource->SetData_QuickMakeSource(nRow, stXMLTreeInfoFile);
			
			nSelectRow = nRow;
		}
	}
	
	m_pgridSource->SetSelRow(nSelectRow);
}

void CQuickMakeDlg::OnBtnLeftAll()
{
	m_pgridDest->ClearAll();
	
	XMLTreeInfoFile stXMLTreeInfoFile;
	for (int nRow = 0; nRow < m_pgridSource->GetNumberRows(); nRow++)
	{
		m_pgridSource->GetData_QuickMakeSource(nRow, stXMLTreeInfoFile);
		
		stXMLTreeInfoFile.bQuickReg = FALSE;
		m_pgridSource->SetData_QuickMakeSource(nRow, stXMLTreeInfoFile);
	}
}

void CQuickMakeDlg::OnBtnUp()
{
	CString strErrorMsg;
	int nSelectDestRow = m_pgridDest->m_nSelectRow;
	if (nSelectDestRow == 0)
	{
		strErrorMsg.Format("제일 상단의 조건명입니다.");
		::MessageBox(m_hWnd, strErrorMsg, PROGRAM_TITLE, MB_OK);
		return;
	}
	
	QuickList stQuickList;
	m_pgridDest->GetData_QuickMakeDest(nSelectDestRow, stQuickList);
	m_pgridDest->DeleteRow(nSelectDestRow);
	
	m_pgridDest->InsertRow(nSelectDestRow - 1);
	m_pgridDest->SetData_QuickMakeDest(nSelectDestRow - 1, stQuickList, TRUE);
}

void CQuickMakeDlg::OnBtnTop()
{
	CString strErrorMsg;
	int nSelectDestRow = m_pgridDest->m_nSelectRow;
	if (nSelectDestRow == 0)
	{
		strErrorMsg.Format("제일 상단의 조건명입니다.");
		::MessageBox(m_hWnd, strErrorMsg, PROGRAM_TITLE, MB_OK);
		return;
	}
	
	QuickList stQuickList;
	m_pgridDest->GetData_QuickMakeDest(nSelectDestRow, stQuickList);
	m_pgridDest->DeleteRow(nSelectDestRow);
	
	m_pgridDest->InsertRow(0);
	m_pgridDest->SetData_QuickMakeDest(0, stQuickList, TRUE);
}

void CQuickMakeDlg::OnBtnDown()
{
	CString strErrorMsg;
	int nSelectDestRow = m_pgridDest->m_nSelectRow;
	if (nSelectDestRow == m_pgridDest->GetNumberRows() - 1)
	{
		strErrorMsg.Format("제일 하단의 조건명입니다.");
		MessageBox(strErrorMsg, (CString)PROGRAM_TITLE, MB_OK);
		return;
	}
	
	QuickList stQuickList;
	m_pgridDest->GetData_QuickMakeDest(nSelectDestRow, stQuickList);
	m_pgridDest->DeleteRow(nSelectDestRow);
	
	m_pgridDest->InsertRow(nSelectDestRow + 1);
	m_pgridDest->SetData_QuickMakeDest(nSelectDestRow + 1, stQuickList, TRUE);
}

void CQuickMakeDlg::OnBtnBottom()
{
	CString strErrorMsg;
	int nSelectDestRow = m_pgridDest->m_nSelectRow;
	if (nSelectDestRow == m_pgridDest->GetNumberRows() - 1)
	{
		strErrorMsg.Format("제일 하단의 조건명입니다.");
		::MessageBox(m_hWnd, strErrorMsg, PROGRAM_TITLE, MB_OK);
		return;
	}
	
	QuickList stQuickList;
	m_pgridDest->GetData_QuickMakeDest(nSelectDestRow, stQuickList);
	m_pgridDest->DeleteRow(nSelectDestRow);
	
	m_pgridDest->AppendRow();
	m_pgridDest->SetData_QuickMakeDest(m_pgridDest->GetNumberRows() - 1, stQuickList, TRUE);
}

void CQuickMakeDlg::OnBtnOk()
{
	CMainFrmWnd *pwndMainFrm = (CMainFrmWnd *)m_pwndParent;
	
	SetUpdateInfoFolder(&pwndMainFrm->m_listUserInfoFolder);
	SetUpdateInfoFolder(&pwndMainFrm->m_listSysInfoFolder);
	
	//# Quick UpData!
	pwndMainFrm->m_listQuickInfo.RemoveAll();
	QuickList stQuickList;
	for (int nRow = 0; nRow < m_pgridDest->GetNumberRows(); nRow++)
	{
		m_pgridDest->GetData_QuickMakeDest(nRow, stQuickList);
		pwndMainFrm->m_listQuickInfo.AddTail(stQuickList);
	}
	
	CDialog::OnOK();
}

void CQuickMakeDlg::SetUpdateInfoFolder(CList	<XMLTreeInfoFolder, XMLTreeInfoFolder&> *plistInfoFolder)
{
	int nQuick, nFolder, nFile;
	POSITION posFile, posFolder;
	XMLTreeInfoFile	stXMLTreeInfoFile;
	XMLTreeInfoFolder	stXMLTreeInfoFolder;
	
	QuickList stQuickList;
	BOOL bQuickReg;
	for (nFolder = 0; nFolder < plistInfoFolder->GetCount(); nFolder++)
	{
		posFolder = plistInfoFolder->FindIndex(nFolder);
		stXMLTreeInfoFolder = plistInfoFolder->GetAt(posFolder);
		
		for (nFile = 0; nFile < stXMLTreeInfoFolder.plistXMLTreeInfoFile->GetCount(); nFile++)
		{
			posFile = stXMLTreeInfoFolder.plistXMLTreeInfoFile->FindIndex(nFile);
			stXMLTreeInfoFile = stXMLTreeInfoFolder.plistXMLTreeInfoFile->GetAt(posFile);
			
			bQuickReg = FALSE;
			for (nQuick = 0; nQuick < m_pgridDest->GetNumberRows(); nQuick++)
			{
				m_pgridDest->GetData_QuickMakeDest(nQuick, stQuickList);
				
				if ((stXMLTreeInfoFile.strFilePath == stQuickList.strFilePath) &&
					(stXMLTreeInfoFile.strFolderPath == stQuickList.strFolderPath))
				{
					bQuickReg = TRUE;
					break;
				}
			}
			
			stXMLTreeInfoFile.bQuickReg = bQuickReg;
			stXMLTreeInfoFolder.plistXMLTreeInfoFile->SetAt(posFile, stXMLTreeInfoFile);
		}
		
		plistInfoFolder->SetAt(posFolder, stXMLTreeInfoFolder);
	}
}

void CQuickMakeDlg::OnBtnCancel()
{
	CDialog::OnCancel();
}

//void CQuickMakeDlg::OnNotifyGrid(WPARAM wParam, LPARAM lParam)
LRESULT CQuickMakeDlg::OnNotifyGrid(WPARAM wParam, LPARAM lParam)
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
			
		}
		else if (stGridEventInfo.strType == "COMBO")
		{
			
		}
	}
	else if (wParam == NM_DBCLICK_L)
	{
		if (stGridEventInfo.strGridType == "QuickMakeSource")
			SetInsert_Dest(stGridEventInfo.nRow);
		else if (stGridEventInfo.strGridType == "QuickMakeDest")
			SetDelete_Dest(stGridEventInfo.nRow);
	}
	
	return TRUE;
}


void CQuickMakeDlg::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	CRect rtMainWnd;
	GetClientRect(rtMainWnd);
	dc.FillSolidRect(rtMainWnd, COLOR_DLG_CANVAS);
}

HBRUSH CQuickMakeDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	
	if (nCtlColor == CTLCOLOR_STATIC)
	{
		pDC->SetBkMode(TRANSPARENT);
		hbr = m_brushBk;
	}
	return hbr;
}
