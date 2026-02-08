#pragma once

typedef struct tagTreeItem
{
	HTREEITEM	hTreeItem;
	CString		szItemName;
	CString		szItemCode;
	int			nIndex;
} TreeItem;

typedef struct tagDrTreeItem
{
	int nIndex;

	CString strDataType;    //# "I" "M" "T" "F" "P"

	CString strMarketCode;
	CString strData;

	CString strTitle;
} DrTreeItem;

// CCheckTreeCtrl
class CCheckTreeCtrl : public CTreeCtrl
{
	DECLARE_DYNAMIC(CCheckTreeCtrl)

public:
	CCheckTreeCtrl();
	virtual ~CCheckTreeCtrl();

private:
	HINSTANCE m_hConditionMng;
	typedef int(*_DLL_Open)(CStringArray *psaMSTCode);
	typedef void(*_DLL_Close)(void);
	typedef BOOL(*_DLL_GetConditionData) (int nType, BOOL bIndustry, int nGroupLockType,
		CStringArray &saGroupName, CStringArray &saCondition);
	typedef  int (*_DLL_GetFind_Packet) (CString strPath, int nTrIndex, BOOL bGetIndustry, BOOL bDefultResultField,
		BOOL bOrdering_Apply, LPSTR &lpPacket, int &nOrderCnt);
	typedef  BOOL (*_DLL_GetFind_Result)(LPVOID	aTRData, long dwTRDateLen, CStringArray &saColData, CStringArray &saResultData, 
		BOOL bDefultResultField);
	
	_DLL_Open				Lib_Open;
	_DLL_Close				Lib_Close;
	_DLL_GetConditionData	Lib_GetConditionData;
	_DLL_GetFind_Packet		Lib_GetFind_Packet;
	_DLL_GetFind_Result		Lib_GetFind_Result;

	HTREEITEM m_htiMarket[30];
	HTREEITEM m_htiPort;
	
//	CStringArray* m_pSaSelectedItem;
	CMap<int, int, TreeItem, TreeItem> m_mapTreeItem;

	CWnd* m_pReWnd;

public:
	CString m_strParentItemText;
	CString	m_strSelectItemText;

protected:
	DECLARE_MESSAGE_MAP()

	BOOL		SetLoad_ConditionMngLib();
	void		SetSearch_MSTCode();
	int			Module_SendTr(CString strTrNo, int nTrLen, BYTE *TrData);
	HTREEITEM	SetTreeItem(HTREEITEM htiParent, CString strItem, int lParam);
	BOOL		GetMarketInfo(int nMarketType, CStringArray *psaCode, CStringArray *psaName);
	BOOL		GetThemeInfo(CStringArray *psaCode, CStringArray *psaName);
	void		SetInitTree_Item(HTREEITEM htiParent, int nItemIndex, CStringArray *psaName, CStringArray *psaCode);

	void		SetTreeData(LPCSTR ROOT_TITLE[], LPCTSTR TYPE_DATA[], CStringArray* saRootTitle, CStringArray* saTypeData);
	void		SetInitTree_Port(HTREEITEM htiParent, int nItemIndex);
	void		GetKwansimGroupName(CStringArray& arrKey, CStringArray& arrGroupName);

//	void		SetCheckedTree(int nIndex);
	void		SetInitTree_HaveItem(HTREEITEM htiParent, int nItemIndex, CStringArray *psaName, CStringArray *psaCode, CString strDivision);
	void		SetSelectItem(HTREEITEM hItem);

public:	
	afx_msg void OnNMClick(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnDestroy();

	void SetCheckChildren(HTREEITEM hItem, BOOL bCheck);
	void UpdateParentState(HTREEITEM hItem);
	BOOL IsCheckAllChild(HTREEITEM hItem);
	BOOL IsCheckAnyChild(HTREEITEM hItem);

	HTREEITEM GetFirstCheckedItem();
	HTREEITEM GetNextCheckedItem(HTREEITEM hItem);
	
	CString	GetItemMaketCode(HTREEITEM hSelItem);
	
//	void SetInit(int nTreeSelect, CStringArray* pSaResult, CList<ST_HAVEITEM, ST_HAVEITEM&> *listHaveItem, CWnd* pReWnd);
	void SetInit(int nTreeSelect, CList<ST_HAVEITEM, ST_HAVEITEM&> *listHaveItem, CWnd* pReWnd);
	void GetSelectMText(CStringArray* psaMText);
	BOOL GetItemCodeFromThemeGroup(CString strThemeGroupCode, CStringArray* pSaCode, CStringArray* pSaName);
	BOOL GetItemCodeFromGwanGroup(CString strKey, CStringArray* arrCode, CStringArray* arrName);
	BOOL IsGwanGroup(HTREEITEM hItem);
	BOOL IsCheckedAnyItem();
};


