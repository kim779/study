#if !defined(AFX_TABCODESEARCH_H__7AC8ECCA_35CB_48BC_9B2D_B97956B6470C__INCLUDED_)
#define AFX_TABCODESEARCH_H__7AC8ECCA_35CB_48BC_9B2D_B97956B6470C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TabCodeSearch.h : header file
//
#include "resource.h"

/////////////////////////////////////////////////////////////////////////////
// CTabCodeSearch dialog
#include "TabDlg.h"
#include "XListCtrlJongData.h"
#include "xySplitterWnd.h"
#include "LeftBarDlg.h"
//@유진삭제
//#include "JongStatusInTab.h"
//@유진삭제
#include "./control/TreeCtrlEx.h"
#include "./control/EditUsr.h"
#include "../../inc/ITreeListMng.h"
#include "afxwin.h"
#include "./control/GfxOutBarCtrl.h"
#include "./control/TrTreeCtrl.h"
#include "../../inc/ExGridCtrl.h"
#include "./define/ChartMsgDef.h"
#include "../Chart_Common/CallPutHelper.h"
#include "../../inc/DRCommon.h"

#include "CondTarget.h"

///////////////////////////////////////////////////////////////////////////////////////////////////
//  (  )
#define NAME_a4017     "a4017"

// 기본입력
typedef struct _a4017In
{
    char    accno               [  11];    // [string,   11] 계좌번호
    char    expcode             [  12];    // [string,   12] 종목코드
} a4017In, *LPa4017In;
#define NAME_a4017In     "a4017In"

// 출력2, occurs
typedef struct _a4017Out2
{
    char    expcode             [  12];    // [string,   12] 종목코드
    char    codename            [  20];    // [string,   20] 종목명
    char    pamt                [   9];    // [long  ,    9] 평균단가
} a4017Out2, *LPa4017Out2;
#define NAME_a4017Out2     "a4017Out2"

///////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////
//  (  )
#define NAME_a4018     "a4018"

// 기본입력
typedef struct _a4018In
{
    char    accno               [  11];    // [string,   11] 계좌번호
    char    expcode             [  12];    // [string,   12] 종목코드
    char    gubun               [   1];    // [string,    1] 일분구분
    char    term                [   3];    // [long  ,    3] 일분간격
    char    basedt              [   8];    // [string,    8] 기준일자
    char    basetm              [   6];    // [string,    6] 기준시간
    char    cnt                 [   3];    // [long  ,    3] 요청건수
} a4018In, *LPa4018In;
#define NAME_a4018In     "a4018In"

// 출력1, occurs
typedef struct _a4018Out2
{
    char    procdt              [   8];    // [string,    8] 체결일자
    char    hhmm                [   4];    // [string,    4] 체결시간
    char    scnt                [   5];    // [long  ,    5] 매도체결건수
    char    sconcprc            [   9];    // [long  ,    9] 매도체결단가
    char    bcnt                [   5];    // [long  ,    5] 매수체결건수
    char    bconcprc            [   9];    // [long  ,    9] 매수체결단가
} a4018Out2, *LPa4018Out2;
#define NAME_a4018Out2     "a4018Out2"
///////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////

// 콜 및 풋 옵션 월 리스트 
//class COptionInfo
//{
//public: 
//	COptionInfo();
//
//// Attributes
//	CString m_strCode;
//
//	int m_nCallPut;				// 2 : call / 3 : put
//	CString m_strEvtPrice;		// 행사가
//
//// Functions
//	void GetCode()
//	{
//		return m_strCode;
//	}
//
//	CString GetYM()
//	{
//		return m_strEvtPrice.Mid(0, 4);
//	}
//
//	BOOL IsCall()
//	{
//		return (m_nCallPut == 2);
//	}
//
//	BOOL IsPut()
//	{
//		return (m_nCallPut == 3);
//	}
//
//	void SetInfo(CString strCode, int nCallPut, CString strEvtPrice)
//	{
//		m_strCode = strCode;
//		m_nCallPut = nCallPut;
//		m_strEvtPrice = strEvtPrice;
//	};
//
//	void SetInfo(CString strOptionInfo)
//	{
//		m_strCode = strOptionInfo.Mid(0, 8);
//		CString strTemp;
//		strTemp = strOptionInfo.Mid(0, 1);
//		if(strTemp == "2")
//			m_nCallPut = 2;
//		else if(strTemp == "3")
//			m_nCallPut = 3;
//		else
//			m_nCallPut = 0;
//
//		strTemp = strOptionInfo.Right(19);
//		m_strEvtPrice = strTemp.Mid(2, 10);
//	};
//
//};

//#define UM_CHANGE_REGKW		   ( WM_USER+525 )		// 관심종목 그룹변경 처리용
//@Solomon관심-->
typedef CList<CItemLongData*, CItemLongData*> List_CItemLongData;
//@Solomon관심<--

