#ifndef __CHARTLINK_H__
#define __CHARTLINK_H__

// ChartLink.h : header file
//
/////////////////////////////////////////////////////////////////////////////
// CChartLink window

#include "../Include_Chart/BlockBaseEnum.h"			// for CScaleBaseData

//Chart의 Tree구조를 저장하기 위한 클래스.
class CMyTreeCtrl;
class CChartLink
{
// Construction
public:
	CChartLink(CChartLink *Parent, int Type, LPSTR Name, HWND p_hOcxWnd);

// Attributes
public:
	HTREEITEM	m_hItem;
	CString		m_strName;
	int			m_iType;

	CString     m_strIndicatorName;
	CString		m_strGraphName;			// (2006/11/29 - Seung-Won, Bae) Support Graph Name for Binding with Indicator Info.
	BOOL        m_bScaleJoin;
	int         m_nScalePosition;
	CString     m_strCheckedPacketItems;
	CString     m_strSubGraphDatas;
	//sy 2006.04.17.
	bool        m_bSignalUse;
	bool		m_bSignalShow;
	//sy end
	//nami 8.8
	bool	m_bLowHighUpDown;
	bool    m_bLowNowUpDown;
	bool    m_bHighNowUpDown;
	bool	m_bPreNowContrast;
	int		m_nCurData;

	//Block
	CString     m_strDateScaleCompart;
	CString     m_strTimeScaleCompart;
	int         m_nHorizonpos;
	int         m_nHorizonStyle;
	int         m_nColumnIndex;
	int         m_nLog;
	int         m_nScaleCompartType;
	//nami 11.21
	int m_nScaleExpressionType;

	//Block, Item.
	CString     m_strScaleStyle;

	CChartLink* m_Parent ;
	CChartLink* m_Child ;
	CChartLink* m_Prev ;
	CChartLink* m_Next ;

	// (2006/11/11 - Seung-Won, Bae) Min/Max Setting
	BOOL							m_bMinMaxLock;
	CScaleBaseData::VERTMINMAXTYPE	m_eMinMaxType;
	double							m_dMax;
	double							m_dMin;
	// (2007/7/19 - Seung-Won, Bae) Support Up/Down Space
	int								m_nUpSpace;
	int								m_nDownSpace;
	//2007.01.22 Scale사용자 설정,호가단위별
	double							m_dHogaNumber;
	double							m_dUserNumber;
	int								m_nUserHogaType;

	// 지표 뒤집기 - ojtaso (20071023)
	BOOL							m_bIsInvert;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CChartLink)
	//}}AFX_VIRTUAL

// Implementation
public:
	void InsertAfterTarget(CChartLink* pItem);
	
	void Cut();
	void Delete();
	BOOL InsertChild(CChartLink* pItem);
	BOOL Insert(CChartLink* pItem);
	BOOL AddChild(CChartLink* pItem);
	BOOL Add(CChartLink* pItem);
	BOOL DeleteAll();
	CChartLink* Find(HTREEITEM hItem);
	virtual ~CChartLink();

private:
	void AutoScaleData(const CString& strName);

// (2008/1/15 - Seung-Won, Bae) for MultiLanguage.
protected:
	HWND	m_hOcxWnd;
public:
	void	SetOcxHwnd( HWND p_hOcxWnd);
};

/////////////////////////////////////////////////////////////////////////////
#endif 
