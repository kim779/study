#if !defined(AFX_INTERFACE_FOR_SUPPORT_REGISTY_3_BY_JUNOK_LEE_B1A__INCLUDED_)
#define AFX_INTERFACE_FOR_SUPPORT_REGISTY_3_BY_JUNOK_LEE_B1A__INCLUDED_

// #include "../../_Include/IRegistryMng.h"
#include "IBaseDefine.h"

// 	IRegistryManager* pRegMng = (IRegistryManager*)AfxGetMainWnd()->SendMessage(RMSG_INTERFACE, 17, 0);
//  pRegMng->GetStrValue(HKEY_CURRENT_USER, strSubKey, "Position_X", "");
//	int nID = 17;
//	IRegistryManager* pMng = (IRegistryManager*)AfxGetPctrInterface(nID);

DECLARE_INTERFACE(IRegistryManager)
{
	STDMETHOD_(void,   SetStrValue)(HKEY hHkey,LPCSTR strKey, LPCSTR strSubKey, LPCSTR strKeyValue) PURE;
	STDMETHOD_(LPCSTR, GetStrValue)(HKEY hHkey,LPCSTR strkey, LPCSTR strsubkey, LPCSTR strDefault) PURE;
};


#endif //AFX_INTERFACE_FOR_SUPPORT_REGISTY_3_BY_JUNOK_LEE_B1A__INCLUDED_
