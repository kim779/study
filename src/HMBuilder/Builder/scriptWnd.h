#pragma once

#include "boldComboBox.h"
#include "nfButton.h"
#include "VBScriptEdit.h"
#include "ttListCtrl.h"

// CScriptWnd

class CScriptWnd : public CDockablePane
{
	DECLARE_DYNAMIC(CScriptWnd)

public:
	CScriptWnd();
	virtual ~CScriptWnd();

	bool	m_bEmulRun;
	bool	m_bwhole;
	CString m_spreObject;
	BOOL	m_toDescription;
	CVBScriptEdit	m_editScript;
	CVBScriptEdit	m_editScript2;

protected:
	CBoldComboBox	m_ctrlCB;
	CBoldComboBox	m_eventCB;
	CNFButton	m_pinBtn;		
	CNFButton	m_pwholeBtn;
	CBoldComboBox	m_listMember;
	CMapStringToString m_mapScript;

	CFont		m_font;
	class	CFormItem* m_formItem;
	HBITMAP		m_hPinBitmap, m_hPinBitmap2;
	struct _mapH*	m_mapH;
	int		m_index;
	CString		m_curdir;
	int		m_prev;
	bool		m_redraw;

	CString		m_click;
	CString		m_dblclk;
	CString		m_change;
	CString		m_char;
	CString		m_keychange;
	CString		m_strDrag;
	CString		m_Scroll;
	CString		m_sAll;

	void	addEventList();
	void	setWholeScript();
	void	setScript(char* dir);
	CString	getEditScript();
	void	insertToEdit(CString str);
	CString getInfoFile(CString sSection, CString sKey, CString sDefault, CString sPath);
	long	getLineCount(CString str);
	CString	parse(CString &src, CString sub);
	void	setEventSel(CString text);
	void	showList(int kind, CString sName);
	DECLARE_MESSAGE_MAP()

public:
	void	Initialize(struct _mapH* mapH);
	void	AddComboString(CString sItem, int data, int type = 0);
	void	ReplaceComboString(CString sItem, int data);
	void	ResetCombo(int type = 1);
	void	LoadAutoList();
	void	SetLinePos(int idx, int nEvent, int linePos);
	CString	GetScript(char *dir);
	CString	GetAllScript(struct _mapH *mapH, int *pLineNum = NULL, int *pIdx= NULL);
	CString GetBookmark(struct _mapH *mapH, int *pLineNum = NULL, int *pIdx= NULL);
	void	SetAllScript(struct _mapH* mapH);
	void	SelectControl(int selctl, bool refresh = false);
	void	WriteScript();
	void	ReloadColors();
	void	SelectEvent(int nEvent);

	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnELSelEndOk();
	afx_msg void OnCLSelEndOk();
	afx_msg	void OnPinClick();
	afx_msg	void OnWholeClick();
	afx_msg void OnMemberDClick();
	afx_msg LRESULT	OnMessage(WPARAM wp, LPARAM lp);
	afx_msg void OnDestroy();
};