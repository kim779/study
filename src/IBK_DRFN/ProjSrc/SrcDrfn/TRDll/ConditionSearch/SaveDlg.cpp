// SaveDlg.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include "SaveDlg.h"

#include "MainFrmWnd.h"
#include "ConditionMakeWnd.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern HINSTANCE g_hInstance;

/////////////////////////////////////////////////////////////////////////////
// CSaveDlg dialog
#pragma warning( disable : 4101 4800 4129 4700 ) 

CSaveDlg::CSaveDlg(CWnd* pParent, CString strLoadedFileName, CString strLoadedFolderPath)
: CDialog(CSaveDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSaveDlg)
	//}}AFX_DATA_INIT
	m_strNotUse = "\\\/\:\*\?\"\<\>\|\,";
	
	m_pwndMain = pParent;
	m_strLoadedFileName = strLoadedFileName;
	m_strLoadedFolderPath = strLoadedFolderPath;
	
	
	m_bUpDate = FALSE;
}

void CSaveDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSaveDlg)
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSaveDlg, CDialog)
//{{AFX_MSG_MAP(CSaveDlg)
ON_WM_CHAR()
ON_WM_PAINT()
ON_WM_CTLCOLOR()
ON_WM_CREATE()
ON_WM_SIZE()
ON_WM_DESTROY()
//}}AFX_MSG_MAP

ON_BN_CLICKED(IDC_BTN_SAVEDEL, OnBtnSaveDel)
ON_BN_CLICKED(IDC_BTN_SAVEOK, OnBtnSaveOk)
ON_BN_CLICKED(IDC_BTN_SAVECANCEL, OnBtnSaveCancel)

ON_MESSAGE(WM_XMLTREE_CTRL, OnNotifyXMLTree)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSaveDlg message handlers

int CSaveDlg::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	HINSTANCE hInstSave = AfxGetResourceHandle();
	AfxSetResourceHandle(g_hInstance);

	LOGFONT	  logFont;
	GETFONT(logFont, 12, 400, m_fontDefault);
	m_brushBk.CreateSolidBrush(COLOR_DLG_CANVAS);

	CMainFrmWnd *pwndMainFrm = (CMainFrmWnd*)m_pwndMain;
		
	m_treeXmlCtrl.Create(this, IDC_TREE_USER);
	
	m_ctrlTitleFile.Create(_T("조건명"), WS_CHILD | WS_VISIBLE | SS_CENTER, 
		CRect(0, 0, 0, 0), this);
	m_ctrlTitleFile.SetFont(&m_fontDefault);
	
	m_editFileName.Create(WS_CHILD | WS_VISIBLE | WS_TABSTOP | WS_BORDER,
		CRect(0, 0, 0, 0), this, IDC_EDIT_FILENAME);
	m_editFileName.ModifyStyleEx(WS_EX_CLIENTEDGE, 0);
	m_editFileName.SetFont(&m_fontDefault);
	
	m_ctrlTitleFolder.Create(_T("그룹명"), WS_CHILD | WS_VISIBLE | SS_CENTER, 
		CRect(0, 0, 0, 0), this);
	m_ctrlTitleFolder.SetFont(&m_fontDefault);
	
	m_editFolderName.Create(WS_CHILD | WS_VISIBLE | WS_TABSTOP | WS_BORDER,
		CRect(0, 0, 0, 0), this, IDC_EDIT_FOLDERNAME);
	m_editFolderName.ModifyStyleEx(WS_EX_CLIENTEDGE, 0);
	m_editFolderName.SetFont(&m_fontDefault);
	
	
	m_btnDel.Create(_T("삭제"), CRect(0, 0, 0, 0), this, IDC_BTN_SAVEDEL);
	pwndMainFrm->SetBtnImg(&m_btnDel, 4);
	
	m_btnOK.Create(_T("저장"), CRect(0, 0, 0, 0), this, IDC_BTN_SAVEOK);
	pwndMainFrm->SetBtnImg(&m_btnOK, 4);
	
	m_btnCancel.Create(_T("닫기"), CRect(0, 0, 0, 0), this, IDC_BTN_SAVECANCEL);
	pwndMainFrm->SetBtnImg(&m_btnCancel, 4);

	AfxSetResourceHandle(hInstSave);
	
	CRect reClient;
	GetClientRect(reClient);
	
	reClient.right = reClient.left + 300;
	reClient.bottom = reClient.top + 300;
	MoveWindow(reClient);
	
	return 0;
}

