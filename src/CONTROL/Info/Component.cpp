// Component.cpp : implementation file
//

#include "stdafx.h"
#include "Info.h"
#include "Component.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CComponent

IMPLEMENT_DYNCREATE(CComponent, CCmdTarget)

CComponent::CComponent()
{
	EnableAutomation();
	m_sRoot.Empty();
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
	DISP_FUNCTION(CComponent, "GetInfoData", GetInfoData, VT_BSTR, VTS_I4 VTS_BSTR VTS_BSTR)
	DISP_FUNCTION(CComponent, "PopData", PopData, VT_BSTR, VTS_BSTR VTS_BSTR VTS_BSTR VTS_BSTR)
	DISP_FUNCTION(CComponent, "PushData", PushData, VT_EMPTY, VTS_BSTR VTS_BSTR VTS_BSTR VTS_BSTR VTS_BSTR)
	DISP_FUNCTION(CComponent, "Pop", Pop, VT_BSTR, VTS_BSTR VTS_BSTR)
	DISP_FUNCTION(CComponent, "Push", Push, VT_EMPTY, VTS_BSTR VTS_BSTR VTS_BSTR)
	//}}AFX_DISPATCH_MAP
END_DISPATCH_MAP()

// Note: we add support for IID_IComponent to support typesafe binding
//  from VBA.  This IID must match the GUID that is attached to the 
//  dispinterface in the .ODL file.

// {983D6E36-76BE-439C-A2BE-C560978D446B}
static const IID IID_IComponent =
{ 0x983d6e36, 0x76be, 0x439c, { 0xa2, 0xbe, 0xc5, 0x60, 0x97, 0x8d, 0x44, 0x6b } };

BEGIN_INTERFACE_MAP(CComponent, CCmdTarget)
	INTERFACE_PART(CComponent, IID_IComponent, Dispatch)
END_INTERFACE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CComponent message handlers

BSTR CComponent::GetInfoData(long nKind, LPCTSTR sFileName, LPCTSTR sSearchKey) 
{
	CString strResult;
	
	switch (nKind)
	{
	case 1:		// [FOLDER] Section
		strResult = LoadFolder(m_sRoot + "\\tab\\" + sFileName, "FOLDER", sSearchKey);
		break;
	}
	return strResult.AllocSysString();
}

CString CComponent::LoadFolder(CString path, CString sSection, CString sSearch)
{
	char	wb[2048]{};
	CString	tmps, sRtn = _T(""), sTmp;
	int	nIndex = 0;

	sSearch.Trim();
 	for (int ii = 0; ii < 100; ii++)
	{
		tmps.Format("%02d", ii);
		GetPrivateProfileString(sSection, tmps, "", wb, sizeof(wb), path);
		tmps = wb;
		
		if (tmps.IsEmpty())
			break;

		sTmp = Parser(tmps, ";");
		nIndex = atoi(tmps);

		tmps = sTmp;
		tmps.Replace("\t", " ");
		if (!sSearch.IsEmpty() && tmps.Find(sSearch) < 0)
			continue;
		
		sTmp.Format("%02d %s\t", nIndex, tmps);
		sRtn += sTmp;
	}
	return sRtn;
}

CString CComponent::Parser(CString &srcstr, CString substr)
{
	if (srcstr.Find(substr) == -1)
	{
		CString temp = srcstr;
		srcstr.Empty();
			return temp;
	}

	CString  temp = srcstr.Left(srcstr.Find(substr));
	srcstr = srcstr.Mid(srcstr.Find(substr) + substr.GetLength());
	return temp;
}

// ljh
BSTR CComponent::PopData(LPCTSTR sUserName, LPCTSTR sIniFileName, LPCTSTR sSectionName, LPCTSTR sKey) 
{
	CString strResult, username = sUserName;

	if (username.IsEmpty())
		strResult = ReadFile(m_sRoot + "\\TAB\\" + sIniFileName, sSectionName, sKey);
	else
		strResult = ReadFile(m_sRoot + "\\USER\\" + sUserName + "\\" + sIniFileName, sSectionName, sKey);

	return strResult.AllocSysString();
}

void CComponent::PushData(LPCTSTR sUserName, LPCTSTR sIniFileName, LPCTSTR sSectionName, LPCTSTR sKey, LPCTSTR sData) 
{
	CString username = sUserName;

	if (username.IsEmpty())
		SaveFile(m_sRoot + "\\TAB\\" + sIniFileName, sSectionName, sKey, sData);
	else
		SaveFile(m_sRoot + "\\USER\\" + sUserName + "\\" + sIniFileName, sSectionName, sKey, sData);
}

CString CComponent::ReadFile(CString path, CString sSection, CString sKey)
{
	char	wb[1024]{};
	CString	tmps, sRtn = _T("");

	GetPrivateProfileString(sSection, sKey, "", wb, sizeof(wb), path);
	tmps = wb;
	
	if (!tmps.IsEmpty())
		sRtn = tmps;

	return sRtn;
}

void CComponent::SaveFile(CString path, CString sSection, CString sKey, CString sData)
{
	WritePrivateProfileString(sSection, sKey, sData, path);
	WritePrivateProfileString(NULL, NULL, NULL, path);
}

BSTR CComponent::Pop(LPCTSTR sSectionName, LPCTSTR sKey) 
{
	CString strResult, sectionName = sSectionName;

	if (sectionName.IsEmpty())
		strResult = ReadFile(m_sRoot + "\\TAB\\" + "usersave.ini", sSectionName, sKey);
	else
		strResult = ReadFile(m_sRoot + "\\USER\\" + "usersave.ini", sSectionName, sKey);

	return strResult.AllocSysString();
}

void CComponent::Push(LPCTSTR sSectionName, LPCTSTR sKey, LPCTSTR sData) 
{
	CString sectionName = sSectionName;

	if (sectionName.IsEmpty())
		SaveFile(m_sRoot + "\\TAB\\" + "usersave.ini", sSectionName, sKey, sData);
	else
		SaveFile(m_sRoot + "\\USER\\" + "usersave.ini", sSectionName, sKey, sData);
}
