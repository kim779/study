// FileBuilder.h: interface for the CFileBuilder class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_FILEBUILDER_H__A6E90D73_0AAB_4FB2_A817_144E654B0915__INCLUDED_)
#define AFX_FILEBUILDER_H__A6E90D73_0AAB_4FB2_A817_144E654B0915__INCLUDED_

#include "../Include_Chart/DLL_Load/IndicatorConditionDefineData.h"
#include "../Include_Chart/DLL_Load/IndicatorList.h"					// for EIndicatorListType
#include "../Include_Chart/MapStringToWord.h"							// for CMapStringToWord

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CFileBuilder  
{
public:
	// file 을 open 하고 data를 읽는다.
	char *	FileRead( const char *p_szFilePathAndName, CString &p_strFileData) const;
	// file 을 생성하고 data를 쓴다.
	bool	FileCreatWrite( const char *p_szFilePathAndName, CStringList &p_slStringData);
};


// ============================================================================
class CIndicatorList;
class CGraphTypeDataList;
class CBaseIndicatorList;
class CDefaultIndicatorList;
class CUserIndicatorList;
class CGraphTypeAndStyleData;
class CGraphTypeData;
class CGraphPartData;
class CGraphData;
class CGraphDataDrawingData;
class CSubGraphData;
class CPenThickness;
class CGraph;		// 복수종목 불러오기 : 복수종목 - ojtaso (20070514)

// indicator data file data 객체로 만들기
#define	_ITEMNAME_START			'['
#define	_ITEMNAME_MIDDLE_END	':'
#define	_ITEMNAME_END			']'
class CIndicatorBuilder
{
public:
	CIndicatorBuilder( HWND p_hOcxWnd)
		{	m_hOcxWnd = p_hOcxWnd;	}

protected:
	char *	GetItemName( char *&p_szDataString) const;

// (2008/1/14 - Seung-Won, Bae) for MultiLanguage.
protected:
	HWND	m_hOcxWnd;
};

// -> graph tyle or style 관련 내용
#define	_GRAPH_TYPE_START	"***** Graph Type *****\r\n"
#define	_GRAPH_TYPE_END		"***** Graph Type End *****\r\n"
#define	_GRAPH_STYLE_START	"***** Graph Draw Style *****\r\n"
#define	_GRAPH_STYLE_END	"***** Graph Draw Style End *****\r\n"

class CGraphTypeOrStyleDataBuilder : public CIndicatorBuilder
{
protected:
	static const int nLen_GRAPH_TYPE_START;
	static const int nLen_GRAPH_TYPE_END;
	static const int nLen_GRAPH_STYLE_START;
	static const int nLen_GRAPH_STYLE_END;

public:
	CGraphTypeOrStyleDataBuilder() : CIndicatorBuilder( NULL)	{}

public:
	bool FileDataBuild_GraphType( char *&p_szDataString, CGraphTypeDataList& graphTypeDataList);
	bool FileDataBuild_GraphStyle( char *&p_szDataString, CGraphTypeDataList& graphStyleDataList);

private:
	bool SetGraphTypeDataList( char *&p_szDataString, CGraphTypeDataList& graphTypeDataList);
	void SetGraphStyleData( char *&p_szDataString, CStringDataList& styleDataList);

	CGraphTypeData* MakeGraphTypeData( char *&p_szDataString);
};


// ============================================================================
// -> indicatorList : default file
#define	_GENERAL_START	"***** General *****\r\n"
#define	_GENERAL_END	"***** General End *****\r\n"
#define	_SPECIAL_START	"***** Special *****\r\n"
#define	_SPECIAL_END	"***** Special End *****\r\n"
#define	_NONEINDE_START	"***** None Indicator *****\r\n"
#define	_NONEINDE_END	"***** None Indicator End *****\r\n"
class CIndicatorInfoMap;
class CDefaultIndicatorListBuilder : public CIndicatorBuilder
{
// (2006/5/27 - Seung-Won, Bae) Parsing Seperators
protected:
	static const int nLen_GENERAL_START;
	static const int nLen_GENERAL_END;
	static const int nLen_SPECIAL_START;
	static const int nLen_SPECIAL_END;
	static const int nLen_NONEINDE_START;
	static const int nLen_NONEINDE_END;

public:
	CDefaultIndicatorListBuilder( HWND p_hOcxWnd)	: CIndicatorBuilder( p_hOcxWnd)		{}

// (2006/5/27 - Seung-Won, Bae) Add 'General/Special' Indicator Info from CFG Info List String to List
public:
	virtual bool FileDataBuild( CBaseIndicatorList* pBaseIndiList, char *&p_szDataString);

// (2006/5/27 - Seung-Won, Bae) Add Indicator Info from CFG Info List String to List
public:
	bool IndicatorListBuild( CIndicatorInfoMap *p_pIndiInfoMap, char *&p_szDataString);

// (2006/5/27 - Seung-Won, Bae) Create Indicator Info Object from CFG Info String
protected:
	virtual CIndicatorInfo* MakeIndicatorInfo( EIndicatorInfoType p_eIndicatorInfoType, char *&p_szDataString);

// (2006/5/27 - Seung-Won, Bae) Set Indicator Info Data from CFG Info String to Info Object.
protected:
	virtual bool ReadInfoCfgStrToObject(CIndicatorInfo* pIndicatorInfo, char *&p_szDataString) = 0;
};

