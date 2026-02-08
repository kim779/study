// DlgFindScript.cpp : implementation file
//

#include "stdafx.h"
#include "systemtrading.h"
#include "DlgFindScript.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgFindScript dialog


CDlgFindScript::CDlgFindScript(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgFindScript::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgFindScript)
	m_bFindFunction = TRUE;
	m_bFindStrategy = TRUE;
	m_bMatchCase = FALSE;
	m_bWholeWord = FALSE;
	m_strText = _T("");
	//}}AFX_DATA_INIT
}


void CDlgFindScript::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgFindScript)
	DDX_Control(pDX, IDCANCEL, m_btnCancel);
	DDX_Control(pDX, IDOK, m_btnFind);
	DDX_Check(pDX, IDC_CHK_FUNCTION, m_bFindFunction);
	DDX_Check(pDX, IDC_CHK_STRATEGY, m_bFindStrategy);
	DDX_Check(pDX, IDC_EDIT_MATCH_CASE, m_bMatchCase);
	DDX_Check(pDX, IDC_EDIT_WHOLE_WORD, m_bWholeWord);
	DDX_Text(pDX, IDC_EDIT_TEXT, m_strText);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgFindScript, CDialog)
	//{{AFX_MSG_MAP(CDlgFindScript)
	ON_EN_CHANGE(IDC_EDIT_TEXT, OnChangeEditText)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgFindScript message handlers

void CDlgFindScript::OnOK() 
{
	UpdateData();
	if(!m_bFindFunction && !m_bFindStrategy)
	{
		AfxMessageBox(_T("적어도 하나의 전략/함수 옵션을 선택하여야 합니다."));
		return;
	}
	
	CDialog::OnOK();
}

void CDlgFindScript::OnChangeEditText() 
{
	CString strText;
	GetDlgItem(IDC_EDIT_TEXT)->GetWindowText(strText);
	GetDlgItem(IDOK)->EnableWindow(strText != _T(""));
}

BOOL CDlgFindScript::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	GetDlgItem(IDOK)->EnableWindow(m_strText != _T(""));
	
	CBitmap bitmap;
	bitmap.LoadBitmap(IDB_BITMAP2);
	
	CImageList imageList;
	imageList.Create(24, 24, ILC_COLOR24|ILC_MASK, 4, 1);
	imageList.Add(&bitmap, RGB(0,255,0));

	m_btnFind.SetIcon(CSize(24,24), imageList.ExtractIcon(1), imageList.ExtractIcon(2));
//	m_btnCancel.SetIcon(CSize(24,24), imageList.ExtractIcon(1), imageList.ExtractIcon(2));

	m_btnFind.SetXButtonStyle(BS_XT_XPFLAT|BS_XT_SHOWFOCUS);
	m_btnCancel.SetXButtonStyle(BS_XT_XPFLAT|BS_XT_SHOWFOCUS);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
