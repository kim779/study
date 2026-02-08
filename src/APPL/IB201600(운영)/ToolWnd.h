#pragma once
// ToolWnd.h : header file
//

#include "../../control/fx_misc/fxImgButton.h"
#include "../../control/fx_misc/fxEdit.h"
#include "../../control/fx_misc/fxStatic.h"
#include "sharemsg.h"


/////////////////////////////////////////////////////////////////////////////
// CToolWnd window


class CToolWnd : public CWnd
{
// Construction
public:
	CToolWnd(CWnd* parent, CWnd* mainWnd);

// Attributes
	CWnd* m_pParent{};
	CWnd*			m_pMainWnd{};
	
	CString			m_root;
	CString			m_name;
	CString			m_user;
	CString			m_id;
	CString			m_home;
//	CBrush			m_bkBrush;
	CString			m_strPal;

	COLORREF		m_clrPallete{};
	char			m_key{};
	CString			m_strParamName, m_strRealCode;
	CFont*			m_font{}, m_fontB{};
	int				m_fontsize{};
	COLORREF		m_clrLineColor{}, m_clrBgColor{}, m_clrDataFgColor{}, m_clrLblFgColor{};
	COLORREF		m_clrColor[10]{};
	int			m_clrColorIdx[10]{};
	
	std::unique_ptr <CfxImgButton> m_btnMatrix{};
	std::unique_ptr<CButton>	m_radio10Hoga{};
	std::unique_ptr<CButton>	m_radio5Hoga{};
	CComboBox		m_cbChangeTab;
	std::unique_ptr<CStatic>	m_labelMatrix{};
	std::unique_ptr <CfxImgButton>	m_btn1;
	std::unique_ptr <CfxImgButton>	m_btn2;
	std::unique_ptr <CfxImgButton>	m_btn3;
	std::unique_ptr <CfxImgButton>	m_btn4;
	std::unique_ptr <CfxImgButton>	m_btn5;
	std::unique_ptr <CfxImgButton>	m_btnSave;
	std::unique_ptr <CButton>	m_chkSelect;
	std::unique_ptr <CButton>	m_chkPrice;
	std::unique_ptr < CfxEdit>		m_pEditFrom;
	std::unique_ptr < CfxEdit>		m_pEditTo;
	std::unique_ptr < CfxStatic>		m_pStString;
	std::unique_ptr < CfxStatic>		m_pStString2;
	std::unique_ptr < CfxStatic>		m_pStPer;

	int				m_nBtnIndex{};

	HBITMAP			m_bitmap_nm{}, m_bitmap_dn{}, m_bitmap_hv{};
	int				m_cbIndex{};				//ÄÞº¸ ÀÎµ¦½º ÀúÀå
	CToolTipCtrl	m_tooltip;
public:

// Operations
	bool			OperInit();
	CString			Variant(int comm, CString data = "");
	CString			GetPalette();
	CBrush*			GetAxBrush(COLORREF clr);
	COLORREF		GetColor(int color);
	CFont*			getAxFont(CString fName, int point=9, bool bBold=false, bool bItalic=false);
	CBitmap*		getAxBitmap(CString path);
	LRESULT			SendAxisMessage(WPARAM wParam, LPARAM lPAram);

	void			 Draw(CDC *pDC, CRect rcClient);
	void			DrawBitmap(CDC *pDC, CRect rc, CString strImage);
	void			DrawRoundBitmap(CDC *pDC, CRect rc, int nKind);

	void			saveRadioBtnInfo(int ihoga);
	void			initRadioBtn(int hoga);
	int			GetRadioCheck();
	void			combodataSetting();
	int			GetComboIndex() { return m_cbIndex; }
	void			SetComboIndex(int index);
	void			ClickButton(int nidx);
	void			saveCheckBoxInfo(int iSelect);
	void			initCheckBox(int iSelect);
	void			loadrange();
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CToolWnd)
	public:
	virtual void OnFinalRelease();
	protected:
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CToolWnd();

	// Generated message map functions
protected:
	//{{AFX_MSG(CToolWnd)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnPaint();
	afx_msg void OnDestroy();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnClose();
	afx_msg void OnTimer(UINT nIDEvent);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
	// Generated OLE dispatch map functions
	//{{AFX_DISPATCH(CToolWnd)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_DISPATCH
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()
};

