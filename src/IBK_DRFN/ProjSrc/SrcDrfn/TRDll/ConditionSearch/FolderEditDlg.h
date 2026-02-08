#if !defined(AFX_FOLDEREDITDLG_H__AD2249B9_E36C_4802_B105_CA2F6704412B__INCLUDED_)
#define AFX_FOLDEREDITDLG_H__AD2249B9_E36C_4802_B105_CA2F6704412B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// FolderEditDlg.h : header file
//
#include "resource.h"
#include "DefineFiles/client_typedef.h"
#include "../../DrCommon/Dr_Control/DrBtnCtrl.h"

/////////////////////////////////////////////////////////////////////////////
// CFolderEditDlg dialog
class CGrid_Comm;
class CFolderEditDlg : public CDialog
{
	// Construction
public:
	CFolderEditDlg(CWnd* pParent = NULL);   // standard constructor
	
	int				m_nMoveFolderRow;
	CString		m_strEditMode;
	
	CWnd *m_pwndMain;
	CList	<XMLTreeInfoFolder, XMLTreeInfoFolder&> *m_plistXMLTreeInfoFolder;
	
private:
	CFont			m_fontDefault;
	CBrush			m_brushBk;
	
	CStatic			m_ctrlToolBar;
	CDrBtnCtrl		m_btnOutFileMove;
	CDrBtnCtrl		m_btnImport;
	CDrBtnCtrl		m_btnExport;
	
	CStatic			m_ctrlBody;
	CGrid_Comm*		m_pgridFolder;
	CStatic			m_ctrlSelectFolder;
	
	CStatic			m_ctrlFolderBar;
	CDrBtnCtrl		m_btnUpFolder;
	CDrBtnCtrl		m_btnDownFolder;
	CDrBtnCtrl		m_btnTopFolder;
	CDrBtnCtrl		m_btnBottomFolder;
	
	CStatic			m_ctrlMoveMenuBar;
	CDrBtnCtrl		m_btnRight;
	CDrBtnCtrl		m_btnRightAll;
	
	CGrid_Comm*		m_pgridFile;
	CStatic			m_ctrlSelectFile;
	
	CStatic			m_ctrlFileBar;
	CDrBtnCtrl		m_btnUpFile;
	CDrBtnCtrl		m_btnDownFile;
	CDrBtnCtrl		m_btnTopFile;
	CDrBtnCtrl		m_btnBottomFile;
	
	CStatic			m_ctrlCtrlMenuBar;
	CDrBtnCtrl		m_btnFolderAdd;
	CDrBtnCtrl		m_btnFolderInsert;
	CDrBtnCtrl		m_btnFolderModify;
	CDrBtnCtrl		m_btnFolderDel;
	
	CDrBtnCtrl		m_btnFileModify;
	CDrBtnCtrl		m_btnFileDel;
	
	CDrBtnCtrl		m_btnOk;
	CDrBtnCtrl		m_btnCancel;
	
	CStatic			m_ctrlEditMenuBar;
	CStatic			m_ctrlEditTitle;
	CEdit			m_editInput;
	CDrBtnCtrl		m_btnEditOk;
	CDrBtnCtrl		m_btnEditCancel;
	
private:
	CString m_strUserDataPath;
	
	BOOL GetLoadXml(CString strFileRoot, MSXML::IXMLDOMDocumentPtr &pXmlDom);
	
	void OnSizeToolBar(CRect reClient);
	void OnSizeBody(CRect reClient);
	void OnSizeMoveBar(CRect reClient);
	void OnSizeCtrlMenuBar(CRect reClient);
	void OnSizeEditBar(CRect reClient);
	
	void SetBtnEnable(BOOL bEnable);
	void SetEditBarView(BOOL bView);
	
	void SetViewFolderToFileList(CString strFolderPath);
	
	
	void SetSelectRow_File(int nRow);
	BOOL SetDelete_File(CString strName, CString strPath);
	BOOL SetModify_File(CString strFolderPath,
		CString strOldName, CString strOldPath, 
		CString strNewName, CString strNewPath);
	
	BOOL SetMove_File(CString strFolderPath, int nTargetPos, int nMovePos);
	BOOL SetMove_File(CString strTargetFolderPath, CString strFilePath, CString strMoveFolderPath);
	
	
	void SetSelectRow_Folder(int nRow);
	BOOL SetAdd_Folder(CString strName, CString strPath);
	BOOL SetInsert_Folder(int nIndex, CString strName, CString strPath);
	BOOL SetDelete_Folder(CString strFolderPath);
	BOOL SetModify_Folder(CString strOldName, CString strOldPath, 
		CString strNewName, CString strNewPath);
	
	void SetMove_Folder(int nTargetPos, int nMovePos);
	
	BOOL GetHaveFile(CString strFilePath, CString strFolderPath);
	BOOL GetHaveFile(CString strFileName, int nInvertRow = -1);
	BOOL GetHaveFolder(CString strFolderName, CString strFolderPath);
	BOOL GetHaveFolder(CString strName, int nInvertRow = -1);
	BOOL GetCheckNAName(CString strType, CString strData);
	
	POSITION GetPosFolder(CList<XMLTreeInfoFolder, XMLTreeInfoFolder&> *pFolderList, CString strFolderPath);
	POSITION GetPosFile(CList<XMLTreeInfoFolder, XMLTreeInfoFolder&> *pFolderList, 
		CString strFolderPath, CString strFilePath);
	POSITION GetPosFile(CList	<XMLTreeInfoFile, XMLTreeInfoFile&> *pFileList, CString strFilePath);
	
	void SetControlState(BOOL bState = TRUE);
	
	void SetInit();
	void SetGrid_FolderCheck(int nSelectRow);
	
	int SetMessageBox(LPCTSTR lpszText, LPCTSTR lpszCaption = NULL, UINT nType = MB_OK);
	
	// Dialog Data
	//{{AFX_DATA(CFolderEditDlg)
	enum { IDD = IDD_FOLDEREDIT_DLG };
	// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA
	
	
	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFolderEditDlg)
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL
	
	// Implementation
protected:
	
	// Generated message map functions
	//{{AFX_MSG(CFolderEditDlg)
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	//}}AFX_MSG
	
	afx_msg void OnBtnOutFileMove();
	afx_msg void OnBtnImport();
	afx_msg void OnBtnExport();
	
	afx_msg void OnBtnRight();
	afx_msg void OnBtnAllRight();
	
	afx_msg void OnBtnFolderAdd();
	afx_msg void OnBtnFolderInsert();
	afx_msg void OnBtnFolderModify();
	afx_msg void OnBtnFolderDel();
	
	afx_msg void OnBtnFileModify();
	afx_msg void OnBtnFileDel();
	
	afx_msg void OnBtnUpFolder();
	afx_msg void OnBtnDownFolder();
	afx_msg void OnBtnTopFolder();
	afx_msg void OnBtnBottomFolder();
	
	afx_msg void OnBtnUpFile();
	afx_msg void OnBtnDownFile();
	afx_msg void OnBtnTopFile();
	afx_msg void OnBtnBottomFile();
	
	afx_msg void OnBtnEditOk();
	afx_msg void OnBtnEditCancel();
	
	afx_msg void OnBtnOk();
	
	//void OnNotifyGrid(WPARAM wParam, LPARAM lParam);
	LRESULT OnNotifyGrid(WPARAM wParam, LPARAM lParam);
	
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FOLDEREDITDLG_H__AD2249B9_E36C_4802_B105_CA2F6704412B__INCLUDED_)
