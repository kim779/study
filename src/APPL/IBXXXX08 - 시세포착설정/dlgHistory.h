#pragma once

#include "imgBtn.h"
#include "exGrid.h"
#include "../../h/interst.h"

class CDlgHistory : public CDialog
{
public:
	CDlgHistory(CWnd* pParent, CWnd* pView);

	//{{AFX_DATA(CDlgHistory)
	enum { IDD = IDD_DLGHISTORY };
	CImgBtn	m_btnCancel;
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CDlgHistory)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CDlgHistory)
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg void OnDestroy();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
	void	SetEnv(CString root, CString user, LOGFONT lf) { m_root = root;	m_user = user; m_lf = lf; }
	void	SetData(int nrec, char* buf);
private:
	void	initConstruct();
	void	destroy();
	void	initGrid();
	void	initGrid(CExGrid* pGrid, UINT idc);
	bool	loadHistory();
	void	updateGrid();
	int	insertGrid(CExGrid* pGrid, UINT idc, int row);
	void	updateHistory(int row, _hisinfo *hisinfo);
	void	setImgBtn(CImgBtn *pButton, int nLength, BOOL bImageFit);
private:
	CWnd* m_pParent{};
	CWnd* m_pView{};
	
	CString		m_user;
	CString		m_root;
	CString		m_sHisFile;

	COLORREF	m_clrBk{};
	std::unique_ptr<CExGrid>	m_pGrid{};
	LOGFONT		m_lf{};


	std::vector<std::unique_ptr<_hisinfo>> _vHistory;
};

