#if !defined(AFX_FILTERSET_H__63CD83F6_3C18_4341_9770_CC6D1D9EFC1E__INCLUDED_)
#define AFX_FILTERSET_H__63CD83F6_3C18_4341_9770_CC6D1D9EFC1E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// FilterSet.h : header file
//

#include "AddControl/Grid_Comm.h"

/////////////////////////////////////////////////////////////////////////////
// CFilterSet dialog
class CCodeCtrl;
class CfxImgButton;
class CFilterSet : public CDialog
{
// Construction

private:
	BOOL	m_bRight;
	
	
public:
	StConVolSet	m_stConVolSet;
	StConVolSet	*m_pMainData;
	CString		m_strFullPath;
	CString		m_strScreenNo;
	CStatic		m_TradingValue_line;

	CfxImgButton m_btnOk;
	CfxImgButton m_btnCancel;

	enum
	{
		ERR_STRMSG_USER = 0,
		ERR_STRMSG_CONVOL,
		ERR_STRMSG_DEALPRICE,
		ERR_STRMSG_ACCUM,
		ERR_STRMSG_PRICE,
		ERR_STRMSG_CHGRATE,
		ERR_STRMSG_EMPTY
	};
	
public:
//	CFilterSet(CWnd* pParent = NULL);   // standard constructor
	CFilterSet(CWnd* pParent, CString strScreenNo, StConVolSet*	pstSrc, COLORREF clrBk);

	BOOL OnApply();

	void SetInit();
	BOOL SetInfo(StConVolSet &stConVolSet);
	BOOL GetInfo();
	
	void Set_DefaultInfo();
	static BOOL GetLoad_ConVolInfo(CString strFullPath, StConVolSet	*pstConVolSet, BOOL bRight);
	static BOOL SetSave_ConVolInfo(CString strFullPath, StConVolSet	*pstConVolSet);

	BOOL ErrorCheck();
	void ErrorMessage(UINT nID, LPCSTR szMsg=NULL);
	void SetUserRight(BOOL bRight);
	void ControlEnable(BOOL bEnable);
	void OnSizeCtrl();

	int GetBtnWidth(int nTextLen);
	void OnSetNumExceptText();

	int SetMessageBox(LPCTSTR lpszText, LPCTSTR lpszCaption = NULL, UINT nType = MB_OK);

	void OnBtnAdd();
	void OnBtnRemove();
	void OnBtnRemoveAll();

// Dialog Data
	//{{AFX_DATA(CFilterSet)
	enum { IDD = IDD_DLG_FILTERSET };
	
	CEdit	m_editChgRateDown;
	CEdit	m_editChgRateUp;
	CEdit	m_editSetPriceDown;
	CEdit	m_editSetPriceUp;
	CEdit	m_editAccumVol;
	CEdit	m_editDealPrice;
	CEdit	m_editConVol;
	CEdit	m_editSound;
	CButton	m_chkSound;				//소리사용
	CButton	m_chkChgRateSet;		//등락률지정
	CButton	m_chkSetPrice;			//가격지정
	CButton	m_chkAccumVol;			//누적거래량
	CButton m_btnSound;
	CButton m_btnSearchSound;
	
	CCodeCtrl*		m_pctrlSelectUJCombo; 
	CWnd*			m_pctrlSelectCodeInput;
	CCodeCtrl*		m_pctrlExceptJMCombo; 
	CWnd*			m_pctrlExceptCodeInput;
	
	CfxImgButton	m_btnAdd;
	CWnd*			m_pWndBtnAdd;
	CfxImgButton	m_btnRemove;
	CWnd*			m_pWndBtnRemove;
	CfxImgButton	m_btnRemoveAll;
	CWnd*			m_pWndBtnRemoveAll;

	CWnd*			m_pCtrlParent;

	CFont			m_fontDefault;
	CGrid_Comm		m_gridExcept;
	CStatic			m_staticNumExcept;

	CStringArray	m_arItemCode;
	CStringArray	m_arItemName;
	CStringArray*	arItemCode;
	CStringArray*	arItemName;

	COLORREF		m_clrBk;
	CBrush			m_brushCtrl;
	

	CWnd* m_pMainParent;


	void SetInitExceptItem(CStringArray* _arItemCode, CStringArray* _arItemName);
	void SetInItExceptGrid();
	void AddExceptItem(LPCTSTR szItemCode, LPCTSTR szItemName);
	void RemoveExceptItem();
	void RemoveAllExceptItem();
	void SetExceptItem();
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFilterSet)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CFilterSet)
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	afx_msg void OnChkAcumdealvol();
	afx_msg void OnChkSetprice();
	afx_msg void OnChkSetupdownrate();
	afx_msg void OnChkSound();
	afx_msg void OnRadioConvolUser(UINT nID);
	afx_msg void OnRadioDealpriceUser(UINT nID);
	virtual void OnOK();
	afx_msg void OnBtnSound();
	afx_msg void OnBtnSearchsound();
	virtual void OnCancel();
	afx_msg void OnRadioUpjong();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()


public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnPaint();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FILTERSET_H__63CD83F6_3C18_4341_9770_CC6D1D9EFC1E__INCLUDED_)
