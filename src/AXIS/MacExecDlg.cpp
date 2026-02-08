// MacExecDlg.cpp : implementation file
//

#include "stdafx.h"
#include "MacExec.h"
#include "MacExecDlg.h"

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
// CMacExecDlg dialog

CMacExecDlg::CMacExecDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CMacExecDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CMacExecDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMacExecDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMacExecDlg)
	DDX_Control(pDX, IDC_LIST, m_list);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CMacExecDlg, CDialog)
	//{{AFX_MSG_MAP(CMacExecDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_ADD, OnAdd)
	ON_BN_CLICKED(IDC_EDIT, OnEdit)
	ON_BN_CLICKED(IDC_DELETE, OnDelete)
	ON_WM_DESTROY()
	ON_NOTIFY(NM_DBLCLK, IDC_LIST, OnDblclkList)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMacExecDlg message handlers

BOOL CMacExecDlg::OnInitDialog()
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
	
	Init();
	Load();
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CMacExecDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CMacExecDlg::OnPaint() 
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
HCURSOR CMacExecDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CMacExecDlg::Init()
{
	m_list.SetExtendedStyle(LVS_EX_FULLROWSELECT);

	m_list.InsertColumn(0, "no", LVCFMT_RIGHT, 30);
	m_list.InsertColumn(1, "Alias", LVCFMT_LEFT, 80);
	m_list.InsertColumn(2, "IP", LVCFMT_LEFT, 100);
	m_list.InsertColumn(3, "Port", LVCFMT_RIGHT, 50);
	m_list.InsertColumn(4, "Reg Key", LVCFMT_LEFT, 120);
	m_list.InsertColumn(5, "UserID", LVCFMT_LEFT, 80);
	m_list.InsertColumn(6, "Password", LVCFMT_LEFT, 50);
	m_list.InsertColumn(7, "FileName", LVCFMT_LEFT, 200);
}

void CMacExecDlg::Load()
{
	Config* config;

	char buffer[MAX_PATH + 1];
	CString s, alias;
	int i = 0;

	while (TRUE)
	{
		s.Format("%03d", i++);
		GetPrivateProfileString("List", s, "", buffer, MAX_PATH, PROFILE_FILE);

		if (CString(buffer).IsEmpty())
			break;

		config = new Config(buffer);
		m_config.AddTail(config);
		config->Load(PROFILE_FILE);
	}

	DisplayListItem();
}

void CMacExecDlg::Save()
{
	int i = 0;
	Config* config;
	CString s;

	WritePrivateProfileSection("List", "", PROFILE_FILE);
	for (POSITION pos = m_config.GetHeadPosition(); pos; i++)
	{
		config = m_config.GetNext(pos);

		s.Format("%03d", i);
		WritePrivateProfileString("List", s, config->alias, PROFILE_FILE);
		config->Save(PROFILE_FILE);
	}
}

void CMacExecDlg::Free()
{
	while (!m_config.IsEmpty())
	{
		delete m_config.RemoveHead();
	}
}

void CMacExecDlg::DisplayListItem()
{
	m_list.DeleteAllItems();

	int i = 0;
	
	for (POSITION pos = m_config.GetHeadPosition(); pos; i++)
	{
		DisplayListItem(i, m_config.GetNext(pos));
	}
}

void CMacExecDlg::DisplayListItem(int row, Config* config, BOOL bAdd)
{
	CString s;

	s.Format("%3d", row + 1);
	if (bAdd)
		m_list.InsertItem(row, s);

	m_list.SetItemText(row, 1, config->alias);
	m_list.SetItemText(row, 2, config->ip);
	s.Format("%d", config->port);
	m_list.SetItemText(row, 3, s);
	m_list.SetItemText(row, 4, config->regKey);
	m_list.SetItemText(row, 5, config->userID);
	m_list.SetItemText(row, 6, config->password.IsEmpty() ? "": "****");
	m_list.SetItemText(row, 7, config->fileName);
}

void CMacExecDlg::OnAdd() 
{
	CConfigDlg dlg;

	if (IDOK == dlg.DoModal())
	{
		Config* cfg = new Config(dlg.m_alias);
		cfg->ip = dlg.m_ip;
		cfg->port = dlg.m_port;
		cfg->fileName = dlg.m_fileName;
		cfg->regKey = dlg.m_regKey;
		cfg->userID = dlg.m_userID;
		cfg->password = dlg.m_password;

		m_config.AddTail(cfg);

		DisplayListItem(m_list.GetItemCount(), cfg);
		Save();
	}
}

