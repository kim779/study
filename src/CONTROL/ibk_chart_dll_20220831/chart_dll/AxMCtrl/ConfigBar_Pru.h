#ifndef __CONFIGBAR_PRU_H__
#define __CONFIGBAR_PRU_H__

#include "AxMConfigBar.h"
#include "CodeFilter.h"

class CAxisGViewApp;
class CAxTargetGroupPru;

typedef struct 
{
	CWnd*	sender;
	LPCTSTR trCode;
	LPCTSTR data;
	int	size;
	char	stat;
} TRINFO, *LPTRINFO;

typedef struct 
{
	CString		code;		// ELW 종목코드
	CString		name;		// 종목명
	CString		ktype;		// 권리유형 01:Call, 02:Put, 03:기타
	CString		pcode;		// 발행사 코드
	CString		pname;		// 발행사명
	CString		mjmt;		// 만기월 (YYYYMMDD)
	CString		bcode[5];	// 기초자산 코드들 "기초자산1;기초자산2;기초자산3;.."
} ELWCode;

typedef struct _basecode
{
	CString		code;		// 기초자산 코드
	CString		name;		// 기초자산명
}BASECode;

struct _ThemeData
{
	CString Name;
	int		Key;
};

#define WM_VIEW_SENDTR	(WM_USER + 333)

#define RKEY_FEATUREITEM	0x01
#define RKEY_HAVEITEM		0x02

class AFX_EXT_CLASS CConfigBarPru : public CAxMConfigBar
{
	friend class CGrpView;
	friend class CGrpMView;
public:
	CConfigBarPru(CWnd* view, CWnd* parent, CFont* font, LPCTSTR info);
	virtual ~CConfigBarPru();
private:
	LPVOID					m_trSender;
	CString	m_root;
	CString m_imgFolder, m_tabFolder, m_userFolder, m_gexFolder, m_userName, m_userID;

	UINT		m_requestKey;
	HTREEITEM	m_requestHaveItem;
protected:
	virtual void	GetMenunameBookmark(CString& menuname) ;
	virtual void	GetMenunames(CStringList& strList);
	virtual void	GetChartKindList(CChartKindList& chartList);
	
	virtual HTREEITEM	SetGroupList(CTreeCtrl* tree, LPCTSTR selectKind = EMPTYSTR);
	virtual void		SetGroupItem(CTreeCtrl* tree, HTREEITEM hItem);
	
	virtual void	LoadInterestItems(CAxMTreeCtrl* tree);
	virtual void	LoadHaveItems(CAxMTreeCtrl* tree);
	virtual void	LoadFeatureItems(CAxMListCtrl* list);

	virtual void	OnSelectItem(LPCTSTR kind_cd);
	virtual void	OnTargetItemUserConfig();
	virtual void	RefreshInterestItem();
public:
	virtual CString GetRoot();
	virtual CString GetImagePath();
	virtual CString GetChartPath();
	virtual CString GetTabPath();
	virtual CString GetUserPath();
	virtual CString GetUserName();
	virtual CString GetUserID();

	BOOL    GetUseAlias();
	
	void	SendRequestFromGroup(CString strCode, char* strData, int nSize, char stat = NULL);
	virtual CString GetTargetItemFileName();
	virtual CString GetCodeName(LPCTSTR code);
protected:
	//{{AFX_MSG(CGrpView)
	//}}AFX_MSG
	afx_msg LRESULT	OnUserMessage(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()		

protected:
	CAxTargetGroupPru*		m_group;
	virtual void	RequestFeatureItems();
	virtual void	RequestHaveItem(HTREEITEM hItem);
	void	RecvFeatureData(WPARAM wParam, LPARAM lParam);
	void	RecvHaveItemData(WPARAM wParam, LPARAM lParam);
	void	ClearAllChildItem(CTreeCtrl* tree, HTREEITEM hItem);
	bool	IsFutureAccount(CString sAccount);
	CString	ledgerTR(CString sAccn, CString sSvcn, CString sGubn);
};

#ifndef ELCODE
#define ELCODE	"elwcod.dat"
#endif // ELCODE

#define GROUP_FILE	"gropcd.ini"
#define THEME_FILE	"themcd.ini"

typedef CTypedPtrArray<CPtrArray, LPTARGETGROUP> CTargetGroupArray;

class CAxTargetGroupPru: public CAxTargetGroup
{
public:
	CAxTargetGroupPru(CWnd* wizard, CWnd* parent, LPCTSTR lpszTabFolder, LPCTSTR lpszUserFolder);
	virtual ~CAxTargetGroupPru();

