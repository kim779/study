// SiteCustomAddin.h: interface for the CSiteCustomAddin class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SITECUSTOMADDIN_H__4DCF1075_E1B5_48E4_9C3B_FECE5FDB70A1__INCLUDED_)
#define AFX_SITECUSTOMADDIN_H__4DCF1075_E1B5_48E4_9C3B_FECE5FDB70A1__INCLUDED_

// (2004.10.08, 배승원) Site별 Custom AddIn Object이다.

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "./Include_AddIn/I139092/_ChartAddInBase.h"		// for CChartAddInBase
#include "./Include_AddIn/I139092/_IChartOcx.h"				// for IChartOCX;
#include "./Include_AddIn/I139092/_AddInCommand.h"

#include "WndAssistIndexManager.h"
#include "../../inc/ISTAddinManager.h"						// for ISTAddinData
#define DATAOPTION_TR	0
#define DATAOPTION_REAL	1

// RQ별 관리 : 복수종목 - ojtaso (20070305)
#include <map>
#include <string>

// RQ별 관리 : 복수종목 - ojtaso (20070305)
typedef std::map<std::string, CWndAssistIndexManager*>		MAP_WNDASSISTINDEXMANAGER;

class CSiteCustomAddin : public CChartAddInBase
{
public:
	CSiteCustomAddin( IChartOCX *p_pIChartOCX, IAddInDllBase *p_pIAddInDllBase);
	virtual ~CSiteCustomAddin();

	// Declare AddIn Event Map
	DECLARE_ADDIN_EVENT_MAP()

// [05/07/08] AddIn 전용 Interface를 제공한다.
//		*. 주의. 반듯이 전용 Interface Pointer로 Converting후 IUnknown의 Pointer로 변환하여 전달한다.
//		*. 또한 다음의 _IWRAPPER_IUNKNOWN_INTERFACE도 정의한다.
protected:
	virtual IUnknown *	GetAddInCustomInterface( void);
// (2005.07.08, 배승원) AddIn Special Interface의 제공을 위하여 기본 Interface (IUnknown용)를 제공한다.
//		*. 위의 GetAddInCustomInterface()의 기능 제공을 위한 것으로, 
//			GetAddInCustomInterface() 삭제시 함께 삭제한다.
protected:
	_IWRAPPER_IUNKNOWN_INTERFACE()

// *******************************************************************************
// Pattern Maker
protected:
//	CWndAssistIndexManager m_assistIndexManager;
// ********************************************************************************

public:
	LPCTSTR m_lpszRQ;		// 현재 RQ : 복수종목 - ojtaso (20070305)
	MAP_WNDASSISTINDEXMANAGER	m_mapWndAssistIndexManager; // RQ별 관리 : 복수종목 - ojtaso (20070305)

// (2004.10.15, 배승원) Real 관련 Event를 전달한다.
protected:
	// [04/10/11] Draw Event를 AddIn DLL에 알려 처리될 수 있도록 한다.
	// virtual void	OnDrawBeforeGraph( CDC *p_pDC);
	virtual void	OnDrawBeforeGraph( HDC p_hDC);
	// [04/10/15] Real Packet의 수신이 끝났음을 AddIn DLL에게 알려 처리토록 한다.
	//virtual void	OnDrdsPacketEnd( WPARAM p_wParam, LPARAM p_lParam);

// (2004.10.22, 배승원) AddIn DLL에게 Mouse Event를 전달한다.
public:
	// JSJ_ADD_50119__차트에서 InvokeAddIn함수 호출함
	 virtual BOOL	InvokeAddIn( int p_nCommandType, long p_lData);	
protected:
	// [06/12/22] 초기화 이벤트에 대한 처리
	virtual void	OnResetChart( void);
	// [04/10/13] TR의 수신을 AddIn DLL들에게 알려 대응토록 한다.
	virtual void	OnPacketData( const char *p_szPacketNames, BOOL p_bWithReset);
	// [04/12/07] TR이나 Real의 수신에 의해 Packet Data가 Update되었음을 AddIn DLL들에 알려 지표등을 처리토록 한다.
	virtual void	OnUpdatePacketData( BOOL p_bTrUpdate);
	// RQ별 Packet : 복수종목 - ojtaso (20070111)
	virtual void	OnPacketDataMultiItem(LPCTSTR lpszRQ, const char *p_szPacketNames, BOOL p_bWithReset);
	// 복수종목별 Real반영 : 복수종목 - ojtaso (20070131)
	virtual void	OnUpdatePacketDataMultiItem( LPCTSTR lpszRQ, BOOL p_bTrUpdate);

protected:
	// 지표를 계산한다.(지표추가, 지표변경)
	// (2006/1/9 - Seung-Won, Bae) Update for Independence of Interface Version Number.
	//			~ CObject * <= const CList<CPacket*, CPacket*> *
	// 해당 RQ의 AssistIndexManager : 복수종목 - ojtaso (20070305)
	virtual BOOL CalculateIndicator(LPCTSTR lpszRQ, LPCTSTR lpszParamCombine, CObject *p_plCPacket);
	// 지표를 삭제한다.
	// 해당 RQ의 AssistIndexManager : 복수종목 - ojtaso (20070305)
	virtual BOOL DeleteIndicator(LPCTSTR lpszRQ, LPCTSTR lpszParamCombine);
	virtual BOOL CalculateAll(const int nDataOption);

// (2006/1/17 - Seung-Won, Bae) Tool Interfaces
//		Caution! If the Event is processed, it returns TRUE.
//					and You have not to pass the Event to a next Event Routine.
protected:
	virtual BOOL	OnAddInToolCommand(	const EAICommandType p_eCommandID, const long p_lParam);	//JS.Kim_20100908 데이타건수 초과시 삭제할때 지표 반영안되는 문제 수정
	virtual BOOL	OnAddInToolCommand(	const EAICommandType p_eCommandID, const char *p_szParam1, const char *p_szParam2);
	virtual BOOL	OnAddInToolCommand(	long &p_lResult, const EAICommandType p_eCommandID, const char *p_szParam1, const char *p_szParam2, CObject *p_pParam);
	virtual BOOL	OnAddInToolCommand(	long &p_lResult, const EAICommandType p_eCommandID, const long p_lParam);

