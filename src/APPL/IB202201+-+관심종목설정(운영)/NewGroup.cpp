// NewGroup.cpp : implementation file
//

#include "stdafx.h"
#include "IB202201.h"
#include "NewGroup.h"
#include "EditGroup.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CNewGroup dialog


CNewGroup::CNewGroup(CWnd* pParent, int dlgMode, CString defName)
	: CDialog(CNewGroup::IDD, pParent)
{
	//{{AFX_DATA_INIT(CNewGroup)
	//}}AFX_DATA_INIT
	m_pParent = pParent;

	m_dlgMode = dlgMode;
	m_defName = defName;
}


void CNewGroup::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CNewGroup)
	DDX_Control(pDX, IDC_GRPNAME, m_grpname);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CNewGroup, CDialog)
	//{{AFX_MSG_MAP(CNewGroup)
	ON_EN_MAXTEXT(IDC_GRPNAME, OnMaxtextGrpname)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CNewGroup message handlers

BOOL CNewGroup::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	m_title.LoadString(IDS_NEWGROUP);
	if (m_dlgMode == 1) m_title.LoadString(IDS_RENGROUP);
	SetWindowText(m_title);

	m_grpname.SetWindowText(m_defName);
	m_grpname.SetLimitText(18);
	m_grpname.SetSel(0, -1);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CNewGroup::OnOK() 
{
	CString	string; 

	m_grpname.GetWindowText(string);
	string.TrimLeft(); 
	string.TrimRight();

	if (string.GetLength() <= 0)
	{
		string.LoadString(IDS_ENOGROUP);
		MessageBox(string, m_title, MB_OK|MB_ICONEXCLAMATION);
		return;
	}

	//메세징 배열 수정
	CString sequence, temp, grpName;
//	int index = 0;
	for(int i=0 ; i<100 ; i++)
	{
		if(m_dlgMode == 0)	//새그룹
		{
			sequence = ((CPage1*)m_pParent)->m_manageGroup[i][0];
			grpName = ((CPage1*)m_pParent)->m_manageGroup[i][3];
		}
		else				//그룹명 바꾸기
			sequence = ((CPage1*)((CEditGroup*)m_pParent)->m_pParent)->m_manageGroup[i][0];
		
		if(!sequence.IsEmpty())
			continue;

		temp.Format("%02d", i+1);
		
		if(i != 100)
		{
			if(m_dlgMode == 0)
			{
				((CPage1*)m_pParent)->m_manageGroup[i][0] = temp;
				((CPage1*)m_pParent)->m_manageGroup[i][1] = "N";
				((CPage1*)m_pParent)->m_manageGroup[i][2] = temp;
				((CPage1*)m_pParent)->m_manageGroup[i][3] = string;
			}
			
			break;
		}
	}
	
	m_defName = string; 
	
	CDialog::OnOK();
}

void CNewGroup::OnCancel() 
{
	CDialog::OnCancel();
}

void CNewGroup::OnMaxtextGrpname() 
{

}