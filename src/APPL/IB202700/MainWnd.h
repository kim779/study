#pragma once
// MainWnd.h : header file
//

#include <map>
using namespace std;

/////////////////////////////////////////////////////////////////////////////
// CMainWnd window
#include "../../control/fx_misc/fxImgButton.h"
#include <afxmt.h>
#include "../../h/axisfire.h"
#include "AxisExt.h"
#include "GridWnd.h"
#include "GroupWnd.h"

class CMainWnd : public CWnd, public CAxisExt
{
// Construction
public:
	CMainWnd(CWnd* pWnd);

// Attributes
public:

protected:
	CString m_home, m_user, m_name;
	CString m_profile, m_fileIMG;
	std::array<std::shared_ptr<CGroupWnd>, MAX_GRID> m_pGroupWnd;

	int m_nType;

	std::unique_ptr<CComboBox> m_cbGroup;
	CfxImgButton	m_btCONFIG;	// 설정
	CfxImgButton	m_btSAVE;	// 저장
	CfxImgButton	m_btMINUS;	// 마이너스
	CfxImgButton	m_btPLUS;	// 플러스

	int			m_nCntGrid;			//그리드 갯수
	int			m_nCurGrid;			//현재 선택된 그리드
	int			m_nPreCurGrid;		//이전에 선택된 그리드
	CString		m_strGroupName;
	CSize		m_size;
	CRect		m_rcDivide[3];
	CRect		m_rcPanel[3];//for rounding
	BOOL		m_bTrigger;

	CArray<int, int>	m_arRATE;
	CArray<CRect, CRect>	m_arRECT;
	CArray<CRect, CRect>	m_arDIVIDE;
	CArray<CRect, CRect>	m_arUpDIV;
	CArray<CRect, CRect>	m_arDnDIV;
	BOOL			m_bDivide;

	BOOL	m_bContinue;			// 관심종목의 필드가 많을경우
	BOOL	m_bSecond;

	CBrush*			m_pBrush;
	COLORREF		m_clrBack;
	char			m_chUse;
	COLORREF		m_bkColor1;
	COLORREF		m_bkColor2;
	COLORREF		m_fgColor[4];
	
	enum _dblAction		m_dblAction;
	int			m_selAction;
	int			m_line;
	BOOL			m_bCurr;
	int			m_fontsize;
	CString			m_fontname;
	CString			m_linkMap;
	BOOL			m_bName;
	BOOL			m_bAllBold;	//2012.11.28 KSJ 전체 굵게
	int			m_rowH;

	CStringArray		m_pGridArray;
	UINT			m_kind;
	BOOL			m_bRTMReady;	//그리드 조회전에 RTS처리하면 에러남.

	CBitmap		*m_hRndBmp[4];
	CBitmap		m_bgBmp;

	CString		m_strTag;
	BOOL		m_bStartMsg;
	CString		m_strMarketTime;
	CString		m_fileCFG;

	BOOL		m_bFirst{}, m_bDLLDATA{};
	BOOL		m_bLastStat{};

// Operations
public:
	void Init();
	void InitGroup(CRect rect, int nCount =1);

	CString GetGroupName(int nGroupIndex = -1);
	int		GetCurGroup()	{ if(m_cbGroup != nullptr)		return m_cbGroup->GetCurSel(); }
	void	SetCurGroup(int nSel);		//현재 선택된 그룹 그룹넘버
	int		GetSelectGroup() { return m_nCurGrid;}
	void	SetSelectGroup(int nSel);// {m_nCurGrid = nSel; }	//현재 선택된 그룹 화면에 보이는 그룹(추가한 그룹)
	CString GetStrTag(){return m_strTag;}
	CString GetMarketTime() { return m_strMarketTime; };	//KSJ 2012.11.26 장운영시간 
	void	SetMarketTime(char* datB);	//2012.11.26 KSJ 장운영구분 조회

	CString SplitString(CString &strData, CString strToken);

	void loadfield();
	void loadcfg();
	void savecfg();
	int	 _httoi(const TCHAR *value);
	
	int	GetFontSize() { return m_fontsize; }
	BOOL	GetCurrBold() { return m_bCurr; }
	BOOL	GetNameBold() { return m_bName; }
	BOOL	GetAllBold() { return m_bAllBold; }
	CString	GetFontName() { return m_fontname; }
	CString	GetLinkmap() { return m_linkMap; }
	char	 GetBkUse() { return m_chUse; }
	COLORREF GetBkColor1() { return m_bkColor1; }
	COLORREF GetBkColor2() { return m_bkColor2; }
	COLORREF GetFgColor(int nIndex) { return m_fgColor[nIndex]; }
	int	GetSelAction() { return m_selAction; }

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMainWnd)
	protected:
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

// Implementation
public:
	void parsingTrigger(CString datB);
	void DrawDivide();
	void Draw(CDC* pDC);
	void DelGrid();
	void AddGrid();
	void OnChangecbGroup();
	bool initializeCOM();	//XML이용
	void parsingGroupList(char* datB, int datL);//그룹리스트
	void queryGroupList();
	void ResizeOper(int cx, int cy);
	void MakeGridRect(CRect rcFull);
	CString GetFileCfg(){return m_fileCFG;}
	void SendOper(class CSendData* sdata);

	void	RequestMarketTime();	//2012.11.26 KSJ 장운영구분 조회

	LONG OperDLLOUB(CRecvData& rdata);

	void DrawRoundRectangle(CDC *pDC, CRect rc);
	void DrawBitmap(CDC *pDC, int x, int y, int w, int h, HBITMAP hBitmap, COLORREF tCol=0xFF00FF);
	void DrawBitmap(CDC *pDC, CRect *pRC, HBITMAP hBitmap, COLORREF tCol=0xFF00FF);

	virtual ~CMainWnd();
	// Generated message map functions
protected:
	//{{AFX_MSG(CMainWnd)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnPaint();
	afx_msg void OnDestroy();
	//}}AFX_MSG
	afx_msg LONG OnUser(WPARAM wParam, LPARAM lParam);
	afx_msg LONG OnManage(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
};

