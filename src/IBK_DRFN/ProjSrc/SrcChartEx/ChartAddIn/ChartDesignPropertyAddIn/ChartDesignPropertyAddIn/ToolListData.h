// ToolListData.h: interface for the Tool List Data Info.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TOOLLISTDATA_H__A45D3973_6237_452B_85CA_5BFA4E0C4DF4__INCLUDED_)
#define AFX_TOOLLISTDATA_H__A45D3973_6237_452B_85CA_5BFA4E0C4DF4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "../Include_Chart/Dll_Load/IMetaTable.h"		// for IMetaTable

//data
extern const int NUM_PACKETNAMEITEM;
extern IMetaTable::E_METACODE ePacketNameItem[];

// Image
const int ROOT = 0;
const int BLOCK = 1;
const int VERT_SCALE = 2;
const int GRAPH = 3;
const int CHILD = 4;

#endif // !defined(AFX_TOOLLISTDATA_H__A45D3973_6237_452B_85CA_5BFA4E0C4DF4__INCLUDED_)
