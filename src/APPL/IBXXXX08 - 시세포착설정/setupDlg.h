#include "../../h/interst.h"
#include "exGrid.h"
#include "exSpin.h"
#include "axStatic.h"
#include "resource.h"
#include <afxtempl.h>
#include "cxPrice.h"
#include "imgBtn.h"

#define ROUND_PANEL		1
#define ROUND_CONTENTS	2

#pragma once

class CSetupDlg : public CDialog
{
public:
	CSetupDlg(CWnd* pParent, CWnd* pView, CString strCodeName, CString strPriceValue);
	
	//{{AFX_DATA(CSetupDlg)
	enum { IDD = IDD_DLGSETUP };
	CImgBtn		m_SearchBtn;
	CImgBtn		m_ListenBtn;
	CImgBtn		m_OkBtn;
	CImgBtn		m_CancelBtn;
	CExSpin		m_spinJmga;
	CExSpin		m_spinJmsu;
	CExSpin		m_spinRate;
	CExSpin		m_spinMsga;
	CExSpin		m_spinMdga;
	CExSpin		m_spinGvol;
	CExSpin		m_spinDiff;
	CExSpin		m_spinCurr;
	CButton		m_OrderExcBtn;
	CComboBox	m_cbMmgb;
	CComboBox	m_cbJmgb;
	CComboBox	m_cbOrderType;
	CComboBox	m_cbDealingType;
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CSetupDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX); 
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CSetupDlg)
	afx_msg void OnDestroy();
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeCbjmgb();
	afx_msg void OnSelchangeCbmmgb();
	afx_msg void OnBtnSearch();
	afx_msg void OnBtnListen();
	afx_msg void OnCxprice();
	virtual void OnCancel();
	afx_msg void OnChkOrderexc();
	afx_msg void OnSelchangeCbdealingType();
	afx_msg void OnPaint();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnBtnOk();
	afx_msg void OnBtnCancel();
	afx_msg void OnChangeOrdercostEdit();
	//}}AFX_MSG
	afx_msg void OnGrSiseLMousedown(NMHDR *pNMHDR, LRESULT *pResult);
	DECLARE_MESSAGE_MAP()

public:
	void	SetFont(LOGFONT lf, CFont *pFont);
	void	SetBFont(LOGFONT blf, CFont *pBFont);
	void	SetCodeInfo(bool bAdd, CString siseS, _codeinfo *codeinfo, int gubn);
	void	GetCodeInfo(_codeinfo *codeinfo);
	void	Alert(const CMapStringToString &rtmStore);
	void	SetRoot(CString root)	{ m_root = root; }
private:
	void	initConstruct();
	void	destroy();
	void	initControl();
	void	initGrid(CExGrid* pGrid, UINT idc);
	int		insertGrid(CExGrid* pGrid, UINT idc, int row);
	void	updateSise();
	void	updateInfo();
	void	saveInfo();
	bool	checkInfo();

	CString	getString(char* datB, int datL);
	void	alertSise(const CMapStringToString &rtmStore, CString symS, int col);
	void	enableItems(int idx, BOOL bEnable, bool bInit);	// bInit: 초기값
	void	radioUDSel(int idx, UINT selidc);
	void	radioAOSel(int idx, UINT selidc);

	CString	getColString(int col, CString tmpS);

	CString  m_strPal;
	CString  GetPalette();
	void	 DrawRoundBitmap(CDC *pDC, CRect rc, int nKind);
	void	 DrawBitmap(CDC *pDC, CRect rc, CString strImage);
	CString  Variant(int comm, CString data);
	CBitmap* getAxBitmap(CString path);
	void	 setImgBtn(CImgBtn *pButton, int nLength, BOOL bImageFit);
	COLORREF GetIndexColor(int index);
	void	 draw_groupBox(CPaintDC *pdc, int ids);
	COLORREF GetAxColor(int iColor);

	bool    (*m_axiscall)(int, WPARAM, LPARAM);

private:
	CWnd* m_pParent{};
	CWnd* m_pView{};
	std::unique_ptr<CExGrid>		m_pSetupTitle;
	std::unique_ptr<CExGrid>		m_pSetupSise;
	CString     m_sCodeName;
	CString     m_sPriceValue;
	_codeinfo	m_codeinfo;
	CString		m_siseS;
	CString		m_sImgDir;	
	LOGFONT		m_lf;	CFont* m_pFont{};
	LOGFONT		m_blf;	CFont* m_pBFont{};
	COLORREF	m_clrBlink{};
	COLORREF    m_clrBk{};
	COLORREF    m_clrWriteBk{};
	COLORREF    m_clrRoundBk{};

	bool		m_bAdd{};
	int			m_gubn{};			// 코드 구분
	CCxPrice	*m_pPrice{};
	CString		m_root;
	CBrush		m_br;
};
