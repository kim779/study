// PinchDLG.cpp : implementation file
//

#include "stdafx.h"
#include "axis.h"
#include "PinchDLG.h"
#include "axMisc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPinchDLG dialog


CPinchDLG::CPinchDLG(CWnd* pParent /*=NULL*/)
	: CDialog(CPinchDLG::IDD, pParent)
{
	m_pParent = pParent;
}


void CPinchDLG::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPinchDLG)
	DDX_Control(pDX, IDC_COMBO4, m_VirSet4);
	DDX_Control(pDX, IDC_COMBO3, m_VirSet3);
	DDX_Control(pDX, IDC_COMBO2, m_VirSet2);
	DDX_Control(pDX, IDC_COMBO1, m_VirSet1);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPinchDLG, CDialog)
	//{{AFX_MSG_MAP(CPinchDLG)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPinchDLG message handlers

void CPinchDLG::OnOK() 
{
	// TODO: Add extra validation here

	m_VirSet1.GetLBText(m_VirSet1.GetCurSel(), m_User[0]);
	m_VirSet2.GetLBText(m_VirSet2.GetCurSel(), m_User[1]);
	m_VirSet3.GetLBText(m_VirSet3.GetCurSel(), m_User[2]);
	m_VirSet4.GetLBText(m_VirSet4.GetCurSel(), m_User[3]);

	CDialog::OnOK();
}

BOOL CPinchDLG::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	m_VirSet1.ResetContent();
	m_VirSet2.ResetContent();
	m_VirSet3.ResetContent();
	m_VirSet4.ResetContent();

	SetData();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CPinchDLG::SetData()
{
	CString	string, text, tmps;
	int	pos, userpos;
	int ii = 0;

	char* dta = (char*)m_pParent->SendMessage(WM_GETPINCH, 0, 0);

	tmps = CString((char*)dta);

	userpos = tmps.Find('\n');

	if (userpos != -1)
	{
		text = tmps.Left(userpos);
		tmps = tmps.Mid(userpos+1);
	}

	for (ii = 0; !text.IsEmpty(); ii++)
	{
		pos = text.Find('\t');
		if (pos != -1)
		{
			string = text.Left(pos);
			text   = text.Mid(pos+1);
		}
		else
		{
			string = text;
			text   = _T("");
		}

		m_VirSet1.AddString(string);
		m_VirSet2.AddString(string);
		m_VirSet3.AddString(string);
		m_VirSet4.AddString(string);
	}

	for (ii = 0; ii < 4; ii++)
	{
		userpos = tmps.Find('\t');

		if (userpos != -1)
		{
			m_User[ii] = tmps.Left(userpos);
			tmps = tmps.Mid(userpos+1);
		}
		else
			m_User[ii] = _T("");
	}

	CString str;
	int	n;

	for (ii = 0 ; ii < m_VirSet1.GetCount(); ii++)
	{
		n = m_VirSet1.GetLBTextLen(ii);
		m_VirSet1.GetLBText(ii, str);

		if (m_User[0] == str)
		{
			m_VirSet1.SetCurSel(ii);
			break;
		}
		else
			m_VirSet1.SetCurSel(0);
	}

	for (ii = 0 ; ii < m_VirSet2.GetCount(); ii++)
	{
		n = m_VirSet2.GetLBTextLen(ii);
		m_VirSet2.GetLBText(ii, str);

		if (m_User[1] == str)
		{
			m_VirSet2.SetCurSel(ii);
			break;
		}
		else
			m_VirSet2.SetCurSel(0);
	}

	for (ii = 0 ; ii < m_VirSet3.GetCount(); ii++)
	{
		n = m_VirSet3.GetLBTextLen(ii);
		m_VirSet3.GetLBText(ii, str);

		if (m_User[2] == str)
		{
			m_VirSet3.SetCurSel(ii);
			break;
		}
		else
			m_VirSet3.SetCurSel(0);
	}

	for (ii = 0 ; ii < m_VirSet4.GetCount(); ii++)
	{
		n = m_VirSet4.GetLBTextLen(ii);
		m_VirSet4.GetLBText(ii, str);

		if (m_User[3] == str)
		{
			m_VirSet4.SetCurSel(ii);
			break;
		}
		else
			m_VirSet4.SetCurSel(0);
	}
}
