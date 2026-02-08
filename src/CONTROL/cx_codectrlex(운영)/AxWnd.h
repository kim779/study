
#pragma once
// AxWnd.h : header file
//

#include <afxtempl.h>
#include "../../h/axisvar.h"
#include "../../h/axisfire.h"

// control ====================================================================================================================
__declspec(dllexport) CWnd* WINAPI axCreate(CWnd *parent, void* pParam);

/////////////////////////////////////////////////////////////////////////////
// CAxWnd window

class CAxWnd : public CWnd
{
// Construction
public:
	CAxWnd(CWnd *pWnd = NULL);
	virtual ~CAxWnd();

	void	SetRtnStr(CString str)  { m_rtnStr = str;  }
	CString	GetRtnStr()		{ return m_rtnStr; }

	COLORREF GetIndexColor(int color);
//	HBITMAP	GetBitmap(CString bmps);
	CBitmap* GetBitmap(CString bmps);

	CPen*	GetAxPen(COLORREF clr, int width, int style);
	CBrush*	GetAxBrush(COLORREF clr);
	CFont*	GetAxFont(CString fName = "굴림체", int point = 9, int style = 0);

	int	OpenView(CString szMap, int iType = typeVIEW);
	bool	RemoveHistory(CString str);
	HCURSOR	GetCursor(int kind);

	// Wizard function =====================================================================================================
	BOOL		SetAxFocus(bool bflag);
	CString		Variant(int iCommand, CString strData = "");
	//======================================================================================================================

	// Control용 tr =======================================================================================================
	void		SendTRC(CString name, CString data, char stat);
	//=====================================================================================================================

	// Util function =========================================================================================================
	CString		Parser(CString &srcstr, CString substr);
	CString		Comma(CString str);

	// 컨트롤인 경우 =====================================================================================================================
	struct	_param	m_Param;

	// virtual function	=====================================================================================================================
	virtual void    Oub(CString szData, int len, int key);
	virtual void    Alert(CString szData);
	virtual void    Notice(CString szData);
	virtual void    Trigger(CString szData);
	virtual void	Domino(CString szData);

	// 컨트롤인 경우...
	virtual void	SetParam(_param *pParam);

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAxWnd)
public:
	protected:
	//}}AFX_VIRTUAL

// Implementation

protected:
	CWnd*	m_pWizard;
	CString	m_rtnStr;
	CFont*	m_pFont;

	// Generated message map functions
protected:
	//{{AFX_MSG(CAxWnd)
	//}}AFX_MSG
	afx_msg long AxMessage(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
};

