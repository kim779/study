// ChartOrderLineAddInImp.h: interface for the CChartOrderLineAddInImp class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CHARTORDERLINEADDINIMP_H__DA9542DB_C2AE_4A00_8CA5_CE7334AB74E4__INCLUDED_)
#define AFX_CHARTORDERLINEADDINIMP_H__DA9542DB_C2AE_4A00_8CA5_CE7334AB74E4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "./Include_AddIn/Include_AddIn.h"							// for Ixxxxxx.h
#include "./Include_Chart/IPropertyMap.h"

#define R_ERROR		-1
#define R_OK		0
#define R_OK_SCROLL	1

#define OL_FILE	"OrderLine.cfg"	// 주문선 저장 파일명

class CLineObj;
typedef CTypedPtrList< CObList, CLineObj *>  COLList;

class CChartOrderLineAddInImp : public CChartAddInBase
{
// AddIn ID
public:
	static CString m_strAddInItemName;

// Constructor & Destructor
public:
	CChartOrderLineAddInImp( IChartOCX *p_pIChartOCX, IAddInDllBase *p_pIAddInDllBase);
	virtual ~CChartOrderLineAddInImp();

// Current Chart Data Info
protected:
    CString		m_strCode;              // 종목코드, Item Code
	CString		m_strCodeName;			// 종목명, Item Name
	CString		m_strDateType;			// 기준봉, Current Date/Time Type String => 일/주/월/분/틱
	int			m_nDateType;			// 날짜 타입, 기준봉
	CString		m_strRQ;				

// Current Chart View Info
protected:
	BOOL		m_bLog;					// 대상 Graph가 등록된 Block의 Log 차트 설정 여부이다.
	BOOL		m_bReverse;				// 대상 Graph가 등록된 Block의 뒤집힘 설정 여부이다.
	double		m_dViewMin;				// View내에 대상 Graph된 View의 Minimun Scale 값이다.
	double		m_dViewMax;				// View내에 대상 Graph된 View의 Maximum Scale 값이다.
	CRect		m_rctBlockRegion;
	CRect		m_rctGraphRegion;
	CRect		m_rctGraphDrwingRegion;
	int			m_nStartIndex;
	int			m_nEndIndex;

// Current Tool State
protected:
    int			m_nCommandType;			// 명령 타입, Tool State Mode
										// 0: Not Initialized.
										// 1: Drawing Mode of new Object
										// 2: Selection Mode (Selected or not)
	DWORD		m_dwMouseActionID;		// Current Mouse Action ID
	CLineObj *	m_pCurSelectedLine;		// 선택된 주문선, Selected Object

// Tool Object Data
protected:
	COLList		m_LineList;						// 주문선 리스트

// (2010/4/17 - Seung-Won, Bae) Make it static to share the Order Line Info within all Chart.
public:
	static CStringList											m_slSavedOrderLines;		// 저장된 모든 주문선
	static CTypedPtrList< CObList, CChartOrderLineAddInImp *>	m_alOrderLineAddInManager;
public:
	COLList *	GetLineList( void)			{	return &m_LineList;		}
	void		InvalidateControl( void)	{	if( m_pIChartOCX) m_pIChartOCX->InvalidateControl();	}

// Chart Data and View Base Info Collecting
protected:
	// Chart Data Info
	//		m_strCode, m_strCodeName		- InvokeAddInStr(), It is called in MChart TR Data Changing.
	//		m_strDateType, m_nDateType		- OnPacketDataMultiItem()
	// Chart View Info
	BOOL	GetChartBasicData( void);
			// OnDrawAfterGraph()

// Current Tool State
protected:
	// Selection Change
	CLineObj *	ChangeSelection( CLineObj *p_pNewSelection);
				// ChangeToolMode()											- Release Selection for new Mode
				// DeleteSelection()										- Release Selection for delete
				// DeleteAll()												- Release Selection for delete all
				// OnAddInToolCommand( EAI_TOOLORDERMGR_DELETE_THIS_TOOL)	- Change Selection for delete
				// OnLButtonDown()											- Select new Creation
				// SelectLine()												- new Selection
				
