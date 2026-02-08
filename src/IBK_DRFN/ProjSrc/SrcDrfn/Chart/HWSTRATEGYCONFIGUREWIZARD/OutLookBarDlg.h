#pragma once

#include "../Common_ST/STGfxOutBarCtrl.h"
#include "../Common_ST/_STLoadmanager.h"
#include "resource.h"

#include "../../inc/ExCommonHeader.h"
#include "../../inc/ExDrawButton.h"
#include "../../inc/ExStatic.h"
#include "../../inc/ExEdit.h"
#include "../../inc/ExComboBox.h"

// COutLookBarDlg 대화 상자입니다.

class COutLookBarDlg : public CDialog
{
	DECLARE_DYNAMIC(COutLookBarDlg)

public:
	COutLookBarDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~COutLookBarDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_OUTLOOKBARDLG };
	CExDrawButton	m_btnFindST;
	CExDrawButton	m_btnNextST;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

public:
	CSTGfxOutBarCtrl			m_wndOutBar;
//	CImageList				m_ilSTIcon;
//	CImageList				m_ilW151H21;
	CImageList		m_ImageListIcon;
	CImageList		m_ImageOutbarIcon;
	CSTLoadManager*			m_pSTLoadMnger;

	COLORREF		m_crBk;
	CBrush			m_brBkColor;
	
	CStringArray	m_arSTPath;
	CString			m_szSTData_STPath;
	CString			m_szSTUser_STPath;
	CString			m_szCurFolder;
	CString			m_szCurSTType;
	CStringArray	m_arFindIndx;
	int				m_nFindIndx;
	STMNG_ITEMINFO	m_RunSTInfo;			// 실행중인 전략정보

public:
	void	SetSTParent(CWnd* pParent);
	void	LoadSTList();
	void	UpdateSTList(int nState, CString szSTPath);
	CString GetSTPathSelected(CString& szSTType);
	void	Resize();
	CRect	GetRect(int nID);
	BOOL	IsUserFolder(int nFolder);


	DECLARE_MESSAGE_MAP()
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnDestroy();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
protected:
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
public:
	afx_msg void OnBnClickedBtnFindst();
	afx_msg void OnBnClickedBtnNextst();
};
