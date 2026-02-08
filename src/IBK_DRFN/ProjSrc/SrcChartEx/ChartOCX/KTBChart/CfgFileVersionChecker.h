// CfgFileVersionChecker.h: interface for the CCfgFileVersionChecker class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CFGFILEVERSIONCHECKER_H__FF40B197_321E_4BA6_A35E_AF73A9E825D5__INCLUDED_)
#define AFX_CFGFILEVERSIONCHECKER_H__FF40B197_321E_4BA6_A35E_AF73A9E825D5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "../Include_Chart/MapStringToWord.h"	// for CMapStringToWord

class CKTBChartCtrl;
class CMainBlock;
class CIndicatorList;
class CIndicatorInfo;

//sy 2004.2.13.
class CCfgFileVersionChecker  
{
public:
	bool	UserFileVersionChecker(CKTBChartCtrl* pChartCtrl, CMainBlock* pMainBlock, bool& bIsCalculate);

protected:
	// user file의 version 가져오기
	bool GetUserVersion(CMainBlock* pMainBlock, double& dDefaultVer, double& dUserVer) const;
};

//-----------------------------------------------------------------------------
// Author		: Seung-Won, Bae	Date :2006/6/2
// Comments		: Base class of Version Up Processing Class
// Using way	: 
// See			: 
//-----------------------------------------------------------------------------
class CCfgFileVersionBase
{
public:
	CCfgFileVersionBase();

// (2006/5/31 - Seung-Won, Bae) Manage Target Indicator Info Name
protected:
	BOOL				m_bInited;
	CMapStringToString	m_mapVersionUpOperation;
// (2006/5/30 - Seung-Won, Bae) Override for Registering and Denying the previous Operation
//		Caution! You should call the base member function in the end of routine.
protected:
	virtual void RegisterPrevOperation( void)	{}
	virtual void DenyPrevOperation( void)		{}

public:
	BOOL			UserFileVersionChecker( CKTBChartCtrl* pChartCtrl, CMainBlock* pMainBlock, bool &p_bSubGraphChanged);

// (2006/6/2 - Seung-Won, Bae) Version Up for Each version and Call Next Version.
protected:
	virtual BOOL	GetNewGraphForDeletedOld( const char *p_szOldGraphName, CString &p_strNewGraphName, BOOL p_bPrevResult = FALSE)		{	return p_bPrevResult;	}
	virtual	BOOL	VersionUpOldInfo( const char *szGraphName, CIndicatorInfo *pSourceInfo, CKTBChartCtrl* pChartCtrl, CMainBlock* pMainBlock, bool &p_bSubGraphChanged)		{	return FALSE;	}
};


#define _V5_00_PANDF_CAL_CND						"v5.00:PANDF:cal_cnd"						// Add cal_cnd.
#define _V5_00_PANDF_DARW_CND					    "v5.00:PANDF:darw_cnd"						// Add darw_cnd.
#define _V5_00_THREELINEBREAK_FILL				    "v5.00:THREELINEBREAK:fill"					// Add fill.
// user file version (5.00) && default file version (4.3 or 4.4)
class CCfgFileVersion5Dot00 : public CCfgFileVersionBase
{
public:
	CCfgFileVersion5Dot00();

// (2006/5/31 - Seung-Won, Bae) Manage Target Indicator Info Name
protected:
	typedef enum
	{
		E_V5_00_PANDF,
		E_V5_00_THREELINEBREAK,
	};

protected:
	CMapStringToWord	m_mapVersionUpIndicators;

// (2006/5/30 - Seung-Won, Bae) Override for Registering and Denying the previous Operation
//		Caution! You should call the base member function in the end of routine.
protected:
	virtual void RegisterPrevOperation( void);
	virtual BOOL	GetNewGraphForDeletedOld( const char *p_szOldGraphName, CString &p_strNewGraphName, BOOL p_bPrevResult);
	virtual	BOOL	VersionUpOldInfo( const char *szGraphName, CIndicatorInfo *pSourceInfo, CKTBChartCtrl* pChartCtrl, CMainBlock* pMainBlock, bool &p_bSubGraphChanged);
};