	// Tool Mode Change
	BOOL		ChangeToolMode( int p_nNewMode);
				// OnAddInToolCommand( EAI_OOA_ON_TOOL_FUNCTION, T_ORDER_LINE,)				- Start/End Drawing Mode
				// OnAddInToolCommand( EAI_OOA_ON_TOOL_FUNCTION, T_TOOL_SELECTION,)			- Release Drawing Mode
				// OnAddInToolCommand( EAI_OOA_ON_TOOL_FUNCTION, T_ALL_TOOL_RESET_STATE,)	- Release Drawing Mode
				// OnResetChart()															- Release Drawing Mode
				// OnCmdMsg( ID_KSA_ORDERLINE_RELEASE)										- Release Drawing Mode by context menu

// Draw
protected:
	//	OnDrawAfterGraph()
	//	DrawCursor()

// Create
protected:
	// It is processed with 4 Step.
	//	1. LButton Down						- OnLButtonDown()
	//  2. Mouse Move with LButton Down		- OnMouseMove()
	//	3. LButton Up						- OnLButtonUp()
	//	4. Config the Object.				- in OnLButtonUp()

// Select
protected:
	BOOL	SelectLine( POINT &p_pt, DWORD p_dwMouseActionID);
			// OnLButtonDown()
			// OnRButtonDown()

// Modify
protected:
	// It is processed with 3 Step.
	//	1. LButton Down						- OnLButtonDown()
	//  2. Mouse Move with LButton Down		- OnMouseMove()
	//	3. LButton Up						- OnLButtonUp()

// Delete
protected:
	// Delete Selected Object
	BOOL	DeleteSelection( BOOL p_bUnregisterOrderManager = TRUE);
			// OnAddInToolCommand( EAI_TOOLORDERMGR_DELETE_THIS_TOOL, "ORDER_LINE", ...)	- Delete by order
			// OnLButtonUp()																- Empty Object
			// OnCmdMsg( ID_KSA_ORDERLINE_DELETE, )											- Delete by Menu
			// OnKeyDown( VK_DELETE, )														- Delete by 'Del' Key
			// OnOLConfig()																	- Canceled new Object
			// OnDeleteLine()																- Delete with file by Menu
	// Delete All Object
	BOOL	DeleteAll( void);
			// ~CChartOrderLineAddInImp()
			// OnAddInToolCommand( T_DELETE_ALL)	- Delete all by Tool Command
			// LoadOrderLine()						- Delete for new Loading.

// Save and Load
protected:
	// OnDeleteLine()	- Delete Line and Save File directly for context menu.
	// OnOLConfig()		- Add new Line with save Btn of dialog.
	// OnOLMan()		- Delete Lines of Item and Save Files directly.
	// LoadOrderLine()	- Reload Lines in TR and Reset Chart.

// Register and Unregister to order manager
protected:
	void	RegPS(		long pLine/*IN*/);				// 오브젝트 addin manager에 등록
			// OnLButtonDown()	- new Object
			// LoadOrderLine()	- Loaded Object
	void	UnregPS(	long pLine/*IN*/);				// addin manager에서 오브젝트 등록 해제
			// DeleteSelection()	- Deletion by not order manager.
			// DeleteAll()			- Remove all

protected:
	void LoadOrderLine();			// 저장된 주문선 불러오기
	void OnOLConfig();				// 주문선 설정
	void OnOLMan();					// 주문선 관리
	void OnDeleteLine();			// 주문선 파일 & 메모리(화면) 삭제

	// 2011.01.06 by SYS >> 분차트 YYYY(년) 추가
	//int GetIndexFromValue(long lx/*IN*/, int& ix/*OUT*/);
	int GetIndexFromValue(__int64 lx/*IN*/, int& ix/*OUT*/);
	// 2011.01.06 by SYS <<

	long GetXPosition( int x);
	
	// 2011.01.06 by SYS >> 분차트 YYYY(년) 추가
	//int GetValueFromIndex(int ix/*IN*/, long& lx/*OUT*/);
	int GetValueFromIndex(int ix/*IN*/, __int64& lx/*OUT*/);
	// 2011.01.06 by SYS <<

