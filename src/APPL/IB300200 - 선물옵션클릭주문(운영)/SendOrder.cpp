// SendOrder.cpp : implementation file
//

#include "stdafx.h"
#include "SendOrder.h"
#include "../../h/fxCommonx.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define	ledgerDLL		0x24
/////////////////////////////////////////////////////////////////////////////
// CSendOrder

CSendOrder::CSendOrder()
{
}

CSendOrder::~CSendOrder()
{
}


BEGIN_MESSAGE_MAP(CSendOrder, CWnd)
	//{{AFX_MSG_MAP(CSendOrder)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CSendOrder message handlers

CString CSendOrder::MakeFSendData(CString sMmgb, CString sAccn, CString sPswd, CString sOgno,
		CString sCode, CString sJqty, CString sJprc, CString sHogb, CString sMdgb)
{
	struct _sendData senddata;

	FillMemory(&senddata, L_senddata, ' ');
	CopyMemory(&senddata.Odgb, "2", 1);
	CopyMemory(&senddata.Mmgb, sMmgb, sizeof(senddata.Mmgb));
	CopyMemory(&senddata.Acno, sAccn, sizeof(senddata.Acno));
	CopyMemory(&senddata.Pswd, sPswd, sizeof(senddata.Pswd));
	CopyMemory(&senddata.Ogno, sOgno, sizeof(senddata.Ogno));
	CopyMemory(&senddata.Code, sCode, sizeof(senddata.Code));
	sJqty.Format("%08d", atoi(sJqty));
	CopyMemory(&senddata.Jqty, sJqty, sizeof(senddata.Jqty));
	sJprc.Format("%010d", atoi(sJprc));
	CopyMemory(&senddata.Jprc, sJprc, sizeof(senddata.Jprc));
	CopyMemory(&senddata.Hogb, sHogb, sizeof(senddata.Hogb));
	CopyMemory(&senddata.Mdgb, sMdgb, sizeof(senddata.Mdgb));
	CopyMemory(&senddata.Prgb, "X"	, sizeof(senddata.Prgb));

	return 	CString((char*)&senddata, L_senddata);	
}

CString CSendOrder::OrderResult(char *pData)
{
	CString sOrderNum = "";
	int nCnt = atoi(CString(pData, 4));
	pData += 4;

	struct _orderResult* pOrd{};
	for (int ii = 0; ii < nCnt; ii++)
	{
		pOrd = (struct _orderResult*)(pData + sizeof(struct _orderResult) * ii);
		sOrderNum += CString(pOrd->Jmno, sizeof(pOrd->Jmno)) + "\t";
	}

	return sOrderNum;
}

CString CSendOrder::LedgerTR(CString sTran, CString sGubn, CString sMaxRow, CString sSvcn, CString sPswd)
{
	CString strUser(_T(""));

	char* pData = (char*)m_pParent->SendMessage(WM_USER, MAKEWPARAM(variantDLL, userCC), 0L);
	if ((long)pData > 1)
		strUser = pData;

	struct _ledgerH ledger;

	FillMemory(&ledger, L_ledgerH, ' ');
	m_pParent->SendMessage(WM_USER, ledgerDLL, (LPARAM)&ledger);

	CopyMemory(&ledger.svcd, (LPCTSTR)sSvcn, sizeof(ledger.svcd));
	CopyMemory(&ledger.usid, (LPCTSTR)strUser, strUser.GetLength());
	CopyMemory(&ledger.brno, sGubn, sGubn.GetLength());
	CopyMemory(&ledger.rcnt, _T("0000"), sizeof(ledger.rcnt));

	//2013.12.23 KSJ 일방향암호화 추가
	if(!sPswd.IsEmpty())
	{
		CopyMemory(&ledger.hsiz, "44", sizeof(ledger.hsiz));
		CopyMemory(&ledger.epwd, sPswd, sPswd.GetLength());
	}

	ledger.fkey[0] = 'C';
	ledger.mkty[0] = '1';
	ledger.odrf[0] = '1';

	return CString((char*)&ledger, L_ledgerH);

/*	if (VariantA(m_pParent, systemCC, "") == "D")
	{
		svr = "H1";
		if (VariantA(m_pParent, orderCC, ""))
			svr = "H2";
		struct	_ledgerH ledger;
		
		FillMemory(&ledger,L_ledgerH, ' ');
		CopyMemory(&ledger.tran, sTran,	sizeof(ledger.tran));
		CopyMemory(&ledger.svr, svr, sizeof(ledger.svr));
		CopyMemory(&ledger.gubn, sGubn, sizeof(ledger.gubn));
		CopyMemory(&ledger.rows, sMaxRow, sizeof(ledger.rows));
		CopyMemory(&ledger.svcn, sSvcn, sizeof(ledger.svcn));
		
		m_pParent->SendMessage(WM_USER, ledgerDLL, (LPARAM)&ledger);
		
		return 	CString((char*)&ledger, L_ledgerDaetooH);
	}
	else
	{
		svr = "11";
		if (VariantA(m_pParent, orderCC, ""))
			svr = "14";
		struct	_ledgerHanaH ledger;
		
		FillMemory(&ledger,L_ledgerH, ' ');
		CopyMemory(&ledger.screen,   sTran, sTran.GetLength());
		CopyMemory(&ledger.dt_ccgb,  svr, svr.GetLength());
		CopyMemory(&ledger.tran,     sSvcn, sSvcn.GetLength());
		CopyMemory(&ledger.dt_svnm,  sSvcn, sSvcn.GetLength());
		CopyMemory(&ledger.gubn,     sGubn, sGubn.GetLength());
		CopyMemory(&ledger.nrec,     sMaxRow, sMaxRow.GetLength());
		
		m_pParent->SendMessage(WM_USER, ledgerDLL, (LPARAM)&ledger);
		return 	CString((char*)&ledger, L_ledgerHanaH);
	}
*/
	return "";
}

CString CSendOrder::MakeFContData(CString sAccn, CString sPswd, CString sGubn, CString sDate)
{
	struct f_contmid c;
	
	FillMemory(&c, L_fContmid, ' ');
	CopyMemory(&c.accn, sAccn, sizeof(c.accn));
	CopyMemory(&c.cogb, sAccn.Mid(8, 2), sizeof(c.cogb));
	CopyMemory(&c.pswd, sPswd, sizeof(c.pswd));
	CopyMemory(&c.gubn, sGubn, sizeof(c.gubn));
	CopyMemory(&c.sort, "1", sizeof(c.sort));
	CopyMemory(&c.date, sDate, sizeof(c.date));
	
	return 	CString((char*)&c, L_fContmid);
}

long CSendOrder::GetledgerSize()
{
	return L_ledgerH;
}

CString CSendOrder::GetErrmsg(LPARAM lParam)
{
	struct	_ledgerH ledger;
	CopyMemory(&ledger, (void*)lParam, L_ledgerH);
	return CString((char*)&ledger.emsg, 130);
}
