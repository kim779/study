#pragma once


// CControlwnd
#include <afxtempl.h>
#include <Afxmt.h>

#define ALLCODE		0
#define GUKCODE		1
#define TONGCODE	2
#define TUKCODE		3
#define JICODE		4
#define GUMCODE		5
#define WHECODE		6
#define GUKTONG		7
#define EGUKTONG	8


class CParam
{
public:
	int		key;			// screen key
	CString		name;			// control symbol name
	CRect		rect;			// control rect
	CString		fonts;			// font name
	int		point;			// font point
	int		style;			// font style
	DWORD		tRGB;			// text color index
	DWORD		pRGB;			// paint color index
	CString		options;		// properties
};

class CCodeData
{
public:
	CString strName;
	CString strCode;
	CString strsuik;
	CString strMangi;
	bool	bFlag;
	COLORREF clrData;
};


struct	_ledgerH {
	//char	len[8];
	char	anam[40];		/* application name	*/
	char	snam[40];		/* service name		*/
	char	fnam[32];		/* function name	*/
	char	fncd[2];		/* function code	*/
	char	guid[32];		/* global id		*/
	char	ctyp[2];		/* channel type		*/
	char	eflg[1];		/* environment type	*/
	char	tflg[1];		/* tran flag		*/
	char	tdat[8];		/* tran send date	*/
	char	ttim[20];		/* tran send time	*/
	char	clip[31];		/* client ip		*/
	char	rtyp[1];		/* response type	*/
	char	rcod[8];		/* response code	*/
	char	rlcd[20];		/* response log code	*/
	char	rtit[50];		/* response title	*/
	char	rbmg[100];		/* response message	*/
	char	rdmg[500];		/* response message	*/
	char	usid[12];		/* user id		*/
	char	crst[2];		/* character set	*/
	char	fill[78];		/* filler		*/
	char	tmno[15];		/* terminal no		*/
	char	tmps[15];		/* terminal position	*/
	char	dept[8];		/* user department	*/
	char	brnc[8];		/* user branch		*/
	char	sale[8];		/* business branch	*/
	char	agnc[8];		/* agency		*/
	char	sfill[38];		/* filler		*/
	char	susid[20];		/* user id		*/
	char	menu[20];		/* menu			*/
};

typedef struct _balst
{
	char gubn[2];				 //구분			01호가
	char junum[7];				//주문번호 
	char date[10];				//날짜			yyyy-mm-dd
	char time[8];				//시각			hh:mm:ss
	char code[12];				//종목코드		
	char name[40];				//종목명		
	char ckind[4];				//채권종류     1000국채  4000지방 3000특수 2000통한 5000금융  6000회사 9999기타
	char mandate[10];			//만기일       yyyy-mm-dd
	char leftdate[6];			//잔존일       
	//char duration[7];			//듀레이션
	char min[7];				//민평
	char suiktype[2];			//수익률유형    01수익률  02가격
	char hogatypecode[2];		//호가유형코드  01매수  02매도 03확정
	char hogatype[6];			//호가유형      매수 매도 확정
	char hoga[7];				//호가			수익률일 경우 소수점 3자리 , 가격일 경우 소수점 1자리
	char contradict[7];			//대비			민평대비
	char volume[6];				//수량			
	char payday[8];				//결제일
	char linegubn[12];			//선교체구분	null, [선], [교 + 키]
	char hightradegubn[1];		//집중거래호가여부  Y/N 
	char sendid[30];			//송신자ID
	char comcode[6];			//회사코드
}BALST;

