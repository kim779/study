#if !defined(AFX_DLGSTCONFIG_H__SEUNGLYONGPARK_2546_ABCD_EF48_3453DC2AD2455__INCLUDED_)
#define AFX_DLGSTCONFIG_H__SEUNGLYONGPARK_2546_ABCD_EF48_3453DC2AD2455__INCLUDED_

#pragma once

#include "Resource.h"
#include "RscDlg.h"
#include "Control/XListCtrl.h"

#include "../../inc/ExCommonHeader.h"
#include "../../inc/ExDrawButton.h"
#include "../../inc/ExEdit.h"

// CDlgSTConfig 대화 상자입니다.

class CDlgSTConfig : public CRscDlg
{
public:
	CDlgSTConfig(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CDlgSTConfig();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_DLG_STCONFIG };
	//CEdit		m_ctlEditSTTitle;
	CExDrawButton m_ctlSaveConfig;
	CExEdit		m_ctlEditSTName;
	CXListCtrl	m_ctlListSTVariables;

public:
	COLORREF m_crBk;
	CBrush*	m_pBrBkColor;
	CWnd*	m_pDlgSTOption;

public:
	void SetColor(COLORREF& rCrBk) { m_crBk = rCrBk;}
	void SetBrush(CBrush*	pBrBkColor) { m_pBrBkColor = pBrBkColor;}
	void ReloadInfo();

protected:
	void LoadInfo();
	BOOL SaveInfo();
	void InitColumn();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
protected:
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
public:
	afx_msg void OnBnClickedButtonSaveconfig();
};

#endif	// AFX_DLGSTCONFIG_H__SEUNGLYONGPARK_2546_ABCD_EF48_3453DC2AD2455__INCLUDED_