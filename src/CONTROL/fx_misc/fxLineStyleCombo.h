#pragma once
// fxLineStyleCombo.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CfxLineStyleCombo window

class AFX_EXT_CLASS CfxLineStyleCombo : public CComboBox
{
// Construction
public:
	CfxLineStyleCombo();

// Attributes
protected:
	std::vector<std::unique_ptr<CPen>> _vPen;

// Operations
public:
	int	GetStyle();
	void	SetStyle(int style);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CfxLineStyleCombo)
	public:
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	protected:
	virtual void PreSubclassWindow();
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CfxLineStyleCombo();

	// Generated message map functions
protected:
	void	loadPen();
	void	removePen();

	//{{AFX_MSG(CfxLineStyleCombo)
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

