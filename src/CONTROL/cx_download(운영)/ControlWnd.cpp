// ControlWnd.cpp : implementation file
//

#include "stdafx.h"
#include "CX_Download.h"
#include "ControlWnd.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CControlWnd

CControlWnd::CControlWnd(_param* pParam, CWnd* pWizard)
{
	EnableAutomation();	

	m_pWizard = pWizard;

	m_clrBack = pParam->pRGB;
	m_clrText = pParam->tRGB;
	m_sParamKey = pParam->key;
	m_sParamName = CString(pParam->name,pParam->name.GetLength());

	m_pFont = GetAxFont(pParam->fonts, pParam->point, pParam->style);
	
	m_bDownload = FALSE;			
	m_strOption = pParam->options;
	m_strFilter = "PDF Files (*.pdf)|*.pdf|";
}

CControlWnd::~CControlWnd()
{
}

void CControlWnd::OnFinalRelease()
{
	// When the last reference for an automation object is released
	// OnFinalRelease is called.  The base class will automatically
	// deletes the object.  Add additional cleanup required for your
	// object before calling the base class.

	CWnd::OnFinalRelease();
}


BEGIN_MESSAGE_MAP(CControlWnd, CWnd)
	//{{AFX_MSG_MAP(CControlWnd)
	ON_WM_CREATE()
	ON_WM_PAINT()
	ON_WM_TIMER()
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_BTN_DOWNLOAD,OnClickDownload)
	ON_BN_CLICKED(IDC_BTN_FOLDER,OnClickFolder)
	ON_BN_CLICKED(IDC_BTN_EXCUTE,OnClickExcute)
	ON_BN_CLICKED(IDC_BTN_CLOSE,OnClickClose)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


BEGIN_DISPATCH_MAP(CControlWnd, CWnd)
	//{{AFX_DISPATCH_MAP(CControlWnd)
	DISP_PROPERTY_NOTIFY(CControlWnd, "address", m_address, OnAddressChanged, VT_BSTR)
	DISP_PROPERTY_NOTIFY(CControlWnd, "protocol", m_protocol, OnProtocolChanged, VT_BSTR)
	DISP_PROPERTY_NOTIFY(CControlWnd, "username", m_username, OnUsernameChanged, VT_BSTR)
	DISP_PROPERTY_NOTIFY(CControlWnd, "pw", m_pw, OnPwChanged, VT_BSTR)
	DISP_PROPERTY_NOTIFY(CControlWnd, "filename", m_filename, OnFilenameChanged, VT_BSTR)
	DISP_PROPERTY_NOTIFY(CControlWnd, "directory", m_directory, OnDirectoryChanged, VT_BSTR)
	DISP_FUNCTION(CControlWnd, "SetProperties", SetProperties, VT_EMPTY, VTS_BSTR)
	DISP_FUNCTION(CControlWnd, "GetProperties", GetProperties, VT_BSTR, VTS_NONE)
	DISP_FUNCTION(CControlWnd, "IsDownload", IsDownload, VT_BSTR, VTS_NONE)
	DISP_FUNCTION(CControlWnd, "DoDownload", DoDownload, VT_EMPTY, VTS_NONE)
	DISP_FUNCTION(CControlWnd, "CheckURLEnable", CheckURLEnable, VT_I4, VTS_BSTR)
	//}}AFX_DISPATCH_MAP
END_DISPATCH_MAP()

// Note: we add support for IID_IControlWnd to support typesafe binding
//  from VBA.  This IID must match the GUID that is attached to the 
//  dispinterface in the .ODL file.

// {BAFA4894-B12F-411D-8106-C037C5C9C8D7}
static const IID IID_IControlWnd =
{ 0xbafa4894, 0xb12f, 0x411d, { 0x81, 0x6, 0xc0, 0x37, 0xc5, 0xc9, 0xc8, 0xd7 } };

BEGIN_INTERFACE_MAP(CControlWnd, CWnd)
	INTERFACE_PART(CControlWnd, IID_IControlWnd, Dispatch)