	int GetIndexFromPoint(CPoint& pt/*IN,OUT*/, int& x/*OUT*/);
	int GetYPositionOnLine(int& x/*IN,OUT*/, CPoint pt1/*IN*/, CPoint pt2/*IN*/);

	BOOL IsValidIndex(int nIndex);	// 주어진 인덱스가 유효한 인덱스인지 여부 판별
	CString TokenizeEx(CString strSrc/*IN*/, const char* pszTokens/*IN*/, int& iStart/*IN, OUT*/);	// 스트링의 추출
	void	OnUpdatePacketDataMultiItem2( IPacket *pIPacketForCur);

	// 20081007 JS.Kim	사용자시간대 선택
	bool	RecalcLineXPoint(const char* szName, int nTimeDiff);

	CPoint m_ptCursor;				// 마우스 클릭 위치

// [05/01/19] AddIn Object가 비정형 Interface를 제공할 수 있도록 제공한다.
protected:
	virtual BOOL	InvokeAddInStr( LPCTSTR p_szCommandName, LPCTSTR p_szData);

// Event Overriding
protected:
	// Declare AddIn Event Map
	DECLARE_ADDIN_EVENT_MAP()

// (2004.10.11, 배승원) AddIn DLL에 OCX Event를 전달한다.
protected:
	// [04/12/25] Draw Event를 AddIn DLL에 알려 처리될 수 있도록 한다.
	virtual void	OnDrawAfterGraph( HDC p_hDC);
	// (2007/1/7 - Seung-Won, Bae) Support AddIn Cursor Operation.
	virtual RECT	DrawCursor( HDC p_hDC);
	// [04/10/11] AddIn DLL들에게 ResetChart 상황을 알려 처리되도록 한다.
	virtual void	OnResetChart( void);
	// RQ별 Packet : 복수종목 - ojtaso (20070111)
	virtual void	OnPacketDataMultiItem(LPCTSTR lpszRQ, const char *p_szPacketNames, BOOL p_bWithReset);
	// (2006/10/4 - Seung-Won, Bae) On Real Packet End
	virtual void	OnRealPacketEnd( const char *p_szRQ);
	// (2006/11/24 - Seung-Won, Bae) On Tool Command
	virtual void	OnToolCommand( const int p_nToolType, const int p_nCmdOption);
	
// (2006/1/17 - Seung-Won, Bae) Tool Interfaces
//		Caution! If the Event is processed, it returns TRUE.
//					and You have not to pass the Event to a next Event Routine.
protected:
	virtual BOOL	OnCmdMsg(			UINT nID, int nCode, ICmdUI *p_pICmdUI);
	virtual BOOL	OnAddInToolCommand( const EAICommandType p_eCommandID, const char *p_szParam, const long p_lParam);
protected:
	virtual BOOL	OnKeyDown(			UINT nChar, UINT nRepCnt, UINT nFlags);
	virtual BOOL	OnMouseMove(		HDC p_hDC, UINT nFlags, POINT &point, const DWORD p_dwMouseActionID, const HBITMAP p_hbmpMainOSB);
	virtual BOOL	OnLButtonDown(		HDC p_hDC, UINT nFlags, POINT &point, const DWORD p_dwMouseActionID);
	virtual BOOL	OnLButtonUp(		HDC p_hDC, UINT nFlags, POINT &point, const DWORD p_dwMouseActionID);
	virtual BOOL	OnLButtonDblClk(	HDC p_hDC, UINT nFlags, POINT &point, const DWORD p_dwMouseActionID);
	virtual BOOL	OnRButtonDown(		HDC p_hDC, UINT nFlags, POINT &point, const DWORD p_dwMouseActionID);
	virtual BOOL	OnRButtonUp(		HDC p_hDC, UINT nFlags, POINT &point, const DWORD p_dwMouseActionID);

	CDialog* m_pDlgConfirm;		//@Solomon100319
};

#endif // !defined(AFX_CHARTORDERLINEADDINIMP_H__DA9542DB_C2AE_4A00_8CA5_CE7334AB74E4__INCLUDED_)
