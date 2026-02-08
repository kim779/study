// STControllerLoader.h: interface for the CSTControllerLoader class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_STCONTROLLERLOADER_H__FA1E0F5D_75B3_42CD_8E31_E4D79BCF90BB__INCLUDED_)
#define AFX_STCONTROLLERLOADER_H__FA1E0F5D_75B3_42CD_8E31_E4D79BCF90BB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

typedef  long (WINAPI* _DLL_GetSTController)();
typedef  long (WINAPI* _DLL_GetSTControllerCfg)();

#include "../Include_ST/ISTController.h"
class CSTControllerLoader  
{
public:
	CSTControllerLoader();
	CISTController *GetSTController();
	CISTController *GetSTControllerCfg();
	virtual ~CSTControllerLoader();
private:
	BOOL LoadSTController();
	BOOL UnloadSTController();
	_DLL_GetSTController m_fnGetSTController;
	_DLL_GetSTControllerCfg	m_fnGetSTControllerCfg;
	HINSTANCE m_hSTController;
	CString m_strSTController;

// (2006/3/5 - Seung-Won, Bae) Manage Debug Version Flag and Path of Chart OCX
protected:
	static int		m_bIsDebugVersion;
	static CString	m_strChartOcxFolder;
};

#endif // !defined(AFX_STCONTROLLERLOADER_H__FA1E0F5D_75B3_42CD_8E31_E4D79BCF90BB__INCLUDED_)
