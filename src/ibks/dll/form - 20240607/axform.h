#if !defined(AFX_AXFORM_H__E24804FD_C094_4776_AD8B_E4A4344102F9__INCLUDED_)
#define AFX_AXFORM_H__E24804FD_C094_4776_AD8B_E4A4344102F9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// axform.h : header file
//

#include <afxtempl.h>
#include "../../h/mapform.h"
#include "fmbase.h"

//
//	Clear target
//
#define	CLR_ALL		0
#define	CLR_INPUT	1
#define	CLR_OUTPUT	2

//
//	DoSomething type definition
//
#define	doPUSH		1
#define	doPUSHX		2
#define	doPOP		3
#define	doPOPX		4
#define	doRELOAD	5
#define	doBLINK		6
#define	doTRIGGER	7
#define	doKEY		8
#define	doEXCEL		9
#define	doEXCELx	10
#define	doEXCEL2	11
#define	doTEXT		12
#define	doEXPORT	13
#define	doPRINT		14
#define	doHITPOS	15
#define	doMODAL		16
#define	doCLEAR		17
#define	doSEND		18
#define	doRSEND		19
#define	doOBJECT	20
#define	doIGNORE	21
#define	doENABLE	22
#define	doFOCUS		23
#define	doCHANGE	24
#define	doIME		25
#define	doATTR		26
// 27 deleted
#define	doRADIO		28
#define	doSETFOCUS	29
#define	doCURSOR	30
#define	doHTS		31
#define	doSIZE		32
#define	doVERS		33
#define	doCOMBO		34
#define	doRATIO		35
#define	doBACK		36
#define	doPORTFOLIO	37

#define doDECOMPRESS	40
/////////////////////////////////////////////////////////////////////////////
// CAxisForm command target

class AFX_EXT_CLASS CAxisForm : public CCmdTarget
{
	DECLARE_DYNCREATE(CAxisForm)

	CAxisForm();           // protected constructor used by dynamic creation
	virtual ~CAxisForm();

// Attributes
private:
	HINSTANCE	m_hCode;
	bool		(APIENTRY* axGetCategory)(char* root, int kind, int index, char* code, bool prev);
	int		(APIENTRY* axGetSpinData)(DWORD type, int nfloat, int xfloat, char* data, double value, bool prev);
	double		(APIENTRY* axGetDelta)(DWORD type, double value, bool prev);

protected:
	CMap	<int, int, int, int&> m_radio;
	CMapWordToOb	m_tabs;
	char*		m_mapB;
	DWORD		m_mapL;

	CObArray	m_fmObs;	// form array
	CMapStringToOb	m_n2Obs;	// form mapping : name to form
	int		m_avi;

	int		m_point;
	CString		m_fonts;

public:
	int		m_key;
	struct	_mapH*		m_mapH;
	struct	_formR*		m_formR;
	struct	_cellR*		m_cellR;
	struct	_pageR*		m_pageR;
	DWORD*	m_valueR;
	char*	m_scriptR;
	char*	m_strR;

	class	CAxisDraw*	m_draw;
	class	CAxisPalette*	m_palette;
	CWnd*		m_view;
	CString		m_root;
	bool		m_drawOnly;
	CRect		m_rect;
	CPoint		m_cursor;

	float		m_hRatio;
	float		m_vRatio;

	WORD		m_LCtype;

	bool		m_oldVers;

// Operations
public:
	bool	LoadMAP(CString mapN);
	void	LoadForm(CRect mRect, bool drawOnly = false);

	CfmBase* GetAtForm(int key);
	bool	FindForm(CString symbol, int& key);
	bool	FindForm(CString symbol, CfmBase*& form);
	int	FindForm(CfmBase* form);

	void	SetFont(bool resize, int fpoint, int fstyle = -1, CString fname = _T(""));
	void	DrawForm(CDC* dc);
	void	ClearForm(int type = CLR_ALL);
	void	ResizeForm(float hRatio, float vRatio, bool font = false, bool redraw = false);
	void	ResizeForm(bool redraw = false);
	void	AdjustForm(CSize size, bool move = false);
	void	OffsetForm(CSize offs);
	void	DominoForm(bool all = true);
	void	ReloadForm(CRect mRect);

	void	ResetSystemForm();

	void	SetRadioGroup(CString symbol, bool checked);
	void	UnCheckRadioGroup(CString symbol);

	void	SetTabPage(int index, CfmBase* tab);
	bool	IsFormVisible(CfmBase* form);

	bool	PlayAVI(bool replay = false);
	void	StopAVI();

	bool	GetCode(int kind, int index, CString& code, bool prev = true);
	int	GetSpinData(DWORD type, int nfloat, int xfloat, CString& data, double value, bool prev);
	double	GetDelta(DWORD type, double value, bool prev);
	bool	IsContrast(int& value);
	int	GetContrast(int key, CString& text);
	void	GetContrastData(CString& text);
	CString	TrimSign(CString& data);
	int	GetDouble(int value);
	BOOL	IsRectVisible(CDC* dc);

	virtual	long DoSomething(int type, CfmBase* form, WPARAM wParam = 0, LPARAM lParam = 0, int wpL = 0, int lpL = 0) { return 0; }
	virtual	int  DoComboBox(CfmBase* form)	{ return 0; }
	virtual	void DoProc(CString procs, CfmBase* form, CString data, int count = -1)	{}
	virtual	bool GetHistory(CString name, CString& codes, bool up = true) { return false; }
	virtual	BOOL IsResizable() { return FALSE; }
	virtual	int  GetHeight() { return 0; }
	virtual	int  GetWidth() { return 0; } 
	
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAxisForm)
	public:
	virtual void OnFinalRelease();
	//}}AFX_VIRTUAL

// Implementation
protected:
	void	addForm(CString symbol, CfmBase* form);
	void	makeString();
	void	removeForm();
	void	getExternalScript(CString& scripts, CString& pubs);
	CString	getScripts(CString name, CString& pubs);
	bool	getRoutine(CString& str, CString& src, bool remove = false);
	bool	findOption(CString& scripts);
	bool	isComment(CString str);
	BOOL	isRectVisible(CDC* dc, int index, CRect rect);

	// Generated message map functions
	//{{AFX_MSG(CAxisForm)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
	// Generated OLE dispatch map functions
	//{{AFX_DISPATCH(CAxisForm)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_DISPATCH
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_AXFORM_H__E24804FD_C094_4776_AD8B_E4A4344102F9__INCLUDED_)
