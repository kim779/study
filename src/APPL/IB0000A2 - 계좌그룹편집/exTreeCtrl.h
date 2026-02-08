#pragma once

enum { imgTROOT = 0, imgTGROUP, imgTGRSEL};

class CExTreeCtrl : public CTreeCtrl
{
public:
	CExTreeCtrl();
	virtual ~CExTreeCtrl();

	//{{AFX_VIRTUAL(CExTreeCtrl)
	//}}AFX_VIRTUAL
protected:
	//{{AFX_MSG(CExTreeCtrl)
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnBeginrdrag(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
	void	Init(CWnd *pParent, CFont *pFont);
	void	SetDragEnabled(bool bDrag = true) { m_bDrag = bDrag; }
private:
	HTREEITEM	copyItem(HTREEITEM hItem, HTREEITEM htiNewParent, HTREEITEM htiAfter = TVI_LAST);

private:
	CWnd		*m_pParent;
	CFont		*m_pFont;
	std::unique_ptr<CImageList> m_ImageList;

	bool		m_bDrag;
	
	HTREEITEM	m_hItemDrag;
	HTREEITEM	m_hItemDrop;
	bool		m_bDragging;

};

