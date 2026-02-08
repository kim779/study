#ifndef __CODEHELPER_H__
#define __CODEHELPER_H__

#define TRCD_HISTORY		1
#define TRCD_ALLCODE		2
#define TRCD_KOSPIUJ		3
#define TRCD_KOSDAQUJ		4
#define TRCD_COMPANY		5
#define TRCD_INTEREST		6
#define TRCD_THEME			7

#define DTIF_KIND			0x01
#define DTIF_SUBKIND		0x02
#define DTIF_INDEX			0x04

#define ID_KOSPI200			101
#define ID_KOSPI200_MANUFACTURE		102
#define ID_KOSPI200_ELECTRONIC		103
#define ID_KOSPI200_BUILDING		104
#define ID_KOSPI200_DISTRIBUTION	105
#define ID_KOSPI200_FINANCE			106
#define ID_KOSPI100			401
#define ID_KOSPI50			402
#define ID_KOSPIIT			403

#define ID_KOSDAQ50			301
#define ID_KOSDAQIT			302
#define	ID_KOSTAR			303
#define	ID_ORAKMUNHWA		241

#define K200_MANUFACTURE	2
#define K200_ELECTRONIC		3
#define K200_BUILDING		4
#define K200_DISTRIBUTION	5
#define K200_FINANCE		6


#define DEF_TARGETGROUP 0

typedef BOOL (*CODEFILTERPROC)(LPVOID node, int kind, int subkind, int index);

enum {
	ikNone = 0,
	ikStock, ikFuture, ikOption, ikStockOption, ikIndex, ikForeign
};
/*
struct _inters
{
	char	gubn;		// 종목구분
						// 0:none, 1:현물, 2:선물, 3:옵션, 4:개별주식옵션, 5:지수 6:해외지수
	char	code[12];	// 종목코드
	char	name[30];	// 종목명
	int		possNum;	// 보유수량
	double	possPrice;	// 보유단가
	char	memo[30];	// 메모
	char	o_check;	// 주문 실행여부
						// 0:none, 1:checked
	char	o_kind;		// 주문구분
						// 1:매도, 2:매수
	char	o_mmgb;		// 매매구분
						// 1:보통, 2:시장가
	int		o_num;		// 주문수량
	double	o_price;	// 주문단가
	char	a_kind;		// 알람 설정
						// 1:그림만, 2:그림과 소리
	int		a_num;		// 조건 항목 갯수
	char	filler[100];// Reserved
};

struct _alarms
{
	char	a_kind;		// 감시항목
				// 0:none, 1:현재가, 2:매도호가, 3:매수호가, 4:등락률, 5:손익평가, 6:수익률
	double	a_value;	// 감시 비교값
	char	a_condition;	// 감시 조건
				// 1:>=, 2:>, 3:=, 4:<, 5:<=
	char	filler[40];	// Reserved
};
*/
typedef struct {
	int kind;
	int subkind;
	int index;
} KIND, *LPKIND;

class AFX_EXT_CLASS CAxInformation {
private:
	CString m_fileName;
	CFile*	m_file;
	CString m_buffer;
	int		m_mode;

	CString GetStartTag(LPCTSTR item);
	CString GetEndTag(LPCTSTR item);
public:
	CAxInformation(LPCTSTR fileName, int mode = modeRead);
	virtual ~CAxInformation();

	enum MODE{ modeRead = 0x01, modeWrite };
public:
	CString GetFileName() const { return m_fileName; }
	void	SetFileName(LPCTSTR lpszFileName) { m_fileName = lpszFileName; }

	CString GetString(LPCTSTR item, LPCTSTR def = "");
	int		GetInt(LPCTSTR item, int def = 0);
	void	SetString(LPCTSTR item, LPCTSTR value);
	void	SetInt(LPCTSTR item, int value);
	void	DeleteAll();
	void	SetMode(int mode);
};

/************************************************************************/
/* CAxTargetGroup                                                       */
/************************************************************************/
typedef struct {
	char name[64];
	int	 group;
} TARGETGROUP, *LPTARGETGROUP;

class AFX_EXT_CLASS CAxTargetGroup {
public:
	CAxTargetGroup(CWnd* wizard, CWnd* parent, LPCTSTR lpszTabFolder, LPCTSTR lpszUserFolder);
	virtual ~CAxTargetGroup();

	void	SetTargetGroup(CTreeCtrl* tree);
	void	SetTargetItem(CTreeCtrl* tree, CListCtrl* list, KIND kind);
protected:
	CWnd*		m_wizard;
	CWnd*		m_parent;
	CTreeCtrl*	m_tree;
	HTREEITEM	m_item;
	CString		m_tabFolder, m_userFolder;
	KIND		m_kind;

	struct upcode* m_upcode;
	int		m_upcount;

	struct hjcode* m_hjcode;
	int		m_hjcount;
	
	struct fjcode* m_fjcode;
	int		m_fjcount;
	
	struct ojcode* m_ojcode;
	int		m_ojcount;
	
	struct pjcode* m_pjcode;
	int		m_pjcount;

	struct elwcode* m_elwcode;
	int		m_elwcount;

	void	LoadCodeMaster(LPCTSTR codeFileName, int& count, void** buffer, int recSize, int headerSize = 0);

	virtual void	LoadJcode() {}
	virtual void	LoadUpcode(){}
	virtual void	LoadFcode() {}
	virtual void	LoadOJcode(){}
	virtual void	LoadPJcode(){}
	virtual void	LoadELWcode() {}
protected:
	virtual void	Setting(CTreeCtrl* tree) = 0;
	virtual void	Setting(CTreeCtrl* tree, CListCtrl* list, KIND kind) = 0;
public:
	hjcode* GetHJCode() { return m_hjcode; }
	upcode* GetUPCode() { return m_upcode; }
	fjcode* GetFJCode() { return m_fjcode; }
	ojcode* GetOJCode() { return m_ojcode; }
	pjcode* GetPJCode() { return m_pjcode; }
	elwcode* GetELWCode() { return m_elwcode; }

	int GetHJCount() { return m_hjcount; }
	int GetUPCount() { return m_upcount; }
	int GetFJCount() { return m_fjcount; }
	int GetOJCount() { return m_ojcount; }
	int GetPJCount() { return m_pjcount; }
	int GetELWCount() { return m_elwcount; }
};

class AFX_EXT_CLASS CAxTargetItem {
private:
	CString m_fileName;
public:
	CAxTargetItem(LPCTSTR fileName);
	virtual ~CAxTargetItem();
	
	void	LoadItemFromFile();
	void	SaveItemToFile();
	static  int		KindToKey(KIND& kind);
	static  void	KeyToKind(int key, KIND& kind);
public:
	void	SetItem(KIND& kind, CString items); 
	void	SetItem(KIND& kind, CListCtrl& list);
	void	SetItem(int key, CString items);
	
	CString	GetItem(KIND& kind);
	void    GetItem(KIND& kind, CListCtrl& list);
	
	CMap <int, int, CString, CString> m_items;
};

#endif // __CODEHELPER_H__