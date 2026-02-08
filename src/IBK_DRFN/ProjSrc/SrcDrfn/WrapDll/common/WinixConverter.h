/**
	@file		WinixConverter.h
	@brief		IN/OUT에 대한 구조.
	@author		sashia
	@date		2009/09/10
*/

#if !defined(AFX_WINIXCONVERTER_H__16F42BCA_D645_4B9B_978C_E7AC60B1F611__INCLUDED_)
#define AFX_WINIXCONVERTER_H__16F42BCA_D645_4B9B_978C_E7AC60B1F611__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ChartProp.h"

#include "../../../SrcSite/include/axisfire.h"

//#include "../common/unitquery.h"
#include "../../DrCommon/DataClassLib/TrQuery.h"
#include "../../DrCommon/DataClassLib/FidQuery.h"
#include "../common/PropertyMap.h"

#include "../../inc/IChartBozoMsgMng.h"
#include "../../inc/IMasterDataMng.h"
#include "../common/MapChartCommon.h"

#pragma	pack(1)

#define	FIDSYMBOL_GRIDSTARTER	_T('$')

/**
	@class		CWinixConverter
	@brief		데이터형식 Base
	@author		sashia
	@date		2009/09/10
*/
class CWinixConverter
{
public:
	CWinixConverter();
	virtual ~CWinixConverter();

	IChartBozoMsgManager		*m_pBozoMng;

	CWnd*		m_pParent;
	CString		m_strMapFile;
	char*		m_pszLayout;

	CChartProp			m_prop;

	CStringArray		m_arrFIDOut;

	GRID_O				m_OutGrid;
	int					m_nRowCount;

	CString				m_strMajorFID;
	CString				m_strMinorFID;

	CString				m_strCurCode;

	BOOL				m_bRunMode;

	CString				m_strDateName, m_strTimeName;

//	Serialize Members --------//
	int					m_nVer;
	int					m_nTRType;//0:FID 1:TR
	int					m_nCnt;
	int					m_nSortType;	//0: 1:
	int					m_nChartType;	//@solomon	통합버전용.
	int					m_nFXChartType;	//20100310_JS.Kim FX에서 매도/매수 여부. 0:매도, 1:매수
//	CStringList			m_propList;
	CPropertyMap		m_propMap;
	CString				m_strGridSymbol;
	int					m_nRealAbs;		//	실시간 데이터 부호로 내려오는지 여부.
	BOOL				m_bDirectSend;

	CArrTrQuery			m_arrTrQuery;
	CArrFidQuery		m_arrFidQuery;
//	CMapTrQuery			m_TrMap;
//	CMapFidQuery		m_FidMap;
//	--------------------------//

	CTrPack				m_inTrQry, m_outTrQry;

	CFidQuery			m_curFidQry;
	CFidQuery			m_defFidQry;

	// 조회 마지막 날자정보.
	CString				m_strLastDate[2];

	//TR조회시에 처리할 NextKey추가
	CString				m_strNextKey;

public:
	virtual void	Copy( CWinixConverter& item);//tour2k const 삭제
	virtual CWinixConverter& operator = ( CWinixConverter& item);//tour2k const 삭제

	void				SetParentWnd(CWnd* pWnd)	{ m_pParent = pWnd;	m_bRunMode = TRUE; };
	void				RemoveAll();

	BOOL				IsRunMode()				{ return m_bRunMode;	};
	void				InitDateTimeFID();


//	CTrPack*			GetQuery()		{ return &m_query;	};
	IPropertyMap*		GetPropMap()	{ return (IPropertyMap*)&m_propMap;	};
	void				SetPropMap(IPropertyMap* pPropMap);


	int					GetFidOutRowCnt()	{ return m_nRowCount;	};

	CString				GetMapFile()	{ return m_strMapFile;	};
	void				SetMapFile(CString strMapFile)	{ m_strMapFile = strMapFile;	};

