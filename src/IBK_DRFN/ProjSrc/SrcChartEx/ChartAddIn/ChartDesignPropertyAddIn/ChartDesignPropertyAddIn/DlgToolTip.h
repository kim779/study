#if !defined(AFX_DLGTOOLTIP_H__69867E8B_0969_4652_9018_A6ECEC93727D__INCLUDED_)
#define AFX_DLGTOOLTIP_H__69867E8B_0969_4652_9018_A6ECEC93727D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgToolTip.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgToolTip dialog
// default tooltip colors
#define IVORY       RGB(255, 255, 220)
#define BLACK		RGB(0, 0, 0)

// this structure holds all the tooltip information
struct ToolTipInfo : public CObject
{
	UINT			nControlID;		// ID of the control
	UINT			nInfoSize;		// number of lines in the info
	CStringArray	nControlInfo;	// container of the information
	COLORREF		nTextColor;		// text color
	COLORREF		nBackColor;		// background color
};

class CDlgToolTip : public CWnd
{
// Construction
public:
	CDlgToolTip();//(CWnd* pParent = NULL);   // standard constructor
	CRect m_ptButtonSize;
	CPoint m_point;
	void HideToolTip(CPoint& point);
	virtual ~CDlgToolTip();	// destructor

	void		Create(CWnd* pWnd);
	void		ErasePreviousToolTipDisplay( UINT );
	void		ShowToolTip( UINT , CPoint& point);		// explicitly shown the tooltip given the control ID

	// NOTE: the user must override the PreTranslateMessage in the calling window in order
	// to handle mousemovent. 
	void		ShowToolTip( CPoint& );		// called only during Mousemovement

	// tooltip functions
	BOOL		AddControlInfo( UINT, CDWordArray &, COLORREF back=IVORY, COLORREF text=BLACK );
	BOOL		RemoveControlInfo( UINT );

	// inline functions
	void		SetFontSize( int size ) { m_nFontSize = size; }

// Dialog Data
	//{{AFX_DATA(CDlgToolTip)
	enum { IDD = IDD_DIGTOOLTIP };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

// Implementation
protected:

private:
	CRect           m_rcToolTip;
	CWnd*			m_pParentWnd;
	int				m_nHeight;
	int				m_nWidth;
	int				m_nFontSize;

	int				m_nTotalLine;
	int				m_maxCharInLine;  

	virtual void	CalculateInfoBoxRect(UINT nControlID, CRect* pInfoRect,CPoint& point);
	virtual void	CalculateHeightAndWidth(CStringArray& straInfos);

	ToolTipInfo*	IsControlIDExisting( UINT );
	void			DisplayInfo( ToolTipInfo* , CPoint& point );

	CObArray		m_aControlInfo;
	UINT			m_currentControlID;

// (2008/1/14 - Seung-Won, Bae) for MultiLanguage.
protected:
	HWND	m_hOcxWnd;
public:
	void	SetOcxHwnd( HWND p_hOcxWnd)		{	m_hOcxWnd = p_hOcxWnd;	}
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGTOOLTIP_H__69867E8B_0969_4652_9018_A6ECEC93727D__INCLUDED_)
