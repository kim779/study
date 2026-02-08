// login.cpp : implementation file
//

#include "stdafx.h"
#include "axislogin.h"
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
	DISP_PROPERTY_EX(CLogin, "User", _getUser, SetNotSupported, VT_BSTR)
	DISP_PROPERTY_EX(CLogin, "Name", _getName, SetNotSupported, VT_BSTR)
	DISP_PROPERTY_EX(CLogin, "Order", _getOrder, SetNotSupported, VT_BOOL)
	DISP_PROPERTY_EX(CLogin, "Cogb", _getCogb, SetNotSupported, VT_BSTR)
	DISP_PROPERTY_EX(CLogin, "Brno", _getBrno, SetNotSupported, VT_BSTR)
	DISP_PROPERTY_EX(CLogin, "Cani", _getCani, SetNotSupported, VT_BSTR)
	DISP_PROPERTY_EX(CLogin, "Utyp", _getUtyp, SetNotSupported, VT_BSTR)
	DISP_PROPERTY_EX(CLogin, "Ctyp", _getCtyp, SetNotSupported, VT_BSTR)
	DISP_PROPERTY_EX(CLogin, "Otpf", _getOtpf, SetNotSupported, VT_BSTR)
	DISP_PROPERTY_EX(CLogin, "Juno", _getJuno, SetNotSupported, VT_BSTR)
	DISP_PROPERTY_EX(CLogin, "Lcog", _getLcog, SetNotSupported, VT_BSTR)
	DISP_PROPERTY_EX(CLogin, "Lpci", _getLpci, SetNotSupported, VT_BSTR)
	DISP_PROPERTY_EX(CLogin, "Lpui", _getLpui, SetNotSupported, VT_BSTR)
	DISP_PROPERTY_EX(CLogin, "Litm", _getLitm, SetNotSupported, VT_BSTR)
	DISP_PROPERTY_EX(CLogin, "Lotm", _getLotm, SetNotSupported, VT_BSTR)
	DISP_PROPERTY_EX(CLogin, "Day0", _getDay0, SetNotSupported, VT_BSTR)
	DISP_PROPERTY_EX(CLogin, "Day1", _getDay1, SetNotSupported, VT_BSTR)
	DISP_PROPERTY_EX(CLogin, "Day2", _getDay2, SetNotSupported, VT_BSTR)
	DISP_PROPERTY_EX(CLogin, "Misf", _getMisf, SetNotSupported, VT_BSTR)
	DISP_PROPERTY_EX(CLogin, "Nick", _getNick, SetNotSupported, VT_BSTR)
	//}}AFX_DISPATCH_MAP
END_DISPATCH_MAP()

// Note: we add support for IID_ILogin to support typesafe binding
//  from VBA.  This IID must match the GUID that is attached to the 
//  dispinterface in the .ODL file.

// {EE69E4E4-5BF8-4CAD-9C8E-54250CF33A1A}
static const IID IID_ILogin =
{ 0xee69e4e4, 0x5bf8, 0x4cad, { 0x9c, 0x8e, 0x54, 0x25, 0xc, 0xf3, 0x3a, 0x1a } };

BEGIN_INTERFACE_MAP(CLogin, CCmdTarget)
	INTERFACE_PART(CLogin, IID_ILogin, Dispatch)
END_INTERFACE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLogin message handlers

BSTR CLogin::_getUser() 
{
	return m_user.AllocSysString();
}

BSTR CLogin::_getName() 
{
	return m_name.AllocSysString();
}

BOOL CLogin::_getOrder() 
{
	return m_order;
}

