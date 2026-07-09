// ScriptBar.h: interface for the CScriptBar class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SCRIPTBAR_H__1034B21F_3D17_43BC_84A3_4911626A4367__INCLUDED_)
#define AFX_SCRIPTBAR_H__1034B21F_3D17_43BC_84A3_4911626A4367__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "stdAfx.h"
#include "mapform.h"
#include "awWcc/libwcc.h"
#include "awCommon/FormItem.h"
#include "Components/crysedit_src/VBScriptEdit.h"
#include "Components/ComboBoxBold.h"
#include "NFBtn.h"

class CScriptBar : public CSizingControlBarG  
{
public:
	CScriptBar();
	virtual ~CScriptBar();
public:
	CString	Parser(CString &srcstr, CString substr);
	void	LoadAutoList();
	void	ReplaceComboString(CString sItem, int data);
	void	setLinePos(int idx, int nEvent, int linePos);
	long	getLineCount(CString str);
	CString	getScript(char *dir);
	CString	getAllScript(struct _mapH *mapH, int *pLineNum = NULL, int *pIdx= NULL);
	void	OnPinBtnClick();
	void	OnPyBtnClick();
	void	setScript(char* dir);
	void	SelectControl(int selctl);
	void	WriteScript();
	CString	GetEditScript();
	void	insertToEdit(CString str);
	void	AddEventList();
	void	Initialize(struct _mapH* mapH);
	void	AddComboString(CString sItem, int data, int type = 0);
	void	ResetCombo(int type = 1);

	void ReloadColors();
	void SelectEvent(int nEvent);
	CString getInfoFile(CString sSection, CString sKey, CString sDefault, CString sPath);
public:
	BOOL		m_bToDescription;
	CVBScriptEdit	m_editScript;
protected:
	CComboBoxBold	m_ctrllistCBO, m_eventlistCBO;
	CNFBtn		m_pinBtn;
	CNFBtn		m_pythonBtn;
	CFont		m_font, m_scriptFont;
	CFormItem	*m_pFormItem;
	HBITMAP		m_hPinBitmap, m_hPinBitmap2;
	struct _mapH	*m_mapH;
	int		m_index;
	CString		m_curdir;
	int		m_prev;
	bool		m_bRedraw;

	CString		m_click;
	CString		m_dblclk;
	CString		m_change;
	CString		m_char;
	CString		m_keychange;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CScriptBar)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

		// Generated message map functions
protected:
	//{{AFX_MSG(CScriptBar)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//}}AFX_MSG
	afx_msg void	OnELSelEndOk();
	afx_msg void	OnCLSelEndOk();
	afx_msg long OnMessage100(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
};

#endif // !defined(AFX_SCRIPTBAR_H__1034B21F_3D17_43BC_84A3_4911626A4367__INCLUDED_)
