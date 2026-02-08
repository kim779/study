// DlgSaveScreen.cpp : implementation file
//

#include "stdafx.h"
#include "ChartCommonAddIn.h"
#include "DlgSaveScreen.h"

#include "../../Include_Chart/Conversion.h"			// for CDataConversion

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgSaveScreen dialog


CDlgSaveScreen::CDlgSaveScreen(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgSaveScreen::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgSaveScreen)
	m_nWidth = 500;
	m_nHeight = 350;
	m_strFileName = _T("temp");
	m_strFilePath = _T("");
	m_nFileType = 1;
	//}}AFX_DATA_INIT
}


void CDlgSaveScreen::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgSaveScreen)
	DDX_Text(pDX, IDC_EDIT1, m_nWidth);
	DDV_MinMaxInt(pDX, m_nWidth, 1, 1024);
	DDX_Text(pDX, IDC_EDIT2, m_nHeight);
	DDV_MinMaxInt(pDX, m_nHeight, 1, 768);
	DDX_Text(pDX, IDC_EDIT3, m_strFileName);
	DDX_Text(pDX, IDC_SAVEFILEPATH, m_strFilePath);
	DDX_CBIndex(pDX, IDC_FILETYPE, m_nFileType);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgSaveScreen, CDialog)
	//{{AFX_MSG_MAP(CDlgSaveScreen)
	ON_BN_CLICKED(IDC_FILEDIALOG, OnFiledialog)
	ON_WM_PAINT()
	ON_CBN_SELCHANGE(IDC_FILETYPE, OnSelchangeFiletype)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


//*********************************************************************************
// Function name		- GetFolder
// Description			- Get a folder path
// Дата модификации		- 25.09.2000
// Кем модифицирована	- S. Sokolenko
// In					-
//						  strSelectedFolder - reference to string for store folder path
// Out				-
//						  lpszTitle - title for caption
//						  hwndOwner - reference to parent window 
//						  strRootFolder - root folder 
//						  strStartFolder - current foldet
// Return				- TRUE if user select OK, else FALSE.
//*********************************************************************************
CString strTmpPath;

int CALLBACK BrowseCallbackProc(HWND hwnd, UINT uMsg, LPARAM lParam, LPARAM lpData)
{
	TCHAR szDir[MAX_PATH];
	switch(uMsg){
	case BFFM_INITIALIZED:
		if (lpData){
			strcpy(szDir, strTmpPath.GetBuffer(strTmpPath.GetLength()));
			SendMessage(hwnd,BFFM_SETSELECTION,TRUE,(LPARAM)szDir);
		}
		break;
	case BFFM_SELCHANGED: {
	   if (SHGetPathFromIDList((LPITEMIDLIST) lParam ,szDir)){
		  SendMessage(hwnd,BFFM_SETSTATUSTEXT,0,(LPARAM)szDir);
	   }
	   break;
	}
	default:
	   break;
	}
         
	return 0;
}

BOOL GetFolder(CString* strSelectedFolder,
				   const char* lpszTitle,
				   const HWND hwndOwner, 
				   const char* strRootFolder, 
				   const char* strStartFolder)
{
	char pszDisplayName[MAX_PATH];
	LPITEMIDLIST lpID;
	BROWSEINFOA bi;
	
	bi.hwndOwner = hwndOwner;
	if (strRootFolder == NULL){
		bi.pidlRoot = NULL;
	}else{
	   LPITEMIDLIST  pIdl = NULL;
	   IShellFolder* pDesktopFolder;
	   char          szPath[MAX_PATH];
	   OLECHAR       olePath[MAX_PATH];
	   ULONG         chEaten;
	   ULONG         dwAttributes;

	   strcpy(szPath, (LPCTSTR)strRootFolder);
	   if (SUCCEEDED(SHGetDesktopFolder(&pDesktopFolder)))
	   {
		   MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, szPath, -1, olePath, MAX_PATH);
		   pDesktopFolder->ParseDisplayName(NULL, NULL, olePath, &chEaten, &pIdl, &dwAttributes);
		   pDesktopFolder->Release();
	   }
	   bi.pidlRoot = pIdl;
	}
	bi.pszDisplayName = pszDisplayName;
	bi.lpszTitle = lpszTitle;
	bi.ulFlags = BIF_RETURNONLYFSDIRS | BIF_STATUSTEXT;
	bi.lpfn = BrowseCallbackProc;
	if (strStartFolder == NULL){
		bi.lParam = FALSE;
	}else{
		strTmpPath.Format("%s", strStartFolder);
		bi.lParam = TRUE;
	}
	bi.iImage = NULL;
	lpID = SHBrowseForFolderA(&bi);
	if (lpID != NULL){
		BOOL b = SHGetPathFromIDList(lpID, pszDisplayName);
		if (b == TRUE){
			strSelectedFolder->Format("%s",pszDisplayName);
			return TRUE;
		}
	}else{
		strSelectedFolder->Empty();
	}
	return FALSE;
}
/////////////////////////////////////////////////////////////////////////////
// CDlgSaveScreen message handlers

void CDlgSaveScreen::SetFilePath(const char *p_szFilePath)
{
	CString strFilePath( p_szFilePath);
	//sy 2003.10.17.
	if(strFilePath.Find(".") < 0){
		m_strFilePath = strFilePath;
		return;
	}

	CString strTemp = strFilePath;
	CString strRealPath;
	while(!strTemp.IsEmpty()){
		CString strTemp2 = CDataConversion::GetStringData(strTemp, "\\");
		if(strTemp2.IsEmpty())
			break;

		if(strRealPath.IsEmpty()){
			strRealPath = strTemp2;
		}
		else{
			strRealPath += ("\\" + strTemp2);
		}
	}

	m_strFilePath = strRealPath;

	CString strRealFileName = CDataConversion::GetStringData(strTemp, ".");
	if(!strRealFileName.IsEmpty()){
		m_strFileName = strRealFileName;
	}
	//end
}

void CDlgSaveScreen::OnFiledialog() 
{
	CString strFolderPath;
	if(GetFolder(&strFolderPath, "", this->m_hWnd, NULL, NULL)){
		if (!strFolderPath.IsEmpty()){
			m_strFilePath = strFolderPath;
			UpdateData(FALSE);
		}
	}
/*
	CFileDialog dlg(TRUE);
	dlg.m_ofn.lpstrInitialDir = m_strFilePath;
	if(dlg.DoModal() == IDOK) {
		m_strFilePath = dlg.GetPathName();
		m_strFileName = dlg.GetFileName();
	}
	UpdateData(FALSE);
*/
}

BOOL CDlgSaveScreen::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// Add "About..." menu item to system menu.
/*
	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		AfxSetResourceHandle( ML_LANGUAGE_RES);
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	// TODO: Add extra initialization here
*/	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgSaveScreen::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CDlgSaveScreen::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}


void CDlgSaveScreen::OnSysCommand(UINT nID, LPARAM lParam)
{
/*
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
*/
}

int CDlgSaveScreen::GetFileType()
{
	return m_nFileType;
}

void CDlgSaveScreen::OnSelchangeFiletype() 
{
	UpdateData(TRUE);
}
