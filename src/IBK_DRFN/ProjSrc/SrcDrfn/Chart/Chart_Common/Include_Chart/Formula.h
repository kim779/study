// Formula.h: interface for the CFormula class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_FORMULA_H__926A8AA2_1239_11D6_9374_0050FC28B229__INCLUDED_)
#define AFX_FORMULA_H__926A8AA2_1239_11D6_9374_0050FC28B229__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CFormula  
{
public:
	CString GetlongEnter()	{return m_longEnter;}
	CString GetlongClear()	{return m_longClear;}
	CString GetshortEnter()	{return m_shortEnter;}
	CString GetshortClear()	{return m_shortClear;}

	CFormula(const CString& longEnter, 
			const CString& longClear, 
			const CString& shortEnter, 
			const CString& shortClear);
	virtual ~CFormula();

private:
	CString m_longEnter;
	CString m_longClear;
	CString m_shortEnter;
	CString m_shortClear;
};

#endif // !defined(AFX_FORMULA_H__926A8AA2_1239_11D6_9374_0050FC28B229__INCLUDED_)
