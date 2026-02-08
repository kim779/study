// QueryGrid.h: interface for the CQueryGrid class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_QUERYGRID_H__9C2E15C2_6A28_41FD_9EDE_332BF2A9E924__INCLUDED_)
#define AFX_QUERYGRID_H__9C2E15C2_6A28_41FD_9EDE_332BF2A9E924__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "../SuperGrid/SuperGridCtrl.h"
#define IDC_COMBOBOXINLISTVIEW 0x1235


#define	DATA_GUBUN	0
#define	DATA_TYPE	1
#define	DATA_BOOL	2

enum {
	COLIDX_NAME = 0,
	COLIDX_TYPE,
	COLIDX_SIZE,
	COLIDX_BYTEORD
};

enum {
	COLIDX_ARRAY = 1,
	COLIDX_CNTNAME
};

class CTypeData
{
public:
	static	void	GetArrTypeName(int nType, CStringArray *pArr);
	static	void	GetArrTypeID(int nType, CDWordArray *pArr);
	static	CString	GetTypeName(int nType, int nID);
	static	int		GetTypeID(int nType, LPCSTR szName);
	
	static	char	*g_szGubunName[];
	static	int		g_nGubunID[];
	static	char	*g_szTypeName[];
	static	int		g_nTypeID[];
	static	char	*g_szBoolName[];
	static	int		g_nBoolID[];
};

class CQueryGrid : public CSuperGridCtrl  
{
public:
	CQueryGrid();
	virtual ~CQueryGrid();

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
	CStringArray	m_ArrCorpName;
	CDWordArray		m_ArrCorpID;
	CStringArray	m_ArrCPName;
	CDWordArray		m_ArrCPID;
	CStringArray	m_ArrGradeName;
	CDWordArray		m_ArrGradeID;
};

#endif // !defined(AFX_QUERYGRID_H__9C2E15C2_6A28_41FD_9EDE_332BF2A9E924__INCLUDED_)
