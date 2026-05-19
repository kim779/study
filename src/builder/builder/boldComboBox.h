#pragma once

struct ITEMDATA
{
	BOOL	bold;
	int	image;
};

// CBoldComboBox

class CBoldComboBox : public CComboBox
{
	DECLARE_DYNAMIC(CBoldComboBox)

public:
	CBoldComboBox();
	virtual ~CBoldComboBox();

	bool m_bflag;
protected:
	CImageList* m_pImageList;
	CMap < ULONG_PTR,ULONG_PTR,ITEMDATA,ITEMDATA >	m_items;

	DECLARE_MESSAGE_MAP()

public:
	bool	IsItemBold(int iItem);
	void	ResetItemInfo();
	void	SetItemImage(int iItem, int iImage);
	void	SetItemImage2(int iItem, int iImage);
	void	SetItemBold(int iItem,bool bold);
	void	SetImageList(CImageList* pImageList) { m_pImageList = CImageList::FromHandle(pImageList->GetSafeHandle()); }
	void    SetColor(bool bflag){ m_bflag = bflag; }
	int     GetItemImage(int iItem);

	virtual void DrawItem(LPDRAWITEMSTRUCT lpDS);
	virtual void MeasureItem(LPMEASUREITEMSTRUCT lpMS);
};


