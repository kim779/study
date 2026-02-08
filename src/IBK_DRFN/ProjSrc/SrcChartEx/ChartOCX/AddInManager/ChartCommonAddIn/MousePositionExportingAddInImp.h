// MousePositionExportingAddInImp.h: interface for the CMousePositionExportingAddInImp class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MOUSEPOSITIONEXPORTINGADDINIMP_H__E6710142_C7F4_4B85_BE06_76BDA95B8C7F__INCLUDED_)
#define AFX_MOUSEPOSITIONEXPORTINGADDINIMP_H__E6710142_C7F4_4B85_BE06_76BDA95B8C7F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#include "../Include_AddIn/I000000/_ChartAddInBase.h"		// for CChartAddInBase


// Declare Interface
interface IChartOCX;
interface IBlock;

class CMousePositionExportingAddInImp : public CChartAddInBase  
{
public:
	CMousePositionExportingAddInImp( IChartOCX *p_pIChartOCX, IAddInDllBase *p_pIAddInDllBase );
	virtual ~CMousePositionExportingAddInImp();
	
	// Declare AddIn Event Map
	DECLARE_ADDIN_EVENT_MAP()

// Attribute
public:
	static CString m_strAddInItemName;		// Multiple Item in DLL

protected:
	CRect m_rctGraphDrawingRegion;
	CRect m_rctBlockRegion;

	CString m_strDateCompartment;
	CString m_strTimeCompartment;

	CString		m_strXScaleFormatedData;
	CString		m_strYScaleFormatedData;
	double		m_dXScaleOriginalData;
	double		m_dYScaleOriginalData;

	CString m_strPacketType;

	int			m_nVScaleGroupIndex;
	IBlock *	m_pIBlock;
	BOOL		m_bPacketTypeIsString;

// (2007/1/17 - Seung-Won, Bae) Support Data Index with Default.
protected:
	BOOL		m_bXDataExport;

// (2007/1/19 - Seung-Won, Bae) Support Not-Change Data Filter
protected:
	BOOL		m_bSameDataFilter;
	double		m_dData;
	long		m_lData;

// Method
protected:
	BOOL GetScaleData( CPoint ptPoint );
	BOOL GetStringfromDataIdx( int nDataIdx );
	
	BOOL GetIBlock( CPoint ptPoint );
	BOOL GetGraphBlockRegion( CPoint ptPoint );			// 
	
	BOOL GetXScaleDataInPoint( CPoint ptPoint );
	BOOL GetYScaleDataInPoint( CPoint ptPoint );

// [05/01/19] AddIn Object가 비정형 Interface를 제공할 수 있도록 제공한다.
protected:
	virtual BOOL	InvokeAddIn( int p_nCommandType, long p_lData);	// Set XDataExport Flag.

protected:
	virtual BOOL	OnMouseMove(		HDC p_hDC, UINT nFlags, POINT &point, const DWORD p_dwMouseActionID, const HBITMAP p_hbmpMainOSB);	// (2006/11/5 - Seung-Won, Bae) Support MainOSB
};

#endif // !defined(AFX_MOUSEPOSITIONEXPORTINGADDINIMP_H__E6710142_C7F4_4B85_BE06_76BDA95B8C7F__INCLUDED_)