END_INTERFACE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CControlWnd message handlers

void CControlWnd::SetProperties(LPCTSTR str) 
{
	// TODO: Add your dispatch handler code here

}

BSTR CControlWnd::GetProperties() 
{
	CString strResult;
	// TODO: Add your dispatch handler code here

	return strResult.AllocSysString();
}

CFont* CControlWnd::GetAxFont(CString fName, int point, int style)
{
	struct	_fontR	fontR {};

	fontR.name   = (char *)fName.GetString();
	fontR.point  = point;
	fontR.italic = false;
	fontR.bold   = 0;

	switch(style)
	{
	case 0: // none
	case 1: // none
		break;
	case 2: // italic
		fontR.italic = true;
		break;
	case 3: // bold
		fontR.bold = FW_BOLD;
		break;
	case 4: // both
		fontR.italic = true;
		fontR.bold = FW_BOLD;
		break;	
	}
	return (CFont*)m_pWizard->SendMessage(WM_USER, getFONT, (long)&fontR);
}

CString CControlWnd::GetAxVariant(int cmd, CString data)
{
	CString	strResult;

	if (m_pWizard)
	{
		const	char* dta = (char*)m_pWizard->SendMessage(WM_USER,
				MAKEWPARAM(variantDLL, cmd), (LPARAM)data.GetString());

		if ((long)dta > 1)
			strResult = dta;
	}

	return strResult;
}

int CControlWnd::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// TODO: Add your specialized creation code here	

	if (m_strOption.Find("DONOTNEEDUI") >= 0)
	{
		ShowWindow(SW_HIDE);
		return 0;
	}

	m_clrBack = m_pWizard->SendMessage(WM_USER, MAKEWPARAM(getPALETTE, 0), (LPARAM)m_clrBack);
	m_clrText = m_pWizard->SendMessage(WM_USER, MAKEWPARAM(getPALETTE, 0), (LPARAM)m_clrText);

	m_brushBack.CreateSolidBrush(m_clrBack);

	CRect	rc;
	CString szn, szu, szd;
	CString strRoot = GetAxVariant(homeCC);

	szn = strRoot + "\\image\\11BTN.BMP";
	szu = strRoot + "\\image\\11BTN_DN.BMP";
	szd = strRoot + "\\image\\11BTN_EN.BMP";

	GetClientRect(rc);	

	m_btnFolder.Create("폴더변경", WS_CHILD|WS_VISIBLE, 
		CRect(CPoint(rc.Width()/2-80*2-5-5, rc.bottom - 30), CSize(80, 20)), this, IDC_BTN_FOLDER);
	m_btnFolder.SetSkin(szn,szd,szu);	
	m_btnFolder.SetFont(m_pFont);

	m_btnDownload.Create("다운로드", WS_CHILD|WS_VISIBLE, 
		CRect(CPoint(rc.Width()/2-80-5, rc.bottom - 30), CSize(80, 20)), this, IDC_BTN_DOWNLOAD);
	m_btnDownload.SetSkin(szn,szd,szu);	
	m_btnDownload.SetFont(m_pFont);

	m_btnExcute.Create("파일실행", WS_CHILD|WS_VISIBLE, 
		CRect(CPoint(rc.Width()/2+5, rc.bottom - 30),CSize(80,20)), this, IDC_BTN_EXCUTE);
	m_btnExcute.SetSkin(szn,szd,szu);	
	m_btnExcute.SetFont(m_pFont);
	m_btnExcute.EnableWindow(FALSE);

	m_btnClose.Create("닫기", WS_CHILD|WS_VISIBLE,
		CRect(CPoint(rc.Width()/2+80+5+5, rc.bottom - 30),CSize(80,20)), this, IDC_BTN_CLOSE);
	m_btnClose.SetSkin(szn,szd,szu);	
	m_btnClose.SetFont(m_pFont);
	m_btnClose.EnableWindow(FALSE);

	m_ctrlProg.Create(WS_CHILD|WS_VISIBLE, 
		CRect(CPoint(rc.left+10, rc.top + 60), CSize(rc.Width()-20,20)), this, IDC_CTRL_PROGRESS);
	m_ctrlProg.SetRange(0, 100);

	m_ctrlMsg.Create("", WS_CHILD|WS_VISIBLE,
		CRect(CPoint(rc.left+10, rc.top + 90), CSize(rc.Width()-20,20)), this, IDC_STATIC_MSG);
	m_ctrlMsg.SetFont(m_pFont);

	m_ctrlName.Create("파일 이름 :", WS_CHILD|WS_VISIBLE,
		CRect(CPoint(rc.left+10,rc.top+10),CSize(70,20)), this, IDC_STATIC_NAME);
	m_ctrlName.SetFont(m_pFont);

	CRect	rcName;
	m_ctrlName.GetWindowRect(rcName);
	ScreenToClient(rcName);

	m_ctrlFileName.Create("", WS_CHILD|WS_VISIBLE,
		CRect(CPoint(rcName.right+5,rcName.top),CSize(rc.Width()-20-70,20)), this, IDC_STATIC_FILENAME);
	m_ctrlFileName.SetFont(m_pFont);

	m_ctrlFolder.Create("저장 위치 :", WS_CHILD|WS_VISIBLE, 
		CRect(CPoint(rcName.left,rcName.bottom+5), CSize(70,20)), this, IDC_STATIC_FOLDER);
	m_ctrlFolder.SetFont(m_pFont); 

	CRect	rcFolder;
	m_ctrlFolder.GetWindowRect(rcFolder);
	ScreenToClient(rcFolder);

	m_ctrlEdit.Create(WS_CHILD|WS_VISIBLE, 
		CRect(CPoint(rcFolder.right+5,rcFolder.top),CSize(rc.Width()-20-70,20)), this, IDC_EDIT_FOLDER);
	m_ctrlEdit.SetFont(m_pFont);

	char	szPath[MAX_PATH];
	if (SUCCEEDED(SHGetFolderPath(NULL,CSIDL_PERSONAL,NULL,0,szPath)))
	{
		CString strPath = szPath;
		strPath += "\\";
		m_ctrlEdit.SetWindowText(strPath);
	}

	return 0;
}

