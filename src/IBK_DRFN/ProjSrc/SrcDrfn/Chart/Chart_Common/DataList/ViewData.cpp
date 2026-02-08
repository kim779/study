// ViewData.cpp: implementation of the CViewData class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ViewData.h"

#include <assert.h>

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
// class CSubGraphViewData

CSubGraphViewData& CSubGraphViewData::operator=(const CSubGraphViewData& subGraphViewData)
{
	if(this == &subGraphViewData)
		return *this;

	m_TYPEGraph = subGraphViewData.GetGraphType();
	m_graphStyle = subGraphViewData.GetGraphStyle();
	m_strTitle = subGraphViewData.GetTitle();
	m_strPacketNames = subGraphViewData.GetPacketNames();
	return *this;
}

// =================================================================================
CSubGraphViewData::CSubGraphViewData():
	m_TYPEGraph(CGraphBaseData::GTNONE),
	m_graphStyle(-1),
	m_strTitle(""),
	m_strPacketNames("")
{
}

CSubGraphViewData::CSubGraphViewData(const CGraphBaseData::GRAPHTYPE type, const CGraphStyle& style, const CString& title, const CString& packetNames):
	m_TYPEGraph(type),
	m_graphStyle(style),
	m_strTitle(title),
	m_strPacketNames(packetNames)
{
}

CGraphBaseData::GRAPHTYPE CSubGraphViewData::GetGraphType() const
{
	return m_TYPEGraph;
}

CGraphStyle CSubGraphViewData::GetGraphStyle() const
{
	return m_graphStyle;
}

CString CSubGraphViewData::GetTitle() const
{
	return m_strTitle;
}

CString CSubGraphViewData::GetPacketNames() const
{
	return m_strPacketNames;
}


//////////////////////////////////////////////////////////////////////
// class CSubGraphViewDataList

CSubGraphViewDataList& CSubGraphViewDataList::operator=(const CSubGraphViewDataList& subGraphViewDataList)
{
	if(this == &subGraphViewDataList)
		return *this;

	for(int nIndex = 0; nIndex < subGraphViewDataList.GetCount(); nIndex++){
		m_subGraphViewDataList.AddTail(subGraphViewDataList.GetAt(nIndex));
	}

	return *this;
}

// ----------------------------------------------------------------------------------
void CSubGraphViewDataList::AddTail(CSubGraphViewData subGraphViewData)
{
	m_subGraphViewDataList.AddTail(subGraphViewData);
}

void CSubGraphViewDataList::RemoveAll()
{
	m_subGraphViewDataList.RemoveAll();
}

// ----------------------------------------------------------------------------------
CList<CSubGraphViewData, CSubGraphViewData&>& CSubGraphViewDataList::GetSubGraphViewDataList()
{
	return m_subGraphViewDataList;
}

const CList<CSubGraphViewData, CSubGraphViewData&>& CSubGraphViewDataList::GetSubGraphViewDataList() const
{
	return m_subGraphViewDataList;
}

POSITION CSubGraphViewDataList::GetHeadPosition() const
{
	return m_subGraphViewDataList.GetHeadPosition();
}

CSubGraphViewData& CSubGraphViewDataList::GetNext(POSITION& pos)
{
	return m_subGraphViewDataList.GetNext(pos);
}

CSubGraphViewData& CSubGraphViewDataList::GetAt(const int nIndex)
{
	POSITION pos = m_subGraphViewDataList.FindIndex(nIndex);
	assert(pos != NULL);
	return m_subGraphViewDataList.GetAt(pos);
}

CSubGraphViewData CSubGraphViewDataList::GetAt(const int nIndex) const
{
	POSITION pos = m_subGraphViewDataList.FindIndex(nIndex);
	assert(pos != NULL);
	return m_subGraphViewDataList.GetAt(pos);
}

int CSubGraphViewDataList::GetCount() const
{
	return m_subGraphViewDataList.GetCount();
}


//////////////////////////////////////////////////////////////////////
// class CRowViewData

CRowViewData& CRowViewData::operator=(const CRowViewData& rowViewData)
{
	if(this == &rowViewData)
		return *this;

	m_graphRegion = rowViewData.GetGraphRegion();
	m_drawingGraphRegion = rowViewData.GetDrawingGraphRegion();
	m_strHorzScaleName = rowViewData.GetHorzScaleName();
	m_subGraphViewDataList = rowViewData.GetSubGraphViewDataList();
	return *this;
}

// ---------------------------------------------------------------------------------
CRowViewData::CRowViewData():
	m_graphRegion(0, 0, 0, 0),
	m_drawingGraphRegion(0, 0, 0, 0)
{
}

CRowViewData::CRowViewData(const CRowViewData& rowViewData):
	m_graphRegion(rowViewData.GetGraphRegion()),
	m_drawingGraphRegion(rowViewData.GetDrawingGraphRegion()),
	m_strHorzScaleName(rowViewData.GetHorzScaleName())
{
	m_subGraphViewDataList = rowViewData.GetSubGraphViewDataList();
}

// ---------------------------------------------------------------------------------
void CRowViewData::RemoveAll()
{
	m_subGraphViewDataList.RemoveAll();
}

void CRowViewData::SetRegion(const CRect& graphRegion, const CRect& drawingGraphRegion)
{
	m_graphRegion = graphRegion;
	m_drawingGraphRegion = drawingGraphRegion;
}

