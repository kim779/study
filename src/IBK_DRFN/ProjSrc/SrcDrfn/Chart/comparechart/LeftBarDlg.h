#if !defined(AFX_LEFTBARDLG_H__B80386F0_5717_4A42_B2BD_CE3773D3AB9B__INCLUDED_)
#define AFX_LEFTBARDLG_H__B80386F0_5717_4A42_B2BD_CE3773D3AB9B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// LeftBarDlg.h : header file
//
#include "./control/RscDlg.h"

#include "resource.h"
/////////////////////////////////////////////////////////////////////////////
// CLeftBarDlg dialog
#include "./control/XListCtrl.h"
#include "UsrAddListDlg.h"

//#include "any2code.h"

//#define	MAXPACKET			4
#define	MAXPACKET			7
/*=============== 2005. 06. 09 sy, nam ============================
//class CWndMapView;
/*=============== 2005. 06. 09 sy, nam ============================*/
#include "XTabCtrl.h"
class CTabDlg;
class CChartMng;
class CRegFavoriteDlg;
class CFavoriteThingsMng;
//class CDrMLib_CDBMgr;
class CLeftBarDlg : public CRscDlg
{
// Construction
public:
	CLeftBarDlg(CWnd* pParent = NULL);   // standard constructor

	//2005. 06. 11 add by sy, nam ----------------------------------------------
	CUsrAddListDlg	m_UsrAddListDlg;
	//--------------------------------------------------------------------------

	//2005.05.16 add by Nam ----------------------------------------------------
	//차트 구분 주식, 선물옵션, 업종 차트 
//	int		m_nChartGubun;
//	void	SetChartGubun(int nGubun) {m_nChartGubun = nGubun;} 
	//--------------------------------------------------------------------------

	// 스킨
	virtual void ChangeSkinColor(COLORREF clrBackground);

	//-00----------------------------------------
	HWND	m_hMainFrame;
	void	SetMainFrame(HWND* pMainFrame);
	void	GetIndicatorNameListIntoComboBox(CComboBox& rComboBox,LPCTSTR p_strGroup);
	//===========================================

	CString GetCurItemName();

	UINT m_nID;
// Dialog Data
	//{{AFX_DATA(CLeftBarDlg)
	enum { IDD = IDD_LEFTBAR };
	CXTabCtrl	m_tabctrl;
	//}}AFX_DATA
	CString m_strRootPath;
	CString m_szImageDir;
	CString m_strConfig;

	BOOL	IsAdvisingCodeOf(LPCTSTR lpCode		);
	void	SetRealPriceData(char *pData, int nSize);

	long	GetCodeMasterArray(long lGubun, CStringArray& arCode, CStringArray& arName);
	void	GetIndicatorNameListIntoListCtrl(CXListCtrl& rxlistCtrl,LPCTSTR p_strGroup);
	void	GetIndicatorNameListIntoTreeCtrl(CTreeCtrl& rxtreeCtrl);

	HRESULT ReceiveData(HWND hTrComm, LPCSTR szTR, LPCSTR szMsgCode, LPCSTR szMessage, LPVOID aTRData, long dwTRDateLen);
	void	SetpChartMng(CChartMng* pChartMng) {m_pChartMng = pChartMng;}
	
//	void	SetpDBMgr(CDrMLib_CDBMgr* pDBMgr) { m_pDBMgr = pDBMgr;	} //2005. 05. 12
//	CDrMLib_CDBMgr*	GetpDBMgr()				  { return m_pDBMgr;	} //2005. 07. 27
//	void	ShowTab(int nIndex);

	void	SetPacketFromOutSide(CString strPacketFromOutSide);
	CString GetPacketFromOutSide();
	BOOL	GetCheckIgnoreShareData() { return m_bIgnoreShareData;}
	void	SetCheckIgnoreShareData(BOOL bIgnore) { m_bIgnoreShareData = bIgnore;}

	BOOL	EnableWindow(BOOL bEnable = TRUE);
	BOOL	GetEnableWindow();
//	void	SetpFavoriteThingsMng(CFavoriteThingsMng* pFavoriteMng) { m_pFavoriteMng = pFavoriteMng;}
	void	SetConfig(BOOL bLoad = TRUE);

private:
//	CFavoriteThingsMng* m_pFavoriteMng;
	CStringList			m_Codes;										// DRDS 코드 저장 변수
	//CAny2Code*			m_pctrlCode;
//	CDrMLib_CDBMgr*				m_pDBMgr;
	CChartMng*			m_pChartMng;
	/*=============== 2005. 06. 09 sy, nam ============================
	CWndMapView*		m_pWndMapView;
	/*=============== 2005. 06. 09 sy, nam ============================*/
	long				m_lJMaster;
	long				m_lKMaster;
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
	afx_msg void OnSelchangeTab(NMHDR* pNMHDR, LRESULT* pResult);	
	afx_msg void OnPaint();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LEFTBARDLG_H__B80386F0_5717_4A42_B2BD_CE3773D3AB9B__INCLUDED_)
