#if !defined(AFX_DLGTIMESETUP_H__320E_1464_BY_JUNOK_LEE_98B1A__INCLUDED_)
#define AFX_DLGTIMESETUP_H__320E_1464_BY_JUNOK_LEE_98B1A__INCLUDED_

// #pragma once

#include "./CfgWndCreate.h"		// 설정파일 처리를 위해서 필요.
#include "./Resource.h"
#include "DlgOptimizer.h"
#include "../../inc/ExStatic.h"

extern UINT CDlgTimeSetup_ID1[];
extern UINT CDlgTimeSetup_ID2[];

// CDlgTimeSetup 대화 상자입니다.
struct ST_VALUEOPTIMAZE;
class CDlgTimeSetup : public CEscDialog
{
	DECLARE_DYNAMIC(CDlgTimeSetup)

public:
	CDlgTimeSetup(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CDlgTimeSetup();

	CCFGWndCreate	m_CFGWnd;
	COLORREF	m_crBk;
	CBrush m_brBkColor;

	void	Reset();
	int		LoadCFG(LPCSTR szCFGFile, LPCSTR szSection);
	void	ParseCFG();

	int		GetCount();
	int		GetInputData(CCFGSectionInfo *pItemInfoList);
	void	SetInputData();
// 대화 상자 데이터입니다.
	enum { IDD = IDD_DLG_TIMESETUP };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.
	virtual BOOL OnInitDialog();
	
	DECLARE_MESSAGE_MAP()
public:
	CArray<ST_VALUEOPTIMAZE,ST_VALUEOPTIMAZE> m_ValueOptimizerArray;
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	CStatic m_ES_Condition0;
	CStatic m_ES_Condition1;
	CStatic m_ES_Condition2;
	CStatic m_ES_Condition3;
	CStatic m_ES_Condition4;
	CStatic m_ES_Condition5;
	CStatic m_ES_Start;
	CStatic m_ES_End;
	CStatic m_ES_Inc;
	CStatic m_ES_Name;
	CStatic m_ES_szTitle;

	CArray<CStatic*,CStatic*> m_ES_Array;
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};
#endif //AFX_DLGTIMESETUP_H__320E_1464_BY_JUNOK_LEE_98B1A__INCLUDED_)