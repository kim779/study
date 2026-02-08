// YScaleSetUpAddInImp.h: interface for the CYScaleSetUpAddInImp class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_YSCALESETUPADDINIMP_H__10CBB4F1_76B7_42A8_8400_DC07241C26DE__INCLUDED_)
#define AFX_YSCALESETUPADDINIMP_H__10CBB4F1_76B7_42A8_8400_DC07241C26DE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#include "../Include_AddIn/I000000/_ChartAddInBase.h"		// for CChartAddInBase

interface IBlock;

//-----------------------------------------------------------------------------
// Author		: Sang-Woo, Cho		Date : 2006 / 11 / 07
// Comments		: YScale 설정 AddIn
// Using way	: 
// See			: 
//-----------------------------------------------------------------------------
class CYScaleSetUpAddInImp : public CChartAddInBase  
{
public:
	enum YSCALE_APPLY_STATE
	{
		APPLY_CANCEL,			// 해지
		APPLY_VALUE,	// 값적용
		APPLY_PERCENT	// 비율적용
	};

public:
	CYScaleSetUpAddInImp(IChartOCX *p_pIChartOCX, IAddInDllBase *p_pIAddInDllBase);
	virtual ~CYScaleSetUpAddInImp();

	// Declare AddIn Event Map
	DECLARE_ADDIN_EVENT_MAP()

// (2006/2/15 - Seung-Won, Bae) Multiple Item in DLL
public:
	static CString		m_strAddInItemName;

public:
	void SetYScaleAllValue( int nYScaleSetUpType, double dMax, double dMin, double dMaxPercent, double dMinPercent );

protected:
	YSCALE_APPLY_STATE	m_eYScaleSetupApplyState;		// YScale 적용 여부

	int m_nVertScaleGroupIndex;
	
	CString m_strGraphName;				// Graph 이름
	CString m_strStockCode;				// 종목코드
	CString m_strPrevStockCode;			// 이전 종목코드
	CString	m_strExtMsg;				// 보조 Message의 잔여 부분이다.
										//		(기본으로 '그래프명:상한,하한'으로 구성되며 뒤에 ':종목코드'가 추가될 수 있다.)
	
	CString m_strFormat;				// 호가 정밀도.

	double	m_dYesterdayClosePrice;		// 전일 종가
	double	m_dUpperBound;				// 상한가
	double	m_dLowerBound;				// 하한가
	
	int m_nYScaleMaxMinSetupType;		// 설정값 / 비율 ( 0 : 설정값, 1 : 비율 )

	// 설정값 - 최대 / 최소값
	double m_dYScaleMaxValue;
	double m_dYScaleMinValue;

	// 비율 - 최대 / 최소값
	double m_dYScaleMaxPercentValue;
	double m_dYScaleMinPercentValue;


protected:
	void ResetYScaleSetUp( const char *p_szBound );
	
	void SetYesterdayClosePrice(const CString& strData);
	
	BOOL SetYScaleInfo( IBlock *pIBlock );

// [05/01/19] AddIn Object가 비정형 Interface를 제공할 수 있도록 제공한다.
protected:
	virtual BOOL	InvokeAddInStr( LPCTSTR p_szCommandName, LPCTSTR p_szData); // (2005/9/2 - Seung-Won, Bae) for String Data

//////////////////////////////////////////////////////////////////////
// Event Routines
//////////////////////////////////////////////////////////////////////
protected:
	// RQ별 Packet : 복수종목 - ojtaso (20070111)
	virtual void	OnPacketDataMultiItem(LPCTSTR lpszRQ, const char *p_szPacketNames, BOOL p_bWithReset);
	// (2006/11/24 - Seung-Won, Bae) On Tool Command
	virtual void	OnToolCommand( const int p_nToolType, const int p_nCmdOption);
	// Support AddIn ENV Value Map
	virtual void	OnLoadEnvironment( IPropertyMap *p_pIAddInEnvMap);
	virtual void	OnSaveEnvironment( IPropertyMap *p_pIAddInEnvMap);
};

#endif // !defined(AFX_YSCALESETUPADDINIMP_H__10CBB4F1_76B7_42A8_8400_DC07241C26DE__INCLUDED_)
