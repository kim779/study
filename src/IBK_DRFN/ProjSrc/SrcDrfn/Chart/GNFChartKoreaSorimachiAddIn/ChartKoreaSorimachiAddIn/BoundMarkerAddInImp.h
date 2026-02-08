// BoundMarkerAddInImp.h: interface for the CBoundMarkerAddInImp class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_BOUNDMARKERADDINIMP_H__94AF516C_0B05_41DA_B10F_36D86C28A709__INCLUDED_)
#define AFX_BOUNDMARKERADDINIMP_H__94AF516C_0B05_41DA_B10F_36D86C28A709__INCLUDED_

// (2004.09.14, 배승원) 당일의 상한가/하한가를 관리, 표시하는 Library Routine Class이다.
// (2004.10.22, 배승원, 분석) 이 기능들은 동양 지존차트의 종목/업종의 가격/비율형 봉차트를 지원하면서,
//			상/하한가를 표시하는 기능을 처리하는 것이다.
//		특히 이 기능은 차트내 1개의 Graph에만 적용할 수 있는 기능으로
//			(기능의 Data 구조가 AddIn 당 1개로 국한된 기술적인 문제이다.)
//			첫번째 가격차트에만 제공되는 기능으로 국한한다.

//-----------------------------------------------------------------------------
// Author		: Seung-Won, Bae	Date :2005/3/10
// Comments		: 
// Using way	: 
// See			: 
//
// Help Message Reference : BOUNDMARK
//	{{{
//		BOUNDMARK : 가격차트의 영역의 우측 Scale에 해당 종목의 상하한선을 표시하도록 알려주는 보조 메시지
//			[Type I] 상하한 Bar 표시
//				BOUNDMARK=GN:UB,LB
//					GN = 가격차트의 Graph Name (Map에 설정된 내용에 따라 변경됨)
//					UB = 해당 종목의 상한가
//					LB = 해당 종목의 하한가
//			[Type II] CAskingPriceIndicator Class Header 참고.
//	}}}
//-----------------------------------------------------------------------------

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "../Include_AddIn_381931/I381931/_ChartAddInBase.h"		// for CChartAddInBase

interface IChartOCX;
interface IChartManager;
class CBoundMarkerAddInImp : public CChartAddInBase
{
public:
	CBoundMarkerAddInImp( IChartOCX *p_pIChartOCX, IAddInDllBase *p_pIAddInDllBase);
	virtual ~CBoundMarkerAddInImp();

	// Declare AddIn Event Map
	DECLARE_ADDIN_EVENT_MAP()

// (2006/2/15 - Seung-Won, Bae) Multiple Item in DLL
public:
	static CString		m_strAddInItemName;
	CMapStringToOb		*m_pmapOrder;
	CString				m_strItemCode;
	BOOL				m_bDraw;
// 0. Bound 값을 관리한다. (Graph Name이 없으면 Bound가 설정되지 않은 상태이다.)
protected:
	double	m_dUpperBound;
	double	m_dLowerBound;
	double	m_dYesterdayClosePrice;//sy 2005.11.18. 전일 종가
	CString	m_strIndicatorName;

// (2004.09.14, 배승원) Bound값을 설정하는 Interface를 제공한다.
protected:
	virtual void	ResetBound( const char *p_szBound);

// (2004.09.30, 배승원) BoundMark를 이용할지 여부를 설정받아 관리/적용한다.
protected:
	bool	m_bExistenceBoundMark; //sy 2005.11.21. m_bEnableMark 가 셋팅되어 있는지를 체크한다.
	BOOL	m_bEnableMark;
protected:
	void	EnableBoundMark( BOOL p_bEnable);
	BOOL	GetBoundMarkFlag( void)				{	return m_bEnableMark;	}
	//sy 2005.11.21.
	bool	IsExistenceBoundMark( void)			{	return m_bExistenceBoundMark;	}

//sy 2005.11.18. 전일종가 (상하한바의 기준)
private:
	void SetYesterdayClosePrice(const CString& strData);
	double GetYesterdayClosePrice() const;
	double GetSteadyPrice() const;

// (2004.10.27, 배승원) View의 환경을 관리하여, CAskingPriceIndicator에서 이용할 수 있도록 한다.
protected:
	BOOL		m_bLog;						// 대상 Graph가 등록된 Block의 Log 차트 설정 여부이다.
	BOOL		m_bReverse;					// 대상 Graph가 등록된 Block의 뒤집힘 설정 여부이다.
	CRect		m_rctBoundMark;				// 상한/하한 표시 영역이다.
	CRect		m_rctGraphRegion;			// View의 전체 Graph 영역이다. (Block의 View 영역이다.)
	CRect		m_rctGraphDrawingRegion;	// View의 Graph Drawing영역이다. (실제 Graph의 좌표 영역이다.)
	double		m_dViewMin;					// View내에 대상 Graph된 View의 Minimun Scale 값이다.
	double		m_dViewMax;					// View내에 대상 Graph된 View의 Maximum Scale 값이다.
	CString		m_strExtMsg;				// 보조 Message의 잔여 부분이다.
											//		(기본으로 '그래프명:상한,하한'으로 구성되며 뒤에 ':종목코드'가 추가될 수 있다.)

// (2004.10.11, 배승원) 기본적인 Interface를 Default로 제공한다. (Derived Class에서 재정의하지 않아도 오류가 나지 않도록)
protected:
	// [04/10/11] Draw Event를 AddIn DLL에 알려 처리될 수 있도록 한다.
	virtual void	OnDrawBeforeGraph( HDC p_hDC);
	// RQ별 Packet : 복수종목 - ojtaso (20070111)
	virtual void	OnPacketDataMultiItem(LPCTSTR lpszRQ, const char *p_szPacketNames, BOOL p_bWithReset);
	// (2006/11/12 - Seung-Won, Bae) Support AddIn ENV Value Map
	virtual void	OnLoadEnvironment( IPropertyMap *p_pIAddInEnvMap);
	virtual void	OnSaveEnvironment( IPropertyMap *p_pIAddInEnvMap);
};

#endif // !defined(AFX_BOUNDMARKERADDINIMP_H__94AF516C_0B05_41DA_B10F_36D86C28A709__INCLUDED_)
