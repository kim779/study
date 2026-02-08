#pragma once

// ControlWnd.h : header file
//

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

class CControlWnd : public CWnd
{
// Construction
public:
	CControlWnd();
	void CreateCtrl(CWnd* pParent, struct _param* pParam);

// Attributes
public:
	CWnd*	m_pParent;
	CParam	m_Param;
	CString m_sUserDir;
	CString m_sCfgFile;

	// Common Config
	CString	m_mapSection;
	CString m_publicSection;
	CString m_cfgDlg;
	CMapStringToString m_publicIdents;
	CMapStringToString m_mapIdents;

	HINSTANCE	m_hCfg;
	CWnd*		m_pCfg;

// Operations
public:
	CString Variant(int comm, CString data);
	void	SetView(int type, CString data);
	CString OptionParser(CString sOption, CString sKey);
	CString Parser(CString &srcstr, CString substr);

	BOOL	SectionExists(CString Section, CString file);
	int	AddSectionInfo(CString Section, CString file, CMapStringToString &map);
	void	WriteSectionInfo(CString Section, CString file, const CMapStringToString &map);
	void	LoadCfg();
	void	SaveCfg();

	void	FreeMapDll();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CControlWnd)
	public:
	virtual void OnFinalRelease();
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CControlWnd();

	// Generated message map functions
protected:
	//{{AFX_MSG(CControlWnd)
	afx_msg void OnDestroy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
	// Generated OLE dispatch map functions
	//{{AFX_DISPATCH(CControlWnd)
	afx_msg BOOL _SetMap(LPCTSTR PrivateCfgName, LPCTSTR PublicCfgName, LPCTSTR CfgMapName);
	afx_msg BOOL _ShowDlg();
	afx_msg BSTR _ReadCfg(LPCTSTR Ident);
	afx_msg void _LoadDefaultCfg();
	afx_msg void _SaveCfg();
	afx_msg void _WriteCfg(LPCTSTR Ident, LPCTSTR Value);
	afx_msg void _LoadCfg();
	afx_msg void _SetCfgDlg(LPCTSTR CfgMapName);
	afx_msg void _SetPrivateCfgName(LPCTSTR PrivateCfgName);
	afx_msg void _SetPublicCfgName(LPCTSTR PublicCfgName);
	//}}AFX_DISPATCH
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()
};
