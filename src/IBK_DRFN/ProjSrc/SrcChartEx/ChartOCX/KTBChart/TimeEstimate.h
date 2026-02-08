// TimeEstimate.h: interface for the CTimeEstimate class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TIMEESTIMATE_H__647D5FEB_8731_46B4_84C9_6D29AB041113__INCLUDED_)
#define AFX_TIMEESTIMATE_H__647D5FEB_8731_46B4_84C9_6D29AB041113__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CTimeEstimate  
{
public:
	CTimeEstimate();
	virtual ~CTimeEstimate();


public:

	BOOL		m_bTimeEstimate;
	
	__int64		m_frq;
	__int64		m_start;	

public:

	void		StartEstimate();
	DWORD		EndEstimate();
	
};

#endif // !defined(AFX_TIMEESTIMATE_H__647D5FEB_8731_46B4_84C9_6D29AB041113__INCLUDED_)
