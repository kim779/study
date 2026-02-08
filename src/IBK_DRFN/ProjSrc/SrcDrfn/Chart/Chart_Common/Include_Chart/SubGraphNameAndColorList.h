// SubGraphNameAndColorList.h: interface for the CSubGraphNameAndColorList class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SUBGRAPHNAMEANDCOLORLIST_H__00825833_E83D_48EC_A168_CB81FF3FAEF5__INCLUDED_)
#define AFX_SUBGRAPHNAMEANDCOLORLIST_H__00825833_E83D_48EC_A168_CB81FF3FAEF5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "Color.h"

class CSubGraphNameAndColor
{
public:
	CSubGraphNameAndColor& operator=(const CSubGraphNameAndColor& data);

public:
	CSubGraphNameAndColor();
	CSubGraphNameAndColor(const CString& strGraphName, const CGraphColor& graphColor);

	void SetGraphName(const CString& strGraphName);
	void SetGraphColor(const CGraphColor& graphColor);
	CString GetGraphName() const;
	CGraphColor GetGraphColor() const;
	CGraphColor& GetGraphColor();

private:
	CString m_strGraphName;
	CGraphColor m_graphColor;
};


class CSubGraphNameAndColorList
{
public:
	CSubGraphNameAndColorList& operator=(const CSubGraphNameAndColorList& data);

public:
	void AddTail(const CSubGraphNameAndColor& data);
	void RemoveAll();
	
	int GetCount() const;
	POSITION GetHeadPosition() const;
	CSubGraphNameAndColor& GetNext(POSITION& pos);
	CSubGraphNameAndColor GetNext(POSITION& pos) const;
	const CList<CSubGraphNameAndColor, CSubGraphNameAndColor>* GetList() const;
	CList<CSubGraphNameAndColor, CSubGraphNameAndColor>* GetList();

private:
	CList<CSubGraphNameAndColor, CSubGraphNameAndColor> m_graphColorList;
};

#endif // !defined(AFX_SUBGRAPHNAMEANDCOLORLIST_H__00825833_E83D_48EC_A168_CB81FF3FAEF5__INCLUDED_)
