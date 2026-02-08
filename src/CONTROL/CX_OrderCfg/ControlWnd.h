#include "CfgWnd.h"

#pragma once
// ControlWnd.h : header file
//


#define	CNT_STOCK	24
#define	CNT_FUTURE	11

/////////////////////////////////////////////////////////////////////////////
// CControlWnd window

typedef struct _SONBQ101_mid {
	char In[5];                        /* 레코드갯수            */
	char zAcntNo[20];                  /* 계좌번호              */
	char zPwd[8];                      /* 비밀번호              */
	char zQryTp[1];                    /* 조회구분              */
	char lOrdAmt[16];                  /* 주문금액              */
	char dRatVal[19];                  /* 비율값                */
	char zFnoIsuNo[32];                /* 선물옵션종목번호      */
	char zBnsTp[1];                    /* 매매구분              */
	char dOrdPrc[13];                  /* 주문가                */
	char zFnoOrdprcPtnCode[2];         /* 선물옵션호가유형코드  */
} SONBQ101_mid;

#define L_SONBQ101_mid sizeof(_SONBQ101_mid)

typedef struct _SONBQ101_mod {
	SONBQ101_mid mid;
	char Out[5];                       /* 레코드갯수            */
	char zAcntNm[40];                  /* 계좌명                */
	char zQryDt[8];                    /* 조회일                */
	char dCurPrc[13];                  /* 현재가                */
	char lOrdAbleQty[16];              /* 주문가능수량          */
	char lNewOrdAbleQty[16];           /* 신규주문가능수량      */
	char lLqdtOrdAbleQty[16];          /* 청산주문가능수량      */
	char lUsePreargMgn[16];            /* 사용예정증거금액      */
	char lUsePreargMnyMgn[16];         /* 사용예정현금증거금액  */
	char lOrdAbleAmt[16];              /* 주문가능금액          */
	char lMnyOrdAbleAmt[16];           /* 현금주문가능금액      */
} SONBQ101_mod;


class CControlWnd : public CWnd
{
// Construction
public:
	CControlWnd();
	virtual ~CControlWnd();

	void CreateCtrl(CWnd* pParent, struct _param* pParam);

// Attributes
public:
	CWnd*	m_pParent;
	CParam	m_Param;
	std::unique_ptr<class CCfg>	m_cfg;

	CString m_sPushPrefix;
	CString	m_sTxCode;
	CString	m_sMmObj, m_sPrcObj, m_sQtyObj;
	int	m_nKind;	// 1.주식, 2:선물옵션, 3:KOPEX

	CString m_rtsCd;
	CString	m_mmgb;
	CString m_price;
	CString m_qty;
	CString m_sise[24];

	bool	m_bDoAlert;
	bool	m_bQueryQty;

	BOOL	m_bNewUnit;	// 2023 호가단위개선 적용

// Operations
public:
	CString Variant(int comm, CString data);
	CString OptionParser(CString sOption, CString sKey);
	CString Parser(CString &srcstr, CString substr);
	void	sendTR(CString name, CString data, BYTE type, char key, BYTE interKey);

	void	processHogaOUB(char* stream);
	void	processAlert(char* stream);
	void	processAlertx(struct _alertR* alertR);
	void	DoQueryHoga();
	void	ReturnHoga();
	void	SendMapVal(CString obj, CString val);
	int	GetAvailQty(int qty);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CControlWnd)
public:
	virtual void OnFinalRelease();
	//}}AFX_VIRTUAL

// Implementation
public:

	// Generated message map functions
protected:
	//{{AFX_MSG(CControlWnd)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	//}}AFX_MSG
	afx_msg long OnMessage(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()

	// Generated OLE dispatch map functions
	//{{AFX_DISPATCH(CControlWnd)
	BOOL m_noFee;
	CString m_itemCd;
	afx_msg void OnItemCdChanged();
	BOOL m_timeOutOrder;
	afx_msg void OnTimeOutOrderChanged();
	CString m_orderGB;
	afx_msg void OnOrderGBChanged();
	CString m_pushPrefix;
	afx_msg void OnPushPrefixChanged();
	BOOL m_keepMM;
	afx_msg void OnKeepMMChanged();
	BOOL m_keepQty;
	afx_msg void OnKeepQtyChanged();
	BOOL m_keepPrc;
	afx_msg void OnKeepPrcChanged();
	afx_msg BOOL _GetNoFee();
	afx_msg void _SetNoFee(BOOL bNewValue);
	afx_msg BOOL _GetTabAccount();
	afx_msg BOOL _GetTabButton();
	afx_msg BOOL _GetTabCode();
	afx_msg BOOL _GetTabMMGB();
	afx_msg BOOL _GetTabPassword();
	afx_msg BOOL _GetTabPrc();
	afx_msg BOOL _GetTabQty();
	afx_msg BOOL _GetEnterOrder();
	afx_msg void SetEnterOrder(BOOL bNewValue);
	afx_msg BSTR _GetMMGB();
	afx_msg void _SetMMGB(LPCTSTR lpszNewValue);
	afx_msg BOOL _GetOrderConfirm();
	afx_msg void _SetOrderConfirm(BOOL bNewValue);
	afx_msg BSTR _GetPrice();
	afx_msg void _SetPrice(LPCTSTR lpszNewValue);
	afx_msg BSTR _GetQty();
	afx_msg void _SetQty(LPCTSTR lpszNewValue);
	afx_msg BOOL _GetTabCondition();
	afx_msg BSTR _GetOrdUnit();
	afx_msg void _SetOrdUnit(LPCTSTR lpszNewValue);
	afx_msg BOOL _ShowDlg();
	afx_msg void _LoadInfo();
	afx_msg void _Pop();
	afx_msg void _Push();
	afx_msg BSTR _ReadCfg(LPCTSTR Key);
	afx_msg void _WriteCfg(LPCTSTR Key, LPCTSTR Value);
	afx_msg void _ClearPush();
	afx_msg void _QueryHoga();
	afx_msg void _SaveCfg();
	afx_msg void _LoadDefaultCfg();
	afx_msg void _QueryQty();
	afx_msg void _SetPrivateCfgName(LPCTSTR PrivateCfgName);
	afx_msg void _SetPublicCfgName(LPCTSTR PublicCfgName);
	afx_msg BSTR _MoveTick(LPCTSTR Prc, short Tick);
	afx_msg void _SetDoQty(BOOL bCheck);
	afx_msg void QueryPrcAbl(LPCTSTR sCode, LPCTSTR sAcc, LPCTSTR sPrc, LPCTSTR sMmgb);
	//}}AFX_DISPATCH
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()
};