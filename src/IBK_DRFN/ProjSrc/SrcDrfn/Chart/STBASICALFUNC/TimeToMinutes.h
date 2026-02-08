// TimeToMinutes.h: interface for the CTimeToMinutes class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TIMETOMINUTES_H__D0D177AA_E195_4C3F_8983_8D954B778935__INCLUDED_)
#define AFX_TIMETOMINUTES_H__D0D177AA_E195_4C3F_8983_8D954B778935__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "../Include_ST/InnerFunction.h"

class CTimeToMinutes  : public CInnerFunction
{
public:
	CTimeToMinutes();
	virtual ~CTimeToMinutes();
	CString GetName(long lType);
	CString GetInputs(long lType);
	double Func(CISTController *pIController , long lType,  const VARIANT FAR& varArg1, const VARIANT FAR& varArg2, const VARIANT FAR& varArg3, const VARIANT FAR& varArg4, const VARIANT FAR& varArg5, const VARIANT FAR& varArg6, const VARIANT FAR& varArg7, const VARIANT FAR& varArg8, const VARIANT FAR& varArg9, const VARIANT FAR& varArg10);
private:
	CString m_strName;

};

#endif // !defined(AFX_TIMETOMINUTES_H__D0D177AA_E195_4C3F_8983_8D954B778935__INCLUDED_)