CString	CControlWnd::GetFolderPath(CString strFilePath, CString& strFileName)
{
	const	int	iFind = strFilePath.ReverseFind('\\');
	const	int	iLen = strFilePath.GetLength();
	
	strFileName = strFilePath.Right(iLen-iFind-1);

	return strFilePath.Left(iFind+1);
}

void CControlWnd::SetMsg(CString str)
{
	m_ctrlMsg.SetWindowText(str);
	m_ctrlMsg.Invalidate();
	//InvalidateRect(m_rcMsg, TRUE);
}

void CControlWnd::OnClickDownload()
{
	m_ctrlProg.SetPos(0);	
	DoIt();	
}

void CControlWnd::OnClickExcute()
{
	CString strPath;
	CString strName;

	m_ctrlEdit.GetWindowText(strPath);
	m_ctrlFileName.GetWindowText(strName);

	strPath += strName;

	ShellExecute(NULL, NULL, strPath, NULL, NULL, SW_SHOW);
}

void CControlWnd::OnClickClose()
{
	m_pWizard->PostMessage(WM_USER,closeDLL);
	DestroyWindow();
}

void CControlWnd::OnClickFolder()
{
	m_ctrlProg.SetPos(0);
	SetMsg("다운로드 버튼을 클릭해주세요");

	CFileDialog fileDlg(FALSE,".pdf",m_filename,OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT,m_strFilter);
	if (fileDlg.DoModal() == IDOK)
	{
		CString strFilePath = fileDlg.GetPathName();		
		CString strFileName;
		CString strFolderName = GetFolderPath(strFilePath, strFileName);
		
		m_ctrlEdit.SetWindowText(strFolderName);
		m_ctrlFileName.SetWindowText(strFileName);

		m_btnExcute.EnableWindow(FALSE);
	}
}