	void SetBar(CConfigBarPru* bar) {
		m_bar = bar;
	}

	void RecvData(WPARAM wParam, LPARAM lParam);
private:
	CConfigBarPru* m_bar;
public:
	// group enumeration
enum {	groupUserSelect = 0,
		groupAllItem = 1, groupKospi200, groupKostar, groupETF, groupFreeboard,		
		groupELW, groupELW_List, groupELW_Asset,
		groupIndex, groupIndexKospi, groupIndexKosdaq, groupIndexSector, groupIndexFreeboard,
		groupIndexItem, groupIndexItemKospi, groupIndexItemKosdaq, groupIndexItemSector, groupIndexItemFreeboard,		
		groupFuture, groupOption, groupCallOption, groupPutOption,
		groupStockOption, groupStockCallOption, groupStockPutOption,
		groupCompany,
		groupUpDownRank,
		groupHoldItem,
		groupHistory,
		groupInterestItem,
		groupThemeItem
	};
	
	CString GetCodeName(CString code, LPVOID* ptrBuffer = NULL);
protected:
	void	SetSubItem(CTreeCtrl* tree, HTREEITEM hItem, int group);
	void	SetgroupIndexUpjong(int kind1, int kind2);

	void	SetgroupELW();
	void	SetgroupOption();
	void	SetgroupStockOption();
	void	SetgroupIndex();
	void	SetgroupIndexItem();
	void	SetgroupCompany();
	void	SetgroupUpDownRank();
	void	SetgroupHoldItem();
	void	SetgroupInterestItem();
	void	SetgroupThemeItem();

	void	SetgroupIndexItemKospi();
	void	SetgroupIndexItemKosdaq();
//	void	SetgroupIndexItemSector();
//	void	SetgroupIndexItemFreeboard();

	void	SetStockOptionItem(HTREEITEM hItem);
	void	SetgroupFromInformationFile(LPCTSTR fileName, int keyLength);
	void	SetgroupFromInformationFile(LPCTSTR fileName, LPCTSTR section, LPCTSTR key);
	
	virtual void	LoadJcode();
	virtual void	LoadUpcode();
	virtual void	LoadFcode();
	virtual void	LoadOJcode();
	virtual void	LoadPJcode();
	virtual void	LoadELWcode();

	void			NormalizeStockOptions();
	void			ClearStockOptions();
	CTargetGroupArray m_stockOptions;

	void	SetTreeItem_ELW(HTREEITEM hItemList, HTREEITEM hItemAsset);
protected:
	virtual void	Setting(CTreeCtrl* tree);
	virtual void	Setting(CTreeCtrl* tree, CListCtrl* list, KIND kind);
protected:
	// insert item, by selected group
	CListCtrl	*m_list, *m_asyncList;
	CTreeCtrl	*m_listTree;
	CString		m_lastTRCode;
	void	InsertListItem(LPVOID arr, int recSize, int count, CODEFILTERPROC proc, ITEMINSERTPROC itemProc);
	void	InsertListItem(LPCTSTR lpszBuffer);
	void	SetListUserSelect();
	void	SetListAllItem();
	void	SetListKospi200();
	void	SetListKostar();	
	void	SetListETF();
	void	SetListFreeboard();
	void	SetListELW();
	void	SetListFuture();
	void	SetListOption();
	void	SetListStockOption();
	void	SetListIndex();
	void	SetListIndexItem();
	void	SetListCompany();
	void	SetListUpDownRank();
	void	SetListHoldItem();
	void	SetListHistory();
	void	SetListInterestItem();
	void	SetListThemeItem();	
	bool	IsFutureAccount(CString sAccount);
	CString	ledgerTR(CString sAccn, CString sSvcn, CString sGubn);


	CArray<ELWCode, ELWCode> m_arrELW;
	CArray<BASECode, BASECode> m_arrBase;
public:
	static BOOL	IsExistPortfolio(LPCTSTR folder, int index);
	static void GetInterestGroupList(LPCTSTR folder, CStringArray& array);
	static void GetInterestItemList(LPCTSTR folder, int index, CStringArray& codeArray, CStringArray& nameArray, CByteArray& kindArray);
	static int	ConvertItemKind(DWORD data);
	static void GetAccountList(CWnd* wnd, CStringList& accts, BOOL useAlias);
	static void GetAccountPassword(CWnd* wnd, LPCTSTR acct, CString& pass);
	static void EncrytoAccountPassword(CWnd* wnd, LPCTSTR acct, LPCTSTR pass, CString& enc);
	static BOOL IsStaff(CWnd* wnd);
};


#endif // __CONFIGBAR_PRU_H__