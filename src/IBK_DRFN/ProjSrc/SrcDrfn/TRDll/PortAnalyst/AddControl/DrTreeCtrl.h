#if !defined(AFX_EXXMLTREECTRL_H__AAAFA8CA_8049_4EBA_8F38_1788C4950280__INCLUDED_)
#define AFX_EXXMLTREECTRL_H__AAAFA8CA_8049_4EBA_8F38_1788C4950280__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ExXmlTreeCtrl.h : header file
//

/************************************************/
/*																		          */
/*							Dr Code Tree Control						*/
/*                                              */
/*																		          */
/*							작성일 : 2006. 12 ~							*/
/*						작성자 : 우동우 (Doori co.)		    */
/*																		          */
/************************************************/

/////////////////////////////////////////////////////////////////////////////
// CDrTreeCtrl window
//#include "../../../inc/IAUTrCommMng.h"
#include "../DefineFiles/server_typedef.h"

typedef struct tagDrTreeItem
{
  int nIndex;

  CString strDataType;    //# "I" "M" "T" "F" "P"

  CString strMarketCode;
  CString strData;

  CString strTitle;
} DrTreeItem;


class CDrTreeCtrl : public CTreeCtrl
{
// Construction
public:
	CDrTreeCtrl();
	virtual ~CDrTreeCtrl();

private:
	HINSTANCE		m_hConditionMng;
	typedef int(*_DLL_Open)(CStringArray *psaMSTCode);
	typedef void(*_DLL_Close)(void);

	typedef BOOL(*_DLL_GetConditionData) (int nType, BOOL bIndustry, int nGroupLockType,
										  CStringArray &saGroupName, CStringArray &saCondition);
	typedef  int (*_DLL_GetFind_Packet) (CString strPath, int nTrIndex, BOOL bGetIndustry, BOOL bDefultResultField,
										 BOOL bOrdering_Apply, LPSTR &lpPacket, int &nOrderCnt);
	typedef  BOOL (*_DLL_GetFind_Result)(LPVOID	aTRData, long dwTRDateLen, CStringArray &saColData, CStringArray &saResultData, 
										 BOOL bDefultResultField);
	typedef void (*_DLL_SetUserFolderPath)(CString strUserFolderPath);

	_DLL_Open				Lib_Open;
	_DLL_Close				Lib_Close;

	_DLL_GetConditionData	Lib_GetConditionData;
	_DLL_GetFind_Packet		Lib_GetFind_Packet;
	_DLL_GetFind_Result		Lib_GetFind_Result;
	_DLL_SetUserFolderPath	Lib_SetUserFolderPath;
	
private:
	int				m_bAllItemView;
	int             m_nIndex_Port;
	int				m_nIndex_Thema;	
	HTREEITEM		m_htiMarket[30];
	HTREEITEM		m_htiPort;
	HTREEITEM		m_htiConditionSearch;

	CImageList		m_imglistTree;

	int				m_nTrIndex;
	CMap <int, int, DrTreeItem, DrTreeItem> m_mapItem;

	CWnd *m_pwndParent;

	MSTItemInfoMap	m_mapMstItemInfo;

	int			m_nPreReceiveLen;

public:
	BOOL	Create(CWnd* pParentWnd, UINT nID);
	void	SetInit();

	int		GetFindTreeItem(HTREEITEM htItemNode, CString strFindText, CArray<HTREEITEM, HTREEITEM> &saFindItemArray);
	void	SetSelectItem(HTREEITEM hSelItem);
	BOOL	SetReceiveData(LPTSTR pData, DWORD dwDataLen, CStringArray& arrCode);
	HRESULT	ReceiveData(LPCSTR szTR, LPVOID	aTRData, long dwTRDateLen);
	int		InMakeTr_P0621(BYTE *pTrData, BYTE* pTrOutData);
	void	GetKwansimGroupName(CStringArray& arrGroupName);
	void	GetItemsWithGroupName(CString strIndex, CStringArray& arrItems);
	BOOL	GetItemNameAndMarket(CString& strCode, CString& strName, int& nMarket);

	//tr수신후 tree에 데이터 셋팅
	void    SetInitTree_PortData(HTREEITEM htiParent, int nItemIndex, CStringArray &saPortName);
	int	GetPortIndex() { return m_nIndex_Port; };
private:
	BOOL	SetLoad_ConditionMngLib();
	void	SetIconLoad();

	int		GetTreeIconIndex_File(BOOL bAlertReg, BOOL bUnitCan);

	void	SetInitTree_Item(HTREEITEM htiParent, int nItemIndex, CStringArray *psaName, CStringArray *psaCode);
	void	SetInitTree_File(HTREEITEM htiParent, int nItemIndex, CStringArray *psaGroupName, CStringArray *psaCondiData);

	void	SetInitTree_Port(HTREEITEM htiParent, int nItemIndex);
	void	SetInit_LibPortPolio(BOOL bInit);
	BOOL	GetMarketInfo(CString strType, CStringArray *psaCode, CStringArray *psaName);
	void	SetControlState(BOOL bState);

	HTREEITEM SetTreeItem(HTREEITEM htiParent, CString strItem, 
																		int nImgIndex, int nImgSelIndex, int lParam);
	BOOL	GetFindFile(CString strFolderPath, CString strFilePath, CList<XMLTreeInfoFolder, XMLTreeInfoFolder&> *pFolderList);
	BOOL	GetFindFolder(CString strFolderPath, CList<XMLTreeInfoFolder, XMLTreeInfoFolder&> *pFolderList);

	void	SetItemAllView(BOOL bAllItemView);
	void	SetItemChildView(HTREEITEM hItem, BOOL bAllItemView);

	BOOL	SetSelectItme(CString strFolderPath, CString strFileName);
	BOOL	GetFindTreeItem(HTREEITEM htItemNode, CString strFindText, HTREEITEM &hItem);

	BOOL	SetDeleteItem(HTREEITEM hSelItem);
	BOOL	GetSelectItemData(HTREEITEM hSelItem);

	BOOL	GetCode(int nKey, CStringArray &saCode);

	int		Modual_SendTr(CString strTrNo, int nTrLen, BYTE *TrData);
	void	SetSearch_MSTCode();

	void	SetSearch_Thema();
	void	SetSearch_ThemaToCode(CString strThemaCode);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDrTreeCtrl)
	public:
	
	//}}AFX_VIRTUAL

	// Generated message map functions
protected:
	//{{AFX_MSG(CDrTreeCtrl)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnDestroy();
	//}}AFX_MSG

	afx_msg void OnNotify_PortPolio(WPARAM wParam, LPARAM lParam);
	afx_msg long OnGetData(WPARAM wParam, LPARAM lParam);

	DECLARE_MESSAGE_MAP()
public:
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_EXXMLTREECTRL_H__AAAFA8CA_8049_4EBA_8F38_1788C4950280__INCLUDED_)
