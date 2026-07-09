#if !defined(AFX_UPLOAD_H__3593B0E1_0A9C_40A7_9C56_0981C6146B57__INCLUDED_)
#define AFX_UPLOAD_H__3593B0E1_0A9C_40A7_9C56_0981C6146B57__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// UpLoad.h : header file
//

#include "../awCommon/FormItem.h"
#include "../workio.h"
/////////////////////////////////////////////////////////////////////////////
// CUpLoad window

class AFX_EXT_CLASS CUpLoad : public CWnd
{
// Construction
public:
	CUpLoad();

// Attributes
public:
	struct	_rsmH*	m_pSRH;
	struct	_rsmH*	m_pRRH;
	struct	_workIO		m_workIO;
	class	CwSock*	m_wsock;
	bool	m_bExport;

protected:
	enum	{ W_SRC, W_BIN, W_NICK, W_RES} m_what;
	CString		m_svrPath, m_localPath, m_status;
	bool		m_fDone;
	int		m_kind;
	struct	_mapH	*m_mapH;
	HANDLE		m_file;
	int		m_size, m_offset;
// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CUpLoad)
	//}}AFX_VIRTUAL

// Implementation
public:
	void	finish();
	bool	sendData();
	int	makeData();
	void	fillResourceList();
	bool	initialize();
	CString	setMapPath(CString mapN);
	void	upLoad(CString localPath, CString svrPath, int kind, bool bEnable, struct _mapH *mapH);
	virtual ~CUpLoad();

	// Generated message map functions
protected:
	//{{AFX_MSG(CUpLoad)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	afx_msg LONG OnReadData(WPARAM wParam, LPARAM lParam);
	afx_msg LONG OnOpenedSock(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_UPLOAD_H__3593B0E1_0A9C_40A7_9C56_0981C6146B57__INCLUDED_)