// ----------------------------------------------------------------------------
// -> indicatorList : default file 형태
class CDefaultTypeIndiListBuilder : public CDefaultIndicatorListBuilder
{
public:
	CDefaultTypeIndiListBuilder( CGraphTypeAndStyleData* pGraphTypeAndStyleData, HWND p_hOcxWnd);

// (2006/5/27 - Seung-Won, Bae) Site Default Info Loading needs CGraphTypeAndStyleData Manager. (why?)
protected:
	CGraphTypeAndStyleData* m_pGraphTypeAndStyleData;

// (2006/5/27 - Seung-Won, Bae) Set Indicator Info Data from CFG Info String to Info Object.
//		It is overrided for Site Default CFG Info String
protected:
	virtual bool ReadInfoCfgStrToObject(CIndicatorInfo* pIndicatorInfo, char *&p_szDataString);
};

// -> indicatorList : user file 형태
// -> indicatorList : user type 이 default type 형태 (user file에만 해당)
class CDefaultUserTypeIndiListBuilder : public CDefaultIndicatorListBuilder
{
public:
	CDefaultUserTypeIndiListBuilder( HWND p_hOcxWnd) : CDefaultIndicatorListBuilder( p_hOcxWnd)		{}

// (2006/5/27 - Seung-Won, Bae) Set Indicator Info Data from CFG Info String to Info Object.
//		It is overrided for User CFG Info String
protected:
	virtual bool ReadInfoCfgStrToObject(CIndicatorInfo* pIndicatorInfo, char *&p_szDataString);

// (2006/5/27 - Seung-Won, Bae) Make CFG String for Writing of User Indicator Info.
public:
	void	GetWritingFileData( const CUserIndicatorList& userIndiList, CStringList &p_slDataString) const;
protected:
	void	GetWritingFileData( EIndicatorListType p_eIndicatorListType, const CIndicatorInfoMap *p_pIndiInfoMap, CStringList &p_slDataString) const;
};

