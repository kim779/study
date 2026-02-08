/**
	@file		unitquery.h
	@brief		IN/OUT에 대한 구조.
	@author		sashia
	@date		2009/08/17
*/

#if !defined(AFX_UNITQUERY_H__16F42BCA_D645_4B9B_978C_E7AC60B1F611__INCLUDED_)
#define AFX_UNITQUERY_H__16F42BCA_D645_4B9B_978C_E7AC60B1F611__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#pragma	pack(1)

#include "TrBlock.h"
#include "../../WrapDll/common/ChartProp.h"

/**
	@class		CTrPack
	@brief		데이터형식 Base
	@author		sashia
	@date		2009/08/13
*/
class CTrPack : public CObject
{
	DECLARE_DYNAMIC(CTrPack);

public:
	CTrPack();
	CTrPack(const CTrPack& obj)	{ *this = obj;	};
	virtual ~CTrPack();

	int			m_nVer;
	CString		m_strID;		//	query ID
	CString		m_strName;		//	query name
	BOOL		m_bInput;		//	Input/Output type(TRUE:Input, FALSE:output)
	CString		m_strNextKey;	//	NextKey

	CArrTrBlock		m_arrBlock;
	CArrTrItem		m_TrItemArr;
	CMapTrItem		m_mapItem;

public:
	virtual void	Copy(const CTrPack& item);
	virtual CTrPack& operator = (const CTrPack& item);

	BOOL		WriteQueryFile(CString strPath);
	BOOL		ReadQueryFile(CString strPath);

	void		RemoveAllData();
	void		RemoveAll();

	int			GetArrCnt(CString strCntItemID);
	DWORD		LoadDataFile(CString strFile);
	void		ReceiveData(const char* szData, int nSize);
	int			GetChartBozoInfo(CString &rString);
	CString		GetNextKey() { return m_strNextKey;}

public:
	BOOL		AddBlock(CTrBlock* pBlock);
	CTrBlock*	GetBlockAt(CString strName);
	CTrBlock*	GetBlockAt(int nIndex);
	int			GetBlockSize();

public:
	int			AddItem(CString strBlockName, CTrItem* pItem);
	int			GetItemSizeFromBlock(CString strBlockName);

	int			GetItemCount()	{ return m_TrItemArr.GetSize();	};
	CTrItem*	GetItemAt(int nIndex);
	CTrItem*	GetItemAt(CString strItemID);

	CString		GetDataAt(CString szKey);
	CString		GetDataAt(int nIndex);

	BOOL		GetArrDataAt(CStringArray* pArr, CString szKey);
	BOOL		GetArrDataAt(CStringArray* pArr, int nIndex);

	BOOL		ResetKeydata();

	int			GetDataBuffer(char *pszData, int nBufSize, CStringArray *pArrPackName=NULL, CArray<int, int> *pArrPackSize=NULL);
	void		SetUseBufAtItem(CString strItemID, BOOL bUse);
	BOOL		GetUseBufAtItem(CString strItemID);

	void		Serialize(CArchive& ar);

	void		SetSortType(int nNewType)	{m_nSortType=nNewType;}
	int			GetSortType()				{return m_nSortType;}
	void		SetRealABS(BOOL bRealABS)	{m_bRealABS=bRealABS;}
	BOOL		GetRealABS()				{return m_bRealABS;}

protected:
	int			m_nSortType;	//0: 1:
	BOOL		m_bRealABS;

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
private:
	CChartProp  m_prop;
public:
	void		SetChartProp( CChartProp& pProp ) { m_prop = pProp; }
	CString		GetPacketType( CString strType );	//패킷의 타입 리턴
	CString		GetBojoMsgDataFromKey( CString strKey );
	CString		CombineDateTime( CString strPacketType, CString strTempDate, CString strTempTime );

	CString		GetLastDate();
};

/**
	@class		CTrQuery
	@brief		데이터형식 Base
	@author		sashia
	@date		2009/09/23
*/
class CTrQuery : public CObject
{
	DECLARE_DYNAMIC(CTrQuery);

public:
	CTrQuery();
	CTrQuery(const CTrQuery& obj)	{ *this = obj;	};
	virtual ~CTrQuery();

	int			m_nVer;
	CString		m_strID;	//	query ID
	CString		m_strTRCode;
	CString		m_strRealSymbol;

	CTrPack		m_InPack;
	CTrPack		m_OutPack;

public:
	virtual void	Copy(const CTrQuery& item);
	virtual CTrQuery& operator = (const CTrQuery& item);

	BOOL		WriteQueryFile(CString strPath);
	BOOL		ReadQueryFile(CString strPath);

	void		Serialize(CArchive& ar);

	void		RemoveAll();
};

typedef	CMap<int, int, CTrQuery, CTrQuery&>			CIntMapTrQuery;
typedef	CMap<CString, LPCTSTR, CTrQuery, CTrQuery&>	CMapTrQuery;
typedef	CArray<CTrQuery, CTrQuery&>					CArrTrQuery;

#pragma	pack()

#endif // !defined(AFX_UNITQUERY_H__16F42BCA_D645_4B9B_978C_E7AC60B1F611__INCLUDED_)
