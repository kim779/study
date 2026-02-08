// BullBearArea.h: interface for the CBullBearArea class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_BULLBEARAREA_H__8E4179B2_A5F1_48BE_ACE9_A010DD26A1E1__INCLUDED_)
#define AFX_BULLBEARAREA_H__8E4179B2_A5F1_48BE_ACE9_A010DD26A1E1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "../Include_ST/InnerFunction.h"

class CBullBearArea  : public CInnerFunction
{
public:
	CBullBearArea();
	virtual ~CBullBearArea();
	CString GetName(long lType) {return m_strName;}
	CString GetInputs(long lType);
	double Func(CISTController *pIController , long lType,  const VARIANT FAR& varArg1, const VARIANT FAR& varArg2, const VARIANT FAR& varArg3, const VARIANT FAR& varArg4, const VARIANT FAR& varArg5, const VARIANT FAR& varArg6, const VARIANT FAR& varArg7, const VARIANT FAR& varArg8, const VARIANT FAR& varArg9, const VARIANT FAR& varArg10);
protected:
	CString m_strName;
	CString m_strInputInfo;
	double	m_dPosValue;
};

#endif // !defined(AFX_BULLBEARAREA_H__8E4179B2_A5F1_48BE_ACE9_A010DD26A1E1__INCLUDED_)
