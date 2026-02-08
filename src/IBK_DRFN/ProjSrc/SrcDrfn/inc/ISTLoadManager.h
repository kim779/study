#if !defined(AFX_INTERFACE_OF_STRATELOADMANAGER_BY_JUNOKLEE_060721A__INCLUDED_)
#define AFX_INTERFACE_OF_STRATELOADMANAGER_BY_JUNOKLEE_060721A__INCLUDED_

// #include "../../inc/ISTLoadManager.h" // 전략로드관련 인터페이스 제공.
#include "../../inc/ISTManager.h"


typedef struct 
{
	BOOL    bUserFolder;
	CString szSTPath;
} STLISTITEMDATA;


interface ISTLoadManager
{
	STDMETHOD_(void, Init)(BOOL bSubFolder, CString szExtName, CImageList* pILSTIcon)  PURE;
	STDMETHOD_(void, LoadST)(CWnd* pOutBar, CStringArray* pArSTPath, DWORD dwSTTypeToLoad, int nDefaultSelFolder) PURE;
	STDMETHOD_(BOOL, UnloadST)()  PURE;
	STDMETHOD_(int,  SearchST)(CString szSTName, CStringArray& arFindIndx)  PURE;
	STDMETHOD_(BOOL, UpdateST)(int nState, CString szSTPath)  PURE;
	STDMETHOD_(int,	 GetFolderST)(DWORD dwInfo) PURE;
	STDMETHOD_(int,	 GetItemST)(DWORD dwInfo) PURE;
	STDMETHOD_(void, SetSelectedItem)(int nFolderIndx = -1, int nItemIndx = -1) PURE;
	STDMETHOD_(void, SetUseBoldFont)(BOOL bUseBoldFont = TRUE) PURE;
};

DECLARE_INTERFACE_(ISTLoadManager2, ISTLoadManager)
{
	// p_LoadOption :	0 이면 LoadST(..) 그대로 호출
	//					1 이면 조합가능한 전략만 로딩(지표,강세/약세,조건등 제외)
	//					2 이면 사용자 전략 로딩 안함.
	//					And(&) 조합가능함.
	STDMETHOD_(void, LoadST2)(CWnd* pOutBar, CStringArray* pArSTPath, DWORD dwSTTypeToLoad, int nDefaultSelFolder, int p_LoadOption) PURE;
	STDMETHOD_(void, SetSelectedItem)(STMNG_ITEMINFO* pInData) PURE;
};

#endif // AFX_INTERFACE_OF_STRATELOADMANAGER_BY_JUNOKLEE_060721A__INCLUDED_