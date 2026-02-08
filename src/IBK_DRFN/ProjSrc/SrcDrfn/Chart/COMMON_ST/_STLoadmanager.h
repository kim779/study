#if !defined(AFX_STLOADMANAGER_IMPLE_H__6029D970_3E68_4DF0_84BE_B12B9DD33C6F__INCLUDED_)
#define AFX_STLOADMANAGER_IMPLE_H__6029D970_3E68_4DF0_84BE_B12B9DD33C6F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// _STLoadManager.h

#include "../../inc/ISTLoadManager.h"
#include "STGfxOutBarCtrl.h"
#include "STListCtrl.h"
//{{ 20061012_ST관련추가
//#include "Path.h"
//}} 20061012_ST관련추가
#include "afxtempl.h"
#include "../../inc/ISTManager.h"

typedef struct {
	CString szSTName;
	CString szRawName;
}STFILEITEM;
typedef CList<STFILEITEM*, STFILEITEM*> List_STFILEITEM;
#define	Delete_List_STFILEITEMP(xpSTList) { for(POSITION pos=xpSTList->GetHeadPosition(); pos; ) { delete xpSTList->GetNext(pos); } xpSTList->RemoveAll(); }
#define	Delete_List_STFILEITEM(xSTList) { for(POSITION pos=xSTList.GetHeadPosition(); pos; ) { delete xSTList.GetNext(pos); } xSTList.RemoveAll(); }

class CSTLoadManager : public ISTLoadManager2
{
// Construction
public:
	STDMETHOD_(void, Init)(BOOL bSubFolder, CString szExtName, CImageList* pILSTIcon);
	STDMETHOD_(void, LoadST)(CWnd* pOutBar, CStringArray* parSTPath, DWORD dwSTTypeToLoad, int nDefaultSelFolder);
	STDMETHOD_(BOOL, UnloadST)();
	STDMETHOD_(int,  SearchST)(CString szSTName, CStringArray& arFindIndx);
	STDMETHOD_(BOOL, UpdateST)(int nState, CString szSTPath);
	STDMETHOD_(int,	 GetFolderST)(DWORD dwInfo);
	STDMETHOD_(int,	 GetItemST)(DWORD dwInfo);
	STDMETHOD_(void, SetSelectedItem)(int nFolderIndx = -1, int nItemIndx = -1);
	STDMETHOD_(void, SetUseBoldFont)(BOOL bUseBoldFont = TRUE) {m_bUseBoldFont = bUseBoldFont;}

	// p_LoadOption :	0 이면 LoadST(..) 그대로 호출
	//					1 이면 조합가능한 전략만 로딩(지표,강세/약세,조건등 제외)
	//					2 이면 사용자 전략 로딩 안함.
	//					And(&) 조합가능함.
	STDMETHOD_(void, LoadST2)(CWnd* pOutBar, CStringArray* pArSTPath, DWORD dwSTTypeToLoad, int nDefaultSelFolder, int p_LoadOption) ;
	STDMETHOD_(void, SetSelectedItem)(STMNG_ITEMINFO* pInData);

// 생성
public:
	CSTLoadManager();
	CSTLoadManager(CImageList* pILSTIcon, CWnd* pParent = NULL);

// 멤버변수	
protected:
	enum
	{
		Basic_STChange_Delete=0x10,		//초급전략의 삭제
		Basic_STChange_Add=0x11,		//초급전략의 추가
		Ext_STChange_SDDelete=0x20,		//고급전략의 사용자단위전략삭제
		Ext_STChange_SDAdd=0x21,		//고급전략의 사용자단위전략추가
		Ext_STChange_Delete=0x40,		//고급전략의 사용자조합전략삭제
		Ext_STChange_Add=0x41,			//고급전략의 사용자조합전략추가
	};

	CSTGfxOutBarCtrl*					m_pOutBarCtrl;
	CStringArray*						m_parSTPath;			// Data / User 폴더 경로 StringArray
	CString								m_szCurrentSTPath;		// 검색중인 전략 폴더경로
	CImageList*							m_pILSTIcon;

	CString								m_szExtName;			// 파일파인더를 통해 검색할 파일 확장자
	BOOL								m_bSubFolders;			// 폴더 내의 서브까지 검색할지 여부

	UINT								m_uSTIndx;				// 폴더를 의미하는 ListCtrl ID Indexing
	CString								m_szSTPath;
	BOOL								m_bSearching;			// 파일파인더를 통해 검색 중
	BOOL								m_bSTUserLoading;		// 파일파인더를 통해 사용자전략파일 검색 중
	UINT								m_uSTUserCount;			// ListCtrl ID 계산을 위해 사용자단위/조합전략 폴더 Counting
	DWORD								m_dwSTTypeToLoad;		// Reserved
	BOOL								m_bUseBoldFont;
	//int									m_nLoadOption;			// 아웃룩바 로드옵션


// 멤버함수
public:
	BOOL GetSelectedItem(STMNG_ITEMINFO& InData);
	void DeleteSTFile(int nUserIndx, CString szSTFile);
	BOOL GetRunSTItemInfo(CString szSTName, STMNG_ITEMINFO& InData);

protected:
	void LoadSTList(BOOL bOnlyUser = FALSE);

	// p_LoadOption :	0 이면 LoadST(..) 그대로 호출
	//					1 이면 조합가능한 전략만 로딩(지표,강세/약세,조건등 제외)
	//					2 이면 사용자 전략 로딩 안함.
	//					And(&) 조합가능함.
	void LoadSTListFromSTIndex(int p_LoadOption, BOOL bOnlyUser = FALSE);

	BOOL IsSTTypeToLoad(CString& szFileTitle, CString& szSTType);
	void AddFolder(LPCTSTR szSTFolderPath, LPCSTR szLastFolerName);
	void AddSTFile(LPCTSTR szSTFilePath, LPCSTR p_szShowName=NULL);
	void GetFolderName(CString szPath, CString& szSTFolder);
	void SetExtName(CString szExtName)	{ m_szExtName = szExtName; }
	int	 GetFolderIndexFromFolderName(CString szFolderName);

	// 서브디렉토리를 찾기위한 루틴
	int	DirectorySearch(LPCSTR pszStartDir, LPCSTR pszExt, BOOL bSub=TRUE);

	// 폴더에서 해당타입의 전략을 읽어서 추가한다.
	void LoadSTFolderList(LPCSTR p_szGroupName, LPCSTR p_szTargetDir, int p_nSTType);

	/*	p_nSTType은 ST_ReadFrom_Dir 일때만 적용한다.
		1	 : @1	-> 지표
		237 : @2,@3,@7 -> 지표, 강세약세 제외
		5	 : @5	-> 강세약세
		999 : 전부로딩 */
	static int GetSTFileList(LPCSTR p_szDir, int p_nSTType, List_STFILEITEM *p_arrSTList);
	static int GetSTFileList(LPCSTR p_aTarDir, CStringArray &p_arr);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STLOADMANAGER_IMPLE_H__6029D970_3E68_4DF0_84BE_B12B9DD33C6F__INCLUDED_)
