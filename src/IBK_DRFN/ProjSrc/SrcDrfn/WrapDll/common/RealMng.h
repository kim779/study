/**
	@file		RealMng.h
	@brief		실시간 데이터 관리
	@author		sashia
	@date		2009/10/06
*/

#if !defined(AFX_REALMNG_H__16F42BCA_D645_4B9B_978C_E7AC60B1F611__INCLUDED_)
#define AFX_REALMNG_H__16F42BCA_D645_4B9B_978C_E7AC60B1F611__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "DataComm.h"
#include "ChartProp.h"

#pragma	pack(1)

class CRealMng : public CObject
{
	DECLARE_DYNAMIC(CRealMng);
public:
	CRealMng();
	virtual ~CRealMng();

	CStringArray		m_arrCode;
	CDataComm			*m_pComm;

public:
	virtual void	Copy(const CRealMng& item);
	virtual CRealMng& operator = (const CRealMng& item);

	void		RemoveAll();

	int			AdviceRealKey(CString strCode);
	CString		UnAdviceRealKey(int nKey);
	int			UnAdviceRealKey(CString strCode);

	CString		GetRealCode(int nKey);
	int			GetRealKey(CString strCode);

	void		SetDataComm(CDataComm *pComm)	{ m_pComm = pComm;	};
	int			SendRealData(int nKey, CFidQuery &qry, CString strRealSym);
	int			SendRealData(int nKey, CString strCode, CTrQuery &qry, CString strRealSym);
};

#pragma	pack()

#endif // !defined(AFX_REALMNG_H__16F42BCA_D645_4B9B_978C_E7AC60B1F611__INCLUDED_)
