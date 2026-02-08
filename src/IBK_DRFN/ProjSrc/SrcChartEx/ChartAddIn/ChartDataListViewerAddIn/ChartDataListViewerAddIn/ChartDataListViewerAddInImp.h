// ChartDataListViewerAddInImp.h: interface for the CChartDataListViewerAddInImp class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CHARTDATALISTVIEWERADDINIMP_H__87872DD8_9073_43F7_AE8D_BFBAF155C985__INCLUDED_)
#define AFX_CHARTDATALISTVIEWERADDINIMP_H__87872DD8_9073_43F7_AE8D_BFBAF155C985__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Include_AddIn.h"							// for Ixxxxxx.h

//20081025 이문수 >>
//class CDlgDataListViewer;
class CDataListViewMngWnd;
//20081025 이문수 <<

class CChartDataListViewerAddInImp : public CChartAddInBase  
{
public:
	CChartDataListViewerAddInImp( IChartOCX *p_pIChartOCX, IAddInDllBase *p_pIAddInDllBase);
	virtual ~CChartDataListViewerAddInImp();

	// Declare AddIn Event Map
	DECLARE_ADDIN_EVENT_MAP()

// (2006/2/15 - Seung-Won, Bae) Multiple Item in DLL
public:
	static CString		m_strAddInItemName;

// (2006/6/22 - Seung-Won, Bae) Manage DataListViwer Dialog
protected:

	//20081025 이문수 >>
	CDataListViewMngWnd *   m_pDataListViewMngWnd;
	//20081025 이문수 <<

//20081023 이문수 >>
public:
	CString GetPacketNameList();
	IChartManager *	GetIChartManager();
	IChartOCX *		GetIChartOCX();
	IPacketManager *GetIPacketManager();
//20081023 이문수 <<


// (2004.10.11, 배승원) 기본적인 Interface를 Default로 제공한다. (Derived Class에서 재정의하지 않아도 오류가 나지 않도록)
protected:
	// (2007/1/18 - Seung-Won, Bae) Notify Recalculation.
	virtual void	OnRecalculationComplete( void);
	// (2006/9/25 - Seung-Won, Bae) On Graph Added.
	virtual void	OnGraphAdded( const char *p_szGraphName, const char *p_szIndicatorName);
	// (2006/9/27 - Seung-Won, Bae) On Graph Deleted.
	virtual void	OnGraphDeleted( const char *p_szGraphName, const char *p_szIndicatorName);
	// (2006/11/24 - Seung-Won, Bae) On Tool Command
	virtual void	OnToolCommand( const int p_nToolType, const int p_nCmdOption);
	// RQ별 Packet : 복수종목 - ojtaso (20070111)
	virtual void	OnPacketDataMultiItem(LPCTSTR lpszRQ, const char *p_szPacketNames, BOOL p_bWithReset);
	// (2006/10/4 - Seung-Won, Bae) On Real Packet End
	virtual void	OnRealPacketEnd( const char *p_szRQ);

// (2006/1/17 - Seung-Won, Bae) Tool Interfaces
//		Caution! If the Event is processed, it returns TRUE.
//					and You have not to pass the Event to a next Event Routine.
protected:
	virtual BOOL	OnCmdMsg(			UINT nID, int nCode, ICmdUI *p_pICmdUI);
	virtual BOOL	OnAddInToolCommand(	const EAICommandType p_eCommandID, const char *p_szParam, const long p_lParam);// 20081007 JS.Kim	사용자시간대 선택
};

#endif // !defined(AFX_CHARTDATALISTVIEWERADDINIMP_H__87872DD8_9073_43F7_AE8D_BFBAF155C985__INCLUDED_)
