// ObjTool.cpp: implementation of the CObjTool class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ObjTool.h"
#include "BlockBaseData.h"

#include "../Include_Chart/Dll_Load/IMetaTable.h"		// for IMetaTable

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CObjTool::CObjTool(CString strName, CString strOption1, CString strOption2)
{
	m_strName = strName;
	m_str1stOption = strOption1;
	m_str2ndOption = strOption2;

	SetType(strName);
}

CObjTool::~CObjTool()
{

}

void CObjTool::SetType(const CString &strName)
{
	int nIndex = GetToolOptionIndex(strName);
	m_nType = (CToolOptionInfo::TOOLOPTION)nIndex;
}

int CObjTool::GetToolOptionIndex(const CString &strName)
{
	for(int i = 0 ; i < IMetaTable::m_nToolOptionCount; i++)
	{
		if( _MTEXT( m_eToolOptions[i]) == strName)
			return i;
	}
	return -1;
}
