#pragma once

#include "CodeCombo.h"
#include "CodeEdit.h"
#include "SubList.h"
#include "../../../h/axisfire.h"
// CControlwnd
/*
struct	_param {
	INT_PTR		key;			// LOWORD(key) : screen key, HIWORD(key) : input size
	CString		name;			// control symbol name
	CRect		rect;			// control rect
	CString		fonts;			// font name
	int			point;			// font point
	int			style;			// font style
	DWORD		tRGB;			// text color index
	DWORD		pRGB;			// paint color index
	CString		options;		// properties
};
*/
class CParam
{
public:
	int		key;			// screen key
	CString		name;			// control symbol name
	CRect		rect;			// control rect
	CString		fonts;			// font name
	int		point;			// font point
	int		style;			// font style
	DWORD		tRGB;			// text color index
	DWORD		pRGB;			// paint color index
	CString		options;		// properties
};



/////////////////////////////////////////////////////////////////////////////

class CControlwnd : public CWnd
{
	DECLARE_DYNAMIC(CControlwnd)

public:
	CControlwnd();
	virtual ~CControlwnd();

	virtual void OnFinalRelease();

	CWnd* m_pWizard;
	class CCodeEdit*  m_pEdit;
	class CCodeCombo* m_pCombo;

	CMapStringToString m_mapAg;   //fx_AgDlg 에서 설정한 값  2자리

	CStringArray m_arrtype;     //2자리 회사구분자
	bool		m_bonlyBal;     //발행사 여부

	CMapStringToString m_mapCodeAg;  // 맵에서 지정한 이구분자를 보고 같은 매서드로 들어온 코드리스트(6자리)를 보여줄지 아니면 반대로 안보여줄지 결정한다
	bool		m_bShowCodeList;    // 맵에서 지정한 이구분자를 보고 같은 매서드로 들어온 코드리스트를 보여줄지 아니면 반대로 안보여줄지 결정한다

	CRect		m_btnRect;
	CString		m_sHistory;
	CString		m_strHome;
	CString		m_sUser;
	CString		m_sSymbol;
	CString		m_tabPath;
	CParam		m_Param;
	bool		m_bValidCheck;
	
	void	ResetAuthAg();
	void	SetParam(struct _param *pParam);
	void	ResizeControls();
	void	SetUnit(int unit);
	CBitmap* GetBitmap(CString path);
	void	DrawBitmap(CDC *pDC, CRect rc, CBitmap *pBitmap);
	void	HistoryMode();
	void	LButtonUp();
	CString Parser(CString &srcstr, CString substr);
	void	CodeListMode();
	void	SetValidCheck(bool bValid);
	void	MouseDrag(CString strData);
	void	SetDrag(BOOL nDrag = FALSE);
	void	SetEditData(CString sData);
	void	SendCtrlMsg(int igubn);
	bool	IsCheckType(CString type);
	void	Trigger(CString strparam);
	void	LoadAuthAg();
	bool	ChecktypeAG(CString type, CString onlytype, CString tmpcode);
	bool	CheckcodeAG(CString type, CString onlytype, CString tmpcode);
private:
	CWnd*		m_parent;
	CFont		m_Font;
	int			m_Unit;
	BOOL		m_bDrag;

	BOOL		m_bDown, m_bHover, m_bTracking;
	enum	    _MouseSate {
		dispidResetCondition = 9L,
		dispidSetBalListType = 8L,
		dispidSetEnable = 7L,
		dispidSetVisible = 6L,
		dispidSetHsymbol = 5L,
		dispidSetType = 4L,
		dispidGetCode = 3L,
		dispidname = 2,
		dispidSetdata = 1L,
		MsNo = 0, MsOver, MsDown} m_MouseState;
	CBitmap	*m_pBitmap[3];

protected:
	DECLARE_MESSAGE_MAP()
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()
public:
	afx_msg LRESULT OnMessage(WPARAM wParam, LPARAM lParam);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnPaint();
	afx_msg LRESULT OnMouseHover(WPARAM wparam, LPARAM lparam);
	afx_msg LRESULT OnMouseLeave(WPARAM wparam, LPARAM lparam);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnDestroy();
protected:
	void SetName(LPCTSTR data);
	BSTR Getname(void);
	void Setname(LPCTSTR newVal);
	BSTR GetCode(void);
	void SetType(LPCTSTR type, LPCTSTR btype);
	void SetHsymbol(LPCTSTR symbol);
	void SetVisible(LONG bVisible);
	void SetEnable(LONG benable);
	void SetBalListType(LPCTSTR list, LPCTSTR type);
	void ResetCondition(void);
public:
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};