void CSaveDlg::OnDestroy() 
{
	CDialog::OnDestroy();
	
	m_fontDefault.DeleteObject();
	m_brushBk.DeleteObject();
	
	if (m_bUpDate == TRUE)
		OnOK();
}

void CSaveDlg::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);
	
	CRect reClient, reBottom, reTemp;
	GetClientRect(reClient);
	
	reClient.left += 5;
	reClient.right -= 5;
	reClient.top += 5;
	reClient.bottom -= 5;
	
	reBottom = reClient;
	
	reBottom.top = reBottom.bottom - 20;
	reTemp = reBottom;
	reTemp.right = reTemp.left + 60;
	m_btnDel.MoveWindow(reTemp);
	
	reTemp = reBottom;
	reTemp.left = reTemp.right - 60;
	m_btnCancel.MoveWindow(reTemp);
	
	reTemp.right = reTemp.left - 2;
	reTemp.left = reTemp.right - 60;
	m_btnOK.MoveWindow(reTemp);
	
	
	reBottom.bottom = reBottom.top - 3;
	reBottom.top = reBottom.bottom - 20;
	reTemp = reBottom;
	
	
	reTemp.top += 5;
	reTemp.right = reTemp.left + 40;
	m_ctrlTitleFile.MoveWindow(reTemp);
	
	reTemp.top -= 4;
	reTemp.left = reTemp.right + 5;
	reTemp.right = reClient.right;
	m_editFileName.MoveWindow(reTemp);
	
	
	reBottom.bottom = reBottom.top - 2;
	reBottom.top = reBottom.bottom - 20;
	reTemp = reBottom;
	
	reTemp.top += 4;
	reTemp.right = reTemp.left + 40;
	m_ctrlTitleFolder.MoveWindow(reTemp);
	
	reTemp.top -= 4;
	reTemp.left = reTemp.right + 5;
	reTemp.right = reClient.right;
	m_editFolderName.MoveWindow(reTemp);
	
	
	reBottom.bottom = reBottom.top - 5;
	reBottom.top = reClient.top;
	m_treeXmlCtrl.MoveWindow(reBottom);
}


void CSaveDlg::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	if (nChar == VK_ESCAPE)
		OnOK();
	
	CDialog::OnChar(nChar, nRepCnt, nFlags);
}

BOOL CSaveDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	CMainFrmWnd *pwndMainFrm = (CMainFrmWnd*)m_pwndMain;
	m_plistXMLTreeInfoFolder = &pwndMainFrm->m_listUserInfoFolder;
	m_treeXmlCtrl.SetInitTreeList(&pwndMainFrm->m_listUserInfoFolder);
	
	m_strUserDataPath = pwndMainFrm->GetFolderPath("USER") + "\\";
	
	if (m_treeXmlCtrl.SetSelectItme(m_strLoadedFolderPath, m_strLoadedFileName) == FALSE)
	{
		m_treeXmlCtrl.SetSelectItme((CString)FOLDER_TEMPFILE, "");
		m_strLoadedFolderPath = (CString)FOLDER_TEMPFILE;
	}
	
	m_editFolderName.SetWindowText(m_strLoadedFolderPath);
	m_editFileName.SetWindowText(m_strLoadedFileName);
	
	m_editFileName.SetFocus();
	m_editFileName.SetSel(0, m_strLoadedFileName.GetLength());
	
	return TRUE;
}

//void CSaveDlg::OnNotifyXMLTree(WPARAM wParam, LPARAM lParam)
LRESULT CSaveDlg::OnNotifyXMLTree(WPARAM wParam, LPARAM lParam)
{
	if (wParam == NM_SELECT_FILE)
	{
		CStringArray *psaData = (CStringArray*)lParam;
		
		if (psaData->GetSize() == 1)		//# Folder
		{
			m_editFolderName.SetWindowText(psaData->GetAt(0));
			m_editFileName.SetWindowText("");
		}
		else if (psaData->GetSize() == 2)		//# File + Folder
		{
			m_editFolderName.SetWindowText(psaData->GetAt(1));
			m_editFileName.SetWindowText(psaData->GetAt(0));
			
			m_editFileName.SetFocus();
			m_editFileName.SetSel(0, psaData->GetAt(0).GetLength());
		}
	}

	return TRUE;
}

