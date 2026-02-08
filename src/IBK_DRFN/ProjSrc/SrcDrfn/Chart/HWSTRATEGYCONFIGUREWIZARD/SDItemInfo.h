// SDItemInfo.h: interface for the CSDItemInfo class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SDITEMINFO_H__FAB3832E_21B8_42B5_98DC_C45E8A11D842__INCLUDED_)
#define AFX_SDITEMINFO_H__FAB3832E_21B8_42B5_98DC_C45E8A11D842__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "../Common_ST/DllSTLoader.h"
#include "../Include_ST/ISTControllerD.h"
class CDrMLib_CDBMgr;
class CSDItemInfo  
{
public:
	CSDItemInfo();
	virtual ~CSDItemInfo();
	BOOL SetNewSDItemInfo(HWND hParentWnd, HWND hSocket, CDrMLib_CDBMgr* pDBMgr, LPCTSTR lpName, int nType, LPCTSTR lpPath, LPCTSTR lpSTName,int nNewSTPos);
	void SetpISTControllerD(LPARAM lParam) { m_pISTControllerD = (CISTControllerD *)lParam;}
	CISTControllerD *GetpISTControllerD() { return m_pISTControllerD;} 
	BOOL UpdateData();
	CString m_strName;
	CString m_strPath;
	CString m_strTitle;
	int		m_nType; // (1)SYSTEM_ST, (2)USER_ST
	CString m_strTimeInfo;
	int		m_nIndexInST;

private:
	CDllSTLoader m_loaderOfSTLoader;
	CISTControllerD *m_pISTControllerD;
};

#endif // !defined(AFX_SDITEMINFO_H__FAB3832E_21B8_42B5_98DC_C45E8A11D842__INCLUDED_)
