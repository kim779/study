// RadarControl.h: interface for the CToolRadarControl class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RADARCONTROL_H__BB35196F_CA15_446B_A5D1_40E5BED68D92__INCLUDED_)
#define AFX_RADARCONTROL_H__BB35196F_CA15_446B_A5D1_40E5BED68D92__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CRadar;
class CToolBarDlg;
interface IChartCtrl;
class CRadarControl  
{
public:
	CRadarControl();
	virtual ~CRadarControl();

// (2003.12.02, 배승원) Radar Chart의 생성/삭제를 처리한다.
protected:
	BOOL	m_bOnRadar;

// (2003.12.02, 배승원) Flat Button으로 구성된 Radar Chart와 Static으로 구성된 Index Status를 생성, 관리한다.
protected:
	CRadar *					m_radar;
	CStatic *					m_static;
//public:
//	void				CreateRadarButton( CFont* pFont, CMiniToolBarDlg* MiniToolBarDlg, CRect controlItemRect,
//							CObjTool* objTool, CString filePath);

// (2003.12.02, 배승원) Radar Status Static에 출력할 Font를 관리한다.
protected:
	CFont *	m_pFont;

// (2003.12.02, 배승원) Zoom & Scroll 관련 기능을 정리한다.
protected:
	// 1. Event를 전달하기 위하여 내부 ToolBar Object Pointer를 관리한다.
//	CMiniToolBarDlg *	m_pMiniToolBarDlg;
	// 2. Radar Chart에게 전달할 Data View 정보를 관리한다.
	//		(처음 생성시와 Zoom Full Mode가 해제되는 시점에서는 SetDataViewInfo가 호출되지 않아 수동 호출한다.)
	int		m_nDataSize;
	int		m_nDataStartIndex;
	int		m_nOnePageDataCount;
public:
	// 1. Zoom Full Mode 여부와 조정된 Data View 정보를 함께 전달받아 처리한다.
	//		(Radar의 경우 Zoom Full Mode여도 Data를 지속적으로 반영한다.)
	void	SetDataViewInfo( int nDataSize, int nDataStartIndex, int nOnePageDataCount, BOOL p_bZoomFullMode);

// (2004.03.11, 배승원) ToolBar의 크기가 변경되는 경우, Radar의 배치도 조정토록한다.
public:
	void	OnToolBarPositionChanged( CRect p_rctToolRect);

// 종찬(04/11/04) Create함수 추가한다. CCtrlFactory에서 이 함수를 호출하여 RadarChartControl을 생성한다.
public:
	bool	Create(CWnd* pParent, UINT nID);

// (2006/11/18 - Seung-Won, Bae) Notify the User Zoom State to MultiChart
protected:
	IChartCtrl	*m_pIChartCtrl;
public:
	void	SetIChartCtrl( IChartCtrl *p_pIChartCtrl)	{	m_pIChartCtrl = p_pIChartCtrl;	}
};

#endif // !defined(AFX_RADARCONTROL_H__BB35196F_CA15_446B_A5D1_40E5BED68D92__INCLUDED_)
