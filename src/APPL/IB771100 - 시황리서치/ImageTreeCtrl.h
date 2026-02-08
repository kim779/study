#pragma once
// ImageTreeCtrl.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CImageTreeCtrl window

//#include "Tmschema.h"

typedef HANDLE HTHEME;          // handle to a section of theme data for class

const DWORD	COLOR_NONE = ((DWORD)-1);
const int TV_NOIMAGE = 0xFFFE;

const UINT HOT_INDEX		= 8;
const UINT DISABLED_INDEX	= 4;

//=============================================================================
// drag operations flags for m_dwDragOps
//=============================================================================
#define XHTMLTREE_DO_CTRL_KEY		0x0001	// TRUE = Ctrl key toggles move/copy;
// FALSE = Ctrl key is ignored
#define XHTMLTREE_DO_SHIFT_KEY		0x0002	// TRUE = Shift key toggles "move
// under" mode; FALSE = Shift key is 
// ignored
#define XHTMLTREE_DO_SCROLL_NORMAL	0x0004	// TRUE = normal drag scroll
#define XHTMLTREE_DO_SCROLL_FAST	0x0008	// TRUE = fast drag scroll
#define XHTMLTREE_DO_COPY_DRAG		0x0010	// TRUE = change default drag from
// move to copy
#define XHTMLTREE_DO_AUTO_EXPAND	0x0020	// TRUE = auto-expand node when 
// cursor hovers

#define XHTMLTREE_DO_DEFAULT		(XHTMLTREE_DO_CTRL_KEY | \
	XHTMLTREE_DO_SHIFT_KEY | \
	XHTMLTREE_DO_SCROLL_NORMAL | \
									 XHTMLTREE_DO_AUTO_EXPAND)
//=============================================================================
// data returned in notification messages
//=============================================================================
struct XHTMLTREEMSGDATA
{
	HWND		hCtrl;				// hwnd of XHtmlTree
	UINT		nCtrlId;			// id of XHtmlTree
	HTREEITEM	hItem;				// current item
};

//=============================================================================
// CXHtmlTree data
//=============================================================================
struct XHTMLTREEDATA
{
	// ctor
	XHTMLTREEDATA()
	{
		bChecked         = FALSE;
		bEnabled         = TRUE;
		bSeparator       = FALSE;
		
		// below are reserved for CXHtmlTree use only
		hTreeCtrl        = 0;
		bExpanded        = FALSE;
		bHasBeenExpanded = FALSE;
		bModified        = FALSE;
		nChildren        = 0;
		nChecked         = 0;
		nSeparators      = 0;
		pszNote          = 0;
		nTipWidth        = 0;
		nCount++;
	}
	
	// dtor
	virtual ~XHTMLTREEDATA()
	{
		//if (pszNote)
		//	delete [] pszNote;
		//pszNote = 0;
		nCount--;
	}
	
	BOOL	bChecked;				// TRUE = item checkbox is checked
	BOOL	bEnabled;				// TRUE = enabled, FALSE = disabled 
	// (gray text)
	BOOL	bSeparator;				// TRUE = item is separator; cannot have children
	
	// below are reserved for CXHtmlTree use only
	
	HWND	hTreeCtrl;				// HWND of tree control
	BOOL	bExpanded;				// TRUE = item is expanded to show its children
	BOOL	bHasBeenExpanded;		// TRUE = item has been expanded (at least once)
	BOOL	bModified;				// TRUE = item has been modified
	int		nChildren;				// total count of children of this 
	// item
	int		nChecked;				// count of children that are 
	// checked - an item in a "mixed" 
	// state is counted as being 
	// unchecked
	int		nSeparators;			// count of children that are separators
	//TCHAR * pszNote;				// note for tooltip
	std::unique_ptr<TCHAR []> pszNote;
	int		nTipWidth;				// width of tooltip for note
//	CXHtmlDraw::XHTMLDRAWSTRUCT ds;	// HTML draw info
	static int nCount;				// incremented in ctor, decremented in dtor
};


