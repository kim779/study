// FolderEditDlg.cpp : implementation file
//

#include "stdafx.h"
#include "FolderEditDlg.h"

#include "MainFrmWnd.h"
#include "AddControl/Grid_Comm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#pragma warning( disable : 4101 4800 4129 4700 )
/////////////////////////////////////////////////////////////////////////////
// CFolderEditDlg dialog

CFolderEditDlg::CFolderEditDlg(CWnd* pParent /*=NULL*/)
: CDialog(CFolderEditDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CFolderEditDlg)
	// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	
	m_pwndMain = pParent;
	
	m_pgridFolder = NULL;
	m_pgridFile = NULL;
}

void CFolderEditDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CFolderEditDlg)
	// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CFolderEditDlg, CDialog)
//{{AFX_MSG_MAP(CFolderEditDlg)
ON_WM_CTLCOLOR()
ON_WM_CREATE()
ON_WM_DESTROY()
ON_WM_SIZE()
ON_WM_PAINT()
//}}AFX_MSG_MAP

ON_BN_CLICKED(IDC_BTN_OUTFILEMOVE, OnBtnOutFileMove)
ON_BN_CLICKED(IDC_BTN_IMPORT, OnBtnImport)
ON_BN_CLICKED(IDC_BTN_EXPORT, OnBtnExport)

ON_BN_CLICKED(IDC_BTN_RIGHT, OnBtnRight)
ON_BN_CLICKED(IDC_BTN_ALLRIGHT, OnBtnAllRight)

ON_BN_CLICKED(IDC_BTN_FOLDERADD, OnBtnFolderAdd)
ON_BN_CLICKED(IDC_BTN_FOLDERINSERT, OnBtnFolderInsert)
ON_BN_CLICKED(IDC_BTN_FOLDERMODIFY, OnBtnFolderModify)
ON_BN_CLICKED(IDC_BTN_FOLDERDEL, OnBtnFolderDel)

ON_BN_CLICKED(IDC_BTN_FILEMODIFY, OnBtnFileModify)
ON_BN_CLICKED(IDC_BTN_FILEDEL, OnBtnFileDel)


ON_BN_CLICKED(IDC_BTN_UPFOLDER, OnBtnUpFolder)
ON_BN_CLICKED(IDC_BTN_DOWNFOLDER, OnBtnDownFolder)
ON_BN_CLICKED(IDC_BTN_TOPFOLDER, OnBtnTopFolder)
ON_BN_CLICKED(IDC_BTN_BOTTOMFOLDER, OnBtnBottomFolder)

ON_BN_CLICKED(IDC_BTN_UPFILE, OnBtnUpFile)
ON_BN_CLICKED(IDC_BTN_DOWNFILE, OnBtnDownFile)
ON_BN_CLICKED(IDC_BTN_TOPFILE, OnBtnTopFile)
ON_BN_CLICKED(IDC_BTN_BOTTOMFILE, OnBtnBottomFile)


ON_BN_CLICKED(IDC_BTN_EDITOK, OnBtnEditOk)
ON_BN_CLICKED(IDC_BTN_EDITCANCEL, OnBtnEditCancel)

ON_BN_CLICKED(IDC_BTN_OK, OnBtnOk)

ON_MESSAGE(WM_NOTIFY_GRIDEVENT, OnNotifyGrid)


END_MESSAGE_MAP()

extern HINSTANCE g_hInstance;

/////////////////////////////////////////////////////////////////////////////
// CFolderEditDlg message handlers

