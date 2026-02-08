#ifndef __CODEHELPER_H__
#define __CODEHELPER_H__


#define TRCD_HISTORY		1
#define TRCD_ALLCODE		2
#define TRCD_KOSPIUJ		3
#define TRCD_KOSDAQUJ		4
#define TRCD_COMPANY		5
#define TRCD_INTEREST		6
#define TRCD_THEME			7

#define ID_KOSPI200		111
#define ID_KOSPI100		112
#define ID_KOSPI50		113
#define ID_KOSPIIT		114

#define ID_KOSDAQ50		222
#define ID_KOSDAQIT		223
#define	ID_KOSTAR		224
#define	ID_ORAKMUNHWA		225

#define DTIF_KIND	0x0001
#define DTIF_SUBKIND	0x0002
#define DTIF_INDEX	0x0004


struct JCODE{
	CString Code;
	CString Name;
	//CString SNam;
	char	size;		// 자본금규모	2:대, 3:중, 4:소
	char	ucdm;		// 업종중분류
	char	ucds;		// 업종소분류
	char	jjug;		// 제조업 구분(27: 제조업)
	char	kpgb;		// KOSPI200 구분
	char	kosd;		// 종목구분
	char	ssgb;		// 소속구분
	char	ucmd;		// 0:보통주, 5:우선주
	char	wsgb;		// 0:일반 1:지배구조우수기업
	char	jsiz;		// 업종자본금 대중소
	char	itgb;		// IT...
	char	star;		// STAR
};


typedef BOOL (*CODEFILTERPROC)(JCODE* p, int index);

CODEFILTERPROC GetCodeFilterProc(int kind, int subkind, int index);


#define GROUP_FILE	"gropcd.ini"
#define THEME_FILE	"themcd.ini"

struct _inters
{
	char	gubn;		// 종목구분
				// 0:none, 1:현물, 2:선물, 3:옵션, 4:개별주식옵션, 5:지수
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
	char	filler[100];	// Reserved
};

typedef struct {
	int kind;
	int subkind;
	int index;
} KIND, *LPKIND;

#define MODE_READ	0x01
#define MODE_WRITE	0x02

class CAxInformation {
private:
	CString m_fileName;
	CFile*	m_file;
	CString m_buffer;
	int		m_mode;

	CString GetStartTag(LPCTSTR item);
	CString GetEndTag(LPCTSTR item);
public:
	CAxInformation(LPCTSTR fileName, int mode = MODE_READ);
	virtual ~CAxInformation();
public:
	CString GetString(LPCTSTR item, LPCTSTR def = "");
	int		GetInt(LPCTSTR item, int def = 0);
	void	SetString(LPCTSTR item, LPCTSTR value);
	void	SetInt(LPCTSTR item, int value);
	void	DeleteAll();
	void	SetMode(int mode);
};

class CAxTargetItem {
private:
	CString m_fileName;
public:
	CAxTargetItem();
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