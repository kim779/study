// Copyright (c) Iuri Apollonio 1998
// Use & modify as you want & need, and leave those 3 lines.
// http://www.codeguru.com

#if !defined(AFX_STGFXOUTBARCTRL_H__28FA2CA4_11B7_11D2_8437_0000B43382FE__INCLUDED_)
#define AFX_STGFXOUTBARCTRL_H__28FA2CA4_11B7_11D2_8437_0000B43382FE__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// GfxOutBarCtrl.h : header file
//

#include "../Common_ST/IGfxOutBarCtrl.h"

/////////////////////////////////////////////////////////////////////////////
// CSTGfxOutBarCtrl window

#include <afxcoll.h>

#define WM_OUTBAR_NOTIFY		WM_USER + 1
#define NM_OB_ITEMCLICK			1
#define NM_OB_ONLABELENDEDIT	2
#define NM_OB_ONGROUPENDEDIT	3
#define NM_OB_DRAGITEM			4
#define NM_FOLDERCHANGE			5

struct OUTBAR_INFO 
{
	int				index;
	const char *	cText;
	int				iDragFrom;
	int				iDragTo;
};

class CSTGfxOutBarCtrl : public CWnd
{
// Construction
	DECLARE_DYNCREATE(CSTGfxOutBarCtrl)
public:
	CSTGfxOutBarCtrl();


	enum { ircIcon = 1, ircLabel = 2, ircAll = 3 };

// Attributes
public:
	COLORREF	crBackGroundColor, crBackGroundColor1;
	COLORREF	crTextColor;
	COLORREF	cr3dFace, cr3dUnFace, /*crLightBorder,*/ crHilightBorder, crShadowBorder, crDkShadowBorder;
	int			iFolderHeight;

	int			xSmallIconLabelOffset, yLargeIconLabelOffset;
	int			ySmallIconSpacing, yLargeIconSpacing;

	int			xLeftMargin, yTopMargin;
	bool		bUpArrow, bDownArrow, bUpPressed, bDownPressed;
	CRect		rcUpArrow, rcDownArrow;
	bool		bLooping;

	int			iHitInternal1, iHitInternal2;

	long		lAnimationTickCount;

	int			iLastSel, iSelAnimTiming;
	int			iSelAnimCount;


	DWORD		dwFlags;

	CPtrArray	arFolder;

	int			iLastFolderHighlighted;
	int			iLastSelectedFolder;
	int			iFirstItem;

	int			iLastItemHighlighted;
	bool		bPressedHighlight;

	int			iLastDragItemDraw, iLastDragItemDrawType;

	class CBarItem
	{
	public:
		CBarItem(const char * name, const int image, DWORD exData);
		virtual  ~CBarItem();
		int				iImageIndex;
		char *			cItem;
		DWORD			dwData;
	};

	class CBarFolder
	{
	public:
		CBarFolder(const char * name, DWORD exData);
		virtual  ~CBarFolder();
		int GetItemCount();
		int InsertItem(int index, const char * text, const int image, const DWORD exData);
		char *			cName;
		DWORD			dwData;
		CImageList *	pLargeImageList;
		CImageList *	pSmallImageList;
		CPtrArray		arItems;
		CWnd *			pChild;
	};

	int iSelFolder;

	CImageList *	pLargeImageList;
	CImageList *	pSmallImageList;

	HCURSOR			hHandCursor;
	HCURSOR			hDragCursor;
	HCURSOR			hNoDragCursor;

	CPen *			pBlackPen;

	CFont			m_Font;

	//COLORREF m_crBk;
	//CBrush	m_brBkColor;

// Operations
public:
	enum { htNothing = -1, htFolder, htItem, htUpScroll, htDownScroll};

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSTGfxOutBarCtrl)
	protected:
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

// Implementation
public:
	void DeleteAllFolder();
	void DrawAnimItem(const int xoffset, const int yoffset, const int index);
	void SetAnimSelHighlight(const int iTime);
	DWORD GetFolderData(int iFolder = -1);
	CWnd * GetFolderChild(int iFolder = -1);
	int AddFolderBar(const char * pFolder, CWnd * pSon, const DWORD exData = 0);
	CString GetItemText(const int index);
	void SetAnimationTickCount(const long value) { lAnimationTickCount = value; };
	long GetAnimationTickCount() { return lAnimationTickCount; };

	void AnimateFolderScroll(const int iFrom, const int iTo);
	int GetDragItemRect(const int index, CRect &rect);
	void DrawDragArrow(CDC * pDC, const int iFrom, const int iTo);
	void SetItemImage(const int index, const int iImage);
	void SetItemData(const int index, const DWORD dwData);
	int  GetItemImage(const int index) const;
	DWORD GetItemData(const int index) const;
	bool IsValidItem(const int index) const;
	void RemoveItem(const int index);
	void SetItemText(const int index, const char * text);
