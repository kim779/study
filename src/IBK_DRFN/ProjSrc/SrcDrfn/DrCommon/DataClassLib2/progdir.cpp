// ChartItemData.cpp: implementation of the CChartItemData class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "progdir.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

CString CProgDir::m_strPathRoot;
CString CProgDir::m_strPathImg;
CString CProgDir::m_strPathBin;
CString CProgDir::m_strPathData;

//////////////////////////////////////////////////////////////////////
// CProgDir
//////////////////////////////////////////////////////////////////////
CProgDir::CProgDir()
{
	MakeDir();
}

CProgDir::~CProgDir()
{
}

void CProgDir::MakeDir()
{
	CString strPath;
	GetModuleFileName(AfxGetApp()->m_hInstance, strPath.GetBufferSetLength(128), 128);
	strPath.ReleaseBuffer();
	int nFind = strPath.ReverseFind('\\');
	if (nFind >= 0)
	{
		CString	strPath2 = strPath.Left(nFind);
		int nFind2 = strPath2.ReverseFind('\\');
		if (nFind2 >= 0) 
			strPath = strPath2.Left(nFind2);
		else 
			strPath = strPath.Left(nFind);
	}

	m_strPathRoot	= strPath;
	m_strPathImg	= strPath + "\\Img";
	m_strPathBin	= strPath + "\\Bin";
	m_strPathData	= strPath + "\\Data";
}

