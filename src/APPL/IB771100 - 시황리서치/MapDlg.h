#pragma once

#include "resource.h"
#include "BkUtil.h"
#include "SComboBox1.h"
#include "xSkinButton.h"
#include "SListCtrl.h"
#include "AxBrower.h"
#include "../../control/fx_misc/fxCodeCtrl.h"
#include "../../control/fx_misc/fxImgButton.h"
#include "../../control/fx_grid/fxgrid.h"
#include "fxTab.h"
#include "STimeCtrl.h"
#include "MapWnd.h"
#include "TreeOptionsCtrl.h"
#include "shareMsg.h"
#include "WxEdit.h"
#include "ColorButton.h"
#include "ColoredListCtrl.h"

#include "../../h/axisfire.h"
#include "../../h/jmcode.h"

// Color Index Define
#define CLR_FORM_BACK		64
#define CLR_PANNEL_BACK		66
#define CLR_PANNEL_LINE		67
#define	CLR_STATIC_BACK		78
#define	CLR_STATIC_FORE		69
#define	CLR_STATIC_LINE		65
#define CLR_TAB_FORE		70
#define CLR_TAB_SELFORE		71
#define CLR_GRID_TEXT		69
#define CLR_GRID_BACK		68
#define CLR_GRID_LINE		75
#define CLR_GRID_HBACK		74
#define CLR_GRID_HTEXT		76
#define CLR_GRID_SBACK		78
#define CLR_GRID_STEXT		69
#define CLR_DATA_UP		94
#define CLR_DATA_DOWN		95
#define CLR_SYMBOL_BACK		160
#define CLR_SYMBOL_FORE		69
#define CLR_SYMBOL_LINE		89
#define CLR_GUIDE_BACK		66
#define CLR_GUIDE_FORE		69
#define CLR_GUIDE_LINE		75

#define WM_DO_INNER_JOB			WM_USER+1982
#define DO_NAVIGATE				7653

#define CTRLMOVEX      0
#define CTRLMOVEY      0
#define CTRLMOVE_X  -342
#define CTRLMOVE_Y  -325




class CMarketDlg;

struct sorting_cod {
	CString cod;
	CString name;
};

class CMapDlg : public CDialog , public CBkUtil
{
public:
	CMapDlg(CWnd* pParent = NULL);
	virtual ~CMapDlg();

