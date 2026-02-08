// SimpleCompareChartAddInImp.h: interface for the CSimpleCompareChartAddInImp class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SIMPLECOMPARECHARTADDINIMP_H__87F7C304_B1A6_4A0F_9014_759CA0DFD544__INCLUDED_)
#define AFX_SIMPLECOMPARECHARTADDINIMP_H__87F7C304_B1A6_4A0F_9014_759CA0DFD544__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "../Include_AddIn/I000000/_ChartAddInBase.h"		// for CChartAddInBase

// (2009/4/13 - Seung-Won, Bae) Compare Chart, Re-Arrange Logic.
class CSimpleCompareChartAddInImp : public CChartAddInBase
{
public:
	CSimpleCompareChartAddInImp( IChartOCX *p_pIChartOCX, IAddInDllBase *p_pIAddInDllBase);
	virtual ~CSimpleCompareChartAddInImp();

	// Declare AddIn Event Map
	DECLARE_ADDIN_EVENT_MAP()

// (2006/2/15 - Seung-Won, Bae) Multiple Item in DLL
public:
	static CString		m_strAddInItemName;

// [05/01/19] AddIn Object가 비정형 Interface를 제공할 수 있도록 제공한다.
protected:
	virtual BOOL	InvokeAddIn( int p_nCommandType, long p_lData);
	virtual BOOL	InvokeAddInStr( LPCTSTR p_szCommandName, LPCTSTR p_szData);

// (2009/6/4 - Seung-Won, Bae) Parse and Generate ChartListInfo.
protected:		// Parsing Meta Data
	int			m_nBlockColumnHeaderLen;
	int			m_nBlockColumnFooterLen;
	int			m_nBlockHeaderLen;
	int			m_nBlockFooterLen;
	int			m_nVertScaleGroupHeaderLen;
	int			m_nVertScaleGroupFooterLen;
	int			m_nGraphHeaderLen;
	int			m_nGraphFooterLen;
protected:
	int			m_nChartCompareType;
	CString		m_strBlockInfo1;
	int			m_nBlockInfo2;
	CString		m_strBlockInfo3;
	CString		m_strVertScaleGroupInfo;
	CStringList	m_slGraph1;
	CStringList	m_slGraph2;
	CStringList	m_slGraphTitle;
	CStringList	m_slGraph3;
protected:
	void		GetAndParseChartListInfo( void);
	void		GenerateAndSetChartListInfo( void);
};

#endif // !defined(AFX_SIMPLECOMPARECHARTADDINIMP_H__87F7C304_B1A6_4A0F_9014_759CA0DFD544__INCLUDED_)
