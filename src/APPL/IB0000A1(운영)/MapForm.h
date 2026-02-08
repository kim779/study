#pragma once
// MapForm.h : header file
//
#include "resource.h"
/////////////////////////////////////////////////////////////////////////////
// CMapForm form view

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif

#include <afxtempl.h>
#include "resource.h"
#include "MyButton.h"

#include "../../dll/ctrl/XListCtrl.h"
#include "NeoStatic.h"
#include "../AxMapForm/AxMapForm.h"
#include "xxgrid.h"

#define LABEL_COUNT	6

extern const COLORREF clrBkGnd;

class	Caccount
{
public:
	CString	m_accname;
	CString	m_account;
	CString	m_pass;
	CString m_acctAlias;
	CString m_own;
	CString m_gpnm;
	CString toString() {
		char buff[1024];
		sprintf(buff, "%s(%s)[%s][%s][%s][%s]\n", m_account, m_accname, m_pass, m_acctAlias, m_own, m_gpnm);
		return buff;
	}
};

class CMapForm : public CAxMapForm
{
public:
	CMapForm();           // protected constructor used by dynamic creation
	virtual ~CMapForm();
	DECLARE_DYNCREATE(CMapForm)

// Form Data
public:
	//{{AFX_DATA(CMapForm)
	enum { IDD = IDD_ACCOUNT };
	CButton	m_btnChangeAll;
	CButton	m_btnChange;
	CButton	m_useAlias;
	CMyButton	m_btOK;
	//}}AFX_DATA

//	CArray	< Caccount *, Caccount * > m_acs;
// Attributes

protected:
	// 2011.02.25 직원의 그룹별 저장기능을 추가하기 위함.
	BOOL m_staff, m_bSaveAllPasswdMode;
	int m_iSaveAllPasswdCnt, m_iSuccessCnt, m_iFailedCnt;
	BOOL m_bFailed;	//2014.09.05 KSJ 실패했을때는 전체저장을 멈춘다.
	CString m_filter;
	CMap<CString, LPCSTR, CStringArray*, CStringArray*> m_groups;
	void FillAccountGrid();
	void SaveAllPasswd(LPCSTR pass);
	void process_CheckAllPSWD(LPCSTR dat);
	void LoadGroupInfo();	// only staff
	void processGroup(char *data, int dlen);
	Caccount* FindAccount(LPCSTR szAcno);
	Caccount* FindAccount(int sel);
	int FindRow(LPCSTR szAcno);
	void FreeGroupData();
	void SetFocusAliasOrPass();

protected:
	CString	m_home;
	CString	m_user;
	CString m_name;
	
	bool	m_checkpass;

	int	m_select;
	int	m_cursor;
	CArray	< Caccount *, Caccount * > m_acs;
	CFont	m_font;
	int m_nAllRow;
// Operations
public:
	HHOOK	m_hook;

	BOOL	CreateMap(CWnd* parent, CWnd* view);
	LRESULT	OnUser(WPARAM wParam, LPARAM lParam);
	void	InputEnter();
	bool	SetNext(bool back);
	void	CloseDll();
	void	SetPswd(int sel, CString pswd);
	void	SetPswd(CString acc, CString pswd);
	CString	GetPswd(int sel);
	void	ImeSet(bool bOn);
	void	MouseMove(MSG* msg);//2012.07.10 KSJ 마우스 이동시 툴팁

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMapForm)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	void	Init();
	void	fit();
	bool	setAccountData(int sel);
	void	process_CheckPSWD(char* dat);
	CWnd*	GetWnd(UINT nID);
	void	loadPass(bool bQuery = true);
	void	queryACC();
	void	processQuery(char* pBytes);
	void	processSave(char* pBytes, int nBytes);
	void	sendTR(CString trC, char* pBytes, int nBytes, char key = 'x');
	CString	Variant(int comm, CString data);
	//**void	EnDecrypt(char *pass, int nCnt);
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
	//{{AFX_MSG(CMapForm)
	afx_msg void OnChangePass();
	afx_msg void OnDelpass();
	afx_msg void OnSaveaccount();
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg void OnOK();
	afx_msg void OnCancel();
	afx_msg void OnDestroy();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnUsealias();
	afx_msg void OnSelchangeGroup();
	afx_msg void OnAllsavesend();
	afx_msg void OnGroupConfig();
	afx_msg void OnCheckLogin();
	//}}AFX_MSG
	afx_msg LONG OnCtrl(WPARAM wParam, LPARAM lParam);
	afx_msg void OnGridClick(NMHDR *pNMHDR, LRESULT *pResult);
	DECLARE_MESSAGE_MAP()

	class	CTooltipDlg*	m_pToolTip;
private:
	void			SetMsg(const char* lspzMsg = _T(""), COLORREF fgColor = 0);
	void			InitGrid();
	CXXGrid*		m_grid;			
	CFont			*m_pFont;
	CNeoStatic		*m_label[LABEL_COUNT];
	CWnd*			m_main;
	BOOL			SelectAccount(int row);
	BOOL			IsValidSelected(int sel);
	void			Clear();

	void			ChangePasswordNotifyToMain(const char* data);
	void			GridRowInit(int rows);
	void			GotoAlias();
	void			SetAlias(int sel, const char* szAlias);
	CString			GetAlias(int sel);
	CString			GetName(int sel);

	void			LoadAccountListFromWizard();

	void			LoadConfig();
	void			SaveConfig();
	CString			GetUserConfigFileName();
	CBrush			m_bkBrush;

	CToolTipCtrl*	m_pTip;
};