// -> indicatorList : user type 이 default type 이전 형태 (user file에만 해당)
class CNotDefaultUserTypeIndiListBuilder : public CDefaultIndicatorListBuilder
{
public:
	CNotDefaultUserTypeIndiListBuilder( CDefaultIndicatorList* pDefaultIndicatorList, HWND p_hOcxWnd);

private:
	CDefaultIndicatorList* m_pDefaultIndicatorList;

// (2006/5/28 - Seung-Won, Bae) CMap for Managing Version Up Operation List with Indicator Name.
protected:
	BOOL					m_bInited;
	CMapStringToString		m_mapVersionUpOperation;

// (2006/5/27 - Seung-Won, Bae) Add 'General/Special' Indicator Info from CFG Info List String to List
// (2006/5/30 - Seung-Won, Bae) Override for Registering and Denying the previous Operation
//		Caution! You should call the base member function in the end of routine.
public:
	virtual bool FileDataBuild( CBaseIndicatorList* pBaseIndiList, char *&p_szDataString);
protected:
	virtual void RegisterPrevOperation( void)	{}
	virtual void DenyPrevOperation( void)		{}

// (2006/5/27 - Seung-Won, Bae) Create Indicator Info Object from CFG Info String
//		It is overrided for Merging with New Version Info
//		It will call UpdateInfoCfgStrToObject() with Site Default CFG Info Copy Object, Not New Empty Object.
protected:
	virtual CIndicatorInfo* MakeIndicatorInfo( EIndicatorInfoType p_eIndicatorInfoType, char *&p_szDataString);
// (2006/5/27 - Seung-Won, Bae) Set Indicator Info Data from CFG Info String to Info Object.
//		It is overrided for User CFG Info String
protected:
	virtual bool ReadInfoCfgStrToObject(CIndicatorInfo* pIndicatorInfo, char *&p_szDataString)	{	return false;	}

// (2006/5/27 - Seung-Won, Bae) Update Indicator Info Object from Old Version CFG Info String.
//		It does		1. Create Old Indicator Info Object
//					2. Version Up Old Indicator Info Object by VersionUpOldInfo().
//					3. Update New Indicator Info Object by CopyDataWithVerificationFrom().
protected:
	virtual bool UpdateInfoCfgStrToObject(CIndicatorInfo* pIndicatorInfo, char *&p_szDataString, const char *p_szName);

// (2006/5/27 - Seung-Won, Bae) Make Old Version Info to New Version Type for Next Version Up.
//		Version Up Target	:	The Item that is supported in Copy with verification. (look IndicatorInfo.h)
//									and It can be changed by User in Old Version.
//									and You want it's changing not to work in new version.
//								Or The Item that is not supported in Copy with verification. (look IndicatorInfo.h)
//									and Customer want it's changing to work in new version.
//		Caution!	1. VersionUpOldInfo() of Base Class must be called for Next Version Up.
//					2. Do Verion Up by Item Unit in Each Indicator Info with Indicator Info Name.
//							Because Next Version Up must control the Previous Version Up Operation.
//					3. return FALSE in only purpose not to load Old Version CFG Info.
protected:
	virtual BOOL VersionUpOldInfo( CIndicatorInfo* pTargetInfo, CIndicatorInfo* pSourceInfo)	{	return true;	}
};


// (2008/6/13 - Seung-Won, Bae) for BuySellOverhaningSupply's Draw Option
#define _V5_20_BUY_SELL_OVERHANGING_SUPPLY_DRAW_CND						"v5.20:BuySellOverhangingSupply:draw_cnd"
class CUserVer5Dot20TypeIndiListBuilder : public CNotDefaultUserTypeIndiListBuilder
{
public:
	CUserVer5Dot20TypeIndiListBuilder( CDefaultIndicatorList* pDefaultIndicatorList, HWND p_hOcxWnd);

protected:
	typedef enum
	{
		E_V5_20_BUY_SELL_OVERHANGING_SUPPLY,
	};

protected:
	CMapStringToWord m_mapVersionUpIndicators;

//		Caution! You should call the base member function in the end of routine.
protected:
	virtual void RegisterPrevOperation( void);
	virtual void DenyPrevOperation( void)		{	CNotDefaultUserTypeIndiListBuilder::DenyPrevOperation();	}

protected:
	virtual BOOL VersionUpOldInfo(CIndicatorInfo* pTargetInfo, CIndicatorInfo* pSourceInfo);
};


// (2006/10/13 - Seung-hwan, Ahn) Version Up for Each version and Call Next Version.
#define _V5_10_VOLUME_FOR_SALE_CAL_CND						"v5.10:OverhangingSupply:cal_cnd"			// Add cal_cnd.

class CUserVer5Dot10TypeIndiListBuilder : public CUserVer5Dot20TypeIndiListBuilder	// CUserVer5Dot00TypeIndiListBuilder
{
public:
	CUserVer5Dot10TypeIndiListBuilder( CDefaultIndicatorList* pDefaultIndicatorList, HWND p_hOcxWnd);

protected:
	typedef enum
	{
		E_V5_10_VOLUME_FOR_SALE,
	};

protected:
	CMapStringToWord m_mapVersionUpIndicators;

//		Caution! You should call the base member function in the end of routine.
protected:
	virtual void RegisterPrevOperation( void);
	virtual void DenyPrevOperation( void)		{	CUserVer5Dot20TypeIndiListBuilder::DenyPrevOperation();	}

protected:
	virtual BOOL VersionUpOldInfo(CIndicatorInfo* pTargetInfo, CIndicatorInfo* pSourceInfo);

protected:
//	void	SetMAComCalToSubCal( CIndicatorInfo* pSourceInfo);
//	void	AddSignalCalculateCondition( CIndicatorInfo* pTargetInfo, CIndicatorInfo* pSourceInfo);
//	void	RemoveOneCalculateCondition( CIndicatorInfo* pSourceInfo);
};