void CRowViewData::SetHorzScaleName(const CString& strHorzScaleName)
{
	m_strHorzScaleName = strHorzScaleName;
}

// ---------------------------------------------------------------------------------
CRect CRowViewData::GetGraphRegion() const
{
	return m_graphRegion;
}

CRect CRowViewData::GetDrawingGraphRegion() const
{
	return m_drawingGraphRegion;
}

CString CRowViewData::GetHorzScaleName() const
{
	return m_strHorzScaleName;
}

CSubGraphViewDataList& CRowViewData::GetSubGraphViewDataList()
{
	return m_subGraphViewDataList;
}

const CSubGraphViewDataList& CRowViewData::GetSubGraphViewDataList() const
{
	return m_subGraphViewDataList;
}

int CRowViewData::GetSubGraphViewDataCount() const
{
	return m_subGraphViewDataList.GetCount();
}

CSubGraphViewData& CRowViewData::GetSubGraphViewData(const int nIndex)
{
	return m_subGraphViewDataList.GetAt(nIndex);
}


//////////////////////////////////////////////////////////////////////
// class CColumnViewData

CColumnViewData& CColumnViewData::operator=(const CColumnViewData& columnViewData)
{
	if(this == &columnViewData)
		return *this;

	for(int nIndex = 0; nIndex < columnViewData.GetCount(); nIndex++){
		m_rowViewDataList.AddTail(columnViewData.GetAt(nIndex));
	}

	return *this;
}

// ---------------------------------------------------------------------------------
CColumnViewData::CColumnViewData()
{
}

CColumnViewData::CColumnViewData(const CColumnViewData& columnViewData)
{
	for(int nIndex = 0; nIndex < columnViewData.GetCount(); nIndex++){
		m_rowViewDataList.AddTail(columnViewData.GetAt(nIndex));
	}
}

// ---------------------------------------------------------------------------------
void CColumnViewData::AddTail(CRowViewData rowViewData)
{
	m_rowViewDataList.AddTail(rowViewData);
}

void CColumnViewData::RemoveAll()
{
	m_rowViewDataList.RemoveAll();
}

// ---------------------------------------------------------------------------------
const CList<CRowViewData, CRowViewData&>& CColumnViewData::GetRowViewDataList() const
{
	return m_rowViewDataList;
}

CRowViewData& CColumnViewData::GetAt(const int nIndex)
{
	POSITION pos = m_rowViewDataList.FindIndex(nIndex);
	assert(pos != NULL);
	return m_rowViewDataList.GetAt(pos);
}

CRowViewData CColumnViewData::GetAt(const int nIndex) const
{
	POSITION pos = m_rowViewDataList.FindIndex(nIndex);
	assert(pos != NULL);
	return m_rowViewDataList.GetAt(pos);
}

bool CColumnViewData::GetAt(const int nIndex, CRowViewData& rowViewData)
{
	rowViewData.RemoveAll();
	POSITION pos = m_rowViewDataList.FindIndex(nIndex);
	if(pos == NULL)
		return false;

	rowViewData = m_rowViewDataList.GetAt(pos);
	return true;
}

int CColumnViewData::GetCount() const
{
	return m_rowViewDataList.GetCount();
}


//////////////////////////////////////////////////////////////////////
// class CViewData

void CViewData::AddTail(CColumnViewData columnViewData)
{
	m_columnViewDataList.AddTail(columnViewData);
}

void CViewData::RemoveAll()
{
	m_columnViewDataList.RemoveAll();
}

// ----------------------------------------------------------------------------------
CColumnViewData& CViewData::GetAt(const int nIndex)
{
	POSITION pos = m_columnViewDataList.FindIndex(nIndex);
	assert(pos != NULL);
	return m_columnViewDataList.GetAt(pos);
}

CColumnViewData CViewData::GetAt(const int nIndex) const
{
	POSITION pos = m_columnViewDataList.FindIndex(nIndex);
	assert(pos != NULL);
	return m_columnViewDataList.GetAt(pos);
}

CRowViewData& CViewData::GetAt(const int nColumnIndex, const int nRowIndex)
{
	POSITION pos = m_columnViewDataList.FindIndex(nColumnIndex);
	assert(pos != NULL);
	return m_columnViewDataList.GetAt(pos).GetAt(nRowIndex);
}

bool CViewData::GetAt(const int nIndex, CColumnViewData& columnViewData)
{
	POSITION pos = m_columnViewDataList.FindIndex(nIndex);
	if(pos == NULL)
		return false;

	columnViewData = m_columnViewDataList.GetAt(pos);
	return true;
}

bool CViewData::GetAt(const int nColumnIndex, const int nRowIndex, CRowViewData& rowViewData)
{
	rowViewData.RemoveAll();
	CColumnViewData columnViewData;
	if(!GetAt(nColumnIndex, columnViewData))
		return false;

	return columnViewData.GetAt(nRowIndex, rowViewData);
}

int CViewData::GetColumnViewDataCount() const
{
	return m_columnViewDataList.GetCount();
}

int CViewData::GetRowViewDataCount(const int nColumnIndex) const
{
	POSITION pos = m_columnViewDataList.FindIndex(nColumnIndex);
	if(pos == NULL)
		return 0;

	CColumnViewData columnViewData = m_columnViewDataList.GetAt(pos);
	return columnViewData.GetCount();
}

CList<CColumnViewData, CColumnViewData&>& CViewData::GetColumnViewDataList()
{
	return m_columnViewDataList;
}

