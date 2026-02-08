#pragma once
// ContractWnd.h : header file
//
#include "ContractGrid.h"
#include "ContractScroll.h"

/////////////////////////////////////////////////////////////////////////////
// CContractWnd window

class CContractWnd : public CWnd
{
// Construction
public:
	CContractWnd(CWnd *pWizard, CWnd *pParent);
	virtual ~CContractWnd();

public:
	void	Init(int DisplayCnt, int Half);
	void	Clear();
	void	Dispatch(char* pData, int digit);
	void	Alert(CString sRts);
	void	Alert(struct _alertR* alertR);
	void	Font(CFont* pFont);
	void	Palette();
	void	ChangeDisplay(int DisplayCnt);
	int	GetMax()	{ return m_max; }

// Attributes
private:
	CCX_FODepthApp	*m_pApp;
	CWnd*	m_pWizard;
	CWnd*	m_pParent;
	CSize	m_BasicSize;
	CFont	*m_pFont;
	std::unique_ptr<class CContractGrid>	m_grid;
	bool	m_bAlert;
	int	m_digit;
	int	m_half;
	int	m_max;
	int	m_display;

	void	WriteAlert(CString str, CString &sPrice, CString &sVolume);
	void	WriteAlert(DWORD* data);
	void	ResizeToFit();
	int	CommaModify(CString &str, int digit, float Pow = 1.);

// Operations
public:
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CContractWnd)
	//}}AFX_VIRTUAL

// Implementation

	// Generated message map functions
protected:
	//{{AFX_MSG(CContractWnd)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};