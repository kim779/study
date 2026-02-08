// CHashPrjDlg.cpp : implementation file
//

#include "stdafx.h"
#include "CHashPrj.h"
#include "CHashPrjDlg.h"

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
// CCHashPrjDlg dialog

CCHashPrjDlg::CCHashPrjDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CCHashPrjDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCHashPrjDlg)
	//}}AFX_DATA_INIT
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CCHashPrjDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCHashPrjDlg)
	DDX_Control(pDX, IDC_FILE, m_inFile);
	DDX_Control(pDX, IDC_OUTHASH, m_outHash);
	DDX_Control(pDX, IDC_HASHALGO, m_hashAlgo);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CCHashPrjDlg, CDialog)
	//{{AFX_MSG_MAP(CCHashPrjDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, OnBrowseFile)
	ON_BN_CLICKED(IDC_BUTTON2, OnDoHash)
	ON_BN_CLICKED(IDC_STRING, OnString)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCHashPrjDlg message handlers

BOOL CCHashPrjDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Set the style to uppercase with spaces
	hashObj.SetHashFormat(UPPERCASE_SPACES);

	// And check the corresponding option
	this->CheckDlgButton(IDC_OPT4, TRUE);

	// Lastly make SHA-1 the default algorithm
	m_hashAlgo.SetCurSel(5);
 
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

	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	// TODO: Add extra initialization here
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CCHashPrjDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CCHashPrjDlg::OnPaint() 
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

HCURSOR CCHashPrjDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CCHashPrjDlg::OnBrowseFile() 
{
	CFileDialog dlg(TRUE, NULL, hashObj.GetHashFile(), OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, "All Files(*.)|*.*||", this);
	if(dlg.DoModal() == IDOK)
	{
		this->SetDlgItemText(IDC_FILE, (LPCSTR)dlg.GetPathName());
	}	
}

void CCHashPrjDlg::OnDoHash() 
{
	this->SetDlgItemText(IDC_OUTHASH, (LPCSTR)"");

	hashObj.SetHashOperation(FILE_HASH);

	CString str;

	this->GetDlgItemText(IDC_FILE, str);

	hashObj.SetHashFile(str);

	// Get the user selected hash style
	if (this->IsDlgButtonChecked(IDC_OPT1))
		hashObj.SetHashFormat(1);
	if (this->IsDlgButtonChecked(IDC_OPT2))
		hashObj.SetHashFormat(2);
	if (this->IsDlgButtonChecked(IDC_OPT3))
		hashObj.SetHashFormat(3);
	if (this->IsDlgButtonChecked(IDC_OPT4))
		hashObj.SetHashFormat(4);

	// Define a buffer to hold the output hash
	CString outHash;

	// Get the current algorithm
	int hashAlgo = m_hashAlgo.GetCurSel() + 1;

	if (hashAlgo < 7)
	{
		hashObj.SetHashAlgorithm(hashAlgo);
		outHash = hashObj.DoHash();
	}

	if (hashAlgo == 7)
	{
		hashObj.SetHashAlgorithm(SHA2);
		hashObj.SetSHA2Strength(256);
		outHash = hashObj.DoHash();
	}
	
	if (hashAlgo == 8)
	{
		hashObj.SetHashAlgorithm(SHA2);
		hashObj.SetSHA2Strength(384);
		outHash = hashObj.DoHash();
	}

	if (hashAlgo == 9)
	{
		hashObj.SetHashAlgorithm(SHA2);
		hashObj.SetSHA2Strength(512);
		outHash = hashObj.DoHash();
	}

	// Set the editbox on the dialog to the resulting hash
	this->SetDlgItemText(IDC_OUTHASH, (LPCSTR)outHash);	
}

void CCHashPrjDlg::OnString() 
{
	this->SetDlgItemText(IDC_OUTHASH, (LPCSTR)"");		

	hashObj.SetHashOperation(STRING_HASH);

	CString str;
	
	str.Empty();

	this->GetDlgItemText(IDC_HASH, str);

	hashObj.SetHashString(str);

	// Get the user selected hash style
	if (this->IsDlgButtonChecked(IDC_OPT1))
		hashObj.SetHashFormat(1);
	if (this->IsDlgButtonChecked(IDC_OPT2))
		hashObj.SetHashFormat(2);
	if (this->IsDlgButtonChecked(IDC_OPT3))
		hashObj.SetHashFormat(3);
	if (this->IsDlgButtonChecked(IDC_OPT4))
		hashObj.SetHashFormat(4);

	// Define a buffer to hold the output hash
	CString outHash;

	// Get the current algorithm
	int hashAlgo = m_hashAlgo.GetCurSel() + 1;

	if (hashAlgo < 7)
	{
		hashObj.SetHashAlgorithm(hashAlgo);
		outHash = hashObj.DoHash();
	}

	if (hashAlgo == 7)
	{
		hashObj.SetHashAlgorithm(SHA2);
		hashObj.SetSHA2Strength(256);
		outHash = hashObj.DoHash();
	}
	
	if (hashAlgo == 8)
	{
		hashObj.SetHashAlgorithm(SHA2);
		hashObj.SetSHA2Strength(384);
		outHash = hashObj.DoHash();
	}

	if (hashAlgo == 9)
	{
		hashObj.SetHashAlgorithm(SHA2);
		hashObj.SetSHA2Strength(512);
		outHash = hashObj.DoHash();
	}

	// Set the editbox on the dialog to the resulting hash
	this->SetDlgItemText(IDC_OUTHASH, (LPCSTR)outHash);		
}