void CSaveDlg::OnBtnSaveDel()
{
	CString strMessage;
	
	HTREEITEM hSelItem;
	CString strFilePath, strFolderPath;
	m_editFolderName.GetWindowText(strFolderPath);
	m_editFileName.GetWindowText(strFilePath);
	if (strFilePath.IsEmpty() == FALSE)
		strFilePath += ".xml";
	
	if (strFilePath.IsEmpty() == TRUE)
	{
		if (strFolderPath.IsEmpty() == TRUE)
			SetMessageBox("삭제할 그룹을 선택하시오.");
		else
		{
			//# Del Folder!
			strMessage = CString("그룹(") + strFolderPath + ") 을 삭제하려 합니다.\n삭제하시겠습니까?\n\n[주의] 본 그룹내의 조건들은 모두 삭제됩니다.";
			if (SetMessageBox(strMessage, (CString)PROGRAM_TITLE, MB_OKCANCEL | MB_TOPMOST) == IDOK)
			{
				if (SetDelete_Folder(strFolderPath) == TRUE)
				{
					//# Tree UpDate!
					hSelItem = m_treeXmlCtrl.GetSelectedItem();
					if (hSelItem != NULL)
						m_treeXmlCtrl.SetDeleteItem(hSelItem);
					
					m_bUpDate = TRUE;
				}
				else
				{
					strMessage.Format("그룹(%s)를 삭제하지 못하였습니다.", strFolderPath);
					SetMessageBox(strMessage, (CString)PROGRAM_TITLE, MB_ICONWARNING | MB_OK);
				}
			}
		}
	}
	else if (strFolderPath.IsEmpty() == TRUE)
	{
		if (strFilePath.IsEmpty() == TRUE)
			SetMessageBox("삭제할 조건을 선택하시오.");
		else
			SetMessageBox("삭제할 그룹를 선택하시오.");
	}
	else
	{
		//# Del File!
		strMessage = CString("조건(") + strFilePath + ") 을 삭제하려 합니다.\n삭제하시겠습니까?";
		if (SetMessageBox(strMessage, (CString)PROGRAM_TITLE, MB_OKCANCEL) == IDOK)
		{
			if (SetDelete_File(strFilePath, strFolderPath) == TRUE)
			{
				//# Tree UpDate!
				hSelItem = m_treeXmlCtrl.GetSelectedItem();
				if (hSelItem != NULL)
					m_treeXmlCtrl.SetDeleteItem(hSelItem);
				
				m_bUpDate = TRUE;
			}
			else
			{
				strMessage.Format("조건(%s)을 삭제하지 못하였습니다.", strFilePath);
				SetMessageBox(strMessage, (CString)PROGRAM_TITLE, MB_ICONWARNING | MB_OK);
			}
		}
	}
}

void CSaveDlg::SetControlState(BOOL bState)
{
	return;
	
	if (bState == TRUE)
		::SetCursor(AfxGetApp()->LoadStandardCursor(IDC_ARROW));	
	else
		::SetCursor(AfxGetApp()->LoadStandardCursor(IDC_WAIT));
}

BOOL CSaveDlg::SetDelete_File(CString strFilePath, CString strFolderPath)
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

BOOL CSaveDlg::SetDelete_Folder(CString strFolderPath)
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

void CSaveDlg::OnBtnSaveCancel()
{
	if (m_bUpDate == TRUE)
	{
		m_bUpDate = FALSE;
		OnOK();
	}
	else
	{
		OnCancel();
	}
}