typedef struct _hogast
{
	char gubn[2];				 //구분			02종목별호가
	char code[12];				//종목코드
	char name[40];				//종목명
	char ckind[4];				//채권종류     1000국채  4000지방 3000특수 2000통합 5000금융  6000회사 9999기타
	char mandate[10];			//만기일
	char leftdate[6];			//잔존일    
	char min[10];				//민평
	char suiktype[2];			//수익률유형 01:수익률 02:가격
	char mdcur[10];				//매도호가현재가
	char mdcurtime[5];			//매도호가현재가업데이트시각  hh:mm
	char mdcurmcont[10];         //매도호가현재가 민평 대비
	char mdjprc[10];				//매도호가직전가
	char mdjcont[10];			//매도호가직전가대비
	char mdhggoga[10];			//매도호가고가
	char mdhggogatime[5];		//매도호가고가업데이트시각  hh;mm
	char mdhgjuga[10];			//매도호가저가
	char mdhgjugatime[5];		//매도호가저가업데이트시각  hh;mm
	char mscur[10];				//매수도호가현재가
	char mscurtime[5];			//매수호가현재가업데이트시각  hh:mm
	char mscurmcont[10];         //매수호가현재가 민평 대비
	char msjprc[10];				//매수호가직전가
	char msjcont[10];			//매수호가직전가대비
	char mshggoga[10];			//매수호가고가
	char mshggogatime[5];		//매수호가고가업데이트시각  hh;mm
	char mshgjuga[10];			//매수호가저가
	char mshgjugatime[5];		//매수호가저가업데이트시각  hh;mm
	char fncur[10];				//확정호가현재가
	char fncurtime[5];			//확정호가현재가업데이트시각  hh:mm
	char fncurmcont[10];         //확정호가현재가 민평 대비
	char fnjprc[10];				//확정호가직전가
	char fnjcont[10];			//확정호가직전가대비
	char fnhggoga[10];			//확정호가고가
	char fnhggogatime[5];		//확정호가고가업데이트시각  hh;mm
	char fnhgjuga[10];			//확정호가저가
	char fnhgjugatime[5];		//확정호가저가업데이트시각  hh;mm
}HOGAST;


typedef struct _ftchest
{
	char date[10];			     //날짜 yyyy-mm-dd
	char time[8];			     //시간 hh:mm:ss
	char comcode[6];			//회사코드
	char code[12];				//종목코드		
	char name[40];			    //채권명
	char ckind[4];				//채권종류     1000국채  4000지방 3000특수 2000통합 5000금융  6000회사 9999기타
	char mandate[10];			//만기일
	char suikrate[7];			//수익률
	char contrasuik[7];			//수익율대비			
	char prc[7];				//가격
	char contraprc[7];			//가격대비
	char volume[6];				//수량
	char payday[10];			//결제일
	char min[7];				//민평
	char minprc[7];				//민평가격
}FTCHEST;


typedef struct _chest
{
	char date[10];			     //날짜 yyyy-mm-dd
	char time[8];			     //시간 hh:mm:ss
	char code[12];			     //채권코드
	char name[40];			    //종목명
	char ckind[4];				//채권종류     1000국채  4000지방 3000특수 2000통합 5000금융  6000회사 9999기타
	char mandate[10];			//만기일
	char leftdate[10];			//잔존일    
	char min[7];				//민평
	char minprc[7];				//민평가격
	char suikrate[7];			//수익률
	char contradict[7];			//대비			민평대비
	char volume[6];				//수량	
	char payday[10];			//결제일
	char comcode[6];			//회사코드
}CHEST;

struct mid
{
	char userid[30];
};

struct mod
{
	char name[30];
};

class CControlwnd : public CWnd
{
	//DECLARE_DYNAMIC(CControlwnd)

public:
	CControlwnd();
	CWnd* m_pWizard;

	CString m_strcode;
	CString m_strmap;
	CString m_sUserID;
	CString m_sUserName;
	CString m_sRoot;
	CString m_sGroupName;
	CString m_shogatype;

	CString m_ssSearchdate;
	CString m_seSearchdate;

	CString m_ssmandate;
	CString m_semandate;

	CString m_strRealData;
	CString	m_strMsgUser;   //메신저 대화할 상대방 아이디
	CString	m_strMsgName;   //메신저 대화할 상대방 이름

	CCriticalSection m_sync;
	CMapStringToString m_mapGroupKey;
	CMap <CString, LPCTSTR, CMap<CString,LPCTSTR, CCodeData, CCodeData>*,  CMap<CString,LPCTSTR, CCodeData, CCodeData>*>  m_mapGroupCodemap;

	int m_imsgcnt;
	bool m_bServerError;
	bool m_brts;
	int m_kind;			//0 종목별최근호가  1 전체호가 2 15분체결 3 종목별현재 4 개별종목
	int m_itype;		//채권종류
	int itestcnt;
	bool m_binter;    //관심그룹에 내용만 보여주는지 여부
	bool m_bsuik;     //true 수익률  false 가격
	bool m_bmyhoga;   //내호가만 보여주기 여부

