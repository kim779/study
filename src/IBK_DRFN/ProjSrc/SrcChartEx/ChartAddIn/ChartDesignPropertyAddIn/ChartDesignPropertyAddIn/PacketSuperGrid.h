#if !defined(AFX_PACKETSUPERGRID_H__09B964C4_953D_11D2_9B04_002018026B76__INCLUDED_)
#define AFX_PACKETSUPERGRID_H__09B964C4_953D_11D2_9B04_002018026B76__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PacketSuperGrid.h : header file
//

#include "resource.h"
#include "SuperGridCtrl.h"
#include "PacketUserDefine.h"
////////////////////////////////
/////////////////////////////////////////////
// CPacketSuperGrid window

class CPacketSuperGrid : public CSuperGridCtrl
{
// Construction
public:
	CPacketSuperGrid();
	
// Attributes
public:
// Operations
public:
	BOOL m_bDrag;
	//HOWTO:
	void InitializeGrid(void);
	void HowToInsertItemsAfterTheGridHasBeenInitialized(const int nIndex, const int nCheck, const CString& str,const CString& szField, const CString& szLength, const CString& szType, const CString& szReal);
	CImageList *CreateDragImageEx(int nItem);
	// Overrides
	void OnControlLButtonDown(UINT nFlags, CPoint point, LVHITTESTINFO& ht);	
	void OnUpdateListViewItem(CTreeItem* lpItem, LV_ITEM *plvItem);
	CItemInfo* CopyData(CItemInfo* lpSrc);
	int GetIcon(const CTreeItem* pItem);
	COLORREF GetCellRGB(void);
	BOOL OnItemExpanding(CTreeItem *pItem, int iItem);
	BOOL OnItemExpanded(CTreeItem* pItem, int iItem);
	BOOL OnCollapsing(CTreeItem *pItem);
	BOOL OnItemCollapsed(CTreeItem *pItem);
	BOOL OnDeleteItem(CTreeItem* pItem, int nIndex);
	BOOL OnVkReturn(void);
	BOOL OnItemLButtonDown(LVHITTESTINFO& ht);
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPacketSuperGrid)
	//}}AFX_VIRTUAL

// Implementation
public:
	void AddItem(const int nCheck, const CString& szpacket, const CString& szField, const CString& szLength, const CString& szType, const CString& szReal);
	virtual ~CPacketSuperGrid();

protected:
	CImageList m_image;//Must provide an imagelist
	//positions and creates/initalize a combobox control
	CComboBox* ShowList(int nItem, int nCol, CStringList *lstItems);
	//helper function called from ShowList...calcs the lists max horz extent
	int CalcHorzExtent(CWnd* pWnd, CStringList *pList);
	// Generated message map functions
protected:
	//{{AFX_MSG(CPacketSuperGrid)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDblclk(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
private: 
	void SetUserDefineRootInit();
	void SetPacketRootInit();
	
	int GetItemIndex(const CString& szPacketName);

	void SetColumnHeader();
	void SetRootItem(const CString& szItemName, CItemInfo* lpItemInfo);
	void SetSubItem(const int nItemIndex, const CString& szPacketName, CItemInfo* lpItemInfo);
	void SetThirdSubItem(const int nItemIndex, const CString& szDefaultItemofCombo, const CString& szPacketName, CItemInfo *lpItemInfo);
	void SetThirdEXCHANGERATEItem(const CString& szDefaultItemofCombo, CItemInfo* lpItemInfo);
	void SetThirdSTOCKMULTIPLEItem(const CString& szDefaultItemofCombo, CItemInfo* lpItemInfo);
	void SetThirdVOLUMEItem(const CString& szPacketName, const CString& szDefaultItemofCombo, CItemInfo* lpItemInfo);
	void SetThirdPRICEItem(const CString& szDefaultItemofCombo, CItemInfo* lpItemInfo);
	void SetThirdTEXTItem(const CString& szDefaultItemofCombo, CItemInfo* lpItemInfo);
	void SetThirdDATEItem(const CString& szDefaultItemofCombo, CItemInfo* lpItemInfo);
	void SetFourSubItem(const CString& szReal, CItemInfo* lpItemInfo);

	enum DATATYPE
	{
		DATE,
		TEXT,
		PRICE,
		VOLUME,
		STOCKMULTIPLE,
		EXCHANGERATE,
		ANOTHER
	};
	CPacketSuperGrid::DATATYPE GetDataType(const int nDataType);

public:
	enum PACKETITEM
	{
		SECONDDATE_ITEM,		// (2007/3/14 - Seung-Won, Bae) Dummy Packet for Date FID in Koscom
		INFODATE_ITEM,
		TEXT_ITEM,
		STARTPRICE_ITEM,
		TOPPRICE_ITEM,
		BOTTOMPRICE_ITEM,
		ENDPRICE_ITEM,
		VOLUME_ITEM,
		ACCUMULATEVOLUME_ITEM,
		ISRAK_ITEM,
		STOCKMULTIPLE_ITEM,
		EXCHANGERATE_ITEM,
		TOTAL_ITEMCOUNT_ITEM,
		UP_ITEMCOUNT_ITEM,
		DOWN_ITEMCOUNT_ITEM,
		ADL_ITEM,
		MOBV_ITEM,
		UP_ITEMVOLUME_ITEM,
		DOWN_ITEMVOLUME_ITEM,
		LEAD_ITEMVOLUME_ITEM,
		NEW_TOP_ITEMCOUNT_ITEM,
		NEW_BOTTOM_ITEMCOUNT_ITEM,
		STOCK_RISE_LIST_ITEM,
		TRADE_FORMULATE_ITEM
	};

// (2008/1/14 - Seung-Won, Bae) for MultiLanguage.
protected:
	HWND	m_hOcxWnd;
public:
	void	SetOcxHwnd( HWND p_hOcxWnd)	{	m_hOcxWnd = p_hOcxWnd;	}
//	HWND	GetOcxHwnd( void)			{	return ( m_pOcxWnd ? m_pOcxWnd->GetSafeHwnd() : NULL);	}
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PACKETSUPERGRID_H__09B964C4_953D_11D2_9B04_002018026B76__INCLUDED_)
