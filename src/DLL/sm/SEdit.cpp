// SEdit.cpp : implementation file
//

#include "stdafx.h"
#include "SEdit.h"
#include "basebar0.h"
#include "../../axis/axMsg.hxx"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSEdit

CSEdit::CSEdit(CDialog* parent)
{
	m_parent = parent;
}

CSEdit::~CSEdit()
{
}


BEGIN_MESSAGE_MAP(CSEdit, CEdit)
	//{{AFX_MSG_MAP(CSEdit)
	ON_CONTROL_REFLECT(EN_CHANGE, OnChange)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSEdit message handlers

void CSEdit::OnChange() 
{
	CString dat;
	GetWindowText(dat);
	if (dat.GetLength() == 4)
	{
		((CBasebar0 *)m_parent)->Append_Map(dat);
		SetWindowText(_T(""));
	}

}