void CMacExecDlg::OnEdit() 
{
	int index = GetListIndex();
	if (index < 0)
	{
		MessageBox("선택된 연결설정 항목이 없습니다.", "확인", MB_OK | MB_ICONINFORMATION);
		return;
	}

	POSITION pos = m_config.FindIndex(index);
	ASSERT(pos);
	Config* cfg = m_config.GetAt(pos);
	ASSERT(cfg);

	CConfigDlg dlg(cfg);

	if (IDOK == dlg.DoModal())
	{
		cfg->alias = dlg.m_alias;
		cfg->ip = dlg.m_ip;
		cfg->port = dlg.m_port;
		cfg->fileName = dlg.m_fileName;
		cfg->regKey = dlg.m_regKey;
		cfg->userID = dlg.m_userID;
		cfg->password = dlg.m_password;

		DisplayListItem(index, cfg, FALSE);
		Save();
	}
}

void CMacExecDlg::OnDelete() 
{
	int index = GetListIndex();
	if (index < 0)
	{
		MessageBox("선택된 연결설정 항목이 없습니다.", "확인", MB_OK | MB_ICONINFORMATION);
		return;
	}

	if (IDOK != MessageBox("삭제된 연결설정 항목은 복구되지 않습니다. \n정말 삭제하시겠습니까?", "삭제확인", MB_OKCANCEL | MB_ICONQUESTION))
		return;

	POSITION pos = m_config.FindIndex(index);
	ASSERT(pos);
	Config* cfg = m_config.GetAt(pos);
	ASSERT(cfg);
	m_config.RemoveAt(pos);

	delete cfg;

	DisplayListItem();
	Save();
}

void CMacExecDlg::OnOK() 
{
	Execute();
}

void CMacExecDlg::OnDestroy() 
{
	CDialog::OnDestroy();
	
	Free();	
}

int CMacExecDlg::GetListIndex()
{
	for (int i = 0; i < m_list.GetItemCount(); i++)
	{
		if (m_list.GetItemState(i, LVIS_SELECTED) & LVIS_SELECTED)
			return i;
	}

	return -1;
}

void CMacExecDlg::OnDblclkList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	*pResult = 0;

	Execute();
}

void CMacExecDlg::Execute()
{
	int index = GetListIndex();
	if (index < 0)
		return;
	
	POSITION pos = m_config.FindIndex(index);
	ASSERT(pos);
	Config* cfg = m_config.GetAt(pos);
	ASSERT(cfg);

	Execute(cfg);
}

void CMacExecDlg::Execute(Config* cfg)
{
	CString cmd(" /f \"\" /g \"\" ");
	CString s;

	/**
	s.Format("/p %c", 0x7f);
	cmd += s;
	**/

	if (cfg->port > 0)
	{
		s.Format(" /t %d", cfg->port);
		cmd += s;
	}

	if (!cfg->ip.IsEmpty())
	{
		s.Format(" /a %s", cfg->ip);
		cmd += s;
	}

	if (!cfg->regKey.IsEmpty())
	{
		s.Format(" /k \"%s\"", cfg->regKey);
		cmd += s;
	}

	if (!cfg->userID.IsEmpty())
	{
		s.Format(" /i %s", cfg->userID);
		cmd += s;
		if (!cfg->password.IsEmpty())
		{
			s.Format(" %s", cfg->password);
			cmd += s;
		}
	}

	cmd += " /x";

	STARTUPINFO		si;
	PROCESS_INFORMATION	pi;

	ZeroMemory(&si, sizeof(STARTUPINFO));
	ZeroMemory(&pi, sizeof(PROCESS_INFORMATION));

	si.cb          = sizeof(STARTUPINFO);
	si.dwFlags     = STARTF_USESHOWWINDOW;
	si.wShowWindow = SW_SHOWNORMAL;

	BOOL bRc = CreateProcess(
			cfg->fileName,				// application name
			(char *)(const char*)cmd,// command line
			NULL,				// process attribute
			NULL,				// thread attribute
			FALSE,				// is inherit handle
			0,					// creation flags
			NULL,				// environment
			NULL,				// current directory
			&si,				// STARTUPINFO
			&pi);				// PROCESS_INFORMATION

}
