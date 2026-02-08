#if !defined(AFX_SAVEDLG_H__C5B4D207_49C3_4EEC_B3D9_6B46A363F064__INCLUDED_)
#define AFX_SAVEDLG_H__C5B4D207_49C3_4EEC_B3D9_6B46A363F064__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SaveDlg.h : header file
//

#include "./AddControls/ExXmlTreeCtrl.h"
#include "../../DrCommon/Dr_Control/DrBtnCtrl.h"


/////////////////////////////////////////////////////////////////////////////
// CSaveDlg dialog
class CSaveDlg : public CDialog
{
public:
	CSaveDlg(CWnd* pParent, CString strLoadedFileName, CString strLoadedFolderPath);   // standard constructor
	CSaveDlg(CWnd* pParent);  

	CWnd*		m_pwndMain;
	CString		m_strLoadedFileName;
	CString		m_strLoadedFolderPath;

	CEdit		m_editFileName;

private:
	CFont		m_fontDefault, m_fontBold;
	CBrush		m_brushBk;
	CString		m_strNotUse;
	BOOL		m_bUpDate;

	//# Del!
	CString			m_strUserDataPath;
	CExXmlTreeCtrl	m_treeXmlCtrl;
	CStatic			m_ctrlTitleFolder;
	CEdit			m_editFolderName;
	CStatic			m_ctrlTitleFile;

	CDrBtnCtrl		m_btnDel;
	CDrBtnCtrl		m_btnOK;
	CDrBtnCtrl		m_btnCancel;

public:
	BOOL SetSaveFile(CString strFolderName, CString strFileName);

private:
	int GetBtnWidth(int nTextLen) { return (int)BTN_WIDTH_BASE + (int)BTN_WIDTH_STEP * nTextLen; }

	BOOL GetCheckSameFile(CString strFolderName, CString strFileName);
	BOOL GetCheckFileName(CString strFileName);
	BOOL GetHaveFolder(CString strFolderPath);

	void SetUpdate_FileData(BOOL bModify, CString strFolderName, CString strFileName);
	int SetMessageBox(LPCTSTR lpszText, LPCTSTR lpszCaption = PROGRAM_TITLE, UINT nType = MB_OK);


	//# Del!
	void SetControlState(BOOL bState = TRUE);

	BOOL SetDelete_File(CString strFilePath, CString strFolderPath);
	BOOL SetDelete_Folder(CString strFolderPath);
		
	// Dialog Data
	//{{AFX_DATA(CSaveDlg)
	enum { IDD = IDD_SAVE_DLG };
	//}}AFX_DATA

	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSaveDlg)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL
	
	// Implementation
protected:
	
	// Generated message map functions
	//{{AFX_MSG(CSaveDlg)
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnDestroy();
	//}}AFX_MSG

	afx_msg void OnBtnSaveDel();
	afx_msg void OnBtnSaveOk();
	afx_msg void OnBtnSaveCancel();

	//afx_msg void OnNotifyXMLTree(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnNotifyXMLTree(WPARAM wParam, LPARAM lParam);

	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SAVEDLG_H__C5B4D207_49C3_4EEC_B3D9_6B46A363F064__INCLUDED_)
