#pragma once
// InfoWnd.h : header file
//

#include <afxtempl.h>

class CParam
{
public:
	int		key=0;			// screen key
	CString		name;			// control symbol name
	CRect		rect;			// control rect
	CString		fonts;			// font name
	int		point=0;		// font point
	int		style=0;		// font style
	DWORD		tRGB=0;			// text color index
	DWORD		pRGB=0;			// paint color index
	CString		options;		// properties
};

/////////////////////////////////////////////////////////////////////////////
// CControlWnd window

struct _Info
{
	CString	key;
	CString	val;
};
#define	IS_SEP	"^^"	// key/val ±¸ºÐÀÚ (key + ^^ + val)

/////////////////////////////////////////////////////////////////////////////
// CInfoWnd window

class CInfoWnd : public CWnd
{
// Construction
public:
	CInfoWnd(CWnd *pView, CWnd *pParent, struct _param* pParam);
	void	AddData(CString sKey, CString sVal);
	void	RemoveData(CString sKey);
	void	RemoveAllData();
	void	ShowData();
	void	SelectData(_Info info);
	CString	GetSelectKey();
	CString	GetRecentKey();
	CString	GetSelectVal();
	CString	GetRecentVal();
	CString	GetAllData();
	int	GetListCount();
	CString	GetListHdrKey();
	CString	GetListHdrVal();
	CString	GetListDataKey(int index);
	CString	GetListDataVal(int index);

// Attributes
public:

private:
	CString Variant(int comm, CString data);
	void	CreateFont(int point, CString sFont);
	CString OptionParser(CString sOption, CString sKey);
	CString Parser(CString &srcstr, CString substr);
	void	ReadSectionInfo(CString sFile, CString sSection = "InfoMgr");
	void	WriteSectionInfo(CString sFile, CString sSection = "InfoMgr");

	CWnd*	m_pView;
	CWnd*	m_pParent;
	CParam	m_Param;
	CString	m_sFileName;
	CString	m_sSection;
	CString	m_sListHdrKey;
	CString	m_sListHdrVal;
	int	m_PopWidth;
	int	m_PopHeight;
	CFont*	m_pFont;
	_Info	m_SelInfo;

	CArray <_Info, _Info> m_arInfo;
	std::unique_ptr<class CInfoPopup> m_pInfoPopup;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CInfoWnd)
	public:
	virtual void OnFinalRelease();
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CInfoWnd();

	// Generated message map functions
protected:
	//{{AFX_MSG(CInfoWnd)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnPaint();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
	// Generated OLE dispatch map functions
	//{{AFX_DISPATCH(CInfoWnd)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_DISPATCH
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()
};