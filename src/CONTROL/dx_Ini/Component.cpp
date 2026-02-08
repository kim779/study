// Component.cpp : implementation file
//

#include "stdafx.h"
#include "DX_Ini.h"
#include "Component.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CComponent

[[gsl::suppress(26409)]]
IMPLEMENT_DYNCREATE(CComponent, CCmdTarget)

CComponent::CComponent()
{
	EnableAutomation();

	m_file = "";
}

CComponent::~CComponent()
{
}


void CComponent::OnFinalRelease()
{
	// When the last reference for an automation object is released
	// OnFinalRelease is called.  The base class will automatically
	// deletes the object.  Add additional cleanup required for your
	// object before calling the base class.

	CCmdTarget::OnFinalRelease();
}


BEGIN_MESSAGE_MAP(CComponent, CCmdTarget)
	//{{AFX_MSG_MAP(CComponent)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BEGIN_DISPATCH_MAP(CComponent, CCmdTarget)
	//{{AFX_DISPATCH_MAP(CComponent)
	DISP_FUNCTION(CComponent, "Open", _Open, VT_BOOL, VTS_BSTR)
	DISP_FUNCTION(CComponent, "SectionExists", _SectionExists, VT_BOOL, VTS_BSTR)
	DISP_FUNCTION(CComponent, "ReadInteger", _ReadInteger, VT_I4, VTS_BSTR VTS_BSTR VTS_I4)
	DISP_FUNCTION(CComponent, "ReadString", _ReadString, VT_BSTR, VTS_BSTR VTS_BSTR VTS_BSTR)
	DISP_FUNCTION(CComponent, "ReadFloat", _ReadFloat, VT_R8, VTS_BSTR VTS_BSTR VTS_R8)
	DISP_FUNCTION(CComponent, "ReadBoolean", _ReadBool, VT_BOOL, VTS_BSTR VTS_BSTR VTS_BOOL)
	DISP_FUNCTION(CComponent, "WriteFloat", _WriteFloat, VT_EMPTY, VTS_BSTR VTS_BSTR VTS_R8)
	DISP_FUNCTION(CComponent, "WriteInteger", _WriteInteger, VT_EMPTY, VTS_BSTR VTS_BSTR VTS_I4)
	DISP_FUNCTION(CComponent, "WriteString", _WriteString, VT_EMPTY, VTS_BSTR VTS_BSTR VTS_BSTR)
	DISP_FUNCTION(CComponent, "WriteBoolean", _WriteBoolean, VT_EMPTY, VTS_BSTR VTS_BSTR VTS_BOOL)
	DISP_FUNCTION(CComponent, "Close", _Close, VT_EMPTY, VTS_NONE)
	//}}AFX_DISPATCH_MAP
END_DISPATCH_MAP()

// Note: we add support for IID_IComponent to support typesafe binding
//  from VBA.  This IID must match the GUID that is attached to the 
//  dispinterface in the .ODL file.

// {7A007DA4-C484-4E1E-9405-4351E06506D4}
static const IID IID_IComponent =
{ 0x7a007da4, 0xc484, 0x4e1e, { 0x94, 0x5, 0x43, 0x51, 0xe0, 0x65, 0x6, 0xd4 } };

BEGIN_INTERFACE_MAP(CComponent, CCmdTarget)
	INTERFACE_PART(CComponent, IID_IComponent, Dispatch)
END_INTERFACE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CComponent message handlers

BOOL CComponent::_Open(LPCTSTR FileName) 
{
	//CFileFind find;
	
	//m_bOpen = find.FindFile(FileName) == TRUE;
	//if (m_bOpen) 
		m_file = FileName;
	
	return TRUE; 
}

BOOL CComponent::_SectionExists(LPCTSTR SectionName) 
{
	char	dat[4096]{};

	return (GetPrivateProfileString(SectionName, NULL, NULL, dat, sizeof(dat), m_file) != 0);
}

long CComponent::_ReadInteger(LPCTSTR Section, LPCTSTR Ident, long Default) 
{
	return GetPrivateProfileInt(Section, Ident, Default, m_file);
}

BSTR CComponent::_ReadString(LPCTSTR Section, LPCTSTR Ident, LPCTSTR Default) 
{
	CString strResult(Default);
	char	dat[256]{};

	GetPrivateProfileString(Section, Ident, Default, dat, sizeof(dat), m_file);
	strResult = dat;

	return strResult.AllocSysString();
}

double CComponent::_ReadFloat(LPCTSTR Section, LPCTSTR Ident, double Default) 
{
	char	dat[256]{};
	CString str;

	str.Format("%f", Default);
	GetPrivateProfileString(Section, Ident, str, dat, sizeof(dat), m_file);
	str = dat;

	return atof(str);
}

BOOL CComponent::_ReadBool(LPCTSTR Section, LPCTSTR Ident, BOOL Default) 
{
	const	int	ival = GetPrivateProfileInt(Section, Ident, Default, m_file);	
	return (ival == 1);
}

void CComponent::_WriteFloat(LPCTSTR Section, LPCTSTR Ident, double Value) 
{
	CString dat;

	dat.Format("%f", Value);
	WritePrivateProfileString(Section, Ident, dat, m_file);
}

void CComponent::_WriteInteger(LPCTSTR Section, LPCTSTR Ident, long Value) 
{
	CString dat;

	dat.Format("%d", Value);
	WritePrivateProfileString(Section, Ident, dat, m_file);
}

void CComponent::_WriteString(LPCTSTR Section, LPCTSTR Ident, LPCTSTR Value) 
{
	WritePrivateProfileString(Section, Ident, Value, m_file);
}

void CComponent::_WriteBoolean(LPCTSTR Section, LPCTSTR Ident, BOOL Value) 
{
	CString dat;

	dat.Format("%d", Value);
	WritePrivateProfileString(Section, Ident, dat, m_file);
}

void CComponent::_Close() 
{
	m_file = "";
}