	void				SetGridSymbol(CString strSymbol);
	CString				GetGridSymbol()	{ return m_strGridSymbol;	};
	void				SetGridSymbol2(CString strSymbol, CString strFIDCode);
	void				SetDiffGridSymbol(CString strFIDCode, CString strSymbol);
	void				SetRowMaxCount(CString strFIDCode, long lMaxCount);

	CArrTrQuery*		GetTrQryArray()		{ return &m_arrTrQuery;		};
	CArrFidQuery*		GetFidQryArray()	{ return &m_arrFidQuery;	};

	int					GetChartType()		{ return m_nChartType;		};
	int					GetFXChartType()	{ return m_nFXChartType;	};		//20100310_JS.Kim FX
	

	int					GetUnitValue(int nMinor);

public:
	void		Serialize(CArchive& ar);
	BOOL		SaveOnlyChartProperties(LPCSTR szSaveFile);		//save to file with Text Format.

	void		GetLineData(CString strData, CStringArray *pArr);
	char*		MakeInboundFIDLayout();
//	char*		GetFIDLayout();	//			{ return m_pszLayout;	};
	void		GetDataPropArr(CString strValue, int nSel, CStringArray &arr);

	CString		GetPacketNameList();
	long		GetRowPacketSize();
	BOOL		ConvReceiveData(char* pszChartData, CChartProp *pProp);
	void		ParsingDataToArr(const char* pszData, int nSize);

	void		ParsingTR(const char* pszData, int nSize);
	void		ParsingFID(const char* pszData, int nSize);

	void		SelectQuery(CString strQryID = _T(""));
	void		ResetFID();

	int			GetTrQuery(CString strQryID, CTrQuery &qry);
	int			GetFidQuery(CString strQryID, CFidQuery &qry);
	void		SetFidQuery(int nIndex, CFidQuery &qry)	{ m_arrFidQuery.SetAt(nIndex, qry);	};

//vntsorl_20110420:[A00000410] 맵차트에서 첫리얼에 새로운 봉이 생기는 오류수정
	int			GetBozoMsg(CString &strBozo, int nPacketType=0);

	CFidBlock*	GetDefaultFIDBlock(int nLeftSym = -1);

public:
	static	IMasterDataManagerLast	*m_pMaster;
	static	char	GetMarketTypeFromCode(CString strCode);
	static	BOOL	GetSiteUpKode(LPCSTR szCode, CString& rCode);

	static BOOL		TbondPrcLToF(char cType, int nPcnt, double dRate, CString &strData);
	static char		GetDecimalRate(CString strDecimalGB, int &nPcnt, double &dRate, CString &strResult);
	static double	DataSeparation(CString strData, int nMask);
	static CString	GetTimeIndexInfo(int nIndex, int nType);

	static	CMapStringToString		m_mapCodeType;
	static	CString	GetCodeType(CString strCode);
	static	void	SetCodeType(CString strCode, CString strType)	{ m_mapCodeType.SetAt(strCode, strType);	};

//<<20100310_JS.Kim FX
public:
	CString			m_strPacketType;
	CString			GetPacketType()			{ return m_strPacketType; };
//>>

	CString			GetLastDate(int nIndex)			{ return m_strLastDate[nIndex]; }

public:		//CME작업을 하면서 추가함.
	int				m_nFidTypeDate, m_nFidTypeTime;
	CString			m_szFidTypeDate, m_szFidTypeTime;
	//KHD 
	int				GetBozoMsgFromTr(CString &strBozo);
	CString			GetMarkeyTypeFromName(CString strMarketName);
public:
	int				SetTDCRDC( CString& strBojoMsg );
	void			SetBojoMsgData(CString strKey, CString strKeyData, CString &strBojoMsg);
	CString			ReflectPoint( CString strSrc );
};

#pragma	pack()

#endif // !defined(AFX_WINIXCONVERTER_H__16F42BCA_D645_4B9B_978C_E7AC60B1F611__INCLUDED_)
