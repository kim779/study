#if !defined(AFX_USROBJECT_H__F92F88B8_2F88_49A2_A771_9B36AADEEB10__INCLUDED_)
#define AFX_USROBJECT_H__F92F88B8_2F88_49A2_A771_9B36AADEEB10__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// usrObject.h : header file
//

#include "resource.h"
#include <afxtempl.h>

class AFX_EXT_CLASS Cobject
{
public:
	CString	m_name;		// object name
	CString	m_data;		// data (user object name)
	bool	m_uob;
};

class CobjectSET
{
public:
	CString	m_name;
	CString	m_data;
	CArray <Cobject *, Cobject *> m_arOBJ;
};

class CmainSET
{
public:
	CString	m_name;
	CString	m_data;
	CArray <CobjectSET*, CobjectSET *> m_arMAIN;
};

/////////////////////////////////////////////////////////////////////////////
// CusrObject dialog

class AFX_EXT_CLASS CusrObject : public CDialog
{
// Construction
public:
	CusrObject(class CWnd* frame, bool userscreen = false);
	CusrObject()	{}		// standard constructor
	virtual	~CusrObject();

// Dialog Data
	//{{AFX_DATA(CusrObject)
	enum { IDD = IDD_USERTBL };
	CTreeCtrl	m_tree;
	CListCtrl	m_list;
	//}}AFX_DATA

public:
	CImageList*	m_dragImage;
	enum	TYPE { tpALL=0, tpOBJ, tpUOB } m_type;

protected:
	int	m_state;
	CFile	m_file;
	CString	m_root;
	CString	m_path;
	bool	m_getobject;
	bool	m_show;
	int	m_delta;
	CRect	m_clientRc;
	
	class	CwSock*	m_wsock;
	struct	_rsmH*	m_SRH;
	struct	_rsmH*	m_RRH;

	CArray <CmainSET *, CmainSET *> m_arSET;
	CString	m_data;
	Cobject*  m_select;
	HTREEITEM m_item;

	CWnd*	m_mainFRM;
	bool	m_userScreen;
	bool	m_load;

	CString	m_ips;
	int	m_port;

public:
	void	SetData(CString data);
	Cobject* GetSelectedData();
	void	Load();
	void	SetType(TYPE type);
	void	SetConnectInfo(CString ips, int port);

protected:
	bool	sendData();
	void	loadFile();
	void	removeAll();
	void	trim(CString& str);
	void	str2data(CString& str, CString& data);
	bool	str2data(CString& str);
	BOOL	addColumn(LPCTSTR str, int item, int width, bool align_left = true);
	BOOL	addItem(int item, int dwData, int subItem, CString& str);
	void	fillList(CobjectSET* objSET);
	CImageList* createDragImage(LPPOINT lpPoint);
	void	goState(int state, CString msg = _T(""));
	void	openObject(CString name);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CusrObject)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CusrObject)
	virtual BOOL OnInitDialog();
	afx_msg void OnTreeSelChanged(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnListBeginDrag(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnReceive();
	afx_msg void OnToggleShow();
	afx_msg void OnListDblClick(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	afx_msg	LONG OnState(WPARAM wParam, LPARAM lParam);
	afx_msg	LONG OnReadData(WPARAM wParma, LPARAM lParam);

	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_USROBJECT_H__F92F88B8_2F88_49A2_A771_9B36AADEEB10__INCLUDED_)
