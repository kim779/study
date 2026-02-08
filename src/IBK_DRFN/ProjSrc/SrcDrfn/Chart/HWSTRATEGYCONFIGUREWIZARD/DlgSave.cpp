// DlgSave.cpp : implementation file
//

#include "stdafx.h"
#include "DlgSave.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgSave dialog
#define CRBACKGROUND	RGB(211,223,241)


CDlgSave::CDlgSave(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgSave::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgSave)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_crBk = CRBACKGROUND;
}


void CDlgSave::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgSave)
	DDX_Control(pDX, IDOK, m_btnOk);
	DDX_Control(pDX, IDCANCEL, m_btnCancel);
	DDX_Control(pDX, IDC_LIST_STFILE, m_listSTFile);
	DDX_Control(pDX, IDC_EDIT_NAME, m_editName);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgSave, CDialog)
	//{{AFX_MSG_MAP(CDlgSave)
	ON_LBN_SELCHANGE(IDC_LIST_STFILE, OnSelchangeListStfile)
	ON_WM_CTLCOLOR()
	ON_WM_DESTROY()
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgSave message handlers

BOOL CDlgSave::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_brBkColor.CreateSolidBrush(m_crBk);
//	m_btnOk.SetSkin(IDB_BITMAP_OKNOR,IDB_BITMAP_OKSEL,IDB_BITMAP_OKNOR,IDB_BITMAP_OKDIS);
//	m_btnCancel.SetSkin(IDB_BITMAP_CANCELNOR,IDB_BITMAP_CANCELSEL,IDB_BITMAP_CANCELNOR,IDB_BITMAP_CANCELDIS);
	m_strSTTitle.TrimRight();
	if(!m_strSTTitle.GetLength())
	{
		m_strSTTitle = "NONAME";	
	}
	m_editName.SetWindowText(m_strSTTitle);
	
	CString szSTFileTitle;
	CFileFind finder;
	BOOL bWorking = finder.FindFile(m_strUserPath);
	while (bWorking)
	{
		bWorking = finder.FindNextFile();

		szSTFileTitle = finder.GetFileTitle();
		//szSTFileTitle.Delete(0, szSTFileTitle.ReverseFind('\\') + 1);
		//int nExt = szSTFileTitle.Find(".ST");
		//szSTFileTitle.Delete(nExt, szSTFileTitle.GetLength());

		m_listSTFile.AddString(szSTFileTitle);
	}	
	int nPosOfList = m_listSTFile.FindStringExact(0,m_strSTTitle);
	if(nPosOfList!=-1)
	{
		m_listSTFile.SetCurSel(nPosOfList);
	}	
	SetTimer(1,100,NULL);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgSave::OnOK() 
{
	// TODO: Add extra validation here
	m_editName.GetWindowText(m_strSTTitle);	
	m_strSTTitle.TrimRight();
	if(!m_strSTTitle.GetLength())
	{
		MessageBox("저장명을 입력해 주십시오","저장");
	}
	else
	{
		if(m_strSTTitle.Find(gSTGubunKey)>=0)
		{
			AfxMessageBox("파일명에 \'@\'문자는 사용할 수 없습니다.");
		}
		else
		{
			CDialog::OnOK();
		}
	}	
}

void CDlgSave::OnSelchangeListStfile() 
{
	// TODO: Add your control notification handler code here
	CString strTitle;
	m_listSTFile.GetText(m_listSTFile.GetCurSel(),strTitle);
	m_editName.SetWindowText(strTitle);	
	m_editName.SetFocus();
	m_editName.SetSel(0, -1);
}

HBRUSH CDlgSave::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	
	// TODO: Change any attributes of the DC here
	switch(nCtlColor) 
	{
	case CTLCOLOR_EDIT:
	case CTLCOLOR_LISTBOX:
		return hbr;
		break;
	default:
		pDC->SetBkColor(m_crBk);
		return m_brBkColor;		
		break;
	}
	
	
	// TODO: Return a different brush if the default is not desired
	return hbr;
}

void CDlgSave::OnDestroy() 
{
	CDialog::OnDestroy();
	
	// TODO: Add your message handler code here
	m_brBkColor.DeleteObject();
	
}

void CDlgSave::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	if(nIDEvent==1)
	{
		m_editName.SetFocus();
		m_editName.SetSel(0, -1);
		KillTimer(1);
	}
	CDialog::OnTimer(nIDEvent);
}
