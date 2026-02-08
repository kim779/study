// SaveDlg.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include "SaveDlg.h"

#include "MainFrmWnd.h"

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

CSaveDlg::CSaveDlg(CWnd* pParent)
:CDialog(CSaveDlg::IDD, pParent)
{
	m_strNotUse = "\\\/\:\*\?\"\<\>\|\,";
	m_pwndMain = pParent;
	m_strLoadedFileName = "";
	m_strLoadedFolderPath = "";
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
	
	LOGFONT	  logFont;
	GETFONT(logFont, 12, 400, m_fontDefault);
	m_brushBk.CreateSolidBrush(COLOR_DLG_CANVAS);

	HINSTANCE hInstSave = AfxGetResourceHandle();
	AfxSetResourceHandle(g_hInstance);
	
	m_treeXmlCtrl.Create(this, IDC_TREE_USER);
	
	m_ctrlTitleFile.Create(_T("조건명"), WS_CHILD | WS_VISIBLE | SS_CENTER, 
		CRect(0, 0, 0, 0), this);
	m_ctrlTitleFile.SetFont(&m_fontDefault);
	
	m_editFileName.Create(WS_CHILD | WS_VISIBLE | WS_TABSTOP,
		CRect(0, 0, 0, 0), this, IDC_EDIT_FILENAME);
	m_editFileName.ModifyStyleEx(0, WS_EX_CLIENTEDGE);
	m_editFileName.SetFont(&m_fontDefault);
	
	m_ctrlTitleFolder.Create(_T("그룹명"), WS_CHILD | WS_VISIBLE | SS_CENTER, 
		CRect(0, 0, 0, 0), this);
	m_ctrlTitleFolder.SetFont(&m_fontDefault);
	
	m_editFolderName.Create(WS_CHILD | WS_VISIBLE | WS_TABSTOP,
		CRect(0, 0, 0, 0), this, IDC_EDIT_FOLDERNAME);
	m_editFolderName.ModifyStyleEx(0, WS_EX_CLIENTEDGE);
	m_editFolderName.SetFont(&m_fontDefault);
	m_editFolderName.EnableWindow(FALSE);
		
	m_btnDel.Create(_T("삭제"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 
		CRect(0, 0, 0, 0), this, IDC_BTN_SAVEDEL);
	m_btnDel.SetFont(&m_fontDefault);
	m_btnDel.SetBtnColor((int)STYLE_SET);
	
	m_btnOK.Create(_T("저장"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 
		CRect(0, 0, 0, 0), this, IDC_BTN_SAVEOK);
	m_btnOK.SetFont(&m_fontDefault);
	m_btnOK.SetBtnColor((int)STYLE_SET);
	
	m_btnCancel.Create(_T("닫기"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 
		CRect(0, 0, 0, 0), this, IDC_BTN_SAVECANCEL);
	m_btnCancel.SetFont(&m_fontDefault);
	m_btnCancel.SetBtnColor((int)STYLE_SET);

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
	reTemp.right = reTemp.left + GetBtnWidth(2);
	m_btnDel.MoveWindow(reTemp);
	
	reTemp = reBottom;
	reTemp.left = reTemp.right - GetBtnWidth(2);
	m_btnCancel.MoveWindow(reTemp);
	
	reTemp.right = reTemp.left - 3;
	reTemp.left = reTemp.right - GetBtnWidth(2);
	m_btnOK.MoveWindow(reTemp);
	
	
	reBottom.bottom = reBottom.top - 4;
	reBottom.top = reBottom.bottom - 18;
	reTemp = reBottom;
	
	
	reTemp.top += 4;
	reTemp.right = reTemp.left + 40;
	m_ctrlTitleFile.MoveWindow(reTemp);
	
	reTemp.top -= 4;
	reTemp.left = reTemp.right + 5;
	reTemp.right = reClient.right;
	m_editFileName.MoveWindow(reTemp);
	
	
	reBottom.bottom = reBottom.top - 2;
	reBottom.top = reBottom.bottom - 18;
	reTemp = reBottom;
	
	reTemp.top += 4;
	reTemp.right = reTemp.left + 40;
	m_ctrlTitleFolder.MoveWindow(reTemp);
	
	reTemp.top -= 4;
	reTemp.left = reTemp.right + 5;
	reTemp.right = reClient.right;
	m_editFolderName.MoveWindow(reTemp);
	
	
	reBottom.bottom = reBottom.top - 3;
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
	m_treeXmlCtrl.SetInitTreeList(pwndMainFrm->GetUserData());
	m_treeXmlCtrl.SetSelectItem(_T("사용자조건"), "");
	
	m_editFolderName.SetWindowText(_T("사용자조건"));
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
//					hSelItem = m_treeXmlCtrl.GetSelectedItem();
//					if (hSelItem != NULL)
//						m_treeXmlCtrl.SetDeleteItem(hSelItem);

					hSelItem = m_treeXmlCtrl.GetSelectedItem();
					if (hSelItem != NULL)
					{
						HTREEITEM htChildNode = m_treeXmlCtrl.GetChildItem(hSelItem);
						while (htChildNode != NULL)
						{
							m_treeXmlCtrl.SetDeleteItem(htChildNode);
							htChildNode = m_treeXmlCtrl.GetNextItem(htChildNode, TVGN_NEXT);
						}
					}
					
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
	if (bState == TRUE)
		::SetCursor(AfxGetApp()->LoadStandardCursor(IDC_ARROW));	
	else
		::SetCursor(AfxGetApp()->LoadStandardCursor(IDC_WAIT));
}

BOOL CSaveDlg::SetDelete_File(CString strFilePath, CString strFolderPath)
{
	SetControlState(FALSE);

	CMainFrmWnd *pwndMainFrm = (CMainFrmWnd*)m_pwndMain;
	pwndMainFrm->DeleteUserData(strFilePath);

	SetControlState();

	return TRUE;
}

BOOL CSaveDlg::SetDelete_Folder(CString strFolderPath)
{
	SetControlState(FALSE);
	
	CMainFrmWnd *pwndMainFrm = (CMainFrmWnd*)m_pwndMain;
	pwndMainFrm->DeleteAllUserData();
	
	SetControlState();

	return TRUE;
}

void CSaveDlg::OnBtnSaveCancel()
{
	OnCancel();
/*	if (m_bUpDate == TRUE)
	{
		m_bUpDate = FALSE;
		OnOK();
	}
	else
	{
		OnCancel();
	}
*/
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
	else if(strFileName.GetLength() > 8)
	{
		SetMessageBox("패턴명이 긴 경우 지정한 이름대로 표시되지 않을 수 있습니다.");
	}
	else if (GetCheckFileName(strFileName) == FALSE)
	{
		m_editFileName.SetFocus();
		m_editFileName.SetSel(0, strFileName.GetLength());
		return;
	}
	
/*	if (GetHaveFolder(strFolderName) == FALSE)
	{
		SetMessageBox("저장그룹을 재선택하시오.", PROGRAM_TITLE, MB_OK);
		m_editFolderName.SetFocus();
		m_editFolderName.SetSel(0, strFileName.GetLength());
		return;
	}
*/	
	BOOL bSaveState = TRUE;
	if (GetCheckSameFile(strFolderName, strFileName) == FALSE)
	{
		if (SetSaveFile(strFolderName, strFileName) == FALSE)
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
			if (SetSaveFile(strFolderName, strFileName) == FALSE)
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
		m_strLoadedFileName = strFileName;
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

	CMainFrmWnd *pwndMainFrm = (CMainFrmWnd*)m_pwndMain;
	bResult = pwndMainFrm->GetFindFile(strFileName);

	return bResult;
}

BOOL CSaveDlg::GetHaveFolder(CString strFolderPath)
{
	BOOL bResult = FALSE;
	
/*	CString strMessage;
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
*/	
	return bResult;
}


void CSaveDlg::SetUpdate_FileData(BOOL bModify, CString strFolderName, CString strFileName)
{
/*	CMainFrmWnd *pwndMainFrm = (CMainFrmWnd*)m_pwndMain;
	
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
*/
}

int CSaveDlg::SetMessageBox(LPCTSTR lpszText, LPCTSTR lpszCaption, UINT nType)
{
	CMainFrmWnd *pwndMainFrm = (CMainFrmWnd*)m_pwndMain;
	return pwndMainFrm->SetMessageBox(lpszText, nType, lpszCaption);
}

void CSaveDlg::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	CRect rtMainWnd;
	GetClientRect(rtMainWnd);
	
	dc.FillSolidRect(rtMainWnd, COLOR_DLG_CANVAS);	
}

HBRUSH CSaveDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	
	if(nCtlColor == CTLCOLOR_STATIC)
	{
		pDC->SetBkMode(TRANSPARENT);
		hbr = m_brushBk;
	}
	
	return hbr;
}

BOOL CSaveDlg::PreTranslateMessage(MSG* pMsg) 
{
	if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_RETURN)
		OnBtnSaveOk();
	
	return CDialog::PreTranslateMessage(pMsg);
}

BOOL CSaveDlg::SetSaveFile(CString strFolderName, CString strFileName)
{
	BOOL bResult = TRUE;
	
/*	CMainFrmWnd *pwndMainFrm = (CMainFrmWnd*)m_pwndMain;
	CConditionMakeWnd *pwndConditionMake = (CConditionMakeWnd *)pwndMainFrm->GetWnd("MAKE");
	
	MSXML::IXMLDOMNodePtr					pNConditions;
	MSXML::IXMLDOMNamedNodeMapPtr  pNAConditions;
	
	MSXML::IXMLDOMNodePtr					pNCondition;
	MSXML::IXMLDOMNamedNodeMapPtr	pNACondition;
	
	MSXML::IXMLDOMNodePtr					pNFormat;
	MSXML::IXMLDOMNamedNodeMapPtr	pNAFormat;
	
	MSXML::IXMLDOMNodePtr					pNAttributeTemp;
	
	CoInitialize(NULL);
	MSXML::IXMLDOMDocumentPtr pDOMConditionIndex("Microsoft.XMLDOM", NULL, CLSCTX_INPROC_SERVER);
	
	try 
	{
		CString strTemp;
		
		MSXML::IXMLDOMProcessingInstructionPtr pPI;
		MSXML::IXMLDOMNamedNodeMapPtr			    pNAPI;
		MSXML::IXMLDOMNodePtr					        pNEncoding;
		
		pPI = pDOMConditionIndex->createProcessingInstruction("xml", "version=\"1.0\"");
		pDOMConditionIndex->appendChild(pPI);
		
		pNEncoding = pDOMConditionIndex->createAttribute("encoding");
		pNEncoding->text = "KSC_5601";
		
		pNAPI = pPI->attributes;
		pNAPI->setNamedItem(pNEncoding);
		
		
		pNConditions = pDOMConditionIndex->createElement("CONDITIONS");
		pDOMConditionIndex->appendChild(pNConditions);
		pNAConditions = pNConditions->attributes;
		
		pNAttributeTemp = pDOMConditionIndex->createAttribute("VERSION");
		pNAttributeTemp->text = _bstr_t((CString)XML_VERSION_USERFILE);
		pNAConditions->setNamedItem(pNAttributeTemp);
		
		
		pNAttributeTemp = pDOMConditionIndex->createAttribute("ALERT");
		pNAttributeTemp->text = _bstr_t((pwndConditionMake->m_btnAlertChk.GetCheck() == TRUE) ? "1" : "0"); 
		pNAConditions->setNamedItem(pNAttributeTemp);
		
		
		//##### 전체 조건 데이타 START #####
		int nDataCnt;
		//# 포트폴리오구성데이터 #
		PortPolio   stPort;
		nDataCnt = pwndConditionMake->m_ctrlPortEdit.m_listPort.GetCount();
		POSITION posPort;
		CString strPort = "";
		CString strPortOpt = "";
		CString strType = "";
		for (int nPortIndex = 0; nPortIndex < nDataCnt; nPortIndex++)
		{
			posPort = pwndConditionMake->m_ctrlPortEdit.m_listPort.FindIndex(nPortIndex);
			stPort = pwndConditionMake->m_ctrlPortEdit.m_listPort.GetAt(posPort);
			
			strPortOpt  = stPort.strOperation;
			strType = (stPort.nType == 1) ? "#":"";
			strPort += stPort.strPortNumber + strType + "_" + stPort.strPortName + "/";
		}
		
		pNAttributeTemp = pDOMConditionIndex->createAttribute("PORTFOLIO_DATA");
		pNAttributeTemp->text = _bstr_t(strPort); 
		pNAConditions->setNamedItem(pNAttributeTemp);
		
		//# 포트폴리오 연산자
		pNAttributeTemp = pDOMConditionIndex->createAttribute("PORTFOLIO_OPERATOR");
		pNAttributeTemp->text = _bstr_t(strPortOpt); 
		pNAConditions->setNamedItem(pNAttributeTemp);
		
		pNAttributeTemp = pDOMConditionIndex->createAttribute("PORTFOLIO_CHK");
		pNAttributeTemp->text = _bstr_t((pwndConditionMake->m_btnPortCheck.GetCheck() == TRUE) ? "1" : "0"); 
		pNAConditions->setNamedItem(pNAttributeTemp);
		
		pNAttributeTemp = pDOMConditionIndex->createAttribute("TARGETBASE_TYPE");
		strTemp.Format("%d", pwndConditionMake->m_nTargetBaseIndex);
		pNAttributeTemp->text = _bstr_t(strTemp); 
		pNAConditions->setNamedItem(pNAttributeTemp);
		
		pNAttributeTemp = pDOMConditionIndex->createAttribute("GROUP_CNT");
		strTemp.Format("%d", pwndConditionMake->m_DataAdmin.GetGroupCnt());
		pNAttributeTemp->text = _bstr_t(strTemp); 
		pNAConditions->setNamedItem(pNAttributeTemp);

		ConditionGroupInfo   stConditionGroup;
		OneElementInfo       stOneElement;
		GroupElementInfo     stGroupElement;
		int nCondSize = 0;
		POSITION posLogic;

		if (pwndConditionMake->m_DataAdmin.SetDataGroupSort() == FALSE)
			return FALSE;
		
		int nIndex_No = -1;

		nDataCnt = pwndConditionMake->m_DataAdmin.m_arrGroups.GetSize();
		for(int nIndex = 0; nIndex < nDataCnt; nIndex++)
		{
			GROUP_INFO* pGroup = pwndConditionMake->m_DataAdmin.m_arrGroups.GetAt(nIndex);
			nCondSize = pGroup->arrConGrpInfo.GetSize();

			for(int j = 0; j < nCondSize; ++j)
			{
				stConditionGroup = pGroup->arrConGrpInfo.GetAt(j);
				
				int nLogicCnt, nLogicIndex;
				BOOL bHaveElement = FALSE;
				if (stConditionGroup.nGroup == 0)
				{
					nLogicCnt = pwndConditionMake->m_ctrlCommonLogicEdit.GetElementCount();
					
					for (nLogicIndex = 0; nLogicIndex < nLogicCnt; nLogicIndex++)
					{
						posLogic = pwndConditionMake->m_ctrlCommonLogicEdit.m_listOneElement.FindIndex(nLogicIndex);
						stOneElement = pwndConditionMake->m_ctrlCommonLogicEdit.m_listOneElement.GetAt(posLogic);
						
						stOneElement.strElement.TrimLeft();
						stOneElement.strElement.TrimRight();
						
						if (stConditionGroup.strElement == stOneElement.strElement)
						{
							bHaveElement = TRUE;
							++nIndex_No;

							break;
						}
					}
				}
				else if (stConditionGroup.nGroup > 0)
				{
					GROUP_ELEMINFO* pElemGroup = pwndConditionMake->m_ctrlOrderLogicEdit.FindGroupInfoWithGroupNumber(stConditionGroup.nGroup);
					if(!pElemGroup)
						continue;

					nLogicCnt = pElemGroup->arrGroupElemInfo.GetSize();
					
					for (nLogicIndex = 0; nLogicIndex < nLogicCnt; ++nLogicIndex)
					{
						stGroupElement = pElemGroup->arrGroupElemInfo.GetAt(nLogicIndex);
						stGroupElement.stOneElementInfo.strElement.TrimLeft();
						stGroupElement.stOneElementInfo.strElement.TrimRight();
						
						if (stConditionGroup.strElement == stGroupElement.stOneElementInfo.strElement)
						{
							bHaveElement = TRUE;
							++nIndex_No;

							break;
						}
					}
				}

				if (bHaveElement == FALSE)
					return FALSE;

				//### User_Define Values Insert ###
				MSXML::IXMLDOMNodePtr pNAttributeTemp;	
				pNCondition = pDOMConditionIndex->createElement("CONDITION");
				pNConditions->appendChild( pNCondition );
				pNACondition = pNCondition->attributes;
				
				pNAttributeTemp = pDOMConditionIndex->createAttribute("CONDITION_FLAG");
				strTemp = (stConditionGroup.stOneConditionInfo.bFalg == TRUE)?"1":"0";
				pNAttributeTemp->text = _bstr_t(strTemp);			
				pNACondition->setNamedItem(pNAttributeTemp);
				
				pNAttributeTemp = pDOMConditionIndex->createAttribute("NAME");
				strTemp.Format("%s", stConditionGroup.stOneConditionInfo.strName);
				pNAttributeTemp->text = _bstr_t(strTemp);			
				pNACondition->setNamedItem(pNAttributeTemp);
				
				pNAttributeTemp = pDOMConditionIndex->createAttribute("TAGNAME");
				strTemp.Format("%s", stConditionGroup.stOneConditionInfo.strTag);
				pNAttributeTemp->text = _bstr_t(strTemp);			
				pNACondition->setNamedItem(pNAttributeTemp);
				
				pNAttributeTemp = pDOMConditionIndex->createAttribute("FILE_NAME");
				strTemp.Format("%s", stConditionGroup.stOneConditionInfo.strMapFile);
				pNAttributeTemp->text = _bstr_t(strTemp);			
				pNACondition->setNamedItem(pNAttributeTemp);
				
				pNAttributeTemp = pDOMConditionIndex->createAttribute("FILE_NAME");
				strTemp.Format("%s", stConditionGroup.stOneConditionInfo.strMapFile);
				pNAttributeTemp->text = _bstr_t(strTemp);			
				pNACondition->setNamedItem(pNAttributeTemp);
				
				pNAttributeTemp = pDOMConditionIndex->createAttribute("NON_ALERT");
				strTemp.Format("%d", stConditionGroup.stOneConditionInfo.nNonAlert);
				pNAttributeTemp->text = _bstr_t(strTemp);			
				pNACondition->setNamedItem(pNAttributeTemp);
				
				pNAttributeTemp = pDOMConditionIndex->createAttribute("CHART_VALSET");
				strTemp.Format("%d", stConditionGroup.stOneConditionInfo.nChartValSet);
				pNAttributeTemp->text = _bstr_t(strTemp);			
				pNACondition->setNamedItem(pNAttributeTemp);
				
				
				
				//## View CheckBox State Valuse Flag ##
				pNAttributeTemp = pDOMConditionIndex->createAttribute("FIELD_VAL");
				strTemp = (stConditionGroup.stOneConditionInfo.bFieldVal == TRUE) ? "TRUE" : "FALSE";
				pNAttributeTemp->text = _bstr_t(strTemp);			
				pNACondition->setNamedItem(pNAttributeTemp);
				
				//## View CheckBox State Flag ##
				pNAttributeTemp = pDOMConditionIndex->createAttribute("FIELD_STATE");
				strTemp = (stConditionGroup.stOneConditionInfo.bFieldState == TRUE) ? "TRUE" : "FALSE";
				pNAttributeTemp->text = _bstr_t(strTemp);			
				pNACondition->setNamedItem(pNAttributeTemp);
				
				pNAttributeTemp = pDOMConditionIndex->createAttribute("FIELD_ENABLE");
				strTemp = (stConditionGroup.stOneConditionInfo.bFieldEnable == TRUE) ? "TRUE" : "FALSE";
				pNAttributeTemp->text = _bstr_t(strTemp);			
				pNACondition->setNamedItem(pNAttributeTemp);
				
				//## Group Info ##
				pNAttributeTemp = pDOMConditionIndex->createAttribute("INDEX_NO");
				strTemp.Format("%d", nIndex_No);
				pNAttributeTemp->text = _bstr_t(strTemp);			
				pNACondition->setNamedItem(pNAttributeTemp);
				
				pNAttributeTemp = pDOMConditionIndex->createAttribute("GROUP_NO");
				if(0 !=stConditionGroup.nGroup)
					strTemp.Format("%d", nIndex+1);
				else
					strTemp.Format("%d", nIndex);
				pNAttributeTemp->text = _bstr_t(strTemp);			
				pNACondition->setNamedItem(pNAttributeTemp);
				
				pNAttributeTemp = pDOMConditionIndex->createAttribute("ORG_GROUP_NO");
				strTemp.Format("%d", stConditionGroup.nGroup);
				pNAttributeTemp->text = _bstr_t(strTemp);			
				pNACondition->setNamedItem(pNAttributeTemp);
				
				//## User_Define Struct to Insert ##
				pNAttributeTemp = pDOMConditionIndex->createAttribute("TARGETBASE_TYPE");
				strTemp.Format("%d", stConditionGroup.stOneConditionInfo.nTargetBaseType);
				pNAttributeTemp->text = _bstr_t(strTemp);			
				pNACondition->setNamedItem(pNAttributeTemp);
				
				pNAttributeTemp = pDOMConditionIndex->createAttribute("TARGETBASE");
				strTemp.Format("%d", stConditionGroup.stOneConditionInfo.stCondition.TargetBase);
				pNAttributeTemp->text = _bstr_t(strTemp);			
				pNACondition->setNamedItem(pNAttributeTemp);
				
				pNAttributeTemp = pDOMConditionIndex->createAttribute("TYPE");
				strTemp.Format("%d", stConditionGroup.stOneConditionInfo.stCondition.Type);
				pNAttributeTemp->text = _bstr_t(strTemp);			
				pNACondition->setNamedItem(pNAttributeTemp);
				
				pNAttributeTemp = pDOMConditionIndex->createAttribute("TYPE1");
				strTemp.Format("%d", stConditionGroup.stOneConditionInfo.stCondition.Type1);
				pNAttributeTemp->text = _bstr_t(strTemp);			
				pNACondition->setNamedItem(pNAttributeTemp);
				
				pNAttributeTemp = pDOMConditionIndex->createAttribute("TYPE2");
				strTemp.Format("%d", stConditionGroup.stOneConditionInfo.stCondition.Type2);
				pNAttributeTemp->text = _bstr_t(strTemp);			
				pNACondition->setNamedItem(pNAttributeTemp);
				
				
				pNAttributeTemp = pDOMConditionIndex->createAttribute("STERM");
				strTemp.Format("%d", stConditionGroup.stOneConditionInfo.stCondition.STerm);
				pNAttributeTemp->text = _bstr_t(strTemp);			
				pNACondition->setNamedItem(pNAttributeTemp);
				
				pNAttributeTemp = pDOMConditionIndex->createAttribute("ETERM");
				strTemp.Format("%d", stConditionGroup.stOneConditionInfo.stCondition.ETerm);
				pNAttributeTemp->text = _bstr_t(strTemp);			
				pNACondition->setNamedItem(pNAttributeTemp);
				
				
				pNAttributeTemp = pDOMConditionIndex->createAttribute("RANK");
				strTemp.Format("%d", stConditionGroup.stOneConditionInfo.stCondition.Rank);
				pNAttributeTemp->text = _bstr_t(strTemp);			
				pNACondition->setNamedItem(pNAttributeTemp);
				
				pNAttributeTemp = pDOMConditionIndex->createAttribute("TREND");
				strTemp.Format("%d", stConditionGroup.stOneConditionInfo.stCondition.Trend);
				pNAttributeTemp->text = _bstr_t(strTemp);			
				pNACondition->setNamedItem(pNAttributeTemp);
				
				pNAttributeTemp = pDOMConditionIndex->createAttribute("SIGNALCNT1");
				strTemp.Format("%d", stConditionGroup.stOneConditionInfo.stCondition.SignalCnt1);
				pNAttributeTemp->text = _bstr_t(strTemp);			
				pNACondition->setNamedItem(pNAttributeTemp);
				
				pNAttributeTemp = pDOMConditionIndex->createAttribute("SIGNALCNT2");
				strTemp.Format("%d", stConditionGroup.stOneConditionInfo.stCondition.SignalCnt2);
				pNAttributeTemp->text = _bstr_t(strTemp);			
				pNACondition->setNamedItem(pNAttributeTemp);
				
				pNAttributeTemp = pDOMConditionIndex->createAttribute("RESULTVIEWNUM");
				strTemp.Format("%d", stConditionGroup.stOneConditionInfo.stCondition.ResultViewNum);
				pNAttributeTemp->text = _bstr_t(strTemp);			
				pNACondition->setNamedItem(pNAttributeTemp);
				
				//### Move Fielder Data ###
				strTemp = "";
				CString strHeadID;
				int nFieldCnt = stConditionGroup.stOneConditionInfo.stHeadMoveInfo.nColCnt;
				for(int nFieldIndex = 0; nFieldIndex < nFieldCnt; nFieldIndex++)
				{
					strHeadID.Format("%d", stConditionGroup.stOneConditionInfo.stHeadMoveInfo.stHeadType[nFieldIndex].lHeadID);
					strTemp += strHeadID + "/";
				}
				pNAttributeTemp = pDOMConditionIndex->createAttribute("FIELD_ID");
				pNAttributeTemp->text = _bstr_t(strTemp);			
				pNACondition->setNamedItem(pNAttributeTemp);
				
				
				
				pNAttributeTemp = pDOMConditionIndex->createAttribute("INDEXTYPE");
				strTemp.Format("%c", stConditionGroup.stOneConditionInfo.stCondition.IndexType);
				pNAttributeTemp->text = _bstr_t(strTemp);			
				pNACondition->setNamedItem(pNAttributeTemp);
				
				
				pNAttributeTemp = pDOMConditionIndex->createAttribute("SEARCHTYPE");
				strTemp.Format("%c", stConditionGroup.stOneConditionInfo.stCondition.SearchType);
				pNAttributeTemp->text = _bstr_t(strTemp);			
				pNACondition->setNamedItem(pNAttributeTemp);
				
				
				pNAttributeTemp = pDOMConditionIndex->createAttribute("OPERATOR");
				if (stConditionGroup.nGroup == 0)
				{
					if ((stOneElement.strOperation == "and") || 
						(stOneElement.strOperation == "next"))
						strTemp = "1";
					else if (stOneElement.strOperation == "or")
						strTemp = "0";
				}
				else
				{
					if ((stGroupElement.stOneElementInfo.strOperation == "and") || 
						(stGroupElement.stOneElementInfo.strOperation == "next"))
						strTemp = "1";
					else if (stGroupElement.stOneElementInfo.strOperation == "or")
						strTemp = "0";
				}
				pNAttributeTemp->text = _bstr_t(strTemp);			
				pNACondition->setNamedItem(pNAttributeTemp);
				
				pNAttributeTemp = pDOMConditionIndex->createAttribute("NOT_USER");
				strTemp = (stConditionGroup.stOneConditionInfo.nNotUserFlag == 1) ? "1" : "0";
				pNAttributeTemp->text = _bstr_t(strTemp);			
				pNACondition->setNamedItem(pNAttributeTemp);
				
				pNAttributeTemp = pDOMConditionIndex->createAttribute("REVERSE");
				strTemp.Format("%c", stConditionGroup.stOneConditionInfo.stCondition.Reverse);
				pNAttributeTemp->text = _bstr_t(strTemp);			
				pNACondition->setNamedItem(pNAttributeTemp);
				
				pNAttributeTemp = pDOMConditionIndex->createAttribute("GUBUN_USER");
				strTemp = (stConditionGroup.stOneConditionInfo.nGubunUserFlag == 1) ? "1" : "0";
				pNAttributeTemp->text = _bstr_t(strTemp);			
				pNACondition->setNamedItem(pNAttributeTemp);
				
				pNAttributeTemp = pDOMConditionIndex->createAttribute("GUBUN");
				strTemp.Format("%c", stConditionGroup.stOneConditionInfo.stCondition.IdxContiGubun);
				pNAttributeTemp->text = _bstr_t(strTemp);			
				pNACondition->setNamedItem(pNAttributeTemp);
				
				
				//# 2005.04.07 => 지표 추가에 대한 Packet 변경
				pNAttributeTemp = pDOMConditionIndex->createAttribute("CODE");
				
				char chCode[7] = {0,};
				memcpy(chCode, stConditionGroup.stOneConditionInfo.stCondition.Code, 
					sizeof(stConditionGroup.stOneConditionInfo.stCondition.Code));
				pNAttributeTemp->text = _bstr_t(chCode);
				pNACondition->setNamedItem(pNAttributeTemp);
				
				
				int nValIndex;
				for(nValIndex = 0; nValIndex < MAX_PARAM; nValIndex++)
				{
					CString arr_temp;
					arr_temp.Format("P%d", nValIndex + 1);
					
					pNAttributeTemp = pDOMConditionIndex->createAttribute(_bstr_t(arr_temp));
					strTemp.Format("%f", stConditionGroup.stOneConditionInfo.stCondition.Param[nValIndex]);
					pNAttributeTemp->text = _bstr_t(strTemp);
					pNACondition->setNamedItem(pNAttributeTemp);
				}
				
				for (nValIndex = 0; nValIndex < MAX_CONDI; nValIndex++)
				{
					CString arr_temp;
					arr_temp.Format("C%d", nValIndex + 1);
					
					pNAttributeTemp = pDOMConditionIndex->createAttribute(_bstr_t(arr_temp));
					strTemp.Format("%f", stConditionGroup.stOneConditionInfo.stCondition.Condition[nValIndex]);
					pNAttributeTemp->text = _bstr_t(strTemp);
					pNACondition->setNamedItem(pNAttributeTemp);
				}
				
				//## Condition Text ##
				pNFormat = pDOMConditionIndex->createElement("FORMAT");
				pNCondition->appendChild( pNFormat );
				pNAFormat = pNFormat->attributes;
				
				
				//### Logic Edit Info ###
				if (stConditionGroup.nGroup == 0)
				{
					pNAttributeTemp = pDOMConditionIndex->createAttribute("LOGIC_ELEMENT");
					pNAttributeTemp->text = _bstr_t(stOneElement.strElement);			
//					pNAttributeTemp->text = _bstr_t(CONDITION_INDEX[nIndex]);
					pNACondition->setNamedItem(pNAttributeTemp);
					
					pNAttributeTemp = pDOMConditionIndex->createAttribute("LOGIC_NOT");
					pNAttributeTemp->text = _bstr_t(stOneElement.strNot);			
					pNACondition->setNamedItem(pNAttributeTemp);
					
					pNAttributeTemp = pDOMConditionIndex->createAttribute("LOGIC_OPERATION");
					pNAttributeTemp->text = _bstr_t(stOneElement.strOperation);			
					pNACondition->setNamedItem(pNAttributeTemp);
					
					pNAttributeTemp = pDOMConditionIndex->createAttribute("LOGIC_VIEW");
					strTemp = (stOneElement.bView == TRUE)?"TRUE":"FALSE";
					pNAttributeTemp->text = _bstr_t(strTemp);			
					pNACondition->setNamedItem(pNAttributeTemp);
					
					pNAttributeTemp = pDOMConditionIndex->createAttribute("LOGIC_BRACKET");
					pNAttributeTemp->text = _bstr_t(stOneElement.strBracket);			
					pNACondition->setNamedItem(pNAttributeTemp);
					
					pNAttributeTemp = pDOMConditionIndex->createAttribute("LOGIC_BRACKETCNT");
					strTemp.Format("%d", stOneElement.lBracketCount);
					pNAttributeTemp->text = _bstr_t(strTemp);			
					pNACondition->setNamedItem(pNAttributeTemp);
				}
				else
				{
					pNAttributeTemp = pDOMConditionIndex->createAttribute("LOGIC_ELEMENT");
					pNAttributeTemp->text = _bstr_t(stGroupElement.stOneElementInfo.strElement);
//					pNAttributeTemp->text = _bstr_t(CONDITION_INDEX[nIndex]);
					pNACondition->setNamedItem(pNAttributeTemp);
					
					pNAttributeTemp = pDOMConditionIndex->createAttribute("LOGIC_NOT");
					pNAttributeTemp->text = _bstr_t(stGroupElement.stOneElementInfo.strNot);			
					pNACondition->setNamedItem(pNAttributeTemp);
					
					pNAttributeTemp = pDOMConditionIndex->createAttribute("LOGIC_OPERATION");
					pNAttributeTemp->text = _bstr_t(stGroupElement.stOneElementInfo.strOperation);			
					pNACondition->setNamedItem(pNAttributeTemp);
					
					pNAttributeTemp = pDOMConditionIndex->createAttribute("LOGIC_VIEW");
					strTemp = (stGroupElement.stOneElementInfo.bView == TRUE)?"TRUE":"FALSE";
					pNAttributeTemp->text = _bstr_t(strTemp);			
					pNACondition->setNamedItem(pNAttributeTemp);
					
					pNAttributeTemp = pDOMConditionIndex->createAttribute("LOGIC_BRACKET");
					pNAttributeTemp->text = _bstr_t(stGroupElement.stOneElementInfo.strBracket);			
					pNACondition->setNamedItem(pNAttributeTemp);
					
					pNAttributeTemp = pDOMConditionIndex->createAttribute("LOGIC_BRACKETCNT");
					strTemp.Format("%d", stGroupElement.stOneElementInfo.lBracketCount);
					pNAttributeTemp->text = _bstr_t(strTemp);			
					pNACondition->setNamedItem(pNAttributeTemp);
				}
				
				strTemp = stConditionGroup.stOneConditionInfo.strText;
				pNFormat->text = _bstr_t(strTemp);
			}
		}

		//input User_define struct -> List Insert
		CString strFileFullRoot;
		if (strFolderName.IsEmpty() == TRUE)
			strFileFullRoot	= pwndMainFrm->GetFolderPath("SYS") + "\\" + strFileName + ".xml";
		else
			strFileFullRoot = pwndMainFrm->GetFolderPath("USER") + "\\" + strFolderName + "\\" + strFileName + ".xml";
		pDOMConditionIndex->save(_bstr_t(strFileFullRoot));
	}
	catch(_com_error &e)
	{
		bResult = FALSE;
	}
*/
	return bResult;
}