//	(2006.09.07,안승환) 
// (2006/9/5 ) Load V5.0
#define _V5_00_PANDF_CAL_CND						"v5.00:PANDF:cal_cnd"						// Add cal_cnd.
#define _V5_00_PANDF_DARW_CND					    "v5.00:PANDF:darw_cnd"						// Add darw_cnd.

class CUserVer5Dot00TypeIndiListBuilder : public CUserVer5Dot10TypeIndiListBuilder	// CUserVer5Dot00TypeIndiListBuilder
{
public:
	CUserVer5Dot00TypeIndiListBuilder( CDefaultIndicatorList* pDefaultIndicatorList, HWND p_hOcxWnd);

protected:
	typedef enum
	{
		E_V5_00_PANDF,
		E_V5_00_THREELINEBREAK,
	};

protected:
	CMapStringToWord m_mapVersionUpIndicators;

//		Caution! You should call the base member function in the end of routine.
protected:
	virtual void RegisterPrevOperation( void);
	virtual void DenyPrevOperation( void)		{	CUserVer5Dot10TypeIndiListBuilder::DenyPrevOperation();	}

protected:
	virtual BOOL VersionUpOldInfo(CIndicatorInfo* pTargetInfo, CIndicatorInfo* pSourceInfo);

protected:
//	void	SetMAComCalToSubCal( CIndicatorInfo* pSourceInfo);
//	void	AddSignalCalculateCondition( CIndicatorInfo* pTargetInfo, CIndicatorInfo* pSourceInfo);
	void	RemoveOneCalculateCondition( CIndicatorInfo* pSourceInfo);
};
// (2006/5/28 - Seung-Won, Bae) In this version up.
// (2006/5/31 - Seung-Won, Bae) Cuation! in V3.21, The Info of User Packet Changing is not saved in User IndiCond CFG.
//		It is saved in User Env CFG.
#define _V3_21_PRICE_GRAPHDATADRAWING		"v3.21:PriceChart:graphdatadrawing"		// LH;LC;HC;PC;전일비; -> LH;LC;HC;전일비;현재가;
#define _V3_21_PRICE_MINMAXDATADRAWING		"v3.21:PriceChart:minmaxdatadrawing"	// new Item. Set Initial Value.
#define _V3_21_PRICE_MA_CAL_CND				"v3.21:Price MA:cal_cnd"				// Set cal_cnd to sub_cal_cnd
#define _V3_21_VOLUME_DRAW_CND				"v3.21:VolumeChart:draw_cnd"			// Add Dummy Draw Condition.
#define _V3_21_VOLUME_MA_CAL_CND			"v3.21:Volume MA:cal_cnd"				// Set cal_cnd to sub_cal_cnd
#define _V3_21_GLANCEBALANCE_CAL_CND		"v3.21:ObviouslyBalanceChart:cal_cnd"	// Change cal_cnd inner-order.
#define _V3_21_ADX_CAL_CND					"v3.21:ADX:cal_cnd"						// Remove adxr cal_cnd.
#define _V3_21_ATR_CAL_CND					"v3.21:ATR:cal_cnd"						// Add signal cal_cnd.
#define _V3_21_BAND_PERCENT_B_CAL_CND		"v3.21:Band %B:cal_cnd"					// Add signal cal_cnd.
#define _V3_21_DPO_CAL_CND					"v3.21:DPO:cal_cnd"						// Add signal cal_cnd.
#define _V3_21_DPO_COLOR					"v3.21:DPO:color"						// Set dpo color to dpo_signal color for base line color
#define _V3_21_ENVELOPE_CAL_CND				"v3.21:Envelope:cal_cnd"				// Add lower bound cal_cnd with upper bound cal_cnd.
#define _V3_21_LRS_CAL_CND					"v3.21:LRS:cal_cnd"						// Add signal cal_cnd.
#define _V3_21_MAC_CAL_CND					"v3.21:MAC:cal_cnd"						// Add upper & lower ratio cal_cnd.
#define _V3_21_MACD_CAL_CND					"v3.21:MACD:cal_cnd"					// Remove ma_type cal_cnd.
#define _V3_21_STANDARD_DEVIATION_CAL_CND	"v3.21:Standard Deviation:cal_cnd"		// Remove ma_type cal_cnd.
#define _V3_21_ZIG_ZAG_CAL_CND				"v3.21:Zig Zag:cal_cnd"					// Remove cal_type cal_cnd.
class CUserVer3Dot21TypeIndiListBuilder : public CUserVer5Dot00TypeIndiListBuilder
{
public:
	CUserVer3Dot21TypeIndiListBuilder( CDefaultIndicatorList* pDefaultIndicatorList, HWND p_hOcxWnd);

// (2006/5/29 - Seung-Won, Bae) Manage Chart Name to check the Target for Version Up.
protected:
	typedef enum
	{
		E_V3_21_PRICE,
		E_V3_21_PRICE_MA,
		E_V3_21_VOLUME,
		E_V3_21_VOLUME_MA,
		E_V3_21_GLANCEBALANCE,
		E_V3_21_ADX,
		E_V3_21_ATR,
		E_V3_21_BAND_PERCENT_B,
		E_V3_21_DPO,
		E_V3_21_ENVELOPE,
		E_V3_21_LRS,
		E_V3_21_MAC,
		E_V3_21_MACD,
		E_V3_21_STANDARD_DEVIATION,
		E_V3_21_ZIG_ZAG,
	};
protected:
	CMapStringToWord m_mapVersionUpIndicators;

// (2006/5/30 - Seung-Won, Bae) Override for Registering and Denying the previous Operation
//		Caution! You should call the base member function in the end of routine.
protected:
	virtual void RegisterPrevOperation( void);
	virtual void DenyPrevOperation( void)		{	CUserVer5Dot00TypeIndiListBuilder::DenyPrevOperation();	}

// (2006/5/27 - Seung-Won, Bae) Make Old Version Info to New Version Type for Next Version Up.
//		Version Up Target	:	The Item that is supported in Copy with verification. (look IndicatorInfo.h)
//									and It can be changed by User in Old Version.
//									and You want it's changing not to work in new version.
//								Or The Item that is not supported in Copy with verification. (look IndicatorInfo.h)
//									and Customer want it's changing to work in new version.
//		Caution!	1. VersionUpOldInfo() of Base Class must be called for Next Version Up.
//					2. Do Verion Up by Item Unit in Each Indicator Info with Indicator Info Name.
//							Because Next Version Up must control the Previous Version Up Operation.
//					3. return FALSE in only purpose not to load Old Version CFG Info.
protected:
	virtual BOOL VersionUpOldInfo(CIndicatorInfo* pTargetInfo, CIndicatorInfo* pSourceInfo);

protected:
	// (2006/5/30 - Seung-Won, Bae) Price and Volume MA's Calculate Condition Processing Routine (to Sub Calculate Condition)
	void	SetMAComCalToSubCal( CIndicatorInfo* pSourceInfo);
	// (2006/5/31 - Seung-Won, Bae) Add signal cal_cnd.
	void	AddSignalCalculateCondition( CIndicatorInfo* pTargetInfo, CIndicatorInfo* pSourceInfo);
	// (2006/5/31 - Seung-Won, Bae) Add remove MA-Type cal_cnd.
	void	RemoveOneCalculateCondition( CIndicatorInfo* pSourceInfo);

// (2006/6/7 - Seung-Won, Bae) Override for Moving of VolumeForSale Chart, from Special to General
// (2006/12/3 - Seung-Won, Bae) Comment for Multiple Graph of the Indicator Info.
//public:
//	virtual bool	FileDataBuild( CBaseIndicatorList* pBaseIndiList, char *&p_szDataString);
};