//tour2k
typedef struct
{
	CStringArray strKey;
	CStringArray strGroupName;
} ST_GwanSimGroup;

class CTabCodeSearch : public CTabDlg
{
// Construction
public:
	CTabCodeSearch(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CTabCodeSearch)
	enum { IDD = IDD_TABCODESEARCH };
	CButton	m_chkAvgPrice;
	CButton	m_chkConcPrice;
	CButton	m_chkRepeat;
	CILButton	m_btnResetOcx;
	CILButton	m_btPrev;
	CILButton	m_btNext;
	CXListCtrlJongData	m_listCode;
	CILButton		m_btPlay;	//004 돌려보기	
	CExComboBox		m_cmbTime;
	//}}AFX_DATA
    CySplitterWnd   m_ySplitterY1;
	void SetRealPriceData(void *pData);
	
//@유진추가
	int m_nCtrlType;			// List or GridCtrl
	CExGridCtrl*	m_pGridCtrl;
	CCallPutIHelper	m_cpHelper;

//	CList<COptionInfo*, COptionInfo*>	m_listOptionInfo1;
//	CList<COptionInfo*, COptionInfo*>	m_listOptionInfo2;
//	CList<COptionInfo*, COptionInfo*>	m_listOptionInfo3;
//	CList<COptionInfo*, COptionInfo*>	m_listOptionInfo4;

	void	InitGrid();
	void	InitGridRowCol(int nRowCount =0);
	void	ChangeGridRowCol();
	void	InsertGridData();
	void	SetMasterData(LIST_CCodeTypeA* pCodeType);
	CString	GetInsertGridDataText(int row, int col);
	void	InitMasterData();
	int		GetGridRow(int nExercise);
//	int		GetGridExercise(int row);
	void	UpdateHogaData(int nRow, int nCol, double dPrice, COLORREF crFgClr, int nPoint=2);
	long	GetRealExercise(int nPrice);
	BOOL	GetCodeInfo(int nType, int& row, int& col, CString& strCode);

	void ChangeListOrGridCtrl(CString szSection);
//@유진추가

//@유진삭제
//	CJongStatusInTab m_JongStatusInTab;
//@유진삭제
	//CExKoscomCtrl m_btnRegFavorite;
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTabCodeSearch)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

	virtual void ChangeSkinColor(COLORREF clrBkgrnd);

public:
//	CArray<CTimerData,CTimerData> m_listTimerData;		// Filter Info
//	int			m_nAlarmFrom;
//	int			m_nAlarmTo;
//	BOOL		m_bUseAlarmSound;
//	BOOL		m_bUseAlarmBkClr;
//	CString		m_strSoundFile;
	void	ReceiveDataFromOutSide();
	void	SetChartTypeControl();
	
	IMasterDataManagerLast*	m_pMasterDataManager;

	CCondTarget			m_CondTarget;
	void	RequestMstCode(LPCTSTR lpBuffer,long lLength);

private:
//004 {{ 돌려보기
	
	BOOL		m_bPlay;		
//004 }}
	int			m_nSelAccount;

	CString		m_strSelectedItemName;
	CString		m_strSelectedParentItemName;
	long		m_lQueryStatus;
	BOOL		m_bSkipSelChangedTreeEvent;

	BOOL		m_bInsertingInListCode;
	HTREEITEM  m_hRootItemUsr;		// 사용자 저장조건
	HTREEITEM  m_hRootItemProposeSpecial;	// 추천조건-특징
	HTREEITEM  m_hRootItemProposeBuy;	// 추천조건-매수
	HTREEITEM  m_hRootItemProposeSell;	// 추천조건-매도
	HTREEITEM  m_hRootItemTheme;	// 시장테마
	HTREEITEM  m_hRootItemSise;		// 시세동향
	HTREEITEM  m_hRootItemInvestor;	// 투자동향
	HTREEITEM  m_hRootItemIndicator;	// 지표신호
	HTREEITEM  m_hRootItemFavorite;		// 관심종목
	HTREEITEM  m_hRootItemAccount;		// 보유종목
	HTREEITEM  m_hRootItemFromOutside;	// 외부로부터의 종목코드
	HTREEITEM  m_hRootItemCurrent;		// 현재 조회된 Item
	HTREEITEM  m_hRootItemCurrentTemp;	// 현재 조회된 Temp Item
	int			m_nSortIndexOfList;
	BOOL		m_bSortTypeOfList;
	CString m_strShareTime;
	long m_nHeightOfSearch;
	BOOL	m_bDeletingTreeData;
	CString m_strPacketFromOutSide;
	BOOL	m_bUseTimer;
	BOOL	m_bIgnoreEditVolume;
	CRect GetRectOfCtrl(INT nID,BOOL bInit=FALSE);
	void	RecalcLayout();
	LONG	OnWmSplitterMoved( UINT wParam, LONG lParam );
	void	InitTreeSearch();
	void	UpdateTreeSearch();
	void	ClearAllTreeSearch();
	void	DeleteChildItem(HTREEITEM hItem);
	void	GetCodeData20001(CString *pStr);
	void	GetCodeData20011(CString *pStr);
	void	GetCodeData00410(CString *pStr);

	void	ReceiveData(LPCSTR szTR, LPVOID aTRData, DWORD dwTRDateLen);
	void	SetConfig(BOOL bLoad = TRUE);
	void	SetTimerInfo(BOOL bLoad = TRUE);
	void	RequestCurrentPrice(LPCTSTR lpBuffer,long lLength);
	void	UpdateItemTheme();
	void	UpdateItemSise(char chType);
	void	RequestJongmokCodesInList(HTREEITEM hItem);
