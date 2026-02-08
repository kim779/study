// IAddInManager.h: interface for the IAddInManager class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_IADDINMANAGER_H__45A3C29A_7BFE_48D1_8565_984832A3CBC3__INCLUDED_)
#define AFX_IADDINMANAGER_H__45A3C29A_7BFE_48D1_8565_984832A3CBC3__INCLUDED_

// (2004.10.07, 배승원) Addin DLL에게 전달되어, Chart Ocx Interface Gate Way 기능을 제공한다.

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//-----------------------------------------------------------------------------
// Author		: Seung-Won, Bae	Date :2006/2/27
// Comments		: AddIn Number Format
// Using way	: 
// See			: 
//-----------------------------------------------------------------------------
//	1. Basic Format		:	SSAAAA	( SS: Site Code,	AAAA: AddIn Code)
//	2. Default AddIn	:	000000	( Chart Inner - Common AddIn)
//	3. Code Generattion	:	A  B  C  D  E  F  G  H  I  J  K  L  M  N  O  P  Q  R  S  T  U  V  W  X  Y  Z
//														  1  1  1  1  1  1  1  1  1  2  2  2  2  2  2  2
//							1  2  3  4  5  6  7  8  9  0  1  2  3  4  5  6  7  8  9  0  1  2  3  4  5  6
//							*							  *								*
//	4. Site Code	:
//						00	~ CO (Common)
//						83	~ HW (Hanwha)
//						42	~ DB (DongBu)
//						91	~ SK (SK)
//						13	~ KC (Koscom)
//						99	~ SS (Samsung)
//
//	5. AddIn Code	:
//						9091	~ STIA ( System Trading Indicator AddIn)
//						3611	~ CPAA ( Candle Pattern Analysis AddIn)
//						3101	~ CATA ( Chart Analysis Tool AddIn)
//						4221	~ DLVA ( Data List Viewer AddIn)
//						4661	~ DPPA ( Design Property Page AddIn)
//						3951	~ CSOA ( Chart Stock Opinion AddIn)			SK, '[3007] 증권사 투자의견' 화면용, 투자의견 Data 처리 AddIn
//						5914	~ ESUD ( Chart Environment Setup Dialog AddIn)
//						3691	~ CPSA ( Chart Period Summary AddIn)
//						3521	~ COLA ( Chart Order Line AddIn)
//						3721	~ CGLA ( Chart Guide Line AddIn)

interface IChartOCX000000;	// Chart Common AddIn
interface IChartOCX839091;	// HW - STIA	(Not Used)
interface IChartOCX429091;	// DB - STIA	Debug Version Only
interface IChartOCX919091;	// SK - STIA	Debug Version Only
interface IChartOCX423611;	// DB - CPAA
interface IChartOCX139091;	// KC - STIA
interface IChartOCX913101;	// SK - CATA	(Not Used Current Version)
interface IChartOCX423101;	// DB - CATA	(Not Used Current Version)
interface IChartOCX133101;	// KC - CATA
interface IChartOCX914221;	// SK - DLVA	(Not Used Current Version)
interface IChartOCX424221;	// DB - DLVA	(Not Used Current Version)
interface IChartOCX134221;	// KC - DLVA
interface IChartOCX914661;	// SK - DPPA	(Not Used Current Version)
interface IChartOCX424661;	// DB - DPPA	(Not Used Current Version)
interface IChartOCX134661;	// KC - DPPA
interface IChartOCX913951;	// SK - CSOA	(Not Used Current Version)
interface IChartOCX135914;	// KC - ESUD
interface IChartOCX425914;	// DB - ESUD	(Not Used Current Version)
interface IChartOCX915914;	// SK - ESUD	(Not Used Current Version)
interface IChartOCX133691;	// KC - CPSA
interface IChartOCX133521;	// KC - COLA
interface IChartOCX993101;	// SS - CATA
interface IChartOCX994221;	// SS - DLVA
interface IChartOCX994661;	// SS - DPPA
interface IChartOCX995914;	// SS - ESUD
interface IChartOCX999091;	// SS - STIA
interface IChartOCX993691;	// SS - CPSA
interface IChartOCX993521;	// SS - COLA
interface IChartOCX003721;	// CO - CGLA
interface IChartOCX999092;	// SS - STIA2

interface IAddInManager : public IUnknown  
{
protected:
	virtual ~IAddInManager()	{}

// (2004.10.07, 배승원) Chart OCX의 Interface를 구하는 Interface를 제공한다.
public:
//////////////////////////////////////////////////////////////////////
	virtual IChartOCX000000 *	GetIChartOCX000000( void)	= 0;	// Chart Common AddIn
	virtual IChartOCX839091 *	GetIChartOCX839091( void)	= 0;	// HW - STIA (Not Used)
	virtual IChartOCX429091 *	GetIChartOCX429091( void)	= 0;	// DB - STIA
	virtual IChartOCX919091 *	GetIChartOCX919091( void)	= 0;	// SK - STIA
	virtual IChartOCX423611 *	GetIChartOCX423611( void)	= 0;	// DB - CPAA
	virtual IChartOCX139091 *	GetIChartOCX139091( void)	= 0;	// KC - STIA
	virtual IChartOCX913101 *	GetIChartOCX913101( void)	= 0;	// SK - CATA
	virtual IChartOCX423101 *	GetIChartOCX423101( void)	= 0;	// DB - CATA
	virtual IChartOCX133101 *	GetIChartOCX133101( void)	= 0;	// KC - CATA
	virtual IChartOCX914221 *	GetIChartOCX914221( void)	= 0;	// SK - DLVA
	virtual IChartOCX424221 *	GetIChartOCX424221( void)	= 0;	// DB - DLVA
	virtual IChartOCX134221 *	GetIChartOCX134221( void)	= 0;	// KC - DLVA
	virtual IChartOCX914661 *	GetIChartOCX914661( void)	= 0;	// SK - DPPA
	virtual IChartOCX424661 *	GetIChartOCX424661( void)	= 0;	// DB - DPPA
	virtual IChartOCX134661 *	GetIChartOCX134661( void)	= 0;	// KC - DPPA
	virtual IChartOCX913951 *	GetIChartOCX913951( void)	= 0;	// SK - CSOA
	virtual IChartOCX135914 *	GetIChartOCX135914( void)	= 0;	// KC - ESUD
	virtual IChartOCX425914 *	GetIChartOCX425914( void)	= 0;	// DB - ESUD
	virtual IChartOCX915914 *	GetIChartOCX915914( void)	= 0;	// SK - ESUD
	virtual IChartOCX133691 *	GetIChartOCX133691( void)	= 0;	// KC - CPSA
	virtual IChartOCX133521 *	GetIChartOCX133521( void)	= 0;	// KC - COLA
	virtual IChartOCX993101 *	GetIChartOCX993101( void)	= 0;	// SS - CATA
	virtual IChartOCX994221 *	GetIChartOCX994221( void)	= 0;	// SS - DLVA
	virtual IChartOCX994661 *	GetIChartOCX994661( void)	= 0;	// SS - DPPA
	virtual IChartOCX995914 *	GetIChartOCX995914( void)	= 0;	// SS - ESUD
	virtual IChartOCX999091 *	GetIChartOCX999091( void)	= 0;	// SS - STIA
	virtual IChartOCX993691 *	GetIChartOCX993691( void)	= 0;	// SS - CPSA
	virtual IChartOCX993521 *	GetIChartOCX993521( void)	= 0;	// SS - COLA
	virtual IChartOCX003721 *	GetIChartOCX003721( void)	= 0;	// CO - CGLA
	virtual IChartOCX999092 *	GetIChartOCX999092( void)	= 0;	// SS - STIA
//////////////////////////////////////////////////////////////////////
// (2006/1/5 - Seung-Won, Bae) Caution. Add to bottom!
//////////////////////////////////////////////////////////////////////
};

#endif // !defined(AFX_IADDINMANAGER_H__45A3C29A_7BFE_48D1_8565_984832A3CBC3__INCLUDED_)
