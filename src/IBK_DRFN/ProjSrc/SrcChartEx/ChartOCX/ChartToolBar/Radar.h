#if !defined(AFX_RADAR_H__FCAD36A5_D98B_11D6_8777_00E07D8FB0D6__INCLUDED_)
#define AFX_RADAR_H__FCAD36A5_D98B_11D6_8777_00E07D8FB0D6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Radar.h : header file
// 

#include "../Include_Chart/OutsideCommonInterfaceFunction.h"		// for COutsideCommonInterfaceFunction

/////////////////////////////////////////////////////////////////////////////
// CRadar window
class CPacketList;
class CPacketListManager;
class CXScaleManager;
interface IChartCtrl;
// (2003.12.04, 배승원, 수정) LButtonDown시나 Dragging에 의한 Client 벗어사는 경우마다,
//		Drawing이 다시 발생하는 것을 제한하기 위하여 CButton이 아니라 CWnd에서 계승받는다.
class CRadar : public CWnd
{
// Construction
public:
	CRadar( IChartCtrl *p_pIChartCtrl);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRadar)
	//}}AFX_VIRTUAL

// Implementation

	// Generated message map functions
protected:
	//{{AFX_MSG(CRadar)
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnPaint();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()

// (2003.12.02, 배승원) Index 값을 출력할 Radar Status Static을 관리한다.
protected:
	CStatic *	m_pstRadarStatus;
public:
	void		SetRadarStatus( CStatic *p_stRadarStatus)	{	m_pstRadarStatus = p_stRadarStatus;	}

// (2003.12.03, 배승원) 출력할 Packet Data의 정보를 설정받아 관리한다.
// (2003.12.04, 배승원) 구간 선택을 통지할 내부 ToolBar Dialog의 Pointer도 함께 관리한다.
protected:
	CString				m_strDataName;
	CPacketList*		m_pPacketList;
	CPacketListManager* m_pPacketListManager;
	CXScaleManager*		m_pxScaleManager;
	
public:
	void		SetData(CPacketList* pPacketList, CPacketListManager* pPacketListManager, CXScaleManager* pxScaleManager, CString& strDataName);	// 종찬(04/10/25) 툴바DLL. DLL제작으로 인해 수정함.

// (2003.12.03) Zoom Full Mode Flag을 관리하여, 해당 Mode에서는 Mouse Tracing외의 작업을 제한한다.
// (2004.02.17) Zoom Full Mode는 전체보기 설정이 아니라 대기매물이 아닌 단일 Chart의 경우,
//	Zoom & Scroll을 제한하기 위한 Mode로 이용한다.
protected:
	BOOL	m_bZoomFullMode;

// (2003.12.03, 배승원) Zoom & Scroll 관련 기능을 정리한다.
//		(*.주의 :	Rectagle과 Region을 구분하여 이용한다.
//					Rectangle은 Right 좌표와 Bottom 좌표 영역이 계산에 포함되지 않으며,
//					Region은 모두 포함시킨다.)
protected:
	// 1. Draw 속도를 빨리 처리하도록 Radar Chart의 Client 영역을 구하여 놓는다.
	CRect	m_rctRadarClient;
	// 2. Draw 속도를 빨리 처리하기 위하여 Radar Chart의 선택된 영역을 Rect로 구하여 놓는다.
	CRect	m_rctRadarSelect;
	// 3. Draw 속도를 빨리 처리하기 위하여 Graph Data의 Min/Max 및 Index 구간도 관리한다.
	int		m_nRadarDataLeft;
	int		m_nRadarDataRight;
	double	m_dRadarDataMin;
	double	m_dRadarDataMax;
	// 4. Draw 속도를 빨리 처리하기 위하여 Graph가 그려지는 영역의 좌표를 미리 구하여 놓는다.
	CRect	m_rgnRadarGraph;
	// 5. Draw 속도를 빨리 처리하기 위하여 Graph의 Scale 변환 수치를 미리 구하여 놓는다.
	double	m_dXScale, m_dYScale;
	// 6. Data View 정보를 관리한다.
	int		m_nDataStartIndex;
	int		m_nOnePageDataCount;
public:
	// 1. Data View 정보를 전달받아 Radar Chart에 반영하는 Interface를 제공한다.
	void	SetDataViewInfo( int nDataSize, int nDataStartIndex, int nOnePageDataCount, BOOL p_bZoomFullMode);

// (2003.12.04, 배승원) 구간 선택 기능을 추가한다.
protected:
	// 1. Dragging 중인 영역의 Region Range를 관리한다.
	long	m_nDragSelectStart;
	long	m_nDragSelectEnd;
protected:
	// 1. Dragging 영역을 출력하는 Routine을 별도로 제공하한다.
	void	DrawDragSelectRectangle( CDC *pDC, long p_nDragSelectStart, long p_nDragSelectEnd, long *p_pPreDragSelectEnd = NULL);
	// 2. Mouse의 Point에 해당하는 Data Index를, Mouse Point를 보정하며 산출하는 Routine을 별도 제공한다.
	//		(조정을 요청하지 않은 경우의 오류는 -1이 Return된다.)
	//		주의:1. Index 조정에 있어서 최좌측의 경우 m_rgnRadarGraph.left + 1로 조정한다.
	//					그것은 Drawing시에 Pen의 굵기가 2인 것과 연관되어 조정된 것이다.
	//			 2. 따라서, 이렇게 조정한 PointX를 가지고 다시 DataIndex를 산출하면 오동작할 수 있으니,
	//					가능하면 1회에 처리한다.
	int		GetDataIndex( long &p_nPointX, BOOL p_bWithCorrection = TRUE) const;
	// 3. 시작과 끝 영역에 해당하는 선택영영에 Rectangle을 출력하는 Routine을 제공한다.
	void	DrawRectangleInRegion( CDC *p_pDC, long p_nDragSelectStart, long p_nDragSelectEnd) const;

// (2004.04.16, 배승원) Radar의 Resize시에도, Resize된 Graph를 보이도록,
//		Radar Graph를 재구성하는 Routine을 별도로 제공한다.
public:
	void	DrawNewRadarView( void);

// (2006/11/18 - Seung-Won, Bae) Notify the User Zoom State to MultiChart
public:
	IChartCtrl *m_pIChartCtrl;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_RADAR_H__FCAD36A5_D98B_11D6_8777_00E07D8FB0D6__INCLUDED_)
