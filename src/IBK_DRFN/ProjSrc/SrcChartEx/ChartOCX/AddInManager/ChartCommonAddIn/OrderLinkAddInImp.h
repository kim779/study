// OrderLinkAddInImp.h: interface for the COrderLinkAddInImp class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ORDERLINKADDINIMP_H__D201031A_2204_4FDF_B676_81AB5EAFB10C__INCLUDED_)
#define AFX_ORDERLINKADDINIMP_H__D201031A_2204_4FDF_B676_81AB5EAFB10C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "BoundMarkerAddInImp.h"

interface IChartManager;

class COrderLinkAddInImp : public CBoundMarkerAddInImp  
{
public:
	COrderLinkAddInImp(IChartOCX *p_pIChartOCX, IAddInDllBase *p_pIAddInDllBase);
	virtual ~COrderLinkAddInImp();

	// Declare AddIn Event Map
	DECLARE_ADDIN_EVENT_MAP()

// (2006/2/15 - Seung-Won, Bae) Multiple Item in DLL
public:
	static CString		m_strAddInItemName;

// (2004.10.25, 배승원) 호가연동을 위한 Member Variable을 정의한다.
protected:
	bool		m_bRun;

	int			m_nIndicatorPosY;				// Current Indicator Drawing Position			(IndicatorPosition)
	BOOL		m_bOnSetCaprture;				// Indicator Drawing Situation Flag			(SetCaptureFlag)
	CRect		m_rctAskingPriceMark;			// Indicator Region of BoundMark			(IndicatorRegion)
	double		m_dIndicatedPrice;				// Indicated Price							(IndicatedPrice)
	// (2006/11/5 - Seung-Won, Bae) Use MainOSB
	//	CBitmap *	m_pTextAreaBackup;			// Backup of Indicator Text Area			(TextAreaBackupBitmap)

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


// (2004.10.25, 배승원) SetCapture()후, 현재 설정하는 Cursor를 관리하면서,
//		ReleaseCapture()할 때까지 수시로 OCX에 Cursor를 설정한다.
//		(다른 분석도구들이 수시로 설정을 해제하여 추가적인 설정이 잘 되지 않는다. 구조가 좀...)
protected:
	HCURSOR		m_hAPCursor;

// (2004.10.25, 배승원) Bound값을 설정하는 Interface를 제공한다.
//		확장된 종목코드 정보를 받아 관리하여 종목공유시에 이용한다.
protected:
	CString			m_strItemCode;
protected:
	virtual void	ResetBound( const char *p_szBound);

// (2004.10.26, 배승원) 호가의 정밀도(Precision)을 관리하여 호가 String의 생성에 참고하도록 한다.
protected:
	double		m_dPacketUnitValue;

// (2004.10.26, 배승원) 조회중인 호가 String을 관리하여, 선택시 바로 공유가 발생하도록 한다.
protected:
	CString		m_strAskingPrice;
	CString		m_strRQ;		// 20081031 이주표

protected:
	int				m_nMarketType;
	int				m_nRatioByYCP;			// (2004.11.02, 배승원) 대비율(등락률)인지 가격인지 Flag을 받아 관리한다.
	double			m_dYClosePrice;
// (2004.11.22, 배승원) 전일종가를 m_apiAskingPriceIndicatorAddInImp로부터 추출하여 CYChangeMark에 설정해 동작토록 한다.
protected:
	double			GetYClosePrice( void) const		{	return m_dYClosePrice;	}

// (2004.10.11, 배승원) 기본적인 Interface를 Default로 제공한다. (Derived Class에서 재정의하지 않아도 오류가 나지 않도록)
protected:
	// [04/10/11] AddIn DLL들에게 ResetChart 상황을 알려 처리되도록 한다.
	virtual void	OnResetChart( void);
	// [04/10/11] Draw Event를 AddIn DLL에 알려 처리될 수 있도록 한다.
	virtual void	OnDrawBeforeGraph( HDC p_hDC);
	// RQ별 Packet : 복수종목 - ojtaso (20070111)
	virtual void	OnPacketDataMultiItem(LPCTSTR lpszRQ, const char *p_szPacketNames, BOOL p_bWithReset);
	virtual void	OnDrawAfterGraph(HDC p_hDC);
	// (2006/11/12 - Seung-Won, Bae) Support AddIn ENV Value Map
	virtual void	OnLoadEnvironment( IPropertyMap *p_pIAddInEnvMap);
	virtual void	OnSaveEnvironment( IPropertyMap *p_pIAddInEnvMap);

// (2004.10.29, 배승원) AddIn DLL에게 그밖의 Windows Event를 전달한다.
protected:
	// (2007/1/7 - Seung-Won, Bae) Support AddIn Cursor Operation.
	virtual RECT	DrawCursor( HDC p_hDC);
		// Chart View Info
	BOOL	GetChartBasicData( void);


// (2006/1/17 - Seung-Won, Bae) Tool Interfaces
//		Caution! If the Event is processed, it returns TRUE.
//					and You have not to pass the Event to a next Event Routine.
protected:
	// Tool Commands
	virtual BOOL	InvokeAddInStr( LPCTSTR p_szCommandName, LPCTSTR p_szData);
	virtual void	OnToolCommand( const int p_nToolType, const int p_nCmdOption );
	virtual BOOL	OnAddInToolCommand(	const EAICommandType p_eCommandID, const long p_lParam);
	virtual BOOL	OnAddInToolCommand(	long &p_lResult, const EAICommandType p_eCommandID);
protected:
	virtual BOOL	OnLButtonDblClk(	HDC p_hDC, UINT nFlags, POINT &point, const DWORD p_dwMouseActionID);
	virtual BOOL	OnMouseMove(		HDC p_hDC, UINT nFlags, POINT &point, const DWORD p_dwMouseActionID, const HBITMAP p_hbmpMainOSB);	// (2006/11/5 - Seung-Won, Bae) Support MainOSB
};

#endif // !defined(AFX_ORDERLINKADDINIMP_H__D201031A_2204_4FDF_B676_81AB5EAFB10C__INCLUDED_)
