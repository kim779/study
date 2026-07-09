#if !defined(AFX_PRESYMBOL_H__3FC10CDC_28AA_498A_908B_4B93D2D204B1__INCLUDED_)
#define AFX_PRESYMBOL_H__3FC10CDC_28AA_498A_908B_4B93D2D204B1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// preSymbol.h : header file
//

#include "resource.h"
#include <afxtempl.h>
#include "symbol.h"


/////////////////////////////////////////////////////////////////////////////
// CpreSymbol dialog

class AFX_EXT_CLASS CpreSymbol : public CDialog
{
// Construction
public:
	CpreSymbol(CWnd* frame, bool user = false);   // standard constructor
	CpreSymbol()	{}
	virtual ~CpreSymbol();

// Dialog Data
	//{{AFX_DATA(CpreSymbol)
	enum { IDD = IDD_SYMTBL };
	CComboBox	m_combo;
	CTreeCtrl	m_tree;
	CListCtrl	m_list;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CpreSymbol)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

public:
	CImageList* m_dragImage;
	bool	m_default;

protected:
	bool	m_user;
	int	m_state;
	int	m_currentP;
	int	m_currentF;
	CFile	m_file;
	CString	m_root;
	CString	m_curfile;
	bool	m_show;
	int	m_delta;
	CRect	m_clientRc;
	
	class	CwSock*	m_wsock;
	struct	_rsmH*	m_SRH;
	struct	_rsmH*	m_RRH;
	enum	{ reqNONE, reqPROJECT, reqFILE } m_request;

	CArray <Cproject *, Cproject *>	m_arSET;
	CArray <Csymbol *, Csymbol *>	m_arSELECT;
	CString	m_data;
	HTREEITEM m_item;

	CWnd*	m_mainFRM;
	bool	m_load;

	CString	m_ips;
	int	m_port;

public:
	void	SetData(CString data);
	int	GetSelectedCount();
	Csymbol* GetSelectedData(int index);
	void	Load();
	void	SetConnectInfo(CString ips, int port);

protected:
	bool	sendData();
	void	loadProject();
	void	loadProject(Cproject* project, CString path);
	void	loadFile();
	void	loadFile(Croot* root);
	void	removeAll();
	void	makeToken(char ch, CString str, CStringArray& tokArr);
	void	parse(Csymbol* symbol, CStringArray& tokArr);
	void	trim(CString& str);
	void	str2data(CString& str, CString& data);
	BYTE	getType(CString str);
	BYTE	getAlignment(CString str);
	DWORD	getAttributes(CString& str, int size);
	CString	getMask(int type, int size);
	BYTE	getKind(CString str, int& type);
	BYTE	getIO(CString str);
	BOOL	addColumn(LPCTSTR str, int item, int width, bool align_left = true);
	BOOL	addItem(int item, int subItem, CString& str);
	void	fillTree();
	void	fillList(CsymbolSET* symSET);
	CImageList* createDragImage(LPPOINT lpPoint);
	void	goState(int state, CString msg = _T(""));
	CString	getPath(CString fileN);
	BYTE	getIOKind();

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CpreSymbol)
	afx_msg void OnServer();
	afx_msg void OnTreeSelChanged(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnListBeginDrag(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnToggleShow();
	virtual BOOL OnInitDialog();
	afx_msg void OnProjectSelChange();
	//}}AFX_MSG
	afx_msg	LONG OnState(WPARAM wParam, LPARAM lParam);
	afx_msg	LONG OnReadData(WPARAM wParam, LPARAM lParam);

	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PRESYMBOL_H__3FC10CDC_28AA_498A_908B_4B93D2D204B1__INCLUDED_)