// ============================================================================
// indicator info

#define	_GROUPNAME					"groupName="
#define	_GRAPHNAME					"graphName="				// (2006/11/29 - Seung-Won, Bae) Support Graph Name for Binding with Indicator Info.
#define	_GRAPHRQ					"graphRQ="					// 복수종목 저장 : 복수종목 - ojtaso (20070514)
#define	_CAL_CND					"cal_cnd="
#define	_DRAW_CND					"draw_cnd="
#define	_BASELINE					"baseline="
#define	_PACKET						"packet="
#define	_SIGNAL						"signal="
#define	_GRAPHDATADRAWING			"graphdatadrawing="
#define	_MINMAXDATADRAWING			"minmaxdatadrawing="
#define	_SUBGRAPH_START				"<SubGraph>\r\n"
#define	_SUBGRAPH_END				"<SubGraphEnd>\r\n"
#define	_SUB_PACKET					"sub_packet="
#define	_SUB_CAL_CND				"sub_cal_cnd="
#define	_SUBGRAPH_NAME				"name="
#define	_SUBGRAPH_TITLE				"title="
#define	_SUBGRAPH_HIDE				"hide="
#define	_SUBGRAPH_COLOR				"color="
#define	_SUBGRAPH_LINETHICKNESS		"linethickness="
#define	_EXPLANATION				"(ex:"
class CDefaultIndicatorInfoBuilder : public CIndicatorBuilder
{
protected:
	static const int nLen_GRAPHNAME;
	static const int nLen_GROUPNAME;
	static const int nLen_GRAPHRQ;		// 복수종목 저장 : 복수종목 - ojtaso (20070514)
	static const int nLen_CAL_CND;
	static const int nLen_DRAW_CND;
	static const int nLen_BASELINE;
	static const int nLen_PACKET;
	static const int nLen_SIGNAL;
	static const int nLen_GRAPHDATADRAWING;
	static const int nLen_MINMAXDATADRAWING;
	static const int nLen_SUBGRAPH_START;
	static const int nLen_SUBGRAPH_END;
	static const int nLen_SUB_PACKET;
	static const int nLen_SUB_CAL_CND;
	static const int nLen_SUBGRAPH_NAME;
	static const int nLen_SUBGRAPH_TITLE;
	static const int nLen_SUBGRAPH_HIDE;
	static const int nLen_SUBGRAPH_COLOR;
	static const int nLen_SUBGRAPH_LINETHICKNESS;
	static const int nLen_EXPLANATION;

public:
	CDefaultIndicatorInfoBuilder( HWND p_hOcxWnd)	: CIndicatorBuilder( p_hOcxWnd)		{}

public:
	virtual bool FileDataBuild( CIndicatorInfo *p_pIndicatorInfo, char *&p_szDataString, const char *p_szName = NULL);
	// 복수종목 불러오기 : 복수종목 - ojtaso (20070514)
	virtual bool FileDataBuild_Graph( char *&p_szDataString, CIndicatorInfo* p_pIndicatorInfo, CGraphPartData& graphPartData, const char *p_szName);
	virtual bool FileDataBuild_CalCondition( char *&p_szDataString, CDoubleList& calConditionList);
	virtual bool FileDataBuild_DrawCondition( char *&p_szDataString, CDoubleList& drawConditionList);
	virtual bool FileDataBuild_BaseLine( char *&p_szDataString, CDoubleList& baseLineList);
	virtual bool FileDataBuild_DoubleList( CIndicatorInfo *p_pIndicatorInfo, char *&p_szDataString);

protected:
	// Singbar Graph 색상 비교기준 추가 - ojtaso (20071121)
	// 기준선 두께, 모양 설정 - ojtaso (20071213)
	virtual bool GetSubGraphTypeAndStyle( char *&p_szDataString, int& nType, int& nStyle, int& nDrawStyle, int& nPaintStyle, int& nBaseLineStyle) const = 0;

private:
	CSubGraphPartData* MakeSubGraphPartData( char *&p_szDataString, const char *p_szIndicatorName);
	bool AddTail_SubGraphData( char *&p_szDataString, CGraphPartData& graphPartData);
	bool AddTail_DoubleList( char *&p_szDataString, const char *p_szKey, int p_nLenKey, CDoubleList& doubleList);
	void AddTail_DoubleData( char *&p_szDataString, CDoubleList& doubleList);

