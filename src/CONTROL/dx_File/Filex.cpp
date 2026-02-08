// Filex.cpp : implementation file
//

#include "stdafx.h"
#include "DX_File.h"
#include "Filex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CFilex

[[gsl::suppress(26409)]]
IMPLEMENT_DYNCREATE(CFilex, CCmdTarget)

CFilex::CFilex()
{
	EnableAutomation();
}

CFilex::~CFilex()
{
}


void CFilex::OnFinalRelease()
{
	// When the last reference for an automation object is released
	// OnFinalRelease is called.  The base class will automatically
	// deletes the object.  Add additional cleanup required for your
	// object before calling the base class.

	CCmdTarget::OnFinalRelease();
}


BEGIN_MESSAGE_MAP(CFilex, CCmdTarget)
	//{{AFX_MSG_MAP(CFilex)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BEGIN_DISPATCH_MAP(CFilex, CCmdTarget)
	//{{AFX_DISPATCH_MAP(CFilex)
	DISP_PROPERTY_EX(CFilex, "Name", GetName, SetNotSupported, VT_BSTR)
	DISP_FUNCTION(CFilex, "Open", _Open, VT_BOOL, VTS_BSTR VTS_BSTR)
	DISP_FUNCTION(CFilex, "Read", _Read, VT_BSTR, VTS_I4)
	DISP_FUNCTION(CFilex, "Seek", _Seek, VT_I4, VTS_I4)
	DISP_FUNCTION(CFilex, "Close", _Close, VT_EMPTY, VTS_NONE)
	DISP_FUNCTION(CFilex, "Write", _Write, VT_EMPTY, VTS_BSTR)
	//}}AFX_DISPATCH_MAP
END_DISPATCH_MAP()

// Note: we add support for IID_IFilex to support typesafe binding
//  from VBA.  This IID must match the GUID that is attached to the 
//  dispinterface in the .ODL file.

// {5B749E8C-DF77-448D-94DA-A412D47B279F}
static const IID IID_IFilex =
{ 0x5b749e8c, 0xdf77, 0x448d, { 0x94, 0xda, 0xa4, 0x12, 0xd4, 0x7b, 0x27, 0x9f } };

BEGIN_INTERFACE_MAP(CFilex, CCmdTarget)
	INTERFACE_PART(CFilex, IID_IFilex, Dispatch)
END_INTERFACE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFilex message handlers

BOOL CFilex::_Open(LPCTSTR mode, LPCTSTR FileN) 
{
	CString	string;
	BOOL	open = FALSE;
	UINT	flag = CFile::typeBinary | CFile::shareDenyNone;

	string = CString(mode);
	string.MakeLower();
	if (string.Find('r') >= 0)
	{
		open = TRUE;
		flag |= CFile::modeRead;
	}
	if (string.Find('w') >= 0)
	{
		open = TRUE;
		flag |= CFile::modeNoTruncate|CFile::modeWrite;
	}
	if (string.Find('c') >= 0)
		flag |= CFile::modeCreate|CFile::modeReadWrite;

	string = CString(FileN);
	if (string.IsEmpty())
	{
		CFileDialog fDlg(open);
		if (fDlg.DoModal() != IDOK)
			return FALSE;

		string = fDlg.GetPathName();
	}

	if (m_File.Open(string, flag))
	{
		m_name = string;
		return TRUE;
	}
	return FALSE;
}

BSTR CFilex::_Read(long size) 
{
	CString string, strResult = _T("");
	char	wb[1024 * 4]{};

	for (int ii = 0; ii < size; ii++)
	{
		if (m_File.ReadString(wb, sizeof(wb)-1))
			strResult += wb;
	}
	return strResult.AllocSysString();
}

void CFilex::_Write(LPCTSTR text) 
{
	CString	string = CString(text);
	const	int	length = string.GetLength();

	m_File.Write(string, length);
}

long CFilex::_Seek(long to) 
{
	if (to)
		return (long)m_File.SeekToEnd();

	m_File.SeekToBegin();
	return 0;
}

void CFilex::_Close() 
{
	m_name.Empty();
	m_File.Close();
}

BSTR CFilex::GetName() 
{
	return m_name.AllocSysString();
}