//	long	GetVolumeLimitation();
//	void	SetVolumeInEdit();
//	void	SetTimeFilter();
	void	DeleteFavoriteChild();
	void	IntRequest();
	void	AdviseMain(int nAdivise);
	void	SetAccountInfo(LPCTSTR lpAccountInfo);
	void	RequestSavedQuery();
	CString		m_strFavoriteGrp;
	BOOL		m_bLBtnDown;
	BOOL		m_bRBtnDown;
	BOOL		m_bDragNDrop;
	BOOL		m_bQueryList;

	//최근 트리 쿼리정보 - 재조회 위함
	CItemLongData* m_pQueryData;
	enum {
		codesearch_sel_gwan=0,			//관심
		codesearch_sel_master_stock,	//주식
		codesearch_sel_master_elw,		//elw
		codesearch_sel_master_jisu,		//업종
		codesearch_sel_master_future,	//선옵
		codesearch_sel_master_fojipyo,	//선물지표
		codesearch_sel_master_foreign,	//해외
		codesearch_sel_master_commodity,//상품선물
		codesearch_sel_oneclick,		//빠른종목검색
		codesearch_sel_imsi,			//사용자종목검색
		codesearch_sel_account,			
		
		codesearch_stock_sel_master_stock=0,	//주식
		codesearch_stock_sel_master_elw,	//elw
		codesearch_stock_sel_oneclick,		//빠른종목검색
		codesearch_stock_sel_imsi,			//사용자종목검색
		codesearch_stock_sel_account,			

		codesearch_elw_sel_master_elw = 0,	//elw
		codesearch_elw_sel_oneclick,		//빠른종목검색
		codesearch_elw_sel_imsi,			//사용자종목검색

		codesearch_upjong_sel_master_jisu=0,		//업종

		codesearch_future_sel_master_future=0,	//선옵

		codesearch_foreign_sel_master=0,	//해외

		codesearch_commodity_sel_master=0,	//상품선물
	};
        ST_GwanSimGroup m_GwanSimGroup;//tour2k
public:	
	void Init_Button();

public:
	HTREEITEM m_hSelTreeItem;
	ITreeListManager2*	m_pTreeListMng;
	void fnMakeTree(CTreeCtrl* pTree, LPCSTR szFile);

	class CTLMCallback : public ITLMCallback
	{
		public:
		STDMETHOD_(void, ReceiveData)(int nRtnState, char aDataType, long dwReserved, int nLength, void* pData) ;
		STDMETHOD_(HRESULT, ReceiveRealData)(WORD wFID, LPCSTR szMainKey, void* pRealData);

		int _DoFilterStockCode(LIST_CCodeTypeA *pListTypeA);
	} m_xTLMCallback;
	friend class CTLMCallback;	
	long m_dwTreeKey; 
//	long		m_lTitleCode;
// Implementation

public:
	BOOL m_bAdvise;
	CString m_strCodes;
	BOOL MakeDrdsConnectAdvise(CString szKey);
	BOOL MakeDrdsUnConnectUnAdvise(CString szKey);

	//2006.08.26 by LYH UI 변경{{
	CGfxOutBarCtrl		m_ctrlOutBar;
	//CList <CTreeCtrl*, CTreeCtrl*> m_ListTree;
	void	RemoveAllTree();

	int		m_nHtsMode;

	CTrTreeCtrl*	m_pGwanTree;				// 관심처리를 하는 CTreeCtrl
	CTrTreeCtrl*	m_pUserAccountList;

	CTrTreeCtrl*	CreateTreeCtrl(CString strName, UINT nID);
	long			ChangeTreeFolder(CString strName, CTrTreeCtrl *pTree, long lTreeKey, BOOL bChangeMode=TRUE);
	void			fnFolderChange(int nChartType, int nSel);

	//2006.08.26 by LYH UI 변경}}