	// 복수종목 지원을 위해 추가 : 복수종목 - ojtaso (20070125)
	virtual BOOL	OnAddInToolCommand(	long &p_lResult, const EAICommandType p_eCommandID, const char *p_szParam1, const char *p_szParam2, const char *p_szParam3, CObject *p_pParam);
	virtual BOOL	OnAddInToolCommand(	const EAICommandType p_eCommandID, const long p_lParam, const char *p_szParam1, const char *p_szParam2, const char *p_szParam3);
	// 복수종목 추가/삭제 : 복수종목 - ojtaso (20070302)
	virtual BOOL	OnAddInToolCommand(	const EAICommandType p_eCommandID, const char *p_szParam);

protected:
	// virtual BOOL	OnLButtonDblClk(	CDC *pDC, UINT nFlags, CPoint &point, const DWORD p_dwMouseActionID);
	virtual BOOL	OnLButtonDblClk(	HDC p_hDC, UINT nFlags, POINT &point, const DWORD p_dwMouseActionID);

	// 해당 RQ의 AssistIndexManager : 복수종목 - ojtaso (20070305)
	void GetAssistIndexManager(CWndAssistIndexManager*& pAssistIndexManager, LPCTSTR lpszRQ = NULL);

public:
	class CSTAddinData : public ISTAddinData
	{
		// ST에서 사용하는 기본 패킷 이외의 사용 가능한 데이터 추가 - ojtaso (20080609)
		STDMETHOD_(int, GetChartData)(LONG& dwDataTime, LONG& dwDataOpen, LONG& dwDataHigh, LONG& dwDataLow, LONG& dwDataClose,LONG& dwDataVolume, LONG& dwDataExt);
	}m_xSTAddinData;
	friend class CSTAddinData;
};

#endif // !defined(AFX_SITECUSTOMADDIN_H__4DCF1075_E1B5_48E4_9C3B_FECE5FDB70A1__INCLUDED_)