	void SetGroupData( char *&p_szDataString, CGraphPartData& graphPartData);
	// 복수종목 불러오기 : 복수종목 - ojtaso (20070514)
	bool SetGraphData( char *&p_szDataString, CIndicatorInfo* p_pIndicatorInfo, CGraphData& graphData, const char *p_szName);
	bool SetSubGraphData( char *&p_szDataString, CSubGraphData& subGraphData, const char *p_szIndicatorName);
	bool SetSubGraphType( char *&p_szDataString, CSubGraphData& subGraphData);
	void SetSubGraphNameAndTitle( char *&p_szDataString, CSubGraphData& subGraphData, const char *p_szIndicatorName);
	void SetSubGraphHiding( char *&p_szDataString, CSubGraphData& subGraphData);
	void SetSubGraphColorAndFill( char *&p_szDataString, CSubGraphData& subGraphData);
	void SetSubGraphThickness( char *&p_szDataString, CSubGraphData& subGraphData);
	bool SetSubGraphPacketNames( char *&p_szDataString, CSubGraphPartData* pSubGraphPartData);
	bool SetSubGraphConditionData( char *&p_szDataString, CDoubleList& doubleList);
};

// -> indicatorList : default file 형태
#define	_SUBGRAPH_TYPE	"type="
class CDefaultTypeIndiInfoBuilder : public CDefaultIndicatorInfoBuilder
{
public:
	CDefaultTypeIndiInfoBuilder( CGraphTypeAndStyleData* pGraphTypeAndStyleData, HWND p_hOcxWnd);

private:
	static const int nLen_SUBGRAPH_TYPE;
	CGraphTypeAndStyleData* m_pGraphTypeAndStyleData;

protected:
	// Singbar Graph 색상 비교기준 추가 - ojtaso (20071121)
	// 기준선 두께, 모양 설정 - ojtaso (20071213)
	virtual bool GetSubGraphTypeAndStyle( char *&p_szDataString, int& nType, int& nStyle, int& nDrawStyle, int& nPaintStyle, int& nBaseLineStyle) const;
};

