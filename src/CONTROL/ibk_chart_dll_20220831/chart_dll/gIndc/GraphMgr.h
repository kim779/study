// GraphMgr.h: interface for the CGraphMgr class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GRAPHMGR_H__30E3AFE4_2244_4421_8799_0B168CF42C79__INCLUDED_)
#define AFX_GRAPHMGR_H__30E3AFE4_2244_4421_8799_0B168CF42C79__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "IndcBase.h"
#include "VarsBase.h"

class AFX_EXT_CLASS CGraphMgr  
{
public:
	CGraphMgr() {}
	virtual ~CGraphMgr() {}

	static CIndcBase* CreateGraph(CWnd* pwndView, CWnd* pwndPnChart, class CDataMgr* pDataMgr, char* pcInfo, char* pcGraph, int iDispDay);
};

#endif // !defined(AFX_GRAPHMGR_H__30E3AFE4_2244_4421_8799_0B168CF42C79__INCLUDED_)
