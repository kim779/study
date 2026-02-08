// ViewData.h: interface for the CViewData class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_VIEWDATA_H__8F347F36_C3A8_468D_9CEE_A7CB21B98C72__INCLUDED_)
#define AFX_VIEWDATA_H__8F347F36_C3A8_468D_9CEE_A7CB21B98C72__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "BlockBaseData.h"

class CSubGraphViewData
{
public:
	CSubGraphViewData& operator=(const CSubGraphViewData& subGraphViewData);

public:
	CSubGraphViewData();
	CSubGraphViewData(const CGraphBaseData::GRAPHTYPE type, const CGraphStyle& style, const CString& title, const CString& packetNames);
	CSubGraphViewData(const CGraphBaseData::GRAPHTYPE type, const CGraphStyle& style, const CString& title, const CString& packetNames, const CString& strRQ);

	CGraphBaseData::GRAPHTYPE GetGraphType() const;
	CGraphStyle GetGraphStyle() const;
	CString GetTitle() const;
	CString GetPacketNames() const;
	CString GetRQ() const;

private:
	CGraphBaseData::GRAPHTYPE m_TYPEGraph; // subGraph type
	CGraphStyle m_graphStyle;	// subGraph style
	CString m_strTitle;			// subGraph title
	CString m_strPacketNames;	// 사용하는 packet name
	CString m_strRQ;			// 사용하는 RQ
};



// CViewData 의 list -> 모든 block에 있는 view data
class CSubGraphViewDataList
{
public:
	CSubGraphViewDataList& operator=(const CSubGraphViewDataList& subGraphViewDataList);

public:
	void AddTail(CSubGraphViewData subGraphViewData);
	void RemoveAll();

	CList<CSubGraphViewData, CSubGraphViewData&>& GetSubGraphViewDataList();
	const CList<CSubGraphViewData, CSubGraphViewData&>& GetSubGraphViewDataList() const;

	POSITION GetHeadPosition() const;
	CSubGraphViewData& GetNext(POSITION& pos);
	CSubGraphViewData& GetAt(const int nIndex);
	CSubGraphViewData GetAt(const int nIndex) const;
	int GetCount() const;

private:
	CList<CSubGraphViewData, CSubGraphViewData&> m_subGraphViewDataList;
};


// row (-> block)
class CRowViewData  
{
public:
	CRowViewData& operator=(const CRowViewData& rowViewData);

public:
	CRowViewData();
	CRowViewData(const CRowViewData& rowViewData);
	
	void RemoveAll();

	void SetRegion(const CRect& graphRegion, const CRect& drawingGraphRegion);
	void SetHorzScaleName(const CString& strHorzScaleName);
	
	CRect GetGraphRegion() const;
	CRect GetDrawingGraphRegion() const;
	CString GetHorzScaleName() const;
	CSubGraphViewDataList& GetSubGraphViewDataList();
	const CSubGraphViewDataList& GetSubGraphViewDataList() const;

	int GetSubGraphViewDataCount() const;
	CSubGraphViewData& GetSubGraphViewData(const int nIndex);

private:
	CRect m_graphRegion;
	CRect m_drawingGraphRegion;
	CString m_strHorzScaleName;
	CSubGraphViewDataList m_subGraphViewDataList;
};


// column (-> blocksColumn)
class CColumnViewData
{
public:
	CColumnViewData& operator=(const CColumnViewData& columnViewData);

public:
	CColumnViewData();
	CColumnViewData(const CColumnViewData& columnViewData);

	void AddTail(CRowViewData rowViewData);
	void RemoveAll();

	const CList<CRowViewData, CRowViewData&>& GetRowViewDataList() const;
	// (2006/3/25 - Seung-Won, Bae) Support None-Const Function for Managing the Position in Other Object's Member.
	//		Const Function supports only Copy Object of CRowViewData. and Copyed Object is not complete.
	//		So. I can backup some CSubGraphViewData's Position in CRowViewData.
	CList<CRowViewData, CRowViewData&>& GetRowViewDataList( void);

	CRowViewData& GetAt(const int nIndex);
	CRowViewData GetAt(const int nIndex) const;
	bool GetAt(const int nIndex, CRowViewData& rowViewData);

	int GetCount() const;

private:
	CList<CRowViewData, CRowViewData&> m_rowViewDataList;
};


class CViewData
{
public:
	void AddTail(CColumnViewData columnViewData);
	void RemoveAll();

	CColumnViewData& GetAt(const int nIndex);
	CColumnViewData GetAt(const int nIndex) const;
	CRowViewData& GetAt(const int nColumnIndex, const int nRowIndex);
	bool GetAt(const int nIndex, CColumnViewData& columnViewData);
	bool GetAt(const int nColumnIndex, const int nRowIndex, CRowViewData& rowViewData);
	CList<CColumnViewData, CColumnViewData&>& GetColumnViewDataList();

	int GetColumnViewDataCount() const;
	int GetRowViewDataCount(const int nColumnIndex) const;

private:
	CList<CColumnViewData, CColumnViewData&> m_columnViewDataList;
};

#endif // !defined(AFX_VIEWDATA_H__8F347F36_C3A8_468D_9CEE_A7CB21B98C72__INCLUDED_)