//	void StartItemEdit(const int index);
	void SetFolderText(const int index, const char * text);
//	void StartGroupEdit(const int index);
	void GetLabelRect(const int iFolder, const int iIndex, CRect &rect);
	void GetIconRect(const int iFolder, const int iIndex, CRect &rect);
	void HighlightItem(const int index, const bool bPressed = false);
	void GetVisibleRange(const int iFolder, int &first, int &last);
	void DrawItem(CDC * pDC, const int iFolder, CRect rc, const int index, const bool bOnlyImage = false);
	CImageList * GetFolderImageList(const int index, const bool bSmall) const;
	CSize GetItemSize(const int iFolder, const int index, const int type);
	void PaintItems(CDC * pDC, const int iFolder, CRect rc);
	CImageList * GetImageList(int nImageList);
	CImageList * SetFolderImageList(const int folder, CImageList * pImageList, int nImageList);
	CImageList * SetImageList(CImageList * pImageList, int nImageList);
	int GetCountPerPage() const;
	void RemoveFolder(const int index);
	int GetSelFolder() const;
	int GetFolderCount() const;
	void SetSelFolder(const int index);
	int GetItemCount() const;
	int InsertItem(const int folder, const int index, const char * text, const int image = -1, const DWORD exData = 0);
	void HighlightFolder(const int index);
	int HitTestEx(const CPoint &point, int &index);
	void GetInsideRect(CRect &rect) const;
	int AddFolder(const char * cFolderName, const DWORD exData);
	void GetItemRect(const int iFolder, const int iIndex, CRect &rect);
	bool GetFolderRect(const int iIndex, CRect &rect) const;
	void ModifyFlag(const DWORD &dwRemove, const DWORD &dwAdd, const UINT redraw = 0);
	DWORD GetFlag() const;
	void SetSmallIconView(const bool bSet);
	bool IsSmallIconView() const;
	BOOL Create(DWORD dwStyle, const RECT& rect, CWnd * pParentWnd, UINT nID, const DWORD dwFlag = fDragItems|fEditGroups|fEditItems|fRemoveGroups|fRemoveItems|fAddGroups|fAnimation|fSelHighlight);
	virtual ~CSTGfxOutBarCtrl();

	//2006.09.25 by LYH Folder Image Ã³¸®
	CImageList *	m_pFolderImageList;
	int m_nNormal;
	int m_nDown;
	int m_nOver;
	void SetFolderImageList(CImageList * pImageList, int nNormal, int nDown, int nOver);
	int			iGap;

	//end

	// Generated message map functions
protected:
	void DrawFolder(CDC * pDC, const int iIdx, CRect rect, const bool bSelected);
	//{{AFX_MSG(CSTGfxOutBarCtrl)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnPaint();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	//}}AFX_MSG
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
//	afx_msg void OnGfxLargeicon();
//	afx_msg void OnUpdateGfxLargeicon(CCmdUI* pCmdUI);
//	afx_msg void OnGfxSmallicon();
//	afx_msg void OnUpdateGfxSmallicon(CCmdUI* pCmdUI);
//	afx_msg void OnGfxRemoveitem();
//	afx_msg void OnUpdateGfxRemoveitem(CCmdUI* pCmdUI);
//	afx_msg void OnGfxRenameitem();
//	afx_msg void OnUpdateGfxRenameitem(CCmdUI* pCmdUI);
//	afx_msg long OnEndLabelEdit(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
};

class CImpl_IGfxOutBarCtrl : public IGfxOutBarCtrl2
{
public:
	CSTGfxOutBarCtrl m_wndOutBar;

	CImpl_IGfxOutBarCtrl();

public:
	STDMETHOD_(CWnd*, GetCoutBarCtrl)()  ;
	STDMETHOD_(BOOL, CreateOutBar)(DWORD dwStyle, const RECT & rect, CWnd * pParentWnd, UINT nID, const DWORD dwFlag) ;
	STDMETHOD_(void, Setcr3dFace)(COLORREF) ;
	STDMETHOD_(void, Setcr3dUnFace)(COLORREF) ;
	STDMETHOD_(CImageList*, SetImageList)(CImageList* pImageList, int nImageList) ;
	STDMETHOD_(void, SetFolderImageList)(CImageList * pImageList, int nNormal, int nDown, int nOver);
	STDMETHOD_(int, AddFolderBar)(LPCSTR szFolderName, CWnd* pChild);
	STDMETHOD_(void, SetSelFolder)(int nIndex);
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STGFXOUTBARCTRL_H__28FA2CA4_11B7_11D2_8437_0000B43382FE__INCLUDED_)
