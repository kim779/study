// SymTbl.h: interface for the CSymTbl class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SYMTBL_H__E2A4A85C_8D86_46AE_AD70_A9B5B30BF5D9__INCLUDED_)
#define AFX_SYMTBL_H__E2A4A85C_8D86_46AE_AD70_A9B5B30BF5D9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "symbol.h"
#include "DownLoad.h"

class AFX_EXT_CLASS CSymTbl : public CWnd
{
public:
	CSymTbl();
	virtual ~CSymTbl();
protected:
	BYTE	getIO(CString str);
	BYTE	getKind(CString str, int& type);
	CString	getMask(int type, int size);
	DWORD	getAttributes(CString& str, int size);
	BYTE	getAlignment(CString str);
	BYTE	getType(CString str);
	void	parse(Csymbol* symbol, CStringArray& tokArr);
	void	makeToken(char ch, CString str, CStringArray& tokArr);
	void	loadFile(Croot* root);
	void	trim(CString& str);
	void	loadProject(Cproject* project, CString path);
	void	str2data(CString& str, CString& data);
	CString getPath(CString fileN);
	BOOL	addItem(CListCtrl *LC, int item, int subItem, CString& str);
public:
	Csymbol* GetSelectedData(int index);
	int	GetSelectedCount(CListCtrl *LC, BYTE iok);
	void	fillCombo(CComboBox *prjCBO);
	void	fillCombo(Cproject *project, CComboBox *grpCBO);
	void	fillList(CListCtrl *LC, CsymbolSET* symSET);
	void	loadProject();
	void	loadFile();
	void	removeAll();
	bool	m_default;
	
public:
	void fillTree(CTreeCtrl *TC);
	HTREEITEM getNextSelectedItem(CTreeCtrl *TC, HTREEITEM hitem);
	HTREEITEM getFirstSelectedItem(CTreeCtrl *TC);
	int GetSelectedCount(CTreeCtrl *TC, BYTE iok);
	bool	m_bOffline, m_bReceive;
	CString	m_root;

protected:
	int	m_currentP;
	int	m_currentF;
	CArray <Cproject *, Cproject *>	m_arSET;
	CArray <Csymbol *, Csymbol *>	m_arSELECT;
	CsymbolSET	*m_pCurrentSymSET;
	CDownLoad	*m_pDownLoad;

protected:
	
// Implementation
protected:
	CString Parser(CString &srcstr, CString substr);

	// Generated message map functions
	//{{AFX_MSG(CSymTbl)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

#endif // !defined(AFX_SYMTBL_H__E2A4A85C_8D86_46AE_AD70_A9B5B30BF5D9__INCLUDED_)
