#if !defined(AFX_LEFTBARDLG_H__B80386F0_5717_4A42_B2BD_CE3773D3AB9B__INCLUDED_)
#define AFX_LEFTBARDLG_H__B80386F0_5717_4A42_B2BD_CE3773D3AB9B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// LeftBarDlg.h : header file
//
#include "RscDlg.h"

#include "resource.h"
/////////////////////////////////////////////////////////////////////////////
// CLeftBarDlg dialog
#include "./control/XListCtrl.h"

//#include "any2code.h"

#define	MAXPACKET			10
#define	ID_LIB_EZFINDER		3000
//2005. 06. 08 by sy, nam
//class CWndMapView;
#include "XTabCtrl.h"
class CStdDialog;
class CTabDlg;
class CChartMng;
class CRegFavoriteDlg;
class CFavoriteThingsMng;
class CLeftBarDlg : public CRscDlg
{
// Construction
public:
	CLeftBarDlg(CWnd* pParent = NULL);   // standard constructor

	//2005. 07. 23 add by sy,nam --------------------------------------------
	//조건 검색 Lib 관련	
	//CEzFinderLib*		m_pEzFinderLib;	
	//-----------------------------------------------------------------------


	//2005.05.16 add by Nam ----------------------------------------------------
	//차트 구분 주식, 선물옵션, 업종 차트 
//@유진삭제
//	int		m_nChartGubun;
//	enum	{STOCK_CHART, UPJONG_CHART, FUTOPT_CHART, ELW_CHART, FOREIGN_CHART, COMMODITY_CHART};	// 상품선물옵션차트 - ojtaso (20071001)
//	void	SetChartGubun(int nGubun) {
//		m_nChartGubun = nGubun; m_tabctrl.m_nChartType = nGubun;} 
//@유진삭제
//@유진추가
	int		m_nMarketType;
	void	SetMarketType(int nMarketType) { m_nMarketType = nMarketType; m_tabctrl.m_nMarketType = nMarketType; }
	
	BOOL	m_bIsRunningSB;
	int		m_nSideBarIncrease;
	void	StartSideBar();
	void	EndSideBar();
//@유진추가
	//--------------------------------------------------------------------------

	//-00----------------------------------------
	HWND	m_hMainFrame;
	void	SetMainFrame(HWND* pMainFrame);
	void	GetIndicatorNameListIntoComboBox(CComboBox& rComboBox,LPCTSTR p_strGroup);
	//===========================================

	UINT m_nID;
// Dialog Data
	//{{AFX_DATA(CLeftBarDlg)
	enum { IDD = IDD_LEFTBAR };	
	CXTabCtrl	m_tabctrl;
	//}}AFX_DATA
	CString m_strRootPath;
	CString m_szImageDir;
	//CString m_strConfig;
	CBrush	m_brBkColor;
	COLORREF m_clrBk;

	void ChangeSkinColor();


	void	OnCodeAdvise	(CString strCode	);
	void	OnCodeUnAdvise	(CString strCode	);
	BOOL	IsAdvisingCodeOf(LPCTSTR lpCode		);
	void	SetRealPriceData(void *pData);

	void	GetIndicatorNameListIntoListCtrl(CXListCtrl& rxlistCtrl,LPCTSTR p_strGroup);
	void	GetIndicatorNameListIntoTreeCtrl(CTreeCtrl& rxtreeCtrl);

	HRESULT ReceiveData(HWND hTrComm, LPCSTR szTR, LPCSTR szMsgCode, LPCSTR szMessage, LPVOID aTRData, long dwTRDateLen);
	void	SetStdDialog(CStdDialog* pStdDialog) { m_pStdDialog = pStdDialog; }
	void	SetpChartMng(CChartMng* pChartMng) {m_pChartMng = pChartMng;}
	
	//void	SetpDBMgr(CDrMLib_CDBMgr* pDBMgr) { m_pDBMgr = pDBMgr;	} //2005. 05. 12
	//CDrMLib_CDBMgr*	GetpDBMgr()				  { return m_pDBMgr;	} //2005. 07. 23
//	void	ShowTab(int nIndex);

	void	SetPacketFromOutSide(CString strPacketFromOutSide);
	CString GetPacketFromOutSide();
	BOOL	GetCheckIgnoreShareData() { return m_bIgnoreShareData;}
	void	SetCheckIgnoreShareData(BOOL bIgnore) { m_bIgnoreShareData = bIgnore;}

	BOOL	EnableWindow(BOOL bEnable = TRUE);
	BOOL	GetEnableWindow();
	void	SetpFavoriteThingsMng(CFavoriteThingsMng* pFavoriteMng) { m_pFavoriteMng = pFavoriteMng;}
	void	SetConfig(BOOL bLoad = TRUE);

	CString GetUserPath();

	void	AddAccountCodeItem(LPVOID aTRData, long dwTRDateLen);
	void	MakeAccountConcData(LPVOID aTRData, long dwTRDateLen);
	void	ResetConcData();

private:
	CFavoriteThingsMng* m_pFavoriteMng;
	CStringList			m_Codes;										// DRDS 코드 저장 변수
	//CAny2Code*			m_pctrlCode;
	//CDrMLib_CDBMgr*				m_pDBMgr;
	CStdDialog*			m_pStdDialog;
	CChartMng*			m_pChartMng;
//2005. 06. 08 by sy, nam===============================================================
//	CWndMapView*		m_pWndMapView;
	long				m_lJMaster;
	long				m_lKMaster;
//## HanWha
	
//## HanWha
	CString m_strPacketFromOutSide;
	BOOL m_bEnable;
	BOOL m_bIgnoreShareData;


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CLeftBarDlg)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CLeftBarDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnDestroy();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnBthidechart();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnPaint();
	//}}AFX_MSG
	HRESULT OnCheckAvgPrice(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LEFTBARDLG_H__B80386F0_5717_4A42_B2BD_CE3773D3AB9B__INCLUDED_)
