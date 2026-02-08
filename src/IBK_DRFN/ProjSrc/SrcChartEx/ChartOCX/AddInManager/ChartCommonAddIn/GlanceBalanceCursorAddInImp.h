// GlanceBalanceCursorAddInImp.h: interface for the CGlanceBalanceCursorAddInImp class.
//
// The Add-In Implementation Class of the function which indicates Span at the mouse cursor
// when we use a "Glance-Balance Indicator".
// 
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GLANCEBALANCECURSORADDINIMP_H__CB9DBB09_6E08_4091_8AA7_169372144A63__INCLUDED_)
#define AFX_GLANCEBALANCECURSORADDINIMP_H__CB9DBB09_6E08_4091_8AA7_169372144A63__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#include "../Include_AddIn/I000000/_ChartAddInBase.h"			// for CChartAddInBase
#include "../Include_AddIn/I000000/_IDoubleList.h"				// for IDoubleList



// Declare Interface ----------------------------------------------------------
interface IChartOCX;


//-----------------------------------------------------------------------------
// Author		: Sang-Woo, Cho		Date : 2006 / 09 / 04
// Comments		: "일목균형표"를 사용할 때 Mouse Cursor에 Span Data를 표시해주는
//				: 기능의 Add-In이다.
// Using way	: 다른 AddIn과 같이 InitAddInDll()을 통해 Load된다.
// See			: CAskingPriceIndicatorAddInImp, CBoundMarkerAddInImp, CTimeMarkerAddInImp를
//				: 참고하여 만듦.
//-----------------------------------------------------------------------------
class CGlanceBalanceCursorAddInImp : public CChartAddInBase  
{
// Constructor & Destructor
public:
	CGlanceBalanceCursorAddInImp( IChartOCX *p_pIChartOCX, IAddInDllBase *p_pIAddInDllBase );
	virtual ~CGlanceBalanceCursorAddInImp();

	// Declare AddIn Event Map
	DECLARE_ADDIN_EVENT_MAP()


// Attributes
public:
	static CString m_strAddInItemName;		// Multiple Item in DLL
	
	CString m_strIndicatorName;				// Indicator Name 


// Attributes
protected:
	// a Flag about using "Span Cursor"
	BOOL m_bGlanceBalanceState;		// "일목균형표"일때 "Span" Data를 보여줄것인지
											// 안보여 줄 것인지를 결정한다.
	BOOL	m_bEnableGlanceBalanceCursor;	// (2006/10/17 - Seung-Won, Bae) Manage flag of Enable State.
	
	// a calculate condition of "Glance-Balance" Indicator
	int m_nPrevSpan;						// "선행 Span" 값
	int m_nAfterSpan;						// "후행 Span" 값
	
	int m_nStartIndex;						// Block에서 첫 Candle의 Index
	int m_nEndIndex;						// Block에서 마지막 Candel의 Index
	int m_nPrevSpanIndex;					// "선행 Span"의 Index
	int m_nAfterSpanIndex;					// "후행 Span"의 Index

	// a Region about Graph Block & Span Data rect
	CRect m_rctBlockRegion;					// View의 전체 Graph 영역이다. (Block의 View 영역이다.)
	CRect m_rctGraphDrawingRegion;			// View의 Graph Drawing영역이다. (실제 Graph의 좌표 영역이다.)

	CPoint m_ptPrevSpan;					// "선행 Span"을 표시할 영역의 중심 Point
	CPoint m_ptAfterSpan;					// "후행 Span"을 표시할 영역의 중심 Point

	CString m_strPrevSpan;					// "선행 Span"을 문자열로 만들어 놓은 변수
	CString m_strAfterSpan;					// "후행 Span"을 문자열로 만들어 놓은 변수

	// a Compartment about X-Scale data
	CString m_strDateCompartment;			// XScale의 자료일자 구분자
	CString m_strTimeCompartment;			// XScale의 시간 구분자

	CPoint	m_ptCursor;
	
// Methods
protected:
	BOOL GetGraphBlockRegion( void );

	BOOL GetSpanData( void );
	BOOL GetSpanTime( CPoint ptPoint );
	
	BOOL GetStringfromDataIdx( int nDataIdx, CString &strSpan );

	// (2006/9/27 - Seung-Won, Bae) On Graph Deleted.
	virtual void	OnGraphAdded( const char *p_szGraphName, const char *p_szIndicatorName);
	virtual void	OnGraphDeleted( const char *p_szGraphName, const char *p_szIndicatorName);

	// Draw Event를 AddIn DLL에 알려 처리될 수 있도록 한다.
	virtual void OnDrawAfterGraph( HDC p_hDC);		

	// (2006/11/12 - Seung-Won, Bae) Support AddIn ENV Value Map
	virtual void	OnLoadEnvironment( IPropertyMap *p_pIAddInEnvMap);
	virtual void	OnSaveEnvironment( IPropertyMap *p_pIAddInEnvMap);
	
	// (2007/1/7 - Seung-Won, Bae) Support AddIn Cursor Operation.
	virtual RECT	DrawCursor( HDC p_hDC);

// Tool Interfaces
// Caution! If the Event is processed, it returns TRUE.
//	and You have not to pass the Event to a next Event Routine.
protected:
	virtual BOOL	OnAddInToolCommand( const EAICommandType p_eCommandID);
	virtual BOOL	OnAddInToolCommand( long &p_lResult, const EAICommandType p_eCommandID);
protected:
	// Mouse cursor의 이동을 처리한다.
	virtual BOOL	OnMouseMove( HDC p_hDC, UINT nFlags, POINT &point, const DWORD p_dwMouseActionID, const HBITMAP p_hbmpMainOSB);	// (2006/11/5 - Seung-Won, Bae) Support MainOSB
	// 제공되지 않은 메뉴 제거 - ojtaso (20071025)
	virtual BOOL	OnCmdMsg(			UINT nID, int nCode, ICmdUI *p_pICmdUI);
};

#endif // !defined(AFX_GLANCEBALANCECURSORADDINIMP_H__CB9DBB09_6E08_4091_8AA7_169372144A63__INCLUDED_)