//@유진삭제
//	BOOL m_bGridExtend;
//@유진삭제
	void ApplySelectedCodes();
	//2007.01.12 by LYH 트리중 조건검색 선택
	void SelectFindData();
	// 이미지
	CImageList		m_ImageListIcon;
	//20061213 정유진 수정
	//LeftBar 트리뷰헤더 이미지 수정
//@유진삭제
	CImageList		m_ImageOutbarIcon;
	//20061213 수정 end
	
	BOOL IsPlayState();
	void RunAutoTimer();

	CTrTreeCtrl* m_pSelectedTree;
	void ClickDefault();
	
	void ShowAccountOperator(BOOL bShow);

	class CMyITrCommSite : public ITrCommSite
	{
	public:
		STDMETHOD(ReceiveData) (long dwKey, LPCSTR szTR, LPCSTR szMsgCode, LPCSTR szMessage, LPVOID	aTRData, long dwTRDateLen)
		{
			METHOD_PROLOGUE(CTabCodeSearch, MyITrCommSite)
				return 1L;
		}

		STDMETHOD(ReceiveRealData) (WORD wFID, LPCSTR szMainKey, void* pRealData)
		{
			METHOD_PROLOGUE(CTabCodeSearch, MyITrCommSite)
				return pThis->ReceiveRealData(wFID, szMainKey, pRealData);
		}

		STDMETHOD(StatusChange) (int nState)
		{
			MYMETHOD_PROLOGUE(CTabCodeSearch, MyITrCommSite)
				return S_OK;	
		}

		STDMETHOD(ReceiveMessage) (long dwKey, LPCSTR szTR, LPCSTR szMsgCode, LPCSTR szMessage)
		{
			MYMETHOD_PROLOGUE(CTabCodeSearch, MyITrCommSite)
			return S_OK;
		}
	} m_xMyITrCommSite;
	friend class CMyITrCommSite;
	HRESULT ReceiveRealData(WORD wFID, LPCSTR szMainKey, void* pRealData)
	{
		return m_xTLMCallback.ReceiveRealData(wFID, szMainKey, pRealData);
	};

	//이름으로 Outbar폴더 선택되어 열리도록.
	int SetSelFolder(LPCSTR szFolderName);
//
public:
	// Generated message map functions
	//{{AFX_MSG(CTabCodeSearch)
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnDestroy();
	afx_msg void OnSelchangedTreesearch(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDblclkListcode(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnBtportfolio();
	afx_msg void OnBtregfavor();
	afx_msg void OnBtsearch();
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnBegindragListcode(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnBeginRdragListcode(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnClickListcode(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnBtall();
	afx_msg void OnRclickListcode(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnBtnext();
	afx_msg void OnBtprev();
	afx_msg void OnBtresetocx();
//	afx_msg void OnBtnRetry();
	afx_msg void OnBtnAddapply();
	afx_msg void OnChkAvgprice();
	afx_msg void OnChkConcprice();
	afx_msg void OnItemchangedListcode(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnItemchangingListcode(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	LRESULT OnIntRequest( UINT wParam, LONG lParam );
	LRESULT OnResetAll( UINT wParam, LONG lParam );
	HRESULT OnAddAccountCodeItem(WPARAM wParam, LPARAM lParam);
	HRESULT OnCheckAvgPrice(WPARAM wParam, LPARAM lParam);
	HRESULT OnMakeAccountConcData(WPARAM wParam, LPARAM lParam);
	HRESULT OnResetConcData(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
	afx_msg void OnBnClickedBtplay();	
	afx_msg void OnStnClickedSplittery1();
	afx_msg void OnLvnItemchangedListcode(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedChkExtend();
	afx_msg long OnOutBarNotify(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnRmsgTabCodeSearch(WPARAM wp, LPARAM lp);

//@Solomon관심-->
	afx_msg long OnWinixNotifyLoad(WPARAM wParam, LPARAM lParam);
	afx_msg long OnWinixNotifySave(WPARAM wParam, LPARAM lParam);
	afx_msg long OnWinixNotifyGroup(WPARAM wParam, LPARAM lParam);
	afx_msg long OnWinixNotifyGroupList(WPARAM wParam, LPARAM lParam);
	afx_msg long OnWinixNotifySaveCode(WPARAM wParam, LPARAM lParam);

	LRESULT OnRmsgWMSize(WPARAM wp, LPARAM lp);

	List_CItemLongData m_ItemLongDataList;
	void DoDeleteTreeItem(CTreeCtrl* pTreeCtrl, CString& szRemain);
	void DoClearItemLongDataList();
	IWinixGwanManager*	m_pGwanMng;	
//@Solomon관심<--
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TABCODESEARCH_H__7AC8ECCA_35CB_48BC_9B2D_B97956B6470C__INCLUDED_)