void CSaveDlg::OnBtnSaveOk() 
{
	CString strFileName, strFolderName;
	m_editFileName.GetWindowText(strFileName);
	m_editFolderName.GetWindowText(strFolderName);
	
	strFileName.TrimLeft();
	strFileName.TrimRight();
	
	strFolderName.TrimLeft();
	strFolderName.TrimRight();
	
	if (strFileName.IsEmpty() == TRUE)
	{
		SetMessageBox("저장할 파일명이 없습니다.", PROGRAM_TITLE, MB_OK);
		m_editFileName.SetFocus();
		return;
	}
	else if (strFolderName.IsEmpty() == TRUE)
	{
		SetMessageBox("저장할 그룹명이 없습니다.", PROGRAM_TITLE, MB_OK);
		m_editFileName.SetFocus();
		return;
	}
	else if (GetCheckFileName(strFileName) == FALSE)
	{
		m_editFileName.SetFocus();
		m_editFileName.SetSel(0, strFileName.GetLength());
		return;
	}
	
	if (GetHaveFolder(strFolderName) == FALSE)
	{
		SetMessageBox("저장그룹을 재선택하시오.", PROGRAM_TITLE, MB_OK);
		m_editFolderName.SetFocus();
		m_editFolderName.SetSel(0, strFileName.GetLength());
		return;
	}
	
	CMainFrmWnd *pwndMainFrm = (CMainFrmWnd*)m_pwndMain;
	CConditionMakeWnd *pCondWnd = (CConditionMakeWnd *)pwndMainFrm->GetWnd("MAKE");

	BOOL bSaveState = TRUE;
	if (GetCheckSameFile(strFolderName, strFileName) == FALSE)
	{
		if (pCondWnd->SetSaveFile(strFolderName, strFileName) == FALSE)
		{
			SetMessageBox("조건 저장시 에러", PROGRAM_TITLE, MB_OK);
			bSaveState = FALSE;
		}
		else
			SetUpdate_FileData(FALSE, strFolderName, strFileName);
	}
	else
	{
		if (SetMessageBox("이미 같은 이름의 사용자 조건이 있습니다. \n기존조건을 이조건으로 바꾸시겠습니까?", 
			PROGRAM_TITLE, MB_OKCANCEL) == IDOK)
		{
			if (pCondWnd->SetSaveFile(strFolderName, strFileName) == FALSE)
			{
				SetMessageBox("조건 저장시 에러", PROGRAM_TITLE, MB_OK);
				bSaveState = FALSE;
			}
			else
				SetUpdate_FileData(TRUE, strFolderName, strFileName);
		}
		else
		{
			m_editFileName.SetFocus();
			m_editFileName.SetSel(0, strFileName.GetLength());
			bSaveState = FALSE;
		}
	}
	
	if (bSaveState == TRUE)
	{
		CMainFrmWnd *pwndMainFrm = (CMainFrmWnd*)m_pwndMain;
		CConditionMakeWnd *pwndConditionMake = (CConditionMakeWnd *)pwndMainFrm->GetWnd("MAKE");
		pwndConditionMake->SetSendFileName(strFileName, strFolderName);
		OnOK();
	}
}


BOOL CSaveDlg::GetCheckFileName(CString strFileName)
{
	CString strErrorMsg;
	if (strFileName.FindOneOf(m_strNotUse.operator const char*()) != -1)
	{
		strErrorMsg.Format("파일명에 다음 문자를 사용할수 없습니다.\n(%s)", m_strNotUse);
		SetMessageBox(strErrorMsg, PROGRAM_TITLE, MB_OK);
		m_editFileName.SetFocus();
		
		return FALSE;
	}
	
	return TRUE;
}

BOOL CSaveDlg::GetCheckSameFile(CString strFolderName, CString strFileName)
{
	BOOL bResult = FALSE;
	
	strFileName += ".xml";
	
	CMainFrmWnd *pwndMainFrm = (CMainFrmWnd*)m_pwndMain;
	bResult = pwndMainFrm->GetFindFile(strFolderName, strFileName, &pwndMainFrm->m_listUserInfoFolder);
	
	return bResult;
}

