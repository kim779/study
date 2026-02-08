// OrderSet.h: interface for the COrderSet class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ORDERSET_H__4BEC8F84_B3AB_4E0B_98BD_A3C6F2144E3B__INCLUDED_)
#define AFX_ORDERSET_H__4BEC8F84_B3AB_4E0B_98BD_A3C6F2144E3B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//객체를 파일에 저장했다가 다시 생성할때 순서대로 저장하기위한 중간과정에 필요한 class

#include <afxtempl.h>

class CDrawingSymbol;
class COrderAndElement;
class COrderSet  
{
public:
	virtual ~COrderSet();

	void Add(const int nIndex, CDrawingSymbol *pDrawingSymbol);

	CDrawingSymbol* GetElement(const int nIndex) const;
	int GetCount() const;
	POSITION GetHeadPosition() const;
	COrderAndElement* GetNext(POSITION& rPosition) const;
	COrderAndElement*& GetNext(POSITION& rPosition);

private:
	//symbol 순서 리스트.
	CList<COrderAndElement*, COrderAndElement* > m_orderList; 
};

#endif // !defined(AFX_ORDERSET_H__4BEC8F84_B3AB_4E0B_98BD_A3C6F2144E3B__INCLUDED_)
