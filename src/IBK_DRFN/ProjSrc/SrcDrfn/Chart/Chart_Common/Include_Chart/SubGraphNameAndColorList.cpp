// SubGraphNameAndColorList.cpp: implementation of the CSubGraphNameAndColorList class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SubGraphNameAndColorList.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
// class CSubGraphNameAndColor

CSubGraphNameAndColor& CSubGraphNameAndColor::operator=(const CSubGraphNameAndColor& data)
{
	if(this == &data)
		return *this;

	m_strGraphName = data.GetGraphName();
	m_graphColor = data.GetGraphColor();
	return *this;
}

CSubGraphNameAndColor::CSubGraphNameAndColor()
{
}

CSubGraphNameAndColor::CSubGraphNameAndColor(const CString& strGraphName, const CGraphColor& colors):
	m_strGraphName(strGraphName),
	m_graphColor(colors)
{
}

// public =====================================================================
void CSubGraphNameAndColor::SetGraphName(const CString& strGraphName)
{
	m_strGraphName = strGraphName;
}

void CSubGraphNameAndColor::SetGraphColor(const CGraphColor& colors)
{
	m_graphColor = colors;
}

CString CSubGraphNameAndColor::GetGraphName() const
{
	return m_strGraphName;
}

CGraphColor CSubGraphNameAndColor::GetGraphColor() const
{
	return m_graphColor;
}

CGraphColor& CSubGraphNameAndColor::GetGraphColor()
{
	return m_graphColor;
}


///////////////////////////////////////////////////////////////////////////////
// class CSubGraphNameAndColorList

CSubGraphNameAndColorList& CSubGraphNameAndColorList::operator=(const CSubGraphNameAndColorList& data)
{
	if(this == &data)
		return *this;

	const CList<CSubGraphNameAndColor, CSubGraphNameAndColor>* pSubGraphList = data.GetList();
	if(pSubGraphList == NULL)
		return *this;

	POSITION pos = pSubGraphList->GetHeadPosition();
	while(pos != NULL)
		AddTail(pSubGraphList->GetNext(pos));

	return *this;
}

void CSubGraphNameAndColorList::AddTail(const CSubGraphNameAndColor& data)
{
	m_graphColorList.AddTail(data);
}

void CSubGraphNameAndColorList::RemoveAll()
{
	m_graphColorList.RemoveAll();
}

// ----------------------------------------------------------------------------
int CSubGraphNameAndColorList::GetCount() const
{
	return m_graphColorList.GetCount();
}

POSITION CSubGraphNameAndColorList::GetHeadPosition() const
{
	return m_graphColorList.GetHeadPosition();
}

CSubGraphNameAndColor& CSubGraphNameAndColorList::GetNext(POSITION& pos)
{
	return m_graphColorList.GetNext(pos);
}

CSubGraphNameAndColor CSubGraphNameAndColorList::GetNext(POSITION& pos) const
{
	return m_graphColorList.GetNext(pos);
}

const CList<CSubGraphNameAndColor, CSubGraphNameAndColor>* CSubGraphNameAndColorList::GetList() const
{
	return &m_graphColorList;
}

CList<CSubGraphNameAndColor, CSubGraphNameAndColor>* CSubGraphNameAndColorList::GetList()
{
	return &m_graphColorList;
}
