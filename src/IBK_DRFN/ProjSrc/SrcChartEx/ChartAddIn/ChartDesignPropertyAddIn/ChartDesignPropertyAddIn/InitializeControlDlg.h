#if !defined(AFX_INITIALIZECONTROLDLG_H__003B6A0A_0880_4A80_BF7D_34E1BF2ADB94__INCLUDED_)
#define AFX_INITIALIZECONTROLDLG_H__003B6A0A_0880_4A80_BF7D_34E1BF2ADB94__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// InitializeControlDlg.h : header file
//
#include "BlockSetDlg.h"
#include "ColorSetDlg.h"
#include "DlgToolTip.h"

/////////////////////////////////////////////////////////////////////////////
// CInitializeControlDlg dialog

interface IPropertyMap;
class CInitializeControlDlg : public CDialog
{
// Construction
public:
	//ocx 포인터 얻기.
	CWnd* GetOcxWnd();

	//cancel을 눌렀을때는 false, ok는 true를 return.
	int  GetReturnValue();

	//ocx의 포인터, ocx에 저장되어 있던 값을 받아서 Setting
	void SetOcxWnd(CWnd* pWnd);
	void SetProperty( IPropertyMap *p_pIPropertyMap);

	CInitializeControlDlg( HWND p_hOcxWnd, CWnd* pParent = NULL);   // standard constructor
	~CInitializeControlDlg();

// Dialog Data
	//{{AFX_DATA(CInitializeControlDlg)
	enum { IDD = IDD_INTIALIZECONTROL };
	CListCtrl	m_BlockColorFontListCtrl;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CInitializeControlDlg)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CImageList m_ImageList;

	// Generated message map functions
	//{{AFX_MSG(CInitializeControlDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg void OnDblclkBlockcolorfont(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnClickBlockcolorfont(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	//ocx의 핸들값
	CWnd* m_pOcxWnd;

	//리스트컨트롤이 처음에 뜰때 강제로 Set해 주었던 것을 풀기위해 저장.
	int m_nPreBlockColorFont;

	//ocx에 보낼 리턴값.
	int m_nReturnValue;  

	//툴팁
	CDlgToolTip	m_toolTip;
	void CreateToolTip();

	//리스트컨트롤 초기화.
	void InitList();

	//Block 설정 Dlg
	CBlockSetDlg m_BlockSetDlg;
	//Color 설정 Dlg
	CColorSetDlg m_ColorSetDlg;

// (2008/1/22 - Seung-Won, Bae) for Multi-Language
protected:
	HWND	m_hOcxWnd;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_INITIALIZECONTROLDLG_H__003B6A0A_0880_4A80_BF7D_34E1BF2ADB94__INCLUDED_)