void CControlWnd::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	CRect	rc;

	GetClientRect(rc);
	
	CBrush brush(m_clrBack);
	dc.FillRect(rc, &brush);	
}

BOOL CControlWnd::CheckDownloadCondition()
{
	if (m_address == "")
	{
		SetMsg("다운 주소를 알 수 없습니다.");						
		return FALSE;
	}

	if (m_filename == "")
	{
		SetMsg("다운받을 파일을 알 수 없습니다.");						
		return FALSE;
	}

	if (m_protocol == "HTTP")
	{		
	}
	else if (m_protocol == "FTP")
	{		
		if (m_username == "" || m_pw == "")
		{
			SetMsg("ID 또는 패스워드를 알수 없습니다.");						
			return FALSE;
		}		
	}
	else
	{		
		SetMsg("알수 없는 프로토콜 입니다. (" + m_protocol + ")" );			
		return FALSE;
	}

	return TRUE;
}

BOOL CControlWnd::DoHttp(CInternetSession1* pSess)
{
	try
	{
		CString strURL;
		strURL = m_address + m_filename;

		/*CHttpConnection* pHttp = pSess->GetHttpConnection(m_address);
		CHttpFile* pFile = pHttp->OpenRequest(HTTP_VERB_GET, m_filename);*/

		CHttpFile* pFile = nullptr;
		pFile = (CHttpFile*) pSess->OpenURL(strURL, CONTEXT_IBKS, 
			INTERNET_FLAG_TRANSFER_BINARY|INTERNET_FLAG_DONT_CACHE|INTERNET_FLAG_RELOAD);	
		//INTERNET_FLAG_EXISTING_CONNECT

		DWORD	dwQueryInfo = 0;
		if (pFile && pFile->QueryInfoStatusCode(dwQueryInfo))
		{
			if (dwQueryInfo >= 400 && dwQueryInfo <= 499)
			{
				SetMsg("서버에 해당 파일이 없습니다.");
				pFile->Close();
				pSess->Close();
				return FALSE;
			}
			if (dwQueryInfo >= 500 && dwQueryInfo <= 599)
			{
				SetMsg("서버에 기술적인 문제가 있습니다.");
				pFile->Close();
				pSess->Close();
				return FALSE;
			}
		}

		if (pFile)
		{
			CFile	file;
			CString strPath, strFileName;

			m_ctrlEdit.GetWindowText(strPath);
			m_ctrlFileName.GetWindowText(strFileName);
			strPath = strPath + strFileName;

			if (!file.Open(strPath, CFile::modeCreate|CFile::modeWrite | CFile::shareDenyNone))
			{
				SetMsg(" 파일을 만들지 못했습니다.");
				pFile->Close();
				pSess->Close();			
				return FALSE;
			}

			SetMsg(strPath+" 다운로드하고 있습니다.");
	
			CString strLength;
			if (!pFile->QueryInfo(HTTP_QUERY_CONTENT_LENGTH, strLength))
			{
				pSess->Close();
				file.Close();
				pFile->Close();				
				SetMsg("파일 길이를 가져올 수 없습니다.");
				return FALSE;
			}
		
			DWORD	dwLen = atoi(strLength);
			DWORD	dwRead = -1;	
			int	percent = 0;
			const	DWORD	dwTotal = dwLen;
			char	buf[1024 * 4]{};

			while (dwRead > 0)
			{
				dwRead = pFile->Read(buf, 1024*4);
				file.Write(buf, dwRead);

				if (dwRead > dwLen)			
					dwLen = 0;
				else
					dwLen -= dwRead;

				percent =(int)(((float)(dwTotal-dwLen)/dwTotal)*100);
				m_ctrlProg.SetPos(percent);
			}
			file.Close();
			pSess->Close();		

			SetMsg(strPath+" 다운로드를 완료 했습니다.");
			return TRUE;
		}	
		else
		{
			SetMsg("파일을 열지 못했습니다.");
			return FALSE;
		} 
	}
	catch(CInternetException* e)
	{
		e->ReportError();
	}
	return FALSE;
}

