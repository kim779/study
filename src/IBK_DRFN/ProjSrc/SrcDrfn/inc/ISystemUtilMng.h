#if !defined(AFX_INTERFACE_OF_SYSTEMUTIL_1464_45BB_BY_JUNOK_LEE__INCLUDED_)
#define AFX_INTERFACE_OF_SYSTEMUTIL_1464_45BB_BY_JUNOK_LEE__INCLUDED_

// #include "../../_include/ISystemUtilMng.h"
#include "IBaseDefine.h"

#define		UUID_ISystemUtilManager			42

//  int nID = 24;
// 	ISystemUtilManager* pManager = (ISystemUtilManager*)AfxGetMainWnd()->SendMessage(RMSG_INTERFACE, nID, 0);
//	ISystemUtilManager* pManager = (ISystemUtilManager*)AfxGetPctrInterface(nID);

DECLARE_INTERFACE(ISystemUtilManager)
{
	// OCX등 등록된 여부(TRUE/FALSE)를 리턴한다.
	// 등록된 OCX나 DLL의 경우 clsid 값도 채워서 리턴해준다.
	// 예.	MS차트의 szProgID = "MSChart20Lib.MSChart.2";
	//		TeeChart의 szProgID = "TeeChart.ChartPageNavigator.7";
	STDMETHOD_(BOOL, IsRegistered)(LPCSTR szProgID, CLSID &clsid) PURE;

	// OCX나 DLL을 등록한다.
	// 리턴값	0 : 성공
	//			1 : DLLRegister OleInitialize 실패
	//			2 : LoadLibraryEx 에러
	//			3 : GetProcAddress(DllRegisterServer) 에러
	//			4 : DllRegisterServer 에러
	STDMETHOD_(int, DLLRegister)(LPCSTR szFileName) PURE;
};

DECLARE_INTERFACE_(ISystemUtilManager2, ISystemUtilManager)
{
	// 파일의 존재여부도 체크한다.
	STDMETHOD_(BOOL, IsRegisteredEx)(LPCSTR szProgID, CLSID &clsid, LPCSTR szOcxName, LPCSTR szFullPath=NULL) PURE;
};


#endif //AFX_INTERFACE_OF_SYSTEMUTIL_1464_45BB_BY_JUNOK_LEE__INCLUDED_
