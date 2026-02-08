// DataMgr.h: interface for the CDataMgr class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DATAMGR_H__5133C7B3_426F_400E_88D5_A9B89B5E1C51__INCLUDED_)
#define AFX_DATAMGR_H__5133C7B3_426F_400E_88D5_A9B89B5E1C51__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "AppData.h"
#include "OrgData.h"

class AFX_EXT_CLASS CDataMgr  
{
public:
	CDataMgr(CWnd *pParent);
	virtual ~CDataMgr();

public:

private:
	CWnd	*m_pParent;
	double	m_dDollar;
	CString	m_strCtim;
	CString	m_strCurr;
	CString	m_strSiga;
	CString	m_strKoga;
	CString	m_strJega;
	CString	m_strGvol;
	CString	m_strCvol;
	CString	m_strGamt;
	CString	m_strMgjy;

	CMap	<int, int, class COrgData*, class COrgData*> m_mapOrgData;	// Org Data Que
	CMap	<int, int, class CAppData*, class CAppData*> m_mapAppData;	// Applicative Data Que

public:

	void	DetachData(int key = -1);
	int	CreateOrgData(char* pcData);
	int	CreateAppData(class COrgData* pOrgData, int iKey, int iGKind, struct _chartinfo* pChartInfo, int iCOption);
	bool	AttachData(int iDtKey, int iCount, int iDummy, char* pcData, int& riDataL, char* pcCurDate, bool bTodayData);
	bool	AttachMapData(int iDtKey, int iCount, char* pcData, int& riDataL, char* pcCurDate, bool bTodayData);
	bool	AttachData2(int dkey, int count, int dummy, char *pData, int &dataL, int id);
	bool	GetData(int key, class COrgData*& pOrgData);
	bool	GetData(int key, class CAppData *&pAppData);
	bool	GetOrgData(int key, class COrgData *&pOrgData);

	int	ParseRealtime(CString sCode, CString strRTData, bool& rbIncrease);
	
	void	SetCurr(int dKey, CString sData);
	void	SetStock(int dKey, CString sData);
	void	SetStockKind(int dKey, char cKind);
	void	GetRtsCode(CString &rcode);

	static int CalcSMA(double *source, double *dest, int count, int start, int size);
	static double CalcSMA(double *source, int size);
	static int CalcSWMA(double *source, double *dest, int count, int start, int size);
	static int CalcEMA(double *source, double *dest, int count, int start, int size);
	static int CalcESMA(double *source, double *dest, int count, int start, int size);

	double	GetDollar()	{ return m_dDollar; };
	LPCTSTR	GetCTIM()	{ return m_strCtim; };
	LPCTSTR	GetCurr()	{ return m_strCurr; };
	LPCTSTR	GetSiga()	{ return m_strSiga; };
	LPCTSTR	GetKoga()	{ return m_strKoga; };
	LPCTSTR	GetJega()	{ return m_strJega; };
	LPCTSTR	GetGvol()	{ return m_strGvol; };
	LPCTSTR	GetCvol()	{ return m_strCvol; };
	LPCTSTR	GetGamt()	{ return m_strGamt; };
	LPCTSTR	GetMgjy()	{ return m_strMgjy; };
	void	SetCurr(CString strTCurr)	{ m_strCurr = strTCurr; };
	void	SetSiga(CString strSiga)	{ m_strSiga = strSiga; };
	void	SetKoga(CString strKoga)	{ m_strKoga = strKoga; };
	void	SetJega(CString strJega)	{ m_strJega = strJega; };

//	void	SetNoDeleteData(int iKey, bool bNoDeleteData);

private:
	int	GetUnusedKey(bool bOrgData);
	
};

#endif // !defined(AFX_DATAMGR_H__5133C7B3_426F_400E_88D5_A9B89B5E1C51__INCLUDED_)