BOOL CControlWnd::DoFtp(CInternetSession1* pSess)
{
	try
	{
		CInternetFile* pFile = nullptr;
		CFtpConnection* pFtp = pSess->GetFtpConnection(m_address, m_username, m_pw);

		if (pFtp == NULL)
		{
			SetMsg("FTP 서버에 연결 하지 못했습니다.");
			pSess->Close();
			return FALSE;
		}

		CString strCur;
		pFtp->GetCurrentDirectory(strCur);
		if (!pFtp->SetCurrentDirectory(m_directory))
		{
			SetMsg("디렉토리를 찾지 못했습니다. (" + m_directory + ")");
			pFtp->Close();
			pSess->Close();
			return FALSE;
		}

		CFtpFileFind fileFind(pFtp);
		CString strFtpPath = m_directory+m_filename;

		if (strCur.GetAt(strCur.GetLength()-1) != '/')	
			strCur += "/";	

		strFtpPath = strCur + strFtpPath;	

		if (!fileFind.FindFile(strFtpPath))
		{
			pFtp->Close();
			pSess->Close();
			SetMsg("서버에서 해당 파일을 찾지 못했습니다.");
		}
		fileFind.FindNextFile();

		DWORD	dwLen = (DWORD)fileFind.GetLength();	
		pFile = pFtp->OpenFile(m_filename);
		if (pFile == nullptr)
		{
			SetMsg("파일을 열지 못했습니다.");
			pFtp->Close();
			pSess->Close(); 
			return FALSE;
		}	

		CString strPath, strFileName;

		m_ctrlEdit.GetWindowText(strPath);
		m_ctrlFileName.GetWindowText(strFileName);
		strPath = strPath + strFileName;

		CFile	file;
		if (!file.Open(strPath, CFile::modeCreate|CFile::modeWrite | CFile::shareDenyNone))
		{
			SetMsg(" 파일을 만들지 못했습니다.");
			pFile->Close();
			pFtp->Close();
			pSess->Close();
		
			return FALSE;
		}

		SetMsg(strPath+" 다운로드하고 있습니다.");

		DWORD	dwRead = 0;	
		const	DWORD	dwTotal = dwLen;
		char	buf[1024 * 4]{};
		int	percent = 0;

		while(dwLen > 0)
		{
			dwRead = pFile->Read(buf, 1024*4);
			file.Write(buf, dwRead);

			if (dwRead > dwLen)			
				dwLen = 0;
			else
				dwLen -= dwRead;

			percent = (int)(((float)(dwTotal-dwLen)/dwTotal)*100);
			m_ctrlProg.SetPos(percent);
		}
	
		file.Close();
		pFile->Close();
		pFtp->Close();
		pSess->Close();		

		SetMsg(strPath+" 다운로드를 완료 했습니다.");
	}
	catch (CInternetException* e)
	{
		e->ReportError();
	}

	return TRUE;
}

BOOL CControlWnd::DoIt()
{	
	if (!CheckDownloadCondition())
		return FALSE; 

	m_btnExcute.EnableWindow(FALSE);
	m_btnDownload.EnableWindow(FALSE);
	m_btnClose.EnableWindow(FALSE);

	CInternetSession1 sess(this->GetSafeHwnd());	

	if (m_protocol == "HTTP")
		m_bDownload = DoHttp(&sess);
	else if (m_protocol == "FTP")
		m_bDownload = DoFtp(&sess);
	
	if (m_bDownload)
		m_btnExcute.EnableWindow(TRUE);
	m_btnDownload.EnableWindow(TRUE);
	m_btnClose.EnableWindow(TRUE);

	m_pWizard->SendMessage(WM_USER, MAKEWPARAM(eventDLL, MAKEWORD(m_sParamKey, evOnDblClk)),(long)(LPCTSTR)m_sParamName);

	return TRUE;
}

