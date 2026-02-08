#if !defined(AFX_USRADDLISTDLG_H__C8524209_D3E9_4D14_BA5B_7FAF30ACA427__INCLUDED_)
#define AFX_USRADDLISTDLG_H__C8524209_D3E9_4D14_BA5B_7FAF30ACA427__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// UsrAddListDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CUsrAddListDlg dialog
#include "./control/RscDlg.h"
#include "./control/NsyButtonListCtrl.h"
#include "./define/ChartMsgDef.h"

//이미지 리스트를 쓸 경우 ======================
#include "../chart_common/ILButton.h"
//==============================================

#define MAX_ITEM_CNT		10    //최대 10개만 추가가 가능하다.
class CStdDialog;
class CUsrAddListDlg : public CRscDlg
{
// Construction
public:
	// 스킨
	virtual void ChangeSkinColor(COLORREF clrBackground);
	COLORREF m_clrBkgrnd;

	// 주어진 종목을 리스트에서 삭제한다.
	void OnBtnDelItem(LPCSTR szDelCode);


	//CImageList* m_pIL_W20H13;
	CImageList* m_pIL_W60H20;


	void SetRootPath(CString strPath);
	CString m_strMainPath;


	void Re_Request_TRData(); 
//	// Dialog 색깔 지정 ====================================
//	CBrush	m_brBkColor;
//	COLORREF m_crBk;

	void AddItem();
	CUsrAddListDlg(CWnd* pParent = NULL);   // standard constructor
	CString m_szImageDir	;
	CString m_strOpCode		;
	CString m_strOpCodeName	;

	CString GetCodeName() { return m_strOpCodeName; }
	
	//CWnd	*m_pChartMng;
	//void	SetpChartMng(CWnd* pWnd){m_pChartMng = pWnd;}
// Dialog Data
	//{{AFX_DATA(CUsrAddListDlg)
	enum { IDD = IDD_DLG_USR_ADD };
	CILButton	m_btnInitAll;
	CILButton	m_btnDelItem;
	CILButton	m_btnAddItem;
	CNsyButtonListCtrl	m_listUsrAdd;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CUsrAddListDlg)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual BOOL OnInitDialog();
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CUsrAddListDlg)
	afx_msg void OnBtnAdd();
	afx_msg void OnBtnDel();
	afx_msg void OnBtnInit();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnDestroy();
	afx_msg void OnPaint();
	//}}AFX_MSG
	LONG OnRecvMsg_CheckBoxStates_From_XListCtrl(WPARAM wp, LPARAM lp);	
	LONG OnRecvMsg_DBClick_From_XListCtrl(WPARAM wp, LPARAM lp);
	DECLARE_MESSAGE_MAP()
		
public:
	int GetRequestList(CStringList& szCodeList, CStringList& szCodeNameList);
	void AddELWIndexCode();
	void DelItem(LPCSTR szDelCode);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_USRADDLISTDLG_H__C8524209_D3E9_4D14_BA5B_7FAF30ACA427__INCLUDED_)
