#if !defined(AFX_CONCLUSION_H__475A51CA_D065_4BA2_A159_9EC5F53922B9__INCLUDED_)
#define AFX_CONCLUSION_H__475A51CA_D065_4BA2_A159_9EC5F53922B9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Conclusion.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CConclusion dialog
#include "useMaindefine.h"
class CConclusion : public CDialog
{
// Construction
public:
	CConclusion(CWnd* pParent);   // standard constructor
	~CConclusion();
protected:
	CWnd*		m_parent;
	CBrush		m_brush;

#ifdef DF_USE_CPLUS17
	std::unique_ptr<class CfxIGrid> m_grid;
	std::unique_ptr<class CfxGrid> m_pGrid;
#else
	class CfxIGrid*	m_grid;
	class CfxGrid* m_pGrid;
#endif

	
	int		m_x{}, m_y{};
	CRect   m_recOri, m_recDef;
	bool	m_binit;
	CStringArray m_arr;
	CFont m_font;
	void ResizeGrid();
	int FindColData(CString strdata);
	double m_dHeightRatio{}, m_dGridLRadio{}, m_dGridTRadio{}, m_dGridRRadio{}, m_dGridBRadio{};
	double m_dBtn1RatioL{}, m_dBtn1RatioR{}, m_dBtn2RatioL{}, m_dBtn2RatioR{}, m_dBtn3RatioL{}, m_dBtn3RatioR{}, m_dBtn4RatioL{}, m_dBtn4RatioR{};
	double m_dBtn1RatioT{}, m_dBtn1RatioB{}, m_dBtn2RatioT{}, m_dBtn2RatioB{}, m_dBtn3RatioT{}, m_dBtn3RatioB{}, m_dBtn4RatioT{}, m_dBtn4RatioB{};

	CString Parser(CString &srcstr, CString substr);
	void LoadCondition();
// Dialog Data
	//{{AFX_DATA(CConclusion)
	enum { IDD = IDD_CONCLUSIONLIST };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CConclusion)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
public:
	CString parseX(CString &srcstr, CString substr);
	int _httoi(const TCHAR *value);
	DWORD GetAttrByStr(CString sAttr);
	COLORREF GetColor(int index);
	bool	Init();
	void	ChangePalette();
	BOOL	AddConclusion(CString dat, int max, bool bConclusion = true);
	BOOL	AddFail(CString dat, int max);
protected:

	// Generated message map functions
	//{{AFX_MSG(CConclusion)
	virtual BOOL OnInitDialog();
	afx_msg void OnClear();
	afx_msg void OnOsetup();
	afx_msg void OnOriRec();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnDestroy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CONCLUSION_H__475A51CA_D065_4BA2_A159_9EC5F53922B9__INCLUDED_)
