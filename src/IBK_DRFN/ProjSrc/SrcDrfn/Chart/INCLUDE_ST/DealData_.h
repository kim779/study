// DealData.h: interface for the CDealData class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DEALDATA_H__91F3027D_1166_48C9_9BBA_67ABF195A063__INCLUDED_)
#define AFX_DEALDATA_H__91F3027D_1166_48C9_9BBA_67ABF195A063__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CDealData  
{
public:
	CDealData();	
	virtual ~CDealData();
	CDealData(const CDealData &rhs);
	CDealData  operator =(const CDealData& rhs );// OR
	CDealData  operator |(const CDealData& rhs );// OR
	CDealData  operator &(const CDealData& rhs );// AND
	void Clear();
	long m_lDealValue;
	long m_lType;
	double m_dPrice;
	double m_dContract;
	CString m_strDealName;
	CStringList m_strListEntryNameForExit;
	int m_nIndex;
};

#endif // !defined(AFX_DEALDATA_H__91F3027D_1166_48C9_9BBA_67ABF195A063__INCLUDED_)
