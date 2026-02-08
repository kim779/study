// ChartExcelDataImportAddInImp.h: interface for the CChartExcelDataImportAddInImp class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CHARTEXCELDATAIMPORTADDINIMP_H__87872DD8_9073_43F7_AE8D_BFBAF155C985__INCLUDED_)
#define AFX_CHARTEXCELDATAIMPORTADDINIMP_H__87872DD8_9073_43F7_AE8D_BFBAF155C985__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Include_AddIn.h"							// for Ixxxxxx.h

class CChartExcelDataImportAddInImp : public CChartAddInBase  
{
public:
	CChartExcelDataImportAddInImp( IChartOCX *p_pIChartOCX, IAddInDllBase *p_pIAddInDllBase);
	virtual ~CChartExcelDataImportAddInImp();

	// Declare AddIn Event Map
	DECLARE_ADDIN_EVENT_MAP()

// (2006/2/15 - Seung-Won, Bae) Multiple Item in DLL
public:
	BOOL LoadExcel();
	static CString		m_strAddInItemName;

// (2008/3/3 - Seung-Won, Bae) for Multi Language
protected:
	HWND		m_hOcxWnd;

// [05/01/19] AddIn Object가 비정형 Interface를 제공할 수 있도록 제공한다.
protected:
	virtual BOOL	InvokeAddIn( int p_nCommandType, long p_lData);


// (2004.10.11, 배승원) AddIn DLL에 OCX Event를 전달한다.
protected:
	// (2006/11/24 - Seung-Won, Bae) On Tool Command
	virtual void	OnToolCommand( const int p_nToolType, const int p_nCmdOption);
	// RQ별 Packet : 복수종목 - ojtaso (20070111)
	virtual void	OnPacketDataMultiItem(LPCTSTR lpszRQ, const char *p_szPacketNames, BOOL p_bWithReset);

// (2006/1/17 - Seung-Won, Bae) Tool Interfaces
//		Caution! If the Event is processed, it returns TRUE.
//					and You have not to pass the Event to a next Event Routine.
protected:
	virtual BOOL	OnCmdMsg(			UINT nID, int nCode, ICmdUI *p_pICmdUI);
};

#endif // !defined(AFX_CHARTEXCELDATAIMPORTADDINIMP_H__87872DD8_9073_43F7_AE8D_BFBAF155C985__INCLUDED_)
