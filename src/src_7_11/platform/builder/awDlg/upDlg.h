#if !defined(AFX_UPDLG_H__B7181562_4694_11D4_A024_00001CD7F9BE__INCLUDED_)
#define AFX_UPDLG_H__B7181562_4694_11D4_A024_00001CD7F9BE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// upDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CUpDlg dialog
//
#include "resource.h"
#include "../awCommon/FormItem.h"
#include "../workio.h"

class AFX_EXT_CLASS CUpDlg : public CDialog
{
// Construction
public:
	CUpDlg(CWnd* pParent = NULL)	{}   // standard constructor
	CUpDlg(CWnd* pParent, struct _mapH* mapH);

	~CUpDlg();

// Dialog Data
	//{{AFX_DATA(CUpDlg)
	enum { IDD = IDD_UP };
	CListBox	m_list;
	CProgressCtrl	m_step;
	CString	m_nick;
	CString	m_object;
	CString	m_source;
	//CString	m_status;
	//}}AFX_DATA

// Implementation
public:
	CString setMapPath(CString mapN);
	int	m_state;
	void	goState(int state, CString msg = _T(""));

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CUpDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	enum	{ W_SRC, W_BIN, W_RES} m_what;
	class	CwSock*	m_wsock;
	int	m_count;
	int	m_index;
	int	m_sendL;
	int	m_offset;
	struct	_mapH*	m_mapH;
	//struct	_formR*	m_formR;
	CFormItem	*m_pFormItem;
	struct	_rsmH*		m_pSRH;
	struct	_rsmH*		m_pRRH;
	struct	_workIO		m_workIO;

	CString		m_dir;
	HANDLE		m_file;
	int		m_size;
	bool		m_ok;

	bool	sendData(bool next = true);
	int	makeData();
	void	updateProgress(int offset = 0);
	void	fillResourceList();

	// Generated message map functions
	//{{AFX_MSG(CUpDlg)
	virtual void OnCancel();
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	afx_msg	LONG OnState(WPARAM wParam, LPARAM lParam);
	afx_msg	LONG OnReadData(WPARAM wParam, LPARAM lParam);
	afx_msg	LONG OnOpenedSock(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_UPDLG_H__B7181562_4694_11D4_A024_00001CD7F9BE__INCLUDED_)
