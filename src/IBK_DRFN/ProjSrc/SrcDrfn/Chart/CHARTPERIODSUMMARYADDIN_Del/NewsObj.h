// NewsObj.h: interface for the CNewsObj class.
//
//////////////////////////////////////////////////////////////////////
//#include "CISBitmap.h"

#if !defined(AFX_NEWSOBJ_H__52F70937_49A7_4319_804F_4A5DC080C42A__INCLUDED_)
#define AFX_NEWSOBJ_H__52F70937_49A7_4319_804F_4A5DC080C42A__INCLUDED_

//#include "Linear.h"	// Added by ClassView

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//#include <afxwin.h>

#define HEIGHT	16
#define WIDTH	16

class CNewsObj : public CObject  
{
	DECLARE_SERIAL(CNewsObj);
public:
	void SetDate(long lDate/*IN*/);
	CNewsObj();
	virtual ~CNewsObj();

	CDC* m_pdcMem;
	CDC m_dcTransMem;
	CBitmap m_bitmap;	
	CBitmap* m_pOldBmp;
	int m_nIndex;		// index
	long m_lxData;		// date
	double m_dyData;	
	long m_lCntNews;	// news count
	CPoint m_ptAt;		//NewsObj의 위치
	CSize m_size;		//NewsObj 그림의 크기
	BOOL m_bSelected;	// 선택 여부
	CString m_strDate;
	void Draw(CDC* pDC);
	void Erase(CDC* pDC);
	BOOL IsInRegion(CPoint point);
	void ButtonDownAt(const CPoint& pt);
	void ButtonUp() { m_bDrag = FALSE; }
	void SetPos(const CPoint& ptNew);
	CPoint GetPos() { return m_ptAt; }
	CRect GetRect() { return CRect(m_ptAt, m_size); }
	BOOL Initialize();
	static void Release();

protected:
	BOOL m_bDrag;		//드래그 여부
	CPoint m_ptOffset;	//NewsObj 위치와 클릭한 위치의 차이 
	COLORREF	m_crBlack;
	COLORREF	m_crWhite;
	CRect m_rc;			// 그리기 영역
};

//////////////////////////////////////////////////////////////////////////
// Object list class
class CNewsObjList : public CObList
{
public:
	CNewsObj* GetNextServer(POSITION pos);	//NewsObj 중에서 다음 서버 얻기
	CNewsObj* FindNewsObj(DWORD type);		// 원하는 중류의 NewsObj찾기
	int GetNewsObjCount(DWORD type);		//원하는 종류의 NewsObj의 수
	int GetTotalNewsObj();					// 전체 NewsObj 수
	void DeleteAll();
	void AddHeadNewsObj(CNewsObj* pNewsObj){ AddHead(pNewsObj); }
	void SetAtNewsObj(POSITION pos, CNewsObj* pNewsObj){ SetAt(pos, pNewsObj);}
	CNewsObj* GetHeadNewsObj(){ return (CNewsObj*)GetHead(); }
	CNewsObj* GetNextNewsObj(POSITION& pos){ return (CNewsObj*)GetNext(pos); }
	CNewsObj* GetPrevNewsObj(POSITION& pos){ return (CNewsObj*)GetPrev(pos); }

};

#endif // !defined(AFX_NEWSOBJ_H__52F70937_49A7_4319_804F_4A5DC080C42A__INCLUDED_)
