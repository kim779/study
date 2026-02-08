// OrderAndElement.h: interface for the COrderAndElement class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ORDERANDELEMENT_H__C0D13D1D_0093_49F5_A4B8_B3DA4C0FFAAA__INCLUDED_)
#define AFX_ORDERANDELEMENT_H__C0D13D1D_0093_49F5_A4B8_B3DA4C0FFAAA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//객체를 파일에 저장했다가 다시 생성할때 순서대로 저장하기위한 중간과정에 필요한 class
class CDrawingSymbol;
class COrderAndElement  
{
public:
	COrderAndElement(const int nIndex, CDrawingSymbol *pDrawingSymbol);

	int GetIndex();
	CDrawingSymbol* GetElement();

private:
	int m_nIndex;
	CDrawingSymbol* m_pDrawingSymbol;

};

#endif // !defined(AFX_ORDERANDELEMENT_H__C0D13D1D_0093_49F5_A4B8_B3DA4C0FFAAA__INCLUDED_)
