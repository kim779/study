#pragma once

#include "boldComboBox.h"
#include "nfButton.h"
#include "VBScriptEdit.h"

// CScriptWnd

class CScriptWnd : public CDockablePane
{
	DECLARE_DYNAMIC(CScriptWnd)

public:
	CScriptWnd();
	virtual ~CScriptWnd();

	bool	m_bwhole;
	CString m_spreObject;
	BOOL	m_toDescription;
	CVBScriptEdit	m_editScript;

protected:
	CBoldComboBox	m_ctrlCB;
	CBoldComboBox	m_eventCB;
	CNFButton	m_pinBtn;
	CNFButton	m_pwholeBtn;
	CNFButton	m_pythonBtn;
	CBoldComboBox	m_listMember;
	CFont		m_font;
	HBITMAP		m_hPinBitmap, m_hPinBitmap2;
	class mapForm*	m_mapH;
	int		m_index;
	CString		m_curdir;
	int		m_prev;
	bool		m_redraw;

	CString		m_click;
	CString		m_dblclk;
	CString		m_change;
	CString		m_char;
	CString		m_keychange;
	CMapStringToString m_mapScript;
	CMap	<int, int, CString, CString>	m_mapCtmFunc;
	CMap	<int, int, CString, CString>	m_mapCtmEvent;

	void	addEventList();
	void	setWholeScript();
	void	setScript(char* dir);
	CString	getEditScript();
	void	insertToEdit(CString str);
	CString getInfoFile(CString sSection, CString sKey, CString sDefault, CString sPath);
	long	getLineCount(CString str);
	CString	parse(CString &src, CString sub);
	void	showList(int kind, CString sName);
	DECLARE_MESSAGE_MAP()

public:
	void	Initialize(class mapForm* mapForm);
	void	AddComboString(CString sItem, int data, int type = 0);
	void	ReplaceComboString(CString sItem, int data);
	void	ResetCombo(int type = 1);
	void	LoadAutoList();
	void	SetLinePos(int idx, int nEvent, int linePos);
	CString	GetScript(CString dir);
	CString	GetAllScript(class mapForm *mapForm, int *pLineNum = NULL, int *pIdx= NULL);
	void	SelectControl(int selctl);
	void	SetAllScript(class mapForm* mapH);
	void	WriteScript();
	void	ReloadColors();
	void	SelectEvent(int nEvent);
	void	SetCtmObjEvent(DWORD idx, CString strCtmEvent);
	void	SetCtmObjFunc(DWORD idx, CString strCtmFunc);
	void	AddCtmObjEvent();

	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnELSelEndOk();
	afx_msg void OnCLSelEndOk();
	afx_msg	void OnPinClick();
	afx_msg	void OnWholeClick();
	afx_msg	void OnPythonClick();
	void	SetPythonMode(bool bPython);
	afx_msg void OnMemberDClick();
	afx_msg LRESULT	OnMessage(WPARAM wp, LPARAM lp);
};