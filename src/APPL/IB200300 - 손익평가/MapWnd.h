#pragma once
// MapWnd.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CMapWnd window
#include "SListCtrl1.h"
#include "SStatic.h"
#include "xSkinButton.h"
#include "../../h/Grid.h"
#include <afxcmn.h>

#define	CNT_STATIC		8		// static °¹¼ö
#define	CNT_TOPDATA		4		// top data ¼ö

#define ROUND_PANEL		1
#define ROUND_CONTENTS	2

#define GAP3	3
#define GAP4	4
#define GAP5	5
#define GAP6	6
#define GAP8	8
#define GAP9	9
class CMapWnd : public CWnd
{
// Construction
public:
	CMapWnd();

// Attributes
public:
	int		m_nCurSel;
	int		m_nPortCurSel;
	int		m_nClkItem;
	int		m_nClkSubItem;

	CWnd*		m_pParent{};
	CFont*		m_pFont{};
	std::unique_ptr<CEdit>  		m_pEdit;
	std::unique_ptr <CSListCtrl1> m_pList;
	std::unique_ptr <CSStatic>	 m_pStatic[8];
	std::unique_ptr <CComboBox>	m_pCombo;
	std::unique_ptr <CxSkinButton>	m_pBtnSave;
	std::unique_ptr <CxSkinButton>	m_pBtnAdd;
	std::unique_ptr <CxSkinButton>	m_pBtnDel;
	std::unique_ptr <CxSkinButton>	m_pBtnChange;

	std::unique_ptr <CxSkinButton>	m_pBtnJango;
	std::unique_ptr<class CJangoDlg> m_pJangoDlg;

	CRect		m_rcs[8];

	CString		m_szRoot;
	CString		m_szUser;
	CString		m_szName;
	CString		m_szCode;
	CString		m_TopData[4];
	
	CStringArray m_PtArray;
	CImageList	m_ilist;

// Operations
public:
	void Alert(char* buff, int len, int key);
	void parsingAlertx(LPARAM lParam);
	void WriteAlert(CString sCode, CString sRtsData);
	void Parsing(char* buff, int len, int key);
	void RegistRTS(CString str, int nCount,int nKey);
	CString	Parser(CString &srcstr, CString substr);
	void Clear();

	void Clear(int nItem,bool bRepaint=true);
	void LoadPortPolio();
	void SavePortPolio();
	void LoadPortList();
	void SavePortList();
	void ChangeColors();
	void InitControl();
	COLORREF GetAxColor(int iColor);
	CFont* GetAxFont(int size ,bool bBold,CString str);
	CString GetAxVariant(int cmd, CString data="");
	CString GetAxCode(int cmd,LPARAM lParam);
	void	SendTR(CString name, CString data, BYTE type, int key);
	CString GetCommaNum(CString str);
	HBITMAP getBitmap(CString path);
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMapWnd)
	//}}AFX_VIRTUAL
private:
	void DrawBitmap(CDC *pDC, CRect rc, CString strImage);
	void DrawRoundBitmap(CDC *pDC, CRect rc, int nKind);
	CString	GetPalette();

// Implementation
public:
	CString m_strPal;
	virtual ~CMapWnd();

	// Generated message map functions
protected:
	//{{AFX_MSG(CMapWnd)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnPaint();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg LONG OnMessage(WPARAM wParam, LPARAM lParam);
	afx_msg LONG OnListDbclk(WPARAM wParam, LPARAM lParam);
	afx_msg LONG OnListClk(WPARAM wParam, LPARAM lParam);
	afx_msg LONG OnDoCalc(WPARAM wParam, LPARAM lParam);
	afx_msg void OnClickList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnBtnSave();
	afx_msg void OnBtnAdd();
	afx_msg void OnBtnDel();
	afx_msg void OnBtnChange();
	afx_msg void OnBtnJango();
	afx_msg void OnSelendokPort();
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRclickList(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

