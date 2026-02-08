#pragma once
// SendOrder.h : header file
//

//#include "../../h/ledger_Hana.h"
//#include "../../h/ledger_Deatoo.h"
#include "../../h/ledger.h"

struct _sendData {
	char	Odgb[1];
	char	Mkgb[1];
	char	Mmgb[1];
	char	Acno[10];
	char	Pswd[8];
	char	Ogno[12];
	char	Code[12];
	char	Jqty[8];
	char	Jprc[10];
	char	Hogb[2];
	char	Jmgb[2];
	char	Mdgb[1];
	char	Prgb[1];
	char	Filler[30];
};

struct _orderResult {
	char	Jmno[6];
	char	Mono[6];
	char	Idat[80];
};

struct f_contmid {
	char	accn[8];
	char	cogb[2];
	char	pswd[8];
	char	gubn[1];
	char	juno[6];
	char	sort[1];
	char	date[8];
	char	cod2[8];
};

struct f_contgrid {
	char	mono[6];	/* 원주문번호		*/
	char	juno[6];	/* 주문  번호		*/
	char	ojno[6];	/* 원주문 번호		*/
	char	cod2[8];	/* 종목코드		*/
	char	hnam[30];	/* 종목명		*/
	char	odgb[8];	/* 주문구분		*/
	char	hogb[1];	/* 주문유형		*/
	char	jprc[11];	/* 주문가격		*/
	char	jqty[7];	/* 주문수량		*/
	char	dlgb[4];	/* 체결구분		*/
	char	dprc[11];	/* 체결가격		*/
	char	dqty[7];	/* 체결수량		*/
	char	wqty[7];	/* 미체결수량		*/
	char	stat[8];	/* 접수상태		*/
};

struct	f_cont {
	char	nrec[4];
	struct	f_contgrid rec[1];
};

#define L_senddata	sizeof(struct _sendData)

#define L_fContmid	sizeof(struct f_contmid)
#define L_fCont		sizeof(struct f_cont) - sizeof(struct f_contgrid)
#define L_fContGrid	sizeof(struct f_contgrid) 

/////////////////////////////////////////////////////////////////////////////
// CSendOrder window

class CSendOrder : public CWnd
{
// Construction
public:
	CSendOrder();
	CWnd* m_pParent{};

// Attributes
public:
	CString OrderResult(char *pData);

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSendOrder)
	//}}AFX_VIRTUAL

// Implementation
public:
	CString GetErrmsg(LPARAM lParam);
	int	GetRecordCnt(LPARAM lParam);
	CString	GetNextkey(LPARAM lParam);
	long	GetLedgerSize();
	CString MakeFContData(CString sAccn, CString sPswd, CString sGubn, CString  sDate);
	CString LedgerTR(CString sTran, CString sGubn, CString sMaxRow = "999", CString sSvcn = "");
	CString MakeFSendData(CString sMmgb, CString sAccn, CString sPswd, CString sOgno, CString sCode, CString sJqty, CString sJprc, CString sHogb, CString sMdgb);
	virtual ~CSendOrder();

	// Generated message map functions
protected:
	//{{AFX_MSG(CSendOrder)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

