#pragma once


// CLogicHelp 대화 상자입니다.
#include "resource.h"
#include "afxwin.h"
#include "ConditionMakeWnd.h"

class CLogicHelp : public CDialog
{
	DECLARE_DYNAMIC(CLogicHelp)

public:
	CLogicHelp(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CLogicHelp();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_LOGICHELP };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.
	
	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnLbnSelchangeList1();
	afx_msg void OnEnChangeEdit1();
	afx_msg void OnPaint();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);

private:
	CBrush			m_brushStatic;
	COLORREF		m_clrMainSkin;

	CfxImgButton	m_btnOk;

	CWnd*			m_pWndMainFrm;
	/*CListBox		m_ctrlList;*/
	/*CEdit			m_ctrlHelpText;*/
	/*CRichEditCtrl	m_ctrlHelpText;*/
	CStringArray	m_arrHelpText;

	CFont m_fontDefault, m_fontBold;

	CStatic			m_stcBkg;
	
	CStatic			m_stcTitle1;
	CStatic			m_stcTitle2;
	CStatic			m_stcTitle3;
	CStatic			m_stcTitle4;
	CStatic			m_stcTitle5;

	CStatic			m_stcInfo1_1;
	CStatic			m_stcInfo1_2;
	CStatic			m_stcInfo2_1;
	CStatic			m_stcInfo3_1;
	CStatic			m_stcInfo4_1;
	CStatic			m_stcInfo4_2;
	CStatic			m_stcInfo5_1;

private:
	void ChangeHelpText(int nSel);
	void OnBtnOk();
	void OnSize();

public:
	virtual BOOL OnInitDialog();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
};
