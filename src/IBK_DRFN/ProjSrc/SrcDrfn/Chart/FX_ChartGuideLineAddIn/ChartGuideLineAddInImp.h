// ChartStockOpinionAddInImp.h: interface for the CChartGuideLineAddInImp class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CHARTSTOCKOPINIONADDINIMP_H__DA9542DB_C2AE_4A00_8CA5_CE7334AB74E4__INCLUDED_)
#define AFX_CHARTSTOCKOPINIONADDINIMP_H__DA9542DB_C2AE_4A00_8CA5_CE7334AB74E4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "./Include_AddIn.h"							// for Ixxxxxx.h

//추가
#include "PatternMaker.h"									// for CPatternMaker
#include "DlgSetGuideLine.h"								// 기준선 설정 작도 Dlg
#include "./include_Addin/I003721/_AddInCommand.h"		// for DECLARE_ADDIN_EVENT_MAP()

#define R_ERROR		-1
#define R_OK		0
#define R_OK_SCROLL	1

// RQ별 관리 : 복수종목 - ojtaso (20070305)
using namespace std;

#include <map>
#include <string>

// RQ별 관리 : 복수종목 - ojtaso (20070305)
//typedef map<string, CPatternMaker*>		MAP_PATTERNMAKER;

class CChartGuideLineAddInImp : public CChartAddInBase
{
public:
	DWORD m_dwMouseActionID;
	virtual BOOL InvokeAddIn( int p_nCommandType, long p_lData);
	static CString m_strAddInItemName;
	
	CChartGuideLineAddInImp( IChartOCX *p_pIChartOCX, IAddInDllBase *p_pIAddInDllBase);
	virtual ~CChartGuideLineAddInImp();

	// Declare AddIn Event Map
	DECLARE_ADDIN_EVENT_MAP()

//	//2005. 03. 02 == 삼성 데모용
//	BOOL	ReadAcctInfoFromFile_ForSamgsungDemo(CString &strAcct, CString &strPass);
	CString	CheckAcctNPassFromMain(HWND hMainFrame, CString strAcct, CString strInputPass, int nAcctCheckType=1);	

	CString m_strUsrPath;
	CString m_strCode;		 //종목코드 	
	void	LoadGlobalCfg(); //환경설정 내용을 읽어온다.
// *******************************************************************************
// Pattern Maker
public:
	//CPatternMaker		m_pmPatternMaker;
	LPCTSTR m_lpszRQ;		// 현재 RQ : 복수종목 - ojtaso (20070305)
//@유진삭제	MAP_PATTERNMAKER	m_mapPatternMaker; // RQ별 관리 : 복수종목 - ojtaso (20070305)
	CMapStringToPtr		m_mapPatternMaker; // RQ별 관리 : 복수종목

	CPatternEnvData		m_PatternEnvData;			// 패턴 환경설정 데이터	
// ********************************************************************************

// (2005.12.01, sy, nam) Real 관련 Event를 전달한다. =======================================================================================
//protected:
//	virtual BOOL	OnRealData( WORD p_wRealRecordID, WORD p_wRealItemID, const char *p_szRealKeyCode, long p_lPacketAddress, double dData);
//==========================================================================================================================================
	void InvokeRealData(int nMarketGubun, LPSTR szCode, LPSTR oPrice, LPSTR hPrice, LPSTR lPrice, LPSTR cPrice);
	
protected:
	virtual void OnDrawAfterGraph( HDC p_hDC);
	virtual void OnDrawBeforeGraph( HDC p_hDC);
	virtual BOOL OnLButtonUp(HDC p_hDC, UINT nFlags, POINT &point, const DWORD p_dwMouseActionID);
	virtual BOOL OnLButtonDown(HDC p_hDC, UINT nFlags, POINT &point, const DWORD p_dwMouseActionID);
	virtual BOOL OnRButtonUp(HDC p_hDC, UINT nFlags, POINT &point, const DWORD p_dwMouseActionID);
	virtual BOOL OnRButtonDown(HDC p_hDC, UINT nFlags, POINT &point, const DWORD p_dwMouseActionID);
	virtual BOOL OnMouseMove(HDC p_hDC, UINT nFlags, POINT &point, const DWORD p_dwMouseActionID);
	//virtual BOOL OnLButtonDblClk(HDC p_hDC, UINT nFlags, POINT &point, const DWORD p_dwMouseActionID);
 	virtual void	OnPacketData( const char *p_szPacketNames, BOOL p_bWithReset);
	virtual void	OnResetChart();
	// RQ별 Packet : 복수종목 - ojtaso (20070111)
	virtual void	OnPacketDataMultiItem(LPCTSTR lpszRQ, const char *p_szPacketNames, BOOL p_bWithReset);
	// 복수종목별 Real반영 : 복수종목 - ojtaso (20070131)
	virtual void	OnUpdatePacketDataMultiItem( LPCTSTR lpszRQ, BOOL p_bTrUpdate);
	// 복수종목 지원을 위해 추가 : 복수종목 - ojtaso (20070125)
	//virtual BOOL	OnAddInToolCommand(	long &p_lResult, const EAICommandType p_eCommandID, const char *p_szParam1, const char *p_szParam2, const char *p_szParam3, CObject *p_pParam);
	//virtual BOOL	OnAddInToolCommand(	const EAICommandType p_eCommandID, const long p_lParam, const char *p_szParam1, const char *p_szParam2, const char *p_szParam3);
	// 복수종목 추가/삭제 : 복수종목 - ojtaso (20070302)
	virtual BOOL	OnAddInToolCommand(	const EAICommandType p_eCommandID, const char *p_szParam);

private:
	BOOL		m_bLog;						// 대상 Graph가 등록된 Block의 Log 차트 설정 여부이다.
	BOOL		m_bReverse;					// 대상 Graph가 등록된 Block의 뒤집힘 설정 여부이다.
	CRect		m_rctBoundMark;
	double		m_dViewMin;					// View내에 대상 Graph된 View의 Minimun Scale 값이다.
	double		m_dViewMax;					// View내에 대상 Graph된 View의 Maximum Scale 값이다.
	CRect	rctBlockRegion, rctGraphRegion ;
	int		nStartIndex, nEndIndex;

	void GetPatternMaker(CPatternMaker*& pPatternMaker, LPCTSTR lpszRQ = NULL);
};

#endif // !defined(AFX_CHARTSTOCKOPINIONADDINIMP_H__DA9542DB_C2AE_4A00_8CA5_CE7334AB74E4__INCLUDED_)
