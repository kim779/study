/**
	@file		FidPack.h
	@brief		IN/OUT에 대한 구조.
	@author		sashia
	@date		2009/08/17
*/

#if !defined(AFX_FIDPACK_H__16F42BCA_D645_4B9B_978C_E7AC60B1F611__INCLUDED_)
#define AFX_FIDPACK_H__16F42BCA_D645_4B9B_978C_E7AC60B1F611__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define		MARKET_STOCK		1
#define		MARKET_FUTOPT		2
#define		MARKET_UPJONG		3

#pragma	pack(1)

#include "FidBlock.h"

/**
	@class		CFidPack
	@brief		데이터형식 Base
	@author		sashia
	@date		2009/09/23
*/
class CFidPack : public CObject
{
	DECLARE_DYNAMIC(CFidPack);

public:
	CFidPack();
	CFidPack(const CFidPack& obj)	{ *this = obj;	};
	virtual ~CFidPack();

	int			m_nVer;
	CString		m_strID;	//	Pack name
	BOOL		m_bInput;	//	Input/Output type(TRUE:Input, FALSE:output)

	CArrFidBlock	m_arrBlock;
	CMapFidItem		m_mapItem;
	CArrFidItem		m_arrItem;
	CMapFidItem		m_mapChartNameItem;

protected:
	int			m_nSortType;	//0: 1:
	BOOL		m_bRealABS;		//

public:
	virtual void	Copy(const CFidPack& item);
	virtual CFidPack& operator = (const CFidPack& item);

	void		RemoveAllData();
	void		RemoveAll();

	void		ReceiveData(const char* szData, int nSize);

public:
	BOOL		AddBlock(CFidBlock* pBlock);
	CFidBlock*	GetBlockAt(CString strName);
	CFidBlock*	GetBlockAt(int nIndex);
	int			GetBlockSize();

public:
	int			AddItem(CString strBlockName, CFidItem* pItem);
	int			GetItemSizeFromBlock(CString strBlockName);

	int			GetItemCount()	{ return m_arrItem.GetSize();	};
	CFidItem*	GetItemAt(int nIndex);
	CFidItem*	GetItemAt(CString strItemID);
	CFidItem*	GetItemChartAt(CString strItemID);

	CString		GetDataAt(CString szKey);
	CString		GetDataAt(int nIndex);

	CString		GetCodeData(CString &strFID, int nIndex=-1);

	BOOL		GetArrDataAt(CStringArray* pArr, CString szKey);
	BOOL		GetArrDataAt(CStringArray* pArr, int nIndex);

	BOOL		ResetKeydata();

	int			GetDataBuffer(char *pszData, int nBufSize);
	void		SetUseBufAtItem(CString strItemID, BOOL bUse);
	BOOL		GetUseBufAtItem(CString strItemID);

	void		Serialize(CArchive& ar);

	void		SetSortType(int nNewType)	{m_nSortType=nNewType;}
	int			GetSortType()				{return m_nSortType;}
	void		SetRealABS(BOOL bRealABS)	{m_bRealABS=bRealABS;}
	BOOL		GetRealABS()				{return m_bRealABS;}
	

public:
	//-->@IMSI-alzioyes
	//For TR
	int			m_nOneArrSize;
	int			m_nTRDataCnt;
	//LPBYTE		m_pTRData;

	int			GetOneArrSize() { return m_nOneArrSize;}
	int			GetTRDataCnt()	{ return m_nTRDataCnt;}
	//LPBYTE		GetTRData()		{ return m_pTRData;}
	//<--@IMSI-alzioyes

	CString		GetGridSymbol();
	void		SetGridSymbol(CString strSymbol);
	CString		GetDiffGridSymbol();
	void		SetDiffGridSymbol(CString strDiffSymbol);
};


/**
	@class		CFidQuery
	@brief		데이터형식 Base
	@author		sashia
	@date		2009/09/23
*/
class CFidQuery : public CObject
{
	DECLARE_DYNAMIC(CFidQuery);

public:
	CFidQuery();
	CFidQuery(const CFidQuery& obj)	{ *this = obj;	};
	virtual ~CFidQuery();

	int			m_nVer;
	CString		m_strID;	//	query ID
	CString		m_strOOPCode;
	CString		m_strRealSymbol;

	int			m_nMarket;
	int			m_nRealKey;

	CFidPack	m_InPack;
	CFidPack	m_OutPack;

public:
	virtual void	Copy(const CFidQuery& item);
	virtual CFidQuery& operator = (const CFidQuery& item);

	BOOL		WriteQueryFile(CString strPath);
	BOOL		ReadQueryFile(CString strPath);

	void		Serialize(CArchive& ar);

	void		RemoveAll();

	void		SetRealSymbol(CString strSymbol);

	static	CString	GetCodeFID(int nMarket);
	static	CString	GetTickFID(int nType);
};

typedef	CMap<int, int, CFidQuery, CFidQuery&>			CIntMapFidQuery;
typedef	CMap<CString, LPCTSTR, CFidQuery, CFidQuery&>	CMapFidQuery;
typedef	CArray<CFidQuery, CFidQuery&>					CArrFidQuery;

#pragma	pack()

#endif // !defined(AFX_FIDPACK_H__16F42BCA_D645_4B9B_978C_E7AC60B1F611__INCLUDED_)
