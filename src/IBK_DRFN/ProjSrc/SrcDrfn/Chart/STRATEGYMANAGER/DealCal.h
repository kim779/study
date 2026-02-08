// DealCal.h: interface for the CDealCal class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DEALCAL_H__73C174F2_ECDC_4AC5_9ACD_13940B75B607__INCLUDED_)
#define AFX_DEALCAL_H__73C174F2_ECDC_4AC5_9ACD_13940B75B607__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CDealCal  
{
public:
	virtual ~CDealCal() {};
	virtual void SetDeal(int nPos, double dValue) = 0;
protected:
	CDealCal() {};
};

#endif // !defined(AFX_DEALCAL_H__73C174F2_ECDC_4AC5_9ACD_13940B75B607__INCLUDED_)
