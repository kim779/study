/**
	@file		FidBlock.h
	@brief		FID Data.
	@author		sashia
	@date		2009/09/23
*/

#if !defined(AFX_FIDBLOCK_H__16F42BCA_D645_4B9B_978C_E7AC60B1F611__INCLUDED_)
#define AFX_FIDBLOCK_H__16F42BCA_D645_4B9B_978C_E7AC60B1F611__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#pragma	pack(1)


/**
	@class		CFidItem
	@brief		FID 데이터형식
	@author		sashia
	@date		2009/09/23
*/
class CFidItem : public CObject
{
	DECLARE_DYNAMIC(CFidItem);

public:
	CFidItem();
	CFidItem(const CFidItem& obj)	{ *this = obj;	};
	CFidItem(CFidItem *pItem);
	virtual ~CFidItem();

protected:
	CString		m_strValue;		//	value.
	int			m_nSize;		//	Data Size(byte 기준)
	CString		m_strItemID;	//	데이터 고유 아이디.
	CString		m_strRealID;	//	실시간 데이터 ID.
	CString		m_strChartID;	//	차트에 그려질 ID.

	BOOL		m_bUseBuf;	//	데이터전체 버퍼 얻을때 사용할지 여부.
public:
	void			SetSize(int nSize)	{ m_nSize = nSize;	};
	int				GetSize()			{ return m_nSize;	};
	CString			GetItemID()					{ return m_strItemID;	};
	void			SetItemID(CString strID)	{ m_strItemID = strID;	};
	CString			GetRealID()					{ return m_strRealID;	};
	void			SetRealID(CString strID)	{ m_strRealID = strID;	};
	CString			GetChartID()					{ return m_strChartID;	};
	void			SetChartID(CString strID)	{ m_strChartID = strID;	};

	void			SetUseBuf(BOOL bUse)	{ m_bUseBuf = bUse;	};
	BOOL			GetUseBuf()				{ return m_bUseBuf;	};

public:
	virtual void	Copy(const CFidItem& item);
	virtual CFidItem& operator = (const CFidItem& item);

	virtual void	RemoveAllData();

	virtual void	SetValue(CString strData, int nIndex = -1);
	virtual CString	GetValue(int nIndex = -1);

public:
	void	Serialize(CArchive &ar, int nVer);
};

typedef	CArray<CFidItem*, CFidItem*>					CArrFidItem;
typedef	CMap<CString, LPCTSTR, CFidItem*, CFidItem*>	CMapFidItem;

/**
	@class		CFidItemArr
	@brief		Array 데이터형식.
	@author		sashia
	@date		2009/09/23
*/
class CFidItemArr : public CFidItem
{
	DECLARE_DYNAMIC(CFidItemArr);

public:
	CFidItemArr();
	CFidItemArr(const CFidItemArr& obj)	{ *this = obj;	};
	CFidItemArr(CFidItemArr *pItem);
	virtual ~CFidItemArr();


protected:
	CStringArray	m_arrValue;	//	Array Value Pointer.

public:
	virtual void	Copy(const CFidItemArr& item);
	virtual CFidItemArr& operator = (const CFidItemArr& item);

	virtual void	RemoveAllData();

	virtual void	SetValue(CString strData, int nIndex = -1);
	virtual CString	GetValue(int nIndex = -1);

public:
	int				GetValueCount()	{ return m_arrValue.GetSize();	};
};

/**
	@class		CFidBlock
	@brief		Block 단위 클래스.
	@author		sashia
	@date		2009/09/23
*/
class CFidBlock : public CObject
{
	DECLARE_DYNAMIC(CFidBlock);

public:
	CFidBlock();
	CFidBlock(const CFidBlock& obj)	{ *this = obj;	};
	CFidBlock(CFidBlock *pItem);
	virtual ~CFidBlock();

	void			RemoveAll();
	void			Serialize(CArchive& ar, int nVer);
	BOOL			IsArray()		{ return m_bArray;	};

	BOOL			m_bArray;			//	배열형식여부	TRUE:배열.
	CString			m_strBlockName;

	//	배열 형식의 블럭일 경우..
	//	배열 개수를 지정하는 아이템의 ID를 가진다.
	CString			m_strCntItemID;

	int				m_nReqRow;

	CArrFidItem		m_arrItem;	//	Array Value Pointer.

public:
	virtual void	Copy(const CFidBlock& item);
	virtual CFidBlock& operator = (const CFidBlock& item);

public:
	int			GetItemSize()	{ return m_arrItem.GetSize();	};
	CFidItem*	GetItemAt(CString strID);
	CFidItem*	GetItemNameAt(CString strID);
	CFidItem*	GetItemAt(int nIndex)		{ return m_arrItem.GetAt(nIndex);	};
	void		AddItem(CFidItem* pItem)	{ m_arrItem.Add(pItem);	};
};

typedef	CArray<CFidBlock*, CFidBlock*>		CArrFidBlock;


#pragma	pack()

#endif // !defined(AFX_FIDBLOCK_H__16F42BCA_D645_4B9B_978C_E7AC60B1F611__INCLUDED_)
