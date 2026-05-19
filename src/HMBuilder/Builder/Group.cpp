// Group.cpp: implementation of the CGroup class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "axisMBuilder.h"
#include "Group.h"

#include "h/mainvar.h"
#include "amCc/LibBuild.h"
//#include "awSock64/download.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CGroup::CGroup()
{
	m_sGroup = "1-group1;";
	m_groupArr.RemoveAll();
}

CGroup::~CGroup()
{
	m_groupArr.RemoveAll();
}

BEGIN_MESSAGE_MAP(CGroup, CWnd)
	//{{AFX_MSG_MAP(CGroup)
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CGroup message handlers

void CGroup::loadFile()
{
	m_sGroup.Empty();
	for (int ii = 1;;ii++)
	{
		CString tmp, sFullPath;
		char twb[256];
		tmp.Format("%02d", ii);
		wccSetTempPath(sFullPath, "mapgroup.ini");
		GetPrivateProfileString("MAPGROUP", tmp, "", twb, 256, sFullPath);
		if (!strlen(twb))
			break;
		tmp.Format("%d-%s;", ii, twb);
		m_groupArr.Add(twb);
		m_sGroup += tmp;
	}
	AfxGetMainWnd()->SendMessage(WM_USER, ID_USR_SETGROUP, 0);
}

int CGroup::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	/*m_groupArr.RemoveAll();

	CString tmps;
	CDownload* dn = new CDownload();
	wccSetTempPath(tmps, "mapgroup.ini");
	if (!dn->Create())
	{
		delete dn;
		return -1;
	}
	if (!dn->Download("work/mapgroup.ini", tmps, 4))
	{
		delete dn;
		return -1;
	}
	delete dn;
	loadFile();*/
	
	return 0;
}

CString CGroup::getGroup()
{
	return m_sGroup;
}

CString CGroup::getGroup(int idx)
{
	if (idx > m_groupArr.GetSize() || idx == 0)
		return "";
	return m_groupArr.GetAt(idx - 1);
}
