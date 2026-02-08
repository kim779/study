// GraphSearchInfo.h: interface for the CGraphSearchInfo class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GRAPHSEARCHINFO_H__FD9ECB4F_8B8C_4DA3_9E27_804B82249DF1__INCLUDED_)
#define AFX_GRAPHSEARCHINFO_H__FD9ECB4F_8B8C_4DA3_9E27_804B82249DF1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "../Include_Chart/ScaleBaseData.h"			// for CScaleBaseData

typedef enum
{
	GST_NONE,
	GST_INDICATORNAME,
	GST_GRAPHNAME,
	GST_SUBGRAPHTITLE,
} EGraphSearchType;

class CGraphImp;
class CBlockImp;
class CSubGraph;
class CBlocksColumn;
class CVertScaleGroup;
class CGraphSearchInfo  
{
public:
	CGraphSearchInfo();
	void				ResetInfo( void);

// Check Routine
public:
	virtual BOOL		FindGraph( const CBlocksColumn *p_pBlocksColumn); 
	virtual BOOL		FindGraph( LPVOID pPacketRQ, const CBlockImp *p_pBlockImp);	// RQ로 검색 : 복수종목 - ojtaso (20070118)
	virtual BOOL		FindGraph( const CBlockImp *p_pBlockImp);
	// (2008/9/15 - Seung-Won, Bae) RQ Search
	virtual BOOL		FindGraph( LPVOID pPacketRQ, const CVertScaleGroup *p_pVertScaleGroup);
	virtual BOOL		FindGraph( const CVertScaleGroup *p_pVertScaleGroup);
	virtual BOOL		FindGraph( const CGraphImp *p_pGraph);
	virtual BOOL		FindGraph( const CSubGraph *p_pSubGraph);

// Search Position and Index
public:
	POSITION			m_posColumnBlock;
	POSITION			m_posRowBlock;
	POSITION			m_posVertScaleGroup;
	POSITION			m_posGraph;
	POSITION			m_posSubGraph;
	int					m_nColumnBlockIndex;
	int					m_nRowBlockIndex;
	int					m_nVertScaleGroupIndex;
	int					m_nGraphIndex;
	int					m_nSubGraphIndex;

// Output Pointer
public:
	CBlocksColumn *		m_pBlocksColumn;
	CBlockImp *			m_pBlockImp;
	CVertScaleGroup *	m_pVertScaleGroup;
	CGraphImp *			m_pGraphImp;
	CSubGraph *			m_pSubGraph;

// (2007/4/12 - Seung-Won, Bae) Support Next Object Search.
public:
	void				SetNextStartColumnBlock( void);
	void				SetNextStartRowBlock( void);
	void				SetNextStartVerticalScaleGroup( void);
	void				SetNextStartGraph( void);
	void				SetNextStartSubGraph( void);
};

class CGraphSearchInfo_BlocksColumn : public CGraphSearchInfo
{
// Check Routine
public:
	virtual BOOL		FindGraph( const CBlocksColumn *p_pBlocksColumn);
};

class CGraphSearchInfo_Block : public CGraphSearchInfo
{
// Check Routine
public:
	virtual BOOL		FindGraph( const CBlockImp *p_pBlockImp);
};

class CGraphSearchInfo_OtherBlock : public CGraphSearchInfo
{
// Check Routine
public:
	virtual BOOL		FindGraph( const CBlockImp *p_pBlockImp);
};

class CGraphSearchInfo_DoubleSizeBlock : public CGraphSearchInfo
{
// Check Routine
public:
	virtual BOOL		FindGraph( const CBlockImp *p_pBlockImp);
};

class CGraphSearchInfo_VerticalScaleGroup : public CGraphSearchInfo
{
// Check Routine
public:
	virtual BOOL		FindGraph( const CVertScaleGroup *p_pVertScaleGroup);
};

class CGraphSearchInfo_InvertVSCaleGroup : public CGraphSearchInfo
{
// Search Key
public:
	BOOL				m_bInvertType;

// Check Routine
public:
	virtual BOOL		FindGraph( const CVertScaleGroup *p_pVertScaleGroup);
};

class CGraphSearchInfo_HasVertScale : public CGraphSearchInfo
{
// Search Key
public:
	CScaleBaseData::VERTSCALEPOSITION	m_eVertScalePosition;	// if -1, Any Visible Vertical Search. (Left, Right, Both)

// Check Routine
public:
	virtual BOOL		FindGraph( const CVertScaleGroup *p_pVertScaleGroup);
};

class CGraphSearchInfo_HasVertScaleAndGridLine : public CGraphSearchInfo_HasVertScale
{
// Check Routine
public:
	virtual BOOL		FindGraph( const CVertScaleGroup *p_pVertScaleGroup);
};

class CGraphSearchInfo_ActiveVertScaleAndGridLine : public CGraphSearchInfo
{
public:
	CGraphSearchInfo_ActiveVertScaleAndGridLine()	{	m_nActiveFlag = -1;	}

public:
	int					m_nActiveFlag;

// Check Routine
public:
	virtual BOOL		FindGraph( const CVertScaleGroup *p_pVertScaleGroup);
};

