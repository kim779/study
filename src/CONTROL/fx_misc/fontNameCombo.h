#pragma once
// fontNameCombo.h : header file
//

struct fontITEM
{
	DWORD	dwType{};
	LOGFONT	lf{};

	fontITEM(){};
	fontITEM(DWORD type, LOGFONT ff)
	{
		dwType = type;
		lf = ff;
	};
};

/////////////////////////////////////////////////////////////////////////////
// CfontNameCombo window

class CfontNameCombo : public CComboBox
{
// Construction
public:
	CfontNameCombo();

// Attributes
public:
	CBitmap	m_mask;
	CBitmap	m_bitmap;
	BOOL	m_loaded;

// Operations
public:
	int	AddFont(LOGFONT* lf, DWORD type);
	CString	GetCurrentName();

	fontITEM* GetFontItem(int sel = -1);
	DWORD	GetFontType(int sel = -1);

protected:
	void	drawMaskedBitmap(CDC* dc, CBitmap* pbmp, CBitmap* pbmpMask, int x, int y, int cx, int cy);
	void	initMaskFromBitmap(CBitmap* pbmp, CBitmap* pbmpMask);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CfontNameCombo)
	public:
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDIS);
	virtual void DeleteItem(LPDELETEITEMSTRUCT lpDIS);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CfontNameCombo();

	// Generated message map functions
protected:
	//{{AFX_MSG(CfontNameCombo)
	afx_msg void OnDropdown();
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()

private:

	vector<std::unique_ptr<fontITEM>> _vFont;
};

