// DataInterfaceAddInImp.h: interface for the CDataInterfaceAddInImp class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DATAINTERFACEADDINIMP_H__63C70C25_DF9E_4B99_9E0C_F321155452C9__INCLUDED_)
#define AFX_DATAINTERFACEADDINIMP_H__63C70C25_DF9E_4B99_9E0C_F321155452C9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "../Include_AddIn/I000000/_ChartAddInBase.h"		// for CChartAddInBase
#include "IPacketQueryer.h"									// for IPacketQueryer

//-----------------------------------------------------------------------------
// Author		: Seung-Won, Bae	Date :2007/3/28
// Comments		: 
// Using way	: Data Interface AddIn.
// See			: 
//-----------------------------------------------------------------------------

class CDataInterfaceAddInImp  : public CChartAddInBase, public IPacketQueryer
{
public:
	CDataInterfaceAddInImp( IChartOCX *p_pIChartOCX, IAddInDllBase *p_pIAddInDllBase);
	virtual ~CDataInterfaceAddInImp();

	// Declare AddIn Event Map
	DECLARE_ADDIN_EVENT_MAP()

// (2006/2/15 - Seung-Won, Bae) Multiple Item in DLL
public:
	static CString		m_strAddInItemName;

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

// (2007/3/28 - Seung-Won, Bae) Suppoert Double Value List of Packet
protected:
	virtual IDblList *	GetDoubleList( const char *p_szChartName, const char *p_szSubGraphName, const int p_nPacketIndex);
};

#endif // !defined(AFX_DATAINTERFACEADDINIMP_H__63C70C25_DF9E_4B99_9E0C_F321155452C9__INCLUDED_)
