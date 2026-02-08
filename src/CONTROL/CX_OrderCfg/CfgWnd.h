#pragma once

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

// ControlWnd.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CCfg window

class CCfg 
{
// Construction
public:
	CCfg();
	virtual ~CCfg();

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

// Operations
public:
	CString Variant(int comm, CString data);
	void	SetView(int type, CString data);
	CString OptionParser(CString sOption, CString sKey);
	CString Parser(CString &srcstr, CString substr);

	int	AddSectionInfo(CString Section, CString file, CMapStringToString &map);
	void	WriteSectionInfo(CString Section, CString file, const CMapStringToString &map);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCfg)
	//}}AFX_VIRTUAL

// Implementation
public:
	void	CreateCtrl(CWnd* pParent, struct _param* pParam);
	BOOL	SetMap(LPCTSTR PrivateCfgName, LPCTSTR PublicCfgName, LPCTSTR CfgMapName);
	BOOL	ShowDlg();
	CString	ReadCfg(LPCTSTR Ident);
	void	LoadDefaultCfg();
	void	SaveCfg(bool SavePrivateOnly);
	void	WriteCfg(LPCTSTR Ident, LPCTSTR Value);
	void	LoadCfg();
	void	SetPrivateCfgName(LPCTSTR PrivateCfgName);
	void	SetPublicCfgName(LPCTSTR PublicCfgName);
	BOOL	GetNoFee();
};