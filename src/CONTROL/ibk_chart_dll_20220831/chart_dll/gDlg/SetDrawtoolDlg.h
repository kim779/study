#if !defined(AFX_SETDRAWTOOLDLG_H__338A8E0C_2C02_420A_B502_5206EA89FB2B__INCLUDED_)
#define AFX_SETDRAWTOOLDLG_H__338A8E0C_2C02_420A_B502_5206EA89FB2B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SetDrawtoolDlg.h : header file
//


/////////////////////////////////////////////////////////////////////////////
// CSetDrawtoolDlg dialog

class CSetDrawtoolDlg : public CDialog
{
// Construction
public:
	//CSetDrawtoolDlg(class CMyTool* pMyTool, char* pcInfo, CWnd* pParent = NULL);   // standard constructor
	CSetDrawtoolDlg(char* pcInfo, CWnd* pwndView, CWnd* pParent = NULL);   // standard constructor
public:


private:
	int	m_iPnToolInfoCount;
	int	m_iBaseToolCount;

	CWnd*	m_pwndView;

	CImageList*	m_pImgLst;
	CAxisGDlgApp*	m_pApp;

	LOGFONT	m_logfont;

	class CExGrid	*m_pBaseGrid;
	class CExGrid	*m_pUsedGrid;
//	class CMyTool*	m_pMyTool;
	
	struct _toolConfig* m_ptoolConfig;
	struct _pnToolInfo* m_pPnToolInfo;
	CMap <int, int, class CPnToolInfo*, class CPnToolInfo*> m_mapPnToolInfo;

public:


private:
	void	InitBaseGrid();
	void	InitUsedGrid();
	void	RemoveBasic();
	void	AddUsedGrid();

	void	AddCell(class CfxGrid* pGrid, int iRow, int iCol, CString strText, LPARAM lParam, int iImageIndex = -1);
	void	AddNewLine();
	void	ChangeRow(int iUpRow, int iDownRow);
	bool	IsExist(int iKind);

//	void	IndexUpDown(bool bUp, int iIndex);
	int	GetLastIndex();
	
//	struct _pnToolInfo* FindPnToolInfoByIndex(int iIndex);
//	struct _pnToolInfo* FindPnToolInfoByKind(int iKind);

public:


// Dialog Data
	//{{AFX_DATA(CSetDrawtoolDlg)
	enum { IDD = IDD_DIALOG_SET_DRAWTOOL };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSetDrawtoolDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult);
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CSetDrawtoolDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	afx_msg void OnButtonUp();
	afx_msg void OnButtonDown();
	afx_msg void OnButtonAdd();
	afx_msg void OnButtonDel();
	afx_msg void OnButtonAddAll();
	afx_msg void OnButtonDelAll();
	virtual void OnOK();
	afx_msg void OnButtonDefault();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SETDRAWTOOLDLG_H__338A8E0C_2C02_420A_B502_5206EA89FB2B__INCLUDED_)
