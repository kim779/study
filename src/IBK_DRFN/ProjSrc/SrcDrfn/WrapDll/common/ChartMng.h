/**
	@file		ChartMng.h
	@brief		차트 그래프 관리.
	@author		sashia
	@date		2009/10/06
*/

#if !defined(AFX_CHARTMNG_H__16F42BCA_D645_4B9B_978C_E7AC60B1F611__INCLUDED_)
#define AFX_CHARTMNG_H__16F42BCA_D645_4B9B_978C_E7AC60B1F611__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ChartProp.h"

#pragma	pack(1)

class CChartMng : public CObject
{
	DECLARE_DYNAMIC(CChartMng);
public:
	CChartMng();
	virtual ~CChartMng();

protected:
	CChartPropArr		m_arrChartProp;

public:
	virtual void	Copy(const CChartMng& item);
	virtual CChartMng& operator = (const CChartMng& item);

	void		RemoveAll();

	int			GetChartProp(CString strKey, CChartProp &info);
	void		GetChartPropArr(CString strCode, CChartPropArr &infoArr);

	void		SetChartProp(CString strKey, CChartProp &info);

	int			GetChartRQ(CString strRQ, CChartProp &info);
	void		RemoveRQ(CString strRQ);

	int			GetSize()	{ return m_arrChartProp.GetSize();	};
	int			GetChartPropAt(int nIndex, CChartProp& info);

	bool		GetRQChartAt(CString &szRQ, int nIndex=0);
};

#pragma	pack()

#endif // !defined(AFX_CHARTMNG_H__16F42BCA_D645_4B9B_978C_E7AC60B1F611__INCLUDED_)
