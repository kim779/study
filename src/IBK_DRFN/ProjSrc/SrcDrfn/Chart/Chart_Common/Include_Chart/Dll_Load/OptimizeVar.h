// OptimizeVar.h: interface for the COptimizeVar class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_OPTIMIZEVAR_H__926A8AA3_1239_11D6_9374_0050FC28B229__INCLUDED_)
#define AFX_OPTIMIZEVAR_H__926A8AA3_1239_11D6_9374_0050FC28B229__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class AFX_EXT_CLASS COptimizeVar : public CObject    
{
public:
	COptimizeVar(const CString& strOptVar);
	COptimizeVar(const CString& strName,
				 const CString& strType,
				 const CString& strOpt,
				 const CString& strMin,
				 const CString& strMax,
				 const CString& strStep,
				 const CString& strExplain);
	virtual ~COptimizeVar();
	void SetVar(const CString& strName,
				 const CString& strType,
				 const CString& strOpt,
				 const CString& strMin,
				 const CString& strMax,
				 const CString& strStep,
				 const CString& strExplain);

public:
	CString m_Name;
	CString m_Type;
	CString m_Opt;
	CString m_Min;
	CString m_Max;
	CString m_Step;
	CString m_Explain;
};

#endif // !defined(AFX_OPTIMIZEVAR_H__926A8AA3_1239_11D6_9374_0050FC28B229__INCLUDED_)
