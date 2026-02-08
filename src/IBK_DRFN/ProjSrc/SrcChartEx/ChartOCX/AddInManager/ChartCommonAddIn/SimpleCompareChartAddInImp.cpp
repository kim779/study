// SimpleCompareChartAddInImp.cpp: implementation of the CSimpleCompareChartAddInImp class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ChartCommonAddIn.h"
#include "SimpleCompareChartAddInImp.h"

#include "../Include_AddIn/I000000/_IString.h"				// for ILPCSTR
#include "../Include_AddIn/I000000/_IChartOCX.h"			// for IChartOCX
#include "../Include_AddIn/I000000/_IChartManager.h"		// for IChartManager

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

#define _BLOCK_COLUMN_HEADER		"BlockColumn;\r\n"
#define _BLOCK_COLUMN_FOOTER		"BlockColumnEnd;\r\n"
#define _BLOCK_HEADER				"Block;\r\n"
#define _BLOCK_FOOTER				"BlockEnd;\r\n"
#define _VERT_SCALE_GROUP_HEADER	"VertScaleGroup;\r\n"
#define _VERT_SCALE_GROUP_FOOTER	"VertScaleGroupEnd;\r\n"
#define _GRAPH_HEADER				"Graph;\r\n"
#define _GRAPH_FOOTER				"GraphEnd;\r\n"

CSimpleCompareChartAddInImp::CSimpleCompareChartAddInImp( IChartOCX *p_pIChartOCX, IAddInDllBase *p_pIAddInDllBase)
	: CChartAddInBase( p_pIChartOCX, p_pIAddInDllBase)
{
	// (2009/6/4 - Seung-Won, Bae) Parse and Generate ChartListInfo.
	// 1. Meta Info
	m_nBlockColumnHeaderLen		= strlen( _BLOCK_COLUMN_HEADER);
	m_nBlockColumnFooterLen		= strlen( _BLOCK_COLUMN_FOOTER);
	m_nBlockHeaderLen			= strlen( _BLOCK_HEADER);
	m_nBlockFooterLen			= strlen( _BLOCK_FOOTER);
	m_nVertScaleGroupHeaderLen	= strlen( _VERT_SCALE_GROUP_HEADER);
	m_nVertScaleGroupFooterLen	= strlen( _VERT_SCALE_GROUP_FOOTER);
	m_nGraphHeaderLen			= strlen( _GRAPH_HEADER);
	m_nGraphFooterLen			= strlen( _GRAPH_FOOTER);
	// 2. User Setting
	m_nChartCompareType = 0;
	m_nBlockInfo2 = -1;
}

CSimpleCompareChartAddInImp::~CSimpleCompareChartAddInImp()
{
}

// (2006/2/15 - Seung-Won, Bae) Multiple Item in DLL
CString CSimpleCompareChartAddInImp::m_strAddInItemName = "SIMPLE_COMPARE";

//////////////////////////////////////////////////////////////////////
// Define AddIn Event Map
//////////////////////////////////////////////////////////////////////
BEGIN_ADDIN_EVENT_MAP( CSimpleCompareChartAddInImp)
END_ADDIN_EVENT_MAP()

// [05/01/19] AddIn Object가 비정형 Interface를 제공할 수 있도록 제공한다.
BOOL CSimpleCompareChartAddInImp::InvokeAddIn( int p_nCommandType, long p_lData)
{
	if( p_nCommandType != 0) return FALSE;

	// (2009/6/4 - Seung-Won, Bae) Parse and Generate ChartListInfo.
	GetAndParseChartListInfo();
	m_nChartCompareType = p_lData;
	GenerateAndSetChartListInfo();

	return TRUE;
}
BOOL CSimpleCompareChartAddInImp::InvokeAddInStr( LPCTSTR p_szCommandName, LPCTSTR p_szData)
{
	if( CString( "TITLE") != p_szCommandName) return FALSE;

	// (2009/6/4 - Seung-Won, Bae) Parse and Generate ChartListInfo.
	GetAndParseChartListInfo();
	CString strTitles( p_szData);
	m_slGraphTitle.RemoveAll();
	int nIndex = strTitles.Find( '|');
	while( 0 <= nIndex)
	{
		CString strTitle = strTitles.Left( nIndex);
		m_slGraphTitle.AddTail( strTitle);
		strTitles = strTitles.Mid( strTitle.GetLength() + 1);
		nIndex = strTitles.Find( '|');
	}
	GenerateAndSetChartListInfo();

	return TRUE;
}

