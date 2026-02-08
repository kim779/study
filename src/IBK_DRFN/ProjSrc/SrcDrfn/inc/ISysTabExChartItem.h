#if !defined(AFX_SYSTABEX_CHARTITEM_H__11999AEF_8200_4DA5_B205_33C1402F54D5__INCLUDED_)
#define AFX_SYSTABEX_CHARTITEM_H__11999AEF_8200_4DA5_B205_33C1402F54D5__INCLUDED_

//#include "../Src/Chart_common/EuChart_ocx/euchart.h"
 
DECLARE_INTERFACE(ISysTabExChartItem)
{
	STDMETHOD_(long, GetChartItem)() PURE;
	STDMETHOD_(void, SetChartItem)(long dwNewChartItem) PURE;
	STDMETHOD_(BOOL, AddPacketAndGraph) () PURE;
	STDMETHOD_(BOOL, SetPacketData)(LPCTSTR szPacketName, long szData, long nLength, BOOL bReset) PURE;
	STDMETHOD_(void, SetNOnePageDataCount)(long propVal) PURE;
	STDMETHOD_(void, ShowInnerToolBar)(BOOL p_bShow) PURE;
	STDMETHOD_(BOOL, InvokeAddIn) (LPCTSTR p_szAddInDllName, short p_nCommandType, long p_lData) PURE;
	STDMETHOD_(BOOL, AddChartBlock) (LPCTSTR lpChartName) PURE;
	STDMETHOD_(CString, GetPacketName)() PURE;
};

#endif // !defined(AFX_SYSTABEX_CHARTITEM_H__11999AEF_8200_4DA5_B205_33C1402F54D5__INCLUDED_)
