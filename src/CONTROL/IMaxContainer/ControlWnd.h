#pragma once
// ControlWnd.h : header file
//
#include "IMAXApi.h"
#include "../../h/param.h"
#include "CMDLSbox.h"
/////////////////////////////////////////////////////////////////////////////
// CControlWnd window

typedef struct _stSACMQ518 //고객권한 조회
{
	char in[5];
	char QryTp[1];					//조회구분   1관린점 2 계좌별 3 월별 5 고객별
	char BrnNo[3];
	char AcntNo[20];
	char CIntNo[20];
	char QryYymm[6];
	char MktTp[1];					//0 전체 1 미국
	char TrxStat[1];					//0 전체 1 적용 2 해지
	char FamtKindCode[1];   //0 전체 1 회사 2 고객
	char CIntTp[1];					//0 전체 1 일반 2 전문가
	char EmpYn[1];				//0 전체 1 고객 2 지원
}ISACMQ518;

#define L_ISACMQ518  sizeof(struct _stSACMQ518)

typedef struct _stSACMQ518_out //고객권한 조회
{
	char outBrn[5];
	char brnNm[40];					//조회구분   1관린점 2 계좌별 3 월별 5 고객별
	char out[5];
	char CIntNo[20];
	char AcntNo[20];
	char AcntNM[40];
	char EngAcntNM[40];
	char CIntTp[1];
	char TrxStat[1];
	char MktTp[1];
	char FamtKindCode[1];
	char TermTp[1];
	char AppSrtYymm[6];
	char AppEndYymm[6];
	char EmpYn[1];
	char IndtpPtnCode[2];
	char IndtpNm[40];
	char PsnlCorpTpNm[20];
	char WkplcNm[40];
	char PostNo[6];
	char AbdngAddr[100];
	char BlwdngAddr[100];
	char MgmtBrnNo[3];
	char	MgmtBrnNm[40];
	char	MgempNo[9];
	char MgempNm[40];
	char	TrxDttm[17];
	char	TrxUserId[16];
	char	AbndDt[8];
	char	AbndRsnTp[1];
	char	TrxBrnNo[3];
	char	TrxTermNo[3];
	char	TrxDt[8];
	char	TrxSeqno[9];
	char	TrxTp[1];
}OSACMQ518;

#define L_OSACMQ518  sizeof(struct _stSACMQ518_out)









typedef struct _stSACMT518 //고객권한 조회
{
	char in[5];
	char TrxTp[1];					//처리구분
	char AcntNo[20];              //계좌번호
	char CIntTp[1];                 //고객구분
	char EmpYn[1];					//사원여부
	char MktTp[1];					//시장 구분 : 0 전체 1 미국
	char IndtpPtnCode[2];		//업종유형코드
	char FamtKindCode[1];		   //요금종류코드
	char TermTp[1];					//기간구분
	char AppSrtYymm[6];         //적용시작년월
	char AppEndYymm[6];         //적용종료년월
	char AcntEngNm[40];			//영문계좌명
	char WkplcNm[40];				//직장명
	char PostNo[6];                    //우편번호
	char AbdngAddr[100];              //동이상주소
	char BlwdngAddr[100];              //동이하주소
}ISACMT518;

#define L_ISACMT518  sizeof(struct _stSACMT518)


typedef struct _stSACMT518_out //고객권한 조회
{
	char outBrn[5];
	char CIntNo[20];					//고객번호
	char PsnlCorpTpNm[20];     	//개인법인구분명
	char TpNm1[40];					//구분명1
	char out[5];
	char CustomNo[20];					//고객번호
	char AcntNo[20];					//계좌번호
	char AcntNM[40];                //계좌명
	char EngAcntNM[40];          //영문계좌명
	char CIntTp[1];                     //고객구분
	char Empyn[1];                    //사원여부
	char MktTp[1];						//시장구분
	char FamtKindCode[1];        //요금종류코드
	char TermTp[1];                  //기간구분
	char AppSrtYymm[6];			//적용시작년월
	char AppEndYymm[6];			//적용종료년월
	char TrxStat[1];						//처리상태
	char TrxStatNm[20];				//처리상태명
	char AbndRsnTp[1];             //해지사유구분
	char IndtpPtnCode[2];         //업종유형코드
	char IndtpNm[40];               //업종명
	char PsnlCorpTp[1];              //개인법인구분
	char PsnCorpTpNm[20];      //개인법인구분명
	char WkplcNm[40];			     //직장명
	char PostNo[6];					//우편번호
	char AbdngAddr[100];			//동이상주소
	char BlwngAddr[100];          //동이하주소
	char MgmtBrnNo[3];            //관리지점번호
	char	MgmtBrnNm[40];         //관리지점명
	char	MgmtEmpNo[9];          //관리사원번호
	char MgempNm[40];            //관리사원명
	char	TrxDttm[17];                 //처리일시
	char	TrxUserId[16];                //처리사용자ID
}OSACMT518;

