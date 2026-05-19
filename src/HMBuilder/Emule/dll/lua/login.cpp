// login.cpp : implementation file
//

#include "stdafx.h"

#include "login.h"
#include "../../h/axis.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CLogin

IMPLEMENT_DYNCREATE(CLogin, CCmdTarget)

CLogin::CLogin()
{
	EnableAutomation();
	clear();
}

CLogin::~CLogin()
{
}


void CLogin::OnFinalRelease()
{
	// When the last reference for an automation object is released
	// OnFinalRelease is called.  The base class will automatically
	// deletes the object.  Add additional cleanup required for your
	// object before calling the base class.

	CCmdTarget::OnFinalRelease();
}


BEGIN_MESSAGE_MAP(CLogin, CCmdTarget)
	//{{AFX_MSG_MAP(CLogin)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BEGIN_DISPATCH_MAP(CLogin, CCmdTarget)
	//{{AFX_DISPATCH_MAP(CLogin)
	//}}AFX_DISPATCH_MAP
END_DISPATCH_MAP()

// Note: we add support for IID_ILogin to support typesafe binding
//  from VBA.  This IID must match the GUID that is attached to the 
//  dispinterface in the .ODL file.

// {DEE758DA-505A-48FF-87CD-4E4CD82266A8}
static const IID IID_ILogin =
{ 0xdee758da, 0x505a, 0x48ff, { 0x87, 0xcd, 0x4e, 0x4c, 0xd8, 0x22, 0x66, 0xa8 } };

BEGIN_INTERFACE_MAP(CLogin, CCmdTarget)
	INTERFACE_PART(CLogin, IID_ILogin, Dispatch)
END_INTERFACE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLogin message handlersd

void CLogin::SetLogin(char* data)
{	
	clear();
	if (data == NULL || strlen(data) <= 0)
		return;

	int	pos;
	CString	string, text;
	struct	_signR*	signR = (struct _signR *)data;

	text = CString(signR->sign, sizeof(signR->sign));
	pos  = text.Find('\0');
	if (pos != -1)
		text = text.Left(pos);
	text.TrimRight();
	m_user = text;

	text = CString(signR->termN, sizeof(signR->termN));
	pos  = text.Find('\0');
	if (pos != -1)
		text = text.Left(pos);
	text.TrimRight();
	m_term = text;

	text = CString(signR->name, sizeof(signR->name));
	pos  = text.Find('\0');
	if (pos != -1)
		text = text.Left(pos);
	text.TrimRight();
	if (!text.IsEmpty())
		m_name = text;
	else
		m_name = _T("Anonymous");

	text = CString(signR->infox, sizeof(signR->infox));
	text.TrimRight();

	for (int ii = 0; !text.IsEmpty(); ii++)
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

		switch (ii)
		{
		case 0:
			m_commKind = text;
			break;
		case 1:
			m_media = text;
			break;
		case 2:
			m_dept = text;
			break;
		case 3:
			m_deptName = text;
			break;
		case 4:
			m_svrNo = text;
			break;
		case 5:
			m_method = text;
			break;
		case 6:
			m_idNo = text;
			break;
		case 7:
			m_grade = text;
			break;
		case 8:
			m_date = text;
			break;
		case 9:
			m_today = text;
			break;
		case 10:
			m_lmedia = text;
			break;
		case 11:
			m_ldate = text;
			break;
		case 12:
			m_ltime = text;
			break;
		case 13:
			m_check = text;
			break;
		case 14:
			m_popup = text;
			break;
		default:
			break;
		}
	}
}

void CLogin::clear()
{
	m_user		= _T("");
	m_name		= _T("");
	m_term		= _T("");

	m_commKind	= _T("");
	m_media		= _T("");
	m_dept		= _T("");
	m_deptName	= _T("");
	m_svrNo		= _T("");
	m_method	= _T("");
	m_idNo		= _T("");
	m_grade		= _T("");
	m_date		= _T("");
	m_today		= _T("");
	m_lmedia	= _T("");
	m_ldate		= _T("");
	m_ltime		= _T("");
	m_check		= _T("");
	m_popup		= _T("");
}

LPCTSTR CLogin::GetUsid()
{
	return (LPCTSTR)m_user.operator LPCTSTR();
}

LPCTSTR CLogin::GetUserName()
{
	return (LPCTSTR)m_name.operator LPCTSTR();
}

LPCTSTR CLogin::GetTerm()
{
	return (LPCTSTR)m_term.operator LPCTSTR();
}

LPCTSTR CLogin::GetCommKind()
{
	return (LPCTSTR)m_commKind.operator LPCTSTR();
}

LPCTSTR CLogin::GetMedia()
{
	return (LPCTSTR)m_media.operator LPCTSTR();
}

LPCTSTR CLogin::GetDept()
{
	return (LPCTSTR)m_dept.operator LPCTSTR();
}

LPCTSTR CLogin::GetDeptName()
{
	return (LPCTSTR)m_deptName.operator LPCTSTR();
}

LPCTSTR CLogin::GetSvrNo()
{
	return (LPCTSTR)m_svrNo.operator LPCTSTR();
}

LPCTSTR CLogin::GetMethod()
{
	return (LPCTSTR)m_method.operator LPCTSTR();
}

LPCTSTR CLogin::GetIdNo()
{
	return (LPCTSTR)m_idNo.operator LPCTSTR();
}

LPCTSTR CLogin::GetGrade()
{
	return (LPCTSTR)m_grade.operator LPCTSTR();
}

LPCTSTR CLogin::GetDate()
{
	return (LPCTSTR)m_date.operator LPCTSTR();
}

LPCTSTR CLogin::GetToday()
{
	return (LPCTSTR)m_today.operator LPCTSTR();
}

LPCTSTR CLogin::GetLmedia()
{
	return (LPCTSTR)m_media.operator LPCTSTR();
}

LPCTSTR CLogin::GetLdate()
{
	return (LPCTSTR)m_ldate.operator LPCTSTR();
}

LPCTSTR CLogin::GetLtime()
{
	return (LPCTSTR)m_ltime.operator LPCTSTR();
}

LPCTSTR CLogin::GetCheck()
{
	return (LPCTSTR)m_check.operator LPCTSTR();
}

LPCTSTR CLogin::GetPopup()
{
	return (LPCTSTR)m_popup.operator LPCTSTR();
}