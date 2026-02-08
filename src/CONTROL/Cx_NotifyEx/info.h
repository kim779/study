#pragma once

#include "libDefine.h"

struct _mingamBase 
{
	CString		hv[4];			// 역사적변동성 (90/60/30/21)
	CString		cdgl;			// CD 금리

	CString		krts;			// 최근월물 선물 code
	CString		kcur;			// 최근월물 선물 현재가
	CString		time;			// 장운용시간
	
	CString		k200rts;		// KOSPI 200 code
	CString		k200cur;		// KOSPI 200 현재가
};

class CInfo  
{
public:
	CInfo(CWnd *pControl);
	virtual ~CInfo();

private:
	CWnd	*m_pControl;

public:
	void	RemainParsing(struct f_rec *frec);
	void	SetNoCont(CString gubnS, CString amtS);
	void	SetNoContNew(int md, int ms);
	void	SetOpSensCal(class COpSensCal *pOpSensCal) { m_pOpSensCal = pOpSensCal; }
	CString	GetTotalData();
	void	CalMingam(_mingamBase *mgBase, bool bAll = true);
	bool	CalPgsonik(CString newCurrS, CString newDiffS);
	void	CalPgsonik();
	int	NoticeData(CMapStringToString& m_mapNotice, CString codeS, bool bNEw);

private:
	void	setNoCont(CString gubnS, CString stateS, int amt);
	int	getNoCont(CString gubnS);
	int	getJKind(CString sCode);
	double	getOptionStrike(CString opCode);
	void	getBase();
	
public:
	CString	sCode,			// 종목코드	00
		sJmm,			// 종목명	01
		sGubn,			// 매매구분	02
		sRemainAmt,		// 잔고수량	03
		sPossibleAmt,		// 주문가능수량	04
		sRPrice,		// 평균단가	05
		sCurr,			// 현재가	06
		sRDiff,			// 평균가대비	07
		sPPrice,		// 평가금액	08
		sPgSonik,		// 평가손익	09
		sSuik,			// 수익률	10
		sMaip,			// 매입금액	11
		sDiff,			// 전일대비	12
		sMchg,			// 미체결	13
		sDelta,			// 델타		14
		sGamma;			// 감마		15
		

	CString	sJjTrade;		// 잔존일수 거래일수
	CString	sJjCalendar;		// 잔존일수 달력일수
	CString	sBdjs;			// 배당지수
	int	m_noCont[2];		// 미체결수량 : 0매도,1매수

	class COpSensCal	*m_pOpSensCal;	// 변동성 계산(델타,감마)
	double	m_dBase;
	
};