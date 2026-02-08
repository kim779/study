#if !defined(AFX_INTERFACE_OF_ACCOUNTMNG_BY_OJTASO_8B1A__INCLUDED_)
#define AFX_INTERFACE_OF_ACCOUNTMNG_BY_OJTASO_8B1A__INCLUDED_

//  #include "IBaseDefine.h"
//  int nID = 15;
// 	IAccountManager* pManager = (IAccountManager*)AfxGetMainWnd()->SendMessage(RMSG_INTERFACE, nID, 0);
//	IAccountManager* pManager = (IAccountManager*)AfxGetPctrInterface(nID);
//
//  만든이 : 오정택
//

#ifndef _STACCTINFO_
#define _STACCTINFO_
// GlobalDlgMapManager.h 에 임시로 정의해서 사용하고 있음.
// 수정시 GlobalDlgMapmanager.h 도 수정해야함.
typedef struct {
	char	acct[11];
	char	name[20];
	char	passwd[8];
	int		nType;		// 1:01, 2:11, 3:33, 4:60, 5:91, 6:99
	BOOL	bTrust;		// 위임계좌 여부 
	BOOL	bSave;		// 계좌비밀번호 저장 
}STACCTINFO, *LPSTACCTINFO;

#ifndef _ST_ACCTLIST_
#define _ST_ACCTLIST_
// 계좌 정보
typedef struct {
	char acct_gb	[ 1];	// 위임구분 0:본인 1:위임
    char acct_no    [11];   // 계좌번호(3+2+6)
    char acct_name  [20];   // 계좌명
} ST_ACCTLIST;

#endif //_ST_ACCTLIST_

#include <AFXTEMPL.H>
typedef CList<LPSTACCTINFO, LPSTACCTINFO> CAcctList;

#endif //_STACCTINFO_

DECLARE_INTERFACE(IAccountManager)
{
	STDMETHOD_(void, ClearAll)() PURE;
	STDMETHOD_(void, Add)(CString _szData, BOOL& bSavePass) PURE;
	STDMETHOD_(int, GetKeyString)(CString _szKey, CString& rString) PURE;
	STDMETHOD_(int, GetKeyString2)(CString _szKey, CString rOrigin, CString& rString) PURE;
	STDMETHOD_(int, GetAllString)(CString& rString) PURE;
	STDMETHOD_(int, GetAllString2)(CString& rString) PURE;
	STDMETHOD_(CString, GetKey)(CString szAcct) PURE;
	STDMETHOD_(void, MoveToHead)(CString _szKey, CString szAcct, CString& rString, BOOL bAddValue) PURE;
	STDMETHOD_(CString, GetAcctPasswd)(CString szAcct) PURE;
	STDMETHOD_(void, SetAcctPasswd)(CString szAcct, CString szPasswd) PURE;
	STDMETHOD_(void, Copy)(LPVOID pOrig) PURE;
};

#endif // AFX_INTERFACE_OF_ACCOUNTMNG_BY_OJTASO_8B1A__INCLUDED_
