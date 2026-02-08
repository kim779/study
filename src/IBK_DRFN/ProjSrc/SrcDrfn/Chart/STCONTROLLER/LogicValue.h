// LogicValue.h: interface for the CLogicValue class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LOGICVALUE_H__DDB7C4B3_CFB2_4761_87E4_BF83BA1480F3__INCLUDED_)
#define AFX_LOGICVALUE_H__DDB7C4B3_CFB2_4761_87E4_BF83BA1480F3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "LogicItem.h"

class CLogicValue  : public CLogicItem
{
public:
	CLogicValue();
	virtual ~CLogicValue();
	virtual double GetValue() {return m_dValue;}
	void SetValue(double dValue) { m_dValue = dValue;}
private:
	double m_dValue;
};

#endif // !defined(AFX_LOGICVALUE_H__DDB7C4B3_CFB2_4761_87E4_BF83BA1480F3__INCLUDED_)