	enum	ctype {
		dispidGetGrouptype = 19L,
		dispidSetSync = 18L,
		dispidMessangerMemoPop = 17L,
		dispidGetCodeInGroup = 16L,
		dispidGetRealDate = 15L,
		dispidSethogatype = 14L,
		dispidSetcodetype = 13L,
		dispidSetmyhoga = 12L,
		dispidSetsuik = 11L,
		dispidSetbInter = 10L,
		dispidSetGroupByName = 9L,
		dispidGetGroupCombo = 8L,
		dispidSetMandate = 7L,
		dispidSetSearchDate = 6L,
		dispidGetAlertData = 5L,
		dispidGetOubData = 4L,
		ONECODE=0, MANYCODE } m_type;
	CParam		m_Param;
	virtual ~CControlwnd();

	virtual void OnFinalRelease();

	void	AddComma(char* pcData);
	void	AddGroup(CString strGroup = _T(""));
	void	SetParam(struct _param *pParam);
	void	LoadFile(CString strgroup = _T(""));
	void	LoadGroupOfFile();	

	void	ShellexcuteLogin();
	void	SendallhogaRTS(struct _balst* pdata);
	void	SendjongcodeRTS(struct _hogast* pdata);
	void	Send15CheRTS(struct _ftchest* pdata);

	bool	Checkcodetype(char* pkind);
	bool	Checkintercode(char* pcode);
	bool	CheckSuikprc(char* type);
	bool	Checkmyhoga(char* pcode);
	bool	Checkhogatype(char* hogatypecode);
	bool	Checksearchdate(char* date);
	bool	Checkmandate(char* mandate);
	bool	CheckMessangerLogin();
	void	JustLogin();

	CString GetSymbolFromFile();
	CString Variant(int nComm, CString strData = _T(""));
	CString Parser(CString &srcstr, CString substr);
	void	SendTR(CString name, CString data, BYTE type, int key);
	
	void	AddListOfCodeMap(CString strgroup, CString sCode, CString sName, CString strFlag, CString strClr);
	
	//void    sendTR(CString trC, char* pBytes, int nBytes, int gubn);
	bool	sendTR(CString trC, char* pBytes, int nBytes, int gubn = 1);

	void	GetledgerH(struct _ledgerH* pLedgerH, int gubn = 1);
	void	SearchInterList();
	void	oubSearchList(char* pdata);

	CString GetCombodata(int len, int icnt);
	CString GetCombodata(int len, int icnt, CString strgroup);
	CString AddSpace(CString strdata, int ilen);

	bool	m_bsync;
	CStringArray m_arr;
	void   test();
protected:
	DECLARE_MESSAGE_MAP()
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()
public:
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg long OnMessage(WPARAM wParam, LPARAM lParam);
protected:
	void SendData(void);

	enum 
	{
		dispidSetMapName = 3L,
		dispidJCode = 2,
		dispidSendData = 1L
	};
	BSTR GetJCode(void);
	void SetJCode(LPCTSTR newVal);
	void SetMapName(LPCTSTR strmap);
	BSTR GetOubData(void);
	BSTR GetAlertData(void);
	void SetSearchDate(LPCTSTR sdate, LPCTSTR edate);
	void SetMandate(LPCTSTR sdate, LPCTSTR edate);
	BSTR GetGroupCombo(LPCTSTR edate);
	void SetGroupByName(LPCTSTR group);
	void SetbInter(LPCTSTR binter);
	void Setsuik(LPCTSTR bsuik);
	void Setmyhoga(LPCTSTR bmyhoga);
	void Setcodetype(LPCTSTR type);
	void Sethogatype(LPCTSTR typehoga);
	void Setcode(LPCTSTR code);
	BSTR GetRealDate(void);
	BSTR GetCodeInGroup(LPCTSTR group);
	void MessangerMemoPop(LPCTSTR userid);
	void SetSync(LPCTSTR sync);
	
	BSTR GetGrouptype(LPCTSTR groupname);
};