// -> indicatorList : user file 형태
#define	_SUBGRAPH_TYPE_INT	"type_int="
class CUserTypeIndiInfoBuilder : public CDefaultIndicatorInfoBuilder
{
// (2006/11/30 - Seung-Won, Bae) Support Indicator List Type Manage.
public:
	CUserTypeIndiInfoBuilder( HWND p_hOcxWnd, EIndicatorListType p_eIndicatorListType = ( EIndicatorListType)-1) : CDefaultIndicatorInfoBuilder( p_hOcxWnd)
		{	m_eIndicatorListType = p_eIndicatorListType;	}
protected:
	EIndicatorListType	m_eIndicatorListType;

private:
	static const int nLen_SUBGRAPH_TYPE_INT;
protected:
	// Singbar Graph 색상 비교기준 추가 - ojtaso (20071121)
	// 기준선 두께, 모양 설정 - ojtaso (20071213)
	virtual bool GetSubGraphTypeAndStyle( char *&p_szDataString, int& nType, int& nStyle, int& nDrawStyle, int& nPaintStyle, int& nBaseLineStyle) const;

public:
	void	GetWritingFileData( const CIndicatorInfo& indicatorInfo, CStringList &p_slDataString) const;
private:
	bool	IsWritingFileData(const CGraphPartData* pGraphPartData) const;
	void	GetWritingFileData_IndicatorName(const CGraphData& graphData, CStringList &p_slDataString) const;
	void	GetWritingFileData_GroupName(const CGraphPartData* pGraphPartData, CStringList &p_slDataString) const;
	void	GetWritingFileData_DoubleList( const char *p_szItemName, const CDoubleList* pDoubleList, CStringList &p_slDataString) const;
	// 복수종목 저장 : 복수종목 - ojtaso (20070514)
	void	GetWritingFileData_Graph(LPCTSTR lpszRQ, const CGraphData& graphData, CStringList &p_slDataString) const;
	void	GetWritingFileData_SubGraphPart(const CSubGraphPartDataList* pSubGraphDataList, CStringList &p_slDataString) const;
	void	GetWritingFileData_SubGraphPart(const CSubGraphPartData* pSubGraphPartData, CStringList &p_slDataString) const;
	void	GetWritingFileData_SubGraph(const CSubGraphData& subGraphData, CStringList &p_slDataString) const;
	void	GetSubGraphType(const CSubGraphData& subGraphData, CStringList &p_slDataString) const;
	void	GetSubGraphNameAndTitle(const CSubGraphData& subGraphData, CStringList &p_slDataString) const;
	void	GetSubGraphHiding(const CSubGraphData& subGraphData, CStringList &p_slDataString) const;
	void	GetSubGraphColorAndFill(const CSubGraphData& subGraphData, CStringList &p_slDataString) const;
	void	GetSubGraphThickness(const CPenThickness& penThickness, CStringList &p_slDataString) const;
	void	GetCalPacketNames(const CString& strCalPacketNames, CStringList &p_slDataString) const;

};

#endif // !defined(AFX_FILEBUILDER_H__A6E90D73_0AAB_4FB2_A817_144E654B0915__INCLUDED_)
