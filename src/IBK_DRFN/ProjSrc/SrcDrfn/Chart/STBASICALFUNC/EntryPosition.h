// EntryPosition.h: interface for the CEntryPosition class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ENTRYPOSITION_H__E98F7D26_F702_4921_8C8F_3FE5FAF50F34__INCLUDED_)
#define AFX_ENTRYPOSITION_H__E98F7D26_F702_4921_8C8F_3FE5FAF50F34__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "../Include_ST/InnerFunction.h"

class CEntryPosition  : public CInnerFunction
{
public:
	CEntryPosition();
	virtual ~CEntryPosition();
	CString GetName(long lType) {return m_strName;}
	CString GetInputs(long lType);
	double Func(CISTController *pIController , long lType,  const VARIANT FAR& varArg1, const VARIANT FAR& varArg2, const VARIANT FAR& varArg3, const VARIANT FAR& varArg4, const VARIANT FAR& varArg5, const VARIANT FAR& varArg6, const VARIANT FAR& varArg7, const VARIANT FAR& varArg8, const VARIANT FAR& varArg9, const VARIANT FAR& varArg10);
protected:
	CString m_strName;
	CString m_strInputInfo;
	double	m_dPosValue;

};

#endif // !defined(AFX_ENTRYPOSITION_H__E98F7D26_F702_4921_8C8F_3FE5FAF50F34__INCLUDED_)
