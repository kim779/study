/***********************************************
	Ultimate Grid 97
	Copyright 1994 - 1997 Dundas Software Ltd.


	class 
		CUGSideHdg
	Purpose
		This class handles the drawing of the side
		heading, plus handles and forwards events
		that took place inside of the side heading.
		NOTE: The whole grid is made up of several 
		parts, top heading, side heading, corner button,
		grid area, scroll bars and a tab area.
	Details
		-This class handles the drawing of the 
		 side heading and is responsibile for drawing
		 the heading in an optomized manner.
		-This class handles all mouse and keyboard
		 events and forwards the events to the
		 CUGCtrl class and other appropriate classes
************************************************/

#undef  AFX_DATA   
#ifdef __UGRID_IMPL__
	#define AFX_DATA AFX_DATA_EXPORT
#else
	#define AFX_DATA AFX_DATA_IMPORT
#endif

class CUGSideHdg : public CWnd
//class AFX_EXT_CLASS CUGSideHdg : public CWnd
{
// Construction
public:
	AFX_EXT_API	CUGSideHdg();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CUGSideHdg)
	protected:
	//}}AFX_VIRTUAL

// Implementation
public:
	AFX_EXT_API	virtual ~CUGSideHdg();

	// Generated message map functions
protected:
	//{{AFX_MSG(CUGSideHdg)
	AFX_EXT_API	afx_msg void OnPaint();
	AFX_EXT_API	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	AFX_EXT_API	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	AFX_EXT_API	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	AFX_EXT_API	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	AFX_EXT_API	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	AFX_EXT_API	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	AFX_EXT_API	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()


protected:
	
	friend CUGCtrl;
	CUGCtrl *		m_ctrl;
	CUGGridInfo *	m_GI;			//pointer to the grid information

	CUGCell			m_cell;			//general purpose cell class

	CUGDrawHint		m_drawHint;		//cell drawing hints

	long			m_bottomRow;	//the row drawn at the bottom

	int				m_isSizing;			//sizing flag
	int				m_canSize;			//sizing flag
	int				m_colOrRowSizing;	// 0 - col 1- row
	long			m_sizingColRow;		// column or row number being sized
	int				m_sizingStartSize;	// original col/row width/height
	int				m_sizingStartPos;	// original starting mouse position
	int				m_sizingStartWidth;//original side heading total width
	int				m_sizingNumRowsDown; //number of rows from top when size started

	RECT			m_focusRect;		//focus rect for column sizing option

	AFX_EXT_API	int GetCellRect(int *col,long *row,RECT *rect);
	AFX_EXT_API	int GetCellRect(int col,long row,RECT *rect);
	AFX_EXT_API	int GetCellFromPoint(CPoint *point,int *col,long *row,RECT *rect);
	AFX_EXT_API	void DrawCellsIntern(CDC *dc);

public:

	AFX_EXT_API	int GetSHColWidth(int col);

	//internal functions
	AFX_EXT_API	void Update();
	AFX_EXT_API	void Moved();
};

#undef  AFX_DATA   
#define AFX_DATA

/////////////////////////////////////////////////////////////////////////////