BOOL CSaveDlg::GetHaveFolder(CString strFolderPath)
{
	BOOL bResult = FALSE;
	
	CString strMessage;
	CMainFrmWnd *pwndMainFrm = (CMainFrmWnd*)m_pwndMain;
	if (pwndMainFrm->GetFindFolder(strFolderPath, &pwndMainFrm->m_listUserInfoFolder) == FALSE)
	{
		strMessage = strFolderPath + " 그룹은 존재하지 않습니다.\n 해당 그룹을 생성하시겠습니까?";
		
		if (SetMessageBox(strMessage, PROGRAM_TITLE, MB_OKCANCEL) == IDOK)
		{
			CString strRoot = pwndMainFrm->GetFolderPath("USER") + "\\" + strFolderPath;
			
			if (CreateDirectory(strRoot, 0) == TRUE)
			{
				XMLTreeInfoFolder		 stXMLTreeInfoFolder;
				
				stXMLTreeInfoFolder.bLock = FALSE;
				stXMLTreeInfoFolder.nFileCnt = 0;
				stXMLTreeInfoFolder.strName = strFolderPath;
				stXMLTreeInfoFolder.strPath = strFolderPath;
				
				stXMLTreeInfoFolder.plistXMLTreeInfoFile = new CList <XMLTreeInfoFile, XMLTreeInfoFile&>;
				
				pwndMainFrm->m_listUserInfoFolder.AddTail(stXMLTreeInfoFolder);
				
				bResult = TRUE;
			}
		}
	}
	else
		bResult = TRUE;
	
	return bResult;
}

void CSaveDlg::SetUpdate_FileData(BOOL bModify, CString strFolderName, CString strFileName)
{
	CMainFrmWnd *pwndMainFrm = (CMainFrmWnd*)m_pwndMain;
	
	POSITION posFolder, posFile;
	XMLTreeInfoFolder		stXMLTreeInfoFolder;
	XMLTreeInfoFile			stXMLTreeInfoFile;
	
	posFolder = pwndMainFrm->GetPosFolder(&pwndMainFrm->m_listUserInfoFolder, strFolderName);
	if (posFolder != NULL)
	{
		stXMLTreeInfoFolder = pwndMainFrm->m_listUserInfoFolder.GetAt(posFolder);
		
		stXMLTreeInfoFile.strType = "USER";		//# USER, SYS
		stXMLTreeInfoFile.strFolderPath = strFolderName;
		stXMLTreeInfoFile.strFilePath = strFileName + ".xml";
		stXMLTreeInfoFile.strFileName = strFileName;
		stXMLTreeInfoFile.bQuickReg = FALSE;
		
		pwndMainFrm->GetFileSetState(stXMLTreeInfoFile.strType,
			stXMLTreeInfoFile.strFolderPath, stXMLTreeInfoFile.strFilePath,
			stXMLTreeInfoFile.strAlertReg, stXMLTreeInfoFile.bUnitCan);
		
		if (bModify == TRUE)
		{
			posFile = pwndMainFrm->GetPosFile(stXMLTreeInfoFolder.plistXMLTreeInfoFile,
				stXMLTreeInfoFile.strFolderPath, stXMLTreeInfoFile.strFilePath);
			
			stXMLTreeInfoFolder.plistXMLTreeInfoFile->SetAt(posFile, stXMLTreeInfoFile);
		}
		else
			stXMLTreeInfoFolder.plistXMLTreeInfoFile->AddTail(stXMLTreeInfoFile);
		
		pwndMainFrm->m_listUserInfoFolder.SetAt(posFolder, stXMLTreeInfoFolder);
		pwndMainFrm->m_pviewConditionList->SetItme_User(bModify, stXMLTreeInfoFile);
	}
}

int CSaveDlg::SetMessageBox(LPCTSTR lpszText, LPCTSTR lpszCaption, UINT nType)
{
	CMainFrmWnd *pwndMainFrm = (CMainFrmWnd*)m_pwndMain;
	return pwndMainFrm->SetMessageBox(lpszText, lpszCaption, nType);
}

void CSaveDlg::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	CMainFrmWnd *pwndMainFrm = (CMainFrmWnd*)m_pwndMain;

	CRect rtMainWnd;
	GetClientRect(rtMainWnd);
	
	dc.FillSolidRect(rtMainWnd, pwndMainFrm->GetAxColor(66));	
}

HBRUSH CSaveDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	
	if(nCtlColor == CTLCOLOR_STATIC)
	{
		CMainFrmWnd* pMainFrmWnd = (CMainFrmWnd*)m_pwndMain;
		pDC->SetBkMode(TRANSPARENT);
		hbr = pMainFrmWnd->GetAxBrush(pMainFrmWnd->GetAxColor(66));
	}
	
	return hbr;
}

BOOL CSaveDlg::PreTranslateMessage(MSG* pMsg) 
{
	if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_RETURN)
		OnBtnSaveOk();
	
	return CDialog::PreTranslateMessage(pMsg);
}