class CGraphSearchInfo_NotOneGraphVScaleGroup : public CGraphSearchInfo
{
// Check Routine
public:
	virtual BOOL		FindGraph( const CVertScaleGroup *p_pVertScaleGroup);
};

class CGraphSearchInfo_AnyGraph : public CGraphSearchInfo
{
// Check Routine
public:
	virtual BOOL		FindGraph( const CGraphImp *p_pGraph);
};

class CGraphSearchInfo_AnyGraphWithRQ : public CGraphSearchInfo
{
// Search Key
public:
	CString				m_strRQ;

// Check Routine
public:
	virtual BOOL		FindGraph( const CGraphImp *p_pGraph);
};

class CGraphSearchInfo_IndicatorName : public CGraphSearchInfo
{
// Search Key
public:
	CString				m_strIndicatorName;

// Check Routine
public:
	virtual BOOL		FindGraph( const CGraphImp *p_pGraph);
};

class CIndicatorInfo;
class CGraphSearchInfo_IndicatorObject : public CGraphSearchInfo
{
public:
	CGraphSearchInfo_IndicatorObject::CGraphSearchInfo_IndicatorObject()
	{
		m_pIndicatorInfo	= NULL;
		m_pExceptGraph		= NULL;
	}
// Search Key
public:
	const CIndicatorInfo *	m_pIndicatorInfo;
	const CGraphImp *			m_pExceptGraph;

// Check Routine
public:
	virtual BOOL		FindGraph( const CGraphImp *p_pGraph);
};

class CGraphSearchInfo_IndicatorNameWithRQ : public CGraphSearchInfo_IndicatorName
{
// Search Key
public:
	CString				m_strRQ;

// Check Routine
public:
	virtual BOOL		FindGraph( const CGraphImp *p_pGraph);
};

class CGraphSearchInfo_GraphName : public CGraphSearchInfo
{
// Search Key
public:
	CString				m_strGraphName;

// Check Routine
public:
	virtual BOOL		FindGraph( const CGraphImp *p_pGraph);
};

// RQ별 찾기 : 복수종목 - ojtaso (20070306)
class CGraphSearchInfo_GraphNameWithRQ : public CGraphSearchInfo_GraphName
{
// Search Key
public:
	CString				m_strRQ;

// Check Routine
public:
	virtual BOOL		FindGraph( const CGraphImp *p_pGraph);
};

class CGraphSearchInfo_GraphNameAndSubGraphName : public CGraphSearchInfo_GraphName
{
// Search Key
public:
	CString				m_strSubGraphName;

// Check Routine
public:
	virtual BOOL		FindGraph( const CGraphImp *p_pGraph);
	virtual BOOL		FindGraph( const CSubGraph *p_pSubGraph);
};

class CGraphSearchInfo_GraphNameAndSubGraphIndex : public CGraphSearchInfo_GraphName
{
// Search Key
public:
	int					m_nSubGraphIndex;

// Check Routine
public:
	virtual BOOL		FindGraph( const CGraphImp *p_pGraph);
};

class CGraphSearchInfo_SubGraphTitle : public CGraphSearchInfo
{
// Search Key
public:
	CString				m_strSubGraphTitle;

// Check Routine
public:
	virtual BOOL		FindGraph( const CSubGraph *p_pSubGraph);
};

class CGraphSearchInfo_VisibleSubGraph : public CGraphSearchInfo
{
// Check Routine
public:
	virtual BOOL		FindGraph( const CSubGraph *p_pSubGraph);
};

class CGraphSearchInfo_VisibleGraphTitle : public CGraphSearchInfo_VisibleSubGraph
{
// Check Routine
public:
	virtual BOOL		FindGraph( const CSubGraph *p_pSubGraph);
};

class CGraphSearchInfo_GraphType : public CGraphSearchInfo
{
// Search Key
public:
	CGraphBaseData::GRAPHTYPE	m_eGraphType;

// Check Routine
public:
	virtual BOOL		FindGraph( const CSubGraph *p_pSubGraph);
};

class CGraphSearchInfo_TypeStyleDrawStyle : public CGraphSearchInfo_GraphType
{
// Search Key
public:
	int							m_nGraphStyle;
	int							m_nDrawStyle;

// Check Routine
public:
	virtual BOOL		FindGraph( const CSubGraph *p_pSubGraph);
};

class CGraphSearchInfo_VisibleTypeStyleDrawStyle : public CGraphSearchInfo_TypeStyleDrawStyle
{
// Check Routine
public:
	virtual BOOL		FindGraph( const CSubGraph *p_pSubGraph);
};

// 한번에 그래프 속성 변경 가능한 메쏘드 추가 - ojtaso (20080602)
class CGraphSearchInfo_SubGraphTitleWithRQ : public CGraphSearchInfo_SubGraphTitle
{
// Search Key
public:
	CString				m_strRQ;

// Check Routine
public:
	virtual BOOL		FindGraph( const CSubGraph *p_pSubGraph);
};

#endif // !defined(AFX_GRAPHSEARCHINFO_H__FD9ECB4F_8B8C_4DA3_9E27_804B82249DF1__INCLUDED_)