// user file version (3.21) && default file version (4.3)
#define _V3_21_PRICE_MA_PACKET				"v3.21:Price MA:packet"				// User select packet
#define _V3_21_RAINBOW_PACKET				"v3.21:Rainbow Chart:packet"		// User select packet
#define _V3_21_BAND_PERCENT_B_PACKET		"v3.21:Band %B:packet"				// User select packet
#define _V3_21_BAND_WIDTH_PACKET			"v3.21:Band Width:packet"			// User select packet
#define _V3_21_BOLLINGER_BANDS_PACKET		"v3.21:Bollinger Bands:packet"		// User select packet
#define _V3_21_CCI_PACKET					"v3.21:CCI:packet"					// User select packet
#define _V3_21_DPO_PACKET					"v3.21:DPO:packet"					// User select packet
#define _V3_21_ENVELOPE_PACKET				"v3.21:Envelope:packet"				// User select packet
#define _V3_21_LRS_PACKET					"v3.21:LRS:packet"					// User select packet
#define _V3_21_MACD_PACKET					"v3.21:MACD:packet"					// User select packet
#define _V3_21_MACD_OSCILLATOR_PACKET		"v3.21:MACD Oscillator:packet"		// User select packet
#define _V3_21_MOMENTUM_PACKET				"v3.21:Momentum:packet"				// User select packet
#define _V3_21_NCO_PACKET					"v3.21:NCO:packet"					// User select packet
#define _V3_21_RSI_PACKET					"v3.21:RSI:packet"					// User select packet
#define _V3_21_STANDARD_DEVIATION_PACKET	"v3.21:Standard Deviation:packet"	// User select packet
#define _V3_21_STANDARD_ERROR_PACKET		"v3.21:Standard Error:packet"		// User select packet
#define _V3_21_TRIX_PACKET					"v3.21:TRIX:packet"					// User select packet
#define _V3_21_VHF_PACKET					"v3.21:VHF:packet"					// User select packet
#define _V3_21_ZIG_ZAG_PACKET				"v3.21:Zig Zag:packet"				// User select packet
#define _V3_21_VOLUME_FOR_SALE_PACKET		"v3.21:Overhanging Supply:packet"	// User select packet
class CCfgFileVersion3Dot21 : public CCfgFileVersion5Dot00
{
public:
	CCfgFileVersion3Dot21();

// (2006/5/31 - Seung-Won, Bae) Manage Target Indicator Info Name
protected:
	typedef enum
	{
		E_V3_21_PRICE_MA,				// SubGraph count, User select packet
		E_V3_21_VOLUME_MA,				// SubGraph count
		E_V3_21_RAINBOW,				// User select packet
		E_V3_21_ABRATIO,				// Signal
		E_V3_21_BAND_PERCENT_B,			// User select packet
		E_V3_21_BAND_WIDTH,				// User select packet
		E_V3_21_BOLLINGER_BANDS,		// User select packet
		E_V3_21_CCI,					// User select packet and Signal
		E_V3_21_DPO,					// User select packet
		E_V3_21_ENVELOPE,				// User select packet
		E_V3_21_LRS,					// User select packet
		E_V3_21_MACD,					// User select packet and Signal
		E_V3_21_MACD_OSCILLATOR,		// User select packet
		E_V3_21_MFI,					// Default packet and Signal
		E_V3_21_MOMENTUM,				// User select packet and Signal
		E_V3_21_NCO,					// User select packet
		E_V3_21_NVI,					// Default packet.
		E_V3_21_OBV,					// Default packet.
		E_V3_21_RSI,					// User select packet and Signal
		E_V3_21_STANDARD_DEVIATION,		// User select packet
		E_V3_21_STANDARD_ERROR,			// User select packet
		E_V3_21_TRIX,					// User select packet and Signal
		E_V3_21_VHF,					// User select packet
		E_V3_21_ZIG_ZAG,				// User select packet
		E_V3_21_VOLUME_FOR_SALE,		// Return None-Special Chart.
		E_V3_21_OLD_PSYCHCHART,			// New Graph
		E_V3_21_OLD_DISPARITYCHART,		// New Graph
		E_V3_21_OLD_ADLINE,				// New Graph
		E_V3_21_OLD_CO,					// New Graph
		E_V3_21_OLD_MI,					// New Graph
		E_V3_21_OLD_PRICEOSCILLATOR,	// New Graph
		E_V3_21_OLD_ROC,				// New Graph
		E_V3_21_OLD_SIGMA,				// New Graph
		E_V3_21_OLD_SONAR,				// New Graph
		E_V3_21_OLD_STOCHASTICS_FAST,	// New Graph
		E_V3_21_OLD_STOCHASTICS_SLOW,	// New Graph
		E_V3_21_OLD_WILLIAM,			// New Graph
	};
protected:
	CMapStringToWord	m_mapVersionUpIndicators;

// (2006/5/30 - Seung-Won, Bae) Override for Registering and Denying the previous Operation
//		Caution! You should call the base member function in the end of routine.
protected:
	virtual void RegisterPrevOperation( void);

// (2006/6/2 - Seung-Won, Bae) Version Up for Each version and Call Next Version.
protected:
	virtual BOOL	GetNewGraphForDeletedOld( const char *p_szOldGraphName, CString &p_strNewGraphName, BOOL p_bPrevResult);
	virtual	BOOL	VersionUpOldInfo( const char *szGraphName, CIndicatorInfo *pSourceInfo, CKTBChartCtrl* pChartCtrl, CMainBlock* pMainBlock, bool &p_bSubGraphChanged);
protected:
	void			SetIndicatorPacketNameWithGraphPacketName( CMainBlock* pMainBlock, const char *p_szGraphName, CIndicatorInfo *pSourceInfo);
};

#endif // !defined(AFX_CFGFILEVERSIONCHECKER_H__FF40B197_321E_4BA6_A35E_AF73A9E825D5__INCLUDED_)
