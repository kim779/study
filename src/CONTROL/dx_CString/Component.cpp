// Component.cpp : implementation file
//

#include "stdafx.h"
#include "DX_CString.h"
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
	DISP_PROPERTY_EX(CComponent, "Text", GetText, SetText, VT_BSTR)
	DISP_FUNCTION(CComponent, "GetLength", GetLength, VT_I4, VTS_NONE)
	DISP_FUNCTION(CComponent, "Empty", Empty, VT_EMPTY, VTS_NONE)
	DISP_FUNCTION(CComponent, "IsEmpty", IsEmpty, VT_BOOL, VTS_NONE)
	DISP_FUNCTION(CComponent, "MakeUpper", MakeUpper, VT_EMPTY, VTS_NONE)
	DISP_FUNCTION(CComponent, "MakeLower", MakeLower, VT_EMPTY, VTS_NONE)
	DISP_FUNCTION(CComponent, "Replace", Replace, VT_I4, VTS_BSTR VTS_BSTR)
	DISP_FUNCTION(CComponent, "Mid", Mid, VT_BSTR, VTS_I4 VTS_I4)
	DISP_FUNCTION(CComponent, "Right", Right, VT_BSTR, VTS_I4)
	DISP_FUNCTION(CComponent, "Left", Left, VT_BSTR, VTS_I4)
	DISP_FUNCTION(CComponent, "Insert", Insert, VT_I4, VTS_I4 VTS_BSTR)
	DISP_FUNCTION(CComponent, "Delete", Delete, VT_I4, VTS_I4 VTS_I4)
	DISP_FUNCTION(CComponent, "Compare", Compare, VT_I4, VTS_BSTR)
	DISP_FUNCTION(CComponent, "CompareNoCase", CompareNoCase, VT_I4, VTS_BSTR)
	DISP_FUNCTION(CComponent, "GetAt", GetAt, VT_I2, VTS_I4)
	DISP_FUNCTION(CComponent, "SetAt", SetAt, VT_EMPTY, VTS_I4 VTS_I2)
	DISP_FUNCTION(CComponent, "TrimLeft", TrimLeft, VT_EMPTY, VTS_NONE)
	DISP_FUNCTION(CComponent, "TrimRight", TrimRight, VT_EMPTY, VTS_NONE)
	DISP_FUNCTION(CComponent, "Find", Find, VT_I4, VTS_BSTR VTS_I4)
	//}}AFX_DISPATCH_MAP
END_DISPATCH_MAP()

// Note: we add support for IID_IComponent to support typesafe binding
//  from VBA.  This IID must match the GUID that is attached to the 
//  dispinterface in the .ODL file.

// {853778F9-6738-47BE-991C-AD90891F75D3}
static const IID IID_IComponent =
{ 0x853778f9, 0x6738, 0x47be, { 0x99, 0x1c, 0xad, 0x90, 0x89, 0x1f, 0x75, 0xd3 } };

BEGIN_INTERFACE_MAP(CComponent, CCmdTarget)
	INTERFACE_PART(CComponent, IID_IComponent, Dispatch)
END_INTERFACE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CComponent message handlers

BSTR CComponent::GetText() 
{
	CString strResult = m_String;

	return strResult.AllocSysString();
}

void CComponent::SetText(LPCTSTR lpszNewValue) 
{
	m_String = lpszNewValue;

}

long CComponent::GetLength() 
{
	return m_String.GetLength();
}

void CComponent::Empty() 
{
	m_String.Empty();
}

BOOL CComponent::IsEmpty() 
{
	return m_String.IsEmpty();
}

void CComponent::MakeUpper() 
{
	m_String.MakeUpper();
}

void CComponent::MakeLower() 
{
	m_String.MakeLower();
}

long CComponent::Replace(LPCTSTR oldStr, LPCTSTR newStr) 
{
	return m_String.Replace(oldStr, newStr);
}

BSTR CComponent::Mid(long nFirst, long nCount) 
{
	CString strResult = m_String.Mid(nFirst, nCount);

	return strResult.AllocSysString();
}

BSTR CComponent::Right(long nCount) 
{
	CString strResult = m_String.Right(nCount);

	return strResult.AllocSysString();
}

BSTR CComponent::Left(long nCount) 
{
	CString strResult = m_String.Left(nCount);

	return strResult.AllocSysString();
}

long CComponent::Insert(long nIndex, LPCTSTR Str) 
{
	return m_String.Insert(nIndex, Str);
}

long CComponent::Delete(long nIndex, long nCount) 
{
	return m_String.Delete(nIndex, nCount);
}

long CComponent::Compare(LPCTSTR Str) 
{
	return m_String.Compare(Str);
}

long CComponent::CompareNoCase(LPCTSTR Str) 
{
	return m_String.CompareNoCase(Str);
}

short CComponent::GetAt(long nIndex) 
{
	return (short)m_String.GetAt(nIndex);
}

void CComponent::SetAt(long nIndex, short nChar) 
{
	m_String.SetAt(nIndex, (char)nChar);
}

void CComponent::TrimLeft() 
{
	m_String.TrimLeft();
}

void CComponent::TrimRight() 
{
	m_String.TrimRight();
}

long CComponent::Find(LPCTSTR pszSub, long iStart) 
{
	return m_String.Find(pszSub, iStart);
}
