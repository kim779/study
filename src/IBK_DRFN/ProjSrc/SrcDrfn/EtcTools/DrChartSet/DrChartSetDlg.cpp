// DrChartSetDlg.cpp : implementation file
//

#include "stdafx.h"
#include "DrChartSet.h"
#include "DrChartSetDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDrChartSetDlg dialog

CDrChartSetDlg::CDrChartSetDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CDrChartSetDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDrChartSetDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CDrChartSetDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDrChartSetDlg)
	DDX_Control(pDX, IDC_RESULTLIST, m_ResultList);
	DDX_Control(pDX, IDC_FILELIST, m_FileList);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CDrChartSetDlg, CDialog)
	//{{AFX_MSG_MAP(CDrChartSetDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BTNDIRLIST, OnBtndirlist)
	ON_BN_CLICKED(IDC_BTNCHANGE, OnBtnchange)
	ON_EN_CHANGE(IDC_EDITDIR, OnChangeEditdir)
	ON_BN_CLICKED(IDC_BTNCHARTSETLUP, OnBtnchartsetlup)
	ON_BN_CLICKED(IDC_BTNCHARTSETLUP2, OnBtnchartsetlup2)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDrChartSetDlg message handlers

BOOL CDrChartSetDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
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
	CString szStartDir = theApp.GetProfileString("DATA", "StartDir", "");
	SetDlgItemText(IDC_EDITDIR, (LPCSTR)szStartDir);
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CDrChartSetDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CDrChartSetDlg::OnPaint() 
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
HCURSOR CDrChartSetDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CDrChartSetDlg::OnBtndirlist() 
{
	m_FileList.ResetContent();
	m_ResultList.ResetContent();

	CString rString;
	if(_DoPreDirectoryCheck(rString)==FALSE)
		return;

	CStringList rList;
	if(_GetFileList(rString, rList)==0)
		return;

	POSITION pos=rList.GetHeadPosition();
	while(pos)
	{
		m_FileList.AddString(rList.GetNext(pos));
	}

	GetDlgItem(IDC_ELIMIT)->EnableWindow(TRUE);
	GetDlgItem(IDC_BTNCHANGE)->EnableWindow(TRUE);
}

BOOL CDrChartSetDlg::_DoPreDirectoryCheck(CString &rString)
{
	GetDlgItemText(IDC_EDITDIR, rString);
	if(rString.GetLength()>0)
	{
		rString.Replace('/', '\\');
		if(rString.GetAt(rString.GetLength()-1)=='\\')
			rString = rString.Left(rString.GetLength()-1);
	}
	if(rString.IsEmpty())
	{
		AfxMessageBox("디렉토리 설정을 먼저 해주시기 바랍니다.");
		GetDlgItem(IDC_EDITDIR)->SetFocus();
		return FALSE;
	}

	theApp.WriteProfileString("DATA", "StartDir", rString);
	m_szTargetDir = rString;
	return TRUE;
}

#include "../../WrapDll/CX_DrChart/LoadPropDlgLib.h"
void CDrChartSetDlg::OnBtnchange() 
{
	m_ResultList.ResetContent();

	CString rString;
	if(_DoPreDirectoryCheck(rString)==FALSE)
		return;

	CPropDlgLib myLib;
	if(myLib.LoadLib() == NULL)
	{
		AfxMessageBox("CX_DrChartDgnDlg.dll 파일을 Load할 수 없습니다.");
		return;
	}

	CString rNHPS;
	GetDlgItemText(IDC_ELIMIT, rNHPS);
	if(rNHPS.IsEmpty())
	{
		rNHPS = _T("3000");
		SetDlgItemText(IDC_ELIMIT, rNHPS);
	}

	CString szFullPath, szTmp;
	int nMode=0;//modify
	CStringList DataList;
	int nRet, nResult;

	szTmp.Format("NHPS\t%s", rNHPS);
	DataList.AddTail(szTmp);
	int nCount = m_FileList.GetCount();
	int i=0;
	for(;i<nCount; i++)
	{
		m_FileList.GetText(i, szTmp);
		szFullPath.Format("%s\\%s", m_szTargetDir, szTmp);
		nRet = myLib.FndrPropertySetDlg(szFullPath, nMode, &DataList, nResult);
		if(nRet>0 && nResult>0)
			szTmp.Insert(0, "성공");
		else
			szTmp.Insert(0, "실패");

		//OutputDebugString(szTmp);OutputDebugString("\n");
		m_ResultList.AddString(szTmp);
	}	
}

void CDrChartSetDlg::OnBtnchartsetlup() 
{
	CString rString;
	if(_DoPreDirectoryCheck(rString)==FALSE)
		return;

	CString szDllFullName;
//	GetDir_DrfnRoot(szDllFullName);
//	szDllFullName += "\\Dev\\CX_DrChartDgnDlg.dll";

	CPropDlgLib myLib;
//	myLib.SetLibName(szDllFullName);
	if(myLib.LoadLib() == NULL)
	{
		AfxMessageBox("CX_DrChartDgnDlg.dll 파일을 Load할 수 없습니다.");
		return;
	}
	
	int nSel = m_FileList.GetCurSel();
	if(nSel<0)
	{
		AfxMessageBox("선택된 파일이 없습니다.");
		return;
	}

	CString szTmp;
	m_FileList.GetText(nSel, szTmp);

	CString szFullPath;
	szFullPath.Format("%s\\%s", m_szTargetDir, szTmp);

	char *pszRet = NULL;
	CWnd *pParent = this;
	const char* pszOpt = NULL;
	pszRet = myLib.FnAxModalPropDlg(pParent, pszOpt, szFullPath);
}

void CDrChartSetDlg::OnBtnchartsetlup2() 
{
	CPropDlgLib myLib;
	if(myLib.LoadLib() == NULL)
	{
		AfxMessageBox("CX_DrChartDgnDlg.dll 파일을 Load할 수 없습니다.");
		return;
	}

	CString szFullPath;
	GetDlgItemText(IDC_MAPFILEPATH, szFullPath);
		
	char *pszRet = NULL;
	CWnd *pParent = this;
	const char* pszOpt = NULL;
	pszRet = myLib.FnAxModalPropDlg(pParent, pszOpt, szFullPath);
}

int CDrChartSetDlg::_GetFileList(LPCSTR pszDir, CStringList& rList)
{
	int nLen = strlen(pszDir);
	if(nLen==0) return 0;

	LPCSTR pszExt = "*.*";
	char aFile[MAX_PATH];
	if(pszDir[nLen-1]=='\\' ||pszDir[nLen-1]=='/')
		sprintf(aFile, "%s%s", pszDir, pszExt);
	else
		sprintf(aFile, "%s\\%s", pszDir, pszExt);

	CString strComponent;
	CFileFind finder;
	BOOL bWorking = finder.FindFile( aFile );
	while (bWorking){
		bWorking = finder.FindNextFile();
		strComponent = finder.GetFileName();
		strComponent.MakeUpper();

		if(!finder.IsDirectory() && strComponent.Find(".MAP")>15)  
		{
			rList.AddTail(strComponent);
		}
	}
	return rList.GetCount();
}

void CDrChartSetDlg::OnChangeEditdir() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here

	GetDlgItem(IDC_ELIMIT)->EnableWindow(FALSE);
	GetDlgItem(IDC_BTNCHANGE)->EnableWindow(FALSE);
}

void CDrChartSetDlg::DoShowVip()
{
	GetDlgItem(IDC_BTNCHARTSETLUP)->ShowWindow(SW_SHOW);
}