void CControlWnd::OnAddressChanged() 
{
	// TODO: Add notification handler code

}

void CControlWnd::OnProtocolChanged() 
{
	// TODO: Add notification handler code

}

void CControlWnd::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	if (nIDEvent == ID_DOWNLOAD_TIMER)
	{
		KillTimer(ID_DOWNLOAD_TIMER);
		DoIt();
	}
	
	CWnd::OnTimer(nIDEvent);
}

void CControlWnd::OnUsernameChanged() 
{
	// TODO: Add notification handler code

}

void CControlWnd::OnPwChanged() 
{
	// TODO: Add notification handler code

}

void CControlWnd::OnFilenameChanged() 
{
	// TODO: Add notification handler code
	m_ctrlFileName.SetWindowText(m_filename);	
}

void CControlWnd::OnDestroy() 
{
	CWnd::OnDestroy();
	
	// TODO: Add your message handler code here

	
}

BSTR CControlWnd::IsDownload() 
{
	CString strResult;
	// TODO: Add your dispatch handler code here

	strResult.Format("%d",m_bDownload);

	return strResult.AllocSysString();
}

void CControlWnd::OnDirectoryChanged() 
{
	// TODO: Add notification handler code

}

HBRUSH CControlWnd::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CWnd::OnCtlColor(pDC, pWnd, nCtlColor);
	
	// TODO: Change any attributes of the DC here
	const	UINT iID = pWnd->GetDlgCtrlID();
	if (iID == IDC_STATIC_NAME		||
		iID == IDC_STATIC_FILENAME	|| 
		iID == IDC_STATIC_FOLDER	|| 
		iID == IDC_STATIC_MSG		||
		iID == IDC_EDIT_FOLDER)
	{
		pDC->SetBkMode(TRANSPARENT);
		//pDC->SetTextCol
		
		return (HBRUSH)m_brushBack;
	}

	
	// TODO: Return a different brush if the default is not desired
	return hbr;
}

void CControlWnd::DoDownload() 
{
	// TODO: Add your dispatch handler code here
	m_ctrlProg.SetPos(0);

	CFileDialog fileDlg(FALSE,".pdf",m_filename,OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT,m_strFilter);
	if (fileDlg.DoModal() == IDOK)
	{
		CString strFilePath = fileDlg.GetPathName();		

		SetMsg(strFilePath + " 다운로드를 시작 합니다.");	

		CString strFileName;
		CString strFolderName = GetFolderPath(strFilePath, strFileName);
		
		m_ctrlEdit.SetWindowText(strFolderName);
		m_ctrlFileName.SetWindowText(strFileName);

		SetTimer(ID_DOWNLOAD_TIMER, 1000, NULL);
	}
	else
	{
		SetMsg("다운받을 경로가 선택되지 않았습니다.");		
	}	
}

long CControlWnd::CheckURLEnable(LPCTSTR strUrl) 
{
	// TODO: Add your dispatch handler code here
	try
	{
		CInternetSession1 sess(this->GetSafeHwnd());	
		CHttpFile* pFile = nullptr;

		pFile = (CHttpFile*) sess.OpenURL(strUrl, CONTEXT_IBKS, 
			INTERNET_FLAG_TRANSFER_BINARY|INTERNET_FLAG_DONT_CACHE|INTERNET_FLAG_RELOAD);	
		//INTERNET_FLAG_EXISTING_CONNECT

		if (pFile == nullptr)
		{
			return -1;
		}

		DWORD dwQueryInfo;
		if (pFile->QueryInfoStatusCode(dwQueryInfo))
		{
			return dwQueryInfo;	
		}

		pFile->Close();
		sess.Close();

	}
	catch (CInternetException* e)
	{
		//e->ReportError();
	}
	
	return -1;
}
