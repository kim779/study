#pragma once

class CMapWnd;
class CRemainFuture : public CWnd
{
public:
	CRemainFuture();
	CRemainFuture(CWnd* pMainWnd);
	virtual ~CRemainFuture();

	//{{AFX_VIRTUAL(CRemainFuture)
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CRemainFuture)
	afx_msg void OnDestroy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
	CMapWnd*	m_pParent;

	CString	m_code,			// 종목코드
		m_name,			// 종목명
		m_gubn,			// 매매구분
		m_possible,		// 주문가능수량
		m_rprice,		// 평균단가
		m_curr,			// 현재가
		m_rdiff,		// 평균가대비
		m_pprice,		// 평가금액
		m_pgsonik,		// 평가손익
		m_suik,			// 수익률
		m_remian,		// 잔고수량
		m_maip,			// 매입금액
		m_diff,			// 전일대비
		m_srate;		// 등락률(시세)
	double	m_dBase;
	CMapStringToPtr	m_NoContMap;
	char	m_system;

public:
	CString TotalData();
	void	ParsingRemainData(char *pData);
	void	ParsingAllaccnData(CString sData);
	int	CalRemainData(CMapStringToString &symbolMap, CString keyS, bool bNewCode);
	void	CalPgsonik();
	bool	CalPgsonik(CString sNCurr, CString sNDiff, CString sNRate);
	void	SetNoCont(CString sMemeGubn, int nAmount);
	double	GetBaseValue(CString code);			//주식선물의 경우, 승수를 codefile에서 읽어옴
private:			
	int	getNoCont(CString sMemeGubn);
	void	parsingNcont(CString sMemeGubn, CString sStatus, int nAmount);
	void	getBase();
	CString	getFormatData(CString src);
	int	getJKind(CString m_code);
	CString parser(CString &srcstr, CString substr);
	CString	getString(char *pChar, int nMinLen);
public:
	void	testSaveFile(CString strText, bool bServer = false);
};