class CImageTreeCtrl : public CTreeCtrl
//=============================================================================
{
//=============================================================================
// Construction
//=============================================================================
public:
	CImageTreeCtrl();
	virtual ~CImageTreeCtrl();

//=============================================================================
// Attributes
//=============================================================================
public:

	enum CheckedState { UNUSED1 = 0, 
						UNCHECKED, CHECKED, TRISTATE, 
						UNUSED2,
						UNCHECKED_DISABLED, CHECKED_DISABLED, TRISTATE_DISABLED };

#ifdef XHTMLHTML
	CXHtmlDrawLink	m_Links;
#endif // XHTMLHTML

	COLORREF	GetBkColor() { return m_crCustomWindow; }
	BOOL		GetBit(DWORD bits, DWORD bitmask) { return bits & bitmask; }
	BOOL		GetCheck(HTREEITEM hItem);
	int			GetCheckedCount();
	int			GetChildrenCheckedCount(HTREEITEM hItem);
	int			GetChildrenCount(HTREEITEM hItem);
	int			GetChildrenDisabledCount(HTREEITEM hItem);
	int			GetDefaultTipWidth();
	COLORREF	GetDisabledColor(COLORREF color);
	BOOL		GetDisplayToolTips() { return m_bToolTip; }
	DWORD		GetDragOps() { return m_dwDragOps; }
	HTREEITEM	GetFirstCheckedItem();
	BOOL		GetHasBeenExpanded(HTREEITEM hItem);
	BOOL		GetHasCheckBoxes() { return m_bCheckBoxes; }
	BOOL		GetHtml() { return m_bHtml; }
	BOOL		GetImages() { return m_bImages; }
	int			GetIndentLevel(HTREEITEM hItem);
	COLORREF	GetInsertMarkColor() { return m_crInsertMark; }
	BOOL		GetItemBold(HTREEITEM hItem);
	CString		GetItemNote(HTREEITEM hItem, BOOL bStripHtml = FALSE);
	int			GetItemNoteWidth(HTREEITEM hItem);
	BOOL		GetItemPath(HTREEITEM hItem, CStringArray& sa, CPtrArray& items);
	BOOL		GetItemRect(HTREEITEM hItem, LPRECT lpRect, BOOL bTextOnly);
	COLORREF	GetItemTextBkColor(HTREEITEM hItem);
	COLORREF	GetItemTextColor(HTREEITEM hItem);
	XHTMLTREEDATA * GetItemDataStruct(HTREEITEM hItem);
	CString		GetItemText(HTREEITEM hItem, BOOL bStripHtml = FALSE) const;
	HTREEITEM	GetLastItem(HTREEITEM hItem);
	LOGFONT *	GetLogfont() { return &m_lf; }
	HTREEITEM	GetNextCheckedItem(HTREEITEM hItem);
	HTREEITEM	GetNextItem(HTREEITEM hItem);
				// do not hide CTreeCtrl version
	HTREEITEM	GetNextItem(HTREEITEM hItem, UINT nCode)	
				{ return CTreeCtrl::GetNextItem(hItem, nCode); }
	HTREEITEM	GetPrevCheckedItem(HTREEITEM hItem);
	HTREEITEM	GetPrevItem(HTREEITEM hItem);
	BOOL		GetReadOnly() { return m_bReadOnly; }
	BOOL		GetSelectFollowsCheck() { return m_bSelectFollowsCheck; }
	COLORREF	GetSeparatorColor() { return m_crSeparator; }		//+++1.6
	int			GetSeparatorCount(HTREEITEM hItem);
	BOOL		GetSmartCheckBox() { return m_bSmartCheck; }
	int			GetStateImage(HTREEITEM hItem);
	BOOL		GetStripHtml() { return m_bStripHtml; }
	COLORREF	GetTextColor() { return m_crCustomWindowText; }
#ifdef XHTMLTOOLTIPS
	CPPToolTip * GetToolTips() { return m_pToolTip; }
#else
	CToolTipCtrl * GetToolTips() { return m_pToolTip.get(); }
#endif // XHTMLTOOLTIPS
	BOOL		GetUseLogfont() { return m_bLogFont; }
	int			GetXmlCount() { return m_nXmlCount; }

	BOOL		IsChecked(HTREEITEM hItem) { return GetCheck(hItem); }
	BOOL		IsChildNodeOf(HTREEITEM hitem, HTREEITEM hitemSuspectedParent);
	BOOL		IsEnabled(HTREEITEM hItem);
	BOOL		IsExpanded(HTREEITEM hItem);
	HTREEITEM	IsOverItem(LPPOINT lpPoint = NULL);
	BOOL		IsSelected(HTREEITEM hItem);
	BOOL		IsSeparator(HTREEITEM hItem);

	COLORREF	SetBkColor(COLORREF rgb);
	void		SetBit(DWORD& bits, DWORD bitmask, BOOL value)
				{
					if (value)
						bits |= bitmask;
					else
						bits &= ~bitmask;
				}
	CImageTreeCtrl&	SetCheck(HTREEITEM hItem, BOOL fCheck = TRUE);
	CImageTreeCtrl&	SetCheckChildren(HTREEITEM hItem, BOOL fCheck);
	CImageTreeCtrl&	SetDefaultTipWidth(int nDefaultTipWidth)
				{ m_nDefaultTipWidth = nDefaultTipWidth; return *this; }
	CImageTreeCtrl&	SetDisplayToolTips(BOOL bFlag) 
				{ m_bToolTip = bFlag; return *this; }
	CImageTreeCtrl&	SetDragOps(DWORD dwOps)
				{ m_dwDragOps = dwOps; return *this; }
	CImageTreeCtrl&	SetDropCursors(UINT nNoDropCursor, UINT nDropCopyCursor, UINT nDropMoveCursor) 
				{ 
					m_nNoDropCursor = nNoDropCursor; 
					m_nDropCopyCursor = nDropCopyCursor; 
					m_nDropMoveCursor = nDropMoveCursor; 
					return *this; 
				}
	CImageTreeCtrl&	SetHasCheckBoxes(BOOL bHasCheckBoxes) 
				{ m_bCheckBoxes = bHasCheckBoxes; return *this; }
	CImageTreeCtrl&	SetHtml(BOOL bFlag) 
				{ m_bHtml = bFlag; return *this; }
	CImageTreeCtrl&	SetImages(BOOL bFlag) 
				{ m_bImages = bFlag; return *this; }
	COLORREF	SetInsertMarkColor(COLORREF rgb);
	BOOL		SetItemBold(HTREEITEM hItem, BOOL bBold);
	CImageTreeCtrl&	SetItemNote(HTREEITEM hItem, LPCTSTR lpszNote, int nTipWidth = 0);
	BOOL		SetItemText(HTREEITEM hItem, LPCTSTR lpszItem);
	COLORREF	SetItemTextBkColor(HTREEITEM hItem, COLORREF rgb);
	COLORREF	SetItemTextColor(HTREEITEM hItem, COLORREF rgb);
	CImageTreeCtrl&	SetItemStateChildren(HTREEITEM hItem, BOOL fCheck);
	CImageTreeCtrl&	SetLogfont(LOGFONT * pLogFont);
	CImageTreeCtrl&	SetReadOnly(BOOL bReadOnly)
				{ m_bReadOnly = bReadOnly; return *this; }
	CImageTreeCtrl&	SetSelectFollowsCheck(BOOL bFlag) 
				{ m_bSelectFollowsCheck = bFlag; return *this; }
	CImageTreeCtrl&	SetSeparatorColor(COLORREF rgb);		//+++1.6
	CImageTreeCtrl&	SetSmartCheckBox(BOOL bFlag) 
				{ m_bSmartCheck = bFlag; return *this; }
	CImageTreeCtrl&	SetStripHtml(BOOL bFlag) 
				{ m_bStripHtml = bFlag; return *this; }
	COLORREF	SetTextColor(COLORREF rgb);
	BOOL CreateCheckboxImageList(CDC *pDC, 
							 CImageList& imagelist, 
							 int nSize, 
							 COLORREF crBackground);

#ifdef XHTMLTOOLTIPS
	CPPToolTip * SetToolTips(CPPToolTip * pWndTip) 
				{ CPPToolTip * old = m_pToolTip; m_pToolTip = pWndTip; return old; }
#else
	// escdream 2022.07.08 - 실행시 내용 확인
	//CToolTipCtrl * SetToolTips(CToolTipCtrl * pWndTip) 
	//			{ CToolTipCtrl * old = m_pToolTip.get(); m_pToolTip.get() = pWndTip; return old; }
#endif // XHTMLTOOLTIPS
	CImageTreeCtrl&	SetUseLogfont(BOOL bFlag) 
				{ m_bLogFont = bFlag; return *this; }

//=============================================================================
// Operations
//=============================================================================
public:
	void		CheckAll(BOOL bCheck);
	void		CollapseBranch(HTREEITEM hItem);
	BOOL		CreateCheckboxImages();
	void		CreateToolTipsForTree();
	BOOL		DeleteItem(HTREEITEM hItem);
	BOOL		EnableBranch(HTREEITEM hItem, BOOL bEnabled);
	BOOL		EnableItem(HTREEITEM hItem, BOOL bEnabled);
	BOOL		EnableWindow(BOOL bEnable = TRUE);
	BOOL		Expand(HTREEITEM hItem, UINT nCode);
	void		ExpandBranch(HTREEITEM hItem);
	void		ExpandAll();
	void		CollapseAll();
	BOOL		DeleteAllItems();
	virtual HTREEITEM FindItem(CString &sSearch, 
							   BOOL bCaseSensitive = FALSE, 
							   BOOL bDownDir = TRUE,
							   BOOL bWholeWord = FALSE, 
							   BOOL bWrap = TRUE,
							   HTREEITEM hItem = NULL);
	int			IncrementChildren(HTREEITEM hItem, int n = 1);
	int			IncrementSeparators(HTREEITEM hItem, int n = 1);
	CImageTreeCtrl&	Initialize(BOOL bCheckBoxes = FALSE, BOOL bToolTip = FALSE);
	HTREEITEM	InsertItem(LPTVINSERTSTRUCT lpInsertStruct, XHTMLTREEDATA * pData = NULL);
	HTREEITEM	InsertItem(UINT nMask, LPCTSTR lpszItem, int nImage, 
					int nSelectedImage, UINT nState, UINT nStateMask, 
					LPARAM lParam, HTREEITEM hParent, HTREEITEM hInsertAfter);
	HTREEITEM	InsertItem(LPCTSTR lpszItem, HTREEITEM hParent = TVI_ROOT, 
					HTREEITEM hInsertAfter = TVI_LAST );
	HTREEITEM	InsertItem(LPCTSTR lpszItem, int nImage, int nSelectedImage, 
					HTREEITEM hParent = TVI_ROOT, 
					HTREEITEM hInsertAfter = TVI_LAST);
	HTREEITEM	InsertSeparator(HTREEITEM hItem);
	void		RedrawItem(HTREEITEM hItem);
	BOOL		SelectItem(HTREEITEM hItem);
	HCURSOR		SetCursor(HCURSOR hCursor);

//=============================================================================
// XML functions
//=============================================================================
#ifdef XHTMLXML

public:
	enum ConvertAction
	{
		NoConvertAction = 0, ConvertToUnicode, ConvertToAnsi
	};

	BOOL		ConvertBuffer(const BYTE * inbuf, 
							  DWORD inlen,				// in bytes
							  BYTE ** outbuf,
							  DWORD& outlen,			// in bytes
							  ConvertAction eConvertAction = NoConvertAction);
	CString		GetXmlText(HTREEITEM hItem, LPCTSTR lpszElem);
	BOOL		LoadXmlFromBuffer(const BYTE * pBuf, DWORD len, ConvertAction eConvertAction);
	BOOL		LoadXmlFromFile(LPCTSTR lpszFile, ConvertAction eConvertAction);
	BOOL		LoadXmlFromResource(HINSTANCE hInstance, LPCTSTR lpszResId, 
					LPCTSTR lpszResType, ConvertAction eConvertAction);
	BOOL		SaveXml(HTREEITEM hItem, LPCTSTR lpszFileName, BOOL bSaveAsUTF16);

protected:
	HTREEITEM	InsertXmlItem(CXmlElement *pElement, HTREEITEM hParent);
	BOOL		LoadXml(CXmlDocument& xml, CXmlElement *pElement, 
					HTREEITEM hParent, int& nCount);

#endif // XHTMLXML

//=============================================================================
// Overrides
//=============================================================================
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CImageTreeCtrl)
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);

protected:
	virtual void PreSubclassWindow();
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

//=============================================================================
// Implementation
//=============================================================================
protected:
	CMap< HTREEITEM, HTREEITEM, XHTMLTREEDATA *, XHTMLTREEDATA *& > m_DataMap;
											// maps HTREEITEM ==> XHTMLTREEDATA
	BOOL			m_bReadOnly;			// TRUE = tree is read-only 
											// (checkboxes disabled)
	BOOL			m_bSmartCheck;			// TRUE = Smart Checkboxes enabled
	BOOL			m_bCheckBoxes;			// TRUE = checkboxes enabled
	BOOL			m_bSelectFollowsCheck;	// TRUE = item will be selected, 
											// after user clicks on checkbox
	BOOL			m_bHtml;
	BOOL			m_bStripHtml;
	BOOL			m_bLogFont;
	BOOL			m_bToolTip;
	BOOL			m_bImages;				// TRUE = allow images
	BOOL			m_bDestroyingTree;
	BOOL			m_bFirstTime;
	BOOL			m_bDragging;
	BOOL			m_bAutoScroll;			// TRUE = tree will automatically 
											// scroll when dragging
	HTREEITEM		m_hAnchorItem;
	HTREEITEM		m_hHotItem;
	HTREEITEM		m_hPreviousItem;
	HTREEITEM		m_hItemButtonDown;
	HTREEITEM		m_hPreviousDropItem;
	DWORD			m_dwDropHoverTime;		// number of ticks over a drop item
	int				m_nPadding;				// horizontal padding in pixels
	int				m_nImageHeight;
	int				m_nToolCount;			// no. of items added to tooltip 
											// control
	int				m_nDefaultTipWidth;
	int				m_nXmlCount;			// count of items loaded from XML 
											// file
	int				m_nDeleted;
	int				m_nDeletedChecked;
	CImageList		m_StateImage;
	LOGFONT			m_lf;
	int				m_nHorzPos;				// initial horz scroll position - saved
											// before in-place edit begins
	UINT			m_nScrollTime;			// used for scrolling while dragging
	DWORD			m_dwDragOps;			// drag features
	UINT			m_nNoDropCursor;		// resource ID for no-drop cursor
	UINT			m_nDropCopyCursor;		// resource ID for drop (copy) cursor
	UINT			m_nDropMoveCursor;		// resource ID for drop (move) cursor
	HCURSOR			m_hNoDropCursor;		// no-drop cursor handle
	HCURSOR			m_hDropCopyCursor;		// drop (copy) cursor handle
	HCURSOR			m_hDropMoveCursor;		// drop (move) cursor handle
	HCURSOR			m_hPreviousCursor;
	HCURSOR			m_hCurrentCursor;

#ifdef XHTMLTOOLTIPS
	CPPToolTip *	m_pToolTip;
#else
	std::unique_ptr< CToolTipCtrl > m_pToolTip{};
#endif // XHTMLTOOLTIPS

	COLORREF		m_crCustomWindow;
	COLORREF		m_crCustomWindowText;
	COLORREF		m_crWindow;
	COLORREF		m_crWindowText;
	COLORREF		m_crAnchorText;
	COLORREF		m_crGrayText;
	COLORREF		m_crHighlight;
	COLORREF		m_crHighlightText;
	COLORREF		m_crInsertMark;
	COLORREF		m_crSeparator;

	void		AdjustEditRect();
	void		AutoScroll(HTREEITEM hItem);
	void		DeleteBranch(HTREEITEM hItem);

	void		DeleteMap();
	int			DrawItemText(CDC *pDC, HTREEITEM hItem, LPCTSTR lpszText, 
					COLORREF crText, COLORREF crTextBackground, 
					COLORREF crBackground, const CRect& rect);
#ifdef XHTMLHTML
	int			DrawItemTextHtml(CDC *pDC, HTREEITEM hItem, LPCTSTR lpszText, 
					COLORREF crText, COLORREF crTextBackground, 
					COLORREF crBackground, COLORREF crAnchorText,
					CRect& rect);
#endif // XHTMLHTML
	int			DrawSeparator(CDC *pDC, HTREEITEM hItem,
					COLORREF crText, COLORREF crBackground, const CRect& rect);
	void		EndDragScroll();
	int			GetNormalImageWidth(HTREEITEM hItem);
	BOOL		IsBadRect(const CRect& r)
				{ return (r.IsRectEmpty() || (r.Height() <= 0) || (r.Width() <= 0)); }
	virtual BOOL IsFindValid(HTREEITEM);
	BOOL		IsLeftButtonUp();
	BOOL		PreDisplayToolTip(BOOL bAlwaysRemoveHtml, CString& strToolTip);
	LRESULT		SendRegisteredMessage(UINT nMessage, HTREEITEM hItem, LPARAM lParam = 0);
	int			SetCheckParent(HTREEITEM hItem, int nCount);
	void		SetColors();
	void		SetHotItem(HTREEITEM hItem, UINT nFlags);

#ifdef XHTMLDRAGDROP
	HCURSOR		GetDragCursor();
	BOOL		IsCtrlDown();
	BOOL		IsDragCopy();
	HTREEITEM	MoveBranch(HTREEITEM hBranch, 
						   HTREEITEM hNewParent, 
						   HTREEITEM hAfter = TVI_LAST);
	HTREEITEM	MoveItem(HTREEITEM hItem, 
						 HTREEITEM hNewParent, 
						 HTREEITEM hAfter = TVI_LAST);
	void		SetDragCursor();
	BOOL		StartMoveBranch(HTREEITEM hItem, 
								HTREEITEM hNewParent, 
								HTREEITEM hAfter = TVI_LAST);
#endif // XHTMLDRAGDROP


//=============================================================================
// Generated message map functions
//=============================================================================
protected:
	//{{AFX_MSG(CImageTreeCtrl)
	afx_msg BOOL OnBeginlabeledit(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg BOOL OnClick(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnCustomDraw(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg BOOL OnDblclk(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDestroy();
	afx_msg BOOL OnEndlabeledit(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnSysColorChange();
	afx_msg void OnTimer(UINT nIDEvent);
	//}}AFX_MSG

#ifdef XHTMLDRAGDROP
	afx_msg void OnBegindrag(NMHDR* pNMHDR, LRESULT* pResult);
#endif // XHTMLDRAGDROP

#ifdef XHTMLTOOLTIPS
	afx_msg void OnDisplayTooltip(NMHDR * pNMHDR, LRESULT * result);
#else
	virtual afx_msg BOOL OnToolTipText(UINT id, NMHDR * pNMHDR, LRESULT * pResult);
#endif // XHTMLTOOLTIPS

	afx_msg BOOL OnSelchanged(NMHDR * pNMHDR, LRESULT * pResult);
	afx_msg BOOL OnSelchanging(NMHDR * pNMHDR, LRESULT * pResult);

	DECLARE_MESSAGE_MAP()
};