CString CLogin::SetLogin(char* data)
{
	if (data == NULL || strlen(data) <= 0)
	{
		clear();
		return _T("");
	}

	int	pos;
	CString	string, text;
	struct	_signR*	signR = (struct _signR *)data;
	CAxisloginApp* app = (CAxisloginApp *)AfxGetApp();

	clear();
	m_order = (signR->flag & flagACN) ? FALSE : TRUE;

	text = CString(signR->sign, sizeof(signR->sign));
	pos  = text.Find('\0');
	if (pos != -1)
		text = text.Left(pos);
	text.TrimRight();
	app->m_user = text;

	text = CString(signR->termN, sizeof(signR->termN));
	pos  = text.Find('\0');
	if (pos != -1)
		text = text.Left(pos);
	text.TrimRight();
	app->m_term = text;

	text = CString(signR->infox, sizeof(signR->infox));
	if (text.Find('\t') == -1)
	{
		setDefault(data);
		return _T("");
	}

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
		case 0:  m_cogb = string;	break;
		case 1:  m_cani = string;	break;
		case 2:  m_brno = string;	break;
		case 3:  m_utyp = string;	break;
		case 4:  m_ctyp = string;	break;
		case 5:  m_otpf = string;	break;
		case 6:  m_misf = string;	break;
		case 7:  m_juno = string;	break;
		case 8:  m_name = string;	break;
		case 9:  m_lcog = string;	break;
		case 10: m_lpci = string;	break;
		case 11: m_lpui = string;	break;
		case 12: m_litm = string;	break;
		case 13: m_lotm = string;	break;
		case 14: m_day0 = string;	break;
		case 15: m_day1 = string;	break;
		case 16: m_day2 = string;	break;
		case 17: m_nick = string;	break;
		}
	}
	app->m_cogb = m_cogb;
	app->m_brno = m_brno;
	if (m_user.IsEmpty() || m_name.IsEmpty())
		setDefault(data);
	return m_returns;
}

CString CLogin::GetLogin(int id)
{
	switch (id)
	{
	case 0:		// postCode
		m_returns = m_brno;	break;
	case 1:		// date
		m_returns.Empty();	break;
	case 2:		// can input account
		m_returns = m_cani;	break;
	default:
		m_returns.Empty();	break;
	}
	return m_returns;
}

void CLogin::clear()
{
	m_returns = _T("");
	m_user    = _T("");
	m_name    = _T("");
	m_cogb    = _T("");
	m_brno    = _T("");
	m_cani    = _T("");
	m_utyp    = _T("");
	m_ctyp    = _T("");
	m_otpf    = _T("");
	m_misf    = _T("");
	m_juno    = _T("");
	m_lcog    = _T("");
	m_lpci    = _T("");
	m_lpui    = _T("");
	m_litm    = _T("");
	m_lotm    = _T("");
	m_day0    = _T("");
	m_day1    = _T("");
	m_day2    = _T("");
	m_nick    = _T("");

	m_order   = FALSE;
}

void CLogin::setDefault(char* data)
{
	int	idx;
	CString	text;
	struct	_signR*	signR = (struct _signR *)data;

	text = CString(signR->sign, sizeof(signR->sign));
	idx  = text.Find('\0');
	if (idx != -1)
		text = text.Left(idx);
	text.TrimRight();
	m_user = text;

	text = CString(signR->name, sizeof(signR->name));
	idx  = text.Find('\0');
	if (idx != -1)
		text = text.Left(idx);
	text.TrimRight();
	if (text.IsEmpty())
		text = _T("Anonymous");
	m_name  = text;
	m_order = (signR->flag & flagACN) ? FALSE : TRUE;
}

BSTR CLogin::_getCogb() 
{
	return m_cogb.AllocSysString();
}

BSTR CLogin::_getBrno() 
{
	return m_brno.AllocSysString();
}

BSTR CLogin::_getCani() 
{
	return m_cani.AllocSysString();
}

BSTR CLogin::_getUtyp() 
{
	return m_utyp.AllocSysString();
}

BSTR CLogin::_getCtyp() 
{
	return m_ctyp.AllocSysString();
}

BSTR CLogin::_getOtpf() 
{
	return m_otpf.AllocSysString();
}

BSTR CLogin::_getJuno() 
{
	return m_juno.AllocSysString();
}

BSTR CLogin::_getLcog() 
{
	return m_lcog.AllocSysString();
}

BSTR CLogin::_getLpci() 
{
	return m_lpci.AllocSysString();
}

BSTR CLogin::_getLpui() 
{
	return m_lpui.AllocSysString();
}

BSTR CLogin::_getLitm() 
{
	return m_litm.AllocSysString();
}

BSTR CLogin::_getLotm() 
{
	return m_lotm.AllocSysString();
}

BSTR CLogin::_getDay0() 
{
	return m_day0.AllocSysString();
}

BSTR CLogin::_getDay1() 
{
	return m_day1.AllocSysString();
}

BSTR CLogin::_getDay2() 
{
	return m_day2.AllocSysString();
}

BSTR CLogin::_getMisf() 
{
	return m_misf.AllocSysString();
}

BSTR CLogin::_getNick() 
{
	return m_nick.AllocSysString();
}