	//{{AFX_VIRTUAL(CMapDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL
	
protected:
	//{{AFX_MSG(CMapDlg)
	virtual BOOL OnInitDialog();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnSizing( UINT nSide, LPRECT lpRect );
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnPaint();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg void OnDestroy();
	afx_msg void OnClose();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg LONG OnComboEnter(WPARAM w,LPARAM l);
	afx_msg void OnSelendokComboSearch();
	afx_msg LONG OnListRequestNext(WPARAM wParam, LPARAM lParam);
	afx_msg void OnRclickListNews(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnClickListNews(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnClickFilterListNews(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg LONG OnWebBrowSize(WPARAM w,LPARAM l);
	afx_msg LONG OnWebBrowSizing(WPARAM w,LPARAM l);
	afx_msg void OnCloseupDatetimepicker(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnRadioTot();
	afx_msg void OnRadioSel();
	afx_msg void OnButtonJongmok();
	afx_msg void OnButtonViewset();
	afx_msg void OnButtonSell();
	afx_msg void OnButtonPrint();
	afx_msg void OnButtonSearchSet();
	afx_msg void OnButtonQuery();
	afx_msg void OnButtonNextpage();
	afx_msg void OnSetNewsType();
	afx_msg void OnToolbarRePosition();
	afx_msg void OnMove(int x, int y);
	afx_msg void OnButtonPreview();
	afx_msg void OnNcDestroy();
	afx_msg void OnSelchangeFontsize();
	afx_msg void OnStaticTitle();
	afx_msg void OnButtonTitle();
	afx_msg void OnRadioTotal();
	afx_msg void OnRadioJongmok();
	afx_msg void OnRadioTotal2();
	afx_msg void OnRadioGongsi();
	afx_msg void OnRadioSihwang();
	afx_msg void OnOnlyJongmok();
	//}}AFX_MSG
	afx_msg void OnSelchangeOrdTab(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg LONG OnManage(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnSend(WPARAM wParam, LPARAM lParam);
	afx_msg LONG OnMessage(WPARAM wParam, LPARAM lParam);
	afx_msg LONG OnWxEditMessage(WPARAM wParam, LPARAM lParam);
	afx_msg LONG OnDoInnerJob(WPARAM wParam, LPARAM lParam);
	
	
	DECLARE_MESSAGE_MAP()

public:
	//{{AFX_DATA(CMapDlg)
	enum { IDD = IDD_MAIN };
	CComboBox	m_pFontSize;
	CStatic	m_pStatic;
	CStatic	m_pStatic2;
	CxSkinButton	m_btnPreview;
	CWxEdit			m_clSearchEdit;
	CxSkinButton	m_btToolPos;
	CButton			m_btTrg;
	CComboBox		m_cblTrgType;
	CTreeOptionsCtrl m_TreeWnd;
	CxSkinButton	m_btnRight;
	CxSkinButton	m_btnLeft;
	CSComboBox		m_comboSearch;
	CxSkinButton	m_btnJM;
	CxSkinButton	m_btnSell;
	CxSkinButton	m_btnSSet;
	CxSkinButton	m_btnQuery;
	CxSkinButton	m_btnPrint;
	CxSkinButton	m_btnNext;
	CxSkinButton	m_btnViewSet;
	CButton			m_radioTotal;
	CButton			m_radioJongmok;
	CButton			m_radioTotal2;
	CButton			m_radioGongsi;
	CButton			m_radioSihwang;
	CStatic			m_staticRadio;
	CStatic			m_staticRadio2;
	CSTimeCtrl		m_TimeCtrl;
	CSListCtrl		m_List;
	CButton			m_bChkAutoTrigger;
	CButton			m_bChkOnlyjongmok;	
	CSListCtrl m_FilterResultListCtrl;

	//}}AFX_DATA

public:
	void		RequestContent(int nIdx);
	void		RequestTitleList( BOOL bKeyword = FALSE, BOOL bNext = FALSE );
	void		RequestCountList( BOOL bNext = FALSE );

	CString		GetAxVariant(int cmd, CString data=_T(""));
	CString		GetAxCode(int cmd,LPARAM lParam);
	CFont*		GetAxFont(int size ,bool bBold,CString str);
	COLORREF	GetAxColor(int iColor);
	CPen*		GetAxPen(COLORREF clr, int width, int style);
	HBITMAP		getBitmap(CString path);

	void		SaveKeyConfig();
private:

	void		initListCtrl();
	void		initFilterListCtrl();
	bool		initControl();
	void		initSymbol();
	void		changeColors();
	void		InitWnd();
	
	bool		loadSearchWordList();			// 검색단어
	bool		saveSearchWordList();
	
	void		sendOOP(int key, char* datb, int datl);
	void		sendTR(CString strCode, void *m, int nLen,int key = 0, bool bOOP =true );
	void		sendTRA(CString trCode, char* datB, int datL, char key);


	void		parsingContent(char *pBytes, int nBytes);
	void		parsingTitleList(int nKey, void* buffer, int len);
	void		parsingSymbol(char* datb, int datl);
	void		parsingAlert(CString dat);
	void		parsingAlertx(DWORD* data);
	void		parsingNewsCount( char* inBuf, int len);

	CString		makeTitle(CString title);
	void		emptyFile();

	void		selectTab(int nIndex);

	bool		isMediaChecked(const news_info& news, bool bAlert = false);

	bool		isMediaChecked2(const news_info& news);      //전체 or 공시 or 시황
	bool		isNewsViewChecked(const news_info& news, bool bAlert = false);	//전체 or 종목
	void		gridAddRow(news_info &news, bool bPushBack = true, bool bCountNews = false);
	void		FiltergridAddRow(news_info &news, bool bPushBack = true, bool bCountNews = false);
	void		openMap(int type, CString data);
	void		sendSymbol(CString code);
	void		parseNews(CString dat, CMapStringToString& ary);
	void		drawMove(CRect rect);
	CRect		getClipRect();
	void		InitTree();					// Initialize Tree(문자열 초기화, Loading 트리 체크, Insert Item)
	int			SetMediaFilter(HTREEITEM hItem);		// Set filtering condition
	CString		GetTreeCode(HTREEITEM hItem, CString str, int & nIndex);	// 현재 선택된 트리 코드
	void		LoadScreenView();
	void		SaveScreenView();

	BOOL		IsAddAlert( void );	
	BOOL		IsNewsCountTR( sendtr_type type )
				{
					if( type >= type_news_count_day && type <= type_news_count_month )
						return true;
					else
						return false;
				}
	CString		GetGubnName( CString sGubn );

	void		CheckTab(int nIdx=0);
	void		ResizeControl();
	void		SetFrameSize();
	void		ShowTreeMode();
	void		ScrapNews(int irow);
	void		LoadScrapNews();
	void		CheckUserPath();
	void		SaveScrapNews();
	void		SaveScrapNews(AList scraplist);
	void		SetScrapNews();
	void		CheckCodeTrigger();
	void		SendMode(BOOL bEnable);

	CString		GetNewGubnText(int igubn);
	void		ResizeNewsTitleField(int iw) ;
	void		SetTitleListHeader(int iw);

	void		DebugLog(CString strText) ;
	void		WriteLog(CString slog);

	void		SetCountNewsCtrl( BOOL bShow );
	
	COLORREF	GetIndexColor(int index);
	int			MoveCtrl(UINT nCtrlID, int nCx, int nCy, int nCtrlWidthGap);
	int			MoveCtrl(CWnd* pWnd, int nCx, int nCy, int nCtrlWidthGap);
	
	// 맵 Round 처리 함
	CString     m_strPal;
	CString		GetPalette();
	void		DrawRoundBitmap(CDC *pDC, CRect rc, int nKind);
	void		DrawBitmap(CDC *pDC, CRect rc, CString strImage);
	CString		Variant(int comm, CString data);
	

	std::unique_ptr<CMarketDlg> m_pFilterWnd;
	
	int			m_iTabIndex;			// 탭 인덱스
	CString		m_strHeadertitle;	
	CString		m_strHeaderKey;			// 헤더 누를시, 팜업창에 사용할 뉴스키값
	CString		m_strHeadercode;		// 헤더 누를시, 팜업창에 사용할 종목코드
	
	int			m_iTabCount;	
	
	CButton			m_chkTrigger;
	CxSkinButton	m_btnTitle;
public:	
	bool Sending;
	CWnd*			m_pParent;
	CString			m_szSysGubn;				// D:대투,H:하나,I:IBK
	BOOL			m_bRetail;					// true:하나R, false:하나IB
	COLORREF		m_clrTitleText[MAX_NEWS];	// 뉴스 종류에 따라 색상 다르게 설정 가능(0:머니투데이, 1:인포스탁, 2:한경뉴스, 3:이데일리)
	CStringArray		m_SearchWordArray;		// 검색 데이타
	CAListArray			m_arSpecialKeyList;
	CNewsListArray		m_arNewsList;
	CUIntArray			m_arStepColor;
	BOOL				m_bSearch;
	bool			m_bCheckNewCount;	

	CArray<sorting_cod,sorting_cod>		m_sortCode;
	CStringArray						m_restrictArray;
	CStringArray						m_wordArray;

	BOOL makeSubCod( mod_content* mod, CString& strRet, CStringArray& arrSubCod );
	BOOL findSubCodName(const CString& strCode, CString& strName );
	void convertLink( CString& str, const CStringArray& arrSubCod );
	void LoadCode();
	
	void Resizetab1(CRect rc, CRect rc1);
	void Resizetab2(CRect rc, CRect rc1);
	void Resizetab3(CRect rc, CRect rc1);
	
	void ChangeInitSize();

private:
	HINSTANCE		m_inst;				// cx_symbol
	CString			m_strSearchKey;

	CCriticalSection	m_section;
	CAListArray			m_arScrapList;
	CMap <CString, LPCTSTR, int, int>	m_arScrapMap;
	BOOL				m_bSelectScrap, m_bTotList, m_bCodeTrigger;
	int					m_iSelectGubn, m_iSelectSubGubn;
	BOOL				m_bTwoToolBar;

	news_count_page_header		m_sPageHeader;

	CBitmap				m_bmpBorder;

	CStringArray		m_arTabNameList, m_arMapList, m_arMapWidth, m_arMapHeight;

	CWnd*			m_symbol;
	std::unique_ptr<CfxCodeCtrl>	m_code;
	CfxTab*			m_pTab;
	CAxBrower*		m_pBrowser;
	CButton			*m_pRadioTot, *m_pRadioSel;

	CBitmap			m_bmpSave;

	class	CMapWnd*	m_pMapWnd1;
	class	CMapWnd*	m_pMapWnd2;

	COLORREF		m_clrLineColor[2];

	CString			m_szRootDir;
	CString			m_szUser;
	CString			m_szFileName;
	CFont			*m_pFont, *m_pBFont;
	COLORREF		m_clrBorder, m_clrRoundColor;
	COLORREF		m_clrText;
	
	GRID_O			m_grido;				// next save field
	char			m_szSave[80];			// next save field
	
	
	bool			m_bSelect[12];			// check조건 [0:시황, 1:공시, 2:머니투데이, 3:인포스탁, 4:한경, 5:이데일리]
	char			m_szCurKey[23];			// key save	

	CString			m_szDate;				// 날짜비교
	CString			m_szCode;				// 종목코드	
	bool			m_bTotSearch;			// radio 전체:종목
	bool			m_bTriggerTR;			// trigger로 들어온 데이타 인 경우 focus처리 안함
	bool			m_bTrigger;				// trigger롤 들어온 데이타 조회
	
	int				m_bx, m_by;					// button image size

	char			m_cDetailSelect[MAX_NEWS][MAX_NEWS_SUB];	

	BOOL			m_bCategory;

	CMapStringToString m_selHistory;
	HTREEITEM		m_GonsiItem;
	HTREEITEM		m_MediaItem;

	bool	m_bFlag2Send;		//두번 조회했는지 여부
	
	bool	m_bFirstSend;	

	typedef struct _jobObj {
		CString msgBuf;
		struct _alertR * alert{};
		int key{};
		int msg{};
		int len{};
	} JOBOBJ;

	void WorkObject( void );
	void DoObject( _jobObj* p );
	void GetObject( WPARAM wParam, LPARAM lParam );
	void GetNavigate( CString str );

	CList<_jobObj,_jobObj> m_jobList;
	
	//박영태 : 임시저장용 트리 선택 아이템
	HTREEITEM	select_hItem;
	int			select_tab;				//서브탭 선택

	void GetGugnAndSubgMask( char* szGubn, char* szSubg );
	void GetCateGugnAndSubgMask( char* szGubn, char* szSubg );
	void GetGugnAndSubgFromTREEITEM( HTREEITEM hItem, int& iGubn, int& iSubg );
	BOOL GetCateGugnAndSubgFromTREEITEM( HTREEITEM hItem, int iGubn, int iSubg );
	void GetSelectHistory();
	void SaveNodeSelect();
	int SplitStringMAP(CString strData, CString strToken, CMapStringToString &arMapList);
	BOOL GetCheckGongsi(int iSel);
	BOOL GetMediaCheck(CString strName);
	
	void	HideControl();
	
	void VisibleFirstTab(int bShow);
	void VisibleSecondTab(int bShow);
	void parseTrigger(CString sData);
	void setOnlyJongmok(bool isCheck);
	

	class	CMapWnd*	m_pThirdMap;
	// 이인호 : fixedframe을 위하여
	int			m_Ypos;
	int			m_YMovepos;
	CRect		m_rcBorder;
	int			m_nHeightStd;	
	int			m_nHeight;
	int			m_nWidth;
	
	// 이장희 : Interface 변경
	int					m_nTreeWidth;
	CRect				m_rcBorderTree;
	CfxImgButton		m_btOption;
	int					m_nOptHeight;
	BOOL				m_bOptCheck;
	std::unique_ptr<class COptionWnd> m_pOptionWnd;

	BOOL			m_bAutoQuery;			// 본문 자동 조회
	BOOL			m_bJmyd;				// 종목연동 check
	CString			m_sFile;				// Ini file Path

	BOOL			m_bDaetoo;	
	CString			m_sCurrPos;			// 현재 트리 위치
	bool			m_bNextPushed;
	int				m_nView;
	int				m_tabIdx;
	bool			m_bCapture;

	int				m_iOldListWidth;

	COLORREF		m_listhdrcolor, m_listlinecolor, m_txtcolor;

	int				m_iWriteLog;
	sendtr_type		m_nTrkey;

	HBRUSH			m_hRoundColor;
	HBRUSH			m_hStaticClr;
	CBrush			m_br;
	int				m_CATESIZEGUBN;
};

class CCommandThread : public CWinThread {
public:
	CCommandThread(CMapDlg* p) { m_pDoInstance = p; }
	~CCommandThread() {}

	virtual int Run();

private:
	CMapDlg*	m_pDoInstance;

//	CSyncObject m_Lock;
};