int CFolderEditDlg::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	HINSTANCE hInstSave = AfxGetResourceHandle();
	AfxSetResourceHandle(g_hInstance);
	
	CMainFrmWnd *pwndMainFrm = (CMainFrmWnd*)m_pwndMain;

	LOGFONT	  logFont;
	GETFONT(logFont, 12, 400, m_fontDefault);
	m_brushBk.CreateSolidBrush(COLOR_DLG_CANVAS);
	
	m_ctrlToolBar.Create(_T(""), WS_CHILD | WS_VISIBLE,// | WS_BORDER,
		CRect(0, 0, 0, 0), this);
	
	m_btnOutFileMove.Create(_T("외부조건이동"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 
		CRect(0, 0, 0, 0), this, IDC_BTN_OUTFILEMOVE);
	m_btnOutFileMove.SetFont(&m_fontDefault);
	
	m_btnImport.Create(_T("가져오기"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 
		CRect(0, 0, 0, 0), this, IDC_BTN_IMPORT);
	m_btnImport.SetFont(&m_fontDefault);
	
	m_btnExport.Create(_T("보내기"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 
		CRect(0, 0, 0, 0), this, IDC_BTN_EXPORT);
	m_btnExport.SetFont(&m_fontDefault);
	
	//#ifndef _DEBUG
	m_btnImport.ShowWindow(SW_HIDE);
	m_btnExport.ShowWindow(SW_HIDE);
	//#endif
	
	m_ctrlBody.Create(_T(""), WS_CHILD,// | WS_VISIBLE,// | WS_BORDER,
		CRect(0, 0, 0, 0), this);
	
	if (m_pgridFolder == NULL)
		m_pgridFolder = new CGrid_Comm(this, "Folder", pwndMainFrm->GetAxColor(74));
	m_pgridFolder->CreateGrid(
		WS_CHILD | WS_VISIBLE | WS_CLIPCHILDREN | WS_CLIPSIBLINGS | SS_SIMPLE | WS_BORDER,
		CRect(0, 0, 0, 0), this, ID_GRID_FOLDER);
	
	m_ctrlFolderBar.Create(_T(""), WS_CHILD | WS_VISIBLE,// | WS_BORDER,
		CRect(0, 0, 0, 0), this);
	
	m_btnUpFolder.Create(_T("▲"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 
		CRect(0, 0, 0, 0), this, IDC_BTN_UPFOLDER);
	m_btnUpFolder.SetFont(&m_fontDefault);
	
	m_btnDownFolder.Create(_T("▼"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 
		CRect(0, 0, 0, 0), this, IDC_BTN_DOWNFOLDER);
	m_btnDownFolder.SetFont(&m_fontDefault);
	
	m_btnTopFolder.Create(_T("▲▲"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 
		CRect(0, 0, 0, 0), this, IDC_BTN_TOPFOLDER);
	m_btnTopFolder.SetFont(&m_fontDefault);
	
	m_btnBottomFolder.Create(_T("▼▼"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 
		CRect(0, 0, 0, 0), this, IDC_BTN_BOTTOMFOLDER);
	m_btnBottomFolder.SetFont(&m_fontDefault);
	
	m_ctrlSelectFolder.Create(_T("선택 그룹"), WS_CHILD | WS_VISIBLE,// | WS_BORDER,
		CRect(0, 0, 0, 0), this);
	m_ctrlSelectFolder.SetFont(&m_fontDefault);
	
	m_ctrlMoveMenuBar.Create(_T(""), WS_CHILD | WS_VISIBLE,// | WS_BORDER,
		CRect(0, 0, 0, 0), this);
	
	m_btnRight.Create(_T("<"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 
		CRect(0, 0, 0, 0), this, IDC_BTN_RIGHT);
	m_btnRight.SetFont(&m_fontDefault);
	
	m_btnRightAll.Create(_T("<<"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 
		CRect(0, 0, 0, 0), this, IDC_BTN_ALLRIGHT);
	m_btnRightAll.SetFont(&m_fontDefault);
	
	if (m_pgridFile == NULL)
		m_pgridFile = new CGrid_Comm(this, "File", pwndMainFrm->GetAxColor(74));
	m_pgridFile->CreateGrid(
		WS_CHILD | WS_VISIBLE | WS_CLIPCHILDREN | WS_CLIPSIBLINGS | SS_SIMPLE | WS_BORDER,
		CRect(0, 0, 0, 0), this, ID_GRID_FILE);
	
	m_ctrlSelectFile.Create(_T("선택 조건"), WS_CHILD | WS_VISIBLE,// | WS_BORDER,
		CRect(0, 0, 0, 0), this);
	m_ctrlSelectFile.SetFont(&m_fontDefault);
	
	m_ctrlFileBar.Create(_T(""), WS_CHILD | WS_VISIBLE,// | WS_BORDER,
		CRect(0, 0, 0, 0), this);
	
	m_btnUpFile.Create(_T("▲"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 
		CRect(0, 0, 0, 0), this, IDC_BTN_UPFILE);
	m_btnUpFile.SetFont(&m_fontDefault);
	
	m_btnDownFile.Create(_T("▼"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 
		CRect(0, 0, 0, 0), this, IDC_BTN_DOWNFILE);
	m_btnDownFile.SetFont(&m_fontDefault);
	
	m_btnTopFile.Create(_T("▲▲"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 
		CRect(0, 0, 0, 0), this, IDC_BTN_TOPFILE);
	m_btnTopFile.SetFont(&m_fontDefault);
	
	m_btnBottomFile.Create(_T("▼▼"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 
		CRect(0, 0, 0, 0), this, IDC_BTN_BOTTOMFILE);
	m_btnBottomFile.SetFont(&m_fontDefault);
	
	m_ctrlCtrlMenuBar.Create(_T(""), WS_CHILD | WS_VISIBLE | WS_BORDER,
		CRect(0, 0, 0, 0), this);
	
	m_btnFolderAdd.Create(_T("추가"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 
		CRect(0, 0, 0, 0), this, IDC_BTN_FOLDERADD);
	m_btnFolderAdd.SetFont(&m_fontDefault);
	
	m_btnFolderInsert.Create(_T("삽입"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 
		CRect(0, 0, 0, 0), this, IDC_BTN_FOLDERINSERT);
	m_btnFolderInsert.SetFont(&m_fontDefault);
	
	m_btnFolderModify.Create(_T("변경"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 
		CRect(0, 0, 0, 0), this, IDC_BTN_FOLDERMODIFY);
	m_btnFolderModify.SetFont(&m_fontDefault);
	
	m_btnFolderDel.Create(_T("삭제"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 
		CRect(0, 0, 0, 0), this, IDC_BTN_FOLDERDEL);
	m_btnFolderDel.SetFont(&m_fontDefault);
	
	m_btnFileModify.Create(_T("변경"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 
		CRect(0, 0, 0, 0), this, IDC_BTN_FILEMODIFY);
	m_btnFileModify.SetFont(&m_fontDefault);
	
	m_btnFileDel.Create(_T("삭제"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 
		CRect(0, 0, 0, 0), this, IDC_BTN_FILEDEL);
	m_btnFileDel.SetFont(&m_fontDefault);
	
	m_btnOk.Create(_T("닫기"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 
		CRect(0, 0, 0, 0), this, IDC_BTN_OK);
	m_btnOk.SetFont(&m_fontDefault);
	
	m_ctrlEditMenuBar.Create(_T(""), WS_CHILD | WS_BORDER,
		CRect(0, 0, 0, 0), this);
	
	m_ctrlEditTitle.Create(_T(""), WS_CHILD,
		CRect(0, 0, 0, 0), this);
	m_ctrlEditTitle.SetFont(&m_fontDefault);
	
	m_editInput.Create(WS_CHILD | WS_TABSTOP,
		CRect(0, 0, 0, 0), this, IDC_EDIT_INPUT);
	m_editInput.ModifyStyleEx(0, WS_EX_CLIENTEDGE);
	m_editInput.SetFont(&m_fontDefault);
	
	m_btnEditOk.Create(_T("확인"), WS_CHILD | BS_PUSHBUTTON, 
		CRect(0, 0, 0, 0), this, IDC_BTN_EDITOK);
	m_btnEditOk.SetFont(&m_fontDefault);
	
	m_btnEditCancel.Create(_T("취소"), WS_CHILD | BS_PUSHBUTTON, 
		CRect(0, 0, 0, 0), this, IDC_BTN_EDITCANCEL);
	m_btnEditCancel.SetFont(&m_fontDefault);

	AfxSetResourceHandle(hInstSave);
	
	CRect reClient;
	GetWindowRect(reClient);
	
	reClient.right = reClient.left + 450;
	reClient.bottom = reClient.top + 390;
	MoveWindow(reClient);
	
	return 0;
}

BOOL CFolderEditDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	this->SetWindowText((CString)PROGRAM_TITLE + " [폴더 편집]");
	
	CMainFrmWnd *pwndMainFrm = (CMainFrmWnd*)m_pwndMain;
	m_plistXMLTreeInfoFolder = &pwndMainFrm->m_listUserInfoFolder;
	
	SetInit();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CFolderEditDlg::SetInit()
{
	m_nMoveFolderRow = -1;
	
	m_strEditMode = "";
	CMainFrmWnd *pwndMainFrm = (CMainFrmWnd*)m_pwndMain;
	m_strUserDataPath = pwndMainFrm->GetFolderPath("USER") + "\\";
	
	POSITION pos;
	XMLTreeInfoFolder stXMLTreeInfoFolder, stInfoFolder;
	CString strTemp;
	
	m_pgridFolder->ClearAll();
	m_pgridFile->ClearAll();
	for (int nRow = 0; nRow < m_plistXMLTreeInfoFolder->GetCount(); nRow++)
	{
		pos = m_plistXMLTreeInfoFolder->FindIndex(nRow);
		stXMLTreeInfoFolder = m_plistXMLTreeInfoFolder->GetAt(pos);
		
		InfoFolder	stInfoFolder;
		stInfoFolder.strName = stXMLTreeInfoFolder.strName;
		stInfoFolder.strPath = stXMLTreeInfoFolder.strPath;
		stInfoFolder.bLock = stXMLTreeInfoFolder.bLock;
		stInfoFolder.bTarget = FALSE;
		
		//# Sync Count!
		stXMLTreeInfoFolder.nFileCnt = stXMLTreeInfoFolder.plistXMLTreeInfoFile->GetCount();
		stInfoFolder.nFileCnt = stXMLTreeInfoFolder.nFileCnt;
		
		m_pgridFolder->AppendRow();
		//m_pgridFolder->SetData_Folder(nRow, stInfoFolder, (m_plistXMLTreeInfoFolder->GetCount() - 1 == nRow) ? TRUE : FALSE);
		m_pgridFolder->SetData_Folder(nRow, stInfoFolder, FALSE);
	}
}

void CFolderEditDlg::OnDestroy() 
{
	//	CDialog::OnDestroy();
	
	if (m_pgridFile != NULL)
	{
		m_pgridFile->DestroyWindow();
		delete m_pgridFile;
		m_pgridFile = NULL;
	}
	
	if (m_pgridFolder != NULL)
	{
		m_pgridFolder->DestroyWindow();
		delete m_pgridFolder;
		m_pgridFolder = NULL;
	}
	
	m_brushBk.DeleteObject();
	m_fontDefault.DeleteObject();
	
	CDialog::OnDestroy();
}

HBRUSH CFolderEditDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	
	if (nCtlColor == CTLCOLOR_STATIC)
	{
		pDC->SetBkMode(TRANSPARENT);
		hbr = m_brushBk;
	}
	
	return hbr;
}

void CFolderEditDlg::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);
	
	int nBtnHeight = 20;
	
	CRect reClient, reTop;
	GetClientRect(reClient);
	reTop = reClient;
	
	reTop.left += 5;
	reTop.right -= 5;
	
	reTop.bottom = reTop.top + 28;
	m_ctrlToolBar.MoveWindow(reTop);
	OnSizeToolBar(reTop);
	
	reTop.top = reTop.bottom;
	reTop.bottom = reClient.bottom - (28 * 2);
	m_ctrlBody.MoveWindow(reTop);
	OnSizeBody(reTop);
	
	reTop.top = reTop.bottom;
	reTop.bottom = reTop.top + 28;
	m_ctrlCtrlMenuBar.MoveWindow(reTop);
	OnSizeCtrlMenuBar(reTop);
	OnSizeEditBar(reTop);
	
	reTop.top = reTop.bottom + 4;
	reTop.bottom = reTop.top + nBtnHeight;
	
	reTop.left = reTop.right - 60;
	m_btnOk.MoveWindow(reTop);
}

void CFolderEditDlg::OnSizeBody(CRect reClient)
{
	int nMoveBarWidth = 42;
	int nFolderWidth = 200;
	
	CRect reLeft, reTemp, reBtnTemp;
	reLeft = reClient;
	
	//int nWidth = (reClient.right - reClient.left - nMoveBarWidth) / 2;
	
	reLeft.top += 2;
	reLeft.right = reLeft.left + nFolderWidth;
	
	reTemp = reLeft;
	reTemp.bottom -= 18 + 24;
	if (m_pgridFolder != NULL)
		m_pgridFolder->MoveWindow(reTemp);
	
	reTemp.top = reTemp.bottom + 2;
	reTemp.bottom = reTemp.top + 24;
	m_ctrlFolderBar.MoveWindow(reTemp);
	
	reBtnTemp = reTemp;
	reBtnTemp.top += 2;
	reBtnTemp.bottom -= 2;
	
	
	int nWidth_MoveBtn = 34;
	
	int nLeftOffset;
	nLeftOffset = reBtnTemp.right - reBtnTemp.left;
	nLeftOffset = (nLeftOffset - (nWidth_MoveBtn * 4 + 2 * 3)) / 2;
	
	reBtnTemp.left += nLeftOffset;
	reBtnTemp.right = reBtnTemp.left + nWidth_MoveBtn;
	m_btnTopFolder.MoveWindow(reBtnTemp);
	
	reBtnTemp.left = reBtnTemp.right + 2;
	reBtnTemp.right = reBtnTemp.left + nWidth_MoveBtn;
	m_btnUpFolder.MoveWindow(reBtnTemp);
	
	reBtnTemp.left = reBtnTemp.right + 2;
	reBtnTemp.right = reBtnTemp.left + nWidth_MoveBtn;
	m_btnDownFolder.MoveWindow(reBtnTemp);
	
	reBtnTemp.left = reBtnTemp.right + 2;
	reBtnTemp.right = reBtnTemp.left + nWidth_MoveBtn;
	m_btnBottomFolder.MoveWindow(reBtnTemp);
	
	
	reTemp.top = reTemp.bottom + 2;
	reTemp.bottom = reClient.bottom - 3;
	m_ctrlSelectFolder.MoveWindow(reTemp);
	
	
	reLeft.left = reLeft.right;
	reLeft.right = reLeft.left + nMoveBarWidth;
	m_ctrlMoveMenuBar.MoveWindow(reLeft);
	OnSizeMoveBar(reLeft);
	
	reLeft.left = reLeft.right;
	reLeft.right = reClient.right;
	
	reTemp = reLeft;
	reTemp.bottom -= 18 + 24;
	if (m_pgridFile != NULL)
		m_pgridFile->MoveWindow(reTemp);
	
	reTemp.top = reTemp.bottom + 2;
	reTemp.bottom = reTemp.top + 24;
	m_ctrlFileBar.MoveWindow(reTemp);
	
	reBtnTemp = reTemp;
	reBtnTemp.top += 2;
	reBtnTemp.bottom -= 2;
	
	nLeftOffset = reBtnTemp.right - reBtnTemp.left;
	nLeftOffset = (nLeftOffset - (nWidth_MoveBtn * 4 + 2 * 3)) / 2;
	
	reBtnTemp.left += nLeftOffset;
	reBtnTemp.right = reBtnTemp.left + nWidth_MoveBtn;
	m_btnTopFile.MoveWindow(reBtnTemp);
	
	reBtnTemp.left = reBtnTemp.right + 2;
	reBtnTemp.right = reBtnTemp.left + nWidth_MoveBtn;
	m_btnUpFile.MoveWindow(reBtnTemp);
	
	reBtnTemp.left = reBtnTemp.right + 2;
	reBtnTemp.right = reBtnTemp.left + nWidth_MoveBtn;
	m_btnDownFile.MoveWindow(reBtnTemp);
	
	reBtnTemp.left = reBtnTemp.right + 2;
	reBtnTemp.right = reBtnTemp.left + nWidth_MoveBtn;
	m_btnBottomFile.MoveWindow(reBtnTemp);
	
	reTemp.top = reTemp.bottom + 2;
	reTemp.bottom = reClient.bottom - 3;
	m_ctrlSelectFile.MoveWindow(reTemp);
}

void CFolderEditDlg::OnSizeCtrlMenuBar(CRect reClient)
{
	int btnHeight = 20;
	int btnWidth = 34;
	
	CRect reLeft, reRight;
	reLeft = reClient;
	reRight = reClient;
	
	reLeft.top += 4;
	reLeft.bottom = reLeft.top + btnHeight;
	
	reLeft.left += 3;
	reLeft.right = reLeft.left + btnWidth;
	m_btnFolderAdd.MoveWindow(reLeft);
	
	reLeft.left = reLeft.right + 2;
	reLeft.right = reLeft.left + btnWidth;
	m_btnFolderInsert.MoveWindow(reLeft);
	
	reLeft.left = reLeft.right + 2;
	reLeft.right = reLeft.left + btnWidth;
	m_btnFolderModify.MoveWindow(reLeft);
	
	reLeft.left = reLeft.right + 2;
	reLeft.right = reLeft.left + btnWidth;
	m_btnFolderDel.MoveWindow(reLeft);
	
	
	reRight.top += 4;
	reRight.bottom = reRight.top + btnHeight;
	
	reRight.right -= 3;
	reRight.left = reRight.right - btnWidth;
	m_btnFileDel.MoveWindow(reRight);
	
	reRight.right = reRight.left - 2;
	reRight.left = reRight.right - btnWidth;
	m_btnFileModify.MoveWindow(reRight);
}

void CFolderEditDlg::OnSizeToolBar(CRect reClient)
{
	int btnHeight = 20;
	
	CRect reRight, reLeft;
	reRight = reClient;
	reLeft = reClient;
	
	reLeft.top += 4;
	reLeft.bottom = reLeft.top + btnHeight;
	
	//reLeft.left += 5;
	reLeft.right = reLeft.left + 100;
	m_btnOutFileMove.MoveWindow(reLeft);
	
	
	reRight.top += 4;
	reRight.bottom = reRight.top + btnHeight;
	
	reRight.right -= 5;
	reRight.left = reRight.right - 60;
	m_btnImport.MoveWindow(reRight);
	
	reRight.right = reRight.left - 2;
	reRight.left = reRight.right - 60;
	m_btnExport.MoveWindow(reRight);
}


void CFolderEditDlg::OnSizeMoveBar(CRect reClient)
{
	int nBtnHeight = 20;
	
	CRect reTop;
	reTop = reClient;
	
	reTop.left += 4;
	reTop.right -= 4;
	
	int nGapTop;
	nGapTop = (reClient.bottom - reClient.top) / 2 - nBtnHeight;
	
	reTop.top = nGapTop;
	
	reTop.bottom = reTop.top + nBtnHeight;
	m_btnRight.MoveWindow(reTop);
	
	reTop.top = reTop.bottom + 2;
	reTop.bottom = reTop.top + nBtnHeight;
	m_btnRightAll.MoveWindow(reTop);
}

void CFolderEditDlg::OnSizeEditBar(CRect reClient)
{
	CRect reLeft, reRight;
	m_ctrlEditMenuBar.MoveWindow(reClient);
	
	reLeft = reClient;
	reRight = reClient;
	
	reLeft.top += 8;
	reLeft.bottom = reLeft.top + 14;
	reLeft.left += 3;
	reLeft.right = reLeft.left + 45;
	m_ctrlEditTitle.MoveWindow(reLeft);
	
	reRight.top += 4;
	reRight.bottom = reRight.top + 20;
	reRight.right -= 3;
	reRight.left = reRight.right - 34;
	m_btnEditCancel.MoveWindow(reRight);
	
	reRight.right = reRight.left - 2;
	reRight.left = reRight.right - 34;
	m_btnEditOk.MoveWindow(reRight);
	
	reLeft.top = reRight.top + 1;
	reLeft.bottom = reLeft.top + 18;
	reLeft.left = reLeft.right + 2;
	reLeft.right = reRight.left - 2;
	m_editInput.MoveWindow(reLeft);
}

void CFolderEditDlg::OnBtnOutFileMove()
{
	CString strMessage;
	if (m_nMoveFolderRow == -1)
	{
		strMessage = "삽입할 그룹이 선택되지 않았습니다. \n삽입할 그룹의 체크박스를 클릭하여 선택하십시요.";
		SetMessageBox(strMessage, (CString)PROGRAM_TITLE, MB_ICONWARNING | MB_OK);
		return;
	}
	
	HINSTANCE hInstSave = AfxGetResourceHandle();
	AfxSetResourceHandle(g_hInstance);
	
	CString strMsg = "";
	static char BASED_CODE szFilter[] = "사용자조건 파일 (*.xml)|*.xml||";
	CFileDialog dlgOpen(TRUE, _T("xml"), NULL, NULL, szFilter, this);
	
	if (dlgOpen.DoModal() == IDOK)
	{
		BOOL bConvertState = FALSE;
		CString strFilePath, strFileName;
		CString strTargetPath, strTargetFilePath, strTargetFileName;
		
		strTargetFilePath = dlgOpen.GetFileName();
		strTargetFileName = dlgOpen.GetFileTitle();
		strTargetPath = dlgOpen.GetPathName();
		
		if (strTargetPath.IsEmpty())
		{
			strMessage = "선택 조건이 없습니다.\n\n조건을 선택 하시오.";
			SetMessageBox(strMessage, (CString)PROGRAM_TITLE, MB_ICONWARNING | MB_OK);
			
			return;
		}
		
		MSXML::IXMLDOMDocumentPtr pXmlDom ("Microsoft.XMLDOM", NULL, CLSCTX_INPROC_SERVER);
		if (GetLoadXml(strTargetPath, pXmlDom) == FALSE)
		{
			strMessage = "선택/입력한 조건이 해당경로에 없습니다.\n\n조건을 재선택 하시오.";
			SetMessageBox(strMessage, (CString)PROGRAM_TITLE, MB_ICONWARNING | MB_OK);
			
			return;
		}
		else
		{
			//# 사용자 조건 파일인지 검사
			MSXML::IXMLDOMNodePtr			    pNConditions;
			MSXML::IXMLDOMNamedNodeMapPtr	pNAConditions;
			
			try 
			{
				pNConditions = pXmlDom->selectSingleNode("CONDITIONS");
				pNAConditions = pNConditions->attributes;
				MSXML::IXMLDOMNodePtr	pNAttributeTemp;
				
				pNAttributeTemp = pNAConditions->getNamedItem("VERSION");
				if (CString((PCHAR)pNAttributeTemp->text) != (CString)XML_VERSION_USERFILE)
				{
					strMessage = "선택 파일은 조건파일이 아니거나 또는 하위 버전입니다.\n선택조건을 컨버터 하시겠습니까?.";
					if (SetMessageBox(strMessage, (CString)PROGRAM_TITLE, MB_ICONWARNING | MB_OKCANCEL) == IDOK)
						bConvertState = TRUE;
					else
						return;
				}
			}
			catch(_com_error &e) 
			{
				strMessage = (CString)PROGRAM_TITLE + "용 조건파일이 아닙니다.\n\n조건을 재선택 하시오.";
				SetMessageBox(strMessage, (CString)PROGRAM_TITLE, MB_ICONWARNING | MB_OK);
				return;
			}
		}
		
		
		InfoFolder	stInfoFolder;
		m_pgridFolder->GetData_Folder(m_nMoveFolderRow, stInfoFolder);
		if (GetHaveFile(strTargetFilePath, stInfoFolder.strPath) == TRUE)
		{
			if (bConvertState == TRUE)
			{
				BOOL bState = TRUE;
				int nFileIndex = 1;
				while (bState == TRUE)
				{
					strFilePath.Format("%s_%d.xml", strTargetFilePath, nFileIndex);
					bState = GetHaveFile(strFilePath, stInfoFolder.strPath);
					
					if (bState == TRUE)
						nFileIndex++;
				}
				
				strFileName.Format("%s_%d", strTargetFileName, nFileIndex);
			}
			else
			{
				strMessage = "이동할 그룹에 같은 이름의 조건이 존재합니다.\n 조건을 재선택 하시오.";
				SetMessageBox(strMessage, (CString)PROGRAM_TITLE, MB_ICONWARNING | MB_OK);
				
				return;
			}
		}
		else
		{
			strFilePath = strTargetFilePath;
			strFileName = strTargetFileName;
		}
		
		XMLTreeInfoFile		stXMLTreeInfoFile;
		XMLTreeInfoFolder	stXMLTreeInfoFolder;
		POSITION pos = GetPosFolder(m_plistXMLTreeInfoFolder, stInfoFolder.strPath);
		if (pos == NULL)
			return;
		stXMLTreeInfoFolder = m_plistXMLTreeInfoFolder->GetAt(pos);
		
		if (bConvertState == TRUE)
		{
		/*
		//# Convert!
		CXmlConvertDlg dlgConvert(this, FALSE);
		dlgConvert.SetConvert_OneFile(strTargetPath.Mid(0, strTargetPath.GetLength() - strTargetFilePath.GetLength()), 
		strTargetFileName,
		stInfoFolder.strPath, strFileName);
			*/
		}
		else
			CFile::Rename(strTargetPath, m_strUserDataPath + stInfoFolder.strPath + "\\" + strFilePath);
		
		
		stXMLTreeInfoFolder.nFileCnt++;
		
		stXMLTreeInfoFile.strType = "USER";;		//# USER, SYS
		stXMLTreeInfoFile.strFolderPath = stInfoFolder.strPath;
		stXMLTreeInfoFile.strFilePath = strFilePath;
		stXMLTreeInfoFile.strFileName = strFileName;
		
		stXMLTreeInfoFile.bQuickReg = FALSE;
		
		CMainFrmWnd *pwndMainFrm = (CMainFrmWnd*)m_pwndMain;
		pwndMainFrm->GetFileSetState("USER", stXMLTreeInfoFile.strFolderPath, stXMLTreeInfoFile.strFilePath, 
			stXMLTreeInfoFile.strAlertReg, stXMLTreeInfoFile.bUnitCan);
		
		stXMLTreeInfoFolder.plistXMLTreeInfoFile->AddTail(stXMLTreeInfoFile);
		m_plistXMLTreeInfoFolder->SetAt(pos, stXMLTreeInfoFolder);
		
		stInfoFolder.nFileCnt = stXMLTreeInfoFolder.nFileCnt;
		m_pgridFolder->SetData_Folder(m_nMoveFolderRow, stInfoFolder, TRUE);
		
		SetViewFolderToFileList(stInfoFolder.strPath);
		SetSelectRow_Folder(m_nMoveFolderRow);
		SetSelectRow_File(-1);
	}

	AfxSetResourceHandle(hInstSave);
}	

BOOL CFolderEditDlg::GetLoadXml(CString strFileRoot, MSXML::IXMLDOMDocumentPtr &pXmlDom)
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



void CFolderEditDlg::OnBtnImport()
{
	
}

void CFolderEditDlg::OnBtnExport()
{
	
}

void CFolderEditDlg::OnBtnRight()
{
	CString strErrorMsg;
	int nRow = m_pgridFile->m_nSelectRow;
	if (nRow == -1)
	{
		strErrorMsg = "선택 조건이 없습니다.\n 조건을 선택 하시오.";
		SetMessageBox(strErrorMsg, (CString)PROGRAM_TITLE, MB_ICONWARNING | MB_OK);
		return;
	}
	else if (m_nMoveFolderRow == -1)
	{
		strErrorMsg = "삽입할 그룹이 선택되지 않았습니다. \n삽입할 그룹의 체크박스를 클릭하여 선택하십시요.";
		SetMessageBox(strErrorMsg, (CString)PROGRAM_TITLE, MB_ICONWARNING | MB_OK);
		return;
	}
	
	InfoFolder	stInfoFolder;
	XMLTreeInfoFile	stXMLTreeInfoFile;
	
	m_pgridFolder->GetData_Folder(m_nMoveFolderRow, stInfoFolder);
	m_pgridFile->GetData_File(nRow, stXMLTreeInfoFile);
	
	if (stInfoFolder.strPath == stXMLTreeInfoFile.strFolderPath)
	{
		strErrorMsg = "이동할 그룹이 현재위치와 같습니다.";
		SetMessageBox(strErrorMsg, (CString)PROGRAM_TITLE, MB_ICONWARNING | MB_OK);
		return;
	}
	
	if (GetHaveFile(stXMLTreeInfoFile.strFilePath, stInfoFolder.strPath) == FALSE)
	{
		SetMove_File(stXMLTreeInfoFile.strFolderPath, stXMLTreeInfoFile.strFilePath, stInfoFolder.strPath);
		
		CString strTemp;
		strTemp.Format("%d", stInfoFolder.nFileCnt + 1);
		m_pgridFolder->SetCellData(m_nMoveFolderRow, 2, strTemp);
		
		m_pgridFile->DeleteRow(nRow);
		
		int nFolderRow;
		for (nFolderRow = 0; nFolderRow < m_pgridFolder->GetNumberRows(); nFolderRow++)
		{
			m_pgridFolder->GetData_Folder(nFolderRow, stInfoFolder);
			if (stXMLTreeInfoFile.strFolderPath == stInfoFolder.strPath)
				break;
		}
		strTemp.Format("%d", stInfoFolder.nFileCnt - 1);
		m_pgridFolder->SetCellData(nFolderRow, 2, strTemp);
		m_pgridFolder->RedrawAll();
		
		if (m_pgridFile->GetNumberRows() > 0)
			SetSelectRow_File(0);
		else
			SetSelectRow_File(-1);
	}
	else
	{
		strErrorMsg = "이동할 그룹에 같은 이름의 조건이 존재합니다.";
		SetMessageBox(strErrorMsg, (CString)PROGRAM_TITLE, MB_ICONWARNING | MB_OK);
	}
}

void CFolderEditDlg::OnBtnAllRight()
{
	CString strErrorMsg;
	int nFileCnt = m_pgridFile->GetNumberRows();
	if (nFileCnt < 0)
	{
		strErrorMsg = "이동할 조건이 없습니다.";
		SetMessageBox(strErrorMsg, (CString)PROGRAM_TITLE, MB_ICONWARNING | MB_OK);
		return;
	}
	else if (m_nMoveFolderRow == -1)
	{
		strErrorMsg = "삽입할 그룹이 선택되지 않았습니다. \n삽입할 그룹의 체크박스를 클릭하여 선택하십시요.";
		SetMessageBox(strErrorMsg, (CString)PROGRAM_TITLE, MB_ICONWARNING | MB_OK);
		return;
	}
	
	InfoFolder	stInfoFolder;
	XMLTreeInfoFile	stXMLTreeInfoFile;
	
	m_pgridFolder->GetData_Folder(m_nMoveFolderRow, stInfoFolder);
	m_pgridFile->GetData_File(0, stXMLTreeInfoFile);
	if (stInfoFolder.strPath == stXMLTreeInfoFile.strFolderPath)
	{
		strErrorMsg = "이동할 그룹이 현재위치와 같습니다.";
		SetMessageBox(strErrorMsg, (CString)PROGRAM_TITLE, MB_ICONWARNING | MB_OK);
		return;
	}
	
	int nMoveCnt = 0;
	int nRow = 0;
	
	
	CString strTemp1;
	while (nRow < m_pgridFile->GetNumberRows())
	{
		if (atoi(m_pgridFile->GetCellData(nRow, 0)) == 1)
		{
			m_pgridFile->GetData_File(nRow, stXMLTreeInfoFile);
			if (GetHaveFile(stXMLTreeInfoFile.strFilePath, stInfoFolder.strPath) == FALSE)
			{
				SetMove_File(stXMLTreeInfoFile.strFolderPath, stXMLTreeInfoFile.strFilePath, stInfoFolder.strPath);
				m_pgridFile->DeleteRow(nRow);
				nMoveCnt++;
				
				nRow = 0;
			}
		}
		else
			nRow++;
	}
	
	CString strTemp;
	strTemp.Format("%d", stInfoFolder.nFileCnt + nMoveCnt);
	m_pgridFolder->SetCellData(m_nMoveFolderRow, 2, strTemp);
	
	int nFolderRow;
	for (nFolderRow = 0; nFolderRow < m_pgridFolder->GetNumberRows(); nFolderRow++)
	{
		m_pgridFolder->GetData_Folder(nFolderRow, stInfoFolder);
		if (stXMLTreeInfoFile.strFolderPath == stInfoFolder.strPath)
			break;
	}
	strTemp.Format("%d", stInfoFolder.nFileCnt - nMoveCnt);
	m_pgridFolder->SetCellData(nFolderRow, 2, strTemp);
	m_pgridFolder->RedrawAll();
	
	if (m_pgridFile->GetNumberRows() > 0)
		SetSelectRow_File(0);
	else
		SetSelectRow_File(-1);
}

void CFolderEditDlg::OnBtnFolderAdd()
{
	SetBtnEnable(FALSE);
	
	m_ctrlEditTitle.SetWindowText("폴더명");
	m_editInput.SetWindowText("");
	m_editInput.SetFocus();
	SetEditBarView(TRUE);
	
	m_strEditMode = "Folder-Add";
}

void CFolderEditDlg::OnBtnFolderInsert()
{
	SetBtnEnable(FALSE);
	
	m_ctrlEditTitle.SetWindowText("폴더명");
	m_editInput.SetWindowText("");
	m_editInput.SetFocus();
	SetEditBarView(TRUE);
	
	m_strEditMode = "Folder-Insert";
}

void CFolderEditDlg::OnBtnFolderModify()
{
	SetBtnEnable(FALSE);
	
	int nRow = m_pgridFolder->m_nSelectRow;
	
	InfoFolder	stInfoFolder;
	m_pgridFolder->GetData_Folder(nRow, stInfoFolder);
	
	m_ctrlEditTitle.SetWindowText("폴더명");
	m_editInput.SetWindowText(stInfoFolder.strName);
	m_editInput.SetFocus();
	m_editInput.SetSel(0, (stInfoFolder.strName).GetLength());
	SetEditBarView(TRUE);
	
	m_strEditMode = "Folder-Modify";
}


void CFolderEditDlg::OnBtnFolderDel()
{
	CString strMessage;
	int nRow = m_pgridFolder->m_nSelectRow;
	if (nRow == -1)
	{
		strMessage = "선택 폴더가 없습니다.\n 폴더를 선택 하시오.";
		SetMessageBox(strMessage, (CString)PROGRAM_TITLE, MB_ICONWARNING | MB_OK);
		return;
	}
	else
	{
		InfoFolder	stInfoFolder;
		m_pgridFolder->GetData_Folder(nRow, stInfoFolder);
		strMessage = CString("폴더(") + stInfoFolder.strName + ") 을 삭제하려 합니다.\n삭제하시겠습니까?\n\n[주의] 본 폴더내의 조건 파일들을 모두 삭제됩니다.";
		if (SetMessageBox(strMessage, (CString)PROGRAM_TITLE, MB_OKCANCEL | MB_TOPMOST) == IDOK)
		{
			SetControlState(FALSE);
			
			if (SetDelete_Folder(stInfoFolder.strPath) == TRUE)
			{
				m_pgridFolder->DeleteRow(nRow);
				
				if (m_pgridFolder->GetNumberRows() == 0)
					m_pgridFolder->SetSelRow(-1);
				else if (nRow < m_pgridFolder->GetNumberRows() - 1)
					m_pgridFolder->SetSelRow(nRow);
				else
					m_pgridFolder->SetSelRow(m_pgridFolder->GetNumberRows() - 1);
			}
			
			SetControlState();
		}
	}
}

BOOL CFolderEditDlg::SetDelete_Folder(CString strFolderPath)
{
	BOOL bResult = FALSE;
	
	SetControlState(FALSE);
	
	strFolderPath.TrimLeft(); 
	strFolderPath.TrimRight();
	
	POSITION posFolder, posFile;
	XMLTreeInfoFile      stXMLTreeInfoFile;
	XMLTreeInfoFolder			stXMLTreeInfoFolder;
	CString strTatgetPath;
	
	int nCount = m_plistXMLTreeInfoFolder->GetCount();
	for (int nFolderIndex = 0; nFolderIndex < nCount; nFolderIndex++)
	{
		posFolder = m_plistXMLTreeInfoFolder->FindIndex(nFolderIndex);
		stXMLTreeInfoFolder = m_plistXMLTreeInfoFolder->GetAt(posFolder);
		
		if (stXMLTreeInfoFolder.strPath == strFolderPath)
		{
			for (int nFileIndex = 0; nFileIndex < stXMLTreeInfoFolder.plistXMLTreeInfoFile->GetCount(); nFileIndex++)
			{
				posFile = stXMLTreeInfoFolder.plistXMLTreeInfoFile->FindIndex(nFileIndex);
				stXMLTreeInfoFile = stXMLTreeInfoFolder.plistXMLTreeInfoFile->GetAt(posFile);
				
				CFile::Remove(m_strUserDataPath + strFolderPath + "\\" + stXMLTreeInfoFile.strFilePath);
			}
			stXMLTreeInfoFolder.plistXMLTreeInfoFile->RemoveAll();
			delete stXMLTreeInfoFolder.plistXMLTreeInfoFile;
			
			CString strDirectory = m_strUserDataPath + strFolderPath;
			SHFILEOPSTRUCT shFileOp = {0};
			char szTemp[MAX_PATH];
			strcpy(szTemp, strDirectory);
			szTemp[strDirectory.GetLength() + 1] = NULL; 
			
			shFileOp.hwnd = NULL;
			shFileOp.wFunc = FO_DELETE;
			shFileOp.pFrom = szTemp;
			shFileOp.pTo = NULL;
			shFileOp.fFlags = FOF_NOCONFIRMATION | FOF_NOERRORUI;
			shFileOp.fAnyOperationsAborted = false;
			shFileOp.hNameMappings = NULL;
			shFileOp.lpszProgressTitle = NULL;
			SHFileOperation(&shFileOp);
			
			m_plistXMLTreeInfoFolder->RemoveAt(posFolder);
			
			bResult = TRUE;
			
			//# Quick Btn Delete!
			CMainFrmWnd *pwndMainFrm = (CMainFrmWnd*)m_pwndMain;
			pwndMainFrm->SetQuickInfo_Delete("FOLDER", strFolderPath);
			
			break;
		}
	}
	
	SetControlState();
	
	return bResult;
}

void CFolderEditDlg::OnBtnFileModify()
{
	SetBtnEnable(FALSE);
	
	int nRow = m_pgridFile->m_nSelectRow;
	
	XMLTreeInfoFile	stXMLTreeInfoFile;
	m_pgridFile->GetData_File(nRow, stXMLTreeInfoFile);
	
	m_ctrlEditTitle.SetWindowText("파일명");
	m_editInput.SetWindowText(stXMLTreeInfoFile.strFileName);
	m_editInput.SetFocus();
	m_editInput.SetSel(0, (stXMLTreeInfoFile.strFileName).GetLength());
	
	SetEditBarView(TRUE);
	
	m_strEditMode = "File-Modify";
}

void CFolderEditDlg::OnBtnFileDel()
{
	CString strMessage;
	int nRow = m_pgridFile->m_nSelectRow;
	if (nRow == -1)
	{
		strMessage = "선택 조건이 없습니다.\n 조건을 선택 하시오.";
		SetMessageBox(strMessage, (CString)PROGRAM_TITLE, MB_ICONWARNING | MB_OK);
		return;
	}
	else
	{
		XMLTreeInfoFile	stXMLTreeInfoFile;
		m_pgridFile->GetData_File(nRow, stXMLTreeInfoFile);
		
		strMessage = CString("조건(") + stXMLTreeInfoFile.strFileName + ") 을 삭제하려 합니다.\n삭제하시겠습니까?";
		if (SetMessageBox(strMessage, (CString)PROGRAM_TITLE, MB_OKCANCEL) == IDOK)
		{
			if (SetDelete_File(stXMLTreeInfoFile.strFilePath, stXMLTreeInfoFile.strFolderPath) == TRUE)
			{
				m_pgridFile->DeleteRow(nRow);
				
				InfoFolder	stInfoFolder;
				for (nRow = 0; nRow < m_pgridFolder->GetNumberRows(); nRow++)
				{
					m_pgridFolder->GetData_Folder(nRow, stInfoFolder);
					if (stXMLTreeInfoFile.strFolderPath == stInfoFolder.strPath)
					{
						stInfoFolder.nFileCnt--;
						m_pgridFolder->SetData_Folder(nRow, stInfoFolder, TRUE);
					}
				}
				
				//# Quick Btn Delete!
				CMainFrmWnd *pwndMainFrm = (CMainFrmWnd*)m_pwndMain;
				pwndMainFrm->SetQuickInfo_Delete("FILE", stXMLTreeInfoFile.strFolderPath, stXMLTreeInfoFile.strFilePath);
			}
			else
			{
				CString strErrorMsg;
				strErrorMsg.Format("조건파일(%s)을 삭제하지 못하였습니다.", stXMLTreeInfoFile.strFilePath);
				SetMessageBox(strMessage, (CString)PROGRAM_TITLE, MB_ICONWARNING | MB_OK);
			}
		}
	}
}


BOOL CFolderEditDlg::SetDelete_File(CString strFilePath, CString strFolderPath)
{
	BOOL bResult = FALSE;
	
	SetControlState(FALSE);
	
	strFilePath.TrimLeft(); 
	strFilePath.TrimRight();
	
	POSITION posFolder, posFile;
	XMLTreeInfoFile      stXMLTreeInfoFile;
	XMLTreeInfoFolder			stXMLTreeInfoFolder;
	CString strTatgetPath;
	
	int nCount = m_plistXMLTreeInfoFolder->GetCount();
	for (int nFolderIndex = 0; nFolderIndex < nCount; nFolderIndex++)
	{
		posFolder = m_plistXMLTreeInfoFolder->FindIndex(nFolderIndex);
		stXMLTreeInfoFolder = m_plistXMLTreeInfoFolder->GetAt(posFolder);
		
		if (stXMLTreeInfoFolder.strPath == strFolderPath)
		{
			for (int nFileIndex = 0; nFileIndex < stXMLTreeInfoFolder.plistXMLTreeInfoFile->GetCount(); nFileIndex++)
			{
				posFile = stXMLTreeInfoFolder.plistXMLTreeInfoFile->FindIndex(nFileIndex);
				stXMLTreeInfoFile = stXMLTreeInfoFolder.plistXMLTreeInfoFile->GetAt(posFile);
				
				if (strFilePath.Compare(stXMLTreeInfoFile.strFilePath) == 0)
				{
					CFile::Remove(m_strUserDataPath + strFolderPath + "\\" + strFilePath);
					stXMLTreeInfoFolder.plistXMLTreeInfoFile->RemoveAt(posFile);
					bResult = TRUE;
					break;
				}
			}
			
			stXMLTreeInfoFolder.nFileCnt--;
			m_plistXMLTreeInfoFolder->SetAt(posFolder, stXMLTreeInfoFolder);
		}
		
		if (bResult == TRUE)
			break;
	}
	
	SetControlState();
	
	return bResult;
}

void CFolderEditDlg::OnBtnEditOk()
{
	CString strData, strMessage;
	int nRow, nFileCnt;
	m_editInput.GetWindowText(strData);
	
	strData.TrimRight(" ");
	
	int nFildPos = m_strEditMode.Find("-");
	CString strType;
	strType = m_strEditMode.Left(nFildPos);
	
	if (strType == "Folder")
	{
		if (strData.IsEmpty() == TRUE)
		{
			strMessage = "그룹명을 입력 하시오.";
			SetMessageBox(strMessage, (CString)PROGRAM_TITLE, MB_ICONWARNING | MB_OK);
			return;
		}
		
		if (GetCheckNAName("폴더", strData) == FALSE)
			return;
	}
	else if (strType == "File")
	{
		if (strData.IsEmpty() == TRUE)
		{
			strMessage = "조건명을 입력 하시오.";
			SetMessageBox(strMessage, (CString)PROGRAM_TITLE, MB_ICONWARNING | MB_OK);
			return;
		}
		
		if (GetCheckNAName("파일", strData) == FALSE)
			return;
	}
	
	if (m_strEditMode == "Folder-Add")
	{
		if (GetHaveFolder(strData, strData) == FALSE)
		{
			if (SetAdd_Folder(strData, strData) == TRUE)
			{
				InfoFolder	stInfoFolder;
				stInfoFolder.strName = strData;
				stInfoFolder.strPath = strData;
				stInfoFolder.bLock = FALSE;
				stInfoFolder.nFileCnt = 0;
				
				m_pgridFolder->AppendRow();
				nRow = m_pgridFolder->GetNumberRows() - 1;
				m_pgridFolder->SetData_Folder(nRow, stInfoFolder, TRUE);
				
				m_ctrlSelectFile.SetWindowText(stInfoFolder.strPath);
			}
		}
	}
	else if (m_strEditMode == "Folder-Insert")
	{
		nRow = m_pgridFolder->m_nSelectRow;
		if (nRow == -1)
		{
			strMessage = "선택 그룹가 없습니다.\n 그룹를 선택 하시오.";
			SetMessageBox(strMessage, (CString)PROGRAM_TITLE, MB_ICONWARNING | MB_OK);
			return;
		}
		else
		{
			if (GetHaveFolder(strData, strData) == FALSE)
			{
				if (SetInsert_Folder(nRow, strData, strData) == TRUE)
				{
					InfoFolder	stInfoFolder;
					stInfoFolder.strName = strData;
					stInfoFolder.strPath = strData;
					stInfoFolder.bLock = FALSE;
					stInfoFolder.nFileCnt = 0;
					stInfoFolder.bTarget = FALSE;
					
					m_pgridFolder->InsertRow(nRow);
					m_pgridFolder->SetSelRow(nRow + 1);
					m_pgridFolder->SetData_Folder(nRow, stInfoFolder, TRUE);
					
					m_ctrlSelectFile.SetWindowText(stInfoFolder.strPath);
				}
			}
		}
	}
	else if (m_strEditMode == "Folder-Modify")
	{
		nRow = m_pgridFolder->m_nSelectRow;
		if (nRow == -1)
		{
			strMessage = "선택 그룹가 없습니다.\n 폴더를 선택 하시오.";
			SetMessageBox(strMessage, (CString)PROGRAM_TITLE, MB_ICONWARNING | MB_OK);
			return;
		}
		else
		{
			InfoFolder	stInfoFolder;
			m_pgridFolder->GetData_Folder(nRow, stInfoFolder);
			
			if (strData == stInfoFolder.strName)
			{
				strMessage = "입력하신 그룹명이 기존과 동일합니다.";
				SetMessageBox(strMessage, (CString)PROGRAM_TITLE, MB_ICONWARNING | MB_OK);
				
				m_editInput.SetFocus();
				m_editInput.SetSel(0, strData.GetLength());
				return;
			}
			else if (GetHaveFolder(strData, nRow) == TRUE)
			{
				strMessage = "입력하신 그룹명이 기존과 같은 그룹이 존재합니다.";
				SetMessageBox(strMessage, (CString)PROGRAM_TITLE, MB_ICONWARNING | MB_OK);
				
				m_editInput.SetFocus();
				m_editInput.SetSel(0, strData.GetLength());
				return;
			}
			else
			{
				if (SetModify_Folder(stInfoFolder.strName, stInfoFolder.strPath, 
					strData, strData) == TRUE)
				{
					stInfoFolder.strName = strData;
					stInfoFolder.strPath = strData;
					m_pgridFolder->SetData_Folder(nRow, stInfoFolder, TRUE);
					
					m_ctrlSelectFile.SetWindowText(stInfoFolder.strPath);
				}
				else
				{
					strMessage = "그룹명 변경중 에러! 재시도 하세요.";
					SetMessageBox(strMessage, (CString)PROGRAM_TITLE, MB_ICONWARNING | MB_OK);
					return;
				}
			}
		}
	}
	else if (m_strEditMode == "File-Modify")
	{
		nRow = m_pgridFile->m_nSelectRow;
		if (nRow == -1)
		{
			strMessage = "선택된 조건명이 없습니다.";
			SetMessageBox(strMessage, (CString)PROGRAM_TITLE, MB_ICONWARNING | MB_OK);
			return;
		}
		else
		{
			XMLTreeInfoFile	stXMLTreeInfoFile;
			m_pgridFile->GetData_File(nRow, stXMLTreeInfoFile);
			
			if (strData == stXMLTreeInfoFile.strFileName)
			{
				strMessage = "입력하신 조건명이 기존과 동일합니다.";
				SetMessageBox(strMessage, (CString)PROGRAM_TITLE, MB_ICONWARNING | MB_OK);
				
				m_editInput.SetFocus();
				m_editInput.SetSel(0, strData.GetLength());
				return;
			}
			else if (GetHaveFile(strData, nRow) == TRUE)
			{
				strMessage = "입력하신 조건명이 기존과 같은 조건이 존재합니다.";
				SetMessageBox(strMessage, (CString)PROGRAM_TITLE, MB_ICONWARNING | MB_OK);
				
				m_editInput.SetFocus();
				m_editInput.SetSel(0, strData.GetLength());
				return;
			}
			else
			{
				if (SetModify_File(stXMLTreeInfoFile.strFolderPath,
					stXMLTreeInfoFile.strFileName, stXMLTreeInfoFile.strFilePath,
					strData, strData + ".xml") == TRUE)
				{
					stXMLTreeInfoFile.strFileName = strData;
					stXMLTreeInfoFile.strFilePath = strData + ".xml";
					m_pgridFile->SetData_File(nRow, stXMLTreeInfoFile, TRUE);
					
					m_ctrlSelectFile.SetWindowText(stXMLTreeInfoFile.strFileName);
				}
				else
				{
					strMessage = "조건명 변경중 에러! 재시도 하세요.";
					SetMessageBox(strMessage, (CString)PROGRAM_TITLE, MB_ICONWARNING | MB_OK);
					return;
				}
			}
		}
	}
	
	m_strEditMode = "";
	
	SetBtnEnable(TRUE);
	SetEditBarView(FALSE);
}


BOOL CFolderEditDlg::SetAdd_Folder(CString strName, CString strPath)
{
	BOOL bResult = FALSE;
	
	if (CreateDirectory(m_strUserDataPath + strPath, 0) == TRUE)
	{
		XMLTreeInfoFolder		 stXMLTreeInfoFolder;
		
		stXMLTreeInfoFolder.bLock = FALSE;
		stXMLTreeInfoFolder.nFileCnt = 0;
		
		stXMLTreeInfoFolder.strName = strName;
		stXMLTreeInfoFolder.strPath = strPath;
		
		stXMLTreeInfoFolder.plistXMLTreeInfoFile = new CList <XMLTreeInfoFile, XMLTreeInfoFile&>;
		
		m_plistXMLTreeInfoFolder->AddTail(stXMLTreeInfoFolder);
		
		bResult = TRUE;
	}
	
	return bResult;
}

BOOL CFolderEditDlg::SetInsert_Folder(int nIndex, CString strName, CString strPath)
{
	BOOL bResult = FALSE;
	
	SetControlState(FALSE);
	
	if (CreateDirectory(m_strUserDataPath + strPath, 0) == TRUE)
	{
		POSITION pos;
		XMLTreeInfoFolder		 stXMLTreeInfoFolder;
		
		stXMLTreeInfoFolder.bLock = FALSE;
		stXMLTreeInfoFolder.nFileCnt = 0;
		
		stXMLTreeInfoFolder.strName = strName;
		stXMLTreeInfoFolder.strPath = strPath;
		
		stXMLTreeInfoFolder.plistXMLTreeInfoFile = new CList <XMLTreeInfoFile, XMLTreeInfoFile&>;
		
		pos = m_plistXMLTreeInfoFolder->FindIndex(nIndex);
		m_plistXMLTreeInfoFolder->InsertBefore(pos, stXMLTreeInfoFolder);
		
		bResult = TRUE;
	}
	
	SetControlState();
	
	return bResult;
	
}


BOOL CFolderEditDlg::SetModify_Folder(CString strOldName, CString strOldPath, 
									  CString strNewName, CString strNewPath)
{
	BOOL bResult = FALSE;
	
	SetControlState(FALSE);
	
	POSITION posFolder, posFile;
	XMLTreeInfoFile      stXMLTreeInfoFile;
	XMLTreeInfoFolder		 stXMLTreeInfoFolder;
	
	int nCount = m_plistXMLTreeInfoFolder->GetCount();
	for (int nFolderIndex = 0; nFolderIndex < nCount; nFolderIndex++)
	{
		posFolder = m_plistXMLTreeInfoFolder->FindIndex(nFolderIndex);
		stXMLTreeInfoFolder = m_plistXMLTreeInfoFolder->GetAt(posFolder);
		
		if (strOldPath == stXMLTreeInfoFolder.strPath)
		{
			for (int nFileIndex = 0; nFileIndex < stXMLTreeInfoFolder.plistXMLTreeInfoFile->GetCount(); nFileIndex++)
			{
				posFile = stXMLTreeInfoFolder.plistXMLTreeInfoFile->FindIndex(nFileIndex);
				stXMLTreeInfoFile = stXMLTreeInfoFolder.plistXMLTreeInfoFile->GetAt(posFile);
				
				stXMLTreeInfoFile.strFolderPath = strNewPath;
				stXMLTreeInfoFolder.plistXMLTreeInfoFile->SetAt(posFile, stXMLTreeInfoFile);
			}
			
			CFile::Rename(m_strUserDataPath + strOldPath, m_strUserDataPath + strNewPath);
			
			stXMLTreeInfoFolder.strName = strNewName;
			stXMLTreeInfoFolder.strPath = strNewPath;
			m_plistXMLTreeInfoFolder->SetAt(posFolder, stXMLTreeInfoFolder);
			
			bResult = TRUE;
			break;
		}
	}
	
	SetControlState();
	
	if (bResult == FALSE)
	{
		CString strErrorMsg;
		strErrorMsg.Format("조건파일(%s)을 변경하지 못하였습니다.", strOldPath);
		SetMessageBox(strErrorMsg, (CString)PROGRAM_TITLE, MB_OK);
	}
	
	return bResult;
}


BOOL CFolderEditDlg::SetModify_File(CString strFolderPath,
									CString strOldName, CString strOldPath, 
									CString strNewName, CString strNewPath)
{
	BOOL bResult = FALSE;
	
	SetControlState(FALSE);
	
	POSITION posFolder, posFile;
	XMLTreeInfoFile      stXMLTreeInfoFile;
	XMLTreeInfoFolder		 stXMLTreeInfoFolder;
	
	int nCount = m_plistXMLTreeInfoFolder->GetCount();
	for (int nFolderIndex = 0; nFolderIndex < nCount; nFolderIndex++)
	{
		posFolder = m_plistXMLTreeInfoFolder->FindIndex(nFolderIndex);
		stXMLTreeInfoFolder = m_plistXMLTreeInfoFolder->GetAt(posFolder);
		
		if (strFolderPath == stXMLTreeInfoFolder.strPath)
		{
			for (int nFileIndex = 0; nFileIndex < stXMLTreeInfoFolder.plistXMLTreeInfoFile->GetCount(); nFileIndex++)
			{
				posFile = stXMLTreeInfoFolder.plistXMLTreeInfoFile->FindIndex(nFileIndex);
				stXMLTreeInfoFile = stXMLTreeInfoFolder.plistXMLTreeInfoFile->GetAt(posFile);
				
				if (strOldPath.Compare(stXMLTreeInfoFile.strFilePath) == 0)
				{
					CFile::Rename(m_strUserDataPath + strFolderPath + "\\" + strOldPath, 
						m_strUserDataPath + strFolderPath + "\\" + strNewPath);
					
					stXMLTreeInfoFile.strFileName = strNewName;
					stXMLTreeInfoFile.strFilePath = strNewPath;
					stXMLTreeInfoFolder.plistXMLTreeInfoFile->SetAt(posFile, stXMLTreeInfoFile);
					
					bResult = TRUE;
					break;
				}
			}
		}
	}
	
	SetControlState();
	
	if (bResult == FALSE)
	{
		CString strErrorMsg;
		strErrorMsg.Format("조건파일(%s)을 변경하지 못하였습니다.", strOldPath);
		SetMessageBox(strErrorMsg, (CString)PROGRAM_TITLE, MB_OK);
	}
	
	return bResult;
}


BOOL CFolderEditDlg::GetHaveFile(CString strFilePath, CString strFolderPath)
{
	BOOL bResult = FALSE;
	
	CFileFind			ffFile;
	BOOL bFindFile;
	CString strFileName, strFileTitle, strTemp;
	
	bFindFile = ffFile.FindFile(m_strUserDataPath + strFolderPath + "\\" + "*.*");
	while (bFindFile)
	{
		bFindFile = ffFile.FindNextFile();
		strFileName = ffFile.GetFileName();
		
		if (!ffFile.IsDirectory())
		{
			strFileTitle = ffFile.GetFileTitle();
			strTemp = strFileName;
			strTemp.Replace(strFileTitle, "");
			strTemp.MakeUpper();
			if ((strTemp.CompareNoCase(".XML") == 0) && (strFilePath == strFileName))
			{
				bResult = TRUE;
				break;
			}
		}
	}
	
	return bResult;
}

BOOL CFolderEditDlg::GetHaveFile(CString strFileName, int nInvertRow)
{
	BOOL bResult = FALSE;
	
	XMLTreeInfoFile	stXMLTreeInfoFile;
	for (int nRow = 0; nRow < m_pgridFile->GetNumberRows(); nRow++)
	{
		if ((nInvertRow != -1) && (nInvertRow != nRow))
		{
			m_pgridFile->GetData_File(nRow, stXMLTreeInfoFile);
			
			if (strFileName == stXMLTreeInfoFile.strFileName)
			{
				bResult = TRUE;
				break;
			}
		}
	}
	
	return bResult;
}

BOOL CFolderEditDlg::GetHaveFolder(CString strName, CString strPath)
{
	BOOL bResult = FALSE;
	
	CFileFind			ffDirectory;
	BOOL	bFindFolder;
	CString strFolderPath, strFolderName;
	
	bFindFolder = ffDirectory.FindFile(m_strUserDataPath + "*.*");
	while (bFindFolder)
	{
		bFindFolder = ffDirectory.FindNextFile();
		
		strFolderPath = ffDirectory.GetFileName();
		strFolderName = ffDirectory.GetFileTitle();
		if (ffDirectory.IsDirectory() && strFolderPath != "." && strFolderPath != "..")
		{
			if (strPath == strFolderName)
			{
				bResult = TRUE;
				break;
			}
		}
	}
	
	return bResult;
}

BOOL CFolderEditDlg::GetHaveFolder(CString strName, int nInvertRow)
{
	BOOL bResult = FALSE;
	
	InfoFolder	stInfoFolder;
	for (int nRow = 0; nRow < m_pgridFolder->GetNumberRows(); nRow++)
	{
		if ((nInvertRow != -1) && (nInvertRow != nRow))
		{
			m_pgridFolder->GetData_Folder(nRow, stInfoFolder);
			
			if (strName == stInfoFolder.strName)
			{
				bResult = TRUE;
				break;
			}
		}
	}
	
	return bResult;
}

BOOL CFolderEditDlg::GetCheckNAName(CString strType, CString strData)
{
	CString strErrorMsg;
	
	CString strNAinName = "\\\/\:\*\?\"\<\>\|";
	if (strData.FindOneOf(strNAinName.operator const char*()) != -1)
	{
		strErrorMsg.Format("%s명에 다음 문자를 사용할수 없습니다.\n(%s)", strType, strNAinName);
		SetMessageBox(strErrorMsg, (CString)PROGRAM_TITLE, MB_OK);
		
		m_editInput.SetFocus();
		return FALSE;
	}
	
	return TRUE;
}

void CFolderEditDlg::OnBtnEditCancel()
{
	m_strEditMode = "";
	
	SetBtnEnable(TRUE);
	SetEditBarView(FALSE);
}

void CFolderEditDlg::OnBtnOk()
{
	OnOK();
}

void CFolderEditDlg::SetBtnEnable(BOOL bEnable)
{
	m_btnImport.EnableWindow(bEnable);
	m_btnExport.EnableWindow(bEnable);
	
	m_btnRight.EnableWindow(bEnable);
	m_btnRightAll.EnableWindow(bEnable);
	
	if (m_pgridFile != NULL)
		m_pgridFile->EnableWindow(bEnable);
	
	if (m_pgridFolder != NULL)
		m_pgridFolder->EnableWindow(bEnable);
	
	m_btnOk.EnableWindow(bEnable);
}

void CFolderEditDlg::SetEditBarView(BOOL bView)
{
	int nCmdShow;
	nCmdShow = (!bView == TRUE)?SW_SHOW : SW_HIDE;
	
	m_ctrlCtrlMenuBar.ShowWindow(nCmdShow);
	m_btnFolderAdd.ShowWindow(nCmdShow);
	m_btnFolderInsert.ShowWindow(nCmdShow);
	m_btnFolderModify.ShowWindow(nCmdShow);
	m_btnFolderDel.ShowWindow(nCmdShow);
	m_btnFileModify.ShowWindow(nCmdShow);
	m_btnFileDel.ShowWindow(nCmdShow);
	
	nCmdShow = (bView == TRUE)?SW_SHOW : SW_HIDE;
	
	m_ctrlEditMenuBar.ShowWindow(nCmdShow);
	m_ctrlEditTitle.ShowWindow(nCmdShow);
	m_editInput.ShowWindow(nCmdShow);
	m_btnEditOk.ShowWindow(nCmdShow);
	m_btnEditCancel.ShowWindow(nCmdShow);
}

//void CFolderEditDlg::OnNotifyGrid(WPARAM wParam, LPARAM lParam)
LRESULT CFolderEditDlg::OnNotifyGrid(WPARAM wParam, LPARAM lParam)
{
	GridEventInfo		stGridEventInfo;
	
	stGridEventInfo = *(GridEventInfo*)lParam;
	if (wParam == NM_CLICK_L)
	{
		if (stGridEventInfo.strGridType == "Folder")
		{
			InfoFolder	stInfoFolder;
			if (m_pgridFolder->GetData_Folder(stGridEventInfo.nRow, stInfoFolder) == TRUE)
			{
				m_btnFolderModify.EnableWindow(!stInfoFolder.bLock);
				m_btnFolderDel.EnableWindow(!stInfoFolder.bLock);
				
				m_btnFolderModify.Invalidate();
				m_btnFolderDel.Invalidate();
				
				if (stGridEventInfo.nCol == 4)
					SetGrid_FolderCheck(stGridEventInfo.nRow);
				else
					SetViewFolderToFileList(stInfoFolder.strPath);
				
				m_ctrlSelectFolder.SetWindowText(stInfoFolder.strName);
				m_ctrlSelectFile.SetWindowText("");
			}
			else
				m_pgridFolder->m_nSelectRow = -1;		//# Init Failure!
		}
		else if (stGridEventInfo.strGridType == "File")
			SetSelectRow_File(stGridEventInfo.nRow);
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
		
	}
	
	return TRUE;
}

void CFolderEditDlg::SetSelectRow_File(int nRow)
{
	if (nRow == -1)
		m_ctrlSelectFile.SetWindowText("");
	else
	{
		XMLTreeInfoFile	stXMLTreeInfoFile;
		m_pgridFile->GetData_File(nRow, stXMLTreeInfoFile);
		
		m_ctrlSelectFile.SetWindowText(stXMLTreeInfoFile.strFileName);
		m_pgridFile->SetSelRow(nRow);
	}
}

void CFolderEditDlg::SetSelectRow_Folder(int nRow)
{
	if (nRow == -1)
		m_ctrlSelectFolder.SetWindowText("");
	else
	{
		InfoFolder	stInfoFolder;
		m_pgridFolder->GetData_Folder(nRow, stInfoFolder);
		
		m_ctrlSelectFolder.SetWindowText(stInfoFolder.strName);
		m_pgridFolder->SetSelRow(nRow);
	}
	m_ctrlSelectFile.SetWindowText("");
}

void CFolderEditDlg::SetGrid_FolderCheck(int nSelectRow)
{
	m_nMoveFolderRow = nSelectRow;
	
	for (int nRow = 0; nRow < m_pgridFolder->GetNumberRows(); nRow++)
	{
		if (nSelectRow == nRow)
		{
			//m_pgridFolder->QuickSetText(4, nRow, "1");
		}
		else
			m_pgridFolder->QuickSetText(4, nRow, "0");
	}
	m_pgridFolder->SetSelRow(nSelectRow);
	m_pgridFolder->RedrawAll();
}

void CFolderEditDlg::OnBtnUpFolder()
{ 
	CString strErrorMsg;
	int nRow = m_pgridFolder->m_nSelectRow;
	if (nRow == -1)
	{
		strErrorMsg.Format("선택된 그룹이 없습니다.");
		SetMessageBox(strErrorMsg, (CString)PROGRAM_TITLE, MB_OK);
	}
	else
	{
		if (nRow == 0)
		{
			strErrorMsg.Format("제일 상단의 그룹입니다.");
			SetMessageBox(strErrorMsg, (CString)PROGRAM_TITLE, MB_OK);
		}
		else
		{
			SetMove_Folder(nRow, nRow - 1);
			
			InfoFolder	stInfoFolder;
			m_pgridFolder->GetData_Folder(nRow, stInfoFolder);
			m_pgridFolder->DeleteRow(nRow);
			
			m_pgridFolder->InsertRow(nRow - 1);
			nRow = nRow - 1;
			m_pgridFolder->SetData_Folder(nRow, stInfoFolder, TRUE);
			m_pgridFolder->SetSelRow(nRow);
		}
	}
}

void CFolderEditDlg::OnBtnDownFolder()
{
	CString strErrorMsg;
	int nRow = m_pgridFolder->m_nSelectRow;
	if (nRow == -1)
	{
		strErrorMsg.Format("선택된 그룹이 없습니다.");
		SetMessageBox(strErrorMsg, (CString)PROGRAM_TITLE, MB_OK);
	}
	else
	{
		if (nRow == m_pgridFolder->GetNumberRows() - 1)
		{
			strErrorMsg.Format("제일 하단의 그룹입니다.");
			SetMessageBox(strErrorMsg, (CString)PROGRAM_TITLE, MB_OK);
		}
		else
		{
			SetMove_Folder(nRow, nRow + 1);
			
			InfoFolder	stInfoFolder;
			m_pgridFolder->GetData_Folder(nRow, stInfoFolder);
			m_pgridFolder->DeleteRow(nRow);
			
			m_pgridFolder->InsertRow(nRow + 1);
			nRow = nRow + 1;
			m_pgridFolder->SetData_Folder(nRow, stInfoFolder, TRUE);
			m_pgridFolder->SetSelRow(nRow);
		}
	}
}

void CFolderEditDlg::OnBtnTopFolder()
{
	CString strErrorMsg;
	int nRow = m_pgridFolder->m_nSelectRow;
	if (nRow == -1)
	{
		strErrorMsg.Format("선택된 그룹이 없습니다.");
		SetMessageBox(strErrorMsg, (CString)PROGRAM_TITLE, MB_OK);
	}
	else
	{
		if (nRow == 0)
		{
			strErrorMsg.Format("제일 상단의 그룹입니다.");
			SetMessageBox(strErrorMsg, (CString)PROGRAM_TITLE, MB_OK);
		}
		else
		{
			SetMove_Folder(nRow, 0);
			
			InfoFolder	stInfoFolder;
			m_pgridFolder->GetData_Folder(nRow, stInfoFolder);
			m_pgridFolder->DeleteRow(nRow);
			
			m_pgridFolder->InsertRow(0);
			nRow = 0;
			m_pgridFolder->SetData_Folder(nRow, stInfoFolder, TRUE);
			m_pgridFolder->SetSelRow(nRow);
		}
	}
}

void CFolderEditDlg::OnBtnBottomFolder()
{
	CString strErrorMsg;
	int nRow = m_pgridFolder->m_nSelectRow;
	if (nRow == -1)
	{
		strErrorMsg.Format("선택된 그룹이 없습니다.");
		SetMessageBox(strErrorMsg, (CString)PROGRAM_TITLE, MB_OK);
	}
	else
	{
		if (nRow == m_pgridFolder->GetNumberRows() - 1)
		{
			strErrorMsg.Format("제일 하단의 그룹입니다.");
			SetMessageBox(strErrorMsg, (CString)PROGRAM_TITLE, MB_OK);
		}
		else
		{
			SetMove_Folder(nRow, m_pgridFolder->GetNumberRows() - 1);
			
			InfoFolder	stInfoFolder;
			m_pgridFolder->GetData_Folder(nRow, stInfoFolder);
			m_pgridFolder->DeleteRow(nRow);
			
			m_pgridFolder->AppendRow();
			nRow = m_pgridFolder->GetNumberRows() - 1;
			m_pgridFolder->SetData_Folder(nRow, stInfoFolder, TRUE);
			m_pgridFolder->SetSelRow(nRow);
		}
	}
}

void CFolderEditDlg::OnBtnUpFile()
{
	CString strErrorMsg;
	int nRow = m_pgridFile->m_nSelectRow;
	if (nRow == -1)
	{
		strErrorMsg.Format("선택된 조건이 없습니다.");
		SetMessageBox(strErrorMsg, (CString)PROGRAM_TITLE, MB_OK);
	}
	else
	{
		if (nRow == 0)
		{
			strErrorMsg.Format("제일 상단의 조건입니다.");
			SetMessageBox(strErrorMsg, (CString)PROGRAM_TITLE, MB_OK);
		}
		else
		{
			XMLTreeInfoFile	stXMLTreeInfoFile;
			m_pgridFile->GetData_File(nRow, stXMLTreeInfoFile);
			m_pgridFile->DeleteRow(nRow);
			
			SetMove_File(stXMLTreeInfoFile.strFolderPath, nRow, nRow - 1);
			
			m_pgridFile->InsertRow(nRow - 1);
			nRow = nRow - 1;
			m_pgridFile->SetData_File(nRow, stXMLTreeInfoFile, TRUE);
			m_pgridFile->SetSelRow(nRow);
		}
	}
}

void CFolderEditDlg::OnBtnDownFile()
{
	CString strErrorMsg;
	int nRow = m_pgridFile->m_nSelectRow;
	if (nRow == -1)
	{
		strErrorMsg.Format("선택된 조건이 없습니다.");
		SetMessageBox(strErrorMsg, (CString)PROGRAM_TITLE, MB_OK);
	}
	else
	{
		if (nRow == m_pgridFile->GetNumberRows() - 1)
		{
			strErrorMsg.Format("제일 하단의 조건입니다.");
			SetMessageBox(strErrorMsg, (CString)PROGRAM_TITLE, MB_OK);
		}
		else
		{
			XMLTreeInfoFile	stXMLTreeInfoFile;
			m_pgridFile->GetData_File(nRow, stXMLTreeInfoFile);
			m_pgridFile->DeleteRow(nRow);
			
			SetMove_File(stXMLTreeInfoFile.strFolderPath, nRow, nRow + 1);
			
			m_pgridFile->InsertRow(nRow + 1);
			nRow = nRow + 1;
			m_pgridFile->SetData_File(nRow, stXMLTreeInfoFile, TRUE);
			m_pgridFile->SetSelRow(nRow);
		}
	}
}

void CFolderEditDlg::OnBtnTopFile()
{
	CString strErrorMsg;
	int nRow = m_pgridFile->m_nSelectRow;
	if (nRow == -1)
	{
		strErrorMsg.Format("선택된 조건이 없습니다.");
		SetMessageBox(strErrorMsg, (CString)PROGRAM_TITLE, MB_OK);
	}
	else
	{
		if (nRow == 0)
		{
			strErrorMsg.Format("제일 상단의 조건입니다.");
			SetMessageBox(strErrorMsg, (CString)PROGRAM_TITLE, MB_OK);
		}
		else
		{
			XMLTreeInfoFile	stXMLTreeInfoFile;
			m_pgridFile->GetData_File(nRow, stXMLTreeInfoFile);
			m_pgridFile->DeleteRow(nRow);
			
			SetMove_File(stXMLTreeInfoFile.strFolderPath, nRow, 0);
			
			m_pgridFile->InsertRow(0);
			nRow = 0;
			m_pgridFile->SetData_File(nRow, stXMLTreeInfoFile, TRUE);
			m_pgridFile->SetSelRow(nRow);
		}
	}
}

void CFolderEditDlg::OnBtnBottomFile()
{
	CString strErrorMsg;
	int nRow = m_pgridFile->m_nSelectRow;
	if (nRow == -1)
	{
		strErrorMsg.Format("선택된 조건이 없습니다.");
		SetMessageBox(strErrorMsg, (CString)PROGRAM_TITLE, MB_OK);
	}
	else
	{
		if (nRow == m_pgridFile->GetNumberRows() - 1)
		{
			strErrorMsg.Format("제일 하단의 조건입니다.");
			SetMessageBox(strErrorMsg, (CString)PROGRAM_TITLE, MB_OK);
		}
		else
		{
			XMLTreeInfoFile	stXMLTreeInfoFile;
			m_pgridFile->GetData_File(nRow, stXMLTreeInfoFile);
			m_pgridFile->DeleteRow(nRow);
			
			SetMove_File(stXMLTreeInfoFile.strFolderPath, nRow, m_pgridFile->GetNumberRows() - 1);
			
			m_pgridFile->AppendRow();
			nRow = m_pgridFile->GetNumberRows() - 1;
			m_pgridFile->SetData_File(nRow, stXMLTreeInfoFile, TRUE);
			m_pgridFile->SetSelRow(nRow);
		}
	}
}


void CFolderEditDlg::SetMove_Folder(int nTargetPos, int nMovePos)
{
	XMLTreeInfoFolder	stXMLTreeInfoFolder;
	
	POSITION posTargetFolder, posMoveFolder;
	InfoFolder	stInfoFolder;
	m_pgridFolder->GetData_Folder(nTargetPos, stInfoFolder);
	
	posTargetFolder = GetPosFolder(m_plistXMLTreeInfoFolder, stInfoFolder.strPath);
	stXMLTreeInfoFolder = m_plistXMLTreeInfoFolder->GetAt(posTargetFolder);
	
	m_pgridFolder->GetData_Folder(nMovePos, stInfoFolder);
	posMoveFolder = GetPosFolder(m_plistXMLTreeInfoFolder, stInfoFolder.strPath);
	
	if ((nMovePos == 0) || (nTargetPos > nMovePos))
		m_plistXMLTreeInfoFolder->InsertBefore(posMoveFolder, stXMLTreeInfoFolder);
	else 
		m_plistXMLTreeInfoFolder->InsertAfter(posMoveFolder, stXMLTreeInfoFolder);
	
	m_plistXMLTreeInfoFolder->RemoveAt(posTargetFolder);
}


BOOL CFolderEditDlg::SetMove_File(CString strFolderPath, int nTargetPos, int nMovePos)
{
	XMLTreeInfoFile	stXMLTreeInfoFile;
	XMLTreeInfoFolder	stXMLTreeInfoFolder;
	
	POSITION posFolder, posTargetFile;
	POSITION posMoveFile;
	
	posFolder = GetPosFolder(m_plistXMLTreeInfoFolder, strFolderPath);
	if (posFolder == NULL)
		return FALSE;
	
	stXMLTreeInfoFolder = m_plistXMLTreeInfoFolder->GetAt(posFolder);
	
	posTargetFile = stXMLTreeInfoFolder.plistXMLTreeInfoFile->FindIndex(nTargetPos);
	stXMLTreeInfoFile = stXMLTreeInfoFolder.plistXMLTreeInfoFile->GetAt(posTargetFile);
	
	posMoveFile = stXMLTreeInfoFolder.plistXMLTreeInfoFile->FindIndex(nMovePos);
	
	if ((nMovePos == 0) || (nTargetPos - nMovePos > 0))
		stXMLTreeInfoFolder.plistXMLTreeInfoFile->InsertBefore(posMoveFile, stXMLTreeInfoFile);
	else if (nTargetPos - nMovePos < 0)
		stXMLTreeInfoFolder.plistXMLTreeInfoFile->InsertAfter(posMoveFile, stXMLTreeInfoFile);
	
	stXMLTreeInfoFolder.plistXMLTreeInfoFile->RemoveAt(posTargetFile);
	
	return TRUE;
}

BOOL CFolderEditDlg::SetMove_File(CString strTargetFolderPath, CString strFilePath, CString strMoveFolderPath)
{
	XMLTreeInfoFile	stXMLTreeInfoFile;
	XMLTreeInfoFolder	stTargetInfoFolder, stMoveInfoFolder;
	
	POSITION posTargetFolder, posTargetFile;
	POSITION posMoveFolder;
	
	posTargetFolder = GetPosFolder(m_plistXMLTreeInfoFolder, strTargetFolderPath);
	if (posTargetFolder == NULL)
		return FALSE;
	stTargetInfoFolder = m_plistXMLTreeInfoFolder->GetAt(posTargetFolder);
	
	posTargetFile = GetPosFile(stTargetInfoFolder.plistXMLTreeInfoFile, strFilePath);
	stXMLTreeInfoFile = stTargetInfoFolder.plistXMLTreeInfoFile->GetAt(posTargetFile);
	
	posMoveFolder = GetPosFolder(m_plistXMLTreeInfoFolder, strMoveFolderPath);
	if (posMoveFolder == NULL)
		return FALSE;
	
	stXMLTreeInfoFile.strFolderPath = strMoveFolderPath;
	stMoveInfoFolder = m_plistXMLTreeInfoFolder->GetAt(posMoveFolder);
	
	stMoveInfoFolder.plistXMLTreeInfoFile->AddTail(stXMLTreeInfoFile);
	stMoveInfoFolder.nFileCnt++;
	m_plistXMLTreeInfoFolder->SetAt(posMoveFolder, stMoveInfoFolder);
	
	stTargetInfoFolder.plistXMLTreeInfoFile->RemoveAt(posTargetFile);
	stTargetInfoFolder.nFileCnt--;
	m_plistXMLTreeInfoFolder->SetAt(posTargetFolder, stTargetInfoFolder);
	
	CFile::Rename(m_strUserDataPath + strTargetFolderPath + "\\" + strFilePath, 
								m_strUserDataPath + strMoveFolderPath + "\\" + strFilePath);
	
	return TRUE;
}


POSITION CFolderEditDlg::GetPosFolder(CList<XMLTreeInfoFolder, XMLTreeInfoFolder&> *pFolderList, 
									  CString strFolderPath)
{
	POSITION posResult = NULL;
	
	XMLTreeInfoFolder	stXMLTreeInfoFolder;
	
	POSITION posFolder;
	int nFolderIndex;
	for (nFolderIndex = 0; nFolderIndex < pFolderList->GetCount(); nFolderIndex++)
	{
		posFolder = pFolderList->FindIndex(nFolderIndex);
		stXMLTreeInfoFolder = pFolderList->GetAt(posFolder);
		
		if (strFolderPath == stXMLTreeInfoFolder.strPath)
		{
			posResult = posFolder;
			break;
		}
	}
	
	return posResult;
}


POSITION CFolderEditDlg::GetPosFile(CList<XMLTreeInfoFolder, XMLTreeInfoFolder&> *pFolderList, 
									CString strFolderPath, CString strFilePath)
{
	POSITION posResult = NULL;
	
	XMLTreeInfoFile	stXMLTreeInfoFile;
	XMLTreeInfoFolder	stXMLTreeInfoFolder;
	
	POSITION posFolder, posFile;
	int nFolderIndex, nFileIndex;
	for (nFolderIndex = 0; nFolderIndex < pFolderList->GetCount(); nFolderIndex++)
	{
		posFolder = pFolderList->FindIndex(nFolderIndex);
		stXMLTreeInfoFolder = pFolderList->GetAt(posFolder);
		
		if (strFolderPath == stXMLTreeInfoFolder.strPath)
		{
			for (nFileIndex = 0; nFileIndex < stXMLTreeInfoFolder.plistXMLTreeInfoFile->GetCount(); nFileIndex++)
			{
				posFile = stXMLTreeInfoFolder.plistXMLTreeInfoFile->FindIndex(nFileIndex);
				stXMLTreeInfoFile = stXMLTreeInfoFolder.plistXMLTreeInfoFile->GetAt(posFile);
				
				if (strFilePath == stXMLTreeInfoFile.strFilePath)
				{
					posResult = posFile;
					break;
				}
			}
		}
	}
	
	return posResult;
}

POSITION CFolderEditDlg::GetPosFile(CList	<XMLTreeInfoFile, XMLTreeInfoFile&> *pFileList,
									CString strFilePath)
{
	POSITION posResult = NULL;
	
	XMLTreeInfoFile	stXMLTreeInfoFile;
	POSITION posFile;
	int nFileIndex;
	
	for (nFileIndex = 0; nFileIndex < pFileList->GetCount(); nFileIndex++)
	{
		posFile = pFileList->FindIndex(nFileIndex);
		stXMLTreeInfoFile = pFileList->GetAt(posFile);
		
		if (strFilePath == stXMLTreeInfoFile.strFilePath)
		{
			posResult = posFile;
			break;
		}
	}
	
	return posResult;
}


void CFolderEditDlg::SetViewFolderToFileList(CString strFolderPath)
{
	m_pgridFile->ClearAll();
	
	XMLTreeInfoFile	stXMLTreeInfoFile;
	XMLTreeInfoFolder	stXMLTreeInfoFolder;
	
	POSITION posFolder, posFile;
	int nFolderIndex, nFileIndex;
	for (nFolderIndex = 0; nFolderIndex < m_plistXMLTreeInfoFolder->GetCount(); nFolderIndex++)
	{
		posFolder = m_plistXMLTreeInfoFolder->FindIndex(nFolderIndex);
		stXMLTreeInfoFolder = m_plistXMLTreeInfoFolder->GetAt(posFolder);
		
		if (strFolderPath == stXMLTreeInfoFolder.strPath)
		{
			for (nFileIndex = 0; nFileIndex < stXMLTreeInfoFolder.plistXMLTreeInfoFile->GetCount(); nFileIndex++)
			{
				posFile = stXMLTreeInfoFolder.plistXMLTreeInfoFile->FindIndex(nFileIndex);
				stXMLTreeInfoFile = stXMLTreeInfoFolder.plistXMLTreeInfoFile->GetAt(posFile);
				
				m_pgridFile->AppendRow();
				m_pgridFile->SetData_File(nFileIndex, stXMLTreeInfoFile, FALSE);
			}
			m_pgridFile->m_nSelectRow = -1;
			m_pgridFile->RedrawAll();
			break;
		}
	}
}

void CFolderEditDlg::SetControlState(BOOL bState)
{
	return;
	if (bState == TRUE)
		::SetCursor(AfxGetApp()->LoadStandardCursor(IDC_ARROW));	
	else
		::SetCursor(AfxGetApp()->LoadStandardCursor(IDC_WAIT));
}

int CFolderEditDlg::SetMessageBox(LPCTSTR lpszText, LPCTSTR lpszCaption, UINT nType)
{
	CMainFrmWnd *pwndMainFrm = (CMainFrmWnd*)m_pwndMain;
	return pwndMainFrm->SetMessageBox(lpszText, PROGRAM_TITLE, nType);
}

BOOL CFolderEditDlg::PreTranslateMessage(MSG* pMsg) 
{
	if (pMsg->message == WM_KEYDOWN)
	{
		if (IDC_EDIT_INPUT == ::GetDlgCtrlID(pMsg->hwnd))
		{
			if (pMsg->wParam == VK_RETURN)
			{
				if (m_strEditMode.IsEmpty() == FALSE)
					OnBtnEditOk();
			}
			else if (pMsg->wParam == VK_ESCAPE)
				OnBtnEditCancel();
		}
		
		return FALSE;
	}
	
	return CDialog::PreTranslateMessage(pMsg);
}

void CFolderEditDlg::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	CRect reClient;
	GetClientRect(reClient);
	
	dc.FillSolidRect(reClient, COLOR_DLG_CANVAS);	
}
