// KeyboardScrollAddInImp.h: interface for the CKeyboardScrollAddInImp class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_KEYBOARDSCROLLADDINIMP_H__B3552936_BFCA_42E7_8DB2_782F1B56CDEE__INCLUDED_)
#define AFX_KEYBOARDSCROLLADDINIMP_H__B3552936_BFCA_42E7_8DB2_782F1B56CDEE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "../../Include_Chart/OutsideCommonInterfaceFunction.h"		// for COutsideCommonInterfaceFunction
#include "../Include_AddIn/I000000/_ChartAddInBase.h"		// for CChartAddInBase

//-----------------------------------------------------------------------------
// Author		: Seung-Won, Bae	Date :2006/3/5
// Comments		: Keyboard Scroll Tool AddIn
// Using way	: 
// See			: 
//-----------------------------------------------------------------------------

class CKeyboardScrollAddInImp : public CChartAddInBase
{
public:
	CKeyboardScrollAddInImp( IChartOCX *p_pIChartOCX, IAddInDllBase *p_pIAddInDllBase);
	virtual ~CKeyboardScrollAddInImp();

	// Declare AddIn Event Map
	DECLARE_ADDIN_EVENT_MAP()

// (2006/2/15 - Seung-Won, Bae) Multiple Item in DLL
public:
	static CString		m_strAddInItemName;

// (2006/1/17 - Seung-Won, Bae) Tool Interfaces for Inquiry Tooltip
//		Caution! If the Event is processed, it returns TRUE.
//					and You have not to pass the Event to a next Event Routine.
protected:
	virtual BOOL	OnKeyDown(			UINT nChar, UINT nRepCnt, UINT nFlags);
	virtual BOOL	OnKeyUp(			UINT nChar, UINT nRepCnt, UINT nFlags);
};

#endif // !defined(AFX_KEYBOARDSCROLLADDINIMP_H__B3552936_BFCA_42E7_8DB2_782F1B56CDEE__INCLUDED_)