// (2009/6/4 - Seung-Won, Bae) Parse and Generate ChartListInfo.
void CSimpleCompareChartAddInImp::GetAndParseChartListInfo( void)
{
	ILPCSTR szChartList = m_pIChartOCX->GetChartList();
	CString strChartList = szChartList;

	// 1. Trim BlockColumn Frame.
	strChartList = strChartList.Mid( m_nBlockColumnHeaderLen, strChartList.GetLength() - m_nBlockColumnHeaderLen - m_nBlockColumnFooterLen);

	// 2. Devide to each Block
	CStringList slBlockList;
	m_nBlockInfo2 = 3;
	CString strBlockInfo2;
	while( strChartList.Left( m_nBlockHeaderLen) == _BLOCK_HEADER)
	{
		CString strBlock = strChartList.Left( strChartList.Find( _BLOCK_FOOTER) + m_nBlockFooterLen);
		strChartList = strChartList.Mid( strBlock.GetLength());
		strBlock = strBlock.Mid( m_nBlockHeaderLen, strBlock.GetLength() - m_nBlockHeaderLen - m_nBlockFooterLen);

		CString strBlockInfo = strBlock.Left( strBlock.Find( "\r\n") + 2);
		strBlock = strBlock.Mid( strBlockInfo.GetLength());

		m_strBlockInfo1 = strBlockInfo.Left( strBlockInfo.Find( ';') + 1);
		strBlockInfo2 = strBlockInfo.Mid( m_strBlockInfo1.GetLength());
		m_strBlockInfo3 = strBlockInfo2.Mid( strBlockInfo2.Find( ';'));
		strBlockInfo2 = strBlockInfo2.Left( strBlockInfo2.GetLength() - m_strBlockInfo3.GetLength());
		int nBlockInfo2 = atoi( strBlockInfo2);
		if( 2 < m_nBlockInfo2)										m_nBlockInfo2 = nBlockInfo2;
		else if( m_nBlockInfo2 != nBlockInfo2 && nBlockInfo2 < 3)	m_nBlockInfo2 = 2;

		slBlockList.AddTail( strBlock);
	}

	// 3. Devide to each Vertical Scale Group
	CStringList slVertScaleGroupList;
	POSITION psnBlock = slBlockList.GetHeadPosition();
	while( psnBlock)
	{
		CString strBlock = slBlockList.GetNext( psnBlock);
		while( strBlock.Left( m_nVertScaleGroupHeaderLen) == _VERT_SCALE_GROUP_HEADER)
		{
			CString strVertScaleGroup = strBlock.Left( strBlock.Find( _VERT_SCALE_GROUP_FOOTER) + m_nVertScaleGroupFooterLen);
			strBlock = strBlock.Mid( strVertScaleGroup.GetLength());
			strVertScaleGroup = strVertScaleGroup.Mid( m_nVertScaleGroupHeaderLen, strVertScaleGroup.GetLength() - m_nVertScaleGroupHeaderLen - m_nVertScaleGroupFooterLen);

			m_strVertScaleGroupInfo = strVertScaleGroup.Left( strVertScaleGroup.Find( "\r\n") + 2);
			strVertScaleGroup = strVertScaleGroup.Mid( m_strVertScaleGroupInfo.GetLength());

			slVertScaleGroupList.AddTail( strVertScaleGroup);
		}
	}

	// 4. Devide to each Graph
	m_slGraph1.RemoveAll();
	m_slGraph2.RemoveAll();
	m_slGraphTitle.RemoveAll();
	m_slGraph3.RemoveAll();
	POSITION psnVertScaleGroup = slVertScaleGroupList.GetHeadPosition();
	while( psnVertScaleGroup)
	{
		CString strVertScaleGroup = slVertScaleGroupList.GetNext( psnVertScaleGroup);
		while( strVertScaleGroup.Left( m_nGraphHeaderLen) == _GRAPH_HEADER)
		{
			CString strGraph = strVertScaleGroup.Left( strVertScaleGroup.Find( _GRAPH_FOOTER) + m_nGraphFooterLen);
			strVertScaleGroup = strVertScaleGroup.Mid( strGraph.GetLength());
			strGraph = strGraph.Mid( m_nGraphHeaderLen, strGraph.GetLength() - m_nGraphHeaderLen - m_nGraphFooterLen);

			CString strGraph1 = strGraph.Left( strGraph.Find( "\r\n") + 2);
			strGraph = strGraph.Mid( strGraph1.GetLength());
			int nIndex = strGraph.Find( ';');
			nIndex = strGraph.Find( ';', nIndex + 1);
			nIndex = strGraph.Find( ';', nIndex + 1);
			nIndex = strGraph.Find( ';', nIndex + 1);
			CString strGraph2 = strGraph.Left( nIndex + 1);
			strGraph = strGraph.Mid( strGraph2.GetLength());
			CString strGraphTitle = strGraph.Left( strGraph.Find( ';'));
			strGraph = strGraph.Mid( strGraphTitle.GetLength() + 2);	// + 2 is for hide flag.

			m_slGraph1.AddTail( strGraph1);
			m_slGraph2.AddTail( strGraph2);
			m_slGraphTitle.AddTail( strGraphTitle);
			m_slGraph3.AddTail( strGraph);
		}
	}
}
void CSimpleCompareChartAddInImp::GenerateAndSetChartListInfo( void)
{
	// 4. Devide to each Graph
	CStringList slVertScaleGroupList;
	if( m_nChartCompareType == 0)	// Ratio
	{
		CString strVertScaleGroup;
		POSITION psnGraph1		= m_slGraph1.GetHeadPosition();
		POSITION psnGraph2		= m_slGraph2.GetHeadPosition();
		POSITION psnGraphTitle	= m_slGraphTitle.GetHeadPosition();
		POSITION psnGraph3		= m_slGraph3.GetHeadPosition();
		while( psnGraph1 && psnGraph2 && psnGraphTitle && psnGraph3)
		{
			CString strGraph1		= m_slGraph1.GetNext( psnGraph1);
			CString strGraph2		= m_slGraph2.GetNext( psnGraph2);
			CString strGraphTitle	= m_slGraphTitle.GetNext( psnGraphTitle);
			CString strGraphShow	= ";0";
			CString strGraph3		= m_slGraph2.GetNext( psnGraph3);
			if( strGraphTitle.IsEmpty()) strGraphShow = ";1";

			strVertScaleGroup += _GRAPH_HEADER + strGraph1 + strGraph2 + strGraphTitle + strGraphShow + strGraph3 + _GRAPH_FOOTER;
		}
		slVertScaleGroupList.AddTail( strVertScaleGroup);
	}
	else	// Simple and Overlay
	{
		POSITION psnGraph1		= m_slGraph1.GetHeadPosition();
		POSITION psnGraph2		= m_slGraph2.GetHeadPosition();
		POSITION psnGraphTitle	= m_slGraphTitle.GetHeadPosition();
		POSITION psnGraph3		= m_slGraph3.GetHeadPosition();
		CString strPreGraphs;
		// (2009/6/25 - Seung-Won, Bae) for Hiden Graphs before the first visible Graph.
		BOOL bAddedVisible = FALSE;
		while( psnGraph1 && psnGraph2 && psnGraphTitle && psnGraph3)
		{
			CString strGraph;
			CString strGraph1		= m_slGraph1.GetNext( psnGraph1);
			CString strGraph2		= m_slGraph2.GetNext( psnGraph2);
			CString strGraphTitle	= m_slGraphTitle.GetNext( psnGraphTitle);
			CString strGraphShow	= ";0";
			CString strGraph3		= m_slGraph2.GetNext( psnGraph3);
			if( strGraphTitle.IsEmpty())		strGraphShow = ";1";
			else if( !strPreGraphs.IsEmpty() && bAddedVisible)
			{
				slVertScaleGroupList.AddTail( strPreGraphs);
				strPreGraphs.Empty();
				bAddedVisible = FALSE;
			}

			strGraph = strGraph1 + strGraph2 + strGraphTitle + strGraphShow + strGraph3;
			strGraph = _GRAPH_HEADER + strGraph + _GRAPH_FOOTER;
			strPreGraphs += strGraph;
			if( !strGraphTitle.IsEmpty()) bAddedVisible = TRUE;
		}
		if( !strPreGraphs.IsEmpty())
		{
			slVertScaleGroupList.AddTail( strPreGraphs);
			strPreGraphs.Empty();
		}
	}

	// 3. Devide to each Vertical Scale Group
	CStringList slBlock;
	if( m_nChartCompareType == 0)	// Ratio
	{
		CString strBlock;
		POSITION psnVertScaleGroup = slVertScaleGroupList.GetHeadPosition();
		while( psnVertScaleGroup)
		{
			CString strVertScaleGroup = slVertScaleGroupList.GetNext( psnVertScaleGroup);
			strBlock += strVertScaleGroup;
		}
		strBlock = _VERT_SCALE_GROUP_HEADER + m_strVertScaleGroupInfo + strBlock + _VERT_SCALE_GROUP_FOOTER;
		slBlock.AddTail( strBlock);
	}
	else	// Simple and Overlay
	{
		POSITION psnVertScaleGroup = slVertScaleGroupList.GetHeadPosition();
		while( psnVertScaleGroup)
		{
			CString strVertScaleGroup = slVertScaleGroupList.GetNext( psnVertScaleGroup);
			strVertScaleGroup = _VERT_SCALE_GROUP_HEADER + m_strVertScaleGroupInfo + strVertScaleGroup + _VERT_SCALE_GROUP_FOOTER;
			slBlock.AddTail( strVertScaleGroup);
		}
	}

	// 2. Devide to each Block Info.
	CString strChartList;
	if( m_nChartCompareType == 1)	// Simple
	{
		int	nRowCount = slBlock.GetCount();
		int i = 0;
		POSITION psnBlock = slBlock.GetHeadPosition();
		while( psnBlock)
		{
			CString strBlock = slBlock.GetNext( psnBlock);
			CString strBlockInfo;
			int nBlockInfo2 = 3;
			i++;
			if( 1 == i && ( 0 == m_nBlockInfo2 || 2 == m_nBlockInfo2)) nBlockInfo2 = 0;
			if( i == nRowCount  && ( 1 == m_nBlockInfo2 || 2 == m_nBlockInfo2)) nBlockInfo2 = 1;
			strBlockInfo.Format( "%d", nBlockInfo2);
			strBlockInfo = m_strBlockInfo1 + strBlockInfo + m_strBlockInfo3;
			strBlock = _BLOCK_HEADER + strBlockInfo + strBlock + _BLOCK_FOOTER;
			strChartList += strBlock;
		}
		m_pIChartManager->SetBlockCount( nRowCount, 1);
	}
	else	// Ratio or Overlay
	{
		CString strBlockInfo;
		strBlockInfo.Format( "%d", m_nBlockInfo2);
		strBlockInfo = m_strBlockInfo1 + strBlockInfo + m_strBlockInfo3;
		POSITION psnBlock = slBlock.GetHeadPosition();
		while( psnBlock)
		{
			CString strBlock = slBlock.GetNext( psnBlock);
			strChartList += strBlock;
		}
		strChartList = _BLOCK_HEADER + strBlockInfo + strChartList + _BLOCK_FOOTER;
		m_pIChartManager->SetBlockCount( 1, 1);
	}

	// 1. Trim BlockColumn Frame.
	strChartList = _BLOCK_COLUMN_HEADER + strChartList + _BLOCK_COLUMN_FOOTER;

	m_pIChartOCX->SetChartList( strChartList);
}
