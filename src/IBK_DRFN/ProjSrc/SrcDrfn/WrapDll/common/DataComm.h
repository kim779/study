/**
	@file		DataComm.h
	@brief		FID, TR 통신 관리.
	@author		sashia
	@date		2009/09/30
*/

#if !defined(AFX_DATACOMM_H__16F42BCA_D645_4B9B_978C_E7AC60B1F611__INCLUDED_)
#define AFX_DATACOMM_H__16F42BCA_D645_4B9B_978C_E7AC60B1F611__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "../../../SrcSite/include/axisfire.h"

#include "ChartProp.h"
#include "../../DrCommon/DataClassLib/TrQuery.h"
#include "../../DrCommon/DataClassLib/FidQuery.h"

#define		COMM_TRKEY_START		1
#define		COMM_FIDKEY_START		51
#define		COMM_REALKEY_START		101

#define		COMM_QUERYSEND_COUNT	50
#define		COMM_RECEIVEREAL_COUNT	150

#pragma	pack(1)

class CDataComm : public CObject
{
	DECLARE_DYNAMIC(CDataComm);
public:
	CDataComm();
	virtual ~CDataComm();

	int				m_nKey;
	CString			m_strChartName;
	CWnd			*m_pCommWnd, *m_pParent;

	CChartPropIntMap		m_mapQuery;

public:
	virtual void	Copy(const CDataComm& item);
	virtual CDataComm& operator = (const CDataComm& item);

	void		RemoveAll();
	void		Serialize(CArchive& ar);

	void		SetKey(int nKey)	{ m_nKey = nKey;	};
	void		SetChartName(CString strName)	{ m_strChartName = strName;	};

public:
	void		SetCommWnd(CWnd* pWnd)	{ m_pParent = pWnd;	m_pCommWnd = pWnd->GetParent();	};

	int			SendTRData(CChartProp &prop, CTrQuery *pTrQry, CString strData);
	int			SendFIDData(CChartProp &prop, CFidQuery &fidQry, CFidBlock *pBlock, CString strData, BOOL bNext);

	int			SendTR(CString strTR, CString strData, int nKey, int nStat);

	int			GetTrKey(BOOL bTR);

	CString		GetFidInboundData(CFidQuery &fidQry, CFidBlock *pDefBlock, BOOL bNext);
	CString		GetFidInboundDataTest(CFidQuery &fidQry, CFidBlock *pDefBlock, BOOL bNext);
	BOOL		GetRcvTr(int nKey, CChartProp &prop);

public:
	static	void	ParsingInputData(CString strData, CStringArray &arr);
};

#pragma	pack()

#endif // !defined(AFX_DATACOMM_H__16F42BCA_D645_4B9B_978C_E7AC60B1F611__INCLUDED_)
