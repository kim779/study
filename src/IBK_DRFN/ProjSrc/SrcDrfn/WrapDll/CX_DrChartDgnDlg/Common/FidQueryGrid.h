// FidQueryGrid.h: interface for the CFidQueryGrid class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_FIDQUERYGRID_H__9C2E15C2_6A28_41FD_9EDE_332BF2A9E924__INCLUDED_)
#define AFX_FIDQUERYGRID_H__9C2E15C2_6A28_41FD_9EDE_332BF2A9E924__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "../SuperGrid/SuperGridCtrl.h"

class CFidQueryGrid : public CSuperGridCtrl  
{
public:
	CFidQueryGrid();
	virtual ~CFidQueryGrid();

	void Initialize();
	void SetInitData();
	CSuperGridCtrl::CTreeItem* InsertNewNode(CSuperGridCtrl::CTreeItem *pParentItem, BOOL bParant=TRUE);
	CSuperGridCtrl::CTreeItem* InsertNewItem(CSuperGridCtrl::CTreeItem *pParentItem, BOOL bParant=TRUE);
	CSuperGridCtrl::CTreeItem *InsertNewSeparator(CSuperGridCtrl::CTreeItem *pParentItem, BOOL bParant=TRUE);
	CSuperGridCtrl::CTreeItem *GetSelectedNodeItem();
	CSuperGridCtrl::CTreeItem *GetSelectedTreeItem();
	COLORREF GetCellRGB();
	int GetIcon(const CTreeItem* pItem);
	BOOL OnDeleteItem(CTreeItem* pItem, int nIndex);

	void ChangeRootName(CString strName, COLORREF color);

private:
	int m_nColCount;

	CImageList		m_ImageList;
	CStringArray	m_ArrBool;
	CDWordArray		m_ArrBoolIndex;
};

#endif // !defined(AFX_FIDQUERYGRID_H__9C2E15C2_6A28_41FD_9EDE_332BF2A9E924__INCLUDED_)