#define L_OSACMT518  sizeof(struct _stSACMT518_out)

static std::unique_ptr<CMDLSbox> m_pMDLSdlg{};

class CControlWnd : public CWnd
{
// Construction
public:
	CControlWnd(CWnd *pParent, struct _param *pParam);

// Attributes
public:
	CWnd*		m_pMainWnd;
	CWnd*		m_pParent;
	CString		m_sHome;
	CString		m_sName;
	CString		m_sUsID;
	class CParam	m_param;

	CIMAXApi	m_IMAXApi;
	CString		m_sViewNo;
	CSize		m_sizeTot;

	BOOL		m_bLoinError;
	HANDLE		m_hView;
	int		m_nGroupNo;
	bool		m_bStaf;

	CString		m_sExid;
	CString		m_sCode;
	CString		m_sCodeName;
	CString		m_sBidUnit;
	CString		m_sAskUnit;
	CString		m_sSelPrice;
	CString		m_sBidPrice;
	CString		m_sAskPrice;
	CString		m_sCurrency;
	CString		m_sOrderQty;
	CString		m_sCodeAll{};    //20230321 종목코드 이벤트올때마다 저장(코드 지워도가지고 있는다)
	CString		m_sCustomerAuth{};
	CString		m_sCustomerNum{};
	CString    m_sJango{};

	int		m_iSkin;    //1 gray,  2 blue,  4 brown
	int		m_iType{};    //0 고객권한서버조회   ,  1 고객권한파일참조 

	BOOL		m_bLoinSuccess{};
	void			AbroadAuth(bool bSet, bool bfile = true);
	enum {
		dispidsJango = 9,
		TYPE_WAIT = 0};
	void			DoWork(int itype, int iSec);

	void		showMDLSbox(CString sMsg, CWnd* parent);
// Operations
public:
	void	Init();
	CString	Variant(int comm, CString data = "");
	void	SetParam(struct _param *pParam);
	void	parseIMAXMessageData(int nKind, CString sIMAXInfo);
	void	IMAXToMapTrigger(CString sValue);
	void	DllEvent(int nKind);

	void	LoginIMAX();
	bool	LoadIMAXView(CString ViewName, CRect rcCli);
	int	openView(int type, CString data);

	CString	Parser(CString& strSrc, CString strSub);
	void SearchCustomAuth();
	void	SendTR_Control(CString name, char* datb, int datl, char stat);
	CString GetAuthAccount();
// for OutputDebugString
	CString	m_tmpX;
	void	dprint();

	CString m_slog{};
	BOOL m_bSearhAuthFinish{};
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CControlWnd)
public:
	virtual void OnFinalRelease();
	//}}AFX_VIRTUAL

protected:

// Implementation
public:
	virtual ~CControlWnd();

	// Generated message map functions
protected:
	//{{AFX_MSG(CControlWnd)
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnDestroy();
	//}}AFX_MSG
	afx_msg long OnMessage(WPARAM wParam, LPARAM lParam);  
	afx_msg long OnMsgFormDlg(WPARAM wParam, LPARAM lParam);
	afx_msg long IMAXOnMessageOPEN(WPARAM wParam, LPARAM lParam);
	afx_msg long IMAXOnMessageCODEChange(WPARAM wParam, LPARAM lParam);
	afx_msg long IMAXOnMessageJano(WPARAM wParam, LPARAM lParam);
	afx_msg long IMAXOnMessageORDERINFO(WPARAM wParam, LPARAM lParam);
	afx_msg long IMAXOnMessageFETCH(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()

	// Generated OLE dispatch map functions
	//{{AFX_DISPATCH(CControlWnd)
	afx_msg void _SetIMAXGroup(short nGroup);
	afx_msg void _SetIMAXCIFId(LPCTSTR CIFId);
	afx_msg void _SetIMAXSkin(short nSkin);
	afx_msg void _SetIMAXData(short nKind, LPCTSTR pData);
	afx_msg BSTR _GetIMAXValue(LPCTSTR sValueText);
	afx_msg short _InitIMAX();
	afx_msg void _SetVisible(BOOL bVisible);
	afx_msg void _SetEnable(BOOL bEnable);
	//}}AFX_DISPATCH
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()
public:
	afx_msg void OnTimer(UINT_PTR nIDEvent);
protected:
	BSTR GetsJango();
	void SetsJango(BSTR newVal);
